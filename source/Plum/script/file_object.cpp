#include "../plum.h"


namespace Plum
{
	namespace ScriptLibrary
	{
		namespace FileObject
		{
			SCRIPT_OBJ_GETTER(index, Wrapper<File>*, libraryName)
			SCRIPT_OBJ_SETTER(newindex, Wrapper<File>*, libraryName)

			int create(lua_State* L)
			{
				const char* filename = lua_tostring(L, 1);
				const char* modeString = lua_tostring(L, 2);
				FileOpenMode mode;

				if(modeString[0] == 'r')
				{
					mode = FileRead;
				}
				else if(modeString[0] == 'w')
				{
					mode = FileWrite;
				}
				else if(modeString[0] == 'a')
				{
					mode = FileAppend;
				}
				else
				{
					luaL_error(L, "Attempt to call plum.File constructor with invalid mode argument (argument #2).\r\nMust be 'r', 'w' or 'a'.");
					return 0;
				}

				File* f = new File(filename, mode);
				// Failure.
				if(!f->active())
				{	
					delete f;
					return 0;
				}

				PLUM_PUSH_DATA(L, File, f, LUA_NOREF);
				return 1;
			}

			int gc(lua_State* L)
			{
				Wrapper<File>* file = PLUM_CHECK_DATA(L, 1, File);

				// Only delete if it doesn't belong to a parent of some sort.
				if(file->parentRef != LUA_NOREF)
				{
					delete file->data;
				}
				else
				{
					luaL_unref(L, LUA_REGISTRYINDEX, file->parentRef);
				}

				return 0;
			}

			int close(lua_State* L)
			{
				Wrapper<File>* file = PLUM_CHECK_DATA(L, 1, File);
				file->data->close();
				return 0;
			}

			int tostring(lua_State* L)
			{
				PLUM_CHECK_DATA(L, 1, File);
				lua_pushstring(L, "(plum.File object)");
				return 1;
			}

			int readU8(lua_State* L)
			{
				Wrapper<File>* file = PLUM_CHECK_DATA(L, 1, File);

				u8 value;
				if(file->data->readU8(value))
				{
					lua_pushinteger(L, value);
					return 1;
				}
				return 0;
			}

			int readU16(lua_State* L)
			{
				Wrapper<File>* file = PLUM_CHECK_DATA(L, 1, File);

				u16 value;
				if(file->data->readU16(value))
				{
					lua_pushinteger(L, value);
					return 1;
				}
				return 0;
			}

			int readU32(lua_State* L)
			{
				Wrapper<File>* file = PLUM_CHECK_DATA(L, 1, File);

				u32 value;
				if(file->data->readU32(value))
				{
					lua_pushinteger(L, value);
					return 1;
				}
				return 0;
			}

			int readInt8(lua_State* L)
			{
				Wrapper<File>* file = PLUM_CHECK_DATA(L, 1, File);

				int8 value;
				if(file->data->readInt8(value))
				{
					lua_pushinteger(L, value);
					return 1;
				}
				return 0;
			}

			int readInt16(lua_State* L)
			{
				Wrapper<File>* file = PLUM_CHECK_DATA(L, 1, File);

				int16 value;
				if(file->data->readInt16(value))
				{
					lua_pushinteger(L, value);
					return 1;
				}
				return 0;
			}

			int readInt32(lua_State* L)
			{
				Wrapper<File>* file = PLUM_CHECK_DATA(L, 1, File);

				int32 value;
				if(file->data->readInt32(value))
				{
					lua_pushinteger(L, value);
					return 1;
				}
				return 0;
			}

			int readFloat(lua_State* L)
			{
				Wrapper<File>* file = PLUM_CHECK_DATA(L, 1, File);

				float value;
				if(file->data->readFloat(value))
				{
					lua_pushnumber(L, value);
					return 1;
				}
				return 0;
			}

			int readDouble(lua_State* L)
			{
				Wrapper<File>* file = PLUM_CHECK_DATA(L, 1, File);

				double value;
				if(file->data->readDouble(value))
				{
					lua_pushnumber(L, value);
					return 1;
				}
				return 0;
			}

			int readBlob(lua_State* L)
			{
				Wrapper<File>* file = PLUM_CHECK_DATA(L, 1, File);
				int blockSize = luaL_checkinteger(L, 2);

				char* buffer = new char[blockSize];
				int size = file->data->readRaw(buffer, blockSize);

				if(size)
				{
					lua_pushlstring(L, buffer, blockSize);
				}

				delete [] buffer;
				return size > 0;
			}

			int readFixedString(lua_State* L)
			{
				Wrapper<File>* file = PLUM_CHECK_DATA(L, 1, File);
				int blockSize = luaL_checkinteger(L, 2);

				char* buffer = new char[blockSize];
				int size = file->data->readRaw(buffer, blockSize);

				// Force termination within block to prevent exceeding the buffer.
				buffer[blockSize - 1] = 0;

				if(size)
				{
					lua_pushstring(L, buffer);
				}

				delete [] buffer;
				return size > 0;
			}

			int readLine(lua_State* L)
			{
				Wrapper<File>* file = PLUM_CHECK_DATA(L, 1, File);

				std::string value;
				if(file->data->readLine(value))
				{
					lua_pushstring(L, value.c_str());
					return 1;
				}
				return 0;
			}
			
			int readFully(lua_State* L)
			{
				Wrapper<File>* file = PLUM_CHECK_DATA(L, 1, File);

				// Get current position.
				unsigned int pos = file->data->tell();
				if(pos == -1)
				{
					return 0;
				}

				// Length to read = position of end - current.
				unsigned int length = 0;
				if(!file->data->seek(pos, SeekEnd))
				{
					return 0;
				}
				length = file->data->tell() - pos;
				if(!file->data->seek(pos, SeekStart))
				{
					return 0;
				}

				// Read the string, making sure to terminate with null byte.
				char* buf = new char[length + 1];
				file->data->readRaw(buf, length);
				buf[length] = 0;
				lua_pushstring(L, buf);

				delete [] buf;

				return 1;
			}

