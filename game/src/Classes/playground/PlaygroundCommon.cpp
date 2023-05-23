#include "playground/PlaygroundCommon.h"

NS_CC_BEGIN

namespace playground_common
{
    float Get360Angle(const Vec2& _v1, const Vec2& _v2)
    {
        float result = GetShortestAngle(_v1, _v2);
        if(result < 0.0f)
        {
            result = 360.0f + result;
        }
        
        return result;
    }

    float GetShortestAngle(const Vec2& _v1, const Vec2& _v2)
    {
        // -1 cuz result will be in ccw order but cc2dx use cw order
        return MATH_RAD_TO_DEG(-_v1.getAngle(_v2));
    }

    Vec2 RotateVector(const Vec2& _v, float _angle360)
    {
        // -1 cuz result will be in ccw order but cc2dx use cw order
        
        const float s = sin(MATH_DEG_TO_RAD(-_angle360));
        const float c = cos(MATH_DEG_TO_RAD(-_angle360));
        
        Vec2 result;
        result.x = _v.x * c - _v.y * s;
        result.y = _v.x * s + _v.y * c;
        
        return result;
    }
}

NS_CC_END
