#include "Object3D.h"

Object3D::Object3D()
{
	Scale_ = { 1,1,1 };
	Rotate_ = {};
	Trans_ = {};

	matWorld.IdentityMatrix();
}

Object3D::~Object3D()
{
}

void Object3D::Init(ID3D12Device* dev)
{
	HRESULT result;

	//�q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;//GPU�ւ̓]���p

	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferDataTransform) + 0xff) & ~0xff;//256�o�C�g�A���C�������g
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

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

}

void Object3D::Update(Matrix4x4& matView, Matrix4x4& matProjection)
{
	//�X�P�[���s��X�V
	matScale = matScaleGeneration(Scale_);

	//��]�s��X�V
	matRotate = matRotateGeneration(Rotate_);

	//���s�ړ��s��X�V
	matTrans = matMoveGeneration(Trans_);

	//���[���h�s��X�V
	matWorld.IdentityMatrix();
	matWorld *= matScale*matRotate*matTrans;

	

	if (parent_ != nullptr)
	{
		matWorld *= parent_->GetWorldMat();
	}

	constTransformMatUpdata(matView,matProjection);
}

void Object3D::Draw(ID3D12GraphicsCommandList* cmdList, D3D12_VERTEX_BUFFER_VIEW& vbView, D3D12_INDEX_BUFFER_VIEW& idView, UINT numIndices,bool ChangeSquareFlag)
{
	//���_�o�b�t�@�̐ݒ�
	cmdList->IASetVertexBuffers(0, 1, &vbView);

	//�C���f�b�N�X�o�b�t�@�̐ݒ�
	cmdList->IASetIndexBuffer(&idView);

	//�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
	cmdList->SetGraphicsRootConstantBufferView(3, constBuffTransform->GetGPUVirtualAddress());

	//�`��R�}���h
	if (ChangeSquareFlag)
	{
		//�l�p�`�ɕ`��
		cmdList->DrawIndexedInstanced(numIndices, 1, 0, 0, 0);
	}
	else
	{
		cmdList->DrawInstanced(3, 1, 0, 0);
	}


}

void Object3D::constTransformMatUpdata(Matrix4x4& matView, Matrix4x4& matProjection)
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

	matRotateX *= matRotateY;

	matRotateZ *= matRotateX;

	matRotate = matRotateZ;

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