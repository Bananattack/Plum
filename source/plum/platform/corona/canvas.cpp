#include <cstdlib>
#include <memory>
#include <stdexcept>
#include <corona.h>
#include "../../core/file.h"
#include "../../core/canvas.h"

namespace
{
    class FileWrapper : public corona::DLLImplementation<corona::File>
    {
        public:
            FileWrapper(plum::File* f)
            {
                file = f;
            }

            virtual ~FileWrapper()
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
                plum::FileSeekMode m;
                switch(mode)
                {
                    case BEGIN: m = plum::FileSeekMode::Start; break;
                    case CURRENT: m = plum::FileSeekMode::Current; break;
                    case END: m = plum::FileSeekMode::End; break;
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
}

namespace plum
{
    Canvas Canvas::load(const std::string& filename)
    {
        std::unique_ptr<corona::File> file(new FileWrapper(new File(filename, FileOpenMode::Read)));
        std::unique_ptr<corona::Image> image(corona::OpenImage(file.get(), corona::PF_R8G8B8A8, corona::FF_AUTODETECT));
        if(!image.get())
        {
            throw std::runtime_error("Couldn't open image '" + filename + "'!\r\n");
        }

        Canvas canvas(image->getWidth(), image->getHeight());
        std::memcpy(canvas.data, image->getPixels(), sizeof(Color) * image->getWidth() * image->getHeight());
        canvas.replaceColor(Color::Magenta, 0);
        return canvas;
    }
}
