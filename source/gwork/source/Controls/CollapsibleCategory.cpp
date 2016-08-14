/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */

#include <Gwork/Controls/CollapsibleCategory.h>
#include <Gwork/Controls/CollapsibleList.h>


namespace Gwk {
    
using namespace Gwk::Controls;

    
class CategoryButton : public Button
{
    GWK_CONTROL_INLINE(CategoryButton, Button)
    {
        SetAlignment(Position::Left | Position::CenterV);
        m_bAlt = false;
    }
    
    void Render(Skin::Base* skin) override
    {
        if (m_bAlt)
        {
            if (IsDepressed() || GetToggleState())
                skin->GetRender()->SetDrawColor(skin->Colors.Category.LineAlt.Button_Selected);
            else if (IsHovered())
                skin->GetRender()->SetDrawColor(skin->Colors.Category.LineAlt.Button_Hover);
            else
                skin->GetRender()->SetDrawColor(skin->Colors.Category.LineAlt.Button);
        }
        else
        {
            if (IsDepressed() || GetToggleState())
                skin->GetRender()->SetDrawColor(skin->Colors.Category.Line.Button_Selected);
            else if (IsHovered())
                skin->GetRender()->SetDrawColor(skin->Colors.Category.Line.Button_Hover);
            else
                skin->GetRender()->SetDrawColor(skin->Colors.Category.Line.Button);
        }
        
        skin->GetRender()->DrawFilledRect(this->GetRenderBounds());
    }
    
    void UpdateColours() override
    {
        if (m_bAlt)
        {
            if (IsDepressed() || GetToggleState())
                return SetTextColor(GetSkin()->Colors.Category.LineAlt.Text_Selected);
            
            if (IsHovered())
                return SetTextColor(GetSkin()->Colors.Category.LineAlt.Text_Hover);
            
            return SetTextColor(GetSkin()->Colors.Category.LineAlt.Text);
        }
        
        if (IsDepressed() || GetToggleState())
            return SetTextColor(GetSkin()->Colors.Category.Line.Text_Selected);
        
        if (IsHovered())
            return SetTextColor(GetSkin()->Colors.Category.Line.Text_Hover);
        
        return SetTextColor(GetSkin()->Colors.Category.Line.Text);
    }
    
    bool m_bAlt;
};


class CategoryHeaderButton : public Button
{
    GWK_CONTROL_INLINE(CategoryHeaderButton, Button)
    {
        SetShouldDrawBackground(false);
        SetIsToggle(true);
        SetAlignment(Position::Center);
    }
    
    void UpdateColours() override
    {
        if (IsDepressed() || GetToggleState())
            return SetTextColor(GetSkin()->Colors.Category.Header_Closed);
        
        return SetTextColor(GetSkin()->Colors.Category.Header);
    }
    
};


GWK_CONTROL_CONSTRUCTOR(CollapsibleCategory)
{
    m_list = nullptr;
    m_button = new CategoryHeaderButton(this);
    m_button->SetText("Category Title");
    m_button->Dock(Position::Top);
    m_button->SetHeight(20);
    SetPadding(Padding(1, 0, 1, 5));
    SetSize(512, 512);
}

Button* CollapsibleCategory::Add(const String& name)
{
    CategoryButton* button = new CategoryButton(this);
    button->SetText(name);
    button->Dock(Position::Top);
    button->SizeToContents();
    button->SetSize(button->Width()+4, button->Height()+4);
    button->SetPadding(Padding(5, 2, 2, 2));
    button->onPress.Add(this, &ThisClass::OnSelection);
    return button;
}

void CollapsibleCategory::OnSelection(Event::Info info)
{
    CategoryButton* child = gwk_cast<CategoryButton>(info.ControlCaller);
    
    if (!child)
        return;
    
    if (m_list)
        m_list->UnselectAll();
    else
        UnselectAll();
    
    child->SetToggleState(true);
    onSelection.Call(this);
}

void CollapsibleCategory::Render(Skin::Base* skin)
{
    skin->DrawCategoryInner(this, m_button->GetToggleState());
}

void CollapsibleCategory::SetText(const String& text)
{
    m_button->SetText(text);
}

void CollapsibleCategory::UnselectAll()
{
    Base::List& children = GetChildren();

    for (auto&& control : children)
    {
        CategoryButton* child = gwk_cast<CategoryButton>(control);

        if (!child)
            continue;

        child->SetToggleState(false);
    }
}

void CollapsibleCategory::PostLayout(Skin::Base* /*skin*/)
{
    if (m_button->GetToggleState())
        SetHeight(m_button->Height());
    else
        SizeToChildren(false, true);

    Base::List& children = GetChildren();
    bool b = true;

    for (auto&& control : children)
    {
        CategoryButton* child = gwk_cast<CategoryButton>(control);

        if (!child)
            continue;

        child->m_bAlt = b;
        child->UpdateColours();
        b = !b;
    }
}

Button* CollapsibleCategory::GetSelected()
{
    Base::List& children = GetChildren();

    for (auto&& control : children)
    {
        CategoryButton* child = gwk_cast<CategoryButton>(control);

        if (!child)
            continue;

        if (child->GetToggleState())
            return child;
    }

    return nullptr;
}


void CollapsibleCategory::SetExpanded(bool expanded)
{
    m_button->SetToggleState(!expanded);
}

bool CollapsibleCategory::IsExpanded() const
{
    return !m_button->GetToggleState();
}
    
} // namespace Gwk

