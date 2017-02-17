/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */


#include <Gwork/Controls/TreeNode.h>
#include <Gwork/Controls/TreeControl.h>
#include <Gwork/Utility.h>

using namespace Gwk;
using namespace Gwk::Controls;

class OpenToggleButton : public Button
{
    GWK_CONTROL_INLINE(OpenToggleButton, Button)
    {
        SetIsToggle(true);
        SetTabable(false);
    }

    void RenderFocus(Skin::Base* /*skin*/) override
    {
    }

    void Render(Skin::Base* skin) override
    {
        skin->DrawTreeButton(this, GetToggleState());
    }

};


class TreeNodeText : public Button
{
    GWK_CONTROL_INLINE(TreeNodeText, Button)
    {
        SetAlignment(Position::Left|Position::CenterV);
        SetShouldDrawBackground(false);
        SetHeight(16);
    }

    void UpdateColors() override
    {
        if (IsDisabled())
            return SetTextColor(GetSkin()->Colors.Button.Disabled);

        if (IsDepressed() || GetToggleState())
            return SetTextColor(GetSkin()->Colors.Tree.Selected);

        if (IsHovered())
            return SetTextColor(GetSkin()->Colors.Tree.Hover);

        SetTextColor(GetSkin()->Colors.Tree.Normal);
    }

};


const int TreeIndentation = 14;

GWK_CONTROL_CONSTRUCTOR(TreeNode)
{
    m_treeControl = nullptr;
    m_toggleButton = new OpenToggleButton(this);
    m_toggleButton->SetBounds(0, 0, 15, 15);
    m_toggleButton->onToggle.Add(this, &TreeNode::OnToggleButtonPress);
    m_title = new TreeNodeText(this);
    m_title->Dock(Position::Top);
    m_title->SetMargin(Margin(16, 0, 0, 0));
    m_title->onDoubleClick.Add(this, &TreeNode::OnDoubleClickName);
    m_title->onDown.Add(this, &TreeNode::OnClickName);
    m_title->onRightPress.Add(this, &TreeNode::OnRightPress);
    m_innerPanel = new Base(this);
    m_innerPanel->Dock(Position::Top);
    m_innerPanel->SetHeight(100);
    m_innerPanel->SetMargin(Margin(TreeIndentation, 1, 0, 0));
    m_innerPanel->Hide();
    m_bRoot = false;
    m_bSelected = false;
    m_bSelectable = true;
}

void TreeNode::Render(Skin::Base* skin)
{
    int iBottom = 0;

    if (m_innerPanel->Children.size() > 0)
        iBottom = m_innerPanel->Children.back()->Y() + m_innerPanel->Y();

    skin->DrawTreeNode(this,
                       m_innerPanel->Visible(),
                       IsSelected(),
                       m_title->Height(),
                       m_title->TextRight(),
                       m_toggleButton->Y() + m_toggleButton->Height()/2,
                       iBottom,
                       GetParent() == m_treeControl);
}

TreeNode* TreeNode::AddNode(const String& strLabel)
{
    TreeNode* node = new TreeNode(this);
    node->SetText(strLabel);
    node->Dock(Position::Top);
    node->SetRoot(gwk_cast<TreeControl>(this) != nullptr);
    node->SetTreeControl(m_treeControl);

    if (m_treeControl)
        m_treeControl->OnNodeAdded(node);

    return node;
}

void TreeNode::Layout(Skin::Base* skin)
{
    if (m_toggleButton)
    {
        if (m_title)
            m_toggleButton->SetPos(0, (m_title->Height()-m_toggleButton->Height()) / 2);

        if (m_innerPanel->NumChildren() == 0)
        {
            m_toggleButton->Hide();
            m_toggleButton->SetToggleState(false);
            m_innerPanel->Hide();
        }
        else
        {
            m_toggleButton->Show();
            m_innerPanel->SizeToChildren(false, true);
        }
    }

    ParentClass::Layout(skin);
}

void TreeNode::PostLayout(Skin::Base* /*skin*/)
{
    if (SizeToChildren(false, true))
        InvalidateParent();
}

void TreeNode::SetText(const String& text)
{
    m_title->SetText(text);
}

const String& TreeNode::GetText()
{
    return m_title->GetText();
}

void TreeNode::SetImage(const String& text)
{
    m_title->SetImage(text);
}

void TreeNode::Open()
{
    m_innerPanel->Show();

    if (m_toggleButton)
        m_toggleButton->SetToggleState(true);

    Invalidate();
}

void TreeNode::Close()
{
    m_innerPanel->Hide();

    if (m_toggleButton)
        m_toggleButton->SetToggleState(false);

    Invalidate();
}

void TreeNode::ExpandAll()
{
    Open();
    Base::List& children = GetChildNodes();

    for (auto&& childnode : children)
    {
        TreeNode* child = gwk_cast<TreeNode>(childnode);

        if (!child)
            continue;

        child->ExpandAll();
    }
}

Button* TreeNode::GetButton()
{
    return m_title;
}

void TreeNode::OnToggleButtonPress(Event::Info)
{
    if (m_toggleButton->GetToggleState())
        Open();
    else
        Close();
}

void TreeNode::OnDoubleClickName(Event::Info)
{
    if (!m_toggleButton->Visible())
        return;

    m_toggleButton->Toggle();
}

void TreeNode::OnClickName(Event::Info)
{
    onNamePress.Call(this);
    SetSelected(!IsSelected());
}

void TreeNode::OnRightPress(Event::Info)
{
    onRightPress.Call(this);
}

void TreeNode::SetSelected(bool b, bool FireEvents)
{
    if (!m_bSelectable)
        return;

    if (m_bSelected == b)
        return;

    m_bSelected = b;

    if (m_title)
        m_title->SetToggleState(m_bSelected);

    if (FireEvents)
    {
        onSelectChange.Call(this);

        if (m_bSelected)
            onSelect.Call(this);
        else
            onUnselect.Call(this);
    }

    Redraw();
}

void TreeNode::DeselectAll()
{
    m_bSelected = false;

    if (m_title)
        m_title->SetToggleState(m_bSelected);

    Base::List& children = GetChildNodes();

    for (auto&& childnode : children)
    {
        TreeNode* child = gwk_cast<TreeNode>(childnode);

        if (!child)
            continue;

        child->DeselectAll();
    }
}

Controls::Base::List& TreeNode::GetChildNodes()
{
    return m_innerPanel->GetChildren();
}
