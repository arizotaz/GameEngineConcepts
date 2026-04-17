// #############################################################################
// # box_collider.h
// #############################################################################
// # Written by Colton Staiduhar
// # Date Created:       03/13/2022
// # Date Ported:        03/10/2026
// # Last Modification:  03/10/2026
// #############################################################################
// # Box Collider 2D is just a means of detecting if two Rectangle interact
// # this does not support rotation
// #############################################################################
// # Ported from the Arizotaz Java Lilac Engine
// #############################################################################

#ifndef GEC_ENGINE_BOX_COLLIDER_H
#define GEC_ENGINE_BOX_COLLIDER_H 1

namespace GEC {
namespace Physics {
    class BoxCollider2D {
    public:
        BoxCollider2D(float x, float y, float width, float height)
        {
            this->SetPosition(x, y);
            this->SetSize(width, height);
        }

        void SetPosition(float x, float y)
        {
            this->x = x;
            this->y = y;
        }

        void SetSize(float width, float height)
        {
            this->width = width;
            this->height = height;
        }

        float X() const
        {
            return this->x;
        }

        float Y() const
        {
            return this->y;
        }

        float W() const
        {
            return this->width;
        }

        float H() const
        {
            return this->height;
        }

        bool IsColliding(BoxCollider2D collider) const
        {
            bool val = this->x - this->width / 2.0F < collider.x + collider.width / 2.0F && this->x + this->width / 2.0F > collider.x - collider.width / 2.0F && this->y - this->height / 2.0F < collider.y + collider.height / 2.0F && this->y + this->height / 2.0F > collider.y - collider.height / 2.0F;
            return val;
        }

    private:
        float x;
        float y;
        float width;
        float height;
    };
}
}

#endif