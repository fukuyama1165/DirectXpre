#include "GaussianBlur.hlsli"

Texture2D<float4> tex : register(t0);//�O�ԃX���b�g�̐ݒ肳�ꂽ�e�N�X�`��
SamplerState smp :register(s0);//�O�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

float Gaussian(float2 drawUV, float2 pickUV, float sigma)
{
    float d = distance(drawUV, pickUV);
    return exp(-(d * d) / (2 * sigma * sigma));

}

float4 main(VSOutput input) : SV_TARGET
{

    float totalWeight = 0;
    float _Sigma = 0.005;//�Œ肾����UV�ő傫����ς���Ɖ�ʂ̊O�������Ɋ|���邱�Ƃ��ł���炵��
    float _StepWidth = 0.001;
    float4 col = float4(0,0,0,0);
    float4 texcolor = tex.Sample(smp, input.uv);

    for (float py = -_Sigma * 2; py <= _Sigma * 2; py += _StepWidth)
    {
        for (float px = -_Sigma * 2; px <= _Sigma * 2; px += _StepWidth)
        {
            float2 pickUV = input.uv + float2(px, py);
            float weight = Gaussian(input.uv, pickUV, _Sigma);
            col += float4(texcolor.xy*pickUV,texcolor.z,texcolor.w) * weight;
            totalWeight += weight;

        }
    }


    col.rgb = col.rgb / totalWeight;
    col.a = 1;

    return col;
}