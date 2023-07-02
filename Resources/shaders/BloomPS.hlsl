#include "Bloom.hlsli"

Texture2D<float4> tex : register(t0);//０番スロットの設定されたテクスチャ
Texture2D<float4> tex1 : register(t1);//1番スロットの設定されたテクスチャ
SamplerState smp :register(s0);//０番スロットに設定されたサンプラー


float Gaussian(float2 drawUV, float2 pickUV, float sigma)
{
    float d = distance(drawUV, pickUV);
    return exp(-(d * d) / (2 * sigma * sigma));

}

float4 GaussianBlur(VSOutput input,float _Sigma = 0.005, float _StepWidth = 0.001)
{

    float totalWeight = 0;
    float4 col = float4(1, 1, 1, 1);
    [loop]
    for (float py = -_Sigma * 2; py <= _Sigma * 2; py += _StepWidth)
    {
        [unroll]
        for (float px = -_Sigma * 2; px <= _Sigma * 2; px += _StepWidth)
        {
            float2 pickUV = input.uv + float2(px, py);
            if ((pickUV.x < 0 || pickUV.x > 1) || (pickUV.y < 0 || pickUV.y > 1))continue;

            float weight = Gaussian(input.uv, pickUV, _Sigma);
            float4 texcolor = tex.Sample(smp, pickUV);
            col += texcolor * weight;
            totalWeight += weight;

        }
    }


    col.rgb = col.rgb / totalWeight;
    col.a = 1;

    return col;
}


float4 main(VSOutput input) : SV_TARGET
{
    //グレースケール
    float4 col = tex.Sample(smp, input.uv);
    float grayScale = col.r * 0.299 + col.g * 0.587 + col.b * 0.114;
    float extract = smoothstep(0.6, 0.9, grayScale);

    float totalWeight = 0;
    float _Sigma = 0.005;//固定だけどUVで大きさを変えると画面の外側だけに掛けることができるらしい
    float _StepWidth = 0.001;
    float4 colTex = float4(1, 1, 1, 1);

    [loop]
    for (float py = -_Sigma * 2; py <= _Sigma * 2; py += _StepWidth)
    {
        [unroll]
        for (float px = -_Sigma * 2; px <= _Sigma * 2; px += _StepWidth)
        {
            float2 pickUV = input.uv + float2(px, py);
            if ((pickUV.x < 0 || pickUV.x > 1) || (pickUV.y < 0 || pickUV.y > 1))continue;

            float weight = Gaussian(input.uv, pickUV, _Sigma);
            float4 texcolor = tex.Sample(smp, pickUV);
            colTex += texcolor * weight;
            totalWeight += weight;

        }
    }


    colTex.rgb = col.rgb / totalWeight;
    colTex.a = 1;
    
    float4 highLumi = (col * extract) + colTex;
    return col + highLumi;
}
