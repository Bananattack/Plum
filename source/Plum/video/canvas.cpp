#pragma once

#include "canvas.h"

#include <cstdlib>
#include <memory>
#include <corona.h>
#include "../engine/file.h"

namespace plum
{
    class CoronaPlumFile : public corona::DLLImplementation<corona::File>
    {
        public:
            CoronaPlumFile(plum::File* f)
            {
                file = f;
            }

            virtual ~CoronaPlumFile()
            {
                delete file;
            }

            virtual int COR_CALL read(void* buffer, int size)
            {
                return file->readRaw(buffer, size);
            }

            virtual int COR_CALL write(const void *buffer, int size)
            {
                return file->writeRaw(buffer, size);
            }

            virtual bool COR_CALL seek(int position, corona::File::SeekMode mode)
            {
                FileSeekMode m;
                switch (mode)
                {
                    case BEGIN: m = SeekStart; break;
                    case CURRENT: m = SeekCurrent; break;
                    case END: m = SeekEnd; break;
                    default: return false;
                }
                return file->seek(position, m);
            }

            virtual int COR_CALL tell()
            {
                return file->tell();
            }
        private:
            plum::File* file;
    };

    Canvas* Canvas::load(const std::string& filename)
    {
        std::unique_ptr<corona::File> file(new CoronaPlumFile(new File(filename, FileRead)));
        std::unique_ptr<corona::Image> image(corona::OpenImage(file.get(), corona::PF_R8G8B8A8, corona::FF_AUTODETECT));
        if(!image.get())
        {
            throw std::runtime_error("Couldn't open image '" + filename + "'!\r\n");
        }

        Canvas* canvas = new Canvas(image->getWidth(), image->getHeight());
        memcpy(canvas->data, image->getPixels(), sizeof(Color) * image->getWidth() * image->getHeight());
        canvas->replaceColor(Color::Magenta, 0);
        return canvas;
    }
}
