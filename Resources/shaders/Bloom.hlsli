cbuffer ConstBufferDataMaterial : register(b0)
{
	float4 color;//いろ
	float4 sigma;//ブルームを掛けるときのブラー用の値(xのみ)
	float4 grayScaleStep;//ブルーム用のグレースケールのsmoothstep用の大きさ(x,yのみ)
};

cbuffer ConstBufferDataTransform : register(b1)
{
	matrix mat;//3D変換行列
};

//頂点シェーダーの出力構造体
//(頂点シェーダーからピクセルシェーダーへのやり取りに使用する)
struct VSOutput
{
	//システム用頂点座標
	float4 svpos:SV_POSITION;
	//uv値
	float2 uv:TEXCOORD;
};