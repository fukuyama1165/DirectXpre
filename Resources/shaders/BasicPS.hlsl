#include "Basic.hlsli"

Texture2D<float4> tex : register(t0);//�O�ԃX���b�g�̐ݒ肳�ꂽ�e�N�X�`��
SamplerState smp :register(s0);//�O�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[


float4 main(VSOutput input) : SV_TARGET
{
	float4 texcolor = float4(tex.Sample(smp,input.uv));

	float3 light = normalize(float3(1,-1,1));
	float diffuse = saturate(dot(-light, input.normal));
	float brightness = diffuse + 0.3f;
	return float4(texcolor.rgb * brightness, texcolor.a) * color;
}