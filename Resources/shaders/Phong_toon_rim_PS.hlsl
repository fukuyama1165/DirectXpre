#include "Phong_toon_rim.hlsli"

Texture2D<float4> tex : register(t0);//�O�ԃX���b�g�̐ݒ肳�ꂽ�e�N�X�`��
SamplerState smp :register(s0);//�O�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[


float4 main(VSOutput input) : SV_TARGET
{

	float4 texcolor = tex.Sample(smp, input.uv);

	////�V�F�[�f�B���O�ɂ��F
	//float4 shadecolor;

	//����x
	const float shininess = 4.0f;

	//���_���王�_�ւ̕����x�N�g��
	float3 eyedir = normalize(cameraPos - input.worldpos.xyz);

	//���C�g�Ɍ������x�N�g���Ɩ@���̓���
	//float3 dotlightnormal = dot(lightV, input.normal);

	//���ˌ��x�N�g��
	//float3 reflect = normalize(-lightV + 2 * dotlightnormal * input.normal);

	

	//�g�U���ˌ�
	//float3 diffuse = dotlightnormal * m_diffuse;

	//���ʔ��ˌ�
	//float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;

	//shadecolor.rgb = (ambient + diffuse + specular) * lightColor;
	//shadecolor.a = m_alpha;

	float4 shadecolor = {0,0,0,1};

	for (uint i = 0; i < SDIR_LIGHT_NUM; i++)
	{

		if (dirLights[i].active)
		{

			//�����ˌ�
			float3 ambient = m_ambient * 0.3 * ambientColor;

			//���C�g�Ɍ������x�N�g���Ɩ@���̓���
			float3 dotlightnormal = saturate(dot(dirLights[i].lightV, input.normal));

			
			//�@���ƃJ�����ւ̃x�N�g���̓���
			half rim = 1.0 - saturate(dot(input.normal, eyedir));

			
			//���Ԃ�g�D�[���悤�H
			dotlightnormal = smoothstep(0.1, 0.15, dotlightnormal);

			//���ˌ��x�N�g��
			float3 reflect = normalize(-dirLights[i].lightV + 2 * input.normal * smoothstep(0.9, 0.95, dot(input.normal, dirLights[i].lightV)));

			//�g�U���ˌ�
			float3 diffuse = dotlightnormal * m_diffuse * dirLights[i].lightColor;

			//���ʔ��ˌ�
			float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * dirLights[i].lightColor;

			//�������C�g�̐F
			float4 rimcolor = { 0,1,0,0 };

			//�������C�g�̋���
			float3 rimfi = rimcolor * pow(rim, 3);

			//�܂Ƃ߂�
			float3 ads = (ambient + diffuse + specular) + rimfi;

			//�S�ĉ��Z����
			shadecolor.rgb += (ads)*dirLights[i].lightColor;

		}

	}


	
	

	

	
	////float3 reflect = normalize(-light + 2 * dot(light, normal) * normal);
	//float3 reflect = normalize(-lightV + 2 * dot(lightV, wnormal.xyz) * wnormal.xyz);




	return shadecolor * texcolor;
}