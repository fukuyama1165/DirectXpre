cbuffer cbuff0:register(b0)
{
	//matrix mat;//3D変換行列
	matrix viewProj;//ビュープロジェクション行列
	matrix world;//ワールド行列
	float3 cameraPos;//カメラ座標(ワールド座標)

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

