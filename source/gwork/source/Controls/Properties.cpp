/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */


#include <Gwork/Gwork.h>
#include <Gwork/Skin.h>
#include <Gwork/Controls/Properties.h>
#include <Gwork/Utility.h>

using namespace Gwk;
using namespace Gwk::Controls;

GWK_CONTROL_CONSTRUCTOR(Properties)
{
    m_SplitterBar = new SplitterBar(this);
    m_SplitterBar->SetPos(80, 0);
    m_SplitterBar->SetCursor(Gwk::CursorType::SizeWE);
    m_SplitterBar->onDragged.Add(this, &Properties::OnSplitterMoved);
    m_SplitterBar->SetShouldDrawBackground(false);
    m_SplitterBar->DoNotIncludeInSize();
}

void Properties::PostLayout(Gwk::Skin::Base* /*skin*/)
{
    if (SizeToChildren(false, true))
        InvalidateParent();

    m_SplitterBar->SetSize(3, Height());
}

void Properties::OnSplitterMoved(Controls::Base* /*control*/)
{
    InvalidateChildren();
}

int Properties::GetSplitWidth()
{
    return m_SplitterBar->X();
}

PropertyRow* Properties::Add(const String& text, const String& value)
{
    return Add(text, new Property::Text(this), value);
}

PropertyRow* Properties::Add(const String& text, Property::Base* pProp, const String& value)
{
    PropertyRow* row = new PropertyRow(this);
    row->Dock(Docking::Top);
    row->GetLabel()->SetText(text);
    row->SetProperty(pProp);
    pProp->SetPropertyValue(value, true);
    m_SplitterBar->BringToFront();
    return row;
}

PropertyRow* Properties::Find(const String& text)
{
    for (Base::List::iterator it = GetChildren().begin(); it != GetChildren().end(); ++it)
    {
        PropertyRow* row = gwk_cast<PropertyRow>(*it);

        if (!row)
            continue;

        if (row->GetLabel()->GetText() == text)
            return row;
    }

    return NULL;
}

void Properties::Clear()
{
    Base::List ChildListCopy = GetChildren();

    for (Base::List::iterator it = ChildListCopy.begin(); it != ChildListCopy.end(); ++it)
    {
        PropertyRow* row = gwk_cast<PropertyRow>(*it);

        if (!row)
            continue;

        row->DelayedDelete();
    }
}

class PropertyRowLabel : public Label
{
    GWK_CONTROL_INLINE(PropertyRowLabel, Label)
    {
        SetAlignment(Docking::Left|Docking::CenterV);
        m_pPropertyRow = NULL;
    }

    void UpdateColours()
    {
        if (IsDisabled())
            return SetTextColor(GetSkin()->Colors.Button.Disabled);

        if (m_pPropertyRow && m_pPropertyRow->IsEditing())
            return SetTextColor(GetSkin()->Colors.Properties.Label_Selected);

        if (m_pPropertyRow && m_pPropertyRow->IsHovered())
            return SetTextColor(GetSkin()->Colors.Properties.Label_Hover);

        SetTextColor(GetSkin()->Colors.Properties.Label_Normal);
    }

    void SetPropertyRow(PropertyRow* p)
    {
        m_pPropertyRow = p;
    }

protected:

    PropertyRow*    m_pPropertyRow;
};


GWK_CONTROL_CONSTRUCTOR(PropertyRow)
{
    m_Property = NULL;
    PropertyRowLabel* pLabel = new PropertyRowLabel(this);
    pLabel->SetPropertyRow(this);
    pLabel->Dock(Docking::Left);
    pLabel->SetAlignment(Docking::Left|Docking::CenterV);
    pLabel->SetMargin(Margin(2, 0, 0, 0));
    m_Label = pLabel;
}

void PropertyRow::Render(Gwk::Skin::Base* skin)
{
    /* SORRY */
    if (IsEditing() != m_bLastEditing)
    {
        OnEditingChanged();
        m_bLastEditing = IsEditing();
    }

    if (IsHovered() != m_bLastHover)
    {
        OnHoverChanged();
        m_bLastHover = IsHovered();
    }

    /* SORRY */
    skin->DrawPropertyRow(this, m_Label->Right(), IsEditing(), IsHovered()|
                          m_Property->IsHovered());
}

void PropertyRow::Layout(Gwk::Skin::Base* /*skin*/)
{
    Properties* pParent = gwk_cast<Properties>(GetParent());

    if (!pParent)
        return;

    m_Label->SetWidth(pParent->GetSplitWidth());

    if (m_Property)
        SetHeight( Max(m_Label->Height(), m_Property->Height()) );
}

void PropertyRow::SetProperty(Property::Base* prop)
{
    m_Property = prop;
    m_Property->SetParent(this);
    m_Property->Dock(Docking::Fill);
    m_Property->onChange.Add(this, &ThisClass::OnPropertyValueChanged);
}

void PropertyRow::OnPropertyValueChanged(Gwk::Controls::Base* /*control*/)
{
    Event::Information info;
    info.String = GetProperty()->GetPropertyValue();
    onChange.Call(this, info);
}

void PropertyRow::OnEditingChanged()
{
    m_Label->Redraw();
}

void PropertyRow::OnHoverChanged()
{
    m_Label->Redraw();
}
