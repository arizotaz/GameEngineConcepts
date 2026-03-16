#include <engine/ui/element.h>
#include <math.h>

namespace GEC {
namespace UI {

    Element::Element()
    {
    }

    void Element::Set(float x, float y, float width, float height)
    {
        this->x = x;
        this->y = y;
        this->width = width;
        this->height = height;
    }

    ElementRenderer::ElementRenderer()
    {
    }
    void ElementRenderer::ClearCycle()
    {
        this->list.clear();
    }

    void ElementRenderer::AddElement(Element* elm, int layer)
    {
        elm->elr = this;

        bool hasLayer = list.find(layer) != list.end();
        if (!hasLayer)
            list.insert({ layer, std::vector<Element*>() });

        list[layer].push_back(elm);
    }

    void ElementRenderer::Update()
    {
        if (changed > 0) --changed;
        for (auto iter = list.rbegin(); iter != list.rend(); ++iter) {
            std::vector<Element*> elements = iter->second;
            for (Element* element : elements) {
                if (element)
                    element->Update();
            }
        }
    }
    void ElementRenderer::Interact()
    {
        for (auto iter = list.rbegin(); iter != list.rend(); ++iter) {
            std::vector<Element*> elements = iter->second;
            for (int i = elements.size() - 1; i >= 0; --i) {

                Element* element = elements[i];

                if (element) {
                    element->Interact();
                    objectIDList += "-" + element->ObejctIdentifier();
                    if (element->ShouldUpdateScreen()) {
                        objectIDList += "_FRUP" + std::to_string(round(random() * 10000));
                    }
                    objectIDList += ";";
                    element->DoNotUpdateScreen();

                }
            }
        }

        if (this->objectIDList != this->lastObjectIDList) {
            changed = 3;
        }
    }
    void ElementRenderer::Render()
    {
        for (auto iter = list.rbegin(); iter != list.rend(); ++iter) {
            std::vector<Element*> elements = iter->second;
            for (Element* element : elements) {
                if (element)
                    element->Render();
            }
        }
    }

    bool ElementRenderer::HasChanged() const {
        return changed > 0;
    }

    void ElementRenderer::MarkChange() {
        changed = 1;
    }

}
}