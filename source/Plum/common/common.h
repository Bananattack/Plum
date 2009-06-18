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
}

#endif