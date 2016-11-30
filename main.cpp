#include <iostream>
#include <vector>
#include <list>
#include <lua5.2/lua.hpp>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <GL/glew.h>
#include <assimp/scene.h>
#include <glm/glm.hpp>
#include <btBulletDynamicsCommon.h>

#include "animation.hpp"
#include "graphics.hpp"
#include "heightmap.hpp"
#include "console.hpp"
#include "camera.hpp"
#include "mesh3d.hpp"
#include "mesh2d.hpp"
#include "scene.hpp"
#include "font.hpp"

#define PROJECT_NAME "Shitty game engine."
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define TIME_STEP_LENGTH 1000/60

int main (void)
{
    if(SDL_Init(SDL_INIT_VIDEO) == 0)
    {
        std::cout << "Successfully initialised SDL2." << std::endl;

        if(TTF_Init() == 0)
        {
            std::cout << "Successfully initialised SDL2_TTF." << std::endl;

            SDL_DisplayMode displayMode;

            SDL_GetDesktopDisplayMode(0, &displayMode);

            SDL_Window *window = SDL_CreateWindow(PROJECT_NAME,
                SDL_WINDOWPOS_UNDEFINED,
                SDL_WINDOWPOS_UNDEFINED,
                displayMode.w,
                displayMode.h,
                SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_INPUT_GRABBED
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

                            Engine::GL::Camera camera;
                            Engine::GL::Scene teapotScene;
                            Engine::GL::Mesh3D teapotMesh;
                            Engine::GL::Mesh2D text;
                            Engine::GL::Font font;
                            Engine::GL::Console console;
                            Engine::GL::AnimatedMesh animatedMesh;
                            animatedMesh.loadFile("models/dwarf.h");

                            if(font.load("fonts/NanumGothic-Bold.ttf") == true) {
                                std::cout << "Successfully loaded font!" << std::endl;
                            }

                            if(font.renderString(text, "Hello, world!") == true) {
                                std::cout << "Successfully created text mesh!" << std::endl;
                            }

                            if(teapotScene.load("models/teapot.obj") == true) {
                                std::cout << "Successfully loaded teapot!" << std::endl;
                                teapotScene.createMesh(teapotMesh);
                            }

                            Uint32 timeStep = 0;

                            while(!finished)
                            {
                                SDL_Event event;

                                Uint32 frameTime = SDL_GetTicks();

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
                                            console.appendInput(font, event.text.text);
                                        }
                                        else if(event.type == SDL_KEYDOWN)
                                        {
                                            if(event.key.keysym.scancode == SDL_SCANCODE_BACKSPACE)
                                            {
                                                console.backspace(font);
                                            }
                                            if(event.key.keysym.scancode == SDL_SCANCODE_RETURN)
                                            {
                                                // TODO
                                            }
                                        }
                                        else if(event.type == SDL_TEXTINPUT)
                                        {
                                            console.appendInput(font, event.text.text);
                                        }
                                    }
                                }

                                while(timeStep > TIME_STEP_LENGTH)
                                {
                                    timeStep -= TIME_STEP_LENGTH;
                                }

                                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                                for(int error = glGetError(); error != GL_NO_ERROR; error = glGetError())
                                {
                                    std::cerr << "An OpenGL eror has occured: " << error << std::endl;
                                }

                                camera.applyView(graphics);

                                graphics.begin3D();
                                    teapotMesh.draw(graphics);
                                graphics.end3D();

                                graphics.begin2D();
                                    text.draw(graphics);
                                    console.draw(graphics);
                                graphics.end2D();

                                SDL_GL_SwapWindow(window);

                                frameTime = SDL_GetTicks() - frameTime;
                                timeStep += frameTime;
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
