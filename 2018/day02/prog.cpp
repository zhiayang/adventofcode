// prog.cpp
// Copyright (c) 2017, zhiayang@gmail.com
// Licensed under the Apache License Version 2.0.

#include <set>
#include <string>
#include <vector>
#include <unordered_set>

#include "tinyformat.h"

int main()
{
	std::vector<std::string> input = {
		"bazvmqthjtrnlosiecxyghkwud",
		"pazvmqbijirzlosiecxyghkwud",
		"pazvtqbmjtrnlosiecxyghkwzd",
		"pazvmqbfjtrjlosnlcxyghkwud",
		"pazvkqbfjtrtlosiecjyghkwud",
		"paztmqbfjtrnbosiecxyglkwud",
		"pazvmqbfjtunlosievxmghkwud",
		"pazvmqbfjtmngosiecyyghkwud",
		"jazvmqbfjtrnlosiecxygikpud",
		"pazvqqbfctrnlosimcxyghkwud",
		"pazvmqbfjtrnwogiecxyihkwud",
		"pazvmqbfjtrqlojiecxeghkwud",
		"payvmqbfjtrzlosiecxyghkwuk",
		"pkzvmqnfjtrnlosieciyghkwud",
		"pazvmqqfjtrnldsiecxyghkwui",
		"pazvmqbfttrqlosiecxywhkwud",
		"gazvmybfjthnlosiecxyghkwud",
		"pazvmqbfjtrnlasiecxygptwud",
		"pktvmqbfjtrnwosiecxyghkwud",
		"pazvmqwfjtrnlosiecxgghkkud",
		"pazvmzkbjtrnlosiecxyghkwud",
		"pazvmqbfjtrnloslecxyghuwui",
		"pezvmqbfjtrnlesieyxyghkwud",
		"cazvmqbfjrrnlosiecxyghkmud",
		"pazvmqrfjjrnlosiecxyghkwnd",
		"pazvmqbgjtrnlosiecxyphtwud",
		"pazvmqbvmtrnlosiecxyghkpud",
		"pazdmqbfjtrnlosiecxyuhkpud",
		"pazvmqbflnrnloshecxyghkwud",
		"pazvvqbfjprilosiecxyghkwud",
		"pazvwqbfjtrllosiecxyghknud",
		"pazvmqbfjtrnloniecxdghkaud",
		"pazvmqbfjtrnlvsuecxynhkwud",
		"ptzvmqwfjtrnlosieccyghkwud",
		"pkzvmqbjjtrnlosiecryghkwud",
		"pazvmqqfjtrexosiecxyghkwud",
		"pazgmqbfjtrneoyiecxyghkwud",
		"paznmqbfjtrnlosiecxydhkwzd",
		"pazvmqbfjtrnaosiwcxsghkwud",
		"pazomqbfjxrnlosiewxyghkwud",
		"pazsmqbfjprnlosiecxrghkwud",
		"pazvmqbfqtrnoosiecxygmkwud",
		"aazvmqbfjtrnlosiacxyghjwud",
		"pazviqbfjtrnlobiecxygrkwud",
		"qazwmqbfjhrnlosiecxyghkwud",
		"pazvmqbfftrnlosiqcxygfkwud",
		"patvmqbfjtonlosircxyghkwud",
		"pazvmqbfjtrnlomaecxyghkpud",
		"paztmqbfjtrulossecxyghkwud",
		"pazvmqbijtrnlobiecxyghkwkd",
		"pazvsqbfjtrnlospecxyghkqud",
		"pbzmmqbfjtrnlosiecxyghkwhd",
		"pezvkqbfjtenlosiecxyghkwud",
		"razvmqbfjkrnlosiecxeghkwud",
		"pazcmqbfjtrnloriecxyghkgud",
		"pazvmqbfftfnlosiecvyghkwud",
		"pazvmqpujtrnlosiepxyghkwud",
		"patvgqbfjtrnloslecxyghkwud",
		"pazvmqbfltrnlosibcxyghswud",
		"pazvmebfjtrnlosaecxyehkwud",
		"pazdmqbejtrnlosiecxyghrwud",
		"pazvmcbfntrplosiecxyghkwud",
		"pszvmqbfjtrnlosivcfyghkwud",
		"puzvmqbfjtrnloeiecxyxhkwud",
		"pazvmqbfjtrivooiecxyghkwud",
		"pazvyqbfjtrngosiwcxyghkwud",
		"pauvmqbfjtrnlosimexyghkwud",
		"pazvmqbfjtrnwoshecxeghkwud",
		"dazvmqbfjtrnloshecxygxkwud",
		"pazvmqbfjtrtdosiecxyghvwud",
		"pazxmqbfjtrnlosieceyghjwud",
		"pazvmqbfjtrnlosihexjghkwud",
		"pazvmqbfjsrnlosiecxughiwud",
		"phzvcqbfjtrqlosiecxyghkwud",
		"pazvmibfjtrnlosjecxxghkwud",
		"pazvmqbfjtrbeosiecxlghkwud",
		"pazvmqyfjttolosiecxyghkwud",
		"fawvmqbfjtrnlosiecxyghkwhd",
		"pazvmqbfjprnxosiecxyghkbud",
		"macvmqbfjtrnlosiesxyghkwud",
		"pazsmqbfjtrflouiecxyghkwud",
		"pacvmqbfjtrnltsiecxyghcwud",
		"pazvmqbfjtymlosiecxygykwud",
		"pazvmqbfjtrclosiecxygukwmd",
		"pazvmqbfjtrnlobiecxphhkwud",
		"mazvmqbhitrnlosiecxyghkwud",
		"pazvmqdtjtrnlrsiecxyghkwud",
		"pazvmqbfjgrnllsieczyghkwud",
		"pazvmqbfjtrilosiecxxgikwud",
		"pazvmqbjjtrnlosreceyghkwud",
		"paxvmmbfjtrilosiecxyghkwud",
		"pazqmwbfjtrnlowiecxyghkwud",
		"pazvmqbfjfrnqosiecxyghkwui",
		"pazvmqbfjtrrgosiecxyghswud",
		"pazvmqnfjtrnlosiecsyghkwmd",
		"paiemqbmjtrnlosiecxyghkwud",
		"pazvmqbfdtqnlosiecxyjhkwud",
		"pazvmxbfjthndosiecxyghkwud",
		"pqzvmqbfjtrnlosiecxbghkzud",
		"pagrmqbfjtrnlosiecxygskwud",
		"pazamqtfjtrnsosiecxyghkwud",
		"pazvmqbfjtrnldshecxyzhkwud",
		"pazvmnbfjtrllosieclyghkwud",
		"snzvmqbfjnrnlosiecxyghkwud",
		"pazvsqbfjdrnlosiecxyghswud",
		"pazvmqnfjfrnlosiecsyghkwud",
		"pazvmqbfjtrnlosiecxjghowum",
		"pazvmqbfjtjnlosieczygfkwud",
		"pazvmqbsjtrnloziecxyghkeud",
		"pazvxqbgjtrnlooiecxyghkwud",
		"pazvmqbfjtrnlooiecxmyhkwud",
		"pazvmqbyftrnlosgecxyghkwud",
		"pazvmqbfjtrnlosiwcxyqhksud",
		"pazvmqkyjtrnlokiecxyghkwud",
		"pazfmqbfjtrnlosijcxyohkwud",
		"pazvmqbfjtrnlociecxygikcud",
		"fazvmqbfjtrnlosiecxyjhkuud",
		"pazvmqbojtknlohiecxyghkwud",
		"pazvmqbfjtgnlosbecxyghkwux",
		"pazvmqbfjtrnlocieckoghkwud",
		"pazvdqbfjtrlltsiecxyghkwud",
		"pazvmqbfjtsnlfsiecxyglkwud",
		"przvpqbfjtrnyosiecxyghkwud",
		"pazvmbrfjtrnlosiecxmghkwud",
		"dazvmqbfttrnlostecxyghkwud",
		"pazvmqbfttdnlosiecxygwkwud",
		"pazvmqbvitrnlosieexyghkwud",
		"pazvmqbfjhrnlosjecxyvhkwud",
		"pazvmqbfstrnlosiecxyggkwpd",
		"bazvmqbfjtrnlmsiecxyohkwud",
		"patmmqbfjtrnlosizcxyghkwud",
		"pazvmqbfwtrglosieqxyghkwud",
		"pazvmqbfjtrnlosiecxdhhkwmd",
		"pazvmqbfjdrnlosnexxyghkwud",
		"oazrrqbfjtrnlosiecxyghkwud",
		"pazvmqbfjcrnlosiecxygakwjd",
		"pazvmqbfjtrnlosifcxfghkwyd",
		"pazvmnbfjtrnlosiecxyahzwud",
		"pazvmqbfgtrnlojiecxyghkgud",
		"pazvmqbfjtrnlaliecxyghkwuy",
		"pazvmqbfjtrnlfsiecrtghkwud",
		"pazvmqbkjtrnloswecxdghkwud",
		"pazvtqbfjtdnlosiecxyghkwuu",
		"pozvmqbfrtrnlosiesxyghkwud",
		"payvmqbfjornlossecxyghkwud",
		"pazvuqbfjtrnlosiscxyghkpud",
		"pgzcmqbfjtrnlotiecxyghkwud",
		"pazvvqbfjtrnlobieyxyghkwud",
		"pazycqbfjtrnlosiecxyzhkwud",
		"pizvdqbfjtrnlosiecxbghkwud",
		"pazvmqbfjtrnloqiecxmgtkwud",
		"gazvmqbfjtrnlusiecxpghkwud",
		"pazvmqdfjtralosiecxyghkwmd",
		"pazvmqbfjtmnlosiecxywhawud",
		"pazvlqbfjtrnlosqecxyghqwud",
		"pazvmqbfjtrnlhsneixyghkwud",
		"kazvmqbfjtrqlosimcxyghkwud",
		"pazvmwbfjtrclosiecxyghkuud",
		"pazvmqjfjtrnlosieckyghpwud",
		"pezvmqbgjtrnloseecxyghkwud",
		"pazvqqbfjtfnlosvecxyghkwud",
		"oazvmqbfjtunlosiecxyghkwad",
		"pazvmqbfjtrncoswecxyghfwud",
		"pazvyqbfjtrnlosqecxygtkwud",
		"pazvmqbfjtrvlzsiecxygwkwud",
		"pazvmqbfjjrnlosiekxylhkwud",
		"madvmqbfjtrnlosircxyghkwud",
		"pazvmybfjtrnlisiecxyghkwbd",
		"pazvmqbjjixnlosiecxyghkwud",
		"pazvmqefjtrnloqiecxyghhwud",
		"pazveqbfjtrnlosiecgygzkwud",
		"pazvmqbfjtrxlosiecxmgwkwud",
		"uazvmqufjtrnlosiecxyghkwuo",
		"pasymqbfjtrnlosiecxyghowud",
		"pazvmqbfjtlnlpsiecxyghswud",
		"pnzvmqbfjprnloszecxyghkwud",
		"pafjmqcfjtrnlosiecxyghkwud",
		"pazvmqxfbtrnloqiecxyghkwud",
		"pazvmzbfjtrnposiccxyghkwud",
		"pazvmqbfjotulosiecxyghkwud",
		"pazvmqbfotrnlosgecxykhkwud",
		"przvmqbfjtrnlosiecxyqhkwcd",
		"pazvmqbfjtsnlogiecxyyhkwud",
		"pazvmqbfrtrnlzsiecxyghkwug",
		"pazvmqbfjtrnlosiecxzgukwuo",
		"pqzvmqbqjtrnlosdecxyghkwud",
		"pazvmqbfjtqqlosiecxughkwud",
		"pazvmqbfjtrnlosiedhyphkwud",
		"pazsmqbcutrnlosiecxyghkwud",
		"pazvmqbgrtrnlosiecxyghpwud",
		"pazemqbfjtznlosiecxyghkvud",
		"pazvkqbfjtrilosiecxyghkwod",
		"pfzvmqbfjtrnlopiecxygjkwud",
		"pazvmqvfjtreloniecxyghkwud",
		"pazvmqbfjernljsiecxgghkwud",
		"pazvmqikjtrnlosiecxyghqwud",
		"pazvmqbfjtrnpesoecxyghkwud",
		"fazvmqbfjtrnlosihchyghkwud",
		"pazvmqbfjtgnloanecxyghkwud",
		"pazvmqsfjqrnlosiecxychkwud",
		"parvmqbfjtrnlosiecxygfuwud",
		"przvmqbfjtrhlosihcxyghkwud",
		"pazvmqbcjtrnlosimcxgghkwud",
		"pazvmqbfjtrnlosceciyjhkwud",
		"pazvkqbfjtrylosivcxyghkwud",
		"pazvmqbfjtrnlgsieoxyghdwud",
		"pazvmqnfstrnlowiecxyghkwud",
		"pazvmqbfdtrnlosieumyghkwud",
		"pazvmqbfjtrnlosyecxfghkwul",
		"pazvmqbfjtrclosivcxyghkcud",
		"pazjmqbfjtrnlosiecxygokwkd",
		"hazvmqbfjtrflosiecxzghkwud",
		"wazvmqbfjtrnlomiecxyphkwud",
		"yazvmqbfjirnkosiecxyghkwud",
		"pczvmqbfjtrnlohiecxyghkwpd",
		"pazvmqbfotrbeosiecxlghkwud",
		"pazvmqbfjtrplosiecxynhzwud",
		"paxvbqbwjtrnlosiecxyghkwud",
		"pazvmqvfjtrnlosiecbyghqwud",
		"pazjmqbfjtrnlosiecxoghkwed",
		"pazvmqbfjtreljsitcxyghkwud",
		"mazamqbfjtrnlosiecxoghkwud",
		"pazvmqbfjjrnposiscxyghkwud",
		"pbrvmqbfjtrnloliecxyghkwud",
		"pazvmqbfjtrnlosiecxgghkyyd",
		"pmzvmqbfntrnlosiecxyghkwuw",
		"pazvzqbfjtrnlosienxyghzwud",
		"pazvmqifjtvnlosrecxyghkwud",
		"tazvmqbhjtjnlosiecxyghkwud",
		"pazvmqbfjtlnxosiecxyghkwuo",
		"pazvmqbfjennlosiecxyghkwxd",
		"pahvmqbfjhrnlosiecxythkwud",
		"pazvmlkfjtrnlxsiecxyghkwud",
		"pfzvmqbojtrnlosieciyghkwud",
		"pazvbqbfjtrollsiecxyghkwud",
		"eazvmqbfjtrnlosiecayghkoud",
		"pazvmqbfjtjnlvsiecxyghkwsd",
		"pazvoqbojtrnlosiecfyghkwud",
		"pazvmqbfjtuslosiecxyghksud",
		"pazvmqbfjnrnlosiedxyghkwup",
		"pazvmqbjjtrnlosieaxyghdwud",
		"pazccqbfjtrhlosiecxyghkwud",
		"pbzvmqkfjtrnlosievxyghkwud",
		"pazvmqrljtrnlosiscxyghkwud",
		"pazvmqbfjfoqlosiecxyghkwud",
		"pazcmqbfjtrnlosiecxyihkwuf",
		"pszvmqbfjtrnnosiacxyghkwud",
		"aazvmqbfjtrnlosieyxyghkwld",
		"pazvrqbfntrnlosiycxyghkwud",
		"pkzvoqbfjtrnlosiecxyghxwud",
	};

	size_t twos = 0;
	size_t threes = 0;

	for(const auto& s : input)
	{
		// loop through each letter, i guess.
		bool did2 = false;
		bool did3 = false;
		for(size_t i = 0; i < s.size(); i++)
		{
			auto occurs = std::count(s.begin(), s.end(), s[i]);
			if(occurs == 2 && !did2) twos++, did2 = true;
			if(occurs == 3 && !did3) threes++, did3 = true;
		}
	}

	tfm::printfln("part 1: check = %d", twos * threes);


	// part 2

	for(size_t i = 0; i < input.size(); i++)
	{
		for(size_t j = 0; j < input.size(); j++)
		{
			if(i == j) continue;

			// loop through each string...
			auto a = input[i];
			auto b = input[j];

			bool fail = false;
			bool found = false;
			for(size_t k = 0; k < a.size(); k++)
			{
				if(a[k] != b[k])
				{
					if(!found)  found = true;
					else        fail = true;
				}

				if(fail) break;
			}

			if(found && !fail)
			{
				tfm::printfln("part 2: %s\n%s", a, b);
			}
		}
	}
}























