#ifndef ENGINE_UI_ELEMENT_PANELS_H
#define ENGINE_UI_ELEMENT_PANELS_H 1

#include <engine/renderobjects.h>
#include <engine/ui/element.h>

namespace GEC {
namespace UI {
    namespace Elements {

        class MouseInteractor : public Element {
        public:
            MouseInteractor()
            {
                mouse = &GEC::Input::Mouse::GetInstance();
            }
            void Update() { }
            void Interact()
            {
                if (!ElementRegistry::GetInstance().ShouldStopInteract())
                    if (MouseInBounds(mouse, x, y, width, height))
                        ElementRegistry::GetInstance().ShouldStopInteract();
            }
            void Render() { }

        protected:
            GEC::Input::Mouse* mouse;
        };

        class Panel : public MouseInteractor {
        public:
            Panel(float b)
                : panelColor(0, 0, 0, 0)
            {
                SetColor(b);
            }
            Panel(float b, float a)
                : panelColor(0, 0, 0, 0)
            {
                SetColor(b, a);
            }
            Panel(float r, float g, float b)
                : panelColor(0, 0, 0, 0)
            {
                SetColor(r, g, b);
            }
            Panel(float r, float g, float b, float a)
                : panelColor(0, 0, 0, 0)
            {
                SetColor(r, g, b, a);
            }

            void SetColor(float b)
            {
                SetColor(b, b, b);
            }
            void SetColor(float b, float a)
            {
                SetColor(b, b, b, a);
            }
            void SetColor(float r, float g, float b)
            {
                SetColor(r, g, b, 255);
            }
            void SetColor(float r, float g, float b, float a)
            {
                panelColor.X() = r;
                panelColor.Y() = g;
                panelColor.W() = b;
                panelColor.H() = a;
            }

            void Render() override
            {
                GEC::Render::SetColor(panelColor);
                GEC::Render::Rect(x,y, 10,width,height);
            }

        protected:
            GEC::Rect<float, float, float, float> panelColor;
        };
    }
}
}

#endif