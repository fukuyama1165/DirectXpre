cbuffer cbuff0:register(b0)
{
	//matrix mat;//3D変換行列
	matrix viewProj;//ビュープロジェクション行列
	matrix world;//ワールド行列
	float3 cameraPos;//カメラ座標(ワールド座標)

	float4 Icolor;

}

cbuffer cbuff1 : register(b1)//定数バッファの番号をbの後ろにつける
{
	float3 m_ambient:packoffset(c0);
	float3 m_diffuse:packoffset(c1);
	float3 m_specular:packoffset(c2);
	float m_alpha : packoffset(c2.w);
	float2 m_tile : packoffset(c3);
}

//cbuffer cbuff2 : register(b2)
//{
//
//	float3 lightV;//ライトへの方向の単位ベクトル
//	float3 lightColor;//ライトの色(RGB)
//
//}

static const uint DIR_LIGHT_NUM = 3;
//天光源の数
static const uint POINTLIGHT_NUM = 100;
//スポットライトの数
static const uint SPOTLIGHT_NUM = 3;

struct DirLight
{

	float4 lightV;
	float4 lightColor;
	float4 active;

};

struct PointLight
{

	float4 lightPos;
	float4 lightColor;
	float4 lightAtten;
	float4 active;

};

struct SpotLight
{
	float4 active;
	float4 lightV;
	float4 lightPos;
	float4 lightColor;
	float4 lightAtten;
	float4 lightFactorAngle;
	

};

cbuffer cbuff2 : register(b2)
{

	float3 ambientColor;
	DirLight dirLights[DIR_LIGHT_NUM];
	PointLight pointLights[POINTLIGHT_NUM];
	SpotLight spotLights[SPOTLIGHT_NUM];
}


//頂点シェーダーの出力構造体
//(頂点シェーダーからピクセルシェーダーへのやり取りに使用する)
struct VSOutput
{
	//システム用頂点座標
	float4 svpos:SV_POSITION;
	//法線ベクトル
	float3 normal:NORMAL;
	float4 worldpos:POSITION;
	//float4 color:COLOR;
	//uv値
	float2 uv:TEXCOORD;
};

//マルチレンダリングの為の構造体
struct PSOutput
{
	float4 target0:SV_TARGET0;
	float4 target1:SV_TARGET1;
};
