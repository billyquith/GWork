/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_CONTROLS_BASE_H
#define GWK_CONTROLS_BASE_H

#include <Gwork/Config.h>
#include <Gwork/UiTypes.h>
#include <Gwork/BaseRender.h>
#include <Gwork/Events.h>
#include <Gwork/Utility.h>
#include <Gwork/Skin.h>
#include <Gwork/ControlList.h>
#include <Gwork/UserData.h>
#include <list>
#include <map>
#include <algorithm>
#include <ponder/pondertype.hpp>

namespace Gwk
{    
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

            Base(Base* parent, const Gwk::String& Name = "");
            virtual ~Base();
            
            static const char* GetTypeNameStatic() { return "Base"; }
            
            virtual const char* GetTypeName() const { return GetTypeNameStatic(); }
            virtual const char* GetParentTypeName() const { return nullptr; }
            
            virtual void DelayedDelete();
            virtual void PreDelete(Gwk::Skin::Base* skin) {}

            virtual void            SetParent(Controls::Base* parent);
            virtual Controls::Base* GetParent() const
            {
                return m_parent;
            }

            virtual Controls::Canvas* GetCanvas();

            virtual Base::List& GetChildren()
            {
                if (m_innerPanel)
                    return m_innerPanel->GetChildren();

                return Children;
            }

            virtual bool            IsChild(Controls::Base* child);
            virtual unsigned int    NumChildren();
            virtual Controls::Base* GetChild(unsigned int i);
            virtual bool            SizeToChildren(bool w = true, bool h = true);
            virtual Gwk::Point     ChildrenSize();
            virtual Controls::Base* FindChildByName(const Gwk::String& name,
                                                    bool bRecursive = false);

            template <class T>
            T* FindChild(const Gwk::String& name, bool bRecursive = false);

            virtual void SetName(const Gwk::String& name)  { m_name = name; }
            virtual const Gwk::String& GetName()           { return m_name; }

            virtual void Think() {}

        protected:

            virtual void AddChild(Controls::Base* child);
            virtual void RemoveChild(Controls::Base* parent);
            virtual void OnChildAdded(Controls::Base* child);
            virtual void OnChildRemoved(Controls::Base* child);

        public:

            virtual void RemoveAllChildren();

            virtual void SendToBack(void);
            virtual void BringToFront(void);
            virtual void BringNextToControl(Controls::Base* child, bool bBehind);

            virtual Gwk::Point LocalPosToCanvas(const Gwk::Point& in = Point(0, 0));
            virtual Gwk::Point CanvasPosToLocal(const Gwk::Point& in);

            virtual void Dock(Position dock);
            virtual Position GetDock() const;

            virtual void RestrictToParent(bool restrict)    { m_bRestrictToParent = restrict; }
            virtual bool ShouldRestrictToParent()           { return m_bRestrictToParent; }

            virtual int X() const       { return m_bounds.x; }  //!< Get X position of Control.
            virtual int Y() const       { return m_bounds.y; }  //!< Get Y position of Control.

            virtual int Width() const   { return m_bounds.w; }  //!< Get Control width.
            virtual int Height() const  { return m_bounds.h; }  //!< Get Control height.

            virtual int Bottom() const
            {
                return m_bounds.y+m_bounds.h+m_margin.bottom;
            }

            //! Get right boundary of control.
            virtual int Right() const
            {
                return m_bounds.x+m_bounds.w+m_margin.right;
            }

            //! Get the margin of the control. 
            virtual const Margin& GetMargin() const     { return m_margin; }
            
            //! Get the padding for the control.
            virtual const Padding& GetPadding() const   { return m_padding; }

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

            virtual const Gwk::Rect& GetBounds() const { return m_bounds; }

            virtual Controls::Base* GetControlAt(int x, int y, bool bOnlyIfMouseEnabled = true);

        protected:

