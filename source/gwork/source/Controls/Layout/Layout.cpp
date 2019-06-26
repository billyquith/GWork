#include <Gwork/Controls/Layout/Layout.h>

namespace Gwk
{
namespace Controls
{


LayoutItem::LayoutItem():
    m_control(nullptr),
    m_parent(nullptr),
    m_layoutParent(nullptr),
    m_sizeFlags(SizeFlag::Fixed, SizeFlag::Fixed),
    m_margin({0, 0, 0, 0}),
    m_padding({0, 0, 0, 0}),
    m_spacing(2)
{

}

LayoutItem::LayoutItem(LayoutType type):
    m_type(type),
    m_parent(nullptr),
    m_layoutParent(nullptr),
    m_control(nullptr),
    m_sizeFlags(SizeFlag::Fixed, SizeFlag::Fixed),
    m_margin({0, 0, 0, 0}),
    m_padding({0, 0, 0, 0}),
    m_spacing(2)
{

}

LayoutItem::~LayoutItem()
{
    for(LayoutItem *child:m_children)
        delete child;

    m_children.clear();
}

void LayoutItem::SetParent(Base *parent)
{
    m_parent=parent;
}

void LayoutItem::SetLayoutParent(LayoutItem *layoutParent)
{
    m_layoutParent=layoutParent;
}

void LayoutItem::AddControl(Base *control)
{
    LayoutItem *layoutItem=new LayoutItem();
    LayoutItem *parentLayout=m_parent->GetLayout();
    LayoutItem *controlLayout=nullptr;

    if(parentLayout)
        controlLayout=parentLayout->FindControl(control);

    //if it already belongs to a layout need to remove
    if(controlLayout)
    {
        LayoutItem *controlParentLayout=controlLayout->GetLayoutParent();

        if(controlParentLayout)
            controlParentLayout->RemoveControl(control);
    }
    
    layoutItem->SetParent(m_parent);
    layoutItem->SetLayoutParent(this);
    layoutItem->SetControl(control);
    m_children.push_back(layoutItem);
}

void LayoutItem::RemoveControl(Base *control)
{
    for(auto iter=m_children.begin(); iter!=m_children.end(); ++iter)
    {
        if((*iter)->GetControl() == control)
        {
            LayoutItem *item=*iter;

            m_children.erase(iter);
            delete item;
            break;
        }
    }
}

LayoutItem *LayoutItem::FindControl(Base *control)
{
    for(auto iter=m_children.begin(); iter!=m_children.end(); ++iter)
    {
        if((*iter)->GetControl()==control)
            return (*iter);
        else
        {
            LayoutItem *item=(*iter)->FindControl(control);

            if(item)
                return item;
        }
    }

    return nullptr;
}

void LayoutItem::AddLayout(LayoutItem *item)
{
    item->SetParent(m_parent);
    item->SetLayoutParent(this);
    m_children.push_back(item);
}

void LayoutItem::RemoveLayout(LayoutItem *item)
{
    for(auto iter=m_children.begin(); iter!=m_children.end(); ++iter)
    {
        if(*iter == item)
        {
            LayoutItem *localItem=*iter;

            m_children.erase(iter);
            delete localItem;
            break;
        }
    }
}

Base *LayoutItem::GetControl()
{
    return m_control;
}

void LayoutItem::SetControl(Base *control)
{
    m_control=control;
}

void LayoutItem::UpdateControl()
{
    if(!m_control)
        return;

//    Rect bounds=m_control->GetNeeded();
//    Margin margin=m_control->GetMargin();
//
//    bounds.x=m_bounds.x+margin.left;
//    bounds.y=m_bounds.y+margin.top;
//    m_control->SetBounds(bounds);
}

void LayoutItem::CalculateSize(Skin::Base *skin, Dim dim)
{
    if(m_control)
    {
        m_control->CalculateSize(skin, dim);

        const Size &preferred=m_control->GetPreferredSize();
        const Rect &bounds=m_control->GetBounds();
        const Margin &margin=m_control->GetMargin();
        const SizeFlags &sizeFlags=m_control->GetSizeFlags();

        if(dim==Dim::X)
        {
            m_minimumSize.width=m_control->GetMinimumSize().width;
            m_maximumSize.width=m_control->GetMaximumSize().width;
            m_preferredSize.width=preferred.width+margin.left+margin.right;
        }
        else
        {
            m_minimumSize.height=m_control->GetMinimumSize().height;
            m_maximumSize.height=m_control->GetMaximumSize().height;
            m_preferredSize.height=preferred.height+margin.top+margin.bottom;
        }

        m_sizeFlags=m_control->GetSizeFlags();
        return;
    }

    for(LayoutItem *child:m_children)
        child->CalculateSize(skin, dim);

    switch(m_type)
    {
        case LayoutType::Horizontal:
            if(dim == Dim::X)
                CalculateHorizontal();
            else
                CalculateMaxHeight();
            break;
        case LayoutType::Vertical:
            if(dim == Dim::X)
                CalculateMaxWidth();
            else
                CalculateVertical();
            break;
    }

}

void LayoutItem::CalculateHorizontal()
{
//    m_bounds.w=0;
    m_preferredSize.width=0;
    m_minimumSize.width=0;
    m_maximumSize.width=std::numeric_limits<int>::max();

    bool addSpacing=false;

    for(LayoutItem *child:m_children)
    {
        const Gwk::Rect &bounds=child->GetBounds();
        SizeFlags sizeFlags=child->GetSizeFlags();

        m_preferredSize.width+=child->GetPreferredSize().width;
        m_minimumSize.width=AddWithOverflow(m_maximumSize.width, child->GetMinimumSize().width);
        m_maximumSize.width=AddWithOverflow(m_maximumSize.width, child->GetMaximumSize().width);
        m_sizeFlags.horizontal=Merge(m_sizeFlags.horizontal, sizeFlags.horizontal);

        if(addSpacing)
            m_preferredSize.width+=m_spacing;
//            m_bounds.w+=m_spacing;
        else
            addSpacing=true;

//        m_bounds.w+=bounds.w;
    }

    m_preferredSize.width+=m_padding.left+m_padding.right;
//    m_bounds.w+=m_padding.left+m_padding.right;
}

void LayoutItem::CalculateMaxHeight()
{
//    m_bounds.h=0;
    m_preferredSize.height=0;
    m_minimumSize.height=0;
    m_maximumSize.height=std::numeric_limits<int>::max();

    for(LayoutItem *child:m_children)
    {
        const Gwk::Rect &bounds=child->GetBounds();
        SizeFlags sizeFlags=child->GetSizeFlags();

        m_preferredSize.height=std::max(m_preferredSize.height, child->GetPreferredSize().height);
        m_minimumSize.height=std::max(m_minimumSize.height, child->GetMinimumSize().height);
        m_maximumSize.height=std::min(m_maximumSize.height, child->GetMaximumSize().height);
        m_sizeFlags.vertical=Merge(m_sizeFlags.vertical, sizeFlags.vertical);
        
//        if(bounds.h>m_bounds.h)
//            m_bounds.h=bounds.h;
    }

    m_preferredSize.height+=m_padding.top+m_padding.bottom;
//    m_bounds.h+=m_padding.top+m_padding.bottom;
}

void LayoutItem::CalculateMaxWidth()
{
//    m_bounds.w=0;
    m_minimumSize.width=0;
    m_maximumSize.width=std::numeric_limits<int>::max();

    for(LayoutItem *child:m_children)
    {
        const Gwk::Rect &bounds=child->GetBounds();
        SizeFlags sizeFlags=child->GetSizeFlags();

        m_preferredSize.width=std::max(m_preferredSize.width, child->GetPreferredSize().width);
        m_minimumSize.width=std::max(m_minimumSize.width, child->GetMinimumSize().width);
        m_maximumSize.width=std::min(m_maximumSize.width, child->GetMaximumSize().width);

        m_sizeFlags.horizontal=Merge(m_sizeFlags.horizontal, sizeFlags.horizontal);

//        if(bounds.w>m_bounds.w)
//            m_bounds.w=bounds.w;
    }
    m_preferredSize.width+=m_padding.left+m_padding.right;
//    m_bounds.w+=m_padding.left+m_padding.right;

}

void LayoutItem::CalculateVertical()
{
//    m_bounds.h=0;
    m_preferredSize.height=0;
    m_minimumSize.height=0;
    m_maximumSize.height=std::numeric_limits<int>::max();

    bool addSpacing=false;
    for(LayoutItem *child:m_children)
    {
        const Gwk::Rect &bounds=child->GetBounds();
        SizeFlags sizeFlags=child->GetSizeFlags();

        m_preferredSize.height+=child->GetPreferredSize().height;
        m_minimumSize.height=AddWithOverflow(m_minimumSize.height, child->GetMinimumSize().height);
        m_maximumSize.height=AddWithOverflow(m_maximumSize.height, child->GetMaximumSize().height);
        m_sizeFlags.vertical=Merge(m_sizeFlags.vertical, sizeFlags.vertical);

        if(addSpacing)
            m_preferredSize.height+=m_spacing;
//            m_bounds.h+=m_spacing;
        else
            addSpacing=true;

//        m_bounds.h+=bounds.h;
    }

    m_preferredSize.height+=m_padding.top+m_padding.bottom;
//    m_bounds.h+=m_padding.top+m_padding.bottom;
}

void LayoutItem::UpdateControlSize(Dim dim)
{
    SizeFlags sizeFlags=m_control->GetSizeFlags();
    Margin margin=m_control->GetMargin();
    Rect bounds=m_control->GetBounds();
    
    if(dim == Dim::X)
    {
        bounds.x=m_bounds.x+margin.left;
        if(bounds.w>m_bounds.w)
        {
            if((sizeFlags.horizontal == SizeFlag::Elastic)||
                (sizeFlags.horizontal == SizeFlag::Shrink)||
                (sizeFlags.horizontal==SizeFlag::Bloat))
                bounds.w=m_bounds.w;
        }
        else if(bounds.w<m_bounds.w)
        {
            if((sizeFlags.horizontal == SizeFlag::Elastic)||
                (sizeFlags.horizontal == SizeFlag::Expand)||
                (sizeFlags.horizontal == SizeFlag::Bloat))
                bounds.w=m_bounds.w;
        }
    }
    else
    {
        bounds.y=m_bounds.y+margin.top;
        if(bounds.h>m_bounds.h)
        {
            if((sizeFlags.vertical == SizeFlag::Elastic)||
                (sizeFlags.vertical == SizeFlag::Shrink) ||
                (sizeFlags.vertical==SizeFlag::Bloat))
                bounds.h=m_bounds.h;
        }
        else if(bounds.h<m_bounds.h)
        {
            if((sizeFlags.vertical == SizeFlag::Elastic)||
                (sizeFlags.vertical == SizeFlag::Expand)||
                (sizeFlags.vertical == SizeFlag::Bloat))
                bounds.h=m_bounds.h;
        }
    }

    m_control->SetBounds(bounds);
}

void LayoutItem::Arrange(Skin::Base *skin, Dim dim)
{
    if(m_control)
    {
        UpdateControlSize(dim);
        m_control->Arrange(skin, dim);
        return;
    }

    switch(m_type)
    {
    case LayoutType::Horizontal:
        if(dim == Dim::X)
            ArrangeHorizontal();
        else
            ArrangeVerticalPos();
        break;
    case LayoutType::Vertical:
        if(dim == Dim::X)
            ArrangeHorizontalPos();
        else
            ArrangeVertical();
        break;
    }

    for(LayoutItem *child:m_children)
        child->Arrange(skin, dim);
}

void LayoutItem::ArrangeHorizontal()
{
    int x=m_bounds.x+m_padding.left;

    int shrink=0;
    int expand=0;
    int bloat=0;

    for(LayoutItem *child:m_children)
    {
        const SizeFlags &sizeFlags=child->GetSizeFlags();

        if(sizeFlags.horizontal==SizeFlag::Shrink)
            shrink++;
        else if(sizeFlags.horizontal==SizeFlag::Expand)
            expand++;
        else if(sizeFlags.horizontal==SizeFlag::Elastic)
        {
            shrink++;
            expand++;
        }
        else if(sizeFlags.horizontal==SizeFlag::Bloat)
            bloat++;
    }

    int extraSpace=m_bounds.w-m_preferredSize.width-m_padding.left-m_padding.right;
    int bloatSpace=0;
    int expandSpace=0;
    int shrinkSpace=0;

    if(extraSpace > 0)
    {
        if(bloat > 0)
            bloatSpace=extraSpace/bloat;
        else if(expand > 0)
            expandSpace=extraSpace/expand;
    }
    else if(extraSpace < 0)
    {
        if(shrink > 0)
            shrinkSpace=extraSpace/shrink;
    }

    for(LayoutItem *child:m_children)
    {
        Rect bounds=child->GetBounds();
        Size preferred=child->GetPreferredSize();
        const SizeFlags &sizeFlags=child->GetSizeFlags();

        bounds.x=x;

        if(sizeFlags.horizontal==SizeFlag::Bloat)
            bounds.w=preferred.width+bloatSpace;
        else if((sizeFlags.horizontal==SizeFlag::Elastic)||(sizeFlags.horizontal==SizeFlag::Expand))
            bounds.w=preferred.width+expandSpace;
        else if((sizeFlags.horizontal==SizeFlag::Elastic)||(sizeFlags.horizontal==SizeFlag::Shrink))
            bounds.w=preferred.width+shrinkSpace;

        child->SetBounds(bounds);
        x+=bounds.w+m_spacing;
    }
}

void LayoutItem::ArrangeVerticalPos()
{
    int y=m_bounds.y+m_padding.top;

    for(LayoutItem *child:m_children)
    {
        Rect bounds=child->GetBounds();
        const Size preferred=child->GetPreferredSize();

        bounds.y=y;
        bounds.h=preferred.height+m_padding.top+m_padding.bottom;

        SizeFlags sizeFlags=child->GetSizeFlags();

        if(preferred.height < m_bounds.h)
        {
            if((sizeFlags.vertical==SizeFlag::Elastic)||(sizeFlags.vertical==SizeFlag::Expand)||(sizeFlags.vertical==SizeFlag::Bloat))
                bounds.h=m_bounds.h-m_padding.top-m_padding.bottom;

        }
        else if(preferred.height > m_bounds.h)
        {
            if((sizeFlags.vertical==SizeFlag::Elastic)||(sizeFlags.vertical==SizeFlag::Shrink))
                bounds.h=m_bounds.h-m_padding.top-m_padding.bottom;
        }
        child->SetBounds(bounds);
    }
}

void LayoutItem::ArrangeHorizontalPos()
{
    int x=m_bounds.x+m_padding.left;

    for(LayoutItem *child:m_children)
    {
        Rect bounds=child->GetBounds();
        const Size preferred=child->GetPreferredSize();

        bounds.x=x;
        bounds.w=preferred.width+m_padding.left+m_padding.right;

//        child->SetPosition(x, bounds.y);

        SizeFlags sizeFlags=child->GetSizeFlags();

        if(preferred.width < m_bounds.w)
        {
            if((sizeFlags.horizontal == SizeFlag::Elastic) || (sizeFlags.horizontal == SizeFlag::Expand) || (sizeFlags.horizontal == SizeFlag::Bloat))
                bounds.w=m_bounds.w-m_padding.left-m_padding.right;
                
        }
        else if(preferred.width > m_bounds.w)
        {
            if((sizeFlags.horizontal == SizeFlag::Elastic) || (sizeFlags.horizontal == SizeFlag::Shrink))
                bounds.w=m_bounds.w-m_padding.left-m_padding.right;
        }
        child->SetBounds(bounds);
        //        child->UpdateControl();
    }
}

void LayoutItem::ArrangeVertical()
{
    int y=m_bounds.y+m_padding.top;

    int shrink=0;
    int expand=0;
    int bloat=0;

    for(LayoutItem *child:m_children)
    {
        const SizeFlags &sizeFlags=child->GetSizeFlags();

        if(sizeFlags.vertical == SizeFlag::Shrink)
            shrink++;
        else if(sizeFlags.vertical == SizeFlag::Expand)
            expand++;
        else if(sizeFlags.vertical == SizeFlag::Elastic)
        {
            shrink++;
            expand++;
        }
        else if(sizeFlags.vertical == SizeFlag::Bloat)
        {
            shrink++;
            bloat++;
        }
    }

    int extraSpace=m_bounds.h-m_preferredSize.height-m_padding.top-m_padding.bottom;
    int bloatSpace=0;
    int expandSpace=0;
    int shrinkSpace=0;

    if(extraSpace > 0)
    {
        if(bloat > 0)
            bloatSpace=extraSpace/bloat;
        else if(expand > 0)
            expandSpace=extraSpace/expand;
    }
    else if(extraSpace < 0)
    {
        if(bloat > 0)
            bloatSpace=extraSpace/bloat;
        else if(shrink > 0)
            shrinkSpace=extraSpace/shrink;
    }

    for(LayoutItem *child:m_children)
    {
        Rect bounds=child->GetBounds();
        Size preferred=child->GetPreferredSize();
        const SizeFlags &sizeFlags=child->GetSizeFlags();

        bounds.y=y;

        if(sizeFlags.vertical==SizeFlag::Bloat)
            bounds.h=preferred.height+bloatSpace;
        else if(sizeFlags.vertical == SizeFlag::Elastic)
            bounds.h=preferred.height+expandSpace+shrinkSpace;
        else if(sizeFlags.vertical == SizeFlag::Expand)
            bounds.h=preferred.height+expandSpace;
        else if(sizeFlags.vertical == SizeFlag::Shrink)
            bounds.h=preferred.height+shrinkSpace;

        child->SetBounds(bounds);
        y+=bounds.h+m_spacing;
    }
}

} //namespace Controls
} //namespace Gwk
