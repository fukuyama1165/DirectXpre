cbuffer cbuff0:register(b0)
{
	matrix mat;//3D�ϊ��s��
}

cbuffer cbuff1 : register(b1)//�萔�o�b�t�@�̔ԍ���b�̌��ɂ���
{
	float3 m_ambient:packoffset(c0);
	float3 m_diffuse:packoffset(c1);
	float3 m_specular:packoffset(c2);
	float m_alpha : packoffset(c2.w);
}


//���_�V�F�[�_�[�̏o�͍\����
//(���_�V�F�[�_�[����s�N�Z���V�F�[�_�[�ւ̂����Ɏg�p����)
struct VSOutput
{
	//�V�X�e���p���_���W
	float4 svpos:SV_POSITION;
	//�@���x�N�g��
	float3 normal:NORMAL;
	//uv�l
	float2 uv:TEXCOORD;
};