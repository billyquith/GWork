
#include "GwenUtil/ImportExport.h"
#include "GwenUtil.h"

namespace Gwen {
namespace ImportExport {


class DesignerFormat : public ImportExport::Base
{
public:

    DesignerFormat();

    virtual Gwen::String Name()
    {
        return "Designer";
    }

    virtual bool CanImport()
    {
        return true;
    }

    virtual void Import(Gwen::Controls::Base* pRoot, const Gwen::String& strFilename);

    virtual bool CanExport()
    {
        return true;
    }

    virtual void Export(Gwen::Controls::Base* pRoot, const Gwen::String& strFilename);

    void ExportToTree(Gwen::Controls::Base* pRoot, GwenUtil::Data::Tree& tree);
    void ImportFromTree(Gwen::Controls::Base* pRoot, GwenUtil::Data::Tree& tree);

};


GWEN_IMPORTEXPORT(DesignerFormat);


DesignerFormat::DesignerFormat()
{
}

void DesignerFormat::Import(Gwen::Controls::Base* pRoot, const Gwen::String& strFilename)
{
    GwenUtil::BString strContents;

    if (!GwenUtil::File::Read(strFilename, strContents))
        return;

    GwenUtil::Data::Tree tree;
    GwenUtil::Data::Json::Import(tree, strContents);

    if (!tree.HasChild("Controls"))
        return;

    ImportFromTree(pRoot, tree.GetChild("Controls"));
}

void DesignerFormat::ImportFromTree(Gwen::Controls::Base* root, GwenUtil::Data::Tree& tree)
{
    ControlFactory::Base* rootFactory = ControlFactory::Find("Base");

    if (root->UserData.Exists("ControlFactory"))
        rootFactory = root->UserData.Get<ControlFactory::Base*>("ControlFactory");

    if (tree.HasChild("Properties"))
    {
        GwenUtil::Data::Tree& props = tree.GetChild("Properties");
        
        GWENUTIL_FOREACH(p, props.Children(), GwenUtil::Data::Tree::List)
        {
            ControlFactory::Property *prop = rootFactory->GetProperty(p->Name());
            if (prop)
            {
                if (p->HasChildren())
                {
                    GWENUTIL_FOREACH(pc, p->Children(), GwenUtil::Data::Tree::List)
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
        GwenUtil::Data::Tree& childObject = tree.GetChild("Children");
        GWENUTIL_FOREACH(c, childObject.Children(), GwenUtil::Data::Tree::List)
        {
            GwenUtil::BString strType = c->ChildValue("Type");
            ControlFactory::Base* factory = ControlFactory::Find(strType);

            if (!factory)
                continue;

            Gwen::Controls::Base* pControl = factory->CreateInstance(root);

            if (!pControl)
                continue;

            // Tell the control we're here and we're queer
            {
                int iPage = c->ChildVar<int>("Page", 0);
                rootFactory->AddChild(root, pControl, iPage);
            }
            pControl->SetMouseInputEnabled(true);
            pControl->UserData.Set("ControlFactory", factory);
            ImportFromTree(pControl, *c);
        }
    }
}

void DesignerFormat::Export(Gwen::Controls::Base* pRoot, const Gwen::String& strFilename)
{
    GwenUtil::Data::Tree tree;
    ExportToTree(pRoot, tree);
    GwenUtil::BString strOutput;

    if (GwenUtil::Data::Json::Export(tree, strOutput, true))
        GwenUtil::File::Write(strFilename, strOutput);
}

void DesignerFormat::ExportToTree(Gwen::Controls::Base* root, GwenUtil::Data::Tree& tree)
{
    GwenUtil::Data::Tree* me = &tree;

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
        GwenUtil::Data::Tree& props = me->AddChild("Properties");
        ControlFactory::Base* pCF = root->UserData.Get<ControlFactory::Base*>("ControlFactory");
        // Save the ParentPage
        {
            int iParentPage = pCF->GetParentPage(root);

            if (iParentPage != 0)
                me->SetChildVar("Page", iParentPage);
        }

        while (pCF)
        {
            for (ControlFactory::Property::List::const_iterator
                 it = pCF->Properties().begin(), itEnd = pCF->Properties().end();
                 it != itEnd; ++it)
            {
                if ((*it)->NumCount() > 0)
                {
                    GwenUtil::Data::Tree& prop = props.AddChild((*it)->Name());

                    for (int i = 0; i < (*it)->NumCount(); i++)
                    {
                        prop.SetChildVar<float>((*it)->NumName(i), (*it)->NumGet(root, i));
                    }
                }
                else
                {
                    props.SetChild((*it)->Name(), (*it)->GetValueAsString(root));
                }
            }

            pCF = pCF->GetBaseFactory();
        }
    }

    ControlList list = ImportExport::Tools::GetExportableChildren(root);

    if (!list.list.empty())
    {
        GwenUtil::Data::Tree& children = me->AddChild("Children");
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
} // namespace Gwen
