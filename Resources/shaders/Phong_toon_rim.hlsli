cbuffer cbuff0:register(b0)
{
	//matrix mat;//3D�ϊ��s��
	matrix viewProj;//�r���[�v���W�F�N�V�����s��
	matrix world;//���[���h�s��
	float3 cameraPos;//�J�������W(���[���h���W)

}

cbuffer cbuff1 : register(b1)//�萔�o�b�t�@�̔ԍ���b�̌��ɂ���
{
	float3 m_ambient:packoffset(c0);
	float3 m_diffuse:packoffset(c1);
	float3 m_specular:packoffset(c2);
	float m_alpha : packoffset(c2.w);
}

//cbuffer cbuff2 : register(b2)
//{
//
//	float3 lightV;//���C�g�ւ̕����̒P�ʃx�N�g��
//	float3 lightColor;//���C�g�̐F(RGB)
//
//}

static const uint DIR_LIGHT_NUM = 3;

struct DirLight
{

	float3 lightV;
	float3 lightColor;
	uint active;

};

cbuffer cbuff2 : register(b2)
{

	float3 ambientColor;
	DirLight dirLights[DIR_LIGHT_NUM];

}


//���_�V�F�[�_�[�̏o�͍\����
//(���_�V�F�[�_�[����s�N�Z���V�F�[�_�[�ւ̂����Ɏg�p����)
struct VSOutput
{
	//�V�X�e���p���_���W
	float4 svpos:SV_POSITION;
	//�@���x�N�g��
	float3 normal:NORMAL;
	float4 worldpos:POSITION;
	//float4 color:COLOR;
	//uv�l
	float2 uv:TEXCOORD;
};