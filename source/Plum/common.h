#ifndef PLUM_COMMON_H
#define PLUM_COMMON_H

// Mmm... plum pie.
const double PLUM_PI = 3.14159265358;

#define PLUM_ABS(a) (((a) < 0) ? -(a) : (a))
#define PLUM_MIN(a, b) (((a) < (b)) ? (a) : (b))
#define PLUM_MAX(a, b) (((a) > (b)) ? (a) : (b))
#define PLUM_SWAP(a, b)	{ \
					(a) = (a) - (b); \
					(b) = (a) + (b); \
					(a) = (b) - (a); \
				}

namespace Plum
{
	typedef size_t uint;
	inline int stringToInteger(const std::string& s)
	{
		return atoi(s.c_str());
	}

	inline bool isStringNumeric(const std::string& s)
	{
		for (unsigned int i = 0; i < s.length(); i++)
		{
			if(!isdigit(s[i]))
			{
				return false;
			}
		}
		return true;
	}

	inline std::string integerToString(const int x)
	{
		std::ostringstream o;
		if (!(o << x)) return "";
		return o.str();
	}

	std::string toLower(const std::string& s);
	std::string toUpper(const std::string& s);

	// Path helpers.
	// Lifted from ika shamelessly
	namespace Path
	{
		// ifdef and blah blah for platform independance
		extern const char* delimeters;

		// strips the path from the filename, and returns it.
		// If relativeto is not specified or is empty, then the entire path is returned.
		// If it is specified, then the path returned is relative to the path contained therein.
		std::string getDirectory(const std::string& s);

		std::string getFileName(const std::string& s);           // same idea, but just returns the filename

		std::string getExtension(const std::string& s);          // returns the extension. (excluding the period)
		std::string replaceExtension(const std::string& s, const std::string& extension); // swaps the extension, and returns the result

		bool equals(const std::string& s, const std::string& t); // returns true if the two paths are the same.  Disregards case in win32.
	};



}

#endif