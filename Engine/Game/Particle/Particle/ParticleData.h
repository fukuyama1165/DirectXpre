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
    Vector3 endScale;
    float liveTime;
    Vector2 randRengeX;
    Vector3 randRengeY;
    Vector3 randRengeZ;
    Vector3 startScale;
};