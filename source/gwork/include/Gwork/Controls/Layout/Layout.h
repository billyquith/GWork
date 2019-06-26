#pragma once
#ifndef GWK_CONTROLS_LAYOUT_BASE_H
#define GWK_CONTROLS_LAYOUT_BASE_H

#include <Gwork/Controls/Base.h>
#include <Gwork/Utility.h>

namespace Gwk
{
namespace Controls
{

enum LayoutType
{
    Horizontal,
    Vertical
};

class LayoutItem
{
public:
    LayoutItem();
    LayoutItem(LayoutType type);
    ~LayoutItem();

    void SetParent(Base *parent);
    Base *GetParent() { return m_parent; }
    void SetLayoutParent(LayoutItem *layoutParent);
    LayoutItem *GetLayoutParent() { return m_layoutParent; }

    void AddControl(Base *control);
    void RemoveControl(Base *control);
    LayoutItem *FindControl(Base *control);

    void AddLayout(LayoutItem *item);
    void RemoveLayout(LayoutItem *item);

    Base *GetControl();
    void SetControl(Base *control);
    void UpdateControl();

    void CalculateSize(Skin::Base *skin, Dim dim);
    void Arrange(Skin::Base *skin, Dim dim);

    const Gwk::Rect &GetBounds() const { return m_bounds; }
    const void SetBounds(Gwk::Rect &bounds) { m_bounds=bounds; }

    void SetPosition(int x, int y) { m_bounds.x=x; m_bounds.y=y; }

    const SizeFlags &GetSizeFlags() const { return m_sizeFlags; }
    const Size &GetMinimumSize() const { return m_minimumSize; }
    const Size &GetMaximumSize() const { return m_maximumSize; }
    const Size &GetPreferredSize() const { return m_preferredSize; }

private:
    void CalculateHorizontal();
    void CalculateMaxHeight();
    void CalculateVertical();
    void CalculateMaxWidth();

    void UpdateControlSize(Dim dim);

    void ArrangeHorizontal();
    void ArrangeVerticalPos();
    void ArrangeVertical();
    void ArrangeHorizontalPos();

    Base *m_parent;
    LayoutItem *m_layoutParent;
    std::vector<LayoutItem *> m_children;
    Base *m_control;

    LayoutType m_type;
    Gwk::Rect m_bounds;
    Margin m_margin;
    Margin m_padding;
    int m_spacing;

    SizeFlags m_sizeFlags;
    Size m_minimumSize;
    Size m_maximumSize;
    Size m_preferredSize;
};

//class LayoutBase:public LayoutItem
//{
//public:
//    LayoutBase(Layout type);
//
//    int GetSpacing();
//    void SetSpacing(int value);
//
//    int GetMargin();
//    void SetMargin(int value);
//    void SetMargins(int left, int top, int right, int bottom);
//
//    Size GetMinimumSize();
//    Size GetMaximumSize();
//
//    void DoThink();
//};

class HorizontalLayout:public LayoutItem
{
public:
    HorizontalLayout():LayoutItem(LayoutType::Horizontal) {}
};

class VerticalLayout:public LayoutItem
{
public:
    VerticalLayout():LayoutItem(LayoutType::Vertical) {}
};

}//namespace Controls
}//namespace Gwk

#endif//GWK_CONTROLS_LAYOUT_BASE_H
