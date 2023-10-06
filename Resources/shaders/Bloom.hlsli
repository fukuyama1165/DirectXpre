cbuffer ConstBufferDataMaterial : register(b0)
{
	float4 color;//����
	float4 sigma;//�u���[�����|����Ƃ��̃u���[�p�̒l(x�̂�)
	float4 grayScaleStep;//�u���[���p�̃O���[�X�P�[����smoothstep�p�̑傫��(x,y�̂�)
};

cbuffer ConstBufferDataTransform : register(b1)
{
	matrix mat;//3D�ϊ��s��
};

//���_�V�F�[�_�[�̏o�͍\����
//(���_�V�F�[�_�[����s�N�Z���V�F�[�_�[�ւ̂����Ɏg�p����)
struct VSOutput
{
	//�V�X�e���p���_���W
	float4 svpos:SV_POSITION;
	//uv�l
	float2 uv:TEXCOORD;
};