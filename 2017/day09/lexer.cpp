// lexer.cpp
// Copyright (c) 2014 - 2015, zhiayang@gmail.com
// Licensed under the Apache License Version 2.0.

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include <string>
#include <string_view>


#include "lexer.h"

#define TAB_WIDTH 4


namespace lexer
{
	static void skipWhitespace(std::string_view& line, Location& pos, size_t* offset)
	{
		size_t skip = 0;
		while(line.length() > skip && (line[skip] == '\t' || line[skip] == ' '))
		{
			(line[skip] == ' ' ? pos.col++ : pos.col += TAB_WIDTH);
			skip++;
		}

		line.remove_prefix(skip);
		(*offset) += skip;
	}

	template <size_t N>
	static bool hasPrefix(const std::string_view& str, char const (&literal)[N])
	{
		if(str.length() < N - 1) return false;
		for(size_t i = 0; i < N - 1; i++)
			if(str[i] != literal[i]) return false;

		return true;
	}

	template <size_t N>
	static bool compare(const std::string_view& str, char const (&literal)[N])
	{
		if(str.length() != N - 1) return false;
		for(size_t i = 0; i < N - 1; i++)
			if(str[i] != literal[i]) return false;

		return true;
	}



	static TokenType prevType = TokenType::Invalid;
	static size_t prevID = 0;
	static bool shouldConsiderUnaryLiteral(std::string_view& stream, Location& pos)
	{
		// check the previous token
		bool res = (prevType != TokenType::Invalid && prevID == pos.fileID && (prevType != TokenType::RParen && prevType != TokenType::RSquare
			&& prevType != TokenType::Identifier && prevType != TokenType::Number && prevType != TokenType::StringLiteral));

		if(!res) return false;

		// check if the current char is a + or -
		if(stream.length() == 0) return false;
		if(stream[0] != '+' && stream[0] != '-') return false;

		// check if there's only spaces between this and the number itself
		for(size_t i = 1; i < stream.length(); i++)
		{
			if(isdigit(stream[i])) return true;
			else if(stream[i] != ' ') return false;
		}

		return false;
	}


