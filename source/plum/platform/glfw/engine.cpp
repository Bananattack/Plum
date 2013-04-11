#include <iostream>

#include "engine.h"
#include "../../core/log.h"

#ifdef _WIN32
#include <GL/glfw3native.h>
#endif

namespace plum
{
    Engine::Impl::Impl()
    {
        if(!glfwInit())
        {
            quit("Couldn't initialize glfw.\n");
        }
        glfwSetTime(0.0);

        glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
        root = glfwCreateWindow(1, 1, GLFW_WINDOWED, "", nullptr);
        glfwMakeContextCurrent(root);
        glEnable(GL_TEXTURE_2D);
    }

    Engine::Impl::~Impl()
    {
        glfwDestroyWindow(root);
        glfwTerminate();
    }

    void Engine::Impl::quit(const std::string& message)
    {
        if(message.length())
        {
            fprintf(stderr, "Exit Requested: %s", message.c_str());
            logFormat("Exit Requested: %s", message.c_str());
#ifdef _WIN32
            MessageBoxA(nullptr, message.c_str(), "Exit Requested", MB_SYSTEMMODAL);
#endif
            throw SystemExit(1);
        }
        throw SystemExit(0);
    }

    void Engine::Impl::refresh()
    {
        for(const auto h : updateHooks)
        {
            if(auto f = h.lock())
            {
                (*f)();
            }
        }
        updateHooks.cleanup();
    }

    Engine::Engine()
        : impl(new Impl())
    {
    }

    Engine::~Engine()
    {
    }

    void Engine::refresh()
    {
        impl->refresh();
    }

    void Engine::quit(const std::string& message)
    {
        impl->quit(message);
    }

    std::shared_ptr<Engine::UpdateHook> Engine::addUpdateHook(const UpdateHook& hook)
    {
        auto ptr = std::make_shared<Engine::UpdateHook>(hook);
        impl->updateHooks.append(ptr);
        return ptr;
    }
}
