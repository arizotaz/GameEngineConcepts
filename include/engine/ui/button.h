// #############################################################################
// # button.h
// #############################################################################
// # Written by Colton Staiduhar
// # Date Created:       03/15/2022
// # Date Ported:        03/15/2026
// # Last Modification:  03/18/2026
// #############################################################################
// # Defines the code used to create a button that can be rendered and managed
// # by the element renderer.  It uses the element registry to track element
// # overlap and only resond to clicks when it is the top element.
// #
// # This function is similar to every other "clickable" derived element with
// # the exception that it can be rendered with text on it.
// #
// #############################################################################

#ifndef ENGINE_UI_ELEMENT_BUTTON_H
#define ENGINE_UI_ELEMENT_BUTTON_H 1

#include <engine/renderobjects.h>
#include <engine/text.h>
#include <engine/ui/element.h>

namespace GEC {
namespace UI {
    namespace Elements {

        /** Basic button class for elements in the element renderer */
        class Button : public Clickable {
        public:
            /** Default Constructor */
            Button()
                : buttonColor(255, 255, 255, 255)
                , textColor(25, 25, 25, 255)
            {
            }

            // Deconstructor
            ~Button() { }

            /** Main Update Loop */
            void Update(){}

            /** Interact Event */
            void Interact()
            {
                Clickable::Interact();
            }

            /** Render the button  */
            void Render()
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

            /** Sets the values of the button
             * @param text - the text to be displayed on the button
             * @param x - the x position of the button
             * @param y - the y position of the button
             * @param width - the width of the button
             * @param height - the height of the button
             */
            void Set(std::string text, float x, float y, float width, float height)
            {
                this->text = text;
                Clickable::Set(x, y, width, height);
            }

            /** Sets the color of the button object
             * @param r - the red channel of the color
             * @param g - the green channal of the color
             * @param b - the blue channel of the color
             */
            void SetButtonColor(float r, float g, float b)
            {
                buttonColor.X() = r;
                buttonColor.Y() = g;
                buttonColor.W() = b;
            }

            /** Sets the color of the button text
             * @param r - the red channel of the button text color
             * @param g - the green channal of the button text color
             * @param b - the blue channel of the button text color
             */
            void SetTextColor(float r, float g, float b)
            {
                textColor.X() = r;
                textColor.Y() = g;
                textColor.W() = b;
            }

            /** Sets the text font to render */
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