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
    class Engine;
    class Input;
    class Keyboard;
    struct Transform;
    class Screen
    {
        public:
            typedef std::function<void(const Event&)> EventHook;

            Screen(Engine& engine, int width, int height, int scale, bool win);
            ~Screen();

            int getWidth() const;
            int getHeight() const;
            int getTrueWidth() const;
            int getTrueHeight() const;
            int getOpacity() const;
            const std::string& getTitle() const;

            void setOpacity(int value);
            void setTitle(const std::string& title);

            Input& closeButton();
            Keyboard& keyboard();
            std::shared_ptr<EventHook> addEventHook(const EventHook& hook);
            void setResolution(int width, int height, int scale, bool win);

            void bind(Image& image);
            void bind(const Transform& transform, int x, int y, int width, int height);
            void bind(Image& image, const Transform& transform, int x, int y, int width, int height);
            void unbind();

            void clear(Color color);
            void clear(int x, int y, int x2, int y2, Color color);

            class Impl;
            std::shared_ptr<Impl> impl;
    };
}

#endif