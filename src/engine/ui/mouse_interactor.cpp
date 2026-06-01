#include <engine/ui/elements.h>

namespace GEC {
namespace UI {
    namespace Elements {
        MouseInteractor::MouseInteractor()
        {
            mouse = &GEC::Input::Mouse::GetInstance();
        }
        void MouseInteractor::Update() { }
        void MouseInteractor::Interact()
        {
            if (!ElementRegistry::GetInstance().ShouldStopInteract())
                if (MouseInBounds(mouse, x, y, width, height))
                    ElementRegistry::GetInstance().StopInteract();
        }
        void MouseInteractor::Render() { }

    }
}
}