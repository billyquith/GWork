/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_CONTROLS_BASE_H
#define GWK_CONTROLS_BASE_H

#include <list>
#include <map>
#include <algorithm>

#include "Gwork/Exports.h"
#include "Gwork/Structures.h"
#include "Gwork/BaseRender.h"
#include "Gwork/Events.h"
#include "Gwork/Utility.h"
#include "Gwork/Skin.h"
#include "Gwork/ControlList.h"
#include "Gwork/UserData.h"

namespace Gwk
{
    namespace Docking
    {
        enum Area
        {
            None        = 0,
            Left        = (1<<1),
            Right       = (1<<2),
            Top         = (1<<3),
            Bottom      = (1<<4),
            CenterV     = (1<<5),
            CenterH     = (1<<6),
            Fill        = (1<<7),
            Center      = CenterV | CenterH
        };
        
    } // Pos

    class TextObject;

    namespace Skin
    {
        class Base;
    }

    namespace Controls
    {
        class Canvas;

        //
        //! This is the base class for all Gwork controls.
        //
        class GWK_EXPORT Base : public Event::Handler
        {
        public:

            typedef std::list<Base*> List;

            typedef std::map<Gwk::String, Gwk::Event::Caller*> AccelMap;

            Base(Base* pParent, const Gwk::String& Name = "");
            virtual ~Base();
            
            static const char* GetTypeNameStatic() { return "Base"; }
            
            virtual const char* GetTypeName() const { return GetTypeNameStatic(); }
            virtual const char* GetParentTypeName() const { return NULL; }
            
            virtual void DelayedDelete();
            virtual void PreDelete(Gwk::Skin::Base* skin) {}

            virtual void            SetParent(Controls::Base* pParent);
            virtual Controls::Base* GetParent() const
            {
                return m_Parent;
            }

            virtual Controls::Canvas* GetCanvas();

            virtual Base::List& GetChildren()
            {
                if (m_InnerPanel)
                    return m_InnerPanel->GetChildren();

                return Children;
            }

            virtual bool            IsChild(Controls::Base* pChild);
            virtual unsigned int    NumChildren();
            virtual Controls::Base* GetChild(unsigned int i);
            virtual bool            SizeToChildren(bool w = true, bool h = true);
            virtual Gwk::Point     ChildrenSize();
            virtual Controls::Base* FindChildByName(const Gwk::String& name,
                                                    bool bRecursive = false);

            template <class T>
            T* FindChild(const Gwk::String& name, bool bRecursive = false);

            virtual void SetName(const Gwk::String& name)  { m_Name = name; }
            virtual const Gwk::String& GetName()           { return m_Name; }

            virtual void Think() {}

        protected:

            virtual void AddChild(Controls::Base* pChild);
            virtual void RemoveChild(Controls::Base* pParent);
            virtual void OnChildAdded(Controls::Base* pChild);
            virtual void OnChildRemoved(Controls::Base* pChild);

        public:

            virtual void RemoveAllChildren();

            virtual void SendToBack(void);
            virtual void BringToFront(void);
            virtual void BringNextToControl(Controls::Base* pChild, bool bBehind);

            virtual Gwk::Point LocalPosToCanvas(const Gwk::Point& in = Point(0, 0));
            virtual Gwk::Point CanvasPosToLocal(const Gwk::Point& in);

            virtual void Dock(Docking::Area dock);
            virtual Docking::Area GetDock() const;

            virtual void RestrictToParent(bool restrict)    { m_bRestrictToParent = restrict; }
            virtual bool ShouldRestrictToParent()           { return m_bRestrictToParent; }

            virtual int X() const       { return m_Bounds.x; }  //!< Get X position of Control.
            virtual int Y() const       { return m_Bounds.y; }  //!< Get Y position of Control.

            virtual int Width() const   { return m_Bounds.w; }  //!< Get Control width.
            virtual int Height() const  { return m_Bounds.h; }  //!< Get Control height.

            virtual int Bottom() const
            {
                return m_Bounds.y+m_Bounds.h+m_Margin.bottom;
            }

            //! Get right boundary of control.
            virtual int Right() const
            {
                return m_Bounds.x+m_Bounds.w+m_Margin.right;
            }

            //! Get the margin of the control. 
            virtual const Margin& GetMargin() const     { return m_Margin; }
            
            //! Get the padding for the control.
            virtual const Padding& GetPadding() const   { return m_Padding; }

