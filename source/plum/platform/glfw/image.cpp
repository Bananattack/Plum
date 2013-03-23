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
        impl->bind();
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0,
            impl->canvas.getTrueWidth(), impl->canvas.getTrueHeight(),
            GL_RGBA, GL_UNSIGNED_BYTE, impl->canvas.getData());
    }

    void Image::draw(int x, int y, Screen& dest)
    {
        dest.bind(*this);
        drawRaw(x, y);
        dest.unbind();
    }

    void Image::draw(int x, int y, const Transform& transform, Screen& dest)
    {
        dest.bind(*this, transform, x, y, impl->canvas.getWidth(), impl->canvas.getHeight());
        drawRaw(0, 0);
        dest.unbind();
    }

    void Image::drawFrame(const Sheet& sheet, int f, int x, int y, Screen& dest)
    {
        dest.bind(*this);
        drawFrameRaw(sheet, f, x, y);
        dest.unbind();
    }

    void Image::drawFrame(const Sheet& sheet, int f, int x, int y, const Transform& transform, Screen& dest)
    {
        dest.bind(*this, transform, x, y, sheet.getWidth(), sheet.getHeight());
        drawFrameRaw(sheet, f, 0, 0);
        dest.unbind();
    }

    void Image::bindRaw()
    {
        impl->bind();
    }

    void Image::drawRaw(int x, int y)
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

        glVertexPointer(2, GL_DOUBLE, 0, vertexArray);
        glTexCoordPointer(2, GL_DOUBLE, 0, textureArray);
        glDrawArrays(GL_QUADS, 0, 4);
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
