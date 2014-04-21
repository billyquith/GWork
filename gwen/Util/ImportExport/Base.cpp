#include "ImportExport.h"

namespace Gwen {
namespace ImportExport {

void InitializeFormats()
{
    DECLARE_GWEN_IMPORTEXPORT(DesignerFormat);
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

ImportExport::Base* Find(Gwen::String strName)
{
    ImportExport::Base::List::iterator it = GetList().begin();
    ImportExport::Base::List::iterator itEnd = GetList().end();
    
    while (it != itEnd)
    {
        if ((*it)->Name() == strName)
            return *it;
        
        ++it;
    }
    
    return NULL;
}

Base::Base()
{
    GetList().push_back(this);
}

namespace Tools
{
    ControlList GetExportableChildren(Gwen::Controls::Base* pRoot)
    {
        ControlList list;
        
        for (int i = 0; i < pRoot->NumChildren(); i++)
        {
            Gwen::Controls::Base* pBaseChild = pRoot->GetChild(i);
            
            if (!pBaseChild)
                continue;
            
            //
            // If we have a child is isn't exportable - maybe it has a
            // child that is
            // We will count it as one of our children.
            //
            if (!pBaseChild->UserData.Exists("ControlFactory"))
            {
                list.Add(GetExportableChildren(pBaseChild));
                continue;
            }
            
            list.Add(pBaseChild);
        }
        
        return list;
    }
    
}

} // namespace ImportExport
} // namespace Gwen
