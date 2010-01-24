#pragma once

namespace Plum
{
	class Directory
	{
		private:
			bool closed;

			bool real;
			ZZIP_DIR* dir;
		public:
			Directory(const char* filename);
			~Directory();

			bool close();

			bool isReal()
			{
				return real;
			}

			bool active()
			{
				return !closed && dir != NULL;
			}

			bool read(std::string& value);

	};
}