            virtual void SetPos(int x, int y);
            virtual void SetPos(const Point& p)     { return SetPos(p.x, p.y); }
            virtual Point GetPos() const            { return Point(X(), Y()); }

            virtual void SetWidth(int w)            { SetSize(w, Height()); }
            virtual void SetHeight(int h)           { SetSize(Width(), h); }

            virtual bool  SetSize(int w, int h);
            virtual bool  SetSize(const Point& p);
            virtual Point GetSize() const           { return Point(Width(), Height()); }

            virtual bool SetBounds(int x, int y, int w, int h);
            virtual bool SetBounds(const Gwk::Rect& bounds);

            virtual void SetPadding(const Padding& padding);
            virtual void SetMargin(const Margin& margin);

            //! MoveTo is identical to SetPos except it uses ShouldRestrictToParent().
            virtual void MoveTo(int x, int y);
            virtual void MoveBy(int x, int y);

            virtual const Gwk::Rect& GetBounds() const { return m_Bounds; }

            virtual Controls::Base* GetControlAt(int x, int y, bool bOnlyIfMouseEnabled = true);

        protected:

            virtual void OnBoundsChanged(Gwk::Rect oldBounds);
            virtual void OnChildBoundsChanged(Gwk::Rect oldChildBounds, Base* pChild);

            virtual void OnScaleChanged();

        public:

            //! InnerBounds is the area inside the control that
            //! doesn't have child controls docked to it.
            virtual const Gwk::Rect& GetInnerBounds() const { return m_InnerBounds; }

        protected:

            Gwk::Rect m_InnerBounds;

        public:

            virtual const Gwk::Rect& GetRenderBounds() const { return m_RenderBounds; }

        protected:

            virtual void UpdateRenderBounds();

        public:

            virtual void DoRender(Gwk::Skin::Base* skin);
            virtual void DoCacheRender(Gwk::Skin::Base* skin, Gwk::Controls::Base* pMaster);
            virtual void RenderRecursive(Gwk::Skin::Base* skin, const Gwk::Rect& cliprect);

            virtual bool ShouldClip()   { return true; }

        protected:

            virtual void Render(Gwk::Skin::Base* skin);
            virtual void RenderUnder(Gwk::Skin::Base* /*skin*/)    {}
            virtual void RenderOver(Gwk::Skin::Base* /*skin*/)     {}
            virtual void RenderFocus(Gwk::Skin::Base* /*skin*/);

        public:

            virtual void SetHidden(bool hidden)
            {
                if (m_bHidden == hidden)
                    return;

                m_bHidden = hidden;
                Invalidate();
                Redraw();
            }

            virtual bool Hidden() const;    //!< Returns true only if this control is hidden.
            virtual bool Visible() const;   //!< Returns false if this control or its parents are hidden.
            
            virtual void Hide()     { SetHidden(true); }    //!< Make control invisible.
            virtual void Show()     { SetHidden(false); }   //!< Make control visible if hidden.

            // Skin
            virtual void              SetSkin(Skin::Base* skin, bool doChildren = false);
            virtual Gwk::Skin::Base* GetSkin(void);

            // Background drawing
            virtual bool ShouldDrawBackground()
            {
                return m_bDrawBackground;
            }

            virtual void SetShouldDrawBackground(bool b)
            {
                m_bDrawBackground = b;
            }

        protected:

            virtual void OnSkinChanged(Gwk::Skin::Base* newSkin);

        public:

            virtual void OnMouseMoved(int x, int y, int deltaX, int deltaY);
            virtual bool OnMouseWheeled(int iDelta);
            
            virtual void OnMouseClickLeft(int /*x*/, int /*y*/, bool /*bDown*/)     {}
            virtual void OnMouseClickRight(int /*x*/, int /*y*/, bool /*bDown*/)    {}

            virtual void OnMouseDoubleClickLeft(int x, int y)
            {
                OnMouseClickLeft(x, y, true);
            }

            virtual void OnMouseDoubleClickRight(int x, int y)
            {
                OnMouseClickRight(x, y, true);
            }
            
            virtual void OnMouseEnter();
            virtual void OnMouseLeave();

            virtual void OnLostKeyboardFocus()              {}
            virtual void OnKeyboardFocus()                  {}

            virtual void SetMouseInputEnabled(bool b)       { m_bMouseInputEnabled = b; }
            virtual bool GetMouseInputEnabled()             { return m_bMouseInputEnabled; }
            