	TokenType getNextToken(const FastVector<std::string_view>& lines, size_t* line, size_t* offset, const std::string_view& whole,
		Location& pos, Token* out, bool crlf)
	{
		bool flag = true;

		if(*line == lines.size())
		{
			out->loc = pos;
			out->type = TokenType::EndOfFile;
			return TokenType::EndOfFile;
		}

		std::string_view stream = lines[*line].substr(*offset);
		if(stream.empty())
		{
			out->loc = pos;
			out->type = TokenType::EndOfFile;
			return TokenType::EndOfFile;
		}



		size_t read = 0;
		size_t unicodeLength = 0;

		// first eat all whitespace
		skipWhitespace(stream, pos, offset);

		Token& tok = *out;
		tok.loc = pos;
		tok.type = TokenType::Invalid;


		// check compound symbols first.
		if(hasPrefix(stream, "//"))
		{
			tok.type = TokenType::Comment;
			// stream = stream.substr(0, 0);
			(*line)++;
			pos.line++;
			pos.col = 0;


			(*offset) = 0;

			// don't assign lines[line] = stream, since over here we've changed 'line' to be the next one.
			flag = false;
			tok.text = "";
		}
		else if(hasPrefix(stream, "=="))
		{
			tok.type = TokenType::EqualsTo;
			tok.text = "==";
			read = 2;
		}
		else if(hasPrefix(stream, ">="))
		{
			tok.type = TokenType::GreaterEquals;
			tok.text = ">=";
			read = 2;
		}
		else if(hasPrefix(stream, "<="))
		{
			tok.type = TokenType::LessThanEquals;
			tok.text = "<=";
			read = 2;
		}
		else if(hasPrefix(stream, "!="))
		{
			tok.type = TokenType::NotEquals;
			tok.text = "!=";
			read = 2;
		}
		else if(hasPrefix(stream, "||"))
		{
			tok.type = TokenType::LogicalOr;
			tok.text = "||";
			read = 2;
		}
		else if(hasPrefix(stream, "&&"))
		{
			tok.type = TokenType::LogicalAnd;
			tok.text = "&&";
			read = 2;
		}
		else if(hasPrefix(stream, "->"))
		{
			tok.type = TokenType::Arrow;
			tok.text = "->";
			read = 2;
		}
		else if(hasPrefix(stream, "++"))
		{
			tok.type = TokenType::DoublePlus;
			tok.text = "++";
			read = 2;
		}
		else if(hasPrefix(stream, "--"))
		{
			tok.type = TokenType::DoubleMinus;
			tok.text = "--";
			read = 2;
		}
		else if(hasPrefix(stream, "+="))
		{
			tok.type = TokenType::PlusEq;
			tok.text = "+=";
			read = 2;
		}
		else if(hasPrefix(stream, "-="))
		{
			tok.type = TokenType::MinusEq;
			tok.text = "-=";
			read = 2;
		}
		else if(hasPrefix(stream, "*="))
		{
			tok.type = TokenType::MultiplyEq;
			tok.text = "*=";
			read = 2;
		}
		else if(hasPrefix(stream, "/="))
		{
			tok.type = TokenType::DivideEq;
			tok.text = "/=";
			read = 2;
		}
		else if(hasPrefix(stream, "%="))
		{
			tok.type = TokenType::ModEq;
			tok.text = "%=";
			read = 2;
		}
		else if(hasPrefix(stream, "&="))
		{
			tok.type = TokenType::AmpersandEq;
			tok.text = "&=";
			read = 2;
		}
		else if(hasPrefix(stream, "|="))
		{
			tok.type = TokenType::PipeEq;
			tok.text = "|=";
			read = 2;
		}
		else if(hasPrefix(stream, "^="))
		{
			tok.type = TokenType::CaretEq;
			tok.text = "^=";
			read = 2;
		}
		else if(hasPrefix(stream, "..."))
		{
			tok.type = TokenType::Ellipsis;
			tok.text = "...";
			read = 3;
		}
		else if(hasPrefix(stream, "..<"))
		{
			tok.type = TokenType::HalfOpenEllipsis;
			tok.text = "..<";
			read = 3;
		}

		// unicode stuff
		else if(hasPrefix(stream, "÷"))
		{
			tok.type = TokenType::Divide;
			read = std::string("÷").length();
			tok.text = "÷";

			unicodeLength = 1;
		}
		else if(hasPrefix(stream, "≠"))
		{
			tok.type = TokenType::NotEquals;
			read = std::string("≠").length();
			tok.text = "≠";

			unicodeLength = 1;
		}
		else if(hasPrefix(stream, "≤"))
		{
			tok.type = TokenType::LessThanEquals;
			read = std::string("≤").length();
			tok.text = "≤";

			unicodeLength = 1;
		}
		else if(hasPrefix(stream, "≥"))
		{
			tok.type = TokenType::GreaterEquals;
			read = std::string("≥").length();
			tok.text = "≥";

			unicodeLength = 1;
		}

		// note some special-casing is needed to differentiate between unary +/- and binary +/-
		// cases where we want binary:
		// ...) + 3
		// ...] + 3
		// ident + 3
		// number + 3
		// string + 3
		// so in every other case we want unary +/-.
		// note: this dumb '<=255' thing is because windows likes to assert useless things.
		else if(!stream.empty() && ((stream[0] >= 1 && (int) stream[0] <= 255 && isdigit(stream[0])) || shouldConsiderUnaryLiteral(stream, pos)))
		{
			// copy it.
			auto tmp = stream;

			if(stream.find('-') == 0 || stream.find('+') == 0)
				tmp.remove_prefix(1);

			int base = 10;
			if(tmp.find("0x") == 0 || tmp.find("0X") == 0)
				base = 16, tmp.remove_prefix(2);

			else if(tmp.find("0b") == 0 || tmp.find("0B") == 0)
				base = 2, tmp.remove_prefix(2);


			// find that shit
			auto end = std::find_if_not(tmp.begin(), tmp.end(), [base](const char& c) -> bool {
				if(base == 10)	return isdigit(c);
				if(base == 16)	return isdigit(c) || (toupper(c) >= 'A' && toupper(c) <= 'F');
				else			return (c == '0' || c == '1');
			});

			tmp.remove_prefix((end - tmp.begin()));

			// check if we have 'e' or 'E'
			bool hadExp = false;
			if(tmp.size() > 0 && (tmp[0] == 'e' || tmp[0] == 'E'))
			{
				if(base != 10)
					abort();

				// find that shit
				auto next = std::find_if_not(tmp.begin() + 1, tmp.end(), isdigit);

				// this does the 'e' as well.
				tmp.remove_prefix(next - tmp.begin());

				hadExp = true;
			}

			size_t didRead = stream.size() - tmp.size();
			auto post = stream.substr(didRead);

			if(!post.empty() && post[0] == '.')
			{
				if(base != 10)
					abort();

				else if(hadExp)
					abort();

				// if the previous token was a '.' as well, then we're doing some tuple access
				// eg. x.0.1 (we would be at '0', having a period both ahead and behind us)

				// if the next token is not a number, then same thing, eg.
				// x.0.z, where the first tuple element of 'x' is a struct or something.

				// so -- lex a floating point *iff* the previous token was not '.', and the next token is a digit.
				if(prevType != TokenType::Period && post.size() > 1 && isdigit(post[1]))
				{
					// yes, parse a floating point
					post.remove_prefix(1), didRead++;

					while(isdigit(post.front()))
						post.remove_prefix(1), didRead++;

					// ok.
				}
				else
				{
					// no, just return the integer token.
					// (which we do below, so just do nothing here)
				}
			}

			tok.text = stream.substr(0, didRead);

			tok.type = TokenType::Number;
			tok.loc.len = didRead;

			read = didRead;
		}
		else if(!stream.empty() && (isalpha(stream[0]) || stream[0] == '_'))
		{
			// get as many letters as possible first

			size_t identLength = 0;

			for(auto c : stream)
			{
				if(c == '_' || isdigit(c) || isalpha(c))
					identLength++;

				else
					break;
			}

			read = identLength;
			tok.text = stream.substr(0, identLength);
			tok.type = TokenType::Identifier;
		}
		else if(!stream.empty() && stream[0] == '"')
		{
			// string literal
			// because we want to avoid using std::string (ie. copying) in the lexer (Token), we must send the string over verbatim.

			// store the starting position
			size_t start = (size_t) (stream.data() - whole.data() + 1);

			// opening "
			pos.col++;

			size_t i = 1;
			size_t didRead = 0;
			for(; stream[i] != '"'; i++)
			{
				if(stream[i] == '\\')
				{
					if(i + 1 == stream.size())
					{
						abort();
					}
					else if(stream[i + 1] == '"')
					{
						// add the quote and the backslash, and skip it.
						didRead += 2;
						pos.col += 2;
						i++;
					}
					// breaking string over two lines
					else if(stream[i + 1] == '\n')
					{
						// skip it, then move to the next line
						pos.line++;
						pos.col = 1;
						(*line)++;

						if(*line == lines.size())
							abort();

						i = 0;

						// just a fudge factor gotten from empirical evidence
						// 3 extra holds for multiple lines, so all is well.

						didRead += 3;
						stream = lines[*line];
						(*offset) = 0;
					}
					else
					{
						// just put the backslash in.
						// and don't skip the next one.
						didRead++;
						pos.col++;
					}

					continue;
				}

				didRead++;
				pos.col++;

				if(i == stream.size() - 1 || stream[i] == '\n')
					abort();
			}

			// closing "
			pos.col++;


			tok.type = TokenType::StringLiteral;
			tok.text = whole.substr(start, didRead);

			tok.loc.len = 2 + didRead;

			stream = stream.substr(i + 1);
			(*offset) += i + 1;

			read = 0;
			flag = false;
		}
		else if(crlf && hasPrefix(stream, "\r\n"))
		{
			read = 2;
			flag = false;

			tok.type = TokenType::NewLine;
			tok.text = "\n";
		}
		else if(!stream.empty())
		{
			if(isascii(stream[0]))
			{
				// check the first char
				switch(stream[0])
				{
					// for single-char things
					case '\n':	tok.type = TokenType::NewLine;				break;
					case '{':	tok.type = TokenType::LBrace;				break;
					case '}':	tok.type = TokenType::RBrace;				break;
					case '(':	tok.type = TokenType::LParen;				break;
					case ')':	tok.type = TokenType::RParen;				break;
					case '[':	tok.type = TokenType::LSquare;				break;
					case ']':	tok.type = TokenType::RSquare;				break;
					case '<':	tok.type = TokenType::LAngle;				break;
					case '>':	tok.type = TokenType::RAngle;				break;
					case '+':	tok.type = TokenType::Plus;					break;
					case '-':	tok.type = TokenType::Minus;				break;
					case '*':	tok.type = TokenType::Asterisk;				break;
					case '/':	tok.type = TokenType::Divide;				break;
					case '\'':	tok.type = TokenType::SQuote;				break;
					case '.':	tok.type = TokenType::Period;				break;
					case ',':	tok.type = TokenType::Comma;				break;
					case ':':	tok.type = TokenType::Colon;				break;
					case '=':	tok.type = TokenType::Equal;				break;
					case '?':	tok.type = TokenType::Question;				break;
					case '!':	tok.type = TokenType::Exclamation;			break;
					case ';':	tok.type = TokenType::Semicolon;			break;
					case '&':	tok.type = TokenType::Ampersand;			break;
					case '%':	tok.type = TokenType::Percent;				break;
					case '|':	tok.type = TokenType::Pipe;					break;
					case '@':	tok.type = TokenType::At;					break;
					case '#':	tok.type = TokenType::Pound;				break;
					case '~':	tok.type = TokenType::Tilde;				break;
					case '^':	tok.type = TokenType::Caret;				break;

					default:
						abort();
				}

				tok.text = stream.substr(0, 1);
				// tok.loc.col += 1;
				read = 1;
			}
			else
			{
				abort();
			}
		}

		stream.remove_prefix(read);

		if(flag)
			(*offset) += read;

		if(tok.type != TokenType::NewLine)
		{
			if(read > 0)
			{
				// note(debatable): put the actual "position" in the front of the token
				pos.col += read;

				// special handling -- things like ƒ, ≤ etc. are one character wide, but can be several *bytes* long.
				pos.len = (unicodeLength > 0 ? unicodeLength : read);
				tok.loc.len = read;
			}
		}
		else
		{
			pos.col = 1;
			pos.line++;

			(*line)++;
			(*offset) = 0;
		}

		// printf("token %s: %d // %d\n", tok.text.to_string().c_str(), tok.pin.col, pos.col);

		prevType = tok.type;
		prevID = tok.loc.fileID;

		return prevType;
	}
}

