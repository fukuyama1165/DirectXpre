#include "Basic.hlsli"


float4 main( float4 pos : POSITION ) : SV_POSITION
{
	return mul(mul(world,viewProj),pos);
}