            virtual void OnBoundsChanged(Gwk::Rect oldBounds);
            virtual void OnChildBoundsChanged(Gwk::Rect oldChildBounds, Base* child);

            virtual void OnScaleChanged();

        public:

            //! InnerBounds is the area inside the control that
            //! doesn't have child controls docked to it.
            virtual const Gwk::Rect& GetInnerBounds() const { return m_innerBounds; }

        protected:

            Gwk::Rect m_innerBounds;

        public:

            virtual const Gwk::Rect& GetRenderBounds() const { return m_renderBounds; }

        protected:

            virtual void UpdateRenderBounds();

        public:

            virtual void DoRender(Gwk::Skin::Base* skin);
            virtual void DoCacheRender(Gwk::Skin::Base* skin, Gwk::Controls::Base* master);
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

            virtual void OnPaste(Event::Info info)         {}
            virtual void OnCopy(Event::Info info)          {}
            virtual void OnCut(Event::Info info)           {}
            virtual void OnSelectAll(Event::Info info)     {}

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
            virtual void OnChildTouched(Controls::Base* child);

            virtual bool IsOnTop();

            virtual bool IsFocussed();
            virtual void Focus();
            virtual void Unfocus();

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

                if (m_parent)
                    m_parent->Redraw();
            }

            virtual void UpdateColours()    {}

            //! Enable caching to texture optimisation for this control.
            //! @Note Must have GetCTT() implemented in the Renderer.
            virtual void EnableCacheToTexture()     { m_bCacheToTexture = true; }
            
            //! Query if this control is cached to a texture.
            virtual bool IsCachedToTexture() const  { return m_bCacheToTexture; }

            virtual void SetCursor(unsigned char c) { m_cursor = c; }

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
                m_toolTip = tooltip;
                if (m_toolTip)
                {
                    m_toolTip->SetParent(this);
                    m_toolTip->SetHidden(true);
                }
            }

            virtual Base* GetToolTip()
            {
                return m_toolTip;
            }

            virtual bool IsMenuComponent();
            virtual void CloseMenus();

            virtual bool IsTabable()                    { return m_tabable; }
            virtual void SetTabable(bool isTabable)     { m_tabable = isTabable; }

            // Accelerator functionality
            void DefaultAccel(Event::Info /*info*/)
            {
                AcceleratePressed();
            }

            virtual void AcceleratePressed()    {}
            virtual bool AccelOnlyFocus()       { return false; }

            virtual bool HandleAccelerator(Gwk::String& accelerator);

            template <typename T>
            void AddAccelerator(const String& accelerator,
                                T func,
                                Gwk::Event::Handler* handler = nullptr)
            {
                if (handler == nullptr)
                    handler = this;

                Gwk::Event::Caller* caller = new Gwk::Event::Caller();
                caller->Add(handler, func);
                Gwk::String str = accelerator;
                Gwk::Utility::Strings::ToUpper(str);
                Gwk::Utility::Strings::Strip(str, " ");
                m_accelerators[ str ] = caller;
            }

            void AddAccelerator(const String& accelerator)
            {
                AddAccelerator(accelerator, &Base::DefaultAccel, this);
            }

            AccelMap m_accelerators;

            // Default Events

            Gwk::Event::Caller onHoverEnter;
            Gwk::Event::Caller onHoverLeave;

            // Childrens List

            Base::List Children;

        protected:

            //! The logical parent.
            //! It's usually what you expect, the control you've parented it to.
            Base* m_parent;

            //! If the InnerPanel exists our children will automatically
            //! become children of that instead of us - allowing us to move
            //! them all around by moving that panel (useful for scrolling etc).
            Base* m_innerPanel;
            
            virtual Base* Inner()   { return m_innerPanel; }

            //! This is the panel's actual parent - most likely the logical
            //! parent's InnerPanel (if it has one). You should rarely need this.
            Base* m_actualParent;

            Base* m_toolTip;

            Skin::Base* m_skin;

            Gwk::Rect m_bounds;
            Gwk::Rect m_renderBounds;

            Padding m_padding;
            Margin m_margin;

            Gwk::String m_name;

            bool m_bRestrictToParent;
            bool m_bDisabled;
            bool m_bHidden;
            bool m_bMouseInputEnabled;
            bool m_bKeyboardInputEnabled;
            bool m_bDrawBackground;

            Position m_dock;

            unsigned char m_cursor;

            bool m_tabable;

        public:

            bool NeedsLayout()
            {
                return m_bNeedsLayout;
            }

            void Invalidate();
            void InvalidateParent()
            {
                if (m_parent)
                    m_parent->Invalidate();
            }

            void InvalidateChildren(bool bRecursive = false);
            void SetPosition(Position pos, int xpadding = 0, int ypadding = 0);

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
                                                void* userData = nullptr);
            virtual bool DragAndDrop_Draggable();
            virtual bool DragAndDrop_ShouldStartDrag()  { return true; }

            virtual void DragAndDrop_StartDragging(Gwk::DragAndDrop::Package* package, int x, int y);
            virtual Gwk::DragAndDrop::Package* DragAndDrop_GetPackage(int x, int y);
            virtual void DragAndDrop_EndDragging(bool /*bSuccess*/, int /*x*/, int /*y*/) {}

        protected:

            DragAndDrop::Package*   m_dragAndDrop_Package;

        public:

            // Receiver
            virtual void DragAndDrop_HoverEnter(Gwk::DragAndDrop::Package* /*package*/,
                                                int /*x*/, int /*y*/)
            {
            }

            virtual void DragAndDrop_HoverLeave(Gwk::DragAndDrop::Package* /*package*/)
            {
            }

            virtual void DragAndDrop_Hover(Gwk::DragAndDrop::Package* /*package*/,
                                           int /*x*/, int /*y*/)
            {
            }

            virtual bool DragAndDrop_HandleDrop(Gwk::DragAndDrop::Package* package, int x, int y);
            virtual bool DragAndDrop_CanAcceptPackage(Gwk::DragAndDrop::Package* /*package*/)
            {
                return false;
            }

            //
            // Useful anim shortcuts
            //

        public:

