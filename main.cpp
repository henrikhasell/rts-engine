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


                            Engine::GL::Mesh3D teapotMesh;
                            Engine::GL::Mesh3D cubeMesh;
                            Engine::GL::Mesh3D sphereMesh;
                            Engine::GL::Mesh3D soldierMesh;

                            Engine::GL::Scene teapotScene;
                            teapotScene.load("models/teapot.obj");

                            Engine::GL::Scene sphereScene;
                            sphereScene.load("models/sphere.obj");

                            Engine::GL::Scene cubeScene;
                            cubeScene.load("models/bigCube.obj");

                            Engine::GL::Scene soldierScene;
                            soldierScene.load("models/dwarf/dwarf.x");

                            Engine::GL::Font font;
                            font.load("fonts/NanumGothicCoding-Bold.ttf");

                            Engine::GL::Mesh2D greetingText;
                            font.renderString(greetingText, "안녕하세요, 세계!");

                            Engine::GL::AnimatedMesh test;
                            test.load("models/dwarf/dwarf.x");
                            return 0;

                            teapotScene.createMesh(teapotMesh);
                            sphereScene.createMesh(sphereMesh);
                            cubeScene.createMesh(cubeMesh);
                            soldierScene.createMesh(soldierMesh);

                            SDL_StartTextInput();

                            Engine::GL::Camera camera;

                            Engine::GL::Mesh2D framesPerSecondMesh;

                            lua_State *luaState = luaL_newstate();
                            luaL_openlibs(luaState);

                            Engine::GL::Console console;

                            Engine::GL::Heightmap heightmap;

                            if(heightmap.load("assets/flat.bmp") == true)
                            {
                                heightmap.generateMesh();
                            }
                            else
                            {
                                std::cerr << "Failed to load heightmap." << std::endl;
                            }

                            btBroadphaseInterface* broadphase = new btDbvtBroadphase();
                            btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
                            btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
                            btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;
                            btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);

                            dynamicsWorld->setGravity(btVector3(0.0f, -10.0f, 0.0f));


                            btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 1);
                            btCollisionShape* fallShape =
                            /*
                                new btBoxShape(
                                    btVector3(10.0f, 10.0f, 10.0f)
                                );
                            */
                                new btSphereShape(1.0f);

                            btDefaultMotionState* fallMotionState =
                                new btDefaultMotionState(
                                    btTransform(
                                        btQuaternion(0, 0, 0, 1),
                                        btVector3(10, 25, 10)
                                    )
                                );

                            btScalar mass = 1;
                            btVector3 fallInertia(0, 0, 0);
                            fallShape->calculateLocalInertia(mass, fallInertia);
                            btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, fallMotionState, fallShape, fallInertia);
                            btRigidBody* fallRigidBody = new btRigidBody(fallRigidBodyCI);
                            dynamicsWorld->addRigidBody(fallRigidBody);

                            btRigidBody::btRigidBodyConstructionInfo heightmapInfo(
                                0.0f,
                                new btDefaultMotionState(
                                    btTransform(
                                        btQuaternion(0, 0, 0, 1),
                                        btVector3(0, 0, 0)
                                    )
                                ),
                                heightmap.generateCollisionMesh(),
                                btVector3()
                            );
                            btRigidBody* heightmapBody = new btRigidBody(heightmapInfo);
                            dynamicsWorld->addRigidBody(heightmapBody);

                            Uint32 framesPerSecond = 0, timeStep = 0;

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

                                while(timeStep > TIME_STEP_LENGTH)
                                {
                                    timeStep -= TIME_STEP_LENGTH;

                                    const Uint8 *keyState = SDL_GetKeyboardState(nullptr);

                                    if(keyState[SDL_SCANCODE_UP])
                                        camera.panForward();
                                    if(keyState[SDL_SCANCODE_DOWN])
                                        camera.panBackward();
                                    if(keyState[SDL_SCANCODE_LEFT])
                                        camera.panLeft();
                                    if(keyState[SDL_SCANCODE_RIGHT])
                                        camera.panRight();

                                    btTransform trans;
                                    dynamicsWorld->stepSimulation(1.0/60.0f, 100);
                                    fallRigidBody->getMotionState()->getWorldTransform(trans);

                                }

                                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                                for(int error = glGetError(); error != GL_NO_ERROR; error = glGetError())
                                {
                                    std::cerr << "An OpenGL eror has occured: " << error << std::endl;
                                }

                                graphics.begin3D();
                                    camera.applyView(graphics);

                                    btTransform transform;
                                    fallRigidBody->getMotionState()->getWorldTransform(transform);
                                    glm::vec3 translate(
                                        transform.getOrigin().getX(),
                                        transform.getOrigin().getY(),
                                        transform.getOrigin().getZ()
                                    );
                                    glm::quat rotate(
                                        transform.getRotation().getW(),
                                        transform.getRotation().getX(),
                                        transform.getRotation().getY(),
                                        transform.getRotation().getZ()
                                    );
                                    sphereMesh.draw(graphics, translate, rotate);

                                    teapotMesh.draw(graphics, glm::vec3(200, 200, 200));
                                    soldierMesh.draw(graphics, glm::vec3(0, 0, 0));
                                    heightmap.draw(graphics);
                                    test.draw(graphics, 0);
                                graphics.end3D();

                                graphics.begin2D();
                                    greetingText.draw(graphics);
                                    framesPerSecondMesh.draw(graphics, glm::vec2(0.0f, FONT_SIZE * 1.0f));
                                    console.draw(graphics);
                                graphics.end2D();

                                font.renderString(framesPerSecondMesh, std::to_string(framesPerSecond).data());

                                SDL_GL_SwapWindow(window);

                                frameTime = SDL_GetTicks() - frameTime;
                                framesPerSecond = frameTime ? (1000 / frameTime) : 1000;
                                timeStep += frameTime;
                            }

                            dynamicsWorld->removeRigidBody(fallRigidBody);
                            delete fallRigidBody->getMotionState();
                            delete fallRigidBody;

                            delete fallShape;

                            delete groundShape;


                            delete dynamicsWorld;
                            delete solver;
                            delete collisionConfiguration;
                            delete dispatcher;
                            delete broadphase;

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
