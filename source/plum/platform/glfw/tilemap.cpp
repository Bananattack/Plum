#include "engine.h"
#include "../../core/image.h"
#include "../../core/sheet.h"
#include "../../core/screen.h"
#include "../../core/tilemap.h"

namespace plum
{
    class Tilemap::Impl
    {
        public:
            Impl(int width, int height)
                : width(width), height(height), vertices(new GLfloat[6 * 4 * sizeof(float) * width * height]), vbo(0)
            {
                glGenBuffers(1, &vbo);
                glBindBuffer(GL_ARRAY_BUFFER, vbo);
                glBufferData(GL_ARRAY_BUFFER, 6 * 4 * sizeof(GLfloat) * width * height, nullptr, GL_DYNAMIC_DRAW);
            }

            ~Impl()
            {
                delete[] vertices;
                glDeleteBuffers(1, &vbo);
            }

            int width, height;
            Sheet sheet;
            GLfloat* vertices;
            GLuint vbo;
    };

    Tilemap::Tilemap(int width, int height)
        : impl(new Impl(width, height)), width(width), height(height), data(new unsigned int[width * height])
    {
        clear(0);
    }

    Tilemap::~Tilemap()
    {
        delete data;
    }

    void Tilemap::draw(Image& img, const Sheet& sheet, int x, int y, const Transform& transform, Screen& dest)
    {
        auto& sht(impl->sheet);
        if(sheet.getColumns() != sht.getColumns()
            || sheet.getRows() != sht.getRows()
            || sheet.getWidth() != sht.getWidth()
            || sheet.getHeight() != sht.getHeight()
            ||  sheet.getPadding() != sht.getPadding())
        {
            impl->sheet = sheet;
            modified = true;
        }

        dest.bindImage(img);
        dest.applyTransform(transform, -x, -y, 0, 0);

        glBindBuffer(GL_ARRAY_BUFFER, impl->vbo);
        if(modified)
        {
            auto& vertices(impl->vertices);

            float vx = 0;
            float vy = 0;
            int k = 0;
            for(int i = 0; i < height; ++i)
            {
                for(int j = 0; j < width; ++j)
                {
                    int sx = 0;
                    int sy = 0;
                    sheet.getFrame(data[i * width + j], sx, sy);

                    float u = float(sx) / img.canvas().getTrueWidth();
                    float v = float(sy) / img.canvas().getTrueHeight();
                    float u2 = float(sx + sheet.getWidth()) / img.canvas().getTrueWidth();
                    float v2 = float(sy + sheet.getHeight()) / img.canvas().getTrueHeight();

                    vertices[k++] = vx; vertices[k++] = vy; vertices[k++] = u; vertices[k++] = v;
                    vertices[k++] = vx; vertices[k++] = vy + sheet.getHeight(); vertices[k++] = u; vertices[k++] = v2;
                    vertices[k++] = vx + sheet.getWidth(); vertices[k++] = vy + sheet.getHeight(); vertices[k++] = u2; vertices[k++] = v2;
                    vertices[k++] = vx + sheet.getWidth(); vertices[k++] = vy + sheet.getHeight(); vertices[k++] = u2; vertices[k++] = v2;
                    vertices[k++] = vx + sheet.getWidth(); vertices[k++] = vy; vertices[k++] = u2; vertices[k++] = v;
                    vertices[k++] = vx; vertices[k++] = vy; vertices[k++] = u; vertices[k++] = v;

                    vx += sheet.getWidth();
                }
                vx -= sheet.getWidth() * width;
                vy += sheet.getHeight();
            }
            glBufferSubData(GL_ARRAY_BUFFER, 0, 6 * 4 * sizeof(GLfloat) * width * height, vertices);
            modified = false;
        }
        
        auto& e(dest.engine().impl);
        glVertexAttribPointer(e->xyAttribute, 2, GL_FLOAT, false, 4 * sizeof(GLfloat), (void*) 0);
        glVertexAttribPointer(e->uvAttribute, 2, GL_FLOAT, false, 4 * sizeof(GLfloat), (void*)(2 * sizeof(float)));
        glEnableVertexAttribArray(e->xyAttribute);
        glEnableVertexAttribArray(e->uvAttribute);
        glDrawArrays(GL_TRIANGLES, 0, 6 * width * height);
        dest.unbindImage();
    }
}