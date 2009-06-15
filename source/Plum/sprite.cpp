#include "plum.h"

namespace Plum
{
	Sprite::Sprite(double x, double y, const char* filename)
	{
		init(x, y, filename);
	}

	Sprite::Sprite(double x, double y, std::string filename)
	{
		init(x, y, filename);
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

	void Sprite::init(double x, double y, std::string filename)
	{
		FILE* f = fopen(filename.c_str(), "r");
		FormatTable* t = ReadFormat(f, filename, "sprite", SPRITE_VERSION);
		fclose(f);

		if(!t->hasNode("image_filename"))
		{
			throw FormatParseException(filename + ": Didn't find an 'image_filename' key.\n");
		}
		texture = new Texture(t->getString("image_filename"));

		if(!t->hasNode("frame_width") || !isStringNumeric(t->getString("frame_width")))
		{
			throw FormatParseException(filename
				+ ": Didn't find an 'frame_width' key, or it was invalid.\n");
		}
		frameWidth = stringToInteger(t->getString("frame_width"));

		if(!t->hasNode("frame_height") || !isStringNumeric(t->getString("frame_height")))
		{
			throw FormatParseException(filename
				+ ": Didn't find an 'frame_height' key, or it was invalid.\n");
		}
		frameHeight = stringToInteger(t->getString("frame_height"));

		if(!t->hasNode("frame_columns") || !isStringNumeric(t->getString("frame_columns")))
		{
			throw FormatParseException(filename
				+ ": Didn't find an 'frame_columns' key, or it was invalid.\n");
		}
		frameColumns = stringToInteger(t->getString("frame_columns"));

		if(t->hasNode("frame_padding"))
		{
			if(!isStringNumeric(t->getString("frame_padding")))
			{
				throw FormatParseException(filename
					+ ": 'frame_padding' key  was invalid.\n");
			}
			framePadding = stringToInteger(t->getString("frame_padding"));
		}
		else
		{
			framePadding = 1;
		}

		if(!t->hasNode("hotspot_x") || !isStringNumeric(t->getString("hotspot_x")))
		{
			throw FormatParseException(filename
				+ ": Didn't find an 'hotspot_x' key, or it was invalid.\n");
		}
		hotspotX = stringToInteger(t->getString("hotspot_x"));

		if(!t->hasNode("hotspot_y") || !isStringNumeric(t->getString("hotspot_y")))
		{
			throw FormatParseException(filename
				+ ": Didn't find an 'hotspot_y' key, or it was invalid.\n");
		}
		hotspotY = stringToInteger(t->getString("hotspot_y"));

		if(!t->hasNode("hotspot_width") || !isStringNumeric(t->getString("hotspot_width")))
		{
			throw FormatParseException(filename
				+ ": Didn't find an 'hotspot_width' key, or it was invalid.\n");
		}
		hotspotWidth = stringToInteger(t->getString("hotspot_width"));

		if(!t->hasNode("hotspot_height") || !isStringNumeric(t->getString("hotspot_height")))
		{
			throw FormatParseException(filename
				+ ": Didn't find an 'hotspot_height' key, or it was invalid.\n");
		}
		hotspotHeight = stringToInteger(t->getString("hotspot_height"));

		if(t->hasNode("animations"))
		{
			FormatTable* animationTable = t->getTable("animations");
			std::map<std::string, FormatNode*>::iterator i;
			
			for (i = animationTable->items.begin(); i != animationTable->items.end(); ++i)
			{
				FormatTable* strandTable = animationTable->getTable(i->first);
				std::map<std::string, FormatNode*>::iterator j;
				AnimationInfo* info = new AnimationInfo();

				// Ignore 'private' table data.
				if(i->first.find("__", 0) == 0)
				{
					continue;
				}
				for(j = strandTable->items.begin(); j != strandTable->items.end(); ++j)
				{
					// Ignore 'private' table data.
					if(j->first.find("__", 0) == 0)
					{
						continue;
					}
					info->scripts[j->first] = strandTable->getString(j->first);
				}
				animationInfo[i->first] = info;
			}
		}
		delete t;

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