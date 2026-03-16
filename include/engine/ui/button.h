#ifndef ENGINE_UI_ELEMENT_BUTTON_H
#define ENGINE_UI_ELEMENT_BUTTON_H 1

#include <engine/renderobjects.h>
#include <engine/ui/element.h>

namespace GEC {
namespace UI {
    namespace Elements {

        class Button : public Clickable {
        public:
            Button()
                : color(255, 255, 255, 255)
            {
            }
            ~Button() { }
            void Update() { }
            void Interact()
            {
                Clickable::Interact();
            }
            void Render()
            {
                if (hover)
                    GEC::Render::SetColor(color.X()-50, color.Y()-50, color.W()-50, color.H());
                else
                    GEC::Render::SetColor(color.X(), color.Y(), color.W(), color.H());
                GEC::Render::Rect(x, y, width, height);
            }

            void Set(std::string text, float x, float y, float width, float height)
            {
                this->text = text;
                Clickable::Set(x, y, width, height);
            }

            void SetColor(float r, float g, float b)
            {
                color.X() = r;
                color.Y() = g;
                color.W() = b;
            }

        protected:
            GEC::Rect<float, float, float, float> color;
            std::string text;
        };

    }
}
}

#endif