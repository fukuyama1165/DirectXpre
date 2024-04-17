#pragma once
#include "Vector3.h"
#include "Vector2.h"
#include <sstream>
struct ParticleData
{
    float ActiveTime;
    std::string Name = "";
    float actionTime;
    float ct;
    float liveTime;
    Vector2 randRengeX;
    Vector2 randRengeY;
    Vector2 randRengeZ;
    Vector3 startScale;
    Vector3 endScale;
};