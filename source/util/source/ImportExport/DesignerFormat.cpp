
#include <Gwork/Util/ImportExport.h>
#include "GworkUtil.h"

#ifndef _MSC_VER
#   include <string.h>
#endif

namespace Gwk {
namespace ImportExport {


class DesignerFormat : public ImportExport::Base
{
public:

    DesignerFormat();

    virtual Gwk::String Name()
    {
        return "Designer";
    }

    virtual bool CanImport()
    {
        return true;
    }

    virtual void Import(Gwk::Controls::Base* root, const Gwk::String& strFilename);

    virtual bool CanExport()
    {
        return true;
    }

    virtual void Export(Gwk::Controls::Base* root, const Gwk::String& strFilename);

    void ExportToTree(Gwk::Controls::Base* root, GwkUtil::Data::Tree& tree);
    void ImportFromTree(Gwk::Controls::Base* root, GwkUtil::Data::Tree& tree);

};


GWK_IMPORTEXPORT(DesignerFormat);


DesignerFormat::DesignerFormat()
{
}

void DesignerFormat::Import(Gwk::Controls::Base* root, const Gwk::String& strFilename)
{
    GwkUtil::BString strContents;

    if (!GwkUtil::File::Read(strFilename, strContents))
        return;

    GwkUtil::Data::Tree tree;
    GwkUtil::Data::Json::Import(tree, strContents);

    if (!tree.HasChild("Controls"))
        return;

    ImportFromTree(root, tree.GetChild("Controls"));
}

void DesignerFormat::ImportFromTree(Gwk::Controls::Base* root, GwkUtil::Data::Tree& tree)
{
    ControlFactory::Base* rootFactory = ControlFactory::Find("Base");

    if (root->UserData.Exists("ControlFactory"))
        rootFactory = root->UserData.Get<ControlFactory::Base*>("ControlFactory");

    if (tree.HasChild("Properties"))
    {
        GwkUtil::Data::Tree& props = tree.GetChild("Properties");
        
        GWKUTIL_FOREACH(p, props.Children(), GwkUtil::Data::Tree::List)
        {
            ControlFactory::Property *prop = rootFactory->GetProperty(p->Name());
            if (prop)
            {
                if (p->HasChildren())
                {
                    GWKUTIL_FOREACH(pc, p->Children(), GwkUtil::Data::Tree::List)
                    {
                        prop->NumSet(root, pc->Name(), pc->Var<float>());
                    }
                }
                else
                {
                    rootFactory->SetControlValue(root, p->Name(), p->Value());
                }
            }
        }
    }

    if (tree.HasChild("Children"))
    {
        GwkUtil::Data::Tree& childObject = tree.GetChild("Children");
        GWKUTIL_FOREACH(c, childObject.Children(), GwkUtil::Data::Tree::List)
        {
            GwkUtil::BString strType = c->ChildValue("Type");
            ControlFactory::Base* factory = ControlFactory::Find(strType);

            if (!factory)
                continue;

            Gwk::Controls::Base* control = factory->CreateInstance(root);

            if (!control)
                continue;

            // Tell the control we're here and we're queer
            {
                int iPage = c->ChildVar<int>("Page", 0);
                rootFactory->AddChild(root, control, iPage);
            }
            control->SetMouseInputEnabled(true);
            control->UserData.Set("ControlFactory", factory);
            ImportFromTree(control, *c);
        }
    }
}

void DesignerFormat::Export(Gwk::Controls::Base* root, const Gwk::String& strFilename)
{
    GwkUtil::Data::Tree tree;
    ExportToTree(root, tree);
    GwkUtil::BString strOutput;

    if (GwkUtil::Data::Json::Export(tree, strOutput, true))
        GwkUtil::File::Write(strFilename, strOutput);
}

void DesignerFormat::ExportToTree(Gwk::Controls::Base* root, GwkUtil::Data::Tree& tree)
{
    GwkUtil::Data::Tree* me = &tree;

    if (strcmp(root->GetTypeName(), "DocumentCanvas") == 0)
        me = &tree.AddChild("Controls");
    else
        me = &tree.AddChild();

    me->SetChild("Type", root->GetTypeName());

    //
    // Set properties from the control factory
    //
    if (root->UserData.Exists("ControlFactory"))
    {
        GwkUtil::Data::Tree& props = me->AddChild("Properties");
        ControlFactory::Base* cF = root->UserData.Get<ControlFactory::Base*>("ControlFactory");
        // Save the ParentPage
        {
            int iParentPage = cF->GetParentPage(root);

            if (iParentPage != 0)
                me->SetChildVar("Page", iParentPage);
        }

        while (cF)
        {
            for (auto const& property : cF->Properties())
            {
                if (property->NumCount() > 0)
                {
                    GwkUtil::Data::Tree& prop = props.AddChild(property->Name());

                    for (int i = 0; i < property->NumCount(); i++)
                    {
                        prop.SetChildVar<float>(property->NumName(i), property->NumGet(root, i));
                    }
                }
                else
                {
                    props.SetChild(property->Name(), property->GetValueAsString(root));
                }
            }

            cF = cF->GetBaseFactory();
        }
    }

    ControlList list = ImportExport::Tools::GetExportableChildren(root);

    if (!list.list.empty())
    {
        GwkUtil::Data::Tree& children = me->AddChild("Children");
        ControlList::List::iterator it = list.list.begin();
        ControlList::List::iterator itEnd = list.list.end();

        while (it != itEnd)
        {
            ExportToTree(*it, children);
            ++it;
        }
    }
}


} // namespace ImportExport
} // namespace Gwk
