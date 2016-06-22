/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_CONTROLS_LISTBOX_H
#define GWK_CONTROLS_LISTBOX_H

#include <Gwork/Gwork.h>
#include <Gwork/Controls/Layout/Table.h>
#include <Gwork/Controls/ScrollControl.h>


namespace Gwk
{
    namespace Controls
    {
        class ScrollControl;

        class GWK_EXPORT ListBox : public ScrollControl
        {
        public:

            GWK_CONTROL(ListBox, ScrollControl);

            typedef std::list<Layout::TableRow*> Rows;

            Layout::TableRow* AddItem(const String& strLabel, const String& strName = "");

            void RemoveItem(Layout::TableRow* row);

            void Render(Skin::Base* skin) override;
            void Layout(Skin::Base* skin) override;

            void UnselectAll();

            void SetAllowMultiSelect(bool bMultiSelect)
            {
                m_bMultiSelect = bMultiSelect;
            }

            bool AllowMultiSelect() const
            {
                return m_bMultiSelect;
            }

            const ListBox::Rows& GetSelectedRows()
            {
                return m_selectedRows;
            }

            virtual Layout::TableRow* GetSelectedRow();
            virtual Gwk::String      GetSelectedRowName();

            virtual void SetSelectedRow(Gwk::Controls::Base* row, bool bClearOthers = true);
            virtual void SelectByString(const String& string, bool bClearOthers = true);

            Gwk::Event::Listener onRowSelected;

            Controls::Layout::Table* GetTable()
            {
                return m_table;
            }

            virtual void Clear() override;

            // Pass through, to embedded table
            void SetColumnCount(int iCount)
            {
                m_table->SetColumnCount(iCount);
            }

            void SetColumnWidth(int iCount, int iSize)
            {
                m_table->SetColumnWidth(iCount, iSize);
            }

        protected:

            void OnRowSelected(Event::Info);
            bool OnKeyDown(bool bDown) override;
            bool OnKeyUp(bool bDown) override;

            Controls::Layout::Table* m_table;
            ListBox::Rows m_selectedRows;

            bool m_bMultiSelect;
        };


    }
}
#endif // ifndef GWK_CONTROLS_LISTBOX_H
