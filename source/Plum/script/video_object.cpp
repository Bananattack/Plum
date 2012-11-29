#include "../plum.h"
#include "../video/video.h"
#include "script.h"

namespace plum
{
    namespace
    {
        const char* METANAME = "plum_video";
        int video_getField(lua_State* L)
        {
            luaL_checkudata(L, 1, METANAME);
            std::string fieldName = luaL_checkstring(L, 2);
            if(luaL_getmetafield(L, 1, std::string("get" + fieldName).c_str()))
            {
                lua_pushvalue(L, 1);
                lua_call(L, 1, 1);
                return 1;
            }
            return luaL_getmetafield(L, 1, fieldName.c_str());
        }

        int video_setField(lua_State* L)
        {
            luaL_checkudata(L, 1, METANAME);
            std::string fieldName = luaL_checkstring(L, 2);
            /* L, 3 is the value to set. */
            if(luaL_getmetafield(L, 1, std::string("set" + fieldName).c_str()))
            {
                lua_pushvalue(L, 1);
                lua_pushvalue(L, 3);
                lua_call(L, 2, 0);
                return 0;
            }
            if(luaL_getmetafield(L, 1, std::string("get" + fieldName).c_str()))
            {
                luaL_error(L, "Attempt to modify readonly field '%s' on plum_video.", fieldName.c_str());
                lua_pop(L, 1);
                return 0;
            }
            luaL_error(L, "Attempt to modify unknown field '%s' on plum_video.", fieldName.c_str());
            return 0;
        }


        int video_toString(lua_State* L)
        {
            luaL_checkudata(L, 1, METANAME);
            lua_pushstring(L, "(plum.Video singleton)");
            return 1;
        }

        int video_clear(lua_State* L)
        {
            luaL_checkudata(L, 1, METANAME);
            Color color = luaL_checkint(L, 2);

            script::instance(L).engine().video.clear(color);
            return 0;
        }

        int video_setPixel(lua_State* L)
        {
            luaL_checkudata(L, 1, METANAME);
            int x = luaL_checkint(L, 2);
            int y = luaL_checkint(L, 3);
            Color color = luaL_checkint(L, 4);
            BlendMode mode = (BlendMode) script::get<int>(L, 5, BlendUnspecified);

            script::instance(L).engine().video.setPixel(x, y, color, mode);
            return 0;
        }

        int video_rect(lua_State* L)
        {
            luaL_checkudata(L, 1, METANAME);
            int x = luaL_checkint(L, 2);
            int y = luaL_checkint(L, 3);
            int x2 = luaL_checkint(L, 4);
            int y2 = luaL_checkint(L, 5);
            Color color = luaL_checkint(L, 6);
            BlendMode mode = (BlendMode) script::get<int>(L, 7, BlendUnspecified);

            script::instance(L).engine().video.rect(x, y, x2, y2, color, mode);
            return 0;
        }

        int video_solidRect(lua_State* L)
        {
            luaL_checkudata(L, 1, METANAME);
            int x = luaL_checkint(L, 2);
            int y = luaL_checkint(L, 3);
            int x2 = luaL_checkint(L, 4);
            int y2 = luaL_checkint(L, 5);
            Color color = luaL_checkint(L, 6);
            BlendMode mode = (BlendMode) script::get<int>(L, 7, BlendUnspecified);

            script::instance(L).engine().video.solidRect(x, y, x2, y2, color, mode);
            return 0;
        }

        int video_line(lua_State* L)
        {
            luaL_checkudata(L, 1, METANAME);
            int x = luaL_checkint(L, 2);
            int y = luaL_checkint(L, 3);
            int x2 = luaL_checkint(L, 4);
            int y2 = luaL_checkint(L, 5);
            Color color = luaL_checkint(L, 6);
            BlendMode mode = (BlendMode) script::get<int>(L, 7, BlendUnspecified);

            script::instance(L).engine().video.line(x, y, x2, y2, color, mode);
            return 0;
        }

        int video_horizontalGradientRect(lua_State* L)
        {
            luaL_checkudata(L, 1, METANAME);
            int x = luaL_checkint(L, 2);
            int y = luaL_checkint(L, 3);
            int x2 = luaL_checkint(L, 4);
            int y2 = luaL_checkint(L, 5);
            Color color = luaL_checkint(L, 6);
            Color color2 = luaL_checkint(L, 7);
            BlendMode mode = (BlendMode) script::get<int>(L, 8, BlendUnspecified);

            script::instance(L).engine().video.horizontalGradientRect(x, y, x2, y2, color, color2, mode);
            return 0;
        }

