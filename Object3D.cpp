#include "Object3D.h"

Texture* Object3D::texture = nullptr;

const float PI = 3.141592653589f;

Object3D::Object3D()
{
	Scale_ = { 1,1,1 };
	Rotate_ = {};
	Trans_ = {};

	matWorld.IdentityMatrix();
	texture = Texture::GetInstance();
}

Object3D::~Object3D()
{
	
}

void Object3D::Init(ID3D12Device* dev)
{

}

void Object3D::Update()
{
	//�X�P�[���s��X�V
	matScale = matScaleGeneration(Scale_);

	//��]�s��X�V
	matRotate = matRotateGeneration(Rotate_);

	//���s�ړ��s��X�V
	matTrans = matMoveGeneration(Trans_);

	//���[���h�s��X�V
	matWorld.IdentityMatrix();
	matWorld = matScale * matRotate * matTrans;

	

	if (parent_ != nullptr)
	{
		matWorld *= parent_->GetWorldMat();
	}

	constTransformMatUpdata(matView,matProjection);
}

void Object3D::Draw(ID3D12GraphicsCommandList* cmdList,bool PipeLineRuleFlag, bool ChangeSquareFlag, bool ChangeTexure)
{

	//�p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐ݒ�
	//������p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`�����Z�b�g����
	//���߂����[���ŕ`������肢����Ƃ���

	if (PipeLineRuleFlag)
	{
		cmdList->SetPipelineState(pipelinestate.Get());
	}
	else
	{
		cmdList->SetPipelineState(pipelinestate2.Get());
	}

	cmdList->SetGraphicsRootSignature(rootsignature.Get());

	//���_�o�b�t�@�r���[�̐ݒ�
	cmdList->IASetVertexBuffers(0, 1, &vbView);


	//�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
	cmdList->SetGraphicsRootConstantBufferView(0, constBuffMaterial->GetGPUVirtualAddress());
	cmdList->SetGraphicsRootConstantBufferView(1, constBuffMaterial2->GetGPUVirtualAddress());

	texture->Draw(cmdList, ChangeTexure);

	//�C���f�b�N�X�o�b�t�@�̐ݒ�
	cmdList->IASetIndexBuffer(&ibView);

	cmdList->IASetVertexBuffers(0, 1, &vbView);

	//�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
	cmdList->SetGraphicsRootConstantBufferView(3, constBuffTransform->GetGPUVirtualAddress());

	//�`��R�}���h
	if (ChangeSquareFlag)
	{
		//�l�p�`�ɕ`��
		cmdList->DrawIndexedInstanced(indices.size(), 1, 0, 0, 0);
	}
	else
	{
		cmdList->DrawInstanced(3, 1, 0, 0);
	}

}

void Object3D::constTransformMatUpdata(Matrix4x4 matView, Matrix4x4 matProjection)
{
	constMapTransform->mat = matWorld * matView * matProjection;
}

void Object3D::SetScale(Float3 scale)
{
	Scale_ = scale;
}
void Object3D::SetRotate(Float3 rotate)
{
	Rotate_ = rotate;
}
void Object3D::SetPos(Float3 pos)
{
	Trans_ = pos;
}

void Object3D::SetParent(Object3D* parent)
{
	if (parent != nullptr)
	{
		parent_ = parent;
	}
}

Float3 Object3D::GetScale()
{
	return Scale_;
}
Float3 Object3D::GetRotate()
{
	return Rotate_;
}
Float3 Object3D::GetPos()
{
	return Trans_;
}

Matrix4x4 Object3D::GetWorldMat()
{
	return matWorld;
}

Object3D* Object3D::GetParent()
{
	return parent_;
}

Matrix4x4 Object3D::matScaleGeneration(Float3 scale)
{
	//�X�P�[�����O�s���錾
	Matrix4x4 matScale;
	matScale.IdentityMatrix();

	//�X�P�[�����O�{�����s��ɐݒ�
	matScale.m[0][0] = scale.x;
	matScale.m[1][1] = scale.y;
	matScale.m[2][2] = scale.z;
	matScale.m[3][3] = 1;

	return matScale;
}

Matrix4x4 Object3D::matRotateXGeneration(float rotateX)
{
	//X����]�s���錾
	Matrix4x4 matRotateX;
	matRotateX.IdentityMatrix();

	//��]�p���s��ɐݒ�(���W�A��)
	matRotateX.m[0][0] = 1;
	matRotateX.m[1][1] = cosf(rotateX);
	matRotateX.m[1][2] = sinf(rotateX);
	matRotateX.m[2][1] = -sinf(rotateX);
	matRotateX.m[2][2] = cosf(rotateX);
	matRotateX.m[3][3] = 1;

	return matRotateX;
}

Matrix4x4 Object3D::matRotateYGeneration(float rotateY)
{
	//Y����]�s���錾
	Matrix4x4 matRotateY;
	matRotateY.IdentityMatrix();

	//��]�p���s��ɐݒ�(���W�A��)
	matRotateY.m[0][0] = cosf(rotateY);
	matRotateY.m[0][2] = -sinf(rotateY);
	matRotateY.m[1][1] = 1;
	matRotateY.m[2][0] = sinf(rotateY);
	matRotateY.m[2][2] = cosf(rotateY);
	matRotateY.m[3][3] = 1;

	return matRotateY;
}

Matrix4x4 Object3D::matRotateZGeneration(float rotateZ)
{
	//Z����]�s���錾
	Matrix4x4 matRotateZ;
	matRotateZ.IdentityMatrix();

	//��]�p���s��ɐݒ�(���W�A��)
	matRotateZ.m[0][0] = cosf(rotateZ);
	matRotateZ.m[0][1] = sinf(rotateZ);
	matRotateZ.m[1][0] = -sinf(rotateZ);
	matRotateZ.m[1][1] = cosf(rotateZ);
	matRotateZ.m[2][2] = 1;
	matRotateZ.m[3][3] = 1;

	return matRotateZ;
}

Matrix4x4 Object3D::matRotateGeneration(Float3 rotate)
{
	//X����]�s���錾
	Matrix4x4 matRotateX = matRotateXGeneration(rotate.x);

	//Y����]�s���錾
	Matrix4x4 matRotateY = matRotateYGeneration(rotate.y);

	//Z����]�s���錾
	Matrix4x4 matRotateZ = matRotateZGeneration(rotate.z);

	//��]�������s���錾
	Matrix4x4 matRotate;
	matRotate.IdentityMatrix();

	//�v�Z�����p�x���v�Z(���Ԃ͉�]�����郂�f���ɂ���ĕς���)


	matRotate = matRotateZ * matRotateX * matRotateY;

	return matRotate;



}

