#include <iostream>
#include <vector>
#include <list>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <GL/glew.h>
#include <lua5.3/lua.hpp>
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

Engine::GL::Graphics *globalGraphics=nullptr;
std::vector<Engine::GL::Texture> *globalButtonApperance = nullptr;
Engine::GL::AnimatedModel *globalModel = nullptr;
Engine::GL::Font *globalFont = nullptr;
Engine::MouseHandler *globalMouseHandler = nullptr;

std::list<void*> allocated;

int lua_Model_draw(lua_State *luaState)
{
    double timeElapsed = lua_tonumber(luaState, 2);

    lua_getfield(luaState, 1, "pointer");
        Engine::GL::AnimatedModel *model = (Engine::GL::AnimatedModel*)lua_touserdata(luaState, -1);
        model->draw(*globalGraphics, timeElapsed);
    lua_pop(luaState, 1);

    return 0;
}

int lua_Model_loadTexture(lua_State *luaState)
{
    const char *path = lua_tostring(luaState, 2);

    lua_getfield(luaState, 1, "pointer");
        Engine::GL::AnimatedModel *model = (Engine::GL::AnimatedModel*)lua_touserdata(luaState, -1);
        model->setTexture(0, path);
    lua_pop(luaState, 1);

    return 0;
}

int lua_Model_addAnimationRange(lua_State *luaState)
{
    const char *name = lua_tostring(luaState, 2);
    double min = lua_tonumber(luaState, 3);
    double max = lua_tonumber(luaState, 4);

    lua_getfield(luaState, 1, "pointer");
        Engine::GL::AnimatedModel *model = (Engine::GL::AnimatedModel*)lua_touserdata(luaState, -1);
        model->addAnimationRange(name, min, max);
    lua_pop(luaState, 1);

    return 0;
}

int lua_Model_setAnimationRange(lua_State *luaState)
{
    const char *name = lua_tostring(luaState, 2);

    lua_getfield(luaState, 1, "pointer");
        Engine::GL::AnimatedModel *model = (Engine::GL::AnimatedModel*)lua_touserdata(luaState, -1);
        model->setAnimationRange(name);
    lua_pop(luaState, 1);
    return 0;
}

int lua_Button_draw(lua_State *luaState)
{
    lua_getfield(luaState, 1, "pointer");
        Engine::GL::Button *button = (Engine::GL::Button*)lua_touserdata(luaState, -1);
        button->draw(*globalGraphics);
    lua_pop(luaState, 1);

    return 0;
}

int lua_Button_setText(lua_State *luaState)
{
    const char *text = lua_tostring(luaState, 2);

    lua_getfield(luaState, 1, "pointer");
        Engine::GL::Button *button = (Engine::GL::Button*)lua_touserdata(luaState, -1);
        button->setText(*globalFont, FONT_SIZE, text);
    lua_pop(luaState, 1);

    return 0;
}

int createButton(lua_State *luaState)
{
    std::cout << "Creating button..." << std::endl;
    double x, y, w, h;
    const char *text;

    x = lua_tonumber(luaState, 1);
    y = lua_tonumber(luaState, 2);
    w = lua_tonumber(luaState, 3);
    h = lua_tonumber(luaState, 4);
    text = lua_tostring(luaState, 5);

    std::cout << "Size: " << x << ", " << y << ", " << w << ", " << h << std::endl;
    // Create new button object:
    Engine::GL::Button *button = new Engine::GL::Button(*globalButtonApperance, x, y, w, h);
    // Set the button text:
    button->setText(*globalFont, FONT_SIZE, text);
    // Add newly allocated object to array to be freed on exit:
    allocated.emplace_back(button);
    // Add the new button to the mouse handler:
    globalMouseHandler->addButton(*button);

    // Create a new table to store the button:
    lua_newtable(luaState);

    // Add pointer value to table:
    lua_pushstring(luaState, "pointer");
    lua_pushlightuserdata(luaState, button);
    lua_settable(luaState, -3);

    // Add draw value to table:
    lua_pushstring(luaState, "draw");
    lua_pushcfunction(luaState, lua_Button_draw);
    lua_settable(luaState, -3);

    // Add setText value to table:
    lua_pushstring(luaState, "setText");
    lua_pushcfunction(luaState, lua_Button_setText);
    lua_settable(luaState, -3);

    // Number of values returned:
    return 1;
}

int createMesh(lua_State *luaState)
{
    const char *path = lua_tostring(luaState, 1);
    Engine::GL::AnimatedModel *model = new Engine::GL::AnimatedModel();
    allocated.emplace_back(model);
    model->loadFile(path);
    model->setTexture(0,"models/ninja/nskingr.jpg");
    model->addAnimationRange("walk", 1, 14);
    model->setAnimationRange("walk");

    // Create a new table to store the button:
    lua_newtable(luaState);

    lua_pushstring(luaState, "pointer");
    lua_pushlightuserdata(luaState, model);
    lua_settable(luaState, -3);

    lua_pushstring(luaState, "loadTexture");
    lua_pushcfunction(luaState, lua_Model_loadTexture);
    lua_settable(luaState, -3);

    lua_pushstring(luaState, "addAnimationRange");
    lua_pushcfunction(luaState, lua_Model_addAnimationRange);
    lua_settable(luaState, -3);

    lua_pushstring(luaState, "setAnimationRange");
    lua_pushcfunction(luaState, lua_Model_setAnimationRange);
    lua_settable(luaState, -3);

    lua_pushstring(luaState, "draw");
    lua_pushcfunction(luaState, lua_Model_draw);
    lua_settable(luaState, -3);

    // Number of values returned:
    return 1;
}


