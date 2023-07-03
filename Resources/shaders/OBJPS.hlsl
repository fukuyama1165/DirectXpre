#include "OBJShaderHeader.hlsli"

Texture2D<float4> tex : register(t0);//０番スロットの設定されたテクスチャ
SamplerState smp :register(s0);//０番スロットに設定されたサンプラー

float4 averagBlur(VSOutput input)
{
	float shiftWidth = 0.005;
	float shiftNum = 3;

	float4 col = float4(0, 0, 0, 0);
	float num = 0;

	[loop]
	for (float py = -shiftNum / 2; py <= shiftNum / 2; py++)
	{
		for (float px = -shiftNum / 2; px <= shiftNum / 2; px++)
		{
			col += tex.Sample(smp, input.uv + float2(px, py) * shiftWidth);
			num++;
		}
	}

	col = col / num;
	col.w = 1;
	return col;

}

float Gaussian(float2 drawUV, float2 pickUV, float sigma)
{
	float d = distance(drawUV, pickUV);
	return exp(-(d * d) / (2 * sigma * sigma));

}


PSOutput main(VSOutput input)
{

	PSOutput output;

	float4 texcolor = tex.Sample(smp, input.uv);

	////シェーディングによる色
	//float4 shadecolor;

	//光沢度
	const float shininess = 4.0f;

	//頂点から視点への方向ベクトル
	float3 eyedir = normalize(cameraPos - input.worldpos.xyz);

	//ライトに向かうベクトルと法線の内積
	//float3 dotlightnormal = dot(lightV, input.normal);

	//反射光ベクトル
	//float3 reflect = normalize(-lightV + 2 * dotlightnormal * input.normal);

	//環境反射光
	float3 ambient = m_ambient;

	//拡散反射光
	//float3 diffuse = dotlightnormal * m_diffuse;

	//鏡面反射光
	//float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;

	//shadecolor.rgb = (ambient + diffuse + specular) * lightColor;
	//shadecolor.a = m_alpha;

	float4 shadecolor = float4(ambientColor * ambient, m_alpha);

	for (uint i = 0; i < DIR_LIGHT_NUM; i++)
	{

		if (dirLights[i].active)
		{

			//ライトに向かうベクトルと法線の内積
			float3 dotlightnormal = dot(dirLights[i].lightV, input.normal);

			//反射光ベクトル
			float3 reflect = normalize(-dirLights[i].lightV + 2 * dotlightnormal * input.normal);

			//拡散反射光
			float3 diffuse = dotlightnormal * m_diffuse;

			//鏡面反射光
			float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;

			//全て加算する
			shadecolor.rgb += (diffuse + specular) * dirLights[i].lightColor;

		}

	}

	for (uint j = 0; j < POINTLIGHT_NUM; j++)
	{

		if (pointLights[j].active)
		{

			//ライトへのベクトル
			float3 lightV = pointLights[j].lightPos - input.worldpos.xyz;

			//ベクトルの長さ
			float d = length(lightV);

			//正規化し、単位ベクトルにする
			lightV = normalize(lightV);

			//距離減衰係数
			float atten = 1.0f / (pointLights[j].lightAtten.x + pointLights[j].lightAtten.y * d + pointLights[j].lightAtten.z * d * d);
			
			//ライトに向かうベクトルと法線の内積
			float3 dotlightnormal = dot(lightV, input.normal);

			//反射光ベクトル
			float3 reflect = normalize(-lightV + 2 * dotlightnormal * input.normal);

			//拡散反射光
			float3 diffuse = dotlightnormal * m_diffuse;

			//鏡面反射光
			float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;

			//全て加算する
			shadecolor.rgb +=atten* (diffuse + specular) * pointLights[j].lightColor;

		}

	}

	for (uint n = 0; n < SPOTLIGHT_NUM; n++)
	{

		if (spotLights[n].active)
		{

			//ライトへのベクトル
			float3 lightV = spotLights[n].lightPos - input.worldpos.xyz;

			//ベクトルの長さ
			float d = length(lightV);

			//正規化し、単位ベクトルにする
			lightV = normalize(lightV);

			//距離減衰係数
			float atten = saturate(1.0f / (spotLights[n].lightAtten.x + spotLights[n].lightAtten.y * d + spotLights[n].lightAtten.z * d * d));

			//角度減衰
			float cos = dot(lightV, spotLights[n].lightV);

			//減衰開始角度から、減衰終了角度に掛けて減衰
			//減衰開始角度の内側は1倍減衰終了角度の外側は0倍の輝度
			float angleAtten = smoothstep(spotLights[n].lightFactorAngle.y, spotLights[n].lightFactorAngle.x, cos);

			//角度減衰を乗算
			atten *= angleAtten;

			//ライトに向かうベクトルと法線の内積
			float3 dotlightnormal = dot(lightV, input.normal);

			//反射光ベクトル
			float3 reflect = normalize(-lightV + 2 * dotlightnormal * input.normal);

			//拡散反射光
			float3 diffuse = dotlightnormal * m_diffuse;

			//鏡面反射光
			float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;

			//全て加算する
			shadecolor.rgb += atten * (diffuse + specular) * spotLights[n].lightColor;

		}

		//if (spotLights[0].active==0)
		//{

		//	//ライトへのベクトル
		//	float3 lightV = spotLights[0].lightPos - input.worldpos.xyz;

		//	//ベクトルの長さ
		//	float d = length(lightV);

		//	//正規化し、単位ベクトルにする
		//	lightV = normalize(lightV);

		//	//距離減衰係数
		//	float atten = saturate(1.0f / (spotLights[0].lightAtten.x + spotLights[0].lightAtten.y * d + spotLights[0].lightAtten.z * d * d));

		//	//角度減衰
		//	float cos = dot(lightV, spotLights[n].lightV);

		//	//減衰開始角度から、減衰終了角度に掛けて減衰
		//	//減衰開始角度の内側は1倍減衰終了角度の外側は0倍の輝度
		//	float angleAtten = smoothstep(spotLights[0].lightFactorAngle.y, spotLights[0].lightFactorAngle.x, cos);

		//	//角度減衰を乗算
		//	atten *= angleAtten;

		//	//ライトに向かうベクトルと法線の内積
		//	float3 dotlightnormal = dot(lightV, input.normal);

		//	//反射光ベクトル
		//	float3 reflect = normalize(-lightV + 2 * dotlightnormal * input.normal);

		//	//拡散反射光
		//	float3 diffuse = dotlightnormal * m_diffuse;

		//	//鏡面反射光
		//	float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;

		//	//全て加算する
		//	shadecolor.rgb += atten * (diffuse + specular) * spotLights[0].lightColor;

		//}

	}


	////float3 reflect = normalize(-light + 2 * dot(light, normal) * normal);
	//float3 reflect = normalize(-lightV + 2 * dot(lightV, wnormal.xyz) * wnormal.xyz);

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

	float4 highLumi = (col * extract) * colTex;

	
	output.target1 = averagBlur(input);
	output.target0 = col * shadecolor;

	return output;
}