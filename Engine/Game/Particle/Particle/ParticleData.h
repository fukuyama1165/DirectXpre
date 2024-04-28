#pragma once
#include "Vector3.h"
#include "Vector2.h"
#include <sstream>
struct ParticleData
{
    float ActiveTime=-1;
    std::string Name = "";
    float actionTime=10;
    float ct=10;
    float liveTime=10;
    Vector2 randRengeX = {-1,1};
    Vector2 randRengeY = { -1,1 };
    Vector2 randRengeZ = { -1,1 };
    Vector3 startScale = { 1,1,1};
    Vector3 endScale = { 1,1,1 };
};