void setWalkAnim(){
    globalModel->setAnimationRange("walk");
}

void setAttackAnim(){
    globalModel->setAnimationRange("attack1");
}

void setJumpAnim(){
    globalModel->setAnimationRange("jump");
}

void setBackflipAnim(){
    globalModel->setAnimationRange("backflip");
}

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
                                globalGraphics = &graphics;
                                std::cout << "Successfully initialised the graphics sub-system." << std::endl;

                                lua_State *luaState = luaL_newstate();
                                luaL_openlibs(luaState);// Load Lua standard library.

                                lua_newtable(luaState);

                                    lua_pushstring(luaState, "createButton");
                                    lua_pushcfunction(luaState, createButton);
                                    lua_settable(luaState, -3);

                                    lua_pushstring(luaState, "createMesh");
                                    lua_pushcfunction(luaState, createMesh);
                                    lua_settable(luaState, -3);

                                    lua_pushstring(luaState, "draw");
                                    lua_pushnil(luaState);
                                    lua_settable(luaState, -3);

                                lua_setglobal(luaState, "engine");

                                Engine::GL::Font font;
                                Engine::GL::TextField textField;
                                Engine::GL::Console console;


                                //Engine::GL::Model3D teapotModel;
/*
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
                                globalModel = &animatedModel;
*/
                                if(font.load("fonts/NanumGothic-Bold.ttf") == true) {
                                    std::cout << "Successfully loaded font!" << std::endl;
                                }
                                globalFont = &font;

                                std::vector<Engine::GL::Texture> buttonApperance(3 * 3 * 4);
                                Engine::GL::Texture::loadSpriteSheet(buttonApperance, "assets/gui/button.png");
                                globalButtonApperance = &buttonApperance;

                                textField.setText(font, FONT_SIZE, "안녕하세요, 세계!");
/*
                                if(font.renderString(text, "Hello, world!") == true) {
                                    std::cout << "Successfully created text mesh!" << std::endl;
                                }
*/
/*
                                if(teapotModel.loadFile("models/teapot.obj") == true) {
                                    std::cout << "Successfully loaded teapot!" << std::endl;
                                }
*/
/*

                                Engine::GL::Button testButton(buttonApperance, 22, 100 + 35 * 1, 200, 30);
                                Engine::GL::Button testButton2(buttonApperance, 22, 100 + 35 * 2, 200, 30);
                                Engine::GL::Button testButton3(buttonApperance, 22, 100 + 35 * 3, 200, 30);
                                Engine::GL::Button testButton4(buttonApperance, 22, 100 + 35 * 4, 200, 30);

                                testButton.setText(font, 18, "Walk");
                                testButton2.setText(font, 18, "Attack");
                                testButton3.setText(font, 18, "Jump");
                                testButton4.setText(font, 18, "Backflip");

                                testButton.setCallback(setWalkAnim);
                                testButton2.setCallback(setAttackAnim);
                                testButton3.setCallback(setJumpAnim);
                                testButton4.setCallback(setBackflipAnim);

                                mouseHandler.addButton(testButton);
                                mouseHandler.addButton(testButton2);
                                mouseHandler.addButton(testButton3);
                                mouseHandler.addButton(testButton4);
*/
                                Engine::MouseHandler mouseHandler;
                                globalMouseHandler = &mouseHandler;

                                if(luaL_dofile(luaState, "main.lua") != 0)
                                {
                                    console.appendOutput(font, lua_tostring(luaState, -1));
                                    lua_pop(luaState, 1);
                                }

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
                                                    console.submitInput(font, luaState);
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


                                    graphics.begin3D();
                                    lua_getglobal(luaState, "engine");
                                        lua_getfield(luaState, -1, "draw3D");
                                        lua_pushnumber(luaState, SDL_GetTicks());
                                        lua_pcall(luaState, 1, 0, 0);
                                    lua_pop(luaState, 1);
                                    graphics.end3D();


                                    graphics.begin2D();
                                        textField.draw(graphics, glm::vec2(0, 0));
                                        console.draw(graphics);

                                    lua_getglobal(luaState, "engine");
                                        lua_getfield(luaState, -1, "draw2D");
                                        lua_pcall(luaState, 0, 0, 0);
                                    lua_pop(luaState, 1);
                                        /*
                                        testButton.draw(graphics);
                                        testButton2.draw(graphics);
                                        testButton3.draw(graphics);
                                        testButton4.draw(graphics);
                                        */
                                    graphics.end2D();

                                    SDL_GL_SwapWindow(window);

                                    frameTime = SDL_GetTicks() - frameTime;
                                    timeStep += frameTime;

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
