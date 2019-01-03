/*
 *  Gwork
 *  Copyright (c) 2013-2018 Billy Quith
 *  See license in Gwork.h
 */

#include <irrlicht.h>

#include <Gwork/Gwork.h>
#include <Gwork/Skins/Simple.h>
#include <Gwork/Skins/TexturedBase.h>
#include <Gwork/Test/Test.h>
#include <Gwork/Input/Irrlicht.h>
#include <Gwork/Renderers/Irrlicht.h>
#include <Gwork/Platform.h>

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#ifdef _IRR_WINDOWS_
#   pragma comment(lib, "Irrlicht.lib")
#endif

class MyEventReceiver : public IEventReceiver
{
public:
    MyEventReceiver(Gwk::IInputEventListener* el) : input()
    {
        input.Initialize(el);
    }

    virtual bool OnEvent(const SEvent& event)
    {
        return input.HandleEvents(event);
    }

private:
    Gwk::Input::Irrlicht input;
};

/*
This is the main method. We can now use main() on every platform.
*/
int main()
{
    /*
    The most important function of the engine is the createDevice()
    function. The IrrlichtDevice is created by it, which is the root
    object for doing anything with the engine. createDevice() has 7
    parameters:
    - deviceType: Type of the device. This can currently be the Null-device,
    one of the two software renderers, D3D8, D3D9, or OpenGL.
    - windowSize: Size of the Window or screen in FullScreenMode to be
    created. In this example we use 1004x650.
    - bits: Amount of color bits per pixel. This should be 16 or 32. The
    parameter is often ignored when running in windowed mode.
    - fullscreen: Specifies if we want the device to run in fullscreen mode
    or not.
    - stencilbuffer: Specifies if we want to use the stencil buffer (for
    drawing shadows).
    - vsync: Specifies if we want to have vsync enabled, this is only useful
    in fullscreen mode.
    - eventReceiver: An object to receive events.
    Always check the return value to cope with unsupported drivers,
    dimensions, etc.
    */
    IrrlichtDevice *device =
        createDevice(video::EDT_OPENGL, dimension2d<u32>(1004, 650), 32,
            false, false, false, NULL);

    if (!device)
        return 1;

    /*
    Set the caption of the window to some nice text. Note that there is an
    'L' in front of the string. The Irrlicht Engine uses wide character
    strings when displaying text.
    */
    device->setWindowCaption(L"Gwork - Irrlicht Sample");

    /*
    Get a pointer to the VideoDriver, the SceneManager and the graphical
    user interface environment, so that we do not always have to write
    device->getVideoDriver(), device->getSceneManager(), or
    device->getGUIEnvironment().
    */
    IVideoDriver* driver = device->getVideoDriver();
    ISceneManager* smgr = device->getSceneManager();
    //IGUIEnvironment* guienv = device->getGUIEnvironment();

    //
    // Create a Gwork Irrlicht Renderer
    //
    Gwk::Platform::RelativeToExecutablePaths paths(GWK_SAMPLE_RESOURCE_DIR);

    Gwk::Renderer::Irrlicht* pRenderer = new Gwk::Renderer::Irrlicht(paths, device);
    //
    // Create a Gwork skin
    //
    Gwk::Skin::TexturedBase* pSkin = new Gwk::Skin::TexturedBase(pRenderer);
    pSkin->Init("DefaultSkin.png");
    pSkin->SetDefaultFont("OpenSans.ttf", 11);
    //
    // Create a Canvas (it's root, on which all other Gwork panels are created)
    //
    Gwk::Controls::Canvas* pCanvas = new Gwk::Controls::Canvas(pSkin);
    pCanvas->SetSize(998, 650 - 24);
    pCanvas->SetDrawBackground(true);
    pCanvas->SetBackgroundColor(Gwk::Color(150, 0, 0, 255));
    //  Texture caching doesn't seem to work properly.
    //  Cached textures never get updated.
    //pCanvas->EnableCacheToTexture();
    //
    // Create our unittest control (which is a Window with controls in it)
    //
    auto pUnit = new TestFrame(pCanvas);
    pUnit->SetPos(10, 10);

    MyEventReceiver* eventReceiver = new MyEventReceiver(pCanvas);
    device->setEventReceiver(eventReceiver);

    int lastFPS = -1;
    while (device->run())
    {
        driver->beginScene(true, true, SColor(255, 100, 101, 140));

        smgr->drawAll();

        pCanvas->RenderCanvas();

        driver->endScene();
        int fps = driver->getFPS();

        if (lastFPS != fps)
        {
            core::stringw str = L"Irrlicht Engine [";
            str += driver->getName();
            str += "] FPS:";
            str += fps;

            device->setWindowCaption(str.c_str());
            lastFPS = fps;
        }
    }

    delete eventReceiver;
    delete pCanvas;
    delete pSkin;
    delete pRenderer;
    /*
    After we are done with the render loop, we have to delete the Irrlicht
    Device created before with createDevice(). In the Irrlicht Engine, you
    have to delete all objects you created with a method or function which
    starts with 'create'. The object is simply deleted by calling ->drop().
    See the documentation at irr::IReferenceCounted::drop() for more
    information.
    */
    device->drop();

    return 0;
}
