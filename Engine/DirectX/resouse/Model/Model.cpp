#include "Model.h"

Model::Model()
{
}

Model::~Model()
{
}


void Model::vertexBuffObjGeneration()
{
#pragma region 頂点バッファの確保

	//頂点バッファの確保辺

	heapprop_.Type = D3D12_HEAP_TYPE_UPLOAD;//GPUへの転送用


	resDesc_.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc_.Width = sizeVB_;//頂点データ全体のサイズ
	resDesc_.Height = 1;
	resDesc_.DepthOrArraySize = 1;
	resDesc_.MipLevels = 1;
	resDesc_.SampleDesc.Count = 1;
	resDesc_.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

#pragma endregion

#pragma region 頂点バッファの生成

	//頂点バッファの生成

	result_ = DirectXInit::GetInstance()->Getdev()->CreateCommittedResource(
		&heapprop_,//ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resDesc_,//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff_)
	);

#pragma endregion


#pragma region 頂点バッファへのデータ転送

	//頂点バッファへのデータ転送
	//GPU上のバッファに対応した仮想メモリを取得
	result_ = vertBuff_->Map(0, nullptr, (void**)&vertMap_);

	//全頂点に対して
	for (uint16_t i = 0; i < vertices_.size(); i++)
	{
		vertMap_[i] = vertices_[i];//座標をコピー
	}


	//つながりを解除
	vertBuff_->Unmap(0, nullptr);

#pragma endregion

#pragma region 頂点バッファビューの生成

	//頂点バッファビューの作成(GPUへの道しるべ)辺

	//頂点バッファビューの作成

	vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();
	vbView_.SizeInBytes = sizeVB_;
	vbView_.StrideInBytes = sizeof(vertices_[0]);

#pragma endregion

}

void Model::indicesBuffGeneration()
{
#pragma region インデックスバッファ

	//リソース設定
	resDesc_.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc_.Width = sizeIB_;//インデックス情報が入る分のサイズ
	resDesc_.Height = 1;
	resDesc_.DepthOrArraySize = 1;
	resDesc_.MipLevels = 1;
	resDesc_.SampleDesc.Count = 1;
	resDesc_.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//インデックスバッファの生成

	result_ = DirectXInit::GetInstance()->Getdev()->CreateCommittedResource(
		&heapprop_,//ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resDesc_,//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff_)
	);

	//インデックスバッファをマッピング
	uint16_t* indexMap = nullptr;
	result_ = indexBuff_->Map(0, nullptr, (void**)&indexMap);
	//全インデックスに対して
	for (uint16_t i = 0; i < indices_.size(); i++)
	{
		indexMap[i] = indices_[i];//インデックスをコピー
	}

	//マッピング解除
	indexBuff_->Unmap(0, nullptr);

#pragma endregion 頂点を通る順番を決める場所

#pragma region インデックスバッファビュー

	//インデックスバッファビューの生成

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

	//頂点バッファを作る
	SetSizeVB();
	for (auto& itr : material_)
	{
		itr.VertexBuffObjSetting(D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_DIMENSION_BUFFER, sizeVB_, 1, 1, 1, 1, D3D12_TEXTURE_LAYOUT_ROW_MAJOR);
		VertexBuffObjGeneration(itr.heapprop_, D3D12_HEAP_FLAG_NONE, itr.resDesc_, D3D12_RESOURCE_STATE_GENERIC_READ);
		
	}
	VertexBuffMap();
	VertexBuffViewGeneration();

	//インデックスバッファをつくる

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


#pragma region 頂点バッファの生成

	//頂点バッファの生成

	result_ = DirectXInit::GetInstance()->Getdev()->CreateCommittedResource(
		&heapProp,//ヒープ設定
		flag,
		&resDesc,//リソース設定
		state,
		nullptr,
		IID_PPV_ARGS(&vertBuff_)
	);

#pragma endregion
}

void AnimationMesh::VertexBuffMap()
{
	//頂点バッファへのデータ転送
	//GPU上のバッファに対応した仮想メモリを取得
	result_ = vertBuff_->Map(0, nullptr, (void**)&vertMap_);

	//全頂点に対して
	for (uint16_t i = 0; i < vertices_.size(); i++)
	{
		vertMap_[i] = vertices_[i];//座標をコピー
	}


	//つながりを解除
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
		&heapProp,//ヒープ設定
		flag,
		&resDesc,//リソース設定
		state,
		nullptr,
		IID_PPV_ARGS(&indexBuff_)
	);
}					
					
void AnimationMesh::IndicesBuffMap()
{					
	//インデックスバッファをマッピング
	uint16_t* indexMap = nullptr;
	result_ = indexBuff_->Map(0, nullptr, (void**)&indexMap);
	//全インデックスに対して
	for (uint16_t i = 0; i < indices_.size(); i++)
	{
		indexMap[i] = indices_[i];//インデックスをコピー
	}

	//マッピング解除
	indexBuff_->Unmap(0, nullptr);
}					
					
void AnimationMesh::IndicesBuffViewGeneration(DXGI_FORMAT format)
{
	ibView_.BufferLocation = indexBuff_->GetGPUVirtualAddress();
	ibView_.Format = format;
	ibView_.SizeInBytes = sizeIB_;
}