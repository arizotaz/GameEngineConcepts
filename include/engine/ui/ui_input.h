// #############################################################################
// # in.h
// #############################################################################
// # Written by Colton Staiduhar
// # Date Created:       03/15/2022
// # Date Ported:        04/15/2026
// # Last Modification:  04/15/2026
// #############################################################################
// # Defines the code used to create different inputs that can be rendered
// # and managed by the element renderer.  It uses the element registry to
// # track element overlap and only resond to clicks when it is the top element.
// #
// #############################################################################

#ifndef ENGINE_UI_ELEMENT_INPUT_H
#define ENGINE_UI_ELEMENT_INPUT_H 1

#include <engine/renderobjects.h>
#include <engine/text.h>
#include <engine/ui/button.h>
#include <engine/ui/element.h>
#include <vector>

namespace GEC {
namespace UI {
    namespace Elements {

        class InputField : public GEC::UI::Elements::Button {
        public:
            InputField()
            {
                this->font = GEC::TextRender::Font::GetDefault();
            }
            ~InputField() { }

            void Update() { }
            void Interact()
            {
                Button::Interact();
                if (this->clicked)
                    selected = true;
                if (GEC::Input::Mouse::GetInstance().LeftPressed() && !this->clicked)
                    selected = false;

                if (selected && !last_state)
                    GEC::Input::Keyboard::GetStringInput();

                if (selected) {
                    this->text += GEC::Input::Keyboard::GetStringInput();
                    if (GEC::Input::Keyboard::IsSpecialKeyPressed(111))
                        this->text = text.substr(0, text.length() - 1);
                }
                changed = false;
                if (!selected && last_state)
                    changed = (text != last_value);

                last_state = selected;
            }

            void Render()
            {
                GEC::Render::SetColor(80, 80, 80);
                GEC::Render::Rect(x, y, width, height);

                if (selected)
                    GEC::Render::SetColor(buttonColor.X(), buttonColor.Y(), buttonColor.W(), buttonColor.H());
                else
                    GEC::Render::SetColor(buttonColor.X() - 30, buttonColor.Y() - 30, buttonColor.W() - 30, buttonColor.H());
                GEC::Render::Rect(x, y, width - 4, height - 4);

                GEC::Render::SetColor(textColor.X(), textColor.Y(), textColor.W(), textColor.H());
                GEC::TextRender::Text text(font);
                text.SetText(this->text);
                text.Align(0, 1);
                float tHeight = height - 8;
                text.Render(x - width / 2 + 4, y, -0.001f, height - 8);
            }

            std::string GetValue() const
            {
                return text;
            }
            void SetValue(std::string text)
            {
                this->text = text;
                this->last_value = this->text;
            }

            /**
             * If the text in the field change when the element is no longer selected
             */
            bool Changed() const {
                return changed;
            }

        private:
            bool selected = false, last_state = false;
            std::string last_value = "";
            bool changed = false;
        };

    }
}
}

#endif