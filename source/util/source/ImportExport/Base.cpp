#include <Gwork/Util/ImportExport.h>

namespace Gwk {
namespace ImportExport {

void InitializeFormats()
{
    DECLARE_GWK_IMPORTEXPORT(DesignerFormat);
}

ImportExport::Base::List& GetList()
{
    static ImportExport::Base::List list;
    static bool Initialized = false;

    if (!Initialized)
    {
        Initialized = true;
        InitializeFormats();
    }

    return list;
}

ImportExport::Base* Find(Gwk::String strName)
{
    for (auto&& item : GetList())
    {
        if (item->Name() == strName)
            return item;
    }

    return nullptr;
}

Base::Base()
{
    GetList().push_back(this);
}

namespace Tools
{
    ControlList GetExportableChildren(Gwk::Controls::Base* root)
    {
        ControlList list;

        for (int i = 0; i < root->NumChildren(); i++)
        {
            Gwk::Controls::Base* baseChild = root->GetChild(i);

            if (!baseChild)
                continue;

            //
            // If we have a child is isn't exportable - maybe it has a
            // child that is
            // We will count it as one of our children.
            //
            if (!baseChild->UserData.Exists("ControlFactory"))
            {
                list.Add(GetExportableChildren(baseChild));
                continue;
            }

            list.Add(baseChild);
        }

        return list;
    }
}

} // namespace ImportExport
} // namespace Gwk