#include <sys/stat.h>
size_t getFileSize(const std::string& path)
{
	struct stat st;
	if(stat(path.c_str(), &st) != 0)
		abort();

	return st.st_size;
}

std::string_view readEntireFile(const std::string& path)
{
	// first, get the size of the file
	size_t fileLength = getFileSize(path);

	auto fd = open(path.c_str(), O_RDONLY, 0);
	if(fd == -1)
	{
		perror("There was an error getting opening the file");
		exit(-1);
	}


	// check if we should mmap
	// explanation: if we have EXTRA_MMAP_FLAGS, then we're getting 2MB pages -- in which case we should probably only do it
	// if we have at least 4mb worth of file.
	// if not, then just 2 * pagesize.
	#define MINIMUM_MMAP_THRESHOLD (EXTRA_MMAP_FLAGS ? (2 * 2 * 1024 * 1024) : 2 * getpagesize())
	#define _

	char* contents = 0;

	// here's the thing -- we use USE_MMAP at *compile-time*, because on windows some of the constants we're going to use here aren't available at all
	// if we include it, then it'll be parsed and everything and error out. So, we #ifdef it away.

	// Problem is, there's another scenario in which we won't want to use mmap -- when the file size is too small. so, that's why the stuff
	// below is structured the way it is.
	#if USE_MMAP
	{
		if(fileLength >= MINIMUM_MMAP_THRESHOLD)
		{
			// ok, do an mmap
			contents = (char*) mmap(0, fileLength, PROT_READ, MAP_PRIVATE | EXTRA_MMAP_FLAGS, fd, 0);
			if(contents == MAP_FAILED)
			{
				perror("There was an error reading the file");
				exit(-1);
			}
		}
	}
	#endif

	if(contents == 0)
	{
		// read normally
		contents = new char[fileLength + 1];
		size_t didRead = read(fd, contents, fileLength);
		if(didRead != fileLength)
		{
			perror("There was an error reading the file");
			exit(-1);
		}
	}

	close(fd);

	return std::string_view(contents, fileLength);
}



