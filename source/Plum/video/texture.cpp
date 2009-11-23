#include "../plum.h"

namespace Plum
{
	Texture::Texture(const std::string& filename)
	{
		init(filename.c_str());
	}

	Texture::Texture(const char* filename)
	{
		init(filename);
	}

	Texture::Texture(Image* image)
	{
		init(image);
	}

	Texture::~Texture()
	{
		glDeleteTextures(1, &textureID);
		delete textureImage;
	}

	void Texture::init(const char* filename)
	{
		Image* image = new Image(filename);
		init(image);
		delete image;
	}

	void Texture::init(Image* image)
	{
		target = GL_TEXTURE_2D;

		textureImage = new Image(nearestPowerOfTwo(image->occupiedWidth), nearestPowerOfTwo(image->occupiedHeight));
		textureImage->clear(0);
		image->blit<SoftOpaqueBlender>(0, 0, textureImage, SoftOpaqueBlender());
		textureImage->setClipRegion(0, 0, image->occupiedWidth - 1, image->occupiedHeight - 1);

		glGenTextures(1, &textureID);
		bind();

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);


		setImageWidth(image->occupiedWidth);
		setImageHeight(image->occupiedHeight);
		setTextureWidth(textureImage->width);
		setTextureHeight(textureImage->height);

		glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexImage2D(target, 0, GL_RGBA8, textureWidth, textureHeight,
			0, GL_RGBA, GL_UNSIGNED_BYTE, textureImage->data);
	}

	int Texture::getImageWidth()
	{
		return imageWidth;
	}

	int Texture::getImageHeight()
	{
		return imageHeight;
	}

	int Texture::getTextureWidth()
	{
		return textureWidth;
	}

	int Texture::getTextureHeight()
	{
		return textureHeight;
	}

	double Texture::getWidthRatio()
	{
		return widthRatio;
	}

	double Texture::getHeightRatio()
	{
		return heightRatio;
	}

	Image* Texture::getImage()
	{
		return textureImage;
	}

	void Texture::setImageWidth(int width)
	{
		this->textureImage->occupiedWidth = this->imageWidth = width;
		updateRatios();
	}

	void Texture::setImageHeight(int height)
	{
		this->textureImage->occupiedHeight = this->imageHeight = height;
		updateRatios();
	}

	void Texture::setTextureWidth(int width)
	{
		this->textureWidth = width;
		updateRatios();
	}

	void Texture::setTextureHeight(int height)
	{
		this->textureHeight = height;
		updateRatios();
	}

	void Texture::updateRatios()
	{
		widthRatio = (textureWidth != 0) ? ((double) imageWidth) / textureWidth : 0;
		heightRatio = (textureHeight != 0) ? ((double) imageHeight) / textureHeight : 0;
	}

	void Texture::refresh()
	{
		bind();
		glTexSubImage2D(target, 0, 0, 0, textureWidth, textureHeight,
			GL_RGBA, GL_UNSIGNED_BYTE, textureImage->data);
	}

	void Texture::bind()
	{
		glBindTexture(target, textureID); 
	}

	void Texture::blit(int x, int y, BlendMode mode, Color tint)
	{
		scaleBlitRegion(0, 0, imageWidth, imageHeight, x, y, imageWidth, imageHeight, mode, tint);
	}

	void Texture::scaleBlit(int x, int y, int width, int height, BlendMode mode, Color tint)
	{
		scaleBlitRegion(0, 0, imageWidth, imageHeight, x, y, width, height, mode, tint);
	}

	void Texture::blitRegion(int sourceX, int sourceY, int sourceX2, int sourceY2,
					int destX, int destY, BlendMode mode, Color tint)
	{
		scaleBlitRegion(sourceX, sourceY, sourceX2, sourceY2, destX, destY,
			PLUM_ABS(sourceX2 - sourceX) + 1, PLUM_ABS(sourceY2 - sourceY) + 1, mode, tint);
	}

	void Texture::scaleBlitRegion(int sourceX, int sourceY, int sourceX2, int sourceY2,
					int destX, int destY, int scaledWidth, int scaledHeight, BlendMode mode, Color tint)
	{
		ColorChannel r, g, b, a;
		getRGBA(tint, r, g, b, a);

		if(sourceX > sourceX2)
		{
			PLUM_SWAP(sourceX, sourceX2);
		}
		if(sourceY > sourceY2)
		{
			PLUM_SWAP(sourceY, sourceY2);
		}
		sourceX = PLUM_MIN(PLUM_MAX(0, sourceX), imageWidth - 1);
		sourceY = PLUM_MIN(PLUM_MAX(0, sourceY), imageHeight - 1);
		sourceX2 = PLUM_MIN(PLUM_MAX(0, sourceX2), imageWidth - 1);
		sourceY2 = PLUM_MIN(PLUM_MAX(0, sourceY2), imageHeight - 1);

		double regionS = ((double) sourceX + 0.5) / textureWidth;
		double regionT = ((double) sourceY + 0.5) / textureHeight;
		double regionS2 = ((double) sourceX2 + 0.5) / textureWidth;
		double regionT2 = ((double) sourceY2 + 0.5) / textureHeight;

		mode = (mode == BlendUnspecified) ? getBlendMode() : mode;
		useHardwareBlender(mode);

		glEnable(GL_TEXTURE_2D);
		glPushMatrix();
		bind();

		useHardwareColor(r, g, b, a);
		glTranslated(destX, destY, 0);

		glBegin(GL_QUADS);
			glTexCoord2d(regionS, regionT);
			glVertex2d(0.0, 0.0);
			glTexCoord2d(regionS, regionT2);
			glVertex2d(0.0, scaledHeight);
			glTexCoord2d(regionS2, regionT2);
			glVertex2d(scaledWidth, scaledHeight);
			glTexCoord2d(regionS2, regionT);
			glVertex2d(scaledWidth, 0.0);
		glEnd();
		glPopMatrix();
	}

	void Texture::rotateBlit(int x, int y, double angle, BlendMode mode, Color tint)
	{
		rotateScaleBlitRegion(0, 0, imageWidth, imageHeight, x, y, angle, 1.0, mode, tint);
	}

	void Texture::rotateScaleBlit(int x, int y, double angle, double scale, BlendMode mode, Color tint)
	{
		rotateScaleBlitRegion(0, 0, imageWidth, imageHeight, x, y, angle, scale, mode, tint);
	}

	void Texture::rotateBlitRegion(int sourceX, int sourceY, int sourceX2, int sourceY2,
					int destX, int destY, double angle, BlendMode mode, Color tint)
	{
		rotateScaleBlitRegion(sourceX, sourceY, sourceX2, sourceY2, destX, destY, angle, 1.0, mode, tint);
	}

	void Texture::rotateScaleBlitRegion(int sourceX, int sourceY, int sourceX2, int sourceY2,
					int destX, int destY, double angle, double scale, BlendMode mode, Color tint)
	{
		ColorChannel r, g, b, a;
		getRGBA(tint, r, g, b, a);

		if(sourceX > sourceX2)
		{
			PLUM_SWAP(sourceX, sourceX2);
		}
		if(sourceY > sourceY2)
		{
			PLUM_SWAP(sourceY, sourceY2);
		}
		sourceX = PLUM_MIN(PLUM_MAX(0, sourceX), imageWidth - 1);
		sourceY = PLUM_MIN(PLUM_MAX(0, sourceY), imageHeight - 1);
		sourceX2 = PLUM_MIN(PLUM_MAX(0, sourceX2), imageWidth - 1);
		sourceY2 = PLUM_MIN(PLUM_MAX(0, sourceY2), imageHeight - 1);

		double regionS = ((double) sourceX + 0.5) / textureWidth;
		double regionT = ((double) sourceY + 0.5) / textureHeight;
		double regionS2 = ((double) sourceX2 + 0.5) / textureWidth;
		double regionT2 = ((double) sourceY2 + 0.5) / textureHeight;

		double width = ((double) sourceX2 - sourceX) * scale;
		double height = ((double) sourceY2 - sourceY) * scale;

		mode = (mode == BlendUnspecified) ? getBlendMode() : mode;
		useHardwareBlender(mode);

		glEnable(GL_TEXTURE_2D);
		glPushMatrix();
		bind();

		useHardwareColor(r, g, b, a);
		glTranslated(destX + width / 2.0, destY + height / 2.0, 0.0);
		glRotated(angle, 0.0, 0.0, 1.0);
		glTranslated(-width / 2.0, -height / 2.0, 0.0);

		glBegin(GL_QUADS);
			glTexCoord2d(regionS, regionT);
			glVertex2d(0.0, 0.0);
			glTexCoord2d(regionS, regionT2);
			glVertex2d(0.0, height + 1);
			glTexCoord2d(regionS2, regionT2);
			glVertex2d(width + 1, height + 1);
			glTexCoord2d(regionS2, regionT);
			glVertex2d(width + 1, 0.0);
		glEnd();
		glPopMatrix();
	}

	// For when performance really matters, bind the texture and figure out blend modes ahead of time,
	// then call this in your loop. Especially important for tilemaps.
	// "raw" because it does less hand-holding. But could possibly be considered very hand-holdy.
	void Texture::rawBlitRegion(int sourceX, int sourceY, int sourceX2, int sourceY2,
					int destX, int destY, double angle, double scale)
	{
		sourceX = PLUM_MIN(PLUM_MAX(0, sourceX), imageWidth - 1);
		sourceY = PLUM_MIN(PLUM_MAX(0, sourceY), imageHeight - 1);
		sourceX2 = PLUM_MIN(PLUM_MAX(0, sourceX2), imageWidth - 1);
		sourceY2 = PLUM_MIN(PLUM_MAX(0, sourceY2), imageHeight - 1);

		double regionS = ((double) sourceX + 0.5) / textureWidth;
		double regionT = ((double) sourceY + 0.5) / textureHeight;
		double regionS2 = ((double) sourceX2 + 0.5) / textureWidth;
		double regionT2 = ((double) sourceY2 + 0.5) / textureHeight;

		double width = ((double) sourceX2 - sourceX) * scale;
		double height = ((double) sourceY2 - sourceY) * scale;

		glPushMatrix();

		glTranslated(destX + width / 2.0, destY + height / 2.0, 0.0);
		glRotated(angle, 0.0, 0.0, 1.0);
		glTranslated(-width / 2.0, -height / 2.0, 0.0);

		glBegin(GL_QUADS);
			glTexCoord2d(regionS, regionT);
			glVertex2d(0.0, 0.0);
			glTexCoord2d(regionS, regionT2);
			glVertex2d(0.0, height + 1);
			glTexCoord2d(regionS2, regionT2);
			glVertex2d(width + 1, height + 1);
			glTexCoord2d(regionS2, regionT);
			glVertex2d(width + 1, 0.0);
		glEnd();
		glPopMatrix();
	}
}
