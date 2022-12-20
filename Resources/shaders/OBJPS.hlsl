#include "OBJShaderHeader.hlsli"

Texture2D<float4> tex : register(t0);//０番スロットの設定されたテクスチャ
SamplerState smp :register(s0);//０番スロットに設定されたサンプラー


float4 main(VSOutput input) : SV_TARGET
{

	
	float4 texcolor = tex.Sample(smp, input.uv);

	return input.color*texclor;
}