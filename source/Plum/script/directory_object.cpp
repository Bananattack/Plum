#include "../plum.h"


namespace Plum
{
	namespace ScriptLibrary
	{
		namespace DirectoryObject
		{
			SCRIPT_OBJ_GETTER(index, Wrapper<Directory>*, libraryName)
			SCRIPT_OBJ_SETTER(newindex, Wrapper<Directory>*, libraryName)

			int create(lua_State* L)
			{
				const char* filename = lua_tostring(L, 1);

				Directory* dir = new Directory(filename);
				// Failure.
				if(!dir->active())
				{	
					delete dir;
					return 0;
				}

				PLUM_PUSH_DATA(L, Directory, dir, LUA_NOREF);
				return 1;
			}

			int gc(lua_State* L)
			{
				Wrapper<Directory>* dir = PLUM_CHECK_DATA(L, 1, Directory);

				// Only delete if it doesn't belong to a parent of some sort.
				if(dir->parentRef != LUA_NOREF)
				{
					delete dir->data;
				}
				else
				{
					luaL_unref(L, LUA_REGISTRYINDEX, dir->parentRef);
				}

				return 0;
			}

			int close(lua_State* L)
			{
				Wrapper<Directory>* dir = PLUM_CHECK_DATA(L, 1, Directory);
				dir->data->close();
				return 0;
			}

			int tostring(lua_State* L)
			{
				PLUM_CHECK_DATA(L, 1, Directory);
				lua_pushstring(L, "(plum.Directory object)");
				return 1;
			}

			int getreal(lua_State* L)
			{
				Wrapper<Directory>* dir = PLUM_CHECK_DATA(L, 1, Directory);
				lua_pushboolean(L, dir->data->isReal());
				return 1;
			}

			int next(lua_State* L)
			{
				Wrapper<Directory>* dir = PLUM_CHECK_DATA(L, 1, Directory);

				std::string value;
				if(dir->data->read(value))
				{
					lua_pushstring(L, value.c_str());
					return 1;
				}
				return 0;
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
				PLUM_BIND_FUNC(next)
				PLUM_BIND_FUNC(getreal)
				PLUM_BIND_FUNC_END_NULL()

				lua_pop(L, 1);
				
				// Push plum namespace.
				lua_getglobal(L, "plum");

				// plum[classname] = <function create>
				lua_pushstring(L, "Directory");
				lua_pushcfunction(L, create);
				lua_settable(L, -3);

				// Pop plum namespace.
				lua_pop(L, 1);
			}
		}
	}
}