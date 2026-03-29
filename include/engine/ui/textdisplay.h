#ifndef ENGINE_UI_ELEMENT_TEXT_DISPLAY_H
#define ENGINE_UI_ELEMENT_TEXT_DISPLAY_H 1

#include <engine/renderobjects.h>
#include <engine/text.h>
#include <engine/ui/element.h>
#include <engine/ui/panels.h>

namespace GEC {
namespace UI {
    namespace Elements {

        class TextDisplay : public MouseInteractor {
        public:
            TextDisplay(std::string text)
                : textColor(0, 0, 0, 255)
                , align(0, 0)
            {
                this->text = text;
            }

            void Align(int horozontal, int vertical)
            {
                align.Set(horozontal, vertical);
            }
            void SetFont(GEC::TextRender::Font* f) {
                this->font = f;
            }

            void SetColor(float b)
            {
                SetColor(b, b, b);
            }
            void SetColor(float b, float a)
            {
                SetColor(b, b, b, a);
            }
            void SetColor(float r, float g, float b)
            {
                SetColor(r, g, b, 255);
            }
            void SetColor(float r, float g, float b, float a)
            {
                textColor.X() = r;
                textColor.Y() = g;
                textColor.W() = b;
                textColor.H() = a;
            }

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