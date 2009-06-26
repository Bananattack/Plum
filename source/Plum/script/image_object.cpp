#include "../plum.h"

namespace Plum
{
	namespace Script
	{
		static ImageWrapper* checkValidImageObject(lua_State* L, int index)
		{
			return (ImageWrapper*) luaL_checkudata(L, index, "plum_image");
		}

		static int imageNew(lua_State* L)
		{
			if(lua_isstring(L, 1))
			{
				const char* filename = lua_tostring(L, 1);

				ImageWrapper* img = (ImageWrapper*) lua_newuserdata(L, sizeof(ImageWrapper));
				luaL_getmetatable(L, "plum_image");
				lua_setmetatable(L, -2);

				img->image = new Image(filename);
				img->canDelete = true;
				
				return 1;
			}
			else if(lua_isnumber(L, 1) && lua_isnumber(L, 2))
			{
				int w = lua_tointeger(L, 1);
				int h = lua_tointeger(L, 2);

				ImageWrapper* img = (ImageWrapper*) lua_newuserdata(L, sizeof(ImageWrapper));
				luaL_getmetatable(L, "plum_image");
				lua_setmetatable(L, -2);

				img->image = new Image(w, h);
				img->canDelete = true;
				
				return 1;
			}
			else
			{
				luaL_error(L, "Attempt to call plum.Image constructor with invalid argument types.\nMust be (string filename) or (int w, int h).");
				return 0;
			}
		}

		void imagePushForTexture(lua_State* L, Texture* tex)
		{
			ImageWrapper* img = (ImageWrapper*) lua_newuserdata(L, sizeof(ImageWrapper));
			luaL_getmetatable(L, "plum_image");
			lua_setmetatable(L, -2);

			img->image = tex->getImage();
			img->canDelete = false;
		}

		static int imageGC(lua_State* L)
		{
			ImageWrapper* img = checkValidImageObject(L, 1);

			// Only delete if it doesn't belong to a texture of some sort.
			if(img->canDelete)
			{
				delete img->image;
			}

			return 0;
		}

		SCRIPT_OBJ_GETTER(imageGetField, ImageWrapper*, "plum_image")
		SCRIPT_OBJ_SETTER(imageSetField, ImageWrapper*, "plum_image")

		static int imageToString(lua_State* L)
		{
			checkValidImageObject(L, 1);
			lua_pushstring(L, "(plum.Image object)");
			return 1;
		}

		static int imageBlit(lua_State* L)
		{
			ImageWrapper* img = checkValidImageObject(L, 1);
			int x = luaL_checkint(L, 2);
			int y = luaL_checkint(L, 3);
			ImageWrapper* dest = checkValidImageObject(L, 4);
			BlendMode mode = (BlendMode) luaL_optint(L, 5, getBlendMode());
			
			switch(mode)
			{
				case BlendOpaque:
					img->image->blit<SoftOpaqueBlender>(x, y, dest->image, SoftOpaqueBlender());
					break;
				case BlendMerge:
					img->image->blit<SoftMergeBlender>(x, y, dest->image, SoftMergeBlender());
					break;
				case BlendPreserve:
					img->image->blit<SoftPreserveBlender>(x, y, dest->image, SoftPreserveBlender());
					break;
				case BlendAdd:
					img->image->blit<SoftAddBlender>(x, y, dest->image, SoftAddBlender());
					break;
				case BlendSubtract:
					img->image->blit<SoftSubtractBlender>(x, y, dest->image, SoftSubtractBlender());
					break;
			}
			return 0;
		}

		static int imageReplaceColor(lua_State* L)
		{
			ImageWrapper* img = checkValidImageObject(L, 1);
			int find = luaL_checkint(L, 2);
			int replace = luaL_checkint(L, 3);
			img->image->replaceColor(find, replace);
			return 0;
		}

		static int imageFlip(lua_State* L)
		{
			ImageWrapper* img = checkValidImageObject(L, 1);
			bool h = lua_toboolean(L, 2) != 0;
			bool v = lua_toboolean(L, 3) != 0;
			img->image->flip(h, v);
			return 0;
		}

		static int imageGetWidth(lua_State* L)
		{
			ImageWrapper* img = checkValidImageObject(L, 1);
			lua_pushnumber(L, img->image->width);

			return 1;
		}

		static int imageGetHeight(lua_State* L)
		{
			ImageWrapper* img = checkValidImageObject(L, 1);
			lua_pushnumber(L, img->image->height);

			return 1;
		}

		const luaL_Reg imageMembers[] = {
			{ "__index", imageGetField },
			{ "__newindex",	imageSetField },
			{ "__tostring",	imageToString },
			{ "__gc", imageGC },
			{ "blit", imageBlit },
			{ "replaceColor", imageReplaceColor },
			{ "flip", imageFlip },
			{ "getwidth", imageGetWidth },
			{ "getheight", imageGetHeight },
			{ NULL, NULL }
		};

		void initImageClass(lua_State* L)
		{
			luaL_newmetatable(L, "plum_image");
			// Duplicate the metatable on the stack.
			lua_pushvalue(L, -1);
			// metatable.__index = metatable
			lua_setfield(L, -2, "__index");
			// Put the members into the metatable.
			luaL_register(L, NULL, imageMembers);
			lua_pop(L, 1);

			// Push plum namespace.
			lua_getglobal(L, "plum");

			// plum.image = <function imageNew>
			lua_pushstring(L, "Image");
			lua_pushcfunction(L, imageNew);
			lua_settable(L, -3);

			// Pop plum namespace.
			lua_pop(L, 1);
		}
	}
}