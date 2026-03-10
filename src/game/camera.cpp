#include <game/camera.h>

void Camera::Set(float x, float y)
{
    pos.Set(x,y);
}
void Camera::Set(GEC::Vector2<float,float> vec) {
    Set(vec.First(),vec.Second());
}

void Camera::Move(float x, float y)
{
    pos.Move(x,y);
}
float Camera::X()
{
    return pos.First();
}
float Camera::Y()
{
    return pos.Second();
}

void Camera::Scale(float value)
{
    scale = value;
}
float Camera::Scale() const
{
    return scale;
}

GEC::Vector3<float, float, float> Camera::Position()
{
    return GEC::Vector3<float, float, float>(pos.First(), pos.Second(), 0);
}