#include "../plum.h"

namespace Plum
{
	Sprite::Sprite(double x, double y, const char* filename, lua_State* state)
	{
		init(x, y, filename, state);
	}

	Sprite::Sprite(double x, double y, std::string filename, lua_State* state)
	{
		init(x, y, filename, state);
	}

	Sprite::~Sprite()
	{
		delete texture;
		delete parser;

		std::map<std::string, AnimationInfo*>::iterator i;
		for(i = animationInfo.begin(); i != animationInfo.end(); ++i)
		{
			delete i->second;
		}
	}

	void Sprite::init(double x, double y, std::string filename, lua_State* state)
	{
		cfg.init(filename, "sprite", state);

		texture = new Texture(Path::getDirectory(filename) + cfg.getStringValue("image_filename"));

		frameWidth = cfg.getIntValue("frame_width");
		frameHeight = cfg.getIntValue("frame_height");

		frameColumns = cfg.getIntValue("frame_columns");
		framePadding = cfg.hasValue("frame_padding") ? cfg.getIntValue("frame_padding") : 1;

		hotspotX = cfg.getIntValue("hotspot_x");
		hotspotY = cfg.getIntValue("hotspot_y");
		hotspotWidth = cfg.getIntValue("hotspot_width");
		hotspotHeight = cfg.getIntValue("hotspot_height");

		lua_pushstring(cfg.lua, "animations");
		lua_rawget(cfg.lua, -2);
		if(!lua_isnil(cfg.lua, -1))
		{
			if(!lua_istable(cfg.lua, -1))
			{
				throw Engine::Exception("Error in '" + filename + "':\r\nKey 'animations' is defined, but is not a valid table value.");
			}
			else
			{

				lua_pushnil(cfg.lua);
				while (lua_next(cfg.lua, -2) != 0)
				{
					if(!lua_isstring(cfg.lua, -2) || !lua_istable(cfg.lua, -1))
					{
						throw Engine::Exception("Error in '" + filename + "':\r\nFound invalid animation group entry.");
					}

					AnimationInfo* info = new AnimationInfo();
					lua_pushnil(cfg.lua);
					while (lua_next(cfg.lua, -2) != 0)
					{
						if(!lua_isstring(cfg.lua, -2) || !lua_isstring(cfg.lua, -1))
						{
							throw Engine::Exception("Error in '" + filename + "':\r\nFound invalid animation strand entry.");
						}
						std::string strandName = lua_tostring(cfg.lua, -2);
						std::string value = lua_tostring(cfg.lua, -1);
						info->scripts[strandName] = value;

						lua_pop(cfg.lua, 1);
					}

					std::string groupName = lua_tostring(cfg.lua, -2);
					animationInfo[groupName] = info;

					lua_pop(cfg.lua, 1);
				}
			}
		}
		lua_pop(cfg.lua, 1);

		parser = new AnimationParser();
		
		this->renderPriority = 0;
		this->x = x;
		this->y = y;
		this->angle = 0.0;
		this->scale = 1.0;
		this->direction = "";
		this->visible = true;
		this->frameIndex = 0;
		this->tint = Color::White;
		this->blendMode = BlendPreserve;
		this->alpha = 255;
	}

	void Sprite::blit()
	{
		if(!visible)
		{
			return;
		}
		int f = parser->getFrame();
		int fx = (f % frameColumns) * (frameWidth + framePadding) + framePadding;
		int fy = (f / frameColumns) * (frameHeight + framePadding) + framePadding;
		texture->rotateScaleBlitRegion(fx, fy, fx + frameWidth - 1, fy + frameHeight - 1,
				(int) x, (int) y, angle, scale, blendMode, tint);
	}

	void Sprite::setAnimation(std::string strand, std::string direction)
	{
		std::map<std::string, AnimationInfo*>::iterator i = animationInfo.find(strand);
		if(i != animationInfo.end())
		{
			std::map<std::string, std::string>::iterator j;
			j = i->second->scripts.find(direction);
			if(j != i->second->scripts.end())
			{
				parser->setScript(j->second);
			}
		}
	}

	void Sprite::update()
	{
		parser->update();
	}

}