#include "Global.h"

float fAngle(float speed)
{
    return (timeGetTime() % (int)speed) * ((2.0f * D3DX_PI) / speed);
}