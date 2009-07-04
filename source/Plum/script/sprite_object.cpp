#include "../plum.h"

namespace Plum
{
	static Sprite** checkValidSpriteObject(lua_State* L, int index)
	{
		return (Sprite**) luaL_checkudata(L, index, "plum_sprite");
	}

	static int spriteNew(lua_State* L)
	{
		double x = luaL_checknumber(L, 1); 
		double y = luaL_checknumber(L, 2); 
		const char* filename = luaL_checkstring(L, 3);

		Sprite** s = (Sprite**) lua_newuserdata(L, sizeof(Sprite*));
		luaL_getmetatable(L, "plum_sprite");
		lua_setmetatable(L, -2);

		*s = new Sprite(x, y, filename, L);
		
		return 1;
	}

	static int spriteGC(lua_State* L)
	{
		Sprite** s = checkValidSpriteObject(L, 1);
		delete *s;

		return 0;
	}

	SCRIPT_OBJ_GETTER(spriteGetField, Sprite**, "plum_sprite")
	SCRIPT_OBJ_SETTER(spriteSetField, Sprite**, "plum_sprite")

	static int spriteToString(lua_State* L)
	{
		checkValidSpriteObject(L, 1);
		lua_pushstring(L, "(plum.Sprite object)");
		return 1;
	}

	static int spriteBlit(lua_State* L)
	{
		Sprite** s = checkValidSpriteObject(L, 1);

		(*s)->blit();
		return 0;
	}

	static int spriteUpdate(lua_State* L)
	{
		Sprite** s = checkValidSpriteObject(L, 1);

		(*s)->update();
		return 0;
	}

	static int spriteSwitchAnimation(lua_State* L)
	{
		Sprite** s = checkValidSpriteObject(L, 1);
		const char* strand = luaL_checkstring(L, 2);
		const char* direction = luaL_checkstring(L, 3);

		(*s)->setAnimation(strand, direction);
		return 0;
	}

	static int spriteGetTexture(lua_State* L)
	{
		Sprite** s = checkValidSpriteObject(L, 1); 
		Script::texturePushForSprite(L, *s);
		return 1;
	}

	static int spriteGetConfig(lua_State* L)
	{
		Sprite** s = checkValidSpriteObject(L, 1); 
		lua_pushvalue((*s)->cfg.lua, -1);
		lua_xmove((*s)->cfg.lua, L, 1);
		return 1;
	}

	static int spriteGetX(lua_State* L)
	{
		Sprite** s = checkValidSpriteObject(L, 1); 
		lua_pushnumber(L, (*s)->x);
		return 1;
	}

	static int spriteSetX(lua_State* L)
	{
		Sprite** s = checkValidSpriteObject(L, 1);
		double value = luaL_checknumber(L, 2);
		(*s)->x = value;
		return 0;
	}

	static int spriteGetY(lua_State* L)
	{
		Sprite** s = checkValidSpriteObject(L, 1); 
		lua_pushnumber(L, (*s)->y);
		return 1;
	}

	static int spriteSetY(lua_State* L)
	{
		Sprite** s = checkValidSpriteObject(L, 1); 
		double value = luaL_checknumber(L, 2);
		(*s)->y = value;
		return 0;
	}

	static int spriteGetAngle(lua_State* L)
	{
		Sprite** s = checkValidSpriteObject(L, 1); 
		lua_pushnumber(L, (*s)->angle);
		return 1;
	}

	static int spriteSetAngle(lua_State* L)
	{
		Sprite** s = checkValidSpriteObject(L, 1); 
		double value = luaL_checknumber(L, 2);
		(*s)->angle = value;
		return 0;
	}

	static int spriteGetScale(lua_State* L)
	{
		Sprite** s = checkValidSpriteObject(L, 1); 
		lua_pushnumber(L, (*s)->scale);
		return 1;
	}

	static int spriteSetScale(lua_State* L)
	{
		Sprite** s = checkValidSpriteObject(L, 1); 
		double value = luaL_checknumber(L, 2);
		(*s)->scale = value;
		return 0;
	}

	static int spriteGetAlpha(lua_State* L)
	{
		Sprite** s = checkValidSpriteObject(L, 1); 
		lua_pushnumber(L, (*s)->alpha);
		return 1;
	}

	static int spriteSetAlpha(lua_State* L)
	{
		Sprite** s = checkValidSpriteObject(L, 1); 
		int value = luaL_checkint(L, 2);
		(*s)->alpha = value;
		return 0;
	}

	static int spriteGetTint(lua_State* L)
	{
		Sprite** s = checkValidSpriteObject(L, 1); 
		lua_pushnumber(L, (*s)->tint.value);
		return 1;
	}

	static int spriteSetTint(lua_State* L)
	{
		Sprite** s = checkValidSpriteObject(L, 1); 
		Color value = luaL_checkint(L, 2);
		(*s)->tint = value;
		return 0;
	}

	static const luaL_Reg spriteMembers[] = {
		{ "__index", spriteGetField },
		{ "__newindex",	spriteSetField },
		{ "__tostring",	spriteToString },
		{ "__gc", spriteGC },
		{ "blit", spriteBlit },
		{ "update", spriteUpdate },
		{ "setAnimation", spriteSwitchAnimation },
		{ "gettexture", spriteGetTexture },
		{ "getconfig", spriteGetConfig },
		{ "getx", spriteGetX },
		{ "setx", spriteSetX },
		{ "gety", spriteGetY },
		{ "sety", spriteSetY },
		{ "getangle", spriteGetAngle },
		{ "setangle", spriteSetAngle },
		{ "getscale", spriteGetScale },
		{ "setscale", spriteSetScale },
		{ "getalpha", spriteGetAlpha },
		{ "setalpha", spriteSetAlpha },
		{ "gettint", spriteGetTint },
		{ "settint", spriteSetTint },
		{ NULL, NULL }
	};

	void Script::initSpriteClass(lua_State* L)
	{
		luaL_newmetatable(L, "plum_sprite");
		// Duplicate the metatable on the stack.
		lua_pushvalue(L, -1);
		// metatable.__index = metatable
		lua_setfield(L, -2, "__index");
		// Put the members into the metatable.
		luaL_register(L, NULL, spriteMembers);
		lua_pop(L, 1);

		// Push plum namespace.
		lua_getglobal(L, "plum");

		// plum.sprite = <function spriteNew>
		lua_pushstring(L, "Sprite");
		lua_pushcfunction(L, spriteNew);
		lua_settable(L, -3);

		// Pop plum namespace.
		lua_pop(L, 1);
	}
}