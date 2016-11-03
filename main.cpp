#include <iostream>
#include <vector>
#include <list>
#include <lua5.2/lua.hpp>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <GL/glew.h>
#include <assimp/scene.h>
#include <glm/glm.hpp>

#include "graphics.hpp"
#include "console.hpp"
#include "mesh3d.hpp"
#include "mesh2d.hpp"
#include "scene.hpp"
#include "font.hpp"

#define PROJECT_NAME "Shitty game engine."
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

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
                                std::cout << "Successfully loaded model." << std::endl;
                            }

                            Engine::GL::Font font;

                            if(font.load("fonts/NanumGothicCoding-Bold.ttf") == true)
                            {
                                std::cout << "Successfully loaded font." << std::endl;
                            }

                            SDL_StartTextInput();

                            Engine::GL::Mesh2D greetingText;
                            Engine::GL::Mesh2D framesPerSecondMesh;
                            font.renderString(greetingText, "안녕하세요, 세계!");

                            lua_State *luaState = luaL_newstate();
                            luaL_openlibs(luaState);

                            Engine::GL::Console console;

                            Engine::GL::Mesh3D heightmap;

                            // Dirty heightmap loading:
                            SDL_Surface *heightmapSurface = SDL_LoadBMP("assets/heightmap.bmp");

                            if(heightmapSurface)
                            {
                                SDL_Surface *optimisedSurface = SDL_CreateRGBSurface(0,
                                    heightmapSurface->w, heightmapSurface->h, 24, 0, 0, 0, 0);

                                if(optimisedSurface)
                                {
                                    SDL_BlitSurface(heightmapSurface, NULL, optimisedSurface, NULL);

                                    std::vector<glm::vec3> vertexArray;
                                    std::vector<GLuint> indexArray;

                                    for(int y = 0; y < optimisedSurface->h; y++)
                                    {
                                        for(int x = 0; x < optimisedSurface->w; x++)
                                        {
                                            Uint8 *pixel = (Uint8*)heightmapSurface->pixels + (heightmapSurface->pitch * y) + (x * heightmapSurface->format->BytesPerPixel);
                                            GLfloat height = (GLfloat)(pixel[0] + pixel[1] + pixel[2]) / 3.0f;
                                            vertexArray.emplace_back(
                                                (GLfloat)(x * 10), height,
                                                (GLfloat)(y * 10)
                                            );
                                        }
                                    }

                                    for(int y = 0; y < optimisedSurface->h - 1; y++)
                                    {
                                        for(int x = 0; x < optimisedSurface->w - 1; x++)
                                        {
                                            GLuint index = y * optimisedSurface->w + x;

                                            indexArray.emplace_back(index + 1);
                                            indexArray.emplace_back(index);
                                            indexArray.emplace_back(index + optimisedSurface->w);

                                            indexArray.emplace_back(index + 1);
                                            indexArray.emplace_back(index + optimisedSurface->w);
                                            indexArray.emplace_back(index + optimisedSurface->w + 1);
                                        }
                                    }

                                    heightmap.setVertices(vertexArray);
                                    heightmap.setIndices(indexArray);
                                    heightmap.calculateNormals();

                                    SDL_FreeSurface(optimisedSurface);
                                }
                                else
                                {
                                    std::cerr << "Failed to create optimised SDL surface." << std::endl;
                                }

                                SDL_FreeSurface(heightmapSurface);
                            }
                            else
                            {
                                std::cerr << "Failed to load assets/heightmap.bmp" << std::endl;
                            }

                            Uint32 framesPerSecond = 0;

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
                                                console.submitInput(font, luaState);
                                            }
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
                                    heightmap.draw(graphics);
                                graphics.end3D();

                                graphics.begin2D();
                                    greetingText.draw(graphics);
                                    framesPerSecondMesh.draw(graphics, glm::vec2(0.0f, FONT_SIZE * 1.0f));
                                    console.draw(graphics);
                                graphics.end2D();

                                font.renderString(framesPerSecondMesh, std::to_string(framesPerSecond).data());

                                SDL_GL_SwapWindow(window);

                                frameTime = SDL_GetTicks() - frameTime;
                                framesPerSecond = frameTime ? 1000 / frameTime : 1000;
                            }

                            lua_close(luaState);
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
