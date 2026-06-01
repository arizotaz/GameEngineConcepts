#include <engine/ui/elements.h>

namespace GEC {
namespace UI {
    namespace Elements {
        TextDisplay::TextDisplay(std::string text)
            : textColor(0, 0, 0, 255)
            , align(0, 0)
        {
            this->text = text;
            this->font = GEC::TextRender::Font::GetDefault();
        }
        void TextDisplay::Align(int horozontal, int vertical)
        {
            align.Set(horozontal, vertical);
        }
        void TextDisplay::SetFont(GEC::TextRender::Font* f)
        {
            this->font = f;
        }
        void TextDisplay::SetColor(float b)
        {
            SetColor(b, b, b);
        }
        void TextDisplay::SetColor(float b, float a)
        {
            SetColor(b, b, b, a);
        }
        void TextDisplay::SetColor(float r, float g, float b)
        {
            SetColor(r, g, b, 255);
        }
        void TextDisplay::SetColor(float r, float g, float b, float a)
        {
            textColor.X() = r;
            textColor.Y() = g;
            textColor.W() = b;
            textColor.H() = a;
        }
        void TextDisplay::Render()
        {
            GEC::Render::SetColor(textColor);
            GEC::TextRender::Text t(font);
            t.SetText(text);
            t.Align(align.First(), align.Second());
            t.Render(x, y, 10.1, height);
        }
    }
}
}