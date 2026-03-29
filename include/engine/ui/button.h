#ifndef ENGINE_UI_ELEMENT_BUTTON_H
#define ENGINE_UI_ELEMENT_BUTTON_H 1

#include <engine/renderobjects.h>
#include <engine/text.h>
#include <engine/ui/element.h>

namespace GEC {
namespace UI {
    namespace Elements {

        class Button : public Clickable {
        public:
            Button()
                : buttonColor(255, 255, 255, 255)
                , textColor(25,25,25, 255)
            {
            }
            ~Button() { }
            void Update() { }
            void Interact()
            {
                Clickable::Interact();
                if (this->clicked) {
                    int i;
                }
            }
            void Render()
            {
                glDisable(GL_DEPTH_TEST);
                if (!hover) GEC::Render::SetColor(buttonColor.X(), buttonColor.Y(), buttonColor.W(),buttonColor.H());
                else GEC::Render::SetColor(buttonColor.X()-30, buttonColor.Y()-30, buttonColor.W()-30,buttonColor.H());
                GEC::Render::Rect(x, y, width, height);

                GEC::Render::SetColor(textColor.X(), textColor.Y(), textColor.W(), textColor.H());
                GEC::TextRender::Text text(font);
                text.SetText(this->text);
                text.Align(1, 1);
                text.Render(x,y, -0.001f, height-height/5);
                
            }

            void Set(std::string text, float x, float y, float width, float height)
            {
                this->text = text;
                Clickable::Set(x, y, width, height);
            }

            void SetButtonColor(float r, float g, float b)
            {
                buttonColor.X() = r;
                buttonColor.Y() = g;
                buttonColor.W() = b;
            }
            void SetTextColor(float r, float g, float b)
            {
                textColor.X() = r;
                textColor.Y() = g;
                textColor.W() = b;
            }

            void SetFont(GEC::TextRender::Font* font)
            {
                this->font = font;
            }

        protected:
            GEC::Rect<float, float, float, float> buttonColor, textColor;
            std::string text;
            GEC::TextRender::Font* font;
        };

    }
}
}

#endif