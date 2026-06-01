#include <engine/ui/elements.h>

namespace GEC {
namespace UI {
    namespace Elements {

        Checkbox::Checkbox()
            : checkboxOffset(0, 0)
        {
            tr = new GEC::TextRender::Text();
        }
        Checkbox::~Checkbox()
        {
            delete tr;
        };

        void Checkbox::Interact()
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
        void Checkbox::Render()
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
            GEC::Render::SetColor(0, 0, 0);
            tr->Render(x + buttonSize + 4, y, 10.1, fontSize);
        }

        bool Checkbox::GetValue() const { return enabled; }
        void Checkbox::SetValue(bool val) { this->enabled = val; }
        bool Checkbox::Changed() const
        {
            return changed;
        }
    }
}
}