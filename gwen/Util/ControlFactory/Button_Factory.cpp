
#include "GwenUtil/ControlFactory.h"
#include "Gwen/Controls.h"

namespace Gwen
{
    namespace ControlFactory
    {
        class Button_Factory : public Gwen::ControlFactory::Base
        {
        public:
            
            GWEN_CONTROL_FACTORY_FOR(Button, Label)
            {
            }
            
            virtual Gwen::Controls::Base* CreateInstance(Gwen::Controls::Base* parent)
            {
                Gwen::Controls::Button* pControl = new Gwen::Controls::Button(parent);
                pControl->SetSize(100, 20);
                pControl->SetText("New Button");
                return pControl;
            }

        };

        GWEN_CONTROL_FACTORY(Button_Factory);
    }
    
    
//    namespace MetaData
//    {
//        
//        class ControlFactory
//        {
//        public:
//            virtual Binder& Create(const char *name, auto createFn)
//            {
//                return *this;
//            }
//            
//        protected:
//            void RegisterFactory(const char *name, )
//        };
//        
//        
//        class LuaBinder
//        {
//            virtual
//        };
//        
//        
//        class Button_Factory : public ControlFactory
//        {
//        public:
//            
//            GWEN_BINDER_CONSTRUCT(Button_Factory, Gwen::ControlFactory2::Base)
//            {
//            }
//            
//            GWEN_BINDING(Button);   // Control to bind
//            
//            virtual Gwen::Controls::Base* CreateInstance(Gwen::Controls::Base* parent)
//            {
//                Gwen::Controls::Button* pControl = new Gwen::Controls::Button(parent);
//                pControl->SetSize(100, 20);
//                pControl->SetText("New Button");
//                return pControl;
//            }
//            
//            void Describe(Binder &bind)
//            {
//                bind
//                    .Property("name",
//                              [] (Controls::Base *c) { return c->Name; },
//                              [] (Controls::Base *c, const char *name) { c->SetName(name); })
//                ;
//                
////                ParentClass::Describe(bind);
////                bind.Property()
//            }
//            
//        };
//        
//        GWEN_CONTROL_FACTORY(Button_Factory);
//    }

}
