// #############################################################################
// # elements.h
// #############################################################################
// # Written by Colton Staiduhar
// # Date Created:       04/27/2026
// # Last Modification:  04/27/2026
// #############################################################################
// # This is a combination header file that implements multiple different types
// # of elements for use in the engine.
// #############################################################################

#ifndef GEC_ENGINE_UI_ELEMENTS
#define GEC_ENGINE_UI_ELEMENTS 1

#include <engine/renderobjects.h>
#include <engine/text.h>
#include <engine/ui/element.h>
#include <vector>
#include <functional>

namespace GEC {
namespace UI {
    namespace Elements {

        // #############################################################################
        // # button
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
        /** Basic button class for elements in the element renderer */
        class Button : public Clickable {
        public:
            /** Default Constructor */
            Button()
                : buttonColor(255, 255, 255, 255)
                , textColor(25, 25, 25, 255)
            {
                this->font = GEC::TextRender::Font::GetDefault();
            }

            // Deconstructor
            ~Button() { }

            /** Main Update Loop */
            virtual void Update() override { }

            /** Interact Event */
            virtual void Interact() override;

            /** Render the button  */
            virtual void Render() override;

            /** Sets the values of the button
             * @param text - the text to be displayed on the button
             * @param x - the x position of the button
             * @param y - the y position of the button
             * @param width - the width of the button
             * @param height - the height of the button
             */
            void Set(std::string text, float x, float y, float width, float height);

            /** Sets the values of the button
             * @param x - the x position of the button
             * @param y - the y position of the button
             * @param width - the width of the button
             * @param height - the height of the button
             */
            void Set(float x, float y, float width, float height);

            /** Sets the color of the button object
             * @param r - the red channel of the color
             * @param g - the green channal of the color
             * @param b - the blue channel of the color
             */
            void SetButtonColor(float r, float g, float b);

            /** Sets the color of the button text
             * @param r - the red channel of the button text color
             * @param g - the green channal of the button text color
             * @param b - the blue channel of the button text color
             */
            void SetTextColor(float r, float g, float b);

            /** Sets the text font to render */
            void SetFont(GEC::TextRender::Font* font);

        protected:
            GEC::Rect<float, float, float, float> buttonColor, textColor;
            std::string text;
            GEC::TextRender::Font* font;
        };

        class ButtonOfButtons : public Button {
        public:
            ButtonOfButtons() { };

            virtual void Update();
            virtual void Interact();
            virtual void Render();

            void AddOption(std::string name, std::function<void()> func);

            virtual ~ButtonOfButtons();

        private:
            bool buttonDropped = false;
            std::vector<Button*> buttons;

            std::vector<GEC::Vector2<std::string, std::function<void()>>> operations;
        };

        // #############################################################################
        // # Panels
        // #############################################################################
        // # Written by Colton Staiduhar
        // # Date Created:       03/17/2022
        // # Last Modification:  03/17/2026
        // #############################################################################
        // # Defines elements panel elements to be used in the program
        // #############################################################################

        /**
         * Similar to the Clickable object in <element.h>
         * Except it hides the click state functions.
         * This type of object only receives click events and stops click throughs
         */
        class MouseInteractor : public Element {
        public:
            MouseInteractor();
            void Update();
            void Interact();
            void Render();

        protected:
            GEC::Input::Mouse* mouse;
        };

        /**
         * Panel Object, used to create simple rectangle element on the screen.
         * This element does not allow for click through operations as it derived from the MouseInteractor Element
         */
        class Panel : public MouseInteractor {
        public:
            /**
             * Create a panel with a luminence value
             */
            Panel(float b);
            /**
             * Create a panel with a luminance and alpha value
             */
            Panel(float b, float a);
            /**
             * Create a panel with R,G,and B values
             */
            Panel(float r, float g, float b);
            /**
             * Create a panel with R,G,B values and an alpha channel
             */
            Panel(float r, float g, float b, float a);

            /**
             * Set the oclor of the panel to a luminance value
             */
            void SetColor(float b);
            /**
             * Set the color of the panel to a luminance value with an alpha channel
             */
            void SetColor(float b, float a);

            /**
             * Set the color of the panel to an RGB Value
             */
            void SetColor(float r, float g, float b);

            /**
             * Sets the color of the panel
             */
            void SetColor(float r, float g, float b, float a);

            /**
             * Render the panel element
             */
            void Render() override;

        protected:
            GEC::Rect<float, float, float, float> panelColor;
        };

        // #############################################################################
        // # textDisplay.h
        // #############################################################################
        // # Written by Colton Staiduhar
        // # Date Created:       03/16/2022
        // # Last Modification:  03/17/2026
        // #############################################################################
        // # Defines an element implementation for the text renderer
        // # This is simply just a means of adapting text to be used in the element
        // # renderer.
        // #############################################################################
        /**
         * A container ot render text with the element renderer
         */
        class TextDisplay : public MouseInteractor {
        public:
            /**
             * Create a text display with a given block of text
             */
            TextDisplay(std::string text);

            /**
             * Executes the Align function of the text object
             */
            void Align(int horozontal, int vertical);

            /**
             * Test the font to be used to render text
             */
            void SetFont(GEC::TextRender::Font* f);

            /**
             * Sets the text color to a luminence value
             */
            void SetColor(float b);
            /**
             * Sets the text color to a luminence value and alpha value
             */
            void SetColor(float b, float a);

            /**
             * Sets the text color to an RGB Value
             */
            void SetColor(float r, float g, float b);

            /**
             * Sets color to an RGBA value
             */
            void SetColor(float r, float g, float b, float a);

            /**
             * The render funciton of the Element
             */
            void Render();

        protected:
            GEC::Vector2<int, int> align;
            GEC::TextRender::Font* font;
            GEC::Rect<float, float, float, float> textColor;
            std::string text;
        };

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
        class InputField : public GEC::UI::Elements::Button {
        public:
            InputField();
            ~InputField();

            void Update();
            void Interact();

            void Render();

            std::string GetValue() const;
            void SetValue(std::string text);
            void SetValueAsUser(std::string text);

            /**
             * If the text in the field change when the element is no longer selected
             */
            bool Changed() const;

        private:
            bool selected = false, last_state = false;
            std::string last_value = "";
            bool changed = false;
        };

        class Checkbox : public Button {
        public:
            Checkbox();
            ~Checkbox();

            virtual void Interact() override;
            virtual void Render() override;

            bool GetValue() const;
            void SetValue(bool val);
            bool Changed() const;

        private:
            bool enabled, lastState, changed;
            GEC::TextRender::Text* tr;
            Vector2<float, float> checkboxOffset;
        };

    }
}
}

#endif