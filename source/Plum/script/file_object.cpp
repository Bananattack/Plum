#include "../plum.h"

namespace Plum
{
	namespace Script
	{
		/*struct FileWrapper
		{
			bool physical;
			union
			{
				FILE* physicalFile;
				ZZIP* zzipFile;
			} f;
		};*/

		static ZZIP_FILE** CheckValidZZIP_FILEObject(lua_State* L, int index)
		{
			return (ZZIP_FILE**) luaL_checkudata(L, index, "plum_file");
		}

		static int file_new(lua_State* L)
		{
			const char* filename = lua_tostring(L, 1);
			const char* mode = lua_tostring(L, 2);

			ZZIP_FILE* file = zzip_fopen_plum(filename, mode);
			if(file)
			{
				ZZIP_FILE** f = (ZZIP_FILE**) lua_newuserdata(L, sizeof(ZZIP_FILE*));
				luaL_getmetatable(L, "plum_file");
				lua_setmetatable(L, -2);

				*f = file;
				return 1;
			}
			return 0;
		}

		static int file_gc(lua_State* L)
		{
			ZZIP_FILE** f = CheckValidZZIP_FILEObject(L, 1);
			zzip_close(*f);

			return 0;
		}

		SCRIPT_OBJ_GETTER(file_getField, ZZIP_FILE**, "plum_file")
		SCRIPT_OBJ_SETTER(file_setField, ZZIP_FILE**, "plum_file")

		static int file_toString(lua_State* L)
		{
			CheckValidZZIP_FILEObject(L, 1);
			lua_pushstring(L, "(plum.File object)");
			return 1;
		}

		static int zzip_getc (ZZIP_FILE *f)
		{
			char c;
			return (zzip_fread(&c, sizeof(char), 1, f) == 0) ? EOF : (int) c;
		}

		static char* zzip_fgets(char *str, int size, ZZIP_FILE *stream)
		{
			int c, i;

			for (i = 0; i < size-1; i++)
			{
				c = zzip_getc(stream);
				if (c == EOF)
				{
					break;
				}
				str[i] = c;
				if (c == '\n')
				{
					str[i++] = '\n';
					break;
				}
			}
			str[i] = 0;

			return str;
		}

		static int file_readByte(lua_State* L)
		{
			ZZIP_FILE** f = CheckValidZZIP_FILEObject(L, 1);
			unsigned char v;
			if(zzip_fread(&v, sizeof(unsigned char), 1, *f) == 0)
			{
				return 0;
			}
			else
			{
				lua_pushinteger(L, v);
				return 1;
			}
		}

		// TODO: endian flips.
		static int file_readInt(lua_State* L)
		{
			ZZIP_FILE** f = CheckValidZZIP_FILEObject(L, 1);
			int v;
			if(zzip_fread(&v, sizeof(int), 1, *f) == 0)
			{
				return 0;
			}
			else
			{
				lua_pushinteger(L, v);
				return 1;
			}
		}

		static int file_readLine(lua_State* L)
		{
			ZZIP_FILE** f = CheckValidZZIP_FILEObject(L, 1);

			// Adapted from FileReadLn in Verge, with modifications to make it fit nicer.
			char buffer[256];
			std::string result = "";
			// EOL status: 0 = none, 1 = newline, 2 = eof
			int eol = 0;
			do
			{
				zzip_fgets(buffer, 255, *f);

				// we didn't read anything, this is eof
				if(buffer[0] == '\0')
				{
					eol = 2; 
				}
				// Last character is a newline character, so it's the end of a line
				else if(buffer[strlen(buffer)-1] == 10 || buffer[strlen(buffer)-1] == 13)
				{
					buffer[strlen(buffer)-1] = '\0';
					eol = 1;
				}

				result += buffer;
			} while(!eol);

			// If we encounter an EOF *and* there was nothing read,
			// return nil to make it easy to tell script-side.
			if(eol == 2 && result.length() == 0)
			{
				return 0;
			}
			// Otherwise, we have a happy line read :D
			else
			{
				lua_pushstring(L, result.c_str());
				return 1;
			}
		}

		const luaL_Reg fileMembers[] = {
			{ "__index", file_getField },
			{ "__newindex",	file_setField },
			{ "__tostring",	file_toString },
			{ "__gc", file_gc },
			{ "readByte", file_readByte },
			{ "readInt", file_readInt },
			{ "readLine", file_readLine },
			{ NULL, NULL }
		};

		void initFileClass(lua_State* L)
		{
			luaL_newmetatable(L, "plum_file");
			// Duplicate the metatable on the stack.
			lua_pushvalue(L, -1);
			// metatable.__index = metatable
			lua_setfield(L, -2, "__index");
			// Put the members into the metatable.
			luaL_register(L, NULL, fileMembers);
			lua_pop(L, 1);

			// Push plum namespace.
			lua_getglobal(L, "plum");

			// plum.file = <function file_new>
			lua_pushstring(L, "File");
			lua_pushcfunction(L, file_new);
			lua_settable(L, -3);

			// Pop plum namespace.
			lua_pop(L, 1);
		}
	}
}