Matrix4x4 Object3D::matMoveGeneration(Float3 translation)
{
	//�ړ����邽�߂̍s���p��
	Matrix4x4 matMove;
	matMove.IdentityMatrix();

	//�s��Ɉړ��ʂ���
	matMove.m[3][0] = translation.x;
	matMove.m[3][1] = translation.y;
	matMove.m[3][2] = translation.z;

	return matMove;
}

void Object3D::basicInit(ID3D12Device* dev, float WinWidth, float WinHeight)
{

	Win_width = WinWidth;
	Win_height = WinHeight;

	basicVertexInit();

	vertexBuffGeneration(dev);

	vertexShaderGeneration();

	pixelShaderGeneration();

	vertexLayout();

	graphicPipelineGeneration();

	descriptorRangeGeneration();

	rootParamGeneration();

	textureSamplerGeneration();

	rootsignatureGeneration(dev);

	constantBuffGeneration(dev);


	indicesBuffGeneration(dev);

	texture->Init(dev);

}

void Object3D::colorChangeInit(ID3D12Device* dev, float WinWidth, float WinHeight)
{

	Win_width = WinWidth;
	Win_height = WinHeight;

	colorChangeVertexInit();

	vertexBuffGeneration(dev);

	vertexShaderGeneration2();

	pixelShaderGeneration2();

	vertexLayout();

	graphicPipelineGeneration();

	descriptorRangeGeneration();

	rootParamGeneration();

	textureSamplerGeneration();

	rootsignatureGeneration(dev);

	constantBuffGeneration(dev);

	indicesBuffGeneration(dev);

	texture->Init(dev);

}

void Object3D::objDrawInit(ID3D12Device* dev, const char filename[])
{
	objVertexBuffGeneration(dev, filename);

	vertexShaderGeneration();

	pixelShaderGeneration();

	vertexLayout();

	graphicPipelineGeneration();

	descriptorRangeGeneration();

	rootParamGeneration();

	textureSamplerGeneration();

	rootsignatureGeneration(dev);

	constantBuffGeneration(dev);

	indicesBuffGeneration(dev);

	texture->Init(dev);

}

void Object3D::basicVertexInit()
{

	Vertex vert = {};

	//���_�f�[�^(���_���̍��W)
					//  x     y    z      u    v
	vert = { {-50.0f,-50.0f,50.0f }, {}, { 0.0f,1.0f } };//�����O
	vertices.emplace_back(vert);
	vert = { {  50.0f,-50.0f,50.0f },{}, {0.0f,0.0f} };//����O
	vertices.emplace_back(vert);
	vert = { {-50.0f, 50.0f,50.0f }, {}, { 1.0f,1.0f } };//�E���O
	vertices.emplace_back(vert);
	vert = { {  50.0f, 50.0f,50.0f },{}, {1.0f,0.0f} };//�E��O
	vertices.emplace_back(vert);

	vert = { { -50.0f,-50.0f,-50.0f },{},{0.0f,1.0f} };//������
	vertices.emplace_back(vert);
	vert = { { -50.0f, 50.0f,-50.0f },{},{0.0f,0.0f} };//�����
	vertices.emplace_back(vert);
	vert = { {  50.0f,-50.0f,-50.0f },{},{1.0f,1.0f} };//�E����
	vertices.emplace_back(vert);
	vert = { {  50.0f, 50.0f,-50.0f },{},{1.0f,0.0f} };//�E���
	vertices.emplace_back(vert);

	vert = { { -50.0f,-50.0f,-50.0f },{},{0.0f,1.0f} };//������
	vertices.emplace_back(vert);
	vert = { { -50.0f,-50.0f, 50.0f },{},{0.0f,0.0f} };//�����
	vertices.emplace_back(vert);
	vert = { { -50.0f, 50.0f,-50.0f },{},{1.0f,1.0f} };//�E����
	vertices.emplace_back(vert);
	vert = { { -50.0f, 50.0f, 50.0f },{},{1.0f,0.0f} };//�E���
	vertices.emplace_back(vert);

	vert = { { 50.0f,-50.0f,-50.0f },{},{0.0f,1.0f} };//������
	vertices.emplace_back(vert);
	vert = { { 50.0f, 50.0f,-50.0f },{},{0.0f,0.0f} };//�����
	vertices.emplace_back(vert);
	vert = { { 50.0f,-50.0f, 50.0f },{},{1.0f,1.0f} };//�E����
	vertices.emplace_back(vert);
	vert = { { 50.0f, 50.0f, 50.0f },{},{1.0f,0.0f} };//�E���
	vertices.emplace_back(vert);

	vert = { { -50.0f,50.0f,-50.0f },{},{0.0f,1.0f} };//������
	vertices.emplace_back(vert);
	vert = { { -50.0f,50.0f, 50.0f },{},{0.0f,0.0f} };//�����
	vertices.emplace_back(vert);
	vert = { {  50.0f,50.0f,-50.0f },{},{1.0f,1.0f} };//�E����
	vertices.emplace_back(vert);
	vert = { {  50.0f,50.0f, 50.0f },{},{1.0f,0.0f} };//�E���
	vertices.emplace_back(vert);

	vert = { { -50.0f,-50.0f,-50.0f },{},{0.0f,1.0f} };//������
	vertices.emplace_back(vert);
	vert = { {  50.0f,-50.0f,-50.0f },{},{0.0f,0.0f} };//�����
	vertices.emplace_back(vert);
	vert = { { -50.0f,-50.0f, 50.0f },{},{1.0f,1.0f} };//�E����
	vertices.emplace_back(vert);
	vert = { {  50.0f,-50.0f, 50.0f },{},{1.0f,0.0f} };//�E���
	vertices.emplace_back(vert);

	////�C���f�b�N�X�f�[�^
	//�O
	indices.emplace_back(0);
	indices.emplace_back(1);
	indices.emplace_back(2);
	indices.emplace_back(2);
	indices.emplace_back(1);
	indices.emplace_back(3);

	//��
	indices.emplace_back(4);
	indices.emplace_back(5);
	indices.emplace_back(6);
	indices.emplace_back(6);
	indices.emplace_back(5);
	indices.emplace_back(7);

	//��
	indices.emplace_back(9);
	indices.emplace_back(8);
	indices.emplace_back(10);
	indices.emplace_back(10);
	indices.emplace_back(9);
	indices.emplace_back(11);

	//�E
	indices.emplace_back(12);
	indices.emplace_back(13);
	indices.emplace_back(14);
	indices.emplace_back(14);
	indices.emplace_back(13);
	indices.emplace_back(15);

	//��
	indices.emplace_back(16);
	indices.emplace_back(17);
	indices.emplace_back(18);
	indices.emplace_back(18);
	indices.emplace_back(17);
	indices.emplace_back(19);

	//��
	indices.emplace_back(20);
	indices.emplace_back(21);
	indices.emplace_back(22);
	indices.emplace_back(22);
	indices.emplace_back(21);
	indices.emplace_back(23);

	

	//���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
	sizeVB = static_cast<UINT>(sizeof(vertices) * vertices.size());
	//�C���f�b�N�X�f�[�^�S�̂̃T�C�Y
	sizeIB = static_cast<UINT>(sizeof(uint16_t) * indices.size());

	////�r���[�ϊ��s��
	eye_ = { 0, 0, -400 };//���_���W
	target_ = { 0, 0, 0 };//�����_���W
	up_ = { 0, 1, 0 };//������x�N�g��

}
void Object3D::colorChangeVertexInit()
{
	Vertex vert = {};

	//���_�f�[�^(�l�_���̍��W)
					//  x     y    z      u    v
	vert = { {-50.0f,-50.0f,50.0f }, {}, { 0.0f,1.0f } };//����
	vertices.emplace_back(vert);
	vert = { {  50.0f,-50.0f,50.0f },{}, {0.0f,0.0f} };//����
	vertices.emplace_back(vert);
	vert = { {-50.0f, 50.0f,50.0f }, {}, { 1.0f,1.0f }};//�E��
	vertices.emplace_back(vert);
	vert = { {  50.0f, 50.0f,50.0f },{}, {1.0f,0.0f} };//�E��
	vertices.emplace_back(vert);

	////�C���f�b�N�X�f�[�^
	indices.emplace_back(0);
	indices.emplace_back(1);
	indices.emplace_back(2);
	indices.emplace_back(1);
	indices.emplace_back(2);
	indices.emplace_back(3);

	//���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
	sizeVB = static_cast<UINT>(sizeof(vertices) * vertices.size());
	//�C���f�b�N�X�f�[�^�S�̂̃T�C�Y
	sizeIB = static_cast<UINT>(sizeof(uint16_t) * indices.size());

	////�r���[�ϊ��s��
	eye_ = { 0, 0, -400 };//���_���W
	target_ = { 0, 0, 0 };//�����_���W
	up_ = { 0, 1, 0 };//������x�N�g��

}