FastVector<Token> getTokens(std::string fullPath)
{
	std::string_view fileContents = readEntireFile(fullPath);

	// split into lines
	bool crlf = false;
	std::string_view view = fileContents;
	FastVector<std::string_view> rawlines;
	{
		// auto p = prof::Profile("lines");

		bool first = true;
		while(true)
		{
			size_t ln = 0;

			if(first || crlf)
			{
				ln = view.find("\r\n");
				if(ln != std::string_view::npos && first)
					crlf = true;
			}

			if((!first && !crlf) || (first && !crlf && ln == std::string_view::npos))
				ln = view.find('\n');

			first = false;

			if(ln != std::string_view::npos)
			{
				new (rawlines.getEmptySlotPtrAndAppend()) std::string_view(view.data(), ln + (crlf ? 2 : 1));
				view.remove_prefix(ln + (crlf ? 2 : 1));
			}
			else
			{
				break;
			}
		}

		// account for the case when there's no trailing newline, and we still have some stuff stuck in the view.
		if(!view.empty())
			new (rawlines.getEmptySlotPtrAndAppend()) std::string_view(view.data(), view.length());

		// p.finish();
	}


	Location pos;
	// auto p = prof::Profile("lex");

	using TokenList = FastVector<Token>;
	TokenList ts;
	{

		size_t curLine = 0;
		size_t curOffset = 0;

		bool flag = true;
		size_t i = 0;

		do {
			auto type = lexer::getNextToken(rawlines, &curLine, &curOffset, fileContents, pos, ts.getEmptySlotPtrAndAppend(), crlf);

			flag = (type != TokenType::EndOfFile);

			if(type == TokenType::Invalid)
				abort();

			i++;

		} while(flag);
	}

	return ts;

	/*
		file reading stats:

		~175ms reading with c++
		~20ms with read() -- split lines ~70ms
		~4ms with mmap() -- split lines ~87ms


		lexing stats:
		raw lexing takes up ~20ms
		adding to the vector takes ~65ms

		=> resizing ends up taking up 45ms of time
	*/
}







