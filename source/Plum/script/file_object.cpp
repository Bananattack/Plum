#include "../plum.h"

namespace Plum
{
	namespace Script
	{
		struct FileWrapper
		{
			// Write mode uses physical files, read mode uses a zzip wrapped file.
			bool write;
			// So we don't double close.
			bool closed;
			union
			{
				FILE* physicalFile;
				ZZIP_FILE* zzipFile;
			} storage;
		};

		static FileWrapper* CheckValidFileWrapperObject(lua_State* L, int index)
		{
			return (FileWrapper*) luaL_checkudata(L, index, "plum_file");
		}

		static int file_new(lua_State* L)
		{
			const char* filename = lua_tostring(L, 1);
			const char* mode = lua_tostring(L, 2);

			bool write;
			FILE* physicalFile = NULL;
			ZZIP_FILE* zzipFile = NULL;

			if(mode[0] == 'r')
			{
				write = false;
				zzipFile = zzip_fopen_plum(filename, "rb");
			}
			else if(mode[0] == 'w')
			{
				write = true;
				physicalFile = fopen(filename, "wb");
			}
			else if(mode[0] == 'a')
			{
				write = true;
				physicalFile = fopen(filename, "ab");
			}
			else
			{
				return 0;
			}
			
			if(zzipFile || physicalFile)
			{
				FileWrapper* f = (FileWrapper*) lua_newuserdata(L, sizeof(FileWrapper));
				luaL_getmetatable(L, "plum_file");
				lua_setmetatable(L, -2);

				f->write = write;
				f->closed = false;
				if(!write)
				{
					f->storage.zzipFile = zzipFile;
				}
				else
				{
					f->storage.physicalFile = physicalFile;
				}

				return 1;
			}
			return 0;
		}

		static int file_close(lua_State* L)
		{
			FileWrapper* f = CheckValidFileWrapperObject(L, 1);
			if(!f->closed)
			{
				if(f->write)
				{
					fclose(f->storage.physicalFile);
				}
				else
				{
					zzip_close(f->storage.zzipFile);
				}
				f->closed = true;
			}
			return 0;
		}

		SCRIPT_OBJ_GETTER(file_getField, FileWrapper*, "plum_file")
		SCRIPT_OBJ_SETTER(file_setField, FileWrapper*, "plum_file")

		static int file_toString(lua_State* L)
		{
			CheckValidFileWrapperObject(L, 1);
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
			FileWrapper* f = CheckValidFileWrapperObject(L, 1);
			// Can't access in write mode. Shoo.
			if(f->write)
			{
				luaL_error(L, "Attempt to read from file which was opened for writing.");
				return 0;
			}

			unsigned char v;
			if(zzip_fread(&v, sizeof(unsigned char), 1,  f->storage.zzipFile) == 0)
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
			FileWrapper* f = CheckValidFileWrapperObject(L, 1);
			// Can't access in write mode. Shoo.
			if(f->write)
			{
				luaL_error(L, "Attempt to read from file which was opened for writing.");
				return 0;
			}

			int v;
			if(zzip_fread(&v, sizeof(int), 1, f->storage.zzipFile) == 0)
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
			FileWrapper* f = CheckValidFileWrapperObject(L, 1);
			// Can't access in write mode. Shoo.
			if(f->write)
			{
				luaL_error(L, "Attempt to read from file which was opened for writing.");
				return 0;
			}

			// Adapted from FileReadLn in Verge, with modifications to make it fit nicer.
			char buffer[256];
			std::string result = "";
			// EOL status: 0 = none, 1 = newline, 2 = eof
			int eol = 0;
			do
			{
				zzip_fgets(buffer, 255,  f->storage.zzipFile);

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

		static int file_writeLine(lua_State* L)
		{
			FileWrapper* f = CheckValidFileWrapperObject(L, 1);
			// Can't do this in read mode. Shoo.
			if(!f->write)
			{
				luaL_error(L, "Attempt to write to file which was opened for reading.");
				return 0;
			}
			const char* message = lua_tostring(L, 2);

			fwrite(message, strlen(message), 1, f->storage.physicalFile);
		}

		const luaL_Reg fileMembers[] = {
			{ "__index", file_getField },
			{ "__newindex",	file_setField },
			{ "__tostring",	file_toString },
			{ "__gc", file_close }, // For the lazy.
			{ "close", file_close }, // For the wise.
			{ "readByte", file_readByte },
			{ "readInt", file_readInt },
			{ "readLine", file_readLine },
			{ "writeLine", file_writeLine },
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