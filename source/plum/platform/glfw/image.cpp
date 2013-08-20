#include <memory>

#include "engine.h"
#include "../../core/image.h"
#include "../../core/sheet.h"
#include "../../core/screen.h"
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
                source.blit<BlendMode::Opaque>(0, 0, canvas);
                canvas.setClipRegion(0, 0, source.getWidth() - 1, source.getHeight() - 1);

                glGenTextures(1, &texture);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, texture);

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,
                    canvas.getTrueWidth(), canvas.getTrueHeight(),
                    0, GL_RGBA, GL_UNSIGNED_BYTE, canvas.getData());

                glGenBuffers(1, &vbo);
            }

            ~Impl()
            {
                glDeleteTextures(1, &texture);
                glDeleteBuffers(1, &vbo);
            }

            GLuint texture;
            GLuint vbo;
            Canvas canvas;
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
        glBindTexture(GL_TEXTURE_2D, impl->texture);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0,
            impl->canvas.getTrueWidth(), impl->canvas.getTrueHeight(),
            GL_RGBA, GL_UNSIGNED_BYTE, impl->canvas.getData());
    }

    void Image::draw(int x, int y, Screen& dest)
    {
        dest.bindImage(*this);
        dest.applyTransform();
        drawRaw(x, y, dest);
        dest.unbindImage();
    }

    void Image::draw(int x, int y, const Transform& transform, Screen& dest)
    {
        dest.bindImage(*this);
        dest.applyTransform(transform, x, y, impl->canvas.getWidth(), impl->canvas.getHeight());
        drawRaw(0, 0, dest);
        dest.unbindImage();
    }

    void Image::drawFrame(const Sheet& sheet, int f, int x, int y, Screen& dest)
    {
        dest.bindImage(*this);
        dest.applyTransform();
        drawFrameRaw(sheet, f, x, y, dest);
        dest.unbindImage();
    }

    void Image::drawFrame(const Sheet& sheet, int f, int x, int y, const Transform& transform, Screen& dest)
    {
        dest.bindImage(*this);
        dest.applyTransform(transform, x, y, sheet.getWidth(), sheet.getHeight());
        drawFrameRaw(sheet, f, 0, 0, dest);
        dest.unbindImage();
    }

    void Image::bindRaw()
    {
        glBindTexture(GL_TEXTURE_2D, impl->texture);
    }

    void Image::drawRaw(int x, int y, Screen& dest)
    {
        float regionS = 0;
        float regionT = 0;
        float regionS2 = float(impl->canvas.getWidth()) / impl->canvas.getTrueWidth();
        float regionT2 = float(impl->canvas.getHeight()) / impl->canvas.getTrueHeight();

        const GLfloat vertices[] = {
            x, y, regionS, regionT,
            x, y + impl->canvas.getHeight(), regionS, regionT2,
            x + impl->canvas.getWidth(), y + impl->canvas.getHeight(), regionS2, regionT2,
            x + impl->canvas.getWidth(), y + impl->canvas.getHeight(), regionS2, regionT2,
            x + impl->canvas.getWidth(), y, regionS2, regionT,
            x, y, regionS, regionT,
        };

        auto& e(dest.engine().impl);
        glBindBuffer(GL_ARRAY_BUFFER, impl->vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);
        glVertexAttribPointer(e->xyAttribute, 2, GL_FLOAT, false, 4 * sizeof(float), (void*) 0);
        glVertexAttribPointer(e->uvAttribute, 2, GL_FLOAT, false, 4 * sizeof(float), (void*)(2 * sizeof(float)));
        glEnableVertexAttribArray(e->xyAttribute);
        glEnableVertexAttribArray(e->uvAttribute);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    void Image::drawFrameRaw(const Sheet& sheet, int f, int x, int y, Screen& dest)
    {
        int sourceX, sourceY;

        if(!sheet.getFrame(f, sourceX, sourceY))
        {
            return;
        }

        float regionS = (float(sourceX)) / impl->canvas.getTrueWidth();
        float regionT = (float(sourceY)) / impl->canvas.getTrueHeight();
        float regionS2 = (float(sourceX + sheet.getWidth())) / impl->canvas.getTrueWidth();
        float regionT2 = (float(sourceY + sheet.getHeight())) / impl->canvas.getTrueHeight();

        const GLfloat vertices[] = {
            x, y, regionS, regionT,
            x, y + sheet.getHeight(), regionS, regionT2,
            x + sheet.getWidth(), y + sheet.getHeight(), regionS2, regionT2,
            x + sheet.getWidth(), y + sheet.getHeight(), regionS2, regionT2,
            x + sheet.getWidth(), y, regionS2, regionT,
            x, y, regionS, regionT,
        };

        auto& e(dest.engine().impl);
        glBindBuffer(GL_ARRAY_BUFFER, impl->vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);
        glVertexAttribPointer(e->xyAttribute, 2, GL_FLOAT, false, 4 * sizeof(float), (void*) 0);
        glVertexAttribPointer(e->uvAttribute, 2, GL_FLOAT, false, 4 * sizeof(float), (void*)(2 * sizeof(float)));
        glEnableVertexAttribArray(e->xyAttribute);
        glEnableVertexAttribArray(e->uvAttribute);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
}
