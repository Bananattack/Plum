#ifndef PLUM_SDL_EVENT_H
#define PLUM_SDL_EVENT_H

#include <SDL.h>

namespace plum
{
    class Event
    {
        public:
            SDL_Event e;

            Event(SDL_Event e)
                : e(e)
            {
            }
    };
}

#endif