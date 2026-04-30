#include <engine/ui/elements.h>

namespace GEC {
namespace UI {
    namespace Elements {
        void Button::Interact()
        {
            Clickable::Interact();
        }
        void Button::Render()
        {
            if (!hover)
                GEC::Render::SetColor(buttonColor.X(), buttonColor.Y(), buttonColor.W(), buttonColor.H());
            else
                GEC::Render::SetColor(buttonColor.X() - 30, buttonColor.Y() - 30, buttonColor.W() - 30, buttonColor.H());
            GEC::Render::Rect(x, y, width, height);

            GEC::Render::SetColor(textColor.X(), textColor.Y(), textColor.W(), textColor.H());
            GEC::TextRender::Text text(font);
            text.SetText(this->text);
            text.Align(1, 1);
            text.Render(x, y, -0.001f, height - height / 5);
        }
        void Button::Set(std::string text, float x, float y, float width, float height)
        {
            this->text = text;
            Set(x, y, width, height);
        }
        void Button::Set(float x, float y, float width, float height)

        {
            Clickable::Set(x, y, width, height);
        }
        void Button::SetButtonColor(float r, float g, float b)
        {
            buttonColor.X() = r;
            buttonColor.Y() = g;
            buttonColor.W() = b;
        }
        void Button::SetTextColor(float r, float g, float b)
        {
            textColor.X() = r;
            textColor.Y() = g;
            textColor.W() = b;
        }
        void Button::SetFont(GEC::TextRender::Font* font)
        {
            this->font = font;
        }
        void ButtonOfButtons::Update()
        {
            float bobYInd = 0;
            bobYInd -= height / 2;

            GEC::Vector2<float, float> bobSize(width, height * .70f);

            for (int i = 0; i < buttons.size(); ++i) {
                bobYInd -= bobSize.Second() / 2;
                buttons[i]->Set(operations[i].First(), x, y + bobYInd, bobSize.First(), bobSize.Second());
                bobYInd -= bobSize.Second() / 2;
                buttons[i]->Update();
            }

            GEC::UI::Elements::Button::Update();
        }
        void ButtonOfButtons::Interact()
        {

            // Call Interact functions of button if they are visible
            if (buttonDropped)
                for (int i = 0; i < buttons.size(); ++i) {
                    buttons[i]->Interact();
                    if (buttons[i]->Clicked()) {
                        operations[i].Second()();

                        // Close the dropdown after an option is pressed
                        buttonDropped = false;
                    }
                }

            // Interact Script of this button, toggle other buttons when clicked
            GEC::UI::Elements::Button::Interact();
            if (Clicked())
                buttonDropped = !buttonDropped;

            // When the user clicks outside the object, close the dropdown
            if (GEC::Input::Mouse::GetInstance().LeftPressed()) {
                bool objectClicked = false;
                for (int i = 0; i < buttons.size(); ++i)
                    if (buttons[i]->Clicked())
                        objectClicked = true;
                if (Clicked())
                    objectClicked = true;

                if (!objectClicked)
                    buttonDropped = false;
            }
        }
        void ButtonOfButtons::Render()
        {
            if (buttonDropped)
                for (int i = 0; i < buttons.size(); ++i)
                    buttons[i]->Render();
            GEC::UI::Elements::Button::Render();
        }
        void ButtonOfButtons::AddOption(std::string name, std::function<void()> func)
        {
            int index = operations.size();
            operations.push_back(GEC::Vector2<std::string, std::function<void()>>(name, func));

            buttons.push_back(new Button());
        }
        ButtonOfButtons::~ButtonOfButtons()
        {
            for (int i = 0; i < buttons.size(); ++i)
                delete buttons[i];
        }

    }
}
}
