/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_CONTROLS_LAYOUT_TABLE_H
#define GWK_CONTROLS_LAYOUT_TABLE_H

#include <Gwork/Controls/Button.h>
#include <Gwork/Utility.h>

namespace Gwk
{
    namespace Controls
    {
        namespace Layout
        {
            class Table;

            class GWK_EXPORT TableRow : public Base
            {
                static const int MaxColumns = 16;

                GWK_CONTROL_INLINE(TableRow, Base)
                {
                    SetEven(false);

                    for (int i = 0; i < MaxColumns; i++)
                    {
                        m_columns[i] = nullptr;
                    }

                    m_columnCount = 0;
                }

                void SetColumnCount(int iCount)
                {
                    if (iCount == m_columnCount)
                        return;

                    if (iCount >= MaxColumns)
                        m_columnCount = MaxColumns;

                    for (int i = 0; i < MaxColumns; i++)
                    {
                        if (i < iCount)
                        {
                            if (!m_columns[i])
                            {
                                m_columns[i] = new Label(this);
                                m_columns[i]->Dock(Position::Left);
                                m_columns[i]->SetPadding(Padding(3, 3, 3, 3));
                            }
                        }
                        else if (m_columns[i])
                        {
                            m_columns[i]->DelayedDelete();
                            m_columns[i] = nullptr;
                        }

                        m_columnCount = iCount;
                    }
                }

                void SetColumnWidth(int i, int iWidth)
                {
                    if (!m_columns[i])
                        return;

                    if (m_columns[i]->Width() == iWidth)
                        return;

                    m_columns[i]->SetWidth(iWidth);
                }

                void SetCellText(int i, const String& strString)
                {
                    if (!m_columns[i])
                        return;

                    m_columns[i]->SetText(strString);
                }

                void SetCellContents(int i, Base* control, bool bEnableMouseInput = false)
                {
                    if (!m_columns[i])
                        return;

                    control->SetParent(m_columns[i]);
                    m_columns[i]->SetMouseInputEnabled(bEnableMouseInput);
                }

                Label* GetCellContents(int i)
                {
                    return m_columns[i];
                }

                void SizeToContents()
                {
                    int iHeight = 0;

                    for (int i = 0; i < m_columnCount; i++)
                    {
                        if (!m_columns[i])
                            continue;

                        // Note, more than 1 child here, because the
                        // label has a child built in ( The Text )
                        if (m_columns[i]->NumChildren() > 1)
                            m_columns[i]->SizeToChildren();
                        else
                            m_columns[i]->SizeToContents();

                        iHeight = std::max(iHeight, m_columns[i]->Height());
                    }

                    SetHeight(iHeight);
                }

                void SetTextColor(const Gwk::Color& color)
                {
                    for (int i = 0; i < m_columnCount; i++)
                    {
                        if (!m_columns[i])
                            continue;

                        m_columns[i]->SetTextColor(color);
                    }
                }

                // You might hate this. Actually I know you will
                virtual const String& GetText(int i)
                {
                    return m_columns[i]->GetText();
                }

                virtual void SetSelected(bool /*b*/)
                {
                }

                //
                // This is sometimes called by derivatives.
                //
                Gwk::Event::Listener onRowSelected;

                virtual bool GetEven()
                {
                    return m_bEvenRow;
                }

                virtual void SetEven(bool b)
                {
                    m_bEvenRow = b;
                }

            private:

                bool m_bEvenRow;
                int m_columnCount;
                Label*  m_columns[MaxColumns];

                friend class Table;


            };


            class GWK_EXPORT Table : public Base
            {
            public:

                GWK_CONTROL_INLINE(Table, Base)
                {
                    m_columnCount = 1;
                    m_defaultRowHeight = 22;

                    for (int i = 0; i < TableRow::MaxColumns; i++)
                    {
                        m_columnWidth[i] = 0;
                    }

                    m_bSizeToContents = false;
                }

