#ifndef ENGINE_UI_ELEMENT_H
#define ENGINE_UI_ELEMENT_H 1

#include <engine/camera.h>
#include <engine/input.h>
#include <engine/structs.h>
#include <iostream>
#include <map>
#include <vector>
namespace GEC {
namespace UI {

    class Element;
    class ElementRenderer;

    class ElementRegistry {
    public:
        static ElementRegistry& GetInstance()
        {
            static ElementRegistry elmr;
            return elmr;
        }

        void Reset()
        {
            stopInteract = false;
        }
        bool ShouldStopInteract()
        {
            return stopInteract;
        }
        void StopInteract()
        {
            stopInteract = true;
        }

    private:
        ElementRegistry()
        {
        }
        ~ElementRegistry() { }

        // List of each window ID with a list of elements mapped with an ID
        std::map<long, std::map<std::string, Element*>> elementlists;

        // Stop Interaction when an item is clicked
        // Should be reset at the beginning of every input cycle
        bool stopInteract = false;
    };

    class Element {
    public:
        Element();

        virtual ~Element() { };
        void Set(float x, float y, float width, float height);

        virtual void Update() = 0;
        virtual void Interact() = 0;
        virtual void Render() = 0;

        float X() const { return x; }
        float Y() const { return y; }
        float Width() const { return width; }
        float Height() const { return height; }
        bool ShouldUpdateScreen() const { return shouldUpdateScreen; }
        void DoNotUpdateScreen() { shouldUpdateScreen = false; }
        std::string ObejctIdentifier() const { return objectIdentifier; }
        std::string BaseObjectID() const
        {
            return std::to_string(x) + "_" + std::to_string(y) + "_" + std::to_string(width) + "_" + std::to_string(height);
        }

        friend class ElementRenderer;

    protected:
        float x;
        float y;
        float width;
        float height;
        float interactX, interactY;
        std::string objectIdentifier;

    private:
        long WindowID;
        bool shouldUpdateScreen;
        ElementRenderer* elr;
    };

    class ElementRenderer {
    public:
        ElementRenderer();

        void ClearCycle();
        void AddElement(Element*, int layer);

        void Update();
        void Interact();
        void Render();
        bool HasChanged() const;
        void MarkChange();

    private:
        std::map<int, std::vector<Element*>> list;
        std::string objectIDList, lastObjectIDList;
        int changed;
    };

    class Clickable : public Element {
    public:
        Clickable()
        {
            mouse = &GEC::Input::Mouse::GetInstance();
        }
        ~Clickable() { }
        virtual void Update() = 0;
        virtual void Interact()
        {
            hover = false;
            down = false;
            clicked = false;
            if (!ElementRegistry::GetInstance().ShouldStopInteract()) {
                if (MouseInBounds(x, y, width, height)) {
                    hover = true;
                    if (changeCursor) {
                        mouse->SetCursor(changeToCursor);
                    }
                    if (mouse->LeftPressed()) {
                        clicked = true;
                    }
                    if (mouse->LeftDown()) {
                        down = true;
                    }
                    ElementRegistry::GetInstance().StopInteract();
                }
            }
            this->objectIdentifier = this->BaseObjectID() + "_c" + std::to_string(clicked) + "_h" + std::to_string(hover) + "_d" + std::to_string(down);

            if (changeCursor) {
                if (!last_hover && hover) {
                    mouse->SetCursor("pointer");
                }
                if (last_hover && !hover) {
                    mouse->SetCursor("cursor");
                }
                last_hover = hover;
            }
        }
        virtual void Render() = 0;

        bool IsDown()
        {
            return down;
        }
        bool IsHovering()
        {
            return this->hover;
        }
        bool Clicked()
        {
            return this->clicked;
        }

    protected:
        bool MouseInBounds(float x, float y, float width, float height)
        {
            GEC::Vector2<float, float> screen = Camera::GetInstance().ViewPort();
            interactX = x;
            interactY = y;

            GEC::Vector2<float, float> pos = mouse->Position();
            pos.Move(-screen/2);
            pos = pos * GEC::Vector2<float,float>(1,-1);
            std::cout << pos.First() << "-" << pos.Second();

            bool value = pos.First() < interactX + width / 2 && pos.First() > interactX - width / 2 && pos.Second() > interactY - height / 2 && pos.Second() < interactY + height / 2;
            return value;
        }

        bool down = false, hover = false, clicked = false;
        bool last_hover = false;

        bool changeCursor = true;
        std::string changeToCursor = "pointer";

        GEC::Input::Mouse* mouse;
    };
}
}

#endif