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
    m_splitterBar = new SplitterBar(this);
    m_splitterBar->SetPos(80, 0);
    m_splitterBar->SetCursor(Gwk::CursorType::SizeWE);
    m_splitterBar->onDragged.Add(this, &Properties::OnSplitterMoved);
    m_splitterBar->SetShouldDrawBackground(false);
    m_splitterBar->DoNotIncludeInSize();
}

void Properties::PostLayout(Gwk::Skin::Base* /*skin*/)
{
    if (SizeToChildren(false, true))
        InvalidateParent();

    m_splitterBar->SetSize(3, Height());
}

void Properties::OnSplitterMoved(Event::Info)
{
    InvalidateChildren();
}

int Properties::GetSplitWidth()
{
    return m_splitterBar->X();
}

PropertyRow* Properties::Add(const String& text, const String& value)
{
    return Add(text, new Property::Text(this), value);
}

PropertyRow* Properties::Add(const String& text, Property::Base* prop, const String& value)
{
    PropertyRow* row = new PropertyRow(this);
    row->Dock(Position::Top);
    row->GetLabel()->SetText(text);
    row->SetProperty(prop);
    prop->SetPropertyValue(value, true);
    m_splitterBar->BringToFront();
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

    return nullptr;
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
        SetAlignment(Position::Left|Position::CenterV);
        m_propertyRow = nullptr;
    }

    void UpdateColours() override
    {
        if (IsDisabled())
            return SetTextColor(GetSkin()->Colors.Button.Disabled);

        if (m_propertyRow && m_propertyRow->IsEditing())
            return SetTextColor(GetSkin()->Colors.Properties.Label_Selected);

        if (m_propertyRow && m_propertyRow->IsHovered())
            return SetTextColor(GetSkin()->Colors.Properties.Label_Hover);

        SetTextColor(GetSkin()->Colors.Properties.Label_Normal);
    }

    void SetPropertyRow(PropertyRow* p)
    {
        m_propertyRow = p;
    }

protected:

    PropertyRow*    m_propertyRow;
};


GWK_CONTROL_CONSTRUCTOR(PropertyRow)
{
    m_property = nullptr;
    PropertyRowLabel* label = new PropertyRowLabel(this);
    label->SetPropertyRow(this);
    label->Dock(Position::Left);
    label->SetAlignment(Position::Left|Position::CenterV);
    label->SetMargin(Margin(2, 0, 0, 0));
    m_label = label;
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
    skin->DrawPropertyRow(this, m_label->Right(), IsEditing(), IsHovered()|
                          m_property->IsHovered());
}

void PropertyRow::Layout(Gwk::Skin::Base* /*skin*/)
{
    Properties* parent = gwk_cast<Properties>(GetParent());

    if (!parent)
        return;

    m_label->SetWidth(parent->GetSplitWidth());

    if (m_property)
        SetHeight( std::max(m_label->Height(), m_property->Height()) );
}

void PropertyRow::SetProperty(Property::Base* prop)
{
    m_property = prop;
    m_property->SetParent(this);
    m_property->Dock(Position::Fill);
    m_property->onChange.Add(this, &ThisClass::OnPropertyValueChanged);
}

void PropertyRow::OnPropertyValueChanged(Event::Info info)
{
    Event::Info sinfo(info);
    sinfo.String = GetProperty()->GetPropertyValue();
    onChange.Call(this, sinfo);
}

void PropertyRow::OnEditingChanged()
{
    m_label->Redraw();
}

void PropertyRow::OnHoverChanged()
{
    m_label->Redraw();
}