                void SetColumnCount(int i)
                {
                    if (m_columnCount == i)
                        return;

                    for (auto&& child : Children)
                    {
                        TableRow* row = gwk_cast<TableRow>(child);

                        if (!row)
                            continue;

                        row->SetColumnCount(i);
                    }

                    m_columnCount = i;
                }

                void SetColumnWidth(int i, int iWidth)
                {
                    if (m_columnWidth[i] == iWidth)
                        return;

                    m_columnWidth[i] = iWidth;
                    Invalidate();
                }

                TableRow* AddRow()
                {
                    TableRow* row = new TableRow(this);
                    AddRow(row);
                    return row;
                }

                void AddRow(TableRow* row)
                {
                    row->SetParent(this);
                    row->SetColumnCount(m_columnCount);
                    row->SetHeight(m_defaultRowHeight);
                    row->Dock(Position::Top);
                    Invalidate();
                }

                TableRow* GetRow(int i)
                {
                    return gwk_cast<TableRow>(GetChild(i));
                }

                unsigned int RowCount(int i)
                {
                    return NumChildren();
                }

                void Remove(TableRow* row)
                {
                    row->DelayedDelete();
                }

                void Clear()
                {
                    for (auto&& child : Children)
                    {
                        TableRow* row = gwk_cast<TableRow>(child);

                        if (!row)
                            continue;

                        Remove(row);
                    }
                }

                void Layout(Skin::Base* skin) override
                {
                    ParentClass::Layout(skin);

                    if (m_bSizeToContents)
                        DoSizeToContents();

                    int iSizeRemainder = Width();
                    int iAutoSizeColumns = 0;

                    for (int i = 0; i < TableRow::MaxColumns && i < m_columnCount; i++)
                    {
                        iSizeRemainder -= m_columnWidth[i];

                        if (m_columnWidth[i] == 0)
                            iAutoSizeColumns++;
                    }

                    if (iAutoSizeColumns > 1)
                        iSizeRemainder /= iAutoSizeColumns;

                    bool bEven = false;

                    for (auto&& child : Children)
                    {
                        TableRow* row = gwk_cast<TableRow>(child);

                        if (!row)
                            continue;

                        row->SizeToContents();
                        row->SetEven(bEven);
                        bEven = !bEven;

                        for (int i = 0; i < TableRow::MaxColumns && i < m_columnCount; i++)
                        {
                            if (m_columnWidth[i] == 0)
                                row->SetColumnWidth(i, iSizeRemainder);
                            else
                                row->SetColumnWidth(i, m_columnWidth[i]);
                        }
                    }

                    InvalidateParent();
                }

                void PostLayout(Skin::Base* /*skin*/) override
                {
                    if (m_bSizeToContents)
                    {
                        SizeToChildren();
                        m_bSizeToContents = false;
                    }
                }

                void SizeToContents()
                {
                    m_bSizeToContents = true;
                    Invalidate();
                }

                void DoSizeToContents()
                {
                    for (int i = 0; i < TableRow::MaxColumns; i++)
                    {
                        m_columnWidth[i] = 10;
                    }

                    for (auto&& child : Children)
                    {
                        TableRow* row = gwk_cast<TableRow>(child);

                        if (!row)
                            continue;

                        row->SizeToContents();

                        for (int i = 0; i < TableRow::MaxColumns; i++)
                        {
                            if (row->m_columns[i])
                            {
                                m_columnWidth[i] = std::max(m_columnWidth[i],
                                                             row->m_columns[i]->Width());
                            }
                        }

                        // iBottom += row->Height();
                    }

                    InvalidateParent();
                }

            private:

                bool m_bSizeToContents;
                int m_columnCount;
                int m_defaultRowHeight;

                int m_columnWidth[ TableRow::MaxColumns ];
            };


        }
    }
}
#endif // ifndef GWK_CONTROLS_LAYOUT_TABLE_H
