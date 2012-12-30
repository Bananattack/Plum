//========================================================================
// Simple multi-window test
// Copyright (c) Camilla Berglund <elmindreda@elmindreda.org>
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would
//    be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such, and must not
//    be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source
//    distribution.
//
//========================================================================
//
// This test creates four windows and clears each in a different color
//
//========================================================================

#include <GL/glfw3.h>

#include <stdio.h>
#include <stdlib.h>

static const char* titles[] =
{
    "Foo",
    "Bar",
    "Baz",
    "Quux"
};

int main(void)
{
    int i;
    GLboolean running = GL_TRUE;
    GLFWwindow windows[4];

    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW: %s\n",
                glfwErrorString(glfwGetError()));
        exit(EXIT_FAILURE);
    }

    for (i = 0;  i < 4;  i++)
    {
        glfwWindowHint(GLFW_POSITION_X, 100 + (i & 1) * 300);
        glfwWindowHint(GLFW_POSITION_Y, 100 + (i >> 1) * 300);
        windows[i] = glfwCreateWindow(200, 200, GLFW_WINDOWED, titles[i], NULL);
        if (!windows[i])
        {
            fprintf(stderr, "Failed to open GLFW window: %s\n",
                    glfwErrorString(glfwGetError()));

            glfwTerminate();
            exit(EXIT_FAILURE);
        }

        glfwMakeContextCurrent(windows[i]);
        glClearColor((GLclampf) (i & 1),
                     (GLclampf) (i >> 1),
                     i ? 0.f : 1.f,
                     0.f);
    }

    while (running)
    {
        for (i = 0;  i < 4;  i++)
        {
            glfwMakeContextCurrent(windows[i]);
            glClear(GL_COLOR_BUFFER_BIT);
            glfwSwapBuffers(windows[i]);

            if (glfwGetWindowParam(windows[i], GLFW_CLOSE_REQUESTED))
                running = GL_FALSE;
        }

        glfwPollEvents();
    }

    glfwTerminate();
    exit(EXIT_SUCCESS);
}

