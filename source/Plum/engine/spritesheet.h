#pragma once
namespace plum
{
    class Spritesheet
    {
        private:
            Image* image;
        public:
            int frameWidth, frameHeight;
            int padding;
            int columns;

            Spritesheet(Canvas* canvas, int w, int h)
            {
                init(canvas, w, h);
            }

            Spritesheet(Image* img, int w, int h)
            {
                init(img->getCanvas(), w, h);
            }
            
            ~Spritesheet()
            {
                delete image;
            }

        private:
            void init(Canvas* canvas, int w, int h)
            {
                image = new Image(canvas);
                frameWidth = w;
                frameHeight = h;
                padding = 0;
                columns = image->getCanvasWidth() / w;
            }

        public:
            Image* getImage()
            {
                return image;
            }

            void bind()
            {
                image->bind();
            }

            Color getFramePixel(int f, int x, int y)
            {
                if(!columns || x < 0 || x >= frameWidth || y < 0 || y >= frameHeight) return 0;

                int fx = (f % columns) * (frameWidth + padding) + padding;
                int fy = (f / columns) * (frameHeight + padding) + padding;
                return image->getCanvas()->getPixel(fx + x, fy + y);
            }

            void blitFrame(int x, int y, int f, BlendMode mode = BlendUnspecified, Color tint = Color::White)
            {
                if(!columns) return;

                int fx = (f % columns) * (frameWidth + padding) + padding;
                int fy = (f / columns) * (frameHeight + padding) + padding;
                image->blitRegion(fx, fy, fx + frameWidth - 1, fy + frameHeight - 1,
                        x, y, mode, tint);
            }

            void rawBlitFrame(int x, int y, int f, double angle, double scale)
            {
                if(!columns) return;

                int fx = (f % columns) * (frameWidth + padding) + padding;
                int fy = (f / columns) * (frameHeight + padding) + padding;
                image->rawBlitRegion(fx, fy, fx + frameWidth - 1, fy + frameHeight - 1,
                        x, y, 0, 1);
            }
    };
}