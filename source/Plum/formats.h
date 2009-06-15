#ifndef PLUM_FORMATS_H
#define PLUM_FORMATS_H

namespace Plum
{
	enum FormatNodeType
	{
		FormatNodeNilType,
		FormatNodeStringType,
		FormatNodeTableType
	};

	struct FormatNode
	{
		FormatNodeType type;
		void* data;
	};

	class FormatTable
	{
		public:
			std::map<std::string, FormatNode*> items;

			FormatTable();
			~FormatTable();

			void clear();
			void print(int tabulation = 0);

			void putNode(std::string key, FormatNode* value);
			void putString(std::string key, std::string value);
			void putTable(std::string key, FormatTable* value);

			FormatNode* getNode(std::string key);
			std::string getString(std::string key);
			FormatTable* getTable(std::string key);
			FormatNodeType getType(std::string key);
			bool hasNode(std::string key);

	};

	typedef FormatTable*(*FormatParseCallback)(FILE* f);
	struct _FormatInternalParserInfo
	{
		const char* name;
		FormatParseCallback callback;
	};

	class FormatParseException : public std::exception
	{
		public:
			FormatParseException(const std::string& message)
				: msg(message)
			{
			}

			virtual const char* what() const throw ()
			{
				return msg.c_str();
			}

			virtual ~FormatParseException() throw ()
			{
			}

		private:
			std::string msg;
	};

	FormatTable* ReadFormat(FILE* f, std::string filename, std::string formatExpected, unsigned int versionCurrent);
}

// The various parser implementations are done here.
#include "mungo.h"

#endif