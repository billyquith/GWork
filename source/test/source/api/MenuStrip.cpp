/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */

#include <Gwork/Test/TestAPI.h>
#include <Gwork/Controls/MenuStrip.h>

using namespace Gwk;

class MenuStrip : public TestUnit
{
public:

    GWK_CONTROL_INLINE(MenuStrip, TestUnit)
    {
        Dock(Position::Fill);
        Gwk::Controls::MenuStrip* menu = new Gwk::Controls::MenuStrip(this);
        {
            Gwk::Controls::MenuItem* root = menu->AddItem("File");
            root->GetMenu()->AddItem("New", "test16.png", "Ctrl + N")
                             ->SetAction(this, &ThisClass::MenuItemSelect);
            root->GetMenu()->AddItem("Load", "test16.png", "Ctrl+")
                             ->SetAction(this,&ThisClass::MenuItemSelect);
            root->GetMenu()->AddItem("Save", "", "CTRL+S")
                             ->SetAction(this, &ThisClass::MenuItemSelect);
            root->GetMenu()->AddItem("Save As..", "", "Ctrl+Shift+S")
                             ->SetAction(this, &ThisClass::MenuItemSelect);
            root->GetMenu()->AddItem("Quit", "", "Ctrl+Q")
                             ->SetAction(this, &ThisClass::MenuItemSelect);
        }
        {
            Gwk::Controls::MenuItem* root = menu->AddItem(
                Utility::Narrow(L"\u043F\u0438\u0440\u0430\u0442\u0441\u0442\u0432\u043E"));
            root->GetMenu()
                ->AddItem(Utility::Narrow(L"\u5355\u5143\u6D4B\u8BD5"))
                ->SetAction(this, &ThisClass::MenuItemSelect);
            root->GetMenu()
                ->AddItem(Utility::Narrow(L"\u0111\u01A1n v\u1ECB th\u1EED nghi\u1EC7m"))
                ->SetAction(this, &ThisClass::MenuItemSelect);
        }
        {
            Gwk::Controls::MenuItem* root = menu->AddItem("Submenu");
            Gwk::Controls::MenuItem* checkable = root->GetMenu()->AddItem("Checkable");
            checkable->SetCheckable(true);
            checkable->SetChecked(true);
            {
                Gwk::Controls::MenuItem* rootB = root->GetMenu()->AddItem("Two");
                rootB->GetMenu()->AddItem("Two.One");
                rootB->GetMenu()->AddItem("Two.Two");
                rootB->GetMenu()->AddItem("Two.Three");
                rootB->GetMenu()->AddItem("Two.Four");
                rootB->GetMenu()->AddItem("Two.Five");
                rootB->GetMenu()->AddItem("Two.Six");
                rootB->GetMenu()->AddItem("Two.Seven");
                rootB->GetMenu()->AddItem("Two.Eight");
                rootB->GetMenu()->AddItem("Two.Nine", "test16.png");
            }
            root->GetMenu()->AddItem("Three");
            root->GetMenu()->AddItem("Four");
            root->GetMenu()->AddItem("Five");
            {
                Gwk::Controls::MenuItem* rootB = root->GetMenu()->AddItem("Six");
                rootB->GetMenu()->AddItem("Six.One");
                rootB->GetMenu()->AddItem("Six.Two");
                rootB->GetMenu()->AddItem("Six.Three");
                rootB->GetMenu()->AddItem("Six.Four");
                rootB->GetMenu()->AddItem("Six.Five", "test16.png");
                {
                    Gwk::Controls::MenuItem* rootC = rootB->GetMenu()->AddItem("Six.Six");
                    rootC->GetMenu()->AddItem("Sheep");
                    rootC->GetMenu()->AddItem("Goose");
                    {
                        Gwk::Controls::MenuItem* rootD = rootC->GetMenu()->AddItem("Camel");
                        rootD->GetMenu()->AddItem("Eyes");
                        rootD->GetMenu()->AddItem("Nose");
                        {
                            Gwk::Controls::MenuItem* rootE = rootD->GetMenu()->AddItem("Hair");
                            rootE->GetMenu()->AddItem("Blonde");
                            rootE->GetMenu()->AddItem("Black");
                            {
                                Gwk::Controls::MenuItem* rootF =
                                    rootE->GetMenu()->AddItem("Red");
                                rootF->GetMenu()->AddItem("Light");
                                rootF->GetMenu()->AddItem("Medium");
                                rootF->GetMenu()->AddItem("Dark");
                            }
                            rootE->GetMenu()->AddItem("Brown");
                        }
                        rootD->GetMenu()->AddItem("Ears");
                    }
                    rootC->GetMenu()->AddItem("Duck");
                }
                rootB->GetMenu()->AddItem("Six.Seven");
                rootB->GetMenu()->AddItem("Six.Eight");
                rootB->GetMenu()->AddItem("Six.Nine");
            }
            root->GetMenu()->AddItem("Seven");
        }
    }


    void MenuItemSelect(Event::Info info)
    {
        auto* menuItem = static_cast<Gwk::Controls::MenuItem*>(info.ControlCaller);
        OutputToLog(Utility::Format("Menu Selected: %s", menuItem->GetText().c_str()));
    }

};


DECLARE_TEST(MenuStrip);
