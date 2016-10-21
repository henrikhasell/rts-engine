#include <irrlicht/irrlicht.h>

int main(int argc, char *argv[])
{
    irr::IrrlichtDevice *device = irr::createDevice(irr::video::EDT_OPENGL);

    irr::video::IVideoDriver *videoDriver = device->getVideoDriver();
    irr::scene::ISceneManager *sceneManager = device->getSceneManager();
    irr::gui::IGUIEnvironment *guiEnvironment = device->getGUIEnvironment();

    while(device->run() == true)
    {
        videoDriver->beginScene();
        sceneManager->drawAll();
        guiEnvironment->drawAll();
        videoDriver->endScene();
    }

    device->drop();

    return 0;
}