void Object3D::vertexBuffGeneration(ID3D12Device* dev)
{
#pragma region ���_�o�b�t�@�̊m��

	//���_�o�b�t�@�̊m�ە�

	heapprop.Type = D3D12_HEAP_TYPE_UPLOAD;//GPU�ւ̓]���p


	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeVB;//���_�f�[�^�S�̂̃T�C�Y
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

#pragma endregion

#pragma region ���_�o�b�t�@�̐���

	//���_�o�b�t�@�̐���

	result = dev->CreateCommittedResource(
		&heapprop,//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resDesc,//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff)
	);

#pragma endregion

#pragma region �@���x�N�g���v�Z

	for (int i = 0; i < indices.size() / 3; i++)
	{//�O�p�`1���ƂɌv�Z���Ă���

		//�O�p�`�̃C���f�b�N�X�����o����,�ꎞ�I�ȕϐ��ɓ����
		unsigned short indices0 = indices[i * 3 + 0];
		unsigned short indices1 = indices[i * 3 + 1];
		unsigned short indices2 = indices[i * 3 + 2];

		//�O�p�`���\�����钸�_���W���x�N�g���ɑ��
		XMVECTOR p0 = XMLoadFloat3(&vertices[indices0].pos);
		XMVECTOR p1 = XMLoadFloat3(&vertices[indices1].pos);
		XMVECTOR p2 = XMLoadFloat3(&vertices[indices2].pos);

		//p0��p1�x�N�g���Ap0��p2�x�N�g�����v�Z(�x�N�g���̌��Z)
		XMVECTOR v1 = XMVectorSubtract(p1, p0);
		XMVECTOR v2 = XMVectorSubtract(p2, p0);

		//�O�ς͗������琂���ȃx�N�g��
		XMVECTOR normal = XMVector3Cross(v1, v2);

		//���K��(������1�ɂ���)
		normal = XMVector3Normalize(normal);

		//���߂��@���𒸓_�f�[�^�ɑ��
		XMStoreFloat3(&vertices[indices0].normal, normal);
		XMStoreFloat3(&vertices[indices1].normal, normal);
		XMStoreFloat3(&vertices[indices2].normal, normal);

	}

#pragma endregion


#pragma region ���_�o�b�t�@�ւ̃f�[�^�]��

	//���_�o�b�t�@�ւ̃f�[�^�]��
	//GPU��̃o�b�t�@�ɑΉ��������z���������擾
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);

	//�S���_�ɑ΂���
	for (int i = 0; i < vertices.size(); i++)
	{
		vertMap[i] = vertices[i];//���W���R�s�[
	}


	//�Ȃ��������
	vertBuff->Unmap(0, nullptr);

#pragma endregion

#pragma region ���_�o�b�t�@�r���[�̐���

	//���_�o�b�t�@�r���[�̍쐬(GPU�ւ̓������)��

	//���_�o�b�t�@�r���[�̍쐬

	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeVB;
	vbView.StrideInBytes = sizeof(vertices[0]);

#pragma endregion

}

