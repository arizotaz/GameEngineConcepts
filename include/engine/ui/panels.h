// #############################################################################
// # Panels.h
// #############################################################################
// # Written by Colton Staiduhar
// # Date Created:       03/17/2022
// # Last Modification:  03/17/2025
// #############################################################################
// # Defines elements panel elements to be used in the program
// #############################################################################


#ifndef ENGINE_UI_ELEMENT_PANELS_H
#define ENGINE_UI_ELEMENT_PANELS_H 1

#include <engine/renderobjects.h>
#include <engine/ui/element.h>

namespace GEC {
namespace UI {
    namespace Elements {

        /**
         * Similar to the Clickable object in <element.h>
         * Except it hides the click state functions.
         * This type of object only receives click events and stops click throughs
         */
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

        /**
         * Panel Object, used to create simple rectangle element on the screen.
         * This element does not allow for click through operations as it derived from the MouseInteractor Element
         */
        class Panel : public MouseInteractor {
        public:
            /**
             * Create a panel with a luminence value
             */
            Panel(float b)
                : panelColor(0, 0, 0, 0)
            {
                SetColor(b);
            }
            /**
             * Create a panel with a luminance and alpha value
             */
            Panel(float b, float a)
                : panelColor(0, 0, 0, 0)
            {
                SetColor(b, a);
            }
            /**
             * Create a panel with R,G,and B values
             */
            Panel(float r, float g, float b)
                : panelColor(0, 0, 0, 0)
            {
                SetColor(r, g, b);
            }
            /**
             * Create a panel with R,G,B values and an alpha channel
             */
            Panel(float r, float g, float b, float a)
                : panelColor(0, 0, 0, 0)
            {
                SetColor(r, g, b, a);
            }

            /**
             * Set the oclor of the panel to a luminance value
             */
            void SetColor(float b)
            {
                SetColor(b, b, b);
            }
            /**
             * Set the color of the panel to a luminance value with an alpha channel
             */
            void SetColor(float b, float a)
            {
                SetColor(b, b, b, a);
            }
            
            /**
             * Set the color of the panel to an RGB Value
             */
            void SetColor(float r, float g, float b)
            {
                SetColor(r, g, b, 255);
            }

            /**
             * Sets the color of the panel
             */
            void SetColor(float r, float g, float b, float a)
            {
                panelColor.X() = r;
                panelColor.Y() = g;
                panelColor.W() = b;
                panelColor.H() = a;
            }

            /**
             * Render the panel element
             */
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