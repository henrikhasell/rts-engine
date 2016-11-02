#include <iostream>
#include <vector>
#include <list>
#include <lua5.2/lua.hpp>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <GL/glew.h>
#include <assimp/scene.h>

#include "graphics.hpp"
#include "mesh3d.hpp"
#include "mesh2d.hpp"
#include "scene.hpp"
#include "font.hpp"

#define PROJECT_NAME "Shitty game engine."
#define SCREEN_WIDTH 800// 1280
#define SCREEN_HEIGHT 600// 720

int main (void)
{
/*
    std::cout << "Initialising Lua..." << std::endl;

    lua_State *luaState = luaL_newstate();
    luaL_openlibs(luaState);

    std::string input;

    while(std::getline(std::cin, input))
    {
        int result = luaL_loadbuffer(luaState, input.c_str(), input.length(), "input") || lua_pcall(luaState, 0, 0, 0);

        if (result != 0)
        {
            std::cerr << lua_tostring(luaState, -1) << std::endl;
            lua_pop(luaState, 1);
        }
    }

    std::cout << "Cleaning up..." << std::endl;

    lua_close(luaState);
*/
    if(SDL_Init(SDL_INIT_VIDEO) == 0)
    {
        std::cout << "Successfully initialised SDL2." << std::endl;

        if(TTF_Init() == 0)
        {
            std::cout << "Successfully initialised SDL2_TTF." << std::endl;

            SDL_Window *window = SDL_CreateWindow(PROJECT_NAME,
                SDL_WINDOWPOS_UNDEFINED,
                SDL_WINDOWPOS_UNDEFINED,
                SCREEN_WIDTH,
                SCREEN_HEIGHT,
                SDL_WINDOW_OPENGL
            );

            if(window != nullptr)
            {
                std::cout << "Successfully created SDL2 window." << std::endl;

                SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
                SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
                SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
                SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

                SDL_GLContext context = SDL_GL_CreateContext(window);

                if(context != nullptr)
                {
                    std::cout << "Successfully created OpenGL context." << std::endl;

                    GLenum glewResult = glewInit();

                    if(glewResult == GLEW_OK)
                    {
                        Engine::GL::Graphics graphics(window);

                        std::cout << "Successfully loaded OpenGL extensions." << std::endl;

                        if(graphics.initialise() == true)
                        {
                            bool finished = false;

                            std::cout << "Successfully initialised the graphics sub-system." << std::endl;

                            Engine::GL::Scene scene;

                            if(scene.load("models/teapot.obj") == true)
                            {
                                std::cout << "Successfully loaded teapot." << std::endl;
                            }

                            Engine::GL::Font font;

                            if(font.load("fonts/NanumGothic-Bold.ttf") == true)
                            {
                                std::cout << "Successfully loaded font." << std::endl;
                            }

                            SDL_StartTextInput();

                            Engine::GL::Mesh2D inputText, greetingText;

                            font.renderString(greetingText, "안녕하세요, 세계!");

                            std::string consoleInput = "";
                            std::list<Engine::GL::Mesh2D> consoleOutput;

                            while(!finished)
                            {
                                SDL_Event event;

                                while(SDL_PollEvent(&event) != 0)
                                {
                                    if(event.type == SDL_QUIT)
                                    {
                                        finished = true;
                                    }
                                    else
                                    {
                                        if(event.type == SDL_TEXTINPUT)
                                        {
                                            if(consoleInput.length() + strlen(event.text.text) < 20)
                                            {// TODO: Extend arbitrary limit.
                                                consoleInput += event.text.text;
                                                font.renderString(inputText, consoleInput.empty() ? " " : consoleInput.data());
                                            }
                                        }
                                        else if(event.type == SDL_KEYDOWN)
                                        {
                                            if(event.key.keysym.scancode == SDL_SCANCODE_BACKSPACE)
                                            {
                                                if(consoleInput.length() != 0)
                                                {
                                                    /*
                                                    for(int last = 0x80; last == 0x80; last = line.back() & 0x00C0)
                                                    {
                                                        printf("%X\n", last);
                                                        line.pop_back();
                                                    }
                                                    */

                                                    int last;

                                                    do
                                                    {
                                                        last = consoleInput.back() & 0x00C0;
                                                        consoleInput.pop_back();
                                                    }
                                                    while(last == 0x80);

                                                }
                                            }
                                            if(event.key.keysym.scancode == SDL_SCANCODE_RETURN)
                                            {
                                                if(consoleInput.empty() == true)
                                                {
                                                    continue;
                                                }
                                                consoleOutput.emplace_back();
                                                font.renderString(consoleOutput.back(), consoleInput.empty() ? " " : consoleInput.data());

                                                if(consoleOutput.size() > 10)
                                                {
                                                    consoleOutput.pop_front();
                                                }

                                                consoleInput.clear();
                                            }
                                            font.renderString(inputText, consoleInput.empty() ? " " : consoleInput.data());
                                        }
                                    }
                                }

                                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                                for(int error = glGetError(); error != GL_NO_ERROR; error = glGetError())
                                {
                                    std::cerr << "An OpenGL eror has occured: " << error << std::endl;
                                }

                                graphics.begin3D();
                                    scene.draw(graphics);
                                    scene.draw(graphics, glm::vec3(0, 100, 0));
                                graphics.end3D();

                                graphics.begin2D();
                                    greetingText.draw(graphics);
                                    glm::vec2 offset(0.0f, 24.0f);
                                    for(Engine::GL::Mesh2D &mesh : consoleOutput)
                                    {
                                        mesh.draw(graphics, offset);
                                        offset.y += 24.0f;
                                    }
                                    inputText.draw(graphics, offset);
                                graphics.end2D();

                                SDL_GL_SwapWindow(window);
                            }
                        }
                        else
                        {
                            std::cerr << "Failed to initialise the graphics sub-system." << std::endl;
                        }


                    }
                    else
                    {
                        std::cerr << "Failed to load OpenGL extensions: " << glewGetErrorString(glewResult) << std::endl;
                    }

                    SDL_GL_DeleteContext(context);
                }
                else
                {
                    std::cerr << "Failed to create OpenGL context:" << SDL_GetError() << std::endl;
                }

                SDL_DestroyWindow(window);
            }
            else
            {
                std::cerr << "Failed to create SDL2 window:" << SDL_GetError() << std::endl;
            }

            TTF_Quit();
        }
        else
        {
            std::cerr << "Failed to initialise SDL2_TTF: " << TTF_GetError() << std::endl;
        }

        SDL_Quit();
    }
    else
    {
        std::cerr << "Failed to initialise SDL2: " << SDL_GetError() << std::endl;
    }

    return 0;
}
