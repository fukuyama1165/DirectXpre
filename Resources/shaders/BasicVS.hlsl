#include "Basic.hlsli"

VSOutput main( float4 pos : POSITION,float3 normal:NORMAL, float2 uv:TEXCOORD)
{
	//VSOutput output;//ピクセルシェーダーに渡す値
	//output.svpos = mul(mat,pos);
	//output.normal = normal;
	//output.uv = uv;
	//return output;

	float3 light = normalize(float3(1, -1, 1));

	float3 lightcolor = float3(1, 1, 1);

	float3 ambient = 1;

	float3 diffuse = dot(-light, normal);

	const float3 eye = float3(0, 0, -20);

	const float shininess = 4.0f;

	float3 eyedir = normalize(eye - pos.xyz);

	float3 reflect = normalize(-light + 2 * dot(light, normal) * normal);

	float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * 1;

	VSOutput output;//ピクセルシェーダーに渡す値
	output.svpos = mul(mul(world, viewProj), pos);
	output.color.rgb = (ambient + diffuse + specular) * lightcolor;
	output.color.a = 1;
	output.uv = uv;
	return output;
}