void Object3D::objVertexBuffGeneration(ID3D12Device* dev, const char filename[])
{
#pragma region OBj�ǂݍ���

	std::vector<XMFLOAT3> positions;
	std::vector<XMFLOAT3> normals;
	std::vector<XMFLOAT2> texcoords;

	//�t�@�C���X�g���[��
	std::ifstream file;

	//0bj�t�@�C�����J��
	file.open(filename);

	//�t�@�C���I�[�v�����s���`�F�b�N
	if (file.fail())
	{
		assert(0);
	}

	std::string line;
	while (std::getline(file, line))
	{

		//1�s���̕�������X�g���[���ɕϊ����ĉ�͂��₷������
		std::istringstream lineStream(line);

		//���p�X�y�[�X��؂�ōs�̐擪��������擾
		std::string key;
		std::getline(lineStream, key, ' ');

		//�擪������v�Ȃ璸�_���W
		if (key == "v")
		{

			//x,y,z���W�ǂݍ���
			XMFLOAT3 position{};
			lineStream >> position.x;
			lineStream >> position.y;
			lineStream >> position.z;

			//���W�f�[�^�ɒǉ�
			positions.emplace_back(position);


		}

		//�擪������vt�Ȃ�e�N�X�`��
		if (key == "vt")
		{

			//U,V�����ǂݍ���
			XMFLOAT2 texcoord{};
			lineStream >> texcoord.x;
			lineStream >> texcoord.y;

			//V�������]
			texcoord.y = 1.0f - texcoord.y;

			//�e�N�X�`�����W�f�[�^�ɒǉ�
			texcoords.emplace_back(texcoord);

		}

		//�擪������vn�Ȃ�@���x�N�g��
		if (key == "vn")
		{

			//x,y,z���W�ǂݍ���
			XMFLOAT3 normal{};
			lineStream >> normal.x;
			lineStream >> normal.y;
			lineStream >> normal.z;

			//���W�f�[�^�ɒǉ�
			normals.emplace_back(normal);


		}

		//�擪������f�Ȃ�|���S��(�O�p�`)
		if (key == "f")
		{

			//���p�X�y�[�X��؂�ōs�̑�����ǂݍ���
			std::string indexString;

			while (std::getline(lineStream, indexString, ' '))
			{

				//���_�C���f�b�N�X1���̕�������X�g���[���ɕϊ����ĉ�͂��₷������
				std::istringstream indexStream(indexString);

				unsigned short indexPosition;
				unsigned short indexNormal;
				unsigned short indexTexcoord;
				indexStream >> indexPosition;

				//�X���b�V�����΂�
				indexStream.seekg(1, std::ios_base::cur);
				indexStream >> indexTexcoord;

				//�X���b�V�����΂�
				indexStream.seekg(1, std::ios_base::cur);
				indexStream >> indexNormal;

				//���_�f�[�^�̒ǉ�
				Vertex vertex{};
				vertex.pos = positions[indexPosition - 1];
				vertex.normal = normals[indexNormal - 1];
				vertex.uv = texcoords[indexTexcoord - 1];
				vertices.emplace_back(vertex);

				indices.emplace_back((unsigned short)indices.size());

			}

		}

	}

	file.close();

	//���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
	sizeVB = static_cast<UINT>(sizeof(vertices) * vertices.size());
	//�C���f�b�N�X�f�[�^�S�̂̃T�C�Y
	sizeIB = static_cast<UINT>(sizeof(uint16_t) * indices.size());

#pragma endregion

	vertexBuffGeneration(dev);
}

void Object3D::vertexShaderGeneration()
{
#pragma region ���_�V�F�[�_�t�@�C���̓ǂݍ��݂ƃR���p�C��

	//���_�V�F�[�_�t�@�C���̓ǂݍ��ݕ�
	//���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/shaders/BasicVS.hlsl",//�V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,//�C���N���[�h�\�ɂ���
		"main",//�G���g���[�|�C���g��
		"vs_5_0",//�V�F�[�_���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,//�f�o�b�N�p�ݒ�
		0,
		&vsBlob,
		&errorBlob
	);

#pragma endregion 

#pragma region ���_�V�F�[�_�̓ǂݍ��ݎ��̃G���[��\������ꏊ

	//���_�V�F�[�_�̓ǂݍ��ݎ��̃G���[��\������ꏊ
	if (FAILED(result))
	{
		//errorBlob����G���[���e��string�^�ɃR�s�[
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		//�G���[���e���E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

#pragma endregion
}

void Object3D::vertexShaderGeneration2()
{
#pragma region ���_�V�F�[�_�t�@�C���̓ǂݍ��݂ƃR���p�C��

	//���_�V�F�[�_�t�@�C���̓ǂݍ��ݕ�
	//���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/shaders/vertexMoveVS.hlsl",//�V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,//�C���N���[�h�\�ɂ���
		"main",//�G���g���[�|�C���g��
		"vs_5_0",//�V�F�[�_���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,//�f�o�b�N�p�ݒ�
		0,
		&vsBlob,
		&errorBlob
	);

#pragma endregion 

#pragma region ���_�V�F�[�_�̓ǂݍ��ݎ��̃G���[��\������ꏊ

	//���_�V�F�[�_�̓ǂݍ��ݎ��̃G���[��\������ꏊ
	if (FAILED(result))
	{
		//errorBlob����G���[���e��string�^�ɃR�s�[
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		//�G���[���e���E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

#pragma endregion
}

void Object3D::pixelShaderGeneration()
{
#pragma region �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��

	//�s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/shaders/BasicPS.hlsl",//�V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,//�C���N���[�h�\�ɂ���
		"main",//�G���g���[�|�C���g��
		"ps_5_0",//�V�F�[�_���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,//�f�o�b�N�p�ݒ�
		0,
		&psBlob,
		&errorBlob
	);

#pragma endregion

#pragma region �s�N�Z���V�F�[�_�̓ǂݍ��ݎ��̃G���[��\������ꏊ

	//�s�N�Z���V�F�[�_�̓ǂݍ��ݎ��̃G���[��\������ꏊ
	if (FAILED(result))
	{
		//errorBlob����G���[���e��string�^�ɃR�s�[
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		//�G���[���e���E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

#pragma endregion
}

void Object3D::pixelShaderGeneration2()
{
#pragma region �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��

	//�s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/shaders/colorChangePS.hlsl",//�V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,//�C���N���[�h�\�ɂ���
		"main",//�G���g���[�|�C���g��
		"ps_5_0",//�V�F�[�_���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,//�f�o�b�N�p�ݒ�
		0,
		&psBlob,
		&errorBlob
	);

#pragma endregion

