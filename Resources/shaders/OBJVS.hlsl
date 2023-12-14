#include "OBJShaderHeader.hlsli"

VSOutput main( float4 pos : POSITION,float3 normal:NORMAL, float2 uv:TEXCOORD)
{
	/*float3 light = normalize(float3(1, -1, 1));
	
	float3 lightcolor = float3(1, 1, 1);*/

	float4 wnormal = normalize(mul(world, float4(normal, 0)));
	float4 wpos = mul(world, pos);

	

	VSOutput output;//ピクセルシェーダーに渡す値
	output.svpos = mul(mul(viewProj,world), pos);
	/*output.color.rgb = (ambient + diffuse + specular) * lightColor;
	output.color.a = m_alpha;*/
	output.worldpos = wpos;
	output.normal = wnormal.xyz;
    output.uv = uv;
	return output;
}