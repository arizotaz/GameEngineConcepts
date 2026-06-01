#include <engine/ui/elements.h>

namespace GEC {
namespace UI {
    namespace Elements {
        Panel::Panel(float b)
            : panelColor(0, 0, 0, 0)
        {
            SetColor(b);
        }
        Panel::Panel(float b, float a)
            : panelColor(0, 0, 0, 0)
        {
            SetColor(b, a);
        }
        Panel::Panel(float r, float g, float b)
            : panelColor(0, 0, 0, 0)
        {
            SetColor(r, g, b);
        }
        Panel::Panel(float r, float g, float b, float a)
            : panelColor(0, 0, 0, 0)
        {
            SetColor(r, g, b, a);
        }
        void Panel::SetColor(float b)
        {
            SetColor(b, b, b);
        }
        void Panel::SetColor(float b, float a)
        {
            SetColor(b, b, b, a);
        }
        void Panel::SetColor(float r, float g, float b)
        {
            SetColor(r, g, b, 255);
        }
        void Panel::SetColor(float r, float g, float b, float a)
        {
            panelColor.X() = r;
            panelColor.Y() = g;
            panelColor.W() = b;
            panelColor.H() = a;
        }
        void Panel::Render()
        {
            GEC::Render::SetColor(panelColor);
            GEC::Render::Rect(x, y, 10, width, height);
        }
    }
}
}