#include <memory>

#include <GL/glfw3.h>

#include "../../core/image.h"
#include "../../core/transform.h"

namespace plum
{
    namespace
    {
        int align(int num)
        {
            if(num <= 0)
            {
                return 0;
            }
            else
            {
                --num;
                num |= num >> 1;
                num |= num >> 2;
                num |= num >> 4;
                num |= num >> 8;
                num |= num >> 16;
                ++num;

                return num;
            }
        }
    }

    Image::Image(const Canvas& source)
        : canvas_(source.getWidth(), source.getHeight(), align(source.getWidth()), align(source.getHeight()))
    {
        canvas_.clear(0);
        source.blit<BlendOpaque>(0, 0, canvas_);
        canvas_.setClipRegion(0, 0, source.getWidth() - 1, source.getHeight() - 1);

        glGenTextures(1, &textureID);
        bind();

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,
            canvas_.getTrueWidth(), canvas_.getTrueHeight(),
            0, GL_RGBA, GL_UNSIGNED_BYTE, canvas_.getData());
    }

    Image::~Image()
    {
        glDeleteTextures(1, &textureID);
    }

    Canvas& Image::canvas()
    {
        return canvas_;
    }

    void Image::refresh()
    {
        bind();
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0,
            canvas_.getTrueWidth(), canvas_.getTrueHeight(),
            GL_RGBA, GL_UNSIGNED_BYTE, canvas_.getData());
    }

    void Image::bind()
    {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textureID); 
    }

    void Image::blit(int x, int y, BlendMode mode)
    {
        scaleBlitRegion(0, 0, canvas_.getWidth(), canvas_.getHeight(), x, y, canvas_.getWidth(), canvas_.getHeight(), mode);
    }

    void Image::scaleBlit(int x, int y, int width, int height, BlendMode mode)
    {
        scaleBlitRegion(0, 0, canvas_.getWidth(), canvas_.getHeight(), x, y, width, height, mode);
    }

    void Image::blitRegion(int sourceX, int sourceY, int sourceX2, int sourceY2,
                    int destX, int destY, BlendMode mode)
    {
        scaleBlitRegion(sourceX, sourceY, sourceX2, sourceY2, destX, destY,
            std::abs(sourceX2 - sourceX) + 1, std::abs(sourceY2 - sourceY) + 1, mode);
    }

    void Image::scaleBlitRegion(int sourceX, int sourceY, int sourceX2, int sourceY2,
                    int destX, int destY, int scaledWidth, int scaledHeight, BlendMode mode)
    {
        if(sourceX > sourceX2)
        {
            std::swap(sourceX, sourceX2);
        }
        if(sourceY > sourceY2)
        {
            std::swap(sourceY, sourceY2);
        }
        sourceX = std::min(std::max(0, sourceX), canvas_.getWidth() - 1);
        sourceY = std::min(std::max(0, sourceY), canvas_.getHeight() - 1);
        sourceX2 = std::min(std::max(0, sourceX2), canvas_.getWidth() - 1);
        sourceY2 = std::min(std::max(0, sourceY2), canvas_.getHeight() - 1);

        double regionS = (double(sourceX) + 0.5) / canvas_.getTrueWidth();
        double regionT = (double(sourceY) + 0.5) / canvas_.getTrueHeight();
        double regionS2 = (double(sourceX2) + 0.5) / canvas_.getTrueWidth();
        double regionT2 = (double(sourceY2) + 0.5) / canvas_.getTrueHeight();

        useHardwareBlender(mode);

        glPushMatrix();
        bind();

        useHardwareColor(255, 255, 255, 255);
        glTranslated(destX, destY, 0);


        const GLdouble vertexArray[] = {
            0.0, 0.0,
            0.0, scaledHeight,
            scaledWidth, scaledHeight,
            scaledWidth, 0.0,
        };

        const GLdouble textureArray[] = {
            regionS, regionT,
            regionS, regionT2,
            regionS2, regionT2,
            regionS2, regionT,
        };


        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);

        glVertexPointer(2, GL_DOUBLE, 0, vertexArray);
        glTexCoordPointer(2, GL_DOUBLE, 0, textureArray);
        glDrawArrays(GL_QUADS, 0, 4);
        
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);

        glPopMatrix();
    }

    void Image::rotateBlit(int x, int y, double angle, BlendMode mode)
    {
        rotateScaleBlitRegion(0, 0, canvas_.getWidth(), canvas_.getHeight(), x, y, angle, 1.0, mode);
    }

    void Image::rotateScaleBlit(int x, int y, double angle, double scale, BlendMode mode)
    {
        rotateScaleBlitRegion(0, 0, canvas_.getWidth(), canvas_.getHeight(), x, y, angle, scale, mode);
    }

    void Image::rotateBlitRegion(int sourceX, int sourceY, int sourceX2, int sourceY2,
                    int destX, int destY, double angle, BlendMode mode)
    {
        rotateScaleBlitRegion(sourceX, sourceY, sourceX2, sourceY2, destX, destY, angle, 1.0, mode);
    }

    void Image::rotateScaleBlitRegion(int sourceX, int sourceY, int sourceX2, int sourceY2,
                    int destX, int destY, double angle, double scale, BlendMode mode)
    {
        if(sourceX > sourceX2)
        {
            std::swap(sourceX, sourceX2);
        }
        if(sourceY > sourceY2)
        {
            std::swap(sourceY, sourceY2);
        }
        sourceX = std::min(std::max(0, sourceX), canvas_.getWidth() - 1);
        sourceY = std::min(std::max(0, sourceY), canvas_.getHeight() - 1);
        sourceX2 = std::min(std::max(0, sourceX2), canvas_.getWidth() - 1);
        sourceY2 = std::min(std::max(0, sourceY2), canvas_.getHeight() - 1);

        double regionS = (double(sourceX) + 0.5) / canvas_.getTrueWidth();
        double regionT = (double(sourceY) + 0.5) / canvas_.getTrueHeight();
        double regionS2 = (double(sourceX2) + 0.5) / canvas_.getTrueWidth();
        double regionT2 = (double(sourceY2) + 0.5) / canvas_.getTrueHeight();

        double width = double(sourceX2 - sourceX) * scale;
        double height = double(sourceY2 - sourceY) * scale;

        useHardwareBlender(mode);

        glPushMatrix();
        bind();

        useHardwareColor(255, 255, 255, 255);
        glTranslated(destX + width / 2.0, destY + height / 2.0, 0.0);
        glRotated(angle, 0.0, 0.0, 1.0);
        glTranslated(-width / 2.0, -height / 2.0, 0.0);

        const GLdouble vertexArray[] = {
            0.0, 0.0,
            0.0, height + 1.0,
            width + 1.0, height + 1.0,
            width + 1.0, 0.0,
        };

        const GLdouble textureArray[] = {
            regionS, regionT,
            regionS, regionT2,
            regionS2, regionT2,
            regionS2, regionT,
        };

        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);

        glVertexPointer(2, GL_DOUBLE, 0, vertexArray);
        glTexCoordPointer(2, GL_DOUBLE, 0, textureArray);
        glDrawArrays(GL_QUADS, 0, 4);
        
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);

        glPopMatrix();
    }

    // For when performance really matters, bind the texture and figure out blend modes ahead of time,
    // then call this in your loop. Especially important for tilemaps.
    // "raw" because it does less hand-holding. But could possibly be considered very hand-holdy.
    void Image::rawBlitRegion(int sourceX, int sourceY, int sourceX2, int sourceY2,
                    int destX, int destY, double angle, double scale)
    {
        sourceX = std::min(std::max(0, sourceX), canvas_.getWidth() - 1);
        sourceY = std::min(std::max(0, sourceY), canvas_.getHeight() - 1);
        sourceX2 = std::min(std::max(0, sourceX2), canvas_.getWidth() - 1);
        sourceY2 = std::min(std::max(0, sourceY2), canvas_.getHeight() - 1);

        double regionS = (double(sourceX) + 0.5) / canvas_.getTrueWidth();
        double regionT = (double(sourceY) + 0.5) / canvas_.getTrueHeight();
        double regionS2 = (double(sourceX2) + 0.5) / canvas_.getTrueWidth();
        double regionT2 = (double(sourceY2) + 0.5) / canvas_.getTrueHeight();

        double width = double(sourceX2 - sourceX) * scale;
        double height = double(sourceY2 - sourceY) * scale;

        glPushMatrix();

        glTranslated(destX + width / 2.0, destY + height / 2.0, 0.0);
        glRotated(angle, 0.0, 0.0, 1.0);
        glTranslated(-width / 2.0, -height / 2.0, 0.0);

        const GLdouble vertexArray[] = {
            0.0, 0.0,
            0.0, height + 1.0,
            width + 1.0, height + 1.0,
            width + 1.0, 0.0,
        };

        const GLdouble textureArray[] = {
            regionS, regionT,
            regionS, regionT2,
            regionS2, regionT2,
            regionS2, regionT,
        };

        glVertexPointer(2, GL_DOUBLE, 0, vertexArray);
        glTexCoordPointer(2, GL_DOUBLE, 0, textureArray);
        glDrawArrays(GL_QUADS, 0, 4);

        glPopMatrix();
    }

    // Draws image, based on a transformation object (saves on complex arg passing)
    void Image::transformBlit(Transform* transform)
    {
        double sourceX, sourceY, sourceX2, sourceY2;
        uint8_t r, g, b, a;
        transform->tint.channels(r, g, b, a);

        if(transform->clip)
        {
            sourceX = std::min<double>(std::max(0.0, transform->clip->x), canvas_.getWidth() - 1);
            sourceY = std::min<double>(std::max(0.0, transform->clip->y), canvas_.getHeight() - 1);
            sourceX2 = std::min<double>(sourceX + transform->clip->width, canvas_.getWidth()) - 1;
            sourceY2 = std::min<double>(sourceY + transform->clip->height, canvas_.getHeight()) - 1;
        }
        else
        {
            sourceX = sourceY = 0;
            sourceX2 = canvas_.getWidth() - 1;
            sourceY2 = canvas_.getHeight() - 1;
        }

        double regionS = (double(sourceX) + 0.5) / canvas_.getTrueWidth();
        double regionT = (double(sourceY) + 0.5) / canvas_.getTrueHeight();
        double regionS2 = (double(sourceX2) + 0.5) / canvas_.getTrueWidth();
        double regionT2 = (double(sourceY2) + 0.5) / canvas_.getTrueHeight();

        double width = double(sourceX2 - sourceX);
        double height = double(sourceY2 - sourceY);

        useHardwareBlender(transform->mode);

        glPushMatrix();
        bind();

        useHardwareColor(r, g, b, a);
        glTranslated(transform->position->x + transform->pivot->x, transform->position->y + transform->pivot->y, 0.0);
        glScaled(transform->scale->x * (1 - transform->mirror * 2), transform->scale->y, 0.0);
        glRotated(transform->angle, 0.0, 0.0, 1.0);
        glTranslated(-transform->pivot->x, -transform->pivot->y, 0.0);

        const GLdouble vertexArray[] = {
            0.0, 0.0,
            0.0, height + 1.0,
            width + 1.0, height + 1.0,
            width + 1.0, 0.0,
        };

        const GLdouble textureArray[] = {
            regionS, regionT,
            regionS, regionT2,
            regionS2, regionT2,
            regionS2, regionT,
        };

        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);

        glVertexPointer(2, GL_DOUBLE, 0, vertexArray);
        glTexCoordPointer(2, GL_DOUBLE, 0, textureArray);
        glDrawArrays(GL_QUADS, 0, 4);
        
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);

        glPopMatrix();
    }
}
