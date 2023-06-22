#include "Model.h"

Model::Model()
{
}

Model::~Model()
{
}


void Model::vertexBuffObjGeneration()
{
#pragma region ���_�o�b�t�@�̊m��

	//���_�o�b�t�@�̊m�ە�

	heapprop_.Type = D3D12_HEAP_TYPE_UPLOAD;//GPU�ւ̓]���p


	resDesc_.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc_.Width = sizeVB_;//���_�f�[�^�S�̂̃T�C�Y
	resDesc_.Height = 1;
	resDesc_.DepthOrArraySize = 1;
	resDesc_.MipLevels = 1;
	resDesc_.SampleDesc.Count = 1;
	resDesc_.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

#pragma endregion

#pragma region ���_�o�b�t�@�̐���

	//���_�o�b�t�@�̐���

	result_ = DirectXInit::GetInstance()->Getdev()->CreateCommittedResource(
		&heapprop_,//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resDesc_,//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff_)
	);

#pragma endregion


#pragma region ���_�o�b�t�@�ւ̃f�[�^�]��

	//���_�o�b�t�@�ւ̃f�[�^�]��
	//GPU��̃o�b�t�@�ɑΉ��������z���������擾
	result_ = vertBuff_->Map(0, nullptr, (void**)&vertMap_);

	//�S���_�ɑ΂���
	for (uint16_t i = 0; i < vertices_.size(); i++)
	{
		vertMap_[i] = vertices_[i];//���W���R�s�[
	}


	//�Ȃ��������
	vertBuff_->Unmap(0, nullptr);

#pragma endregion

#pragma region ���_�o�b�t�@�r���[�̐���

	//���_�o�b�t�@�r���[�̍쐬(GPU�ւ̓������)��

	//���_�o�b�t�@�r���[�̍쐬

	vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();
	vbView_.SizeInBytes = sizeVB_;
	vbView_.StrideInBytes = sizeof(vertices_[0]);

#pragma endregion

}

void Model::indicesBuffGeneration()
{
#pragma region �C���f�b�N�X�o�b�t�@

	//���\�[�X�ݒ�
	resDesc_.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc_.Width = sizeIB_;//�C���f�b�N�X��񂪓��镪�̃T�C�Y
	resDesc_.Height = 1;
	resDesc_.DepthOrArraySize = 1;
	resDesc_.MipLevels = 1;
	resDesc_.SampleDesc.Count = 1;
	resDesc_.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//�C���f�b�N�X�o�b�t�@�̐���

	result_ = DirectXInit::GetInstance()->Getdev()->CreateCommittedResource(
		&heapprop_,//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resDesc_,//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff_)
	);

	//�C���f�b�N�X�o�b�t�@���}�b�s���O
	uint16_t* indexMap = nullptr;
	result_ = indexBuff_->Map(0, nullptr, (void**)&indexMap);
	//�S�C���f�b�N�X�ɑ΂���
	for (uint16_t i = 0; i < indices_.size(); i++)
	{
		indexMap[i] = indices_[i];//�C���f�b�N�X���R�s�[
	}

	//�}�b�s���O����
	indexBuff_->Unmap(0, nullptr);

#pragma endregion ���_��ʂ鏇�Ԃ����߂�ꏊ

#pragma region �C���f�b�N�X�o�b�t�@�r���[

	//�C���f�b�N�X�o�b�t�@�r���[�̐���

	ibView_.BufferLocation = indexBuff_->GetGPUVirtualAddress();
	ibView_.Format = DXGI_FORMAT_R16_UINT;
	ibView_.SizeInBytes = sizeIB_;

#pragma endregion

}

AnimationMesh::AnimationMesh()
{

}

AnimationMesh::~AnimationMesh()
{

}