            virtual void SetKeyboardInputEnabled(bool b)    { m_bKeyboardInputEnabled = b; }
            virtual bool GetKeyboardInputEnabled() const    { return m_bKeyboardInputEnabled; }

            virtual bool NeedsInputChars()                  { return false; }

            virtual bool OnChar(Gwk::UnicodeChar /*c*/)    { return false; }

            virtual bool OnKeyPress(int iKey, bool bPress = true);
            virtual bool OnKeyRelease(int iKey);

            virtual void OnPaste(Controls::Base* /*pFrom*/)         {}
            virtual void OnCopy(Controls::Base* /*pFrom*/)          {}
            virtual void OnCut(Controls::Base* /*pFrom*/)           {}
            virtual void OnSelectAll(Controls::Base* /*pFrom*/)     {}

            virtual bool OnKeyTab(bool bDown);
            virtual bool OnKeySpace(bool /*bDown*/)         { return false; }
            virtual bool OnKeyReturn(bool /*bDown*/)        { return false; }
            virtual bool OnKeyBackspace(bool /*bDown*/)     { return false; }
            virtual bool OnKeyDelete(bool /*bDown*/)        { return false; }
            virtual bool OnKeyRight(bool /*bDown*/)         { return false; }
            virtual bool OnKeyLeft(bool /*bDown*/)          { return false; }
            virtual bool OnKeyHome(bool /*bDown*/)          { return false; }
            virtual bool OnKeyEnd(bool /*bDown*/)           { return false; }
            virtual bool OnKeyUp(bool /*bDown*/)            { return false; }
            virtual bool OnKeyDown(bool /*bDown*/)          { return false; }
            virtual bool OnKeyEscape(bool /*bDown*/)        { return false; }
            
            virtual bool IsHovered();
            virtual bool ShouldDrawHover();

            virtual void Touch();
            virtual void OnChildTouched(Controls::Base* pChild);

            virtual bool IsOnTop();

            virtual bool HasFocus();
            virtual void Focus();
            virtual void Blur();

            // Other
            virtual void SetDisabled(bool active)
            {
                if (m_bDisabled == active)
                    return;

                m_bDisabled = active; Redraw();
            }

            virtual bool IsDisabled()
            {
                return m_bDisabled;
            }

            virtual void Redraw()
            {
                UpdateColours();
                m_bCacheTextureDirty = true;

                if (m_Parent)
                    m_Parent->Redraw();
            }

            virtual void UpdateColours()    {}

            //! Enable caching to texture optimisation for this control.
            //! @Note Must have GetCTT() implemented in the Renderer.
            virtual void EnableCacheToTexture()     { m_bCacheToTexture = true; }
            
            //! Query if this control is cached to a texture.
            virtual bool IsCachedToTexture() const  { return m_bCacheToTexture; }

            virtual void SetCursor(unsigned char c) { m_Cursor = c; }

            virtual void UpdateCursor();

            virtual Gwk::Point GetMinimumSize()
            {
                return Gwk::Point(1, 1);
            }

            virtual Gwk::Point GetMaximumSize()
            {
                return Gwk::Point(4096, 4096);
            }

            virtual void SetToolTip(const Gwk::String& strText);
            virtual void SetToolTip(Base* tooltip)
            {
                m_ToolTip = tooltip;
                if (m_ToolTip)
                {
                    m_ToolTip->SetParent(this);
                    m_ToolTip->SetHidden(true);
                }
            }

            virtual Base* GetToolTip()
            {
                return m_ToolTip;
            }

            virtual bool IsMenuComponent();
            virtual void CloseMenus();

            virtual bool IsTabable()                    { return m_Tabable; }
            virtual void SetTabable(bool isTabable)     { m_Tabable = isTabable; }

            // Accelerator functionality
            void DefaultAccel(Gwk::Controls::Base* /*pCtrl*/)
            {
                AcceleratePressed();
            }

            virtual void AcceleratePressed()    {}
            virtual bool AccelOnlyFocus()       { return false; }

            virtual bool HandleAccelerator(Gwk::String& accelerator);

            template <typename T>
            void AddAccelerator(const String& accelerator, T func,
                                Gwk::Event::Handler* handler = NULL)
            {
                if (handler == NULL)
                    handler = this;

                Gwk::Event::Caller* caller = new Gwk::Event::Caller();
                caller->Add(handler, func);
                Gwk::String str = accelerator;
                Gwk::Utility::Strings::ToUpper(str);
                Gwk::Utility::Strings::Strip(str, " ");
                m_Accelerators[ str ] = caller;
            }