#if GWK_ANIMATE

            virtual void Anim_widthIn(float fLength, float fDelay = 0.0f, float fEase = 1.0f);
            virtual void Anim_heightIn(float fLength, float fDelay = 0.0f, float fEase = 1.0f);
            virtual void Anim_widthOut(float fLength, bool bHide = true, float fDelay = 0.0f,
                                       float fEase = 1.0f);
            virtual void Anim_heightOut(float fLength, bool bHide = true, float fDelay = 0.0f,
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
                return nullptr;
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

            virtual void SetAction(Event::Handler* object,
                                   Handler::FunctionWithInformation function,
                                   const Gwk::Event::Packet& packet)
            {
            }

            virtual int GetNamedChildren(Gwk::ControlList& list, const Gwk::String& strName,
                                         bool bDeep = true);
            virtual Gwk::ControlList GetNamedChildren(const Gwk::String& strName,
                                                       bool bDeep = true);

        public:

            UserDataStorage UserData;   // TODO - optimise memory usage.

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
            return nullptr;

        Gwk::Controls::Base* result = p->DynamicCast(T::GetIdentifier());

        return result != nullptr ? static_cast<T*>(result) : nullptr;
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
    THISNAME(Gwk::Controls::Base* parent, const Gwk::String& name = "")

#define GWK_CONTROL_INLINE(THISNAME, BASENAME) \
    GWK_CONTROL(THISNAME, BASENAME) : ParentClass(parent, name)

#define GWK_CONTROL_CONSTRUCTOR(THISNAME) \
    THISNAME::THISNAME(Gwk::Controls::Base* parent, const Gwk::String& name) \
        : ParentClass(parent, name)

} // namespace Gwk

#endif // ifndef GWK_CONTROLS_BASE_H
