// #############################################################################
// # element.h
// #############################################################################
// # Written by Colton Staiduhar
// # Date Created:       03/12/2022
// # Last Modification:  03/18/2026
// #############################################################################
// # This defines the main interface and usage of multiple critial managers for
// # element renderering, interactions, and management.
// #############################################################################

#ifndef ENGINE_UI_ELEMENT_H
#define ENGINE_UI_ELEMENT_H 1

#include <engine/camera.h>
#include <engine/input.h>
#include <engine/structs.h>
#include <iostream>
#include <map>
#include <string>
#include <vector>

namespace GEC {
namespace UI {

    // Define prototypes because these all rely on eachother
    class Element;
    class ElementRenderer;

    /**
     * A registry of default elements the engine can recognize
     * Also holds the flag responsible for stopping click-through events
     */
    class ElementRegistry {
    public:
        /** Gets the singleton instance of the ElementRegistry object */
        static ElementRegistry& GetInstance()
        {
            static ElementRegistry elmr;
            return elmr;
        }

        /** Reset the click-through flag for elements */
        void Reset()
        {
            stopInteract = false;
        }

        /** Returns the current state of the click-through flag
         * Supposed to be used for elements to know when to stop accepting inputs
         */
        bool ShouldStopInteract()
        {
            return stopInteract;
        }

        /** Sets the interact flag so that elements stop accepting inputs
         * Should be called when an element is interacted with to stop other elements from continuing to call interaction
         */
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

    /** Element Structure, this is the declaration of the base element */
    class Element {
    public:
        /** Default Constructor */
        Element();

        /** Deconsturctor - depends on derived element */
        virtual ~Element() { };

        /** Sets the position and size of the element */
        void Set(float x, float y, float width, float height);

        /** Update loop of the element */
        virtual void Update() = 0;

        /** Interact event of the element
         * This code is called every frame, but is used to capture interact events
         */
        virtual void Interact() = 0;

        /** Render the element */
        virtual void Render() = 0;

        /** @returns the x position of the element */
        float X() const { return x; }
        /** @returns the y position of the element */
        float Y() const { return y; }
        /** @returns the width of the element */
        float Width() const { return width; }
        /** @returns the height of the element */
        float Height() const { return height; }
        /** The element should tell the window to force redraw of the screen */
        bool ShouldUpdateScreen() const { return shouldUpdateScreen; }
        /** This element does not need to force the windows to redraw */
        void DoNotUpdateScreen() { shouldUpdateScreen = false; }
        
        /** Returns the object idenifier of the element (used to find screen changes) */
        std::string ObejctIdentifier() const { return objectIdentifier; }

        /** A basic identifier to be used with ObjectIdentifier
         * @returns an object identifier that looks something like "x-y-width-height"
         */
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

        bool MouseInBounds(GEC::Input::Mouse* mouse, float x, float y, float width, float height)
        {
            GEC::Vector2<float, float> screen = Camera::GetInstance().ViewPort();
            interactX = x;
            interactY = y;

            GEC::Vector2<float, float> pos = mouse->Position();
            pos.Move(-screen / 2);
            pos = pos * GEC::Vector2<float, float>(1, -1);

            bool value = pos.First() < interactX + width / 2 && pos.First() > interactX - width / 2 && pos.Second() > interactY - height / 2 && pos.Second() < interactY + height / 2;
            return value;
        }

    private:
        long WindowID;
        bool shouldUpdateScreen;
        ElementRenderer* elr;
    };

    /** Main Element Renderer - responsible for rendering elements */
    class ElementRenderer {
    public:
    
        /** Default constuctor */
        ElementRenderer();

        /** Clears the list of elements in the element renderer, effectively resetting the screen */
        void ClearCycle();

        /** Adds an element to the screen on a given layer
         * @param elm - the element to add
         * @param layer - the layer to add it to
         */
        void AddElement(Element*, int layer);

        /**
         * Update loop, updates all elements in the ElementRenderer
         */
        void Update();

        /**
         * Interact loop of the element renderer, this calls each elements interact function.
         * Elements are responsible for ignoring interaction overlap
         */
        void Interact();

        /**
         * Draws each element in the Element Renderer
         */
        void Render();

        /**
         * @returns if any elements have changed states, looks for changes in elements ObjcetIdentifiers
         */
        bool HasChanged() const;

        /**
         * Force the HasChanged() function to return true for a few frames
         */
        void MarkChange();

    private:
        std::map<int, std::vector<Element*>> list;
        std::string objectIDList, lastObjectIDList;
        int changed;
    };

    /**
     * An element template that inplements the ability to click, hover, and hold the device with a mouse cursor
     */
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
                if (MouseInBounds(mouse, x, y, width, height)) {
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

        /**
         * @returns true for as long as the user is holding down the left mouse button ontop of this object
         */
        bool IsDown()
        {
            return down;
        }
        /**
         * @returns true for as long as the user has the cursor ontop of this object
         */
        bool IsHovering()
        {
            return this->hover;
        }
        /**
         * @returns true when the user clicks the object
         */
        bool Clicked()
        {
            return this->clicked;
        }

    protected:
        bool down = false, hover = false, clicked = false;
        bool last_hover = false;

        bool changeCursor = true;
        std::string changeToCursor = "pointer";

        GEC::Input::Mouse* mouse;
    };
}
}

#endif