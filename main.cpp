#include <iostream>
#include <vector>
#include <list>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <GL/glew.h>
#include <assimp/scene.h>
#include <glm/glm.hpp>

#include "animation.hpp"
#include "graphics.hpp"
#include "heightmap.hpp"
#include "console.hpp"
#include "camera.hpp"
#include "mesh3d.hpp"
#include "mesh2d.hpp"
#include "scene.hpp"
#include "font.hpp"
#include "textfield.hpp"
#include "button.hpp"
#include "mouse.hpp"

#define PROJECT_NAME "Shitty game engine."
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define TIME_STEP_LENGTH 1000/60

int main (void)
{
    if(SDL_Init(SDL_INIT_VIDEO) == 0)
    {
        std::cout << "Successfully initialised SDL2." << std::endl;

        if(IMG_Init(IMG_INIT_PNG) == IMG_INIT_PNG)
        {
            std::cout << "Successfully initialised SDL2_image." << std::endl;

            if(TTF_Init() == 0)
            {
                std::cout << "Successfully initialised SDL2_TTF." << std::endl;

                SDL_DisplayMode displayMode;

                SDL_GetDesktopDisplayMode(0, &displayMode);

                SDL_Window *window = SDL_CreateWindow(PROJECT_NAME,
                    SDL_WINDOWPOS_UNDEFINED,
                    SDL_WINDOWPOS_UNDEFINED,
                    800,//displayMode.w,
                    600,//displayMode.h,
                    SDL_WINDOW_OPENGL /*| SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_INPUT_GRABBED*/
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

                                Engine::GL::Font font;
                                Engine::GL::TextField textField;
                                Engine::GL::Model3D teapotModel;
                                Engine::GL::Console console;



                                Engine::GL::AnimatedModel animatedModel;
                                animatedModel.loadFile("models/ninja/ninja.b3d");
                                animatedModel.setTexture(0,"models/ninja/nskingr.jpg");
                                animatedModel.addAnimationRange("walk", 1, 14);
                                animatedModel.addAnimationRange("stealth walk", 15, 30);
                                animatedModel.addAnimationRange("jump", 94, 102);
                                animatedModel.addAnimationRange("attack1", 32, 44);
                                animatedModel.addAnimationRange("idle1", 184, 205);
                                animatedModel.addAnimationRange("idle2", 206, 250);
                                animatedModel.addAnimationRange("idle3", 251, 300);
                                animatedModel.addAnimationRange("backflip", 146, 158);
                                animatedModel.addAnimationRange("climb", 159, 165);
                                animatedModel.addAnimationRange("death1", 166, 173);
                                animatedModel.addAnimationRange("death2", 174, 182);
                                animatedModel.addAnimationRange("kick", 73, 83);

                                animatedModel.setAnimationRange("walk");

                                if(font.load("fonts/Jellee-Roman.ttf") == true) {
                                    std::cout << "Successfully loaded font!" << std::endl;
                                }

                                textField.setText(font, FONT_SIZE, "Hello, world!");
/*
                                if(font.renderString(text, "Hello, world!") == true) {
                                    std::cout << "Successfully created text mesh!" << std::endl;
                                }
*/
                                if(teapotModel.loadFile("models/teapot.obj") == true) {
                                    std::cout << "Successfully loaded teapot!" << std::endl;
                                }

                                std::vector<Engine::GL::Texture> buttonApperance(3 * 3 * 4);
                                Engine::GL::Texture::loadSpriteSheet(buttonApperance, "assets/gui/button.png");

                                Engine::GL::Button testButton(buttonApperance, 22, 100 + 35 * 1, 200, 30);
                                Engine::GL::Button testButton2(buttonApperance, 22, 100 + 35 * 2, 200, 30);
                                Engine::GL::Button testButton3(buttonApperance, 22, 100 + 35 * 3, 200, 30);
                                Engine::GL::Button testButton4(buttonApperance, 22, 100 + 35 * 4, 200, 30);

                                testButton.setText(font, 18, "Walk");
                                testButton2.setText(font, 18, "Attack");
                                testButton3.setText(font, 18, "Jump");
                                testButton4.setText(font, 18, "Backflip");

                                Engine::MouseHandler mouseHandler;
                                mouseHandler.addButton(testButton);
                                mouseHandler.addButton(testButton2);
                                mouseHandler.addButton(testButton3);
                                mouseHandler.addButton(testButton4);

                                Uint32 timeStep = 0;

                                std::cout << "Initialisation time: " << SDL_GetTicks() << std::endl;

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

                                                }
                                            }
                                            else if(event.type == SDL_MOUSEMOTION)
                                            {
                                                mouseHandler.handleMouseMotionEvent(&event.motion);
                                            }
                                            else if(event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP)
                                            {
                                                mouseHandler.handleMouseButtonEvent(&event.button);
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
                                        std::cerr << "An OpenGL eror has occured: " << gluErrorString(error) << std::endl;
                                    }

                                    //camera.applyView(graphics);

                                    //graphics.begin3D();
                                        // teapotModel.draw(graphics);
                                    //graphics.end3D();

                                    graphics.beginAnim();
                                        animatedModel.draw(graphics, (double)SDL_GetTicks() / 100.0);
                                    graphics.endAnim();


                                    graphics.begin2D();
                                        textField.draw(graphics);
                                        console.draw(graphics);
                                        testButton.draw(graphics);
                                        testButton2.draw(graphics);
                                        testButton3.draw(graphics);
                                        testButton4.draw(graphics);
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

            IMG_Quit();
        }
        else
        {
            std::cerr << "Failed to initialise SDL2_image: " << IMG_GetError() << std::endl;
        }

        SDL_Quit();
    }
    else
    {
        std::cerr << "Failed to initialise SDL2: " << SDL_GetError() << std::endl;
    }

    return 0;
}
