cbuffer ConstBufferDataMaterial:register(b0)
{
	float4 color;//色(RGBA)
}

cbuffer ConstBufferDataMaterial2 : register(b1)//定数バッファの番号をbの後ろにつける
{
	float4 posm;//位置(XYZ)
}

//頂点シェーダーの出力構造体
//(頂点シェーダーからピクセルシェーダーへのやり取りに使用する)
struct VSOutput
{
	//システム用頂点座標
	float4 svpos:SV_POSITION;
	//uv値
	float2 uv:TEXCOORD;
};