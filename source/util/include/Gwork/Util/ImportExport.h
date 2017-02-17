/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */

#pragma once

#include <Gwork/Gwork.h>
#include <Gwork/Controls.h>
#include <Gwork/Util/ControlReflect.h>

using namespace Gwk;

namespace Gwk
{
    namespace ImportExport
    {
        class Base
        {
        public:

            Base();

            virtual Gwk::String Name() = 0;

            virtual bool CanImport() = 0;
            virtual void Import(Gwk::Controls::Base* root, const Gwk::String& strFilename) = 0;

            virtual bool CanExport() = 0;
            virtual void Export(Gwk::Controls::Base* root, const Gwk::String& strFilename) = 0;

        public:

            typedef std::list<ImportExport::Base*> List;

        protected:

        };


        ImportExport::Base::List& GetList();
        ImportExport::Base*       Find(Gwk::String strName);

        namespace Tools
        {
            ControlList GetExportableChildren(Gwk::Controls::Base* root);
        }
    }
}

#define GWK_IMPORTEXPORT(name) \
    void GworkImportExport_##name() \
    { \
        static name my_instance; \
    }

#define DECLARE_GWK_IMPORTEXPORT(name) \
    void GworkImportExport_##name(); \
    GworkImportExport_##name();