#pragma region �s�N�Z���V�F�[�_�̓ǂݍ��ݎ��̃G���[��\������ꏊ

	//�s�N�Z���V�F�[�_�̓ǂݍ��ݎ��̃G���[��\������ꏊ
	if (FAILED(result))
	{
		//errorBlob����G���[���e��string�^�ɃR�s�[
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		//�G���[���e���E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

#pragma endregion
}

void Object3D::vertexLayout()
{
#pragma region ���_���C�A�E�g�̐ݒ�

	//���_���C�A�E�g
	//�O���t�B�b�N�p�C�v���C���Œ��_����̃f�[�^�ɉ����������邩���߂�
	//����͍Œ����3D���W�����������Ă���炵��
	inputLayout[0] =
	{
		"POSITION",//�Z�}���e�B�b�N��
		0,//�����Z�}���e�B�b�N������������Ƃ��̃C���f�b�N�X
		DXGI_FORMAT_R32G32B32_FLOAT,//�v�f���ƃr�b�g����Ԃ�(XYZ�̂R��float�^�Ȃ̂�R32G32B32_FLOAT)
		0,//���̓X���b�g�C���f�b�N�X
		D3D12_APPEND_ALIGNED_ELEMENT,//�f�[�^�̃I�t�Z�b�g�l(D3D12_APPEND_ALIGNED_ELEMENT���Ǝ����ݒ�)
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,//���̓f�[�^����(�W����D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA)
		0,//��x�ɕ`�悷��C���X�^���X��
	};
	//���W�ȊO�Ɂ@�F�A�e�N�X�`��UV�Ȃǂ�n���ꍇ�͂���ɑ�����
	inputLayout[1] =
	{//�@���x�N�g��
		"NORMAL",
		0,
		DXGI_FORMAT_R32G32B32_FLOAT,
		0,
		D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
		0
	};

	inputLayout[2] =
	{//UV���W
		"TEXCOORD",
		0,
		DXGI_FORMAT_R32G32_FLOAT,
		0,
		D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
		0
	};

#pragma endregion
}

void Object3D::graphicPipelineGeneration()
{
#pragma region �O���t�B�b�N�X�p�C�v���C���ݒ�

	//�O���t�B�b�N�X�p�C�v���C����

	//���_�V�F�[�_�A�s�N�Z���V�F�[�_���p�C�v���C���ɐݒ�
	gpipeline.VS.pShaderBytecode = vsBlob->GetBufferPointer();
	gpipeline.VS.BytecodeLength = vsBlob->GetBufferSize();
	gpipeline.PS.pShaderBytecode = psBlob->GetBufferPointer();
	gpipeline.PS.BytecodeLength = psBlob->GetBufferSize();

	//�T���v���}�X�N�ƃ��X�^���C�U�X�e�[�g�̐ݒ�
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;//�W���ݒ�
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;//�J�����O���Ȃ�
	gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;//�|���S�����h��Ԃ�
	gpipeline.RasterizerState.DepthClipEnable = true;//�[�x�N���b�s���O��L����


	//�u�����h�X�e�[�g�̐ݒ�
	//gpipeline.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;//RGBA�S�Ẵ`�����l����`��
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = gpipeline.BlendState.RenderTarget[0];
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

#pragma region �u�����h�X�e�[�g�̋��ʐݒ�(�A���t�@�l)

	blenddesc.BlendEnable = true;//�u�����h��L���ɂ���
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;//���Z
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;//�\�[�X�̒l��100%�g��
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;//�f�X�g�̒l��0%�g��

#pragma endregion


#pragma region ���Z����

	//blenddesc.BlendOp = D3D12_BLEND_OP_ADD;//���Z
	//blenddesc.SrcBlend = D3D12_BLEND_ONE;//�\�[�X�̒l��100%�g��
	//blenddesc.DestBlend = D3D12_BLEND_ONE;//�f�X�g�̒l��100%�g��

#pragma endregion �R�����g�A�E�g��

#pragma region ���Z����

	//blenddesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;//�f�X�g����\�[�X�����Z
	//blenddesc.SrcBlend = D3D12_BLEND_ONE;//�\�[�X�̒l��100%�g��
	//blenddesc.DestBlend = D3D12_BLEND_ONE;//�f�X�g�̒l��100%�g��

#pragma endregion �R�����g�A�E�g��

#pragma region �F���]

	//blenddesc.BlendOp = D3D12_BLEND_OP_ADD;//���Z
	//blenddesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;//1.0f-�f�X�g�J���[�̒l
	//blenddesc.DestBlend = D3D12_BLEND_ZERO;//�g��Ȃ�

#pragma endregion �R�����g�A�E�g��

#pragma region ����������

	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;//���Z
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;//�\�[�X�̃A���t�@�l
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;//1.0f-�\�[�X�̃A���t�@�l

#pragma endregion



	//���_���C�A�E�g�̐ݒ�
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	//�}�`�̌`����O�p�`�ɐݒ�
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	//���̑��̐ݒ�
	gpipeline.NumRenderTargets = 1;//�`��Ώۂ͂P��
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;//0�`255�w���RGBA
	gpipeline.SampleDesc.Count = 1;//�P�s�N�Z���ɂ��P��T���v�����O

#pragma endregion �ʏ�`��|���S������h��Ԃ�(�O�p�`)

#pragma region �O���t�B�b�N�X�p�C�v���C���Q�̐ݒ�

//�O���t�B�b�N�X�p�C�v���C����


	//���_�V�F�[�_�A�s�N�Z���V�F�[�_���p�C�v���C���ɐݒ�
	gpipeline2.VS.pShaderBytecode = vsBlob->GetBufferPointer();
	gpipeline2.VS.BytecodeLength = vsBlob->GetBufferSize();
	gpipeline2.PS.pShaderBytecode = psBlob->GetBufferPointer();
	gpipeline2.PS.BytecodeLength = psBlob->GetBufferSize();

	//�T���v���}�X�N�ƃ��X�^���C�U�X�e�[�g�̐ݒ�
	gpipeline2.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;//�W���ݒ�
	gpipeline2.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;//�J�����O���Ȃ�
	gpipeline2.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;//���C���[�t���[���`��
	gpipeline2.RasterizerState.DepthClipEnable = true;//�[�x�N���b�s���O��L����

	//�u�����h�X�e�[�g�̐ݒ�
	//gpipeline2.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;//RGBA�S�Ẵ`�����l����`��
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc2 = gpipeline2.BlendState.RenderTarget[0];
	blenddesc2.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

#pragma region ���ʐݒ�(�A���t�@�l)

	blenddesc2.BlendEnable = true;//�u�����h��L���ɂ���
	blenddesc2.BlendOpAlpha = D3D12_BLEND_OP_ADD;//���Z
	blenddesc2.SrcBlendAlpha = D3D12_BLEND_ONE;//�\�[�X�̒l��100%�g��
	blenddesc2.DestBlendAlpha = D3D12_BLEND_ZERO;//�f�X�g�̒l��0%�g��

#pragma endregion

#pragma region ����������

	blenddesc2.BlendOp = D3D12_BLEND_OP_ADD;//���Z
	blenddesc2.SrcBlend = D3D12_BLEND_SRC_ALPHA;//�\�[�X�̃A���t�@�l
	blenddesc2.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;//1.0f-�\�[�X�̃A���t�@�l

#pragma endregion

	//���_���C�A�E�g�̐ݒ�
	gpipeline2.InputLayout.pInputElementDescs = inputLayout;
	gpipeline2.InputLayout.NumElements = _countof(inputLayout);

	//�}�`�̌`����O�p�`�ɐݒ�
	gpipeline2.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	//���̑��̐ݒ�
	gpipeline2.NumRenderTargets = 1;//�`��Ώۂ͂P��
	gpipeline2.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;//0�`255�w���RGBA
	gpipeline2.SampleDesc.Count = 1;//�P�s�N�Z���ɂ��P��T���v�����O

#pragma endregion �ʏ�`�惏�C���[�t���[���`��(�O�p�`)

}

