#include <engine/ui/elements.h>

namespace GEC {
namespace UI {
    namespace Elements {
        InputField::InputField()
        {
            this->font = GEC::TextRender::Font::GetDefault();
        }
        InputField::~InputField() { }

        void InputField::Update() { }
        void InputField::Interact()
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

        void InputField::Render()
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

        std::string InputField::GetValue() const
        {
            return text;
        }
        void InputField::SetValue(std::string text)
        {
            this->text = text;
            this->last_value = this->text;
        }
        void InputField::SetValueAsUser(std::string text)
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
        bool InputField::Changed() const
        {
            return changed;
        }
    }
}
}