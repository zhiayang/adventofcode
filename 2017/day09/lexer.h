// lexer.h
// Copyright (c) 2014 - 2017, zhiayang@gmail.com
// Licensed under the Apache License Version 2.0.


#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string>


template <typename T>
class FastVector
{
	public:
	FastVector()
	{
		this->array = 0;
		this->length = 0;
		this->capacity = 0;
	}
	FastVector(size_t initSize)
	{
		this->array = (T*) malloc(initSize * sizeof(T));
		this->capacity = initSize;
		this->length = 0;
	}

	FastVector(const FastVector& other)
	{
		this->array = (T*) malloc(other.capacity * sizeof(T));
		memmove(this->array, other.array, other.length * sizeof(T));

		this->capacity = other.capacity;
		this->length = other.length;
	}

	FastVector& operator = (const FastVector& other)
	{
		this->array = (T*) malloc(other.capacity * sizeof(T));
		memmove(this->array, other.array, other.length * sizeof(T));

		this->capacity = other.capacity;
		this->length = other.length;

		return *this;
	}

	FastVector(FastVector&& other)
	{
		// move.
		this->array = other.array;
		this->length = other.length;
		this->capacity = other.capacity;

		other.array = 0;
		other.length = 0;
		other.capacity = 0;
	}

	FastVector& operator = (FastVector&& other)
	{
		if(this != &other)
		{
			if(this->array)
				free(this->array);

			// move.
			this->array = other.array;
			this->length = other.length;
			this->capacity = other.capacity;

			other.array = 0;
			other.length = 0;
			other.capacity = 0;
		}

		return *this;
	}

	~FastVector()
	{
		if(this->array != 0)
			free(this->array);
	}

	size_t size() const
	{
		return this->length;
	}

	T& operator[] (size_t index) const
	{
		return this->array[index];
	}

	T* getEmptySlotPtrAndAppend()
	{
		this->autoResize();

		this->length++;
		return &this->array[this->length - 1];
	}

	void autoResize()
	{
		if(this->length == this->capacity)
		{
			if(this->capacity == 0)
				this->capacity = 64;

			this->array = (T*) realloc(this->array, this->capacity * 2 * sizeof(T));

			this->capacity *= 2;
		}
	}

	private:
	T* array = 0;
	size_t capacity;
	size_t length;
};


struct Location
{
	size_t line;
	size_t fileID;
	size_t col;
	size_t len;
};


enum class TokenType
{
	Invalid,				// 0
	LBrace,					// 50
	RBrace,					// 51
	LParen,					// 52
	RParen,					// 53
	LSquare,				// 54
	RSquare,				// 55
	LAngle,					// 56
	RAngle,					// 57
	Plus,					// 58
	Minus,					// 59
	Asterisk,				// 60
	Divide,					// 61
	SQuote,					// 62
	DQuote,					// 63
	Period,					// 64
	Comma,					// 65
	Colon,					// 66
	Equal,					// 67
	Question,				// 68
	Exclamation,			// 69
	Semicolon,				// 70
	Ampersand,				// 71
	Percent,				// 72
	Pipe,					// 73
	LogicalOr,				// 74
	LogicalAnd,				// 75
	At,						// 76
	Pound,					// 77
	Tilde,					// 78
	Caret,					// 79
	Underscore,				// 80
	Arrow,					// 81
	EqualsTo,				// 82
	NotEquals,				// 83
	GreaterEquals,			// 84
	LessThanEquals,			// 85
	ShiftLeft,				// 86
	ShiftRight,				// 87
	DoublePlus,				// 88
	DoubleMinus,			// 89
	PlusEq,					// 90
	MinusEq,				// 91
	MultiplyEq,				// 92
	DivideEq,				// 93
	ModEq,					// 94
	ShiftLeftEq,			// 95
	ShiftRightEq,			// 96
	AmpersandEq,			// 97
	PipeEq,					// 98
	CaretEq,				// 99
	Ellipsis,				// 100
	HalfOpenEllipsis,		// 101
	DoubleColon,			// 102
	Identifier,				// 103
	UnicodeSymbol,			// 104
	Number,					// 105
	StringLiteral,			// 106
	NewLine,				// 107
	Comment,				// 108
	EndOfFile,				// 109

	Attr_Raw,				// 110
	Attr_EntryFn,			// 111
	Attr_NoMangle,			// 112
};


struct Token
{
	Location loc;
	TokenType type = TokenType::Invalid;
	std::string_view text;

	operator TokenType() const { return this->type; }
	std::string str() const { return std::string(this->text.data(), this->text.size()); }
};



FastVector<Token> getTokens(std::string fullPath);



