/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */

#include <Gwork/Test/Test.h>
#include <Gwork/Controls/MenuStrip.h>

using namespace Gwk;

class MenuStrip : public GUnit
{
public:

    GWK_CONTROL_INLINE(MenuStrip, GUnit)
    {
        Dock(Docking::Fill);
        Gwk::Controls::MenuStrip* menu = new Gwk::Controls::MenuStrip(this);
        {
            Gwk::Controls::MenuItem* pRoot = menu->AddItem("File");
            pRoot->GetMenu()->AddItem("New", "test16.png", "Ctrl + N")->SetAction(this,
                                                                                    &ThisClass::MenuItemSelect);
            pRoot->GetMenu()->AddItem("Load", "test16.png", "Ctrl+")->SetAction(this,
                                                                                   &ThisClass::MenuItemSelect);
            pRoot->GetMenu()->AddItem("Save", "", "CTRL+S")->SetAction(this,
                                                                        &ThisClass::MenuItemSelect);
            pRoot->GetMenu()->AddItem("Save As..", "", "Ctrl+Shift+S")->SetAction(this,
                                                                                   &ThisClass::MenuItemSelect);
            pRoot->GetMenu()->AddItem("Quit", "", "Ctrl+Q")->SetAction(this,
                                                                        &ThisClass::MenuItemSelect);
        }
        {
            Gwk::Controls::MenuItem* pRoot = menu->AddItem(
                Utility::Narrow(L"\u043F\u0438\u0440\u0430\u0442\u0441\u0442\u0432\u043E"));
            pRoot->GetMenu()
                ->AddItem(Utility::Narrow(L"\u5355\u5143\u6D4B\u8BD5"))
                ->SetAction(this, &ThisClass::MenuItemSelect);
            pRoot->GetMenu()
                ->AddItem(Utility::Narrow(L"\u0111\u01A1n v\u1ECB th\u1EED nghi\u1EC7m"))
                ->SetAction(this, &ThisClass::MenuItemSelect);
        }
        {
            Gwk::Controls::MenuItem* pRoot = menu->AddItem("Submenu");
            Gwk::Controls::MenuItem* pCheckable = pRoot->GetMenu()->AddItem("Checkable");
            pCheckable->SetCheckable(true);
            pCheckable->SetChecked(true);
            {
                Gwk::Controls::MenuItem* pRootB = pRoot->GetMenu()->AddItem("Two");
                pRootB->GetMenu()->AddItem("Two.One");
                pRootB->GetMenu()->AddItem("Two.Two");
                pRootB->GetMenu()->AddItem("Two.Three");
                pRootB->GetMenu()->AddItem("Two.Four");
                pRootB->GetMenu()->AddItem("Two.Five");
                pRootB->GetMenu()->AddItem("Two.Six");
                pRootB->GetMenu()->AddItem("Two.Seven");
                pRootB->GetMenu()->AddItem("Two.Eight");
                pRootB->GetMenu()->AddItem("Two.Nine", "test16.png");
            }
            pRoot->GetMenu()->AddItem("Three");
            pRoot->GetMenu()->AddItem("Four");
            pRoot->GetMenu()->AddItem("Five");
            {
                Gwk::Controls::MenuItem* pRootB = pRoot->GetMenu()->AddItem("Six");
                pRootB->GetMenu()->AddItem("Six.One");
                pRootB->GetMenu()->AddItem("Six.Two");
                pRootB->GetMenu()->AddItem("Six.Three");
                pRootB->GetMenu()->AddItem("Six.Four");
                pRootB->GetMenu()->AddItem("Six.Five", "test16.png");
                {
                    Gwk::Controls::MenuItem* pRootC = pRootB->GetMenu()->AddItem("Six.Six");
                    pRootC->GetMenu()->AddItem("Sheep");
                    pRootC->GetMenu()->AddItem("Goose");
                    {
                        Gwk::Controls::MenuItem* pRootD = pRootC->GetMenu()->AddItem("Camel");
                        pRootD->GetMenu()->AddItem("Eyes");
                        pRootD->GetMenu()->AddItem("Nose");
                        {
                            Gwk::Controls::MenuItem* pRootE = pRootD->GetMenu()->AddItem("Hair");
                            pRootE->GetMenu()->AddItem("Blonde");
                            pRootE->GetMenu()->AddItem("Black");
                            {
                                Gwk::Controls::MenuItem* pRootF =
                                    pRootE->GetMenu()->AddItem("Red");
                                pRootF->GetMenu()->AddItem("Light");
                                pRootF->GetMenu()->AddItem("Medium");
                                pRootF->GetMenu()->AddItem("Dark");
                            }
                            pRootE->GetMenu()->AddItem("Brown");
                        }
                        pRootD->GetMenu()->AddItem("Ears");
                    }
                    pRootC->GetMenu()->AddItem("Duck");
                }
                pRootB->GetMenu()->AddItem("Six.Seven");
                pRootB->GetMenu()->AddItem("Six.Eight");
                pRootB->GetMenu()->AddItem("Six.Nine");
            }
            pRoot->GetMenu()->AddItem("Seven");
        }
    }


    void MenuItemSelect(Base* pControl)
    {
        Gwk::Controls::MenuItem* pMenuItem = (Gwk::Controls::MenuItem*)pControl;
        UnitPrint(Utility::Format("Menu Selected: %s", pMenuItem->GetText().c_str()));
    }

};


DEFINE_UNIT_TEST(MenuStrip);
