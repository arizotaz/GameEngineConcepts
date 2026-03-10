#include <game/camera.h>

void Camera::SetPos(float x, float y)
{
    cameraView.First().Set(x, y);
}
void Camera::SetPos(GEC::Vector2<float, float> vec)
{
    SetPos(vec.First(), vec.Second());
}

void Camera::Move(float x, float y)
{
    cameraView.First().Move(x, y);
}
float Camera::X()
{
    return cameraView.First().First();
}
float Camera::Y()
{
    return cameraView.First().Second();
}

void Camera::SetScale(float value)
{
    scale = value;
}
float Camera::GetScale() const
{
    return scale;
}

void Camera::SetScreen(float x, float y)
{
    cameraView.Second().Set(x, y);
}
void Camera::SetScreen(GEC::Vector2<float, float> vec)
{
    SetScreen(vec.First(), vec.Second());
}

void Camera::SetPosAndSize(float x, float y, float w, float h)
{
    cameraView.X() = x;
    cameraView.Y() = y;
    cameraView.W() = w;
    cameraView.H() = h;
}
void Camera::SetPosAndSize(GEC::Vector2<float, float> pos, GEC::Vector2<float, float> canvas)
{
    SetPosAndSize(pos.First(), pos.Second(), canvas.First(), canvas.Second());
}
void Camera::SetPosAndSize(GEC::Rect<float, float, float, float> rect) {
    SetPosAndSize(rect.First(),rect.Second());
}

GEC::Vector2<float, float> Camera::ViewPort()
    {
        return GEC::Vector2(cameraView.Second());
    }


GEC::Vector3<float, float, float> Camera::Position()
{
    return GEC::Vector3<float, float, float>(cameraView.First().First(), cameraView.First().Second(), 0);
}