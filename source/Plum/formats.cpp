#include "plum.h"

namespace Plum
{
	FormatTable::FormatTable()
	{
		items.clear();
	}

	FormatTable::~FormatTable()
	{
		clear();
	}

	void FormatTable::clear()
	{
		std::map<std::string, FormatNode*>::iterator i;
		for(i = items.begin(); i != items.end(); ++i)
		{
			if(i->second->type == FormatNodeStringType)
			{
				free(i->second->data);
			}
			else if(i->second->type == FormatNodeTableType)
			{
				delete (FormatTable*) i->second->data;
			}
			delete i->second;
		}
		items.clear();
	}

	void FormatTable::print(int tabulation)
	{
		bool separator = false;
		std::map<std::string, FormatNode*>::iterator i;
		int j;
		for(j = 0; j < tabulation; j++)
		{
			putc('\t', stdout);
		}
		printf("{\n");
		for(i = items.begin(); i != items.end(); ++i)
		{
			if(separator)
			{
				printf(", \n");
			}
			for(j = 0; j < tabulation + 1; j++)
			{
				putc('\t', stdout);
			}
			printf("'%s': ", i->first.c_str());
			if(i->second->type == FormatNodeStringType)
			{
				printf("'%s'", (char*) i->second->data);
			}
			else if(i->second->type == FormatNodeTableType)
			{
				FormatTable* t = (FormatTable*) i->second->data;
		
				putc('\n', stdout);
				t->print(tabulation + 1);
			}
			separator = true;
		}
		putc('\n', stdout);
		for(j = 0; j < tabulation; j++)
		{
			putc('\t', stdout);
		}
		printf("}");
		if(!tabulation)
		{
			putc('\n', stdout);
		}
	}

	void FormatTable::putNode(std::string key, FormatNode* value)
	{
		std::map<std::string, FormatNode*>::iterator i = items.find(key);
		if(i != items.end())
		{
			if(i->second->type == FormatNodeStringType)
			{
				free(i->second->data);
			}
			else if(i->second->type == FormatNodeTableType)
			{
				delete (FormatTable*) i->second->data;
			}
			delete i->second;
		}

		items[key] = value;
	}

	void FormatTable::putString(std::string key, std::string value)
	{
		char* s = (char*) malloc(sizeof(char) * (value.length() + 1));
		strcpy(s, value.c_str());
		s[value.length()] = '\0';

		FormatNode* node = new FormatNode;
		node->type = FormatNodeStringType;
		node->data = s;

		putNode(key, node);
	}

	void FormatTable::putTable(std::string key, FormatTable* value)
	{
		FormatNode* node = new FormatNode;
		node->type = FormatNodeTableType;
		node->data = value;

		putNode(key, node);
	}

	FormatNode* FormatTable::getNode(std::string key)
	{
		std::map<std::string, FormatNode*>::iterator i = items.find(key);
		if(i != items.end())
		{
			return i->second;
		}
		return NULL;
	}

	std::string FormatTable::getString(std::string key)
	{
		FormatNode* node = getNode(key);
		if(node && node->type == FormatNodeStringType)
		{
			return std::string((char*) node->data);
		}
		return "";
	}

	FormatTable* FormatTable::getTable(std::string key)
	{
		FormatNode* node = getNode(key);
		if(node && node->type == FormatNodeTableType)
		{
			return (FormatTable*) node->data;
		}
		return NULL;
	}

	FormatNodeType FormatTable::getType(std::string key)
	{
		FormatNode* node = getNode(key);
		if(node)
		{
			return node->type;
		}
		return FormatNodeNilType;
	}

	bool FormatTable::hasNode(std::string key)
	{
		return getNode(key) != NULL;
	}

	static const _FormatInternalParserInfo parsers[] = {
		{"mungo", MungoParseFile},
		{NULL, NULL}
	};

	FormatTable* ReadFormat(FILE* f, std::string filename, std::string formatExpected, unsigned int versionCurrent)
	{
		char buffer[128];
		char* piece;
		char headerFormat[33];
		unsigned int headerVersion;
		char headerParserName[33];

		if(!f)
		{
			throw FormatParseException(filename + ": Problem reading file.\n");
		}

		fgets(buffer, 9, f);
		if(strcmp(buffer, "brockoly"))
		{
			throw FormatParseException(filename + ": File is not a valid brockoly format.\n");
		}

		fgets(buffer, 128, f);
		if(!(piece = strtok(buffer, "-")))
		{
			throw FormatParseException(filename + ": Expected a format chunk but the header line is malformed.\n");
		}
		strncpy(headerFormat, piece, 32);

		if(strcmp(headerFormat, formatExpected.c_str()))
		{
			throw FormatParseException(filename + ": Expected a heading of 'brockoly-" + formatExpected + "-...' but got 'brockoly-" + std::string(headerFormat) + "-...' instead.\n");
		}

		if(!(piece = strtok(NULL, "-")) || !sscanf(piece, "%X", &headerVersion))
		{
			throw FormatParseException(filename + ": Expected a version chunk but the header line is malformed.\n");
		}
		if(headerVersion > versionCurrent)
		{
			std::ostringstream oss;
			oss << "The file has a version of ";
			oss << std::setfill('0') << std::setw(8) << std::hex << headerVersion;
			oss << ", which is greater than this framework's supported version of ";
			oss << std::setfill('0') << std::setw(8) << std::hex << versionCurrent;
			oss << "\n";
			throw FormatParseException(filename + ": " + oss.str());
		}


		if(!(piece = strtok(NULL, "-")))
		{
			throw FormatParseException(filename + ": Expected a parser chunk but the header line is malformed.\n");
		}
		strncpy(headerParserName, piece, 32);
		while(headerParserName[strlen(headerParserName) - 1] == '\n' || headerParserName[strlen(headerParserName) - 1] == '\r')
		{
			headerParserName[strlen(headerParserName) - 1] = '\0';
		}
		
		int i = 0;
		FormatParseCallback parserCallback = NULL;
		while(parsers[i].name != NULL)
		{
			if(!strcmp(parsers[i].name, headerParserName))
			{
				parserCallback = parsers[i].callback;
				break;
			}
			i++;
		}
		if(parserCallback == NULL)
		{
			throw FormatParseException(filename + ": Parser '" + std::string(headerParserName) + "' is not a name of a registered parser.\n");
		}
		return parserCallback(f);
	}
}
