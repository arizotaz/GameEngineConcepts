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
                    if (GEC::Input::Keyboard::IsKeyPressed(8) || GEC::Input::Keyboard::IsKeyPressed(127))
                        this->text = text.substr(0, text.length() - 2);
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
            void SetValueAsUser(std::string text)
            {
                this->text = text;
                this->last_value = "";
                this->changed = true;
                this->selected = false;
                this->last_state = true;
            }

            /**
             * If the text in the field change when the element is no longer selected
             */
            bool Changed() const
            {
                return changed;
            }

        private:
            bool selected = false, last_state = false;
            std::string last_value = "";
            bool changed = false;
        };

        class Checkbox : public Button {
        public:
            Checkbox()
                : checkboxOffset(0, 0)
            {
                tr = new GEC::TextRender::Text();
            }
            ~Checkbox()
            {
                delete tr;
            };

            virtual void Interact() override
            {
                float lastX = x;
                float lastY = y;
                float lastWidth = width;
                float lastHeight = height;

                x = x + checkboxOffset.First();
                y = y + checkboxOffset.Second();
                width = height;
                height = height;

                Button::Interact();

                x = lastX;
                y = lastY;
                width = lastWidth;
                height = lastHeight;

                if (Clicked())
                    enabled = !enabled;

                changed = false;
                if (lastState != enabled)
                    changed = true;
                lastState = enabled;
            }
            virtual void Render() override
            {

                float fontSize = height / 1.5f;
                float buttonSize = height;
                tr->SetText(this->text);
                tr->Align(0, 1);

                GEC::Vector2<float, float> tsize = tr->GetSize(fontSize) / fontSize;

                float nSize = tsize.First() + 4 + buttonSize;

                checkboxOffset.Set(buttonSize / 2, 0);

                GEC::Render::SetColor(150);
                GEC::Render::Rect(x + checkboxOffset.First(), y + checkboxOffset.Second(), buttonSize, buttonSize);
                GEC::Render::SetColor(80);
                GEC::Render::Rect(x + checkboxOffset.First(), y + checkboxOffset.Second(), buttonSize - 4, buttonSize - 4);
                if (enabled) {
                    GEC::Render::SetColor(255);
                    GEC::Render::Rect(x + checkboxOffset.First(), y + checkboxOffset.Second(), buttonSize - 6, buttonSize - 6);
                }
                GEC::Render::SetColor(0,0,0);
                tr->Render(x + buttonSize + 4, y, 10.1, fontSize);
            }

            bool GetValue() const { return enabled; }
            void SetValue(bool val) { this->enabled = val; }
            bool Changed() const
            {
                return changed;
            }

        private:
            bool enabled, lastState, changed;
            GEC::TextRender::Text* tr;
            Vector2<float, float> checkboxOffset;
        };

    }
}
}

#endif