            void AddAccelerator(const String& accelerator)
            {
                AddAccelerator(accelerator, &Base::DefaultAccel, this);
            }

            AccelMap m_Accelerators;

            // Default Events

            Gwk::Event::Caller onHoverEnter;
            Gwk::Event::Caller onHoverLeave;

            // Childrens List

            Base::List Children;

        protected:

            //! The logical parent.
            //! It's usually what you expect, the control you've parented it to.
            Base* m_Parent;

            //! If the InnerPanel exists our children will automatically
            //! become children of that instead of us - allowing us to move
            //! them all around by moving that panel (useful for scrolling etc).
            Base* m_InnerPanel;
            
            virtual Base* Inner()   { return m_InnerPanel; }

            //! This is the panel's actual parent - most likely the logical
            //! parent's InnerPanel (if it has one). You should rarely need this.
            Base* m_ActualParent;

            Base* m_ToolTip;

            Skin::Base* m_Skin;

            Gwk::Rect m_Bounds;
            Gwk::Rect m_RenderBounds;

            Padding m_Padding;
            Margin m_Margin;

            Gwk::String m_Name;

            bool m_bRestrictToParent;
            bool m_bDisabled;
            bool m_bHidden;
            bool m_bMouseInputEnabled;
            bool m_bKeyboardInputEnabled;
            bool m_bDrawBackground;

            Docking::Area m_iDock;

            unsigned char m_Cursor;

            bool m_Tabable;

        public:

            bool NeedsLayout()
            {
                return m_bNeedsLayout;
            }

            void Invalidate();
            void InvalidateParent()
            {
                if (m_Parent)
                    m_Parent->Invalidate();
            }

            void InvalidateChildren(bool bRecursive = false);
            void Position(unsigned int pos, int xpadding = 0, int ypadding = 0);

        protected:

            virtual void RecurseLayout(Skin::Base* skin);
            virtual void Layout(Skin::Base* skin);
            virtual void PostLayout(Skin::Base* /*skin*/)
            {
            }

            bool m_bNeedsLayout;
            bool m_bCacheTextureDirty;
            bool m_bCacheToTexture;

            //
            // Drag + Drop

        public:

            // Giver

            virtual void DragAndDrop_SetPackage(bool bDraggable, const String& strName = "",
                                                void* pUserData = NULL);
            virtual bool DragAndDrop_Draggable();
            virtual bool DragAndDrop_ShouldStartDrag()  { return true; }

            virtual void DragAndDrop_StartDragging(Gwk::DragAndDrop::Package* pPackage, int x, int y);
            virtual Gwk::DragAndDrop::Package* DragAndDrop_GetPackage(int x, int y);
            virtual void DragAndDrop_EndDragging(bool /*bSuccess*/, int /*x*/, int /*y*/) {}

        protected:

            DragAndDrop::Package*   m_DragAndDrop_Package;

        public:

            // Receiver
            virtual void DragAndDrop_HoverEnter(Gwk::DragAndDrop::Package* /*pPackage*/,
                                                int /*x*/, int /*y*/)
            {
            }

            virtual void DragAndDrop_HoverLeave(Gwk::DragAndDrop::Package* /*pPackage*/)
            {
            }

            virtual void DragAndDrop_Hover(Gwk::DragAndDrop::Package* /*pPackage*/,
                                           int /*x*/, int /*y*/)
            {
            }

            virtual bool DragAndDrop_HandleDrop(Gwk::DragAndDrop::Package* pPackage, int x, int y);
            virtual bool DragAndDrop_CanAcceptPackage(Gwk::DragAndDrop::Package* /*pPackage*/)
            {
                return false;
            }

            //
            // Useful anim shortcuts
            //

        public:

#ifndef GWK_NO_ANIMATION

            virtual void Anim_WidthIn(float fLength, float fDelay = 0.0f, float fEase = 1.0f);
            virtual void Anim_HeightIn(float fLength, float fDelay = 0.0f, float fEase = 1.0f);
            virtual void Anim_WidthOut(float fLength, bool bHide = true, float fDelay = 0.0f,
                                       float fEase = 1.0f);
            virtual void Anim_HeightOut(float fLength, bool bHide = true, float fDelay = 0.0f,
                                        float fEase = 1.0f);

#endif

            //
            // Dynamic casting, see gwk_cast below
            //

        public:

            static const char* GetIdentifier()
            {
                return GetTypeNameStatic();
            }

