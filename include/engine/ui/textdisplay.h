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


#ifndef ENGINE_UI_ELEMENT_TEXT_DISPLAY_H
#define ENGINE_UI_ELEMENT_TEXT_DISPLAY_H 1

#include <engine/renderobjects.h>
#include <engine/text.h>
#include <engine/ui/element.h>
#include <engine/ui/panels.h>

namespace GEC {
namespace UI {
    namespace Elements {

        /**
         * A container ot render text with the element renderer
         */
        class TextDisplay : public MouseInteractor {
        public:

            /**
             * Create a text display with a given block of text
             */
            TextDisplay(std::string text)
                : textColor(0, 0, 0, 255)
                , align(0, 0)
            {
                this->text = text;
            }

            /**
             * Executes the Align function of the text object
             */
            void Align(int horozontal, int vertical)
            {
                align.Set(horozontal, vertical);
            }

            /**
             * Test the font to be used to render text
             */
            void SetFont(GEC::TextRender::Font* f) {
                this->font = f;
            }

            /**
             * Sets the text color to a luminence value
             */
            void SetColor(float b)
            {
                SetColor(b, b, b);
            }
            /**
             * Sets the text color to a luminence value and alpha value
             */
            void SetColor(float b, float a)
            {
                SetColor(b, b, b, a);
            }

            /**
             * Sets the text color to an RGB Value
             */
            void SetColor(float r, float g, float b)
            {
                SetColor(r, g, b, 255);
            }

            /**
             * Sets color to an RGBA value
             */
            void SetColor(float r, float g, float b, float a)
            {
                textColor.X() = r;
                textColor.Y() = g;
                textColor.W() = b;
                textColor.H() = a;
            }

            /**
             * The render funciton of the Element
             */
            void Render()
            {
                GEC::Render::SetColor(textColor);
                GEC::TextRender::Text t(font);
                t.SetText(text);
                t.Align(align.First(), align.Second());
                t.Render(x, y, 10.1, height);
            }

        protected:
            GEC::Vector2<int, int> align;
            GEC::TextRender::Font* font;
            GEC::Rect<float, float, float, float> textColor;
            std::string text;
        };

    }
}
}

#endif