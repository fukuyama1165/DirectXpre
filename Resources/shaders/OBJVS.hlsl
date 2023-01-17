#include "OBJShaderHeader.hlsli"

VSOutput main( float4 pos : POSITION,float3 normal:NORMAL, float2 uv:TEXCOORD)
{
	/*float3 light = normalize(float3(1, -1, 1));
	
	float3 lightcolor = float3(1, 1, 1);*/

	float4 wnormal = normalize(mul(world, float4(normal, 0)));
	float4 wpos = mul(world, pos);

	float3 ambient = m_ambient;

	float3 diffuse = dot(lightV, wnormal.xyz) * m_diffuse;

	//const float3 eye = float3(0, 0, -20);

	const float shininess = 4.0f;

	//float3 eyedir = normalize(eye - pos.xyz);

	float3 eyedir = normalize(cameraPos - wpos.xyz);

	//float3 reflect = normalize(-light + 2 * dot(light, normal) * normal);
	float3 reflect = normalize(-lightV + 2 * dot(lightV, wnormal.xyz) * wnormal.xyz);

	float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;

	VSOutput output;//ピクセルシェーダーに渡す値
	output.svpos = mul(mul(viewProj,world), pos);
	output.color.rgb = (ambient + diffuse + specular) * lightColor;
	output.color.a = m_alpha;
	output.uv = uv;
	return output;
}