void Object3D::descriptorRangeGeneration()
{
#pragma region �f�X�N���v�^�����W�̐ݒ�
	descriptorRange.NumDescriptors = 1;//��x�̕`��Ɏg���e�N�X�`�����P���Ȃ̂�1
	descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange.BaseShaderRegister = 0;//�e�N�X�`�����W�X�^0��
	descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

#pragma endregion
}

void Object3D::rootParamGeneration()
{
#pragma region ���[�g�p�����[�^

	//���[�g�p�����[�^�̐ݒ�

	//�F
	rootParam[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;//�萔�o�b�t�@�r���[
	rootParam[0].Descriptor.ShaderRegister = 0;//�萔�o�b�t�@�ԍ�
	rootParam[0].Descriptor.RegisterSpace = 0;//�f�t�H���g�l
	rootParam[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//�S�ẴV�F�[�_�[���猩����

	//�ʒu
	rootParam[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;//�萔�o�b�t�@�r���[
	rootParam[1].Descriptor.ShaderRegister = 1;//�萔�o�b�t�@�ԍ�
	rootParam[1].Descriptor.RegisterSpace = 0;//�f�t�H���g�l
	rootParam[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//�S�ẴV�F�[�_�[���猩����

	//�摜�f�[�^�p
	rootParam[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//���
	rootParam[2].DescriptorTable.pDescriptorRanges = &descriptorRange;//�f�X�N���v�^�����W
	rootParam[2].DescriptorTable.NumDescriptorRanges = 1;//�f�X�N���v�^�����W��
	rootParam[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//�S�ẴV�F�[�_���猩����

	rootParam[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;//�萔�o�b�t�@�r���[
	rootParam[3].Descriptor.ShaderRegister = 2;//�萔�o�b�t�@�ԍ�
	rootParam[3].Descriptor.RegisterSpace = 0;//�f�t�H���g�l
	rootParam[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//�S�ẴV�F�[�_�[���猩����

#pragma endregion �萔�o�b�t�@�𑝂₵���瑝�₷�Ƃ��낪����
}

void Object3D::textureSamplerGeneration()
{
#pragma region �e�N�X�`���T���v���[�̐ݒ�

	//�e�N�X�`���T���v���[�̐ݒ�
	sampleDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;//���J��Ԃ�(�^�C�����O)
	sampleDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;//�c�J��Ԃ�(�^�C�����O)
	sampleDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;//���s�J��Ԃ�(�^�C�����O)
	sampleDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;//�{�[�_�[�̎��͍�
	sampleDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;//�S�ă��j�A���
	sampleDesc.MaxLOD = D3D12_FLOAT32_MAX;//�~�b�v�}�b�v�ő�l
	sampleDesc.MinLOD = 0.0f;//�~�b�v�}�b�v�ŏ��l
	sampleDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	sampleDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;//�s�N�Z���V�F�[�_����̂ݎg�p�\

#pragma endregion
}

void Object3D::rootsignatureGeneration(ID3D12Device* dev)
{
#pragma region ���[�g�V�O�l�`���ݒ�

	//���[�g�V�O�l�`���̐ݒ�
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParam;//���[�g�p�����[�^�̐擪�A�h���X
	rootSignatureDesc.NumParameters = _countof(rootParam);//���[�g�p�����[�^��
	rootSignatureDesc.pStaticSamplers = &sampleDesc;
	rootSignatureDesc.NumStaticSamplers = 1;

	//���[�g�V�O�l�`���̐���
	Microsoft::WRL::ComPtr<ID3DBlob> rootSigBlob = nullptr;
	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	result = dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootsignature));


	//�p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
	gpipeline.pRootSignature = rootsignature.Get();

	//�p�C�v���C��2�ɂ����[�g�V�O�l�`�����Z�b�g
	gpipeline2.pRootSignature = rootsignature.Get();

#pragma endregion �萔�o�b�t�@�𑝂₵���烋�[�g�p�����[�^�����������p�����[�^��������������

#pragma region �f�v�X�X�e���V���X�e�[�g

	//�f�v�X�X�e���V���X�e�[�g�̐ݒ�
	gpipeline.DepthStencilState.DepthEnable = true;//�[�x�e�X�g���s��
	gpipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;//�������݋���
	gpipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;//��������΍��i
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;//�[�x�l�t�H�[�}�b�g

	gpipeline2.DepthStencilState.DepthEnable = true;//�[�x�e�X�g���s��
	gpipeline2.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;//�������݋���
	gpipeline2.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;//��������΍��i
	gpipeline2.DSVFormat = DXGI_FORMAT_D32_FLOAT;//�[�x�l�t�H�[�}�b�g



#pragma endregion


#pragma region �p�C�v���C���X�e�[�g�̐���

	//�p�C�v���C���X�e�[�g�̐���

	result = dev->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelinestate));

	//�p�C�v���C���X�e�[�g�Q�̐���

	result = dev->CreateGraphicsPipelineState(&gpipeline2, IID_PPV_ARGS(&pipelinestate2));

#pragma endregion

}

void Object3D::constantBuffGeneration(ID3D12Device* dev)
{
#pragma region �萔�o�b�t�@

	//�萔�o�b�t�@�̐����p�̐ݒ�
	
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;//GPU�ւ̓]���p


	

	cbResourceDesc = constBuffResourceGeneration(sizeof(ConstBufferDataMaterial));

	//�萔�o�b�t�@�̐���
	result = dev->CreateCommittedResource(
		&cbHeapProp,//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffMaterial)
	);
	assert(SUCCEEDED(result));

	cbResourceDesc = constBuffResourceGeneration(sizeof(ConstBufferDataMaterial2));

	//�萔�o�b�t�@�̐���
	result = dev->CreateCommittedResource(
		&cbHeapProp,//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffMaterial2)
	);
	assert(SUCCEEDED(result));

	
	cbResourceDesc = constBuffResourceGeneration(sizeof(ConstBufferDataTransform));

	result = dev->CreateCommittedResource(
		&cbHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffTransform));
	assert(SUCCEEDED(result));

	result = constBuffTransform->Map(0, nullptr, (void**)&constMapTransform);
	assert(SUCCEEDED(result));

	//cbResourceDesc = constBuffResourceGeneration(sizeof(ConstBufferDataTransform));

	////�萔�o�b�t�@�̐���
	//result = dev->CreateCommittedResource(
	//	&cbHeapProp,//�q�[�v�ݒ�
	//	D3D12_HEAP_FLAG_NONE,
	//	&cbResourceDesc,//���\�[�X�ݒ�
	//	D3D12_RESOURCE_STATE_GENERIC_READ,
	//	nullptr,
	//	IID_PPV_ARGS(&constBuffTransform0)
	//);
	//assert(SUCCEEDED(result));


	////�萔�o�b�t�@�̃}�b�s���O
	//
	result = constBuffMaterial->Map(0, nullptr, (void**)&constMapMaterial);//�}�b�s���O
	assert(SUCCEEDED(result));

	//�}�b�s���O�������GPU��VRSM��CPU�ƘA������
	//Unmap������ƂȂ��肪��������邪�萔�o�b�t�@�͏��������邱�Ƃ������̂�
	//���Ȃ��Ă����v

	//�l���������ނƎ����I�ɓ]�������



	constMapMaterial->color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);


	result = constBuffMaterial2->Map(0, nullptr, (void**)&constMapMaterial2);//�}�b�s���O
	assert(SUCCEEDED(result));

	constMapMaterial2->posM = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);

	

	//���s���ˍs��̌v�Z
	//constMapTransform->mat = XMMatrixOrthographicOffCenterLH(0.0f,Win_width,Win_height, 0.0f, 0.0f, 1.0f);

	//�������e�s��̌v�Z

	matProjection = perspectiveProjectionGeneration((45.0f * (PI / 180)), 0.1f, 1000.0f);

	matView = matViewGeneration(eye_, target_, up_);



	//�r���[�ϊ��s��	
	Update();

#pragma endregion
}

D3D12_RESOURCE_DESC Object3D::constBuffResourceGeneration(int size)
{
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (size + 0xff) & ~0xff;//256�o�C�g�A���C�������g
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	return cbResourceDesc;
}

void Object3D::indicesBuffGeneration(ID3D12Device* dev)
{
#pragma region �C���f�b�N�X�o�b�t�@

	//���\�[�X�ݒ�
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeIB;//�C���f�b�N�X��񂪓��镪�̃T�C�Y
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//�C���f�b�N�X�o�b�t�@�̐���

	result = dev->CreateCommittedResource(
		&heapprop,//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resDesc,//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff)
	);

	//�C���f�b�N�X�o�b�t�@���}�b�s���O
	uint16_t* indexMap = nullptr;
	result = indexBuff->Map(0, nullptr, (void**)&indexMap);
	//�S�C���f�b�N�X�ɑ΂���
	for (int i = 0; i < indices.size(); i++)
	{
		indexMap[i] = indices[i];//�C���f�b�N�X���R�s�[
	}

	//�}�b�s���O����
	indexBuff->Unmap(0, nullptr);

#pragma endregion ���_��ʂ鏇�Ԃ����߂�ꏊ

#pragma region �C���f�b�N�X�o�b�t�@�r���[

	//�C���f�b�N�X�o�b�t�@�r���[�̐���

	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeIB;

#pragma endregion

}

Matrix4x4 Object3D::matViewGeneration(Float3 eye, Float3 target, Float3 up)
{

	Float3 zVer = target - eye;

	zVer.normalize();

	Float3 xVer = up.cross(zVer);

	xVer.normalize();

	Float3 yVer = zVer.cross(xVer);


	Matrix4x4 cameraRotateMat = {};

	cameraRotateMat.IdentityMatrix();

	cameraRotateMat.m[0][0] = xVer.x;
	cameraRotateMat.m[0][1] = xVer.y;
	cameraRotateMat.m[0][2] = xVer.z;

	cameraRotateMat.m[1][0] = yVer.x;
	cameraRotateMat.m[1][1] = yVer.y;
	cameraRotateMat.m[1][2] = yVer.z;

	cameraRotateMat.m[2][0] = zVer.x;
	cameraRotateMat.m[2][1] = zVer.y;
	cameraRotateMat.m[2][2] = zVer.z;

	cameraRotateMat.m[3][0] = eye.x;
	cameraRotateMat.m[3][1] = eye.y;
	cameraRotateMat.m[3][2] = eye.z;

	Matrix4x4 ans = {};

	ans = cameraRotateMat.InverseMatrix();

	Float3 eyeDis = {};
	eyeDis = { target.x - eye.x,target.y - eye.y,target.z - eye.z };

	Float3 R2 = eyeDis.normalize();

	Float3 R0 = up.cross(R2);
	R0 = R0.normalize();

	Float3 R1 = R2.cross(R0);

	Float3 NegEyePosition = { -eye.x,-eye.y,-eye.z };

	Float3 D0 = float3Dat(R0, NegEyePosition);
	Float3 D1 = float3Dat(R1, NegEyePosition);
	Float3 D2 = float3Dat(R2, NegEyePosition);

	Matrix4x4 M;
	M.m[0][0] = R0.x;
	M.m[0][1] = R0.y;
	M.m[0][2] = R0.z;
	M.m[0][3] = D0.x;

	M.m[1][0] = R1.x;
	M.m[1][1] = R1.y;
	M.m[1][2] = R1.z;
	M.m[1][3] = D1.x;

	M.m[2][0] = R2.x;
	M.m[2][1] = R2.y;
	M.m[2][2] = R2.z;
	M.m[2][3] = D2.x;

	M.m[3][0] = 0.0f;
	M.m[3][1] = 0.0f;
	M.m[3][2] = 0.0f;
	M.m[3][3] = 1.0f;

	//M = XMMatrixTranspose(M);

	//// x.x,x.y,y.x,y.y
	//XMVECTOR vTemp1 = _mm_shuffle_ps(M.r[0], M.r[1], _MM_SHUFFLE(1, 0, 1, 0));
	//// x.z,x.w,y.z,y.w
	//XMVECTOR vTemp3 = _mm_shuffle_ps(M.r[0], M.r[1], _MM_SHUFFLE(3, 2, 3, 2));
	//// z.x,z.y,w.x,w.y
	//XMVECTOR vTemp2 = _mm_shuffle_ps(M.r[2], M.r[3], _MM_SHUFFLE(1, 0, 1, 0));
	//// z.z,z.w,w.z,w.w
	//XMVECTOR vTemp4 = _mm_shuffle_ps(M.r[2], M.r[3], _MM_SHUFFLE(3, 2, 3, 2));

	Float4 vTemp1 = { M.m[0][0],M.m[0][1],M.m[1][0],M.m[1][1] };
	Float4 vTemp2 = { M.m[0][2],M.m[0][3],M.m[1][2],M.m[1][3] };
	Float4 vTemp3 = { M.m[2][0],M.m[2][1],M.m[3][0],M.m[3][1] };
	Float4 vTemp4 = { M.m[2][2],M.m[2][3],M.m[3][2],M.m[3][3] };

	//XMMATRIX mResult;
	//// x.x,y.x,z.x,w.x
	//mResult.r[0] = _mm_shuffle_ps(vTemp1, vTemp2, _MM_SHUFFLE(2, 0, 2, 0));
	//// x.y,y.y,z.y,w.y
	//mResult.r[1] = _mm_shuffle_ps(vTemp1, vTemp2, _MM_SHUFFLE(3, 1, 3, 1));
	//// x.z,y.z,z.z,w.z
	//mResult.r[2] = _mm_shuffle_ps(vTemp3, vTemp4, _MM_SHUFFLE(2, 0, 2, 0));
	//// x.w,y.w,z.w,w.w
	//mResult.r[3] = _mm_shuffle_ps(vTemp3, vTemp4, _MM_SHUFFLE(3, 1, 3, 1));

	Matrix4x4 mResult;
	mResult.m[0][0] = vTemp1.x;
	mResult.m[0][1] = vTemp1.z;
	mResult.m[0][2] = vTemp3.x;
	mResult.m[0][3] = vTemp3.z;

	mResult.m[1][0] = vTemp1.y;
	mResult.m[1][1] = vTemp1.w;
	mResult.m[1][2] = vTemp3.y;
	mResult.m[1][3] = vTemp3.w;

	mResult.m[2][0] = vTemp2.x;
	mResult.m[2][1] = vTemp2.z;
	mResult.m[2][2] = vTemp4.x;
	mResult.m[2][3] = vTemp4.z;

	mResult.m[3][0] = vTemp2.y;
	mResult.m[3][1] = vTemp2.w;
	mResult.m[3][2] = vTemp4.y;
	mResult.m[3][3] = vTemp4.w;

	//XMMATRIX matvi;

	//XMFLOAT3 eye2;
	//XMFLOAT3 target2;
	//XMFLOAT3 up2;

	////�r���[�ϊ��s��
	//eye2 = { eye.x, eye.y, eye.z };//���_���W
	//target2 = { target.x, target.y, target.z };//�����_���W
	//up2 = { up.x, up.y, up.z };//������x�N�g��

	//matvi = XMMatrixLookAtLH(XMLoadFloat3(&eye2), XMLoadFloat3(&target2), XMLoadFloat3(&up2));

	return mResult;
}

Matrix4x4 Object3D::perspectiveProjectionGeneration(float FovAngleY, float NearZ, float FarZ)
{
	Matrix4x4 ans = {};

	/*ans.IdentityMatrix();

	float fReciprocalWidth = 1.0f / (ViewRight - 0);
	float fReciprocalHeight = 1.0f / (0 - ViewBottom);

	float fRange = 1.0f / (FarZ - NearZ);

	ans.m[0][0] = {fReciprocalWidth * 2};

	ans.m[1][1] = { fReciprocalHeight * 2 };

	ans.m[2][2] = { fRange };

	ans.m[3][0] = { -(0 + ViewRight) * fReciprocalWidth };
	ans.m[3][1] = { -(0 + ViewBottom) * fReciprocalHeight };
	ans.m[3][2] = { fRange * -NearZ };*/

	float SinFov;
	float CosFov;
	float AspectRatio = Win_width / Win_height;
	sinCos(SinFov, CosFov, 0.5f * FovAngleY);

	float fRange = FarZ / (FarZ - NearZ);

	float Height = CosFov / SinFov;


	ans.m[0][0] = Height / AspectRatio;

	ans.m[1][1] = Height;

	ans.m[2][2] = fRange;
	ans.m[2][3] = 1.0f;

	ans.m[3][2] = (-fRange) * NearZ;

	return ans;

}

void Object3D::sinCos(float& Sin, float& Cos, float angle)
{
	Sin = sinf(angle);
	Cos = cosf(angle);
}

Float3 Object3D::float3Dat(Float3 A, Float3 B)
{
	Float3 num = { A.x * B.x,A.y * B.y,A.z * B.z };

	num.x = num.x + num.y + num.z;

	num = { num.x,num.x,num.x };

	return num;

}

void Object3D::deleteTexture()
{
	if (texture != nullptr)
	{
		texture->instanceDelete();
	}
}

void Object3D::matViewUpdata(Float3 eye, Float3 target, Float3 up)
{

	eye_ = eye;
	target_ = target;
	up_ = up;

	matView = matViewGeneration(eye_, target_, up_);

	Update();

}
