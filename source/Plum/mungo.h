#ifndef PLUM_MUNGO_H
#define PLUM_MUNGO_H

namespace Plum
{
	enum MungoState
	{
		MungoStateNormal,
		MungoStateComment,
		MungoStateQuote,
		MungoStateListDelimiterGather,
	};

	typedef FormatParseException MungoParseException;
	typedef FormatTable MungoTable;
	typedef std::map<std::string, MungoTable*> MungoDelimiterLookupTable;
	class MungoParser
	{	
		private:
			inline static bool CharacterIsListDelimiter(char c)
			{
				return c == '*' || c == '-' || c == '+' || c == '|' || c == '~' || c == '>' || c == '=' || c == '#';
			}

			inline static bool CharacterIsMappingDelimiter(char c)
			{
				return c == ':';
			}

			inline static bool CharacterIsQuote(char c)
			{
				return c == '\'' || c == '\"' || c == '`';
			}

			inline static bool CharacterIsCommentStart(char c)
			{
				return c == '[';
			}

			inline static bool CharacterIsCommentEnd(char c)
			{
				return c == ']';
			}

			inline static bool CharacterIsWhitespace(char c)
			{
				return c == ' ' || c == '\t' || c == '\n' || c == '\r';
			}

			int lineIndex;
			MungoState state;
			std::string key;
			std::string value;
			std::string delim;
			char quote;
			bool attachDelimitersToValue;
			bool mapping;

			MungoDelimiterLookupTable activeDelimiterLookup;
			std::string delimiterCurrent;
			MungoTable* listCurrent;
			MungoTable* mainTable;

			int startingLineOffset;
		public:
			MungoParser(int startingLineOffset);
			~MungoParser();

			MungoTable* parseFile(FILE* f);
		private:
			void addStringEntry();
			void handleListDelimiter();
	};

	FormatTable* MungoParseFile(FILE* f);
}

#endif