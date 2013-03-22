#include <memory>

#include <GL/glfw3.h>

#include "../../core/image.h"
#include "../../core/sheet.h"
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


    class Image::Impl
    {
        public:
            Impl(const Canvas& source)
                : canvas(source.getWidth(), source.getHeight(), align(source.getWidth()), align(source.getHeight()))
            {
                canvas.clear(0);
                source.blit<BlendOpaque>(0, 0, canvas);
                canvas.setClipRegion(0, 0, source.getWidth() - 1, source.getHeight() - 1);

                glGenTextures(1, &textureID);
                bind();

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,
                    canvas.getTrueWidth(), canvas.getTrueHeight(),
                    0, GL_RGBA, GL_UNSIGNED_BYTE, canvas.getData());
            }

            ~Impl()
            {
                glDeleteTextures(1, &textureID);
            }


            void bind()
            {
                glEnable(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D, textureID); 
            }

            // A backend software canvas that this image's raw texture copies.
            // Useful if the textures need to be refreshed later.
            Canvas canvas;
            // The GL texture ID
            unsigned int textureID;
    };

    Image::Image(const Canvas& source)
        : impl(new Impl(source))
    {
    }

    Image::~Image()
    {
    }

    Canvas& Image::canvas()
    {
        return impl->canvas;
    }

    const Canvas& Image::canvas() const
    {
        return impl->canvas;
    }

    void Image::refresh()
    {
        bind();
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0,
            impl->canvas.getTrueWidth(), impl->canvas.getTrueHeight(),
            GL_RGBA, GL_UNSIGNED_BYTE, impl->canvas.getData());
    }

    void Image::startBatch()
    {
        glColor4ub(255, 255, 255, getOpacity());
        glEnable(GL_TEXTURE_2D);
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    }

    void Image::endBatch()
    {
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }

    void Image::bind()
    {
        impl->bind();
    }

    void Image::draw(int x, int y)
    {
        double regionS = 0;
        double regionT = 0;
        double regionS2 = double(impl->canvas.getWidth()) / impl->canvas.getTrueWidth();
        double regionT2 = double(impl->canvas.getHeight()) / impl->canvas.getTrueHeight();

        const GLdouble vertexArray[] = {
            x, y,
            x, y + impl->canvas.getHeight(),
            x + impl->canvas.getWidth(), y + impl->canvas.getHeight(),
            x + impl->canvas.getWidth(), y,
        };

        const GLdouble textureArray[] = {
            regionS, regionT,
            regionS, regionT2,
            regionS2, regionT2,
            regionS2, regionT,
        };

        useHardwareBlender(BlendPreserve);
        glColor4ub(255, 255, 255, getOpacity());
        bind();
        
        startBatch();
        glVertexPointer(2, GL_DOUBLE, 0, vertexArray);
        glTexCoordPointer(2, GL_DOUBLE, 0, textureArray);
        glDrawArrays(GL_QUADS, 0, 4);
        endBatch();
    }

    void Image::draw(int x, int y, const Transform& transform)
    {
        uint8_t r, g, b, a;
        transform.tint.channels(r, g, b, a);

        double regionS = 0;
        double regionT = 0;
        double regionS2 = double(impl->canvas.getWidth()) / impl->canvas.getTrueWidth();
        double regionT2 = double(impl->canvas.getHeight()) / impl->canvas.getTrueHeight();

        double width = double(impl->canvas.getWidth());
        double height = double(impl->canvas.getHeight());

        useHardwareBlender(transform.mode);
        glColor4ub(r, g, b, a * getOpacity() / 255);

        glPushMatrix();
        bind();

        glTranslated(x, y, 0.0);
        glTranslated(floor(width / 2), floor(height / 2), 0.0);

        glScaled(transform.scaleX * (1 - transform.mirror * 2), transform.scaleY, 0.0);
        glRotated(transform.angle, 0.0, 0.0, 1.0);
        glTranslated(-floor(width / 2), -floor(height / 2), 0.0);

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

        startBatch();
        glVertexPointer(2, GL_DOUBLE, 0, vertexArray);
        glTexCoordPointer(2, GL_DOUBLE, 0, textureArray);
        glDrawArrays(GL_QUADS, 0, 4);
        endBatch();

        glPopMatrix();
    }

    void Image::drawFrame(const Sheet& sheet, int f, int x, int y)
    {
        useHardwareBlender(BlendPreserve);
        glColor4ub(255, 255, 255, getOpacity());
        bind();

        startBatch();
        drawFrameRaw(sheet, f, x, y);
        endBatch();
    }

    void Image::drawFrame(const Sheet& sheet, int f, int x, int y, const Transform& transform)
    {
        uint8_t r, g, b, a;
        transform.tint.channels(r, g, b, a);

        int sourceX, sourceY;

        if(!sheet.getFrame(f, sourceX, sourceY))
        {
            return;
        }

        double regionS = (double(sourceX)) / impl->canvas.getTrueWidth();
        double regionT = (double(sourceY)) / impl->canvas.getTrueHeight();
        double regionS2 = (double(sourceX + sheet.getWidth())) / impl->canvas.getTrueWidth();
        double regionT2 = (double(sourceY + sheet.getHeight())) / impl->canvas.getTrueHeight();

        double width = double(sheet.getWidth());
        double height = double(sheet.getHeight());

        useHardwareBlender(transform.mode);
        glColor4ub(r, g, b, a * getOpacity() / 255);

        glPushMatrix();
        bind();

        glTranslated(x, y, 0.0);
        glTranslated(floor(width / 2), floor(height / 2), 0.0);

        glScaled(transform.scaleX * (1 - transform.mirror * 2), transform.scaleY, 0.0);
        glRotated(transform.angle, 0.0, 0.0, 1.0);
        glTranslated(-floor(width / 2), -floor(height / 2), 0.0);

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

        startBatch();
        glVertexPointer(2, GL_DOUBLE, 0, vertexArray);
        glTexCoordPointer(2, GL_DOUBLE, 0, textureArray);
        glDrawArrays(GL_QUADS, 0, 4);
        endBatch();

        glPopMatrix();
    }

    void Image::drawFrameRaw(const Sheet& sheet, int f, int x, int y)
    {
        int sourceX, sourceY;

        if(!sheet.getFrame(f, sourceX, sourceY))
        {
            return;
        }

        double regionS = (double(sourceX)) / impl->canvas.getTrueWidth();
        double regionT = (double(sourceY)) / impl->canvas.getTrueHeight();
        double regionS2 = (double(sourceX + sheet.getWidth())) / impl->canvas.getTrueWidth();
        double regionT2 = (double(sourceY + sheet.getHeight())) / impl->canvas.getTrueHeight();

        const GLdouble vertexArray[] = {
            x, y,
            x, y + sheet.getHeight(),
            x + sheet.getWidth(), y + sheet.getHeight(),
            x + sheet.getWidth(), y,
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
    }
}
