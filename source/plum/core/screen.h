#ifndef PLUM_VIDEO_H
#define PLUM_VIDEO_H

#include <string>
#include <memory>
#include <functional>
#include "color.h"
#include "blending.h"

namespace plum
{
    class Event;
    class Image;
    class Canvas;
    class Engine;
    class Keyboard;
    class Mouse;
    struct Input;
    struct Transform;

    class Screen
    {
        public:
            typedef std::function<void(const Event&)> EventHook;

            Screen(Engine& engine, int width, int height, int scale, bool win);
            ~Screen();

            bool getDefaultClose() const;
            bool getWindowed() const;
            int getWidth() const;
            int getHeight() const;
            int getTrueWidth() const;
            int getTrueHeight() const;
            int getOpacity() const;
            const std::string& getTitle() const;

            void setDefaultClose(bool value);
            void setWindowed(bool value);
            void setOpacity(int value);
            void setTitle(const std::string& value);

            Input& closeButton();
            Keyboard& keyboard();
            Mouse& mouse();
            Engine& engine();
            std::shared_ptr<EventHook> addEventHook(const EventHook& hook);
            void setResolution(int width, int height, int scale, bool win);

            void bindImage(Image& image);
            void unbindImage();
            void applyTransform();
            void applyTransform(const Transform& transform, int x, int y, int width, int height);

            void clear(Color color);
            void clear(int x, int y, int x2, int y2, Color color);

            void grab(int sx, int sy, int sx2, int sy2, int dx, int dy, Canvas& dest);

            class Impl;
            std::shared_ptr<Impl> impl;
    };
}

#endif