#include <game/deltatime.h>

float GetMainDeltaTime()
{
    if (MainLoopDeltaTime == nullptr) MainLoopDeltaTime = new GEC::DeltaTime();
    return MainLoopDeltaTime->Get();
}
void UpdateMainDeltaTime()
{
    if (MainLoopDeltaTime == nullptr) MainLoopDeltaTime = new GEC::DeltaTime();
    MainLoopDeltaTime->Update();
}