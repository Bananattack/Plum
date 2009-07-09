#pragma once
namespace Plum
{
	class SHA1
	{
		public:
			static void digestString(const std::string& str, u8 hash[20]);
			static bool digestFile(const std::string& str, u8 hash[20]);
	};
}