/*
 *  Gwork
 *  Copyright (c) 2012 Facepunch Studios
 *  Copyright (c) 2013-2016 Billy Quith
 *  See license in Gwork.h
 */

#include <Gwork/Platform.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>

#include <Gwork/Input/Allegro5.h>


static Gwk::Input::Allegro      g_gworkInput;
static ALLEGRO_EVENT_QUEUE*     g_event_queue = nullptr;
static ALLEGRO_DISPLAY*         g_display = nullptr;

static const ALLEGRO_SYSTEM_MOUSE_CURSOR g_cursorConversion[] =
{
    ALLEGRO_SYSTEM_MOUSE_CURSOR_ARROW,      // IDC_ARROW
    ALLEGRO_SYSTEM_MOUSE_CURSOR_EDIT,       // IDC_IBEAM
    ALLEGRO_SYSTEM_MOUSE_CURSOR_RESIZE_N,   // IDC_SIZENS
    ALLEGRO_SYSTEM_MOUSE_CURSOR_RESIZE_E,   // IDC_SIZEWE
    ALLEGRO_SYSTEM_MOUSE_CURSOR_RESIZE_N,   // IDC_SIZENWSE
    ALLEGRO_SYSTEM_MOUSE_CURSOR_RESIZE_E,   // IDC_SIZENESW
    ALLEGRO_SYSTEM_MOUSE_CURSOR_MOVE,       // IDC_SIZEALL
    ALLEGRO_SYSTEM_MOUSE_CURSOR_UNAVAILABLE,// IDC_NO
    ALLEGRO_SYSTEM_MOUSE_CURSOR_BUSY,       // IDC_WAIT
    ALLEGRO_SYSTEM_MOUSE_CURSOR_LINK        // IDC_HAND
};


void Gwk::Platform::Sleep(unsigned int iMS)
{
    al_rest(iMS*0.001);
}

void Gwk::Platform::SetCursor(unsigned char iCursor)
{
    al_set_system_mouse_cursor(g_display, g_cursorConversion[iCursor]);
}

Gwk::String Gwk::Platform::GetClipboardText()
{
    Gwk::String str;

    if (al_clipboard_has_text(g_display) )
    {
        char* clip = al_get_clipboard_text(g_display);

        if (clip != nullptr)
        {
            str = clip;
            al_free(clip);
        }
    }

    return str;
}

bool Gwk::Platform::SetClipboardText(const Gwk::String& str)
{
    return al_set_clipboard_text(g_display, str.c_str());
}

float Gwk::Platform::GetTimeInSeconds()
{
    return al_get_time();
}

bool Gwk::Platform::FileOpen(const String& Name,
                             const String& StartPath,
                             const String& Extension,
                             String& filePathOut)
{
    ALLEGRO_FILECHOOSER* chooser =
        al_create_native_file_dialog(StartPath.c_str(),
                                     Name.c_str(),
                                     "*.*",  //
                                     // Extension.c_str(),
                                     ALLEGRO_FILECHOOSER_FILE_MUST_EXIST);

    if (al_show_native_file_dialog(g_display, chooser))
    {
        if (al_get_native_file_dialog_count(chooser) != 0)
        {
            filePathOut = al_get_native_file_dialog_path(chooser, 0);
        }
    }

    al_destroy_native_file_dialog(chooser);
    return true;
}

bool Gwk::Platform::FileSave(const String& Name,
                             const String& StartPath,
                             const String& Extension,
                             String& filePathOut)
{
    ALLEGRO_FILECHOOSER* chooser = al_create_native_file_dialog(StartPath.c_str(),
                                                                Name.c_str(),
                                                                "*.*", // Extension.c_str(),
                                                                ALLEGRO_FILECHOOSER_SAVE);

    if (al_show_native_file_dialog(g_display, chooser))
    {
        if (al_get_native_file_dialog_count(chooser) != 0)
        {
            filePathOut = al_get_native_file_dialog_path(chooser, 0);
        }
    }

    al_destroy_native_file_dialog(chooser);
    return true;
}

bool Gwk::Platform::FolderOpen(const String& Name,
                               const String& StartPath,
                               String& filePathOut)
{
    ALLEGRO_FILECHOOSER* chooser = al_create_native_file_dialog(StartPath.c_str(),
                                                                Name.c_str(),
                                                                "*.*", // Extension.c_str(),
                                                                ALLEGRO_FILECHOOSER_FOLDER);

    if (al_show_native_file_dialog(g_display, chooser))
    {
        if (al_get_native_file_dialog_count(chooser) != 0)
        {
            filePathOut = al_get_native_file_dialog_path(chooser, 0);
        }
    }

    al_destroy_native_file_dialog(chooser);
    return true;
}

