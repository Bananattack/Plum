
#include "../core/image.h"
#include "../core/canvas.h"
#include "../core/sheet.h"
#include "script.h"

namespace plum
{
    namespace script
    {
        template<> const char* meta<Sheet>()
        {
            return "plum.Sheet";
        }
    }

    namespace
    {
        typedef Sheet Self;

        int create(lua_State* L)
        {
            if(script::is<int>(L, 1) && script::is<int>(L, 2) && script::is<int>(L, 3) && script::is<int>(L, 4))
            {
                int frameWidth = script::get<int>(L, 1);
                int frameHeight = script::get<int>(L, 2);
                int columns = script::get<int>(L, 3);
                int rows = script::get<int>(L, 4);
                script::push(L, new Sheet(frameWidth, frameHeight, columns, rows), LUA_NOREF);
                return 1;
            }
            else if(script::is<int>(L, 1) && script::is<int>(L, 2) && script::is<Image>(L, 3))
            {
                int frameWidth = script::get<int>(L, 1);
                int frameHeight = script::get<int>(L, 2);
                auto img = script::ptr<Image>(L, 3);
                script::push(L, new Sheet(frameWidth, frameHeight, img->canvas().getWidth() / frameWidth, img->canvas().getHeight() / frameHeight), LUA_NOREF);
                return 1;
            }
            else if(script::is<int>(L, 1) && script::is<int>(L, 2) && script::is<Canvas>(L, 3))
            {
                int frameWidth = script::get<int>(L, 1);
                int frameHeight = script::get<int>(L, 2);
                auto canvas = script::ptr<Canvas>(L, 3);
                script::push(L, new Sheet(frameWidth, frameHeight, canvas->getWidth() / frameWidth, canvas->getHeight() / frameHeight), LUA_NOREF);
                return 1;
            }
            luaL_error(L, "Attempt to call plum.Sheet constructor with invalid argument types.\r\n"
                "Must be (int frameWidth, int frameHeight, int columns, int rows)"
                " or (int frameWidth, int frameHeight, Image img)"
                " or (int frameWidth, int frameHeight, Canvas canvas).");
            return 0;
        }

        int gc(lua_State* L)
        {
            return script::wrapped<Self>(L, 1)->gc(L);
        }

        int index(lua_State* L)
        {
            return script::wrapped<Self>(L, 1)->index(L);
        }

        int newindex(lua_State* L)
        {
            return script::wrapped<Self>(L, 1)->newindex(L);
        }

        int tostring(lua_State* L)
        {
            return script::wrapped<Self>(L, 1)->tostring(L);
        }

        int getFrame(lua_State* L)
        {
            auto sheet = script::ptr<Sheet>(L, 1);
            int f = script::get<int>(L, 2);

            int x, y;
            if(sheet->getFrame(f, x, y))
            {
               script::push(L, x);
               script::push(L, y);
               return 2;
            }
            else
            {
                return 0;
            }
        }

        int get_width(lua_State* L)
        {
            auto sheet = script::ptr<Sheet>(L, 1);
            script::push(L, sheet->getWidth());
            return 1;
        }

        int set_width(lua_State* L)
        {
            auto sheet = script::ptr<Sheet>(L, 1);
            sheet->setWidth(script::get<int>(L, 2));
            return 0;
        }

        int get_height(lua_State* L)
        {
            auto sheet = script::ptr<Sheet>(L, 1);
            script::push(L, sheet->getHeight());
            return 1;
        }

        int set_height(lua_State* L)
        {
            auto sheet = script::ptr<Sheet>(L, 1);
            sheet->setHeight(script::get<int>(L, 2));
            return 0;
        }

        int get_padding(lua_State* L)
        {
            auto sheet = script::ptr<Sheet>(L, 1);
            script::push(L, sheet->getPadding());
            return 1;
        }

        int set_padding(lua_State* L)
        {
            auto sheet = script::ptr<Sheet>(L, 1);
            sheet->setPadding(script::get<bool>(L, 2));
            return 0;
        }

        int get_columns(lua_State* L)
        {
            auto sheet = script::ptr<Sheet>(L, 1);
            script::push(L, sheet->getColumns());
            return 1;
        }

        int set_columns(lua_State* L)
        {
            auto sheet = script::ptr<Sheet>(L, 1);
            sheet->setColumns(script::get<int>(L, 2));
            return 0;
        }

        int get_rows(lua_State* L)
        {
            auto sheet = script::ptr<Sheet>(L, 1);
            script::push(L, sheet->getRows());
            return 1;
        }

        int set_rows(lua_State* L)
        {
            auto sheet = script::ptr<Sheet>(L, 1);
            sheet->setRows(script::get<int>(L, 2));
            return 0;
        }
    }

    namespace script
    {
        void initSheetObject(lua_State* L)
        {
            luaL_newmetatable(L, meta<Self>());
            // Duplicate the metatable on the stack.
            lua_pushvalue(L, -1);
            // metatable.__index = metatable
            lua_setfield(L, -2, "__index");

            // Put the members into the metatable.
            const luaL_Reg functions[] = {
                {"__gc", gc},
                {"__index", index},
                {"__newindex", newindex},
                {"__tostring", tostring},
                {"getFrame", getFrame},
                {"get_width", get_width},
                {"set_width", set_width},
                {"get_height", get_height},
                {"set_height", set_height},
                {"get_padding", get_padding},
                {"set_padding", set_padding},
                {"get_columns", get_columns},
                {"set_columns", set_columns},
                {"get_rows", get_rows},
                {"set_rows", set_rows},
                {nullptr, nullptr}
            };
            luaL_setfuncs(L, functions, 0);

            lua_pop(L, 1);

            // Push plum namespace.
            lua_getglobal(L, "plum");

            // plum.Sheet = <function create>
            script::push(L, "Sheet");
            lua_pushcfunction(L, create);
            lua_settable(L, -3);

            // Pop plum namespace.
            lua_pop(L, 1);
        }
    }
}