			int readVergeCompressed(lua_State* L)
			{
				Wrapper<File>* file = PLUM_CHECK_DATA(L, 1, File);
				int blockSize = luaL_checkinteger(L, 2);

				char* buffer = new char[blockSize];
				bool success = file->data->readVergeCompressed(buffer, blockSize);

				if(success)
				{
					lua_pushlstring(L, buffer, blockSize);
				}

				delete [] buffer;
				return success;
			}

			int writeU8(lua_State* L)
			{
				Wrapper<File>* file = PLUM_CHECK_DATA(L, 1, File);
				int value = luaL_checkinteger(L, 2);

				lua_pushboolean(L, file->data->writeU8(value));
				return 1;
			}

			int writeU16(lua_State* L)
			{
				Wrapper<File>* file = PLUM_CHECK_DATA(L, 1, File);
				int value = luaL_checkinteger(L, 2);

				lua_pushboolean(L, file->data->writeU16(value));
				return 1;
			}

			int writeU32(lua_State* L)
			{
				Wrapper<File>* file = PLUM_CHECK_DATA(L, 1, File);
				int value = luaL_checkinteger(L, 2);

				lua_pushboolean(L, file->data->writeU32(value));
				return 1;
			}

			int writeInt8(lua_State* L)
			{
				Wrapper<File>* file = PLUM_CHECK_DATA(L, 1, File);
				int value = luaL_checkinteger(L, 2);

				lua_pushboolean(L, file->data->writeInt8(value));
				return 1;
			}

			int writeInt16(lua_State* L)
			{
				Wrapper<File>* file = PLUM_CHECK_DATA(L, 1, File);
				int value = luaL_checkinteger(L, 2);

				lua_pushboolean(L, file->data->writeInt16(value));
				return 1;
			}

			int writeInt32(lua_State* L)
			{
				Wrapper<File>* file = PLUM_CHECK_DATA(L, 1, File);
				int value = luaL_checkinteger(L, 2);

				lua_pushboolean(L, file->data->writeInt32(value));
				return 1;
			}

			int writeFloat(lua_State* L)
			{
				Wrapper<File>* file = PLUM_CHECK_DATA(L, 1, File);
				double value = luaL_checknumber(L, 2);

				lua_pushboolean(L, file->data->writeFloat((float) value));
				return 1;
			}

			int writeDouble(lua_State* L)
			{
				Wrapper<File>* file = PLUM_CHECK_DATA(L, 1, File);
				double value = luaL_checknumber(L, 2);

				lua_pushboolean(L, file->data->writeDouble(value));
				return 1;
			}

			int writeString(lua_State* L)
			{
				Wrapper<File>* file = PLUM_CHECK_DATA(L, 1, File);
				size_t length = 0;
				const char* value = luaL_checklstring(L, 2, &length);

				lua_pushboolean(L, file->data->writeString(value, length));
				return 1;
			}

			int writeLine(lua_State* L)
			{
				Wrapper<File>* file = PLUM_CHECK_DATA(L, 1, File);
				size_t length = 0;
				const char* value = luaL_checklstring(L, 2, &length);

				lua_pushboolean(L, file->data->writeLine(value, length));
				return 1;
			}

			void openLibrary(lua_State* L)
			{
				luaL_newmetatable(L, libraryName);
				// Duplicate the metatable on the stack.
				lua_pushvalue(L, -1);
				// metatable.__index = metatable
				lua_setfield(L, -2, "__index");

				// Put the members into the metatable.
				PLUM_BIND_FUNC_BEGIN()
				PLUM_BIND_META(gc)
				PLUM_BIND_META(index)
				PLUM_BIND_META(newindex)
				PLUM_BIND_META(tostring)
				PLUM_BIND_FUNC(close)
				PLUM_BIND_FUNC(readU8)
				PLUM_BIND_FUNC(readU16)
				PLUM_BIND_FUNC(readU32)
				PLUM_BIND_FUNC(readInt8)
				PLUM_BIND_FUNC(readInt16)
				PLUM_BIND_FUNC(readInt32)
				PLUM_BIND_FUNC(readFloat)
				PLUM_BIND_FUNC(readDouble)
				PLUM_BIND_FUNC(readBlob)
				PLUM_BIND_FUNC(readFixedString)
				PLUM_BIND_FUNC(readLine)
				PLUM_BIND_FUNC(readFully)
				PLUM_BIND_FUNC(readVergeCompressed)
				PLUM_BIND_FUNC(writeU8)
				PLUM_BIND_FUNC(writeU16)
				PLUM_BIND_FUNC(writeU32)
				PLUM_BIND_FUNC(writeInt8)
				PLUM_BIND_FUNC(writeInt16)
				PLUM_BIND_FUNC(writeInt32)
				PLUM_BIND_FUNC(writeFloat)
				PLUM_BIND_FUNC(writeDouble)
				PLUM_BIND_FUNC(writeString)
				PLUM_BIND_FUNC(writeLine)
				PLUM_BIND_FUNC_END_NULL()

				lua_pop(L, 1);
				
				// Push plum namespace.
				lua_getglobal(L, "plum");

				// plum.File = <function create>
				lua_pushstring(L, "File");
				lua_pushcfunction(L, create);
				lua_settable(L, -3);

				// Pop plum namespace.
				lua_pop(L, 1);
			}
		}
	}
}