static bool InitAllegro()
{
    if (al_is_system_installed())
        return true;
    
	if (!al_init())
        return false;
	
    ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
    if (!event_queue)
        return false;
	
    al_init_image_addon();
    al_init_font_addon();
    al_init_primitives_addon();
    al_init_ttf_addon();
    al_install_mouse();
    al_install_keyboard();
    al_init_native_dialog_addon();
    al_register_event_source(event_queue, al_get_mouse_event_source());
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    
    return true;
}

void Gwk::Platform::SetPlatformWindow(void* handle)
{
    g_display = reinterpret_cast<ALLEGRO_DISPLAY*>(handle);
}

void* Gwk::Platform::CreatePlatformWindow(int x, int y, int w, int h,
                                          const Gwk::String& strWindowTitle)
{
    // Check Allegro has been initialised.
    if (!InitAllegro())
        return nullptr;

    al_set_new_window_position(x, y);
    al_set_new_display_flags(ALLEGRO_WINDOWED|ALLEGRO_FRAMELESS);
    ALLEGRO_DISPLAY* display = al_create_display(w, h);

    if (!display)
        return nullptr;

    g_display = display;
    al_set_window_title(display, strWindowTitle.c_str());     // invisible as
                                                              // frameless?
    g_event_queue = al_create_event_queue();

    if (!g_event_queue)
        return nullptr;

    al_init_image_addon();
    al_init_font_addon();
    al_init_primitives_addon();
    al_init_ttf_addon();
    al_install_mouse();
    al_install_keyboard();
    al_register_event_source(g_event_queue, al_get_display_event_source(display));
    al_register_event_source(g_event_queue, al_get_mouse_event_source());
    al_register_event_source(g_event_queue, al_get_keyboard_event_source());
    
    return display;
}

void Gwk::Platform::DestroyPlatformWindow(void* ptr)
{
    ALLEGRO_DISPLAY* display = (ALLEGRO_DISPLAY*)ptr;
    al_destroy_display(display);
    al_destroy_event_queue(g_event_queue);
    g_event_queue = nullptr;
}

bool Gwk::Platform::MessagePump(void* window)
{
//    static bool firstCall = true;
//    if (firstCall)
//    {
//        firstCall = false;
////        g_gworkInput.Initialize(ptarget);   TODO - initialise input to canvas
//    }

    ALLEGRO_EVENT ev;
    while (al_get_next_event(g_event_queue, &ev))
    {
        g_gworkInput.ProcessMessage(ev);
    }
    
    return false;
}

void Gwk::Platform::SetBoundsPlatformWindow(void* ptr, int x, int y, int w, int h)
{
    ALLEGRO_DISPLAY* display = (ALLEGRO_DISPLAY*)ptr;
    al_set_window_position(display, x, y);

    if (al_get_display_width(display) != w || al_get_display_height(display) != h)
        al_resize_display(display, w, h);
}

void Gwk::Platform::SetWindowMaximized(void* ptr, bool bMax, Gwk::Point& newPos,
                                        Gwk::Point& newSize)
{
    ALLEGRO_DISPLAY* display = (ALLEGRO_DISPLAY*)ptr;

    if (bMax)
    {
        // Go full screen allowing for any other screen real estate.
        ALLEGRO_MONITOR_INFO info;
        al_get_monitor_info(0, &info);
        int w = info.x2-info.x1, h = info.y2-info.y1;
#if defined (ALLEGRO_MACOSX)
        const int c_titleBarHeight = 20;
        al_resize_display(display, w, h-c_titleBarHeight);
        al_set_window_position(display, 0, c_titleBarHeight);
#else
        al_resize_display(display, w, h);
        al_set_window_position(display, 0, 0);
#endif
    }
    else
    {
        // Restore to a reasonable size.
        ALLEGRO_MONITOR_INFO info;
        al_get_monitor_info(0, &info);
        int w = info.x2-info.x1, h = info.y2-info.y1;
        al_resize_display(display, w/2, h/2);
        al_set_window_position(display, w/4, h/4);
    }

    al_get_window_position(display, &newPos.x, &newPos.y);
    newSize.x = al_get_display_width(display);
    newSize.y = al_get_display_height(display);
}

void Gwk::Platform::SetWindowMinimized(void* ptr, bool bMinimized)
{
}

bool Gwk::Platform::IsFocussedPlatformWindow(void* ptr)
{
    return true;
}

void Gwk::Platform::GetDesktopSize(int& w, int& h)
{
    if (!al_is_system_installed())
        al_init();

    ALLEGRO_MONITOR_INFO info;
    al_get_monitor_info(0, &info);
    w = info.x2-info.x1;
    h = info.y2-info.y1;
}

void Gwk::Platform::GetCursorPos(Gwk::Point& po)
{
    ALLEGRO_MOUSE_STATE mouse;
    al_get_mouse_state(&mouse);
    int wx, wy;
    al_get_window_position(g_display, &wx, &wy);
    po.x = mouse.x+wx;
    po.y = mouse.y+wy;
}

