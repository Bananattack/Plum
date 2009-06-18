#include "../plum.h"

namespace Plum
{
	std::string toLower(const std::string& s)
	{
		std::string t(s);

		for (uint i = 0; i < t.length(); i++)
		{
			if (t[i] >= 'A' && t[i] <= 'Z')
			{
				t[i] ^= 32;
			}
		}

		return t;
	}

	std::string toUpper(const std::string& s)
	{
		std::string t(s);

		for (uint i = 0; i < t.length(); i++)
		{
			if (t[i] >= 'a' && t[i] <= 'z')
			{
				t[i] ^= 32;
			}
		}

		return t;
	}
}