            virtual Gwk::Controls::Base* DynamicCast(const char* Variable)
            {
                return NULL;
            }

        public:

            void DoNotIncludeInSize()       { m_bIncludeInSize = false; }
            bool ShouldIncludeInSize()      { return m_bIncludeInSize; }

        protected:

            bool m_bIncludeInSize;

        public:

            virtual String GetChildValue(const Gwk::String& strName);
            virtual String GetValue();
            virtual void       SetValue(const String& strValue);
            virtual void       DoAction() {}

            virtual void SetAction(Event::Handler* pObject,
                                   Handler::FunctionWithInformation pFunction,
                                   const Gwk::Event::Packet& packet)
            {
            }

            virtual int GetNamedChildren(Gwk::ControlList& list, const Gwk::String& strName,
                                         bool bDeep = true);
            virtual Gwk::ControlList GetNamedChildren(const Gwk::String& strName,
                                                       bool bDeep = true);

        public:

            UserDataStorage UserData;

        };


    }

    /**
     *  To avoid using dynamic_cast we have gwk_cast.
     *
     *  Each class in Gwork includes GWK_DYNAMIC. You don't have to include this
     *  macro anywhere as it's automatically included in the GWK_CONTROL macro.
     *
     *  GWK_DYNAMIC adds 2 functions:
     *
     *  * GetIdentifier() :-
     *      a static function with a static variable inside, which returns
     *      the address of the static variable. The variable is defined
     *      as a string containing "BASECLASSNAME:CLASSNAME". This string
     *      should be as unique as possible - or the compiler might optimize the
     *      variables together - which means that when this function returns the
     *      address it could be the same on one or more variables. Something to
     *      bear in mind.
     *
     *  * DynamicCast :-
     *      Non static, takes an address returned by GetIdentifier().
     *      Will return an address of a control if the control can safely be cast to
     *      the class from which the identifier was taken.
     *
     *  Really you shouldn't actually have to concern yourself with that stuff.
     *  The only thing you should use in theory is gwk_cast - which is used
     *  just the same as dynamic cast - except for one difference. We pass in
     *  the class name, not a pointer to the class:
     *
     *      gwk_cast<MyControl>(control)
     *      dynamic_cast<MyControl*>(control)
     *
     */
    template <class T>
    inline T* gwk_cast(Gwk::Controls::Base* p)
    {
        if (!p)
            return NULL;

        Gwk::Controls::Base* pReturn = p->DynamicCast(T::GetIdentifier());

        if (!pReturn)
            return NULL;

        return static_cast<T*>(pReturn);
    }

    template <class T>
    inline T* Controls::Base::FindChild(const Gwk::String& name, bool bRecursive)
    {
        return gwk_cast<T>(FindChildByName(name, bRecursive));
    }

#define GWK_DYNAMIC(THISNAME, BASENAME) \
    static const char* GetIdentifier()                                  \
    {                                                                   \
        static const char* ident = #BASENAME ":" #THISNAME;             \
        return ident;                                                   \
    }                                                                   \
    virtual Gwk::Controls::Base* DynamicCast(const char* Variable) override \
    {                                                                   \
        if (GetIdentifier() == Variable)                                \
            return this;                                                \
                                                                        \
        return ParentClass::DynamicCast(Variable);                        \
    }

#define GWK_CLASS(THISNAME, BASENAME) \
    typedef BASENAME ParentClass; \
    typedef THISNAME ThisClass;

// To be placed in the controls .h definition.
#define GWK_CONTROL(THISNAME, BASENAME) \
public: \
    GWK_CLASS(THISNAME, BASENAME)  \
    GWK_DYNAMIC(THISNAME, BASENAME) \
    static  const char* GetTypeNameStatic() { return #THISNAME; } \
    virtual const char* GetTypeName() const override { return GetTypeNameStatic(); } \
    virtual const char* GetParentTypeName() const override { return ParentClass::GetTypeNameStatic(); } \
    THISNAME(Gwk::Controls::Base* pParent, const Gwk::String& pName = "")

#define GWK_CONTROL_INLINE(THISNAME, BASENAME) \
    GWK_CONTROL(THISNAME, BASENAME) : ParentClass(pParent, pName)

#define GWK_CONTROL_CONSTRUCTOR(THISNAME) \
    THISNAME::THISNAME(Gwk::Controls::Base* pParent, const Gwk::String& pName) \
        : ParentClass(pParent, pName)

} // namespace Gwk

#endif // ifndef GWK_CONTROLS_BASE_H
