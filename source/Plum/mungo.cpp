#include "plum.h"

namespace Plum
{

	MungoParser::MungoParser(int startingLineOffset)
	{
		this->startingLineOffset = startingLineOffset;
	}

	MungoParser::~MungoParser()
	{
	}

	MungoTable* MungoParser::parseFile(FILE* f)
	{
		if(!f)
		{
			throw MungoParseException("File is not open.\n");
		}

		unsigned int i;
		int c;
		char buffer[256];
		
		lineIndex = startingLineOffset;
		state = MungoStateNormal;
		attachDelimitersToValue = false;
		mapping = false;
		key = "";
		value = "";
		delim = "";
		quote = -1;		
		
		activeDelimiterLookup.clear();
		delimiterCurrent = "";
		listCurrent = NULL;
		mainTable = NULL;

		while(fgets(buffer, 256, f))
		{
			for(i = 0; i < strlen(buffer); i++)
			{
				c = buffer[i];
				if(state != MungoStateQuote && state != MungoStateComment)
				{
					if(CharacterIsListDelimiter(c))
					{
						if(state != MungoStateListDelimiterGather)
						{
							if(attachDelimitersToValue)
							{
								value += c;
							}
							else
							{
								delim = c;
								state = MungoStateListDelimiterGather;
							}
						}
						else
						{
							delim += c;
						}
					}
					else if(CharacterIsMappingDelimiter(c))
					{
						if(!listCurrent)
						{
							std::ostringstream oss;
							oss << "Attempt to define mapping before a list was started on line " << lineIndex << ".\n";
							throw MungoParseException(oss.str());
						}
						if(!value.length())
						{
							std::ostringstream oss;
							oss << "Expected a key identifier but got mapping delimiter '" << c << "' on line " << lineIndex << ".\n";
							throw MungoParseException(oss.str());
						}
						key = value;
						value = "";
						mapping = true;
					}
					else if(CharacterIsCommentStart(c))
					{
						state = MungoStateComment;
					}
					else if(CharacterIsQuote(c))
					{
						if(state == MungoStateListDelimiterGather)
						{
							// Stop gathering list delimiters, switch active list or make a new one.
							handleListDelimiter();
						}
						else if(!listCurrent)
						{
							std::ostringstream oss;
							oss << "Attempt to define mapping before a list was started on line " << lineIndex << ".\n";
							throw MungoParseException(oss.str());
						}
						state = MungoStateQuote;
						quote = c;
						attachDelimitersToValue = false;
					}
					// Whitespace is only used to separate list delimiters from value identifiers.
					// Absence of whitespace makes -4 a string value
					// Presence of whitespace makes - 4 a list item with the value 4. See the point?
					else if(CharacterIsWhitespace(c))
					{
						if (c == '\n')
						{
							lineIndex++;
						}
						if(state == MungoStateListDelimiterGather)
						{
							// Stop gathering list delimiters, switch active list or make a new one.
							handleListDelimiter();
						}
						attachDelimitersToValue = false;
					}
					else if (c >= 32 && c < 127) // Printable character
					{
						if(state == MungoStateListDelimiterGather)
						{
							value += delim;
							delim = "";
							state = MungoStateNormal;
						}
						value += c;
						attachDelimitersToValue = true;
					}
					else
					{
						std::ostringstream oss;
						oss << "Unprintable character ";
						oss << c;
						oss << " was encountered on line " << lineIndex << ".\n";
						throw MungoParseException(oss.str());
					}
				}
				else if(state == MungoStateQuote)
				{
					if(c == quote)
					{
						state = MungoStateNormal;
						quote = -1;
					}
					else if (c == '\n')
					{
						value += c;
						lineIndex++;
					}
					else
					{
						value += c;
					}
				}
				else if(state == MungoStateComment)
				{
					if (c == '\n')
					{
						lineIndex++;
					}
					else if(CharacterIsCommentEnd(c))
					{
						state = MungoStateNormal;
					}
				}

			}
		}
		// Handle the very last item, if there is one.
		if(key.length() || value.length() || state == MungoStateListDelimiterGather)
		{
			handleListDelimiter();
		}
		if(!mainTable)
		{
			return new MungoTable();
		}
		return mainTable;
	}

