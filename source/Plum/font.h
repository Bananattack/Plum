#ifndef PLUM_FONT_H
#define PLUM_FONT_H
namespace Plum
{
	class Font
	{
		public:
			static const int FONT_COLUMNS = 20;
			static const int FONT_ROWS = 5;

			Texture* texture;
			int width, height;
			int letterSpacing;
			int glyphWidth[FONT_COLUMNS * FONT_ROWS];

			Font(std::string filename)
			{
				init(filename.c_str());
			}

			Font(const char* filename)
			{
				init(filename);
			}

			~Font()
			{
				delete texture;
			}
		private:
			void init(const char* filename);
			bool isColumnEmpty(int cell, int column);
		public:
			void enableVariableWidth();
			void printChar(int x, int y, char c, BlendMode mode);
			void print(int x, int y, const std::string s, BlendMode mode = BlendUnspecified);
			
	};
}
#endif