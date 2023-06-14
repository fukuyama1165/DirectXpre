#include "Phong_toon_rim.hlsli"

Texture2D<float4> tex : register(t0);//０番スロットの設定されたテクスチャ
SamplerState smp :register(s0);//０番スロットに設定されたサンプラー


float4 main(VSOutput input) : SV_TARGET
{

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

	

	//拡散反射光
	//float3 diffuse = dotlightnormal * m_diffuse;

	//鏡面反射光
	//float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;

	//shadecolor.rgb = (ambient + diffuse + specular) * lightColor;
	//shadecolor.a = m_alpha;

	float4 shadecolor = {0,0,0,1};

	for (uint i = 0; i < SDIR_LIGHT_NUM; i++)
	{

		if (dirLights[i].active)
		{

			//環境反射光
			float3 ambient = m_ambient * 0.3 * ambientColor;

			//ライトに向かうベクトルと法線の内積
			float3 dotlightnormal = saturate(dot(dirLights[i].lightV, input.normal));

			
			//法線とカメラへのベクトルの内積
			half rim = 1.0 - saturate(dot(input.normal, eyedir));

			
			//たぶんトゥーンよう？
			dotlightnormal = smoothstep(0.1, 0.15, dotlightnormal);

			//反射光ベクトル
			float3 reflect = normalize(-dirLights[i].lightV + 2 * input.normal * smoothstep(0.9, 0.95, dot(input.normal, dirLights[i].lightV)));

			//拡散反射光
			float3 diffuse = dotlightnormal * m_diffuse * dirLights[i].lightColor;

			//鏡面反射光
			float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * dirLights[i].lightColor;

			//リムライトの色
			float4 rimcolor = { 0,1,0,0 };

			//リムライトの強さ
			float3 rimfi = rimcolor * pow(rim, 3);

			//まとめる
			float3 ads = (ambient + diffuse + specular) + rimfi;

			//全て加算する
			shadecolor.rgb += (ads)*dirLights[i].lightColor;

		}

	}


	
	

	

	
	////float3 reflect = normalize(-light + 2 * dot(light, normal) * normal);
	//float3 reflect = normalize(-lightV + 2 * dot(lightV, wnormal.xyz) * wnormal.xyz);




	return shadecolor * texcolor;
}