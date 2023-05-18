#pragma once

#include <Config.h>

NS_CC_BEGIN

namespace common
{
    //#TODO: Return shortest angle between two vectors. CW - positive, CCW - negative [0-180]
    extern float GetShortestAngle(const Vec2& _v1, const Vec2& _v2);

    //#TODO: Return full 360 angle (CW) between two vectors
    extern float Get360Angle(const Vec2& _v1, const Vec2& _v2);

    //#TODO: Rotate vector by specified angle
    extern Vec2 RotateVector(const Vec2& _v, float _angle360);
}

NS_CC_END