void AnimationMesh::Init()
{

	//���_�o�b�t�@�����
	SetSizeVB();
	for (auto& itr : material_)
	{
		itr.VertexBuffObjSetting(D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_DIMENSION_BUFFER, sizeVB_, 1, 1, 1, 1, D3D12_TEXTURE_LAYOUT_ROW_MAJOR);
		VertexBuffObjGeneration(itr.heapprop_, D3D12_HEAP_FLAG_NONE, itr.resDesc_, D3D12_RESOURCE_STATE_GENERIC_READ);
		
	}
	VertexBuffMap();
	VertexBuffViewGeneration();

	//�C���f�b�N�X�o�b�t�@������

	SetSizeIB();
	for (auto& itr : material_)
	{
		itr.IndicesBuffSetting(D3D12_RESOURCE_DIMENSION_BUFFER, sizeIB_, 1, 1, 1, 1, D3D12_TEXTURE_LAYOUT_ROW_MAJOR);
		IndicesBuffGeneration(itr.heapprop_, D3D12_HEAP_FLAG_NONE, itr.resDesc_, D3D12_RESOURCE_STATE_GENERIC_READ);
	}
	IndicesBuffMap();
	IndicesBuffViewGeneration(DXGI_FORMAT_R16_UINT);
}

void AnimationMesh::SetSizeVB()
{
	sizeVB_ = static_cast<unsigned int>(sizeof(AnimationVertex) * vertices_.size());
}

void AnimationMesh::SetSizeIB()
{
	sizeIB_ = static_cast<unsigned int>(sizeof(unsigned short) * indices_.size());
}

void AnimationMesh::VertexBuffObjGeneration(const D3D12_HEAP_PROPERTIES& heapProp, D3D12_HEAP_FLAGS flag, const D3D12_RESOURCE_DESC resDesc, D3D12_RESOURCE_STATES state)
{


#pragma region ���_�o�b�t�@�̐���

	//���_�o�b�t�@�̐���

	result_ = DirectXInit::GetInstance()->Getdev()->CreateCommittedResource(
		&heapProp,//�q�[�v�ݒ�
		flag,
		&resDesc,//���\�[�X�ݒ�
		state,
		nullptr,
		IID_PPV_ARGS(&vertBuff_)
	);

#pragma endregion
}

void AnimationMesh::VertexBuffMap()
{
	//���_�o�b�t�@�ւ̃f�[�^�]��
	//GPU��̃o�b�t�@�ɑΉ��������z���������擾
	result_ = vertBuff_->Map(0, nullptr, (void**)&vertMap_);

	//�S���_�ɑ΂���
	for (uint16_t i = 0; i < vertices_.size(); i++)
	{
		vertMap_[i] = vertices_[i];//���W���R�s�[
	}


	//�Ȃ��������
	vertBuff_->Unmap(0, nullptr);
}

void AnimationMesh::VertexBuffViewGeneration()
{
	vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();
	vbView_.SizeInBytes = sizeVB_;
	vbView_.StrideInBytes = sizeof(vertices_[0]);
}				
					
void AnimationMesh::IndicesBuffGeneration(const D3D12_HEAP_PROPERTIES& heapProp, D3D12_HEAP_FLAGS flag, const D3D12_RESOURCE_DESC resDesc, D3D12_RESOURCE_STATES state)
{					

	result_ = DirectXInit::GetInstance()->Getdev()->CreateCommittedResource(
		&heapProp,//�q�[�v�ݒ�
		flag,
		&resDesc,//���\�[�X�ݒ�
		state,
		nullptr,
		IID_PPV_ARGS(&indexBuff_)
	);
}					
					
void AnimationMesh::IndicesBuffMap()
{					
	//�C���f�b�N�X�o�b�t�@���}�b�s���O
	uint16_t* indexMap = nullptr;
	result_ = indexBuff_->Map(0, nullptr, (void**)&indexMap);
	//�S�C���f�b�N�X�ɑ΂���
	for (uint16_t i = 0; i < indices_.size(); i++)
	{
		indexMap[i] = indices_[i];//�C���f�b�N�X���R�s�[
	}

	//�}�b�s���O����
	indexBuff_->Unmap(0, nullptr);
}					
					
void AnimationMesh::IndicesBuffViewGeneration(DXGI_FORMAT format)
{
	ibView_.BufferLocation = indexBuff_->GetGPUVirtualAddress();
	ibView_.Format = format;
	ibView_.SizeInBytes = sizeIB_;
}