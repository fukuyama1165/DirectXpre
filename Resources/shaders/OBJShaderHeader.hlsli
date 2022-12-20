cbuffer cbuff0:register(b0)
{
	matrix mat;//3D変換行列
}

cbuffer cbuff1 : register(b1)//定数バッファの番号をbの後ろにつける
{
	float3 m_ambient:packoffset(c0);
	float3 m_diffuse:packoffset(c1);
	float3 m_specular:packoffset(c2);
	float m_alpha : packoffset(c2.w);
}


//頂点シェーダーの出力構造体
//(頂点シェーダーからピクセルシェーダーへのやり取りに使用する)
struct VSOutput
{
	//システム用頂点座標
	float4 svpos:SV_POSITION;
	//法線ベクトル
	//float3 normal:NORMAL;
	float4 color:COLOR;
	//uv値
	float2 uv:TEXCOORD;
};