	void MungoParser::addStringEntry()
	{
		if(!key.length())
		{
			// There was no key mapping involved. This means we'll make this a numbered list item
			int counter = stringToInteger(listCurrent->getString("__counter"));
			key = integerToString(counter);
			counter++;
			listCurrent->putString("__counter", integerToString(counter));
		}
		else if(key == "__depth")
		{
			std::ostringstream oss;
			oss << "__depth is a reserved property for internal use only, attempted to write on line " << lineIndex << ".\n";
			throw MungoParseException(oss.str());
		}
		listCurrent->putString(key, value);
		key = "";
		value = "";
		mapping = false;
	}


	void MungoParser::handleListDelimiter()
	{
		// Delimiter is the same as the current.
		// We're still at the same depth in the list, just add the current item and be done with it.
		if(delim == delimiterCurrent)
		{
			addStringEntry();
		}
		else
		{
			// Check if the delimiter is in the lookup of already active lists.
			MungoDelimiterLookupTable::iterator it = activeDelimiterLookup.find(delim);
			// A list that is already in active use (outer scope), make that become the current list, and clean up a bit.
			if(it != activeDelimiterLookup.end())
			{
				MungoTable* t = it->second;
				int depth = stringToInteger(t->getString("__depth"));

				// Add the last entry we had waiting to be appended.
				addStringEntry();

				// Delete all lists that are deeper than the list we're switching scope to, we don't need them anymore.
				for(it = activeDelimiterLookup.begin(); it != activeDelimiterLookup.end();)
				{
					if(stringToInteger(it->second->getString("__depth")) > depth)
					{
						activeDelimiterLookup.erase(it++);
					}
					else
					{
						++it;
					}
				}

				// Change current list.
				listCurrent = t;
			}
			// Entirely new list
			else
			{
				if(!mainTable)
				{
					mainTable = listCurrent = new MungoTable();
					mainTable->putString("__depth", "0");
				//	fprintf(stderr, "Startup => new list(%s)\n", delim.c_str());
				}
				else if(!mapping)
				{
					int depth = stringToInteger(listCurrent->getString("__depth"));

					// Shove the table in there.
					MungoTable* t = new MungoTable();
					listCurrent->putTable(value, t);
					listCurrent = t;
					listCurrent->putString("__depth", integerToString(depth + 1));

				//	fprintf(stderr, "Typical %s => new list(%s)\n", value.c_str(), delim.c_str());

					value = "";
				}
				else
				{
					int depth = stringToInteger(listCurrent->getString("__depth"));

					//fprintf(stderr,"weird %s => %s\n", key.c_str(), value.c_str());
					//fprintf(stderr, "[number] => new list(%s)\n", delim.c_str());
					// This is a WEIRD case.
					// We just completed a list entry, but now we want to create an unnamed list to nest inside.
					addStringEntry();
					// And nowwww....
					// There was no key mapping involved for this list entry. This means we'll make this a numbered list item
					int counter = stringToInteger(listCurrent->getString("__counter"));
					key = integerToString(counter);
					counter++;
					listCurrent->putString("__counter", integerToString(counter));

					// Shove the table in there.
					MungoTable* t = new MungoTable();
					listCurrent->putTable(key, t);
					listCurrent = t;
					listCurrent->putString("__depth", integerToString(depth + 1));
				}
				// Add the delimiter to the list of actively used delimiter
				activeDelimiterLookup[delim] = listCurrent;
			}
		}
		// End delimiter lookup state.
		state = MungoStateNormal;
	}

	MungoTable* MungoParseFile(FILE* f)
	{
		MungoParser mp(2); // Since the brockoly headers take up a line, we begin at line 2 here.
		return mp.parseFile(f);
	}
}
