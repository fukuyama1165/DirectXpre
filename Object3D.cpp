#include "Object3D.h"

Object3D::Object3D()
{
	Scale_ = { 1,1,1 };
	Rotate_ = {};
	Trans_ = {};

	matWorld = XMMatrixIdentity();
}

Object3D::~Object3D()
{
}

void Object3D::Init(ID3D12Device* dev)
{
	HRESULT result;

	//ヒープ設定
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;//GPUへの転送用

	//リソース設定
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferDataTransform) + 0xff) & ~0xff;//256バイトアラインメント
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

void Object3D::Update(XMMATRIX& matView, XMMATRIX& matProjection)
{
	////スケール行列更新
	//matScale = matScaleGeneration(Scale_);

	////回転行列更新
	//matRotate = matRotateGeneration(Rotate_);

	////平行移動行列更新
	//matTrans = matMoveGeneration(Trans_);

	////ワールド行列更新
	//matWorld.IdentityMatrix();
	//matWorld *= matScale*matRotate*matTrans;

	// スケール行列更新
	matScale = XMMatrixScaling(Scale_.x, Scale_.y, Scale_.z);

	//回転行列更新
	matRotate = XMMatrixIdentity();

	matRotate *= XMMatrixRotationZ(Rotate_.z);
	matRotate *= XMMatrixRotationX(Rotate_.x);
	matRotate *= XMMatrixRotationY(Rotate_.y);

	//平行移動行列更新
	matTrans = XMMatrixTranslation(Trans_.x, Trans_.y, Trans_.z);

	//ワールド行列更新
	matWorld = XMMatrixIdentity();
	matWorld *= matScale;
	matWorld *= matRotate;
	matWorld *= matTrans;

	if (parent_ != nullptr)
	{
		matWorld *= parent_->GetWorldMat();
	}

	constTransformMatUpdata(matView,matProjection);
}

void Object3D::Draw(ID3D12GraphicsCommandList* cmdList, D3D12_VERTEX_BUFFER_VIEW& vbView, D3D12_INDEX_BUFFER_VIEW& idView, UINT numIndices,bool ChangeSquareFlag)
{
	//頂点バッファの設定
	cmdList->IASetVertexBuffers(0, 1, &vbView);

	//インデックスバッファの設定
	cmdList->IASetIndexBuffer(&idView);

	//定数バッファビュー(CBV)の設定コマンド
	cmdList->SetGraphicsRootConstantBufferView(3, constBuffTransform->GetGPUVirtualAddress());

	//描画コマンド
	if (ChangeSquareFlag)
	{
		//四角形に描画
		cmdList->DrawIndexedInstanced(numIndices, 1, 0, 0, 0);
	}
	else
	{
		cmdList->DrawInstanced(3, 1, 0, 0);
	}


}

void Object3D::constTransformMatUpdata(XMMATRIX& matView, XMMATRIX& matProjection)
{
	constMapTransform->mat = matWorld * matView * matProjection;
}

void Object3D::SetScale(XMFLOAT3 scale)
{
	Scale_ = scale;
}
void Object3D::SetRotate(XMFLOAT3 rotate)
{
	Rotate_ = rotate;
}
void Object3D::SetPos(XMFLOAT3 pos)
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

XMFLOAT3 Object3D::GetScale()
{
	return Scale_;
}
XMFLOAT3 Object3D::GetRotate()
{
	return Rotate_;
}
XMFLOAT3 Object3D::GetPos()
{
	return Trans_;
}

XMMATRIX Object3D::GetWorldMat()
{
	return matWorld;
}

Object3D* Object3D::GetParent()
{
	return parent_;
}