        int video_verticalGradientRect(lua_State* L)
        {
            luaL_checkudata(L, 1, METANAME);
            int x = luaL_checkint(L, 2);
            int y = luaL_checkint(L, 3);
            int x2 = luaL_checkint(L, 4);
            int y2 = luaL_checkint(L, 5);
            Color color = luaL_checkint(L, 6);
            Color color2 = luaL_checkint(L, 7);
            BlendMode mode = (BlendMode) script::get<int>(L, 8, BlendUnspecified);

            script::instance(L).engine().video.verticalGradientRect(x, y, x2, y2, color, color2, mode);
            return 0;
        }


        int video_circle(lua_State* L)
        {
            luaL_checkudata(L, 1, METANAME);
            int x = luaL_checkint(L, 2);
            int y = luaL_checkint(L, 3);
            int xrad = luaL_checkint(L, 4);
            int yrad = luaL_checkint(L, 5);
            Color color = luaL_checkint(L, 6);
            BlendMode mode = (BlendMode) script::get<int>(L, 7, BlendUnspecified);

            script::instance(L).engine().video.circle(x, y, xrad, yrad, color, mode);
            return 0;
        }

        int video_solidCircle(lua_State* L)
        {
            luaL_checkudata(L, 1, METANAME);
            int x = luaL_checkint(L, 2);
            int y = luaL_checkint(L, 3);
            int xrad = luaL_checkint(L, 4);
            int yrad = luaL_checkint(L, 5);
            Color color = luaL_checkint(L, 6);
            BlendMode mode = (BlendMode) script::get<int>(L, 7, BlendUnspecified);

            script::instance(L).engine().video.solidCircle(x, y, xrad, yrad, color, mode);
            return 0;
        }

        int video_getWidth(lua_State* L)
        {
            luaL_checkudata(L, 1, METANAME);
            lua_pushinteger(L, script::instance(L).engine().video.getScreenWidth());
            return 1;
        }

        int video_getHeight(lua_State* L)
        {
            luaL_checkudata(L, 1, METANAME);
            lua_pushinteger(L, script::instance(L).engine().video.getScreenHeight());
            return 1;
        }

        int video_getBlendMode(lua_State* L)
        {
            luaL_checkudata(L, 1, METANAME);
            lua_pushinteger(L, getBlendMode());
            return 1;
        }

        int video_setBlendMode(lua_State* L)
        {
            luaL_checkudata(L, 1, METANAME);
            BlendMode mode = (BlendMode) luaL_checkint(L, 2);
            setBlendMode(mode);
            return 0;
        }

        int video_getOpacity(lua_State* L)
        {
            luaL_checkudata(L, 1, METANAME);
            lua_pushinteger(L, getOpacity());
            return 1;
        }

        int video_setOpacity(lua_State* L)
        {
            luaL_checkudata(L, 1, METANAME);
            int v = luaL_checkint(L, 2);
            setOpacity(v);
            return 0;
        }

        const luaL_Reg videoMembers[] = {
            { "__index", video_getField },
            { "__newindex", video_setField },
            { "__tostring",    video_toString },

            { "clear", video_clear },
            { "setPixel", video_setPixel },
            { "line", video_line },
            { "rect", video_rect },
            { "solidRect", video_solidRect },
            { "circle", video_circle },
            { "solidCircle", video_solidCircle },
            { "horizontalGradientRect", video_horizontalGradientRect },
            { "verticalGradientRect", video_verticalGradientRect },

            { "getwidth", video_getWidth },
            { "getheight", video_getHeight },
            { "getblendMode", video_getBlendMode },
            { "setblendMode", video_setBlendMode },
            { "getopacity", video_getOpacity },
            { "setopacity", video_setOpacity },

            { NULL, NULL }
        };
    }

    namespace script
    {
        void initVideoModule(lua_State* L)
        {
            // Load timer metatable
            luaL_newmetatable(L, METANAME);
            // Duplicate the metatable on the stack.
            lua_pushvalue(L, -1);
            // metatable.__index = metatable
            lua_setfield(L, -2, "__index");
            // Put the members into the metatable.
            luaL_register(L, NULL, videoMembers);
            lua_pop(L, 1);

            // Push plum namespace.
            lua_getglobal(L, "plum");

            // Create video
            lua_newuserdata(L, 0); // empty userdata (change this if it causes problems).
            lua_pushvalue(L, -1);
            lua_setfield(L, -3, "video");

            luaL_getmetatable(L, METANAME);
            lua_setmetatable(L, -2);

            // Pop video.
            lua_pop(L, 1);

            // Pop plum namespace.
            lua_pop(L, 1);
        }
    }
}