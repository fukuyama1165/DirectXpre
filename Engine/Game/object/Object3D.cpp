#include "Object3D.h"
#include "DirectXInit.h"
#include "BaseCollider.h"

LightGroup* Object3D::SLightGroup_ = nullptr;

using namespace DirectX;


Object3D::Object3D()
{
	Scale_ = { 1,1,1 };
	Rotate_ = {};
	pos_ = {};

	matWorld_.IdentityMatrix();

}

Object3D::~Object3D()
{
	
}


void Object3D::Update()
{
	forward_ = { 0.0f,0.0f,1.0f };

	matWorldGeneration();

	forward_ = VectorMat(forward_, matWorld_);

	Camera now = *Camera::nowCamera;

	constTransformMatUpdata(now.eye_, now.matView_, now.matProjection_);

	

}

void Object3D::Draw(const std::string& ChangeTexure,const bool& PipeLineRuleFlag,const bool& ChangeSquareFlag)
{

	//プリミティブ形状(三角形リスト)
	DirectXInit::GetInstance()->GetcmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//パイプラインステートとルートシグネチャの設定
	//作ったパイプラインステートとルートシグネチャをセットする
	//決めたルールで描画をお願いするところ

	if (PipeLineRuleFlag)
	{
		DirectXInit::GetInstance()->GetcmdList().Get()->SetPipelineState(pipeline_.pipelinestate_.Get());
	}
	else
	{
		DirectXInit::GetInstance()->GetcmdList().Get()->SetPipelineState(gpipeline2_.pipelinestate_.Get());
	}

	DirectXInit::GetInstance()->GetcmdList().Get()->SetGraphicsRootSignature(rootsignature_.Get());

	//頂点バッファビューの設定
	DirectXInit::GetInstance()->GetcmdList().Get()->IASetVertexBuffers(0, 1, &vbView_);


	//定数バッファビュー(CBV)の設定コマンド
	/*cmdList->SetGraphicsRootConstantBufferView(0, constBuffMaterial->GetGPUVirtualAddress());
	cmdList->SetGraphicsRootConstantBufferView(1, constBuffMaterial2->GetGPUVirtualAddress());*/

	

	//インデックスバッファの設定
	DirectXInit::GetInstance()->GetcmdList().Get()->IASetIndexBuffer(&ibView_);

	//DirectXInit::GetInstance()->GetcmdList().Get()->IASetVertexBuffers(0, 1, &vbView_);

	//定数バッファビュー(CBV)の設定コマンド
	DirectXInit::GetInstance()->GetcmdList().Get()->SetGraphicsRootConstantBufferView(0, constBuffTransform_->GetGPUVirtualAddress());
	DirectXInit::GetInstance()->GetcmdList().Get()->SetGraphicsRootConstantBufferView(2, constBuffB1_->GetGPUVirtualAddress());
	
	SLightGroup_->Draw(3);

	if (materialTextureNum_ == "")
	{

		Texture::GetInstance()->Draw(ChangeTexure);
	}
	else
	{
		Texture::GetInstance()->Draw(materialTextureNum_);
	}

	//描画コマンド
	if (ChangeSquareFlag)
	{
		//四角形に描画
		DirectXInit::GetInstance()->GetcmdList().Get()->DrawIndexedInstanced((uint32_t)indices_.size(), 1, 0, 0, 0);
	}
	else
	{
		DirectXInit::GetInstance()->GetcmdList().Get()->DrawInstanced(3, 1, 0, 0);
	}

}

void Object3D::FBXDraw(const AnimationModel& model,const bool& PipeLineRuleFlag)
{


	//パイプラインステートとルートシグネチャの設定
	//作ったパイプラインステートとルートシグネチャをセットする
	//決めたルールで描画をお願いするところ

	//プリミティブ形状(三角形リスト)
	DirectXInit::GetInstance()->GetcmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	if (PipeLineRuleFlag)
	{
		DirectXInit::GetInstance()->GetcmdList().Get()->SetPipelineState(pipeline_.pipelinestate_.Get());
	}
	else
	{
		DirectXInit::GetInstance()->GetcmdList().Get()->SetPipelineState(gpipeline2_.pipelinestate_.Get());
	}


	DirectXInit::GetInstance()->GetcmdList().Get()->SetGraphicsRootSignature(rootsignature_.Get());

	

	//定数バッファビュー(CBV)の設定コマンド
	DirectXInit::GetInstance()->GetcmdList().Get()->SetGraphicsRootConstantBufferView(0, constBuffTransform_->GetGPUVirtualAddress());

	for (uint16_t i = 0; i < model.nodes_.size(); i++)
	{
		for (uint16_t j = 0; j < model.nodes_[i]->meshes_.size(); j++)
		{
			for (uint16_t m = 0; m < model.nodes_[i]->meshes_[j]->material_.size(); m++)
			{
				DirectXInit::GetInstance()->GetcmdList().Get()->SetGraphicsRootConstantBufferView(2, model.nodes_[i]->meshes_[j]->material_[m].constBuffB1_->GetGPUVirtualAddress());
			}
		}
	}

	SLightGroup_->Draw(3);

	model.Draw();
	
	//DirectXInit::GetInstance()->GetcmdList().Get()->SetGraphicsRootConstantBufferView(2, constBuffB1_->GetGPUVirtualAddress());

	

}

void Object3D::constTransformMatUpdata(const Vector3& eye, const Matrix4x4& matView, const Matrix4x4& matProjection)
{
	//constMapTransform->mat = matWorld * matView * matProjection;

	constMapTransform_->viewProj_ = matView;
	constMapTransform_->viewProj_ *= matProjection;
	constMapTransform_->world_ = matWorld_;
	constMapTransform_->cameraPos_ = {eye.x,eye.y,eye.z};
	constMapTransform_->color = color_;

}

void Object3D::SetScale(const Vector3& scale)
{
	Scale_ = scale;
}
void Object3D::SetRotate(const Vector3& rotate)
{
	Rotate_ = rotate;
}
void Object3D::SetPos(const Vector3& pos)
{
	pos_ = pos;
}

void Object3D::SetParent(Object3D* parent)
{
	if (parent != nullptr)
	{
		parent_ = parent;
	}
}

void Object3D::SetCollider(BaseCollider* Collider)
{

	Collider;

}

Vector3 Object3D::GetScale()
{
	return Scale_;
}
Vector3 Object3D::GetRotate()
{
	return Rotate_;
}
Vector3 Object3D::GetPos()
{
	return pos_;
}

Matrix4x4 Object3D::GetWorldMat()
{
	return matWorld_;
}

Object3D* Object3D::GetParent()
{
	return parent_;
}

Vector3 Object3D::GetWorldPos()
{
	Vector3 ans;

	ans.x = matWorld_.m[3][0];
	ans.y = matWorld_.m[3][1];
	ans.z = matWorld_.m[3][2];

	return ans;

}



void Object3D::matWorldGeneration()
{
	//スケール行列更新
	matScale_ = matScaleGeneration(Scale_);

	//回転行列更新
	if (!useQuaternion)
	{
		matRotate_ = QuaternionMatRotateGeneration(Rotate_);
	}
	else
	{
		matRotate_ = Quaternion::MakeRotateMatrix(Quaternion::Normalize(quaternionRot_));
	}

	//平行移動行列更新
	matTrans_ = matMoveGeneration(pos_);

	//ワールド行列更新
	matWorld_.IdentityMatrix();
	matWorld_ = matScale_ * matRotate_;

	if (billboardMode_ == BillboardMode::AllBillboard)
	{
		matWorld_*= Camera::nowCamera->matBillboard;
	}
	else if (billboardMode_ == BillboardMode::YBillboard)
	{
		matWorld_ *= Camera::nowCamera->matYBillboard;
	}

	matWorld_*= matTrans_;



	if (parent_ != nullptr)
	{
		matWorld_ *= parent_->GetWorldMat();
	}
}


std::string Object3D::loadTexture(const std::string& filename, std::string handle)
{
	return Texture::GetInstance()->loadTexture(filename,handle);
}

void Object3D::basicInit()
{

	
	name_ = typeid(*this).name();

	basicVertexInit();

	vertexBuffGeneration();

	vertexShaderGeneration();

	pixelShaderGeneration();

	vertexLayout();

	descriptorRangeGeneration();

	rootParamGeneration();

	textureSamplerGeneration();

	rootsignatureGeneration();

	constantBuffGeneration();


	indicesBuffGeneration();

}

void Object3D::colorChangeInit()
{

	name_ = typeid(*this).name();

	colorChangeVertexInit();

	vertexBuffGeneration();

	vertexShaderGeneration2();

	pixelShaderGeneration2();

	vertexLayout();

	descriptorRangeGeneration();

	rootParamGeneration();

	textureSamplerGeneration();

	rootsignatureGeneration();

	constantBuffGeneration();

	indicesBuffGeneration();

	//texture->Init(dev);

}

void Object3D::objDrawInit(const std::string& directoryPath, const char filename[], bool smoothing)
{

	name_ = typeid(*this).name();
	
	objVertexBuffGeneration(directoryPath, filename,smoothing);

	vertexShaderGeneration3();

	pixelShaderGeneration3();

	vertexLayout();

	descriptorRangeGeneration();

	rootParamGeneration();

	textureSamplerGeneration();

	rootsignatureGeneration();

	constantBuffGeneration();

	indicesBuffGeneration();	

}

void Object3D::FBXInit()
{

	name_ = typeid(*this).name();

	vertexShaderGeneration3();

	pixelShaderGeneration3();

	vertexLayout();

	descriptorRangeGeneration();

	rootParamGeneration();

	textureSamplerGeneration();

	rootsignatureGeneration();

	constantBuffFBXGeneration();

}

void Object3D::boarPolygonInit()
{

	name_ = typeid(*this).name();

	colorChangeVertexInit();

	vertexBuffGeneration();

	vertexShaderGeneration3();

	pixelShaderGeneration3();

	vertexLayout();

	descriptorRangeGeneration();

	rootParamGeneration();

	textureSamplerGeneration();

	rootsignatureGeneration();

	constantBuffGeneration();

	indicesBuffGeneration();

}

void Object3D::basicVertexInit()
{

	Vertex vert = {};

	//頂点データ(八点分の座標)
					//  x     y    z      u    v
	vert = { {-50.0f,50.0f,50.0f }, {}, { 0.0f,0.0f } };//左下前
	vertices_.emplace_back(vert);
	vert = { { -50.0f,-50.0f,50.0f },{}, {0.0f,1.0f} };//左上前
	vertices_.emplace_back(vert);
	vert = { {50.0f, 50.0f,50.0f }, {}, { 1.0f,0.0f } };//右下前
	vertices_.emplace_back(vert);
	vert = { {  50.0f,-50.0f,50.0f },{}, {1.0f,1.0f} };//右上前
	vertices_.emplace_back(vert);

	vert = { { -50.0f,50.0f,-50.0f },{},{0.0f,0.0f} };//左下後
	vertices_.emplace_back(vert);
	vert = { { -50.0f, -50.0f,-50.0f },{},{0.0f,1.0f} };//左上後
	vertices_.emplace_back(vert);
	vert = { {  50.0f,50.0f,-50.0f },{},{1.0f,0.0f} };//右下後
	vertices_.emplace_back(vert);
	vert = { {  50.0f, -50.0f,-50.0f },{},{1.0f,1.0f} };//右上後
	vertices_.emplace_back(vert);


	////インデックスデータ
	//前
	indices_.emplace_back(uint16_t(0));
	indices_.emplace_back(uint16_t(1));
	indices_.emplace_back(uint16_t(2));
	indices_.emplace_back(uint16_t(2));
	indices_.emplace_back(uint16_t(1));
	indices_.emplace_back(uint16_t(3));

	//後
	indices_.emplace_back(uint16_t(4));
	indices_.emplace_back(uint16_t(5));
	indices_.emplace_back(uint16_t(6));
	indices_.emplace_back(uint16_t(6));
	indices_.emplace_back(uint16_t(5));
	indices_.emplace_back(uint16_t(7));

	//左
	indices_.emplace_back(uint16_t(2));
	indices_.emplace_back(uint16_t(3));
	indices_.emplace_back(uint16_t(6));
	indices_.emplace_back(uint16_t(6));
	indices_.emplace_back(uint16_t(3));
	indices_.emplace_back(uint16_t(7));

	//右
	indices_.emplace_back(uint16_t(4));
	indices_.emplace_back(uint16_t(5));
	indices_.emplace_back(uint16_t(0));
	indices_.emplace_back(uint16_t(0));
	indices_.emplace_back(uint16_t(5));
	indices_.emplace_back(uint16_t(1));

	//上
	indices_.emplace_back(uint16_t(1));
	indices_.emplace_back(uint16_t(5));
	indices_.emplace_back(uint16_t(3));
	indices_.emplace_back(uint16_t(3));
	indices_.emplace_back(uint16_t(5));
	indices_.emplace_back(uint16_t(7));

	//下
	indices_.emplace_back(uint16_t(0));
	indices_.emplace_back(uint16_t(4));
	indices_.emplace_back(uint16_t(2));
	indices_.emplace_back(uint16_t(2));
	indices_.emplace_back(uint16_t(4));
	indices_.emplace_back(uint16_t(6));



	//頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
	sizeVB_ = static_cast<uint32_t>(sizeof(vertices_[0]) * vertices_.size());
	//インデックスデータ全体のサイズ
	sizeIB_ = static_cast<uint32_t>(sizeof(uint16_t) * indices_.size());

	////ビュー変換行列
	//eye_ = { 0, 0, -400 };//視点座標
	//target_ = { 0, 0, 0 };//注視点座標
	//up_ = { 0, 1, 0 };//上方向ベクトル

}
void Object3D::colorChangeVertexInit()
{
	Vertex vert = {};

	//頂点データ(四点分の座標)
					//  x     y    z      u    v
	vert = { {-1.0f,-1.0f,0.0f }, {}, { 0.0f,1.0f } };//左下
	vertices_.emplace_back(vert);
	vert = { {  1.0f,-1.0f,0.0f },{}, {1.0f,1.0f} };//左上
	vertices_.emplace_back(vert);
	vert = { {-1.0f, 1.0f,0.0f }, {}, { 0.0f,0.0f } };//右下
	vertices_.emplace_back(vert);
	vert = { {  1.0f, 1.0f,0.0f },{}, {1.0f,0.0f} };//右上
	vertices_.emplace_back(vert);

	////インデックスデータ
	indices_.emplace_back(uint16_t(0));
	indices_.emplace_back(uint16_t(1));
	indices_.emplace_back(uint16_t(2));
	indices_.emplace_back(uint16_t(2));
	indices_.emplace_back(uint16_t(1));
	indices_.emplace_back(uint16_t(3));

	//頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
	sizeVB_ = static_cast<uint32_t>(sizeof(vertices_[0]) * vertices_.size());
	//インデックスデータ全体のサイズ
	sizeIB_ = static_cast<uint32_t>(sizeof(uint16_t) * indices_.size());

	////ビュー変換行列
	//eye_ = { 0, 0, -400 };//視点座標
	//target_ = { 0, 0, 0 };//注視点座標
	//up_ = { 0, 1, 0 };//上方向ベクトル

}

void Object3D::vertexBuffGeneration()
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

#pragma region 法線ベクトル計算

	for (uint32_t i = 0; i < indices_.size() / 3; i++)
	{//三角形1つごとに計算していく

		//三角形のインデックスを取り出して,一時的な変数に入れる
		unsigned short indices0 = indices_[i * 3 + 0];
		unsigned short indices1 = indices_[i * 3 + 1];
		unsigned short indices2 = indices_[i * 3 + 2];

		XMFLOAT3 vert0(vertices_[indices0].pos_.x, vertices_[indices0].pos_.y, vertices_[indices0].pos_.z);
		XMFLOAT3 vert1(vertices_[indices1].pos_.x, vertices_[indices1].pos_.y, vertices_[indices1].pos_.z);
		XMFLOAT3 vert2(vertices_[indices2].pos_.x, vertices_[indices2].pos_.y, vertices_[indices2].pos_.z);

		//三角形を構成する頂点座標をベクトルに代入
		XMVECTOR p0 = XMLoadFloat3(&vert0);
		XMVECTOR p1 = XMLoadFloat3(&vert1);
		XMVECTOR p2 = XMLoadFloat3(&vert2);

		//p0→p1ベクトル、p0→p2ベクトルを計算(ベクトルの減算)
		XMVECTOR v1 = XMVectorSubtract(p1, p0);
		XMVECTOR v2 = XMVectorSubtract(p2, p0);

		//外積は両方から垂直なベクトル
		XMVECTOR normal = XMVector3Cross(v1, v2);

		//正規化(長さを1にする)
		normal = XMVector3Normalize(normal);

		//求めた法線を頂点データに代入
		
		XMStoreFloat3(&vertices_[indices0].normal_, normal);
		XMStoreFloat3(&vertices_[indices1].normal_, normal);
		XMStoreFloat3(&vertices_[indices2].normal_, normal);

	}

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

void Object3D::vertexBuffObjGeneration()
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

void Object3D::objVertexBuffGeneration(const std::string& directoryPath, const char filename[],bool smoothing)
{
#pragma region OBj読み込み

	std::vector<XMFLOAT3> positions;
	std::vector<XMFLOAT3> normals;
	std::vector<XMFLOAT2> texcoords;

	std::string path = directoryPath;
	std::string fileName = filename;
	std::string filepath = path + fileName;

	//ファイルストリーム
	std::ifstream file;

	//0bjファイルを開く
	file.open(filepath);

	//ファイルオープン失敗をチェック
	if (file.fail())
	{
		assert(0);
	}

	std::string line;
	while (std::getline(file, line))
	{

		//1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream lineStream(line);

		//半角スペース区切りで行の先頭文字列を取得
		std::string key;
		std::getline(lineStream, key, ' ');

		//先頭文字列がvなら頂点座標
		if (key == "v")
		{

			//x,y,z座標読み込み
			XMFLOAT3 position{};
			lineStream >> position.x;
			lineStream >> position.y;
			lineStream >> position.z;

			//座標データに追加
			positions.emplace_back(position);


		}

		//先頭文字列がvtならテクスチャ
		if (key == "vt")
		{

			//U,V成分読み込み
			XMFLOAT2 texcoord{};
			lineStream >> texcoord.x;
			lineStream >> texcoord.y;

			//V方向反転
			texcoord.y = 1.0f - texcoord.y;

			//テクスチャ座標データに追加
			texcoords.emplace_back(texcoord);

		}

		//先頭文字列がvnなら法線ベクトル
		if (key == "vn")
		{

			//x,y,z座標読み込み
			XMFLOAT3 normal{};
			lineStream >> normal.x;
			lineStream >> normal.y;
			lineStream >> normal.z;

			//座標データに追加
			normals.emplace_back(normal);


		}

		//先頭文字列がfならポリゴン(三角形)
		if (key == "f")
		{

			//半角スペース区切りで行の続きを読み込む
			std::string indexString;

			while (std::getline(lineStream, indexString, ' '))
			{

				//頂点インデックス1個分の文字列をストリームに変換して解析しやすくする
				std::istringstream indexStream(indexString);

				unsigned short indexPosition;
				unsigned short indexNormal;
				unsigned short indexTexcoord;
				indexStream >> indexPosition;

				//スラッシュを飛ばす
				indexStream.seekg(1, std::ios_base::cur);
				indexStream >> indexTexcoord;

				//スラッシュを飛ばす
				indexStream.seekg(1, std::ios_base::cur);
				indexStream >> indexNormal;

				//頂点データの追加
				Vertex vertex{};
				vertex.pos_ = positions[indexPosition - 1];
				vertex.normal_ = normals[indexNormal - 1];
				vertex.uv_ = texcoords[indexTexcoord - 1];
				vertices_.emplace_back(vertex);

				if (smoothing)
				{
					smoothData_[indexPosition].emplace_back(static_cast<unsigned short>(vertices_.size() - 1));
				}

				indices_.emplace_back(static_cast<unsigned short>(indices_.size()));

			}

		}

		//先頭文字列ががmtllibならマテリアル
		if (key == "mtllib")
		{

			//マテリアルのファイル名読み込み
			std::string materialName;
			lineStream >> materialName;
			std::string materialPath = path + materialName;

			//マテリアル読み込み
			loadMaterial(materialPath, path);


		}

	}

	file.close();

	if (smoothing)
	{

		for (auto itr = smoothData_.begin(); itr != smoothData_.end(); itr++)
		{
			//各面用の共通頂点コレクション
			std::vector<unsigned short>& v = itr->second;

			//全頂点の法線を平均する
			XMVECTOR normal = {};
			for (unsigned short index : v)
			{
				normal += XMVectorSet(vertices_[index].normal_.x, vertices_[index].normal_.y, vertices_[index].normal_.z, 0);
				

			}
			normal = XMVector3Normalize(normal / (float)v.size());

			//共通法線を使用する全ての頂点データに書き込む
			for (unsigned short index : v)
			{

				vertices_[index].normal_ = { normal.m128_f32[0],normal.m128_f32[1],normal.m128_f32[2] };



			}


		}

	}

	//頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
	sizeVB_ = static_cast<uint32_t>(sizeof(vertices_[0]) * vertices_.size());
	//インデックスデータ全体のサイズ
	sizeIB_ = static_cast<uint32_t>(sizeof(uint16_t) * indices_.size());

#pragma endregion

	vertexBuffObjGeneration();
}

void Object3D::vertexShaderGeneration()
{
	vs = vs.ShaderLoad("BasicVS", "Resources/shaders/BasicVS.hlsl", "main", "vs_5_0");
}

void Object3D::vertexShaderGeneration2()
{
	vs = vs.ShaderLoad("vertexMoveVS", "Resources/shaders/vertexMoveVS.hlsl", "main", "vs_5_0");
}

void Object3D::vertexShaderGeneration3()
{
	vs= vs.ShaderLoad("OBJVS", "Resources/shaders/OBJVS.hlsl", "main", "vs_5_0");
}

void Object3D::vertexShaderGeneration4()
{
	vs = vs.ShaderLoad("Phong_toon_rim_VS", "Resources/shaders/Phong_toon_rim_VS.hlsl", "main", "vs_5_0");
}

void Object3D::vertexShaderFBXGeneration()
{
	vs = vs.ShaderLoad("OBJVS", "Resources/shaders/OBJVS.hlsl", "main", "vs_5_0");
}

void Object3D::pixelShaderGeneration()
{
	ps = ps.ShaderLoad("BasicPS", "Resources/shaders/BasicPS.hlsl", "main", "ps_5_0");
}

void Object3D::pixelShaderGeneration2()
{
	ps = ps.ShaderLoad("colorChangePS", "Resources/shaders/colorChangePS.hlsl", "main", "ps_5_0");
}

void Object3D::pixelShaderGeneration3()
{
	ps = ps.ShaderLoad("OBJPS", "Resources/shaders/OBJPS.hlsl", "main", "ps_5_0");
}

void Object3D::pixelShaderGeneration4()
{
	ps = ps.ShaderLoad("colorChangePS", "Resources/shaders/colorChangePS.hlsl", "main", "ps_5_0");
}


void Object3D::pixelShaderFBXGeneration()
{
	ps = ps.ShaderLoad("colorChangePS", "Resources/shaders/colorChangePS.hlsl", "main", "ps_5_0");
}

void Object3D::vertexLayout()
{
#pragma region 頂点レイアウトの設定

	//頂点レイアウト
	//グラフィックパイプラインで頂点一つ分のデータに何を持たせるか決める
	//これは最低限の3D座標だけ持たせているらしい
	D3D12_INPUT_ELEMENT_DESC newInputLayout;
	newInputLayout =
	{
		"POSITION",//セマンティック名
		0,//同じセマンティック名が複数あるときのインデックス
		DXGI_FORMAT_R32G32B32_FLOAT,//要素数とビット数を返す(XYZの３つでfloat型なのでR32G32B32_FLOAT)
		0,//入力スロットインデックス
		D3D12_APPEND_ALIGNED_ELEMENT,//データのオフセット値(D3D12_APPEND_ALIGNED_ELEMENTだと自動設定)
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,//入力データ識別(標準はD3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA)
		0,//一度に描画するインスタンス数
	};
	inputLayout_.push_back(newInputLayout);
	//座標以外に　色、テクスチャUVなどを渡す場合はさらに続ける
	newInputLayout =
	{//法線ベクトル
		"NORMAL",
		0,
		DXGI_FORMAT_R32G32B32_FLOAT,
		0,
		D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
		0
	};

	inputLayout_.push_back(newInputLayout);

	newInputLayout =
	{//UV座標
		"TEXCOORD",
		0,
		DXGI_FORMAT_R32G32_FLOAT,
		0,
		D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
		0
	};

	inputLayout_.push_back(newInputLayout);

#pragma endregion
}

void Object3D::descriptorRangeGeneration()
{
#pragma region デスクリプタレンジの設定
	descriptorRange_.NumDescriptors = 1;//一度の描画に使うテクスチャが１枚なので1
	descriptorRange_.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange_.BaseShaderRegister = 0;//テクスチャレジスタ0番
	descriptorRange_.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

#pragma endregion
}

void Object3D::rootParamGeneration()
{
#pragma region ルートパラメータ

	//ルートパラメータの設定

	//色
	//rootParam[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;//定数バッファビュー
	//rootParam[0].Descriptor.ShaderRegister = 0;//定数バッファ番号
	//rootParam[0].Descriptor.RegisterSpace = 0;//デフォルト値
	//rootParam[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//全てのシェーダーから見える

	////位置
	//rootParam[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;//定数バッファビュー
	//rootParam[1].Descriptor.ShaderRegister = 1;//定数バッファ番号
	//rootParam[1].Descriptor.RegisterSpace = 0;//デフォルト値
	//rootParam[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//全てのシェーダーから見える

	D3D12_ROOT_PARAMETER newRootParam{};

	newRootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;//定数バッファビュー
	newRootParam.Descriptor.ShaderRegister = 0;//定数バッファ番号
	newRootParam.Descriptor.RegisterSpace = 0;//デフォルト値
	newRootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//全てのシェーダーから見える

	rootParam_.push_back(newRootParam);

	//画像データ用
	newRootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//種類
	newRootParam.DescriptorTable.pDescriptorRanges = &descriptorRange_;//デスクリプタレンジ
	newRootParam.DescriptorTable.NumDescriptorRanges = 1;//デスクリプタレンジ数
	newRootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//全てのシェーダから見える

	rootParam_.push_back(newRootParam);

	newRootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;//定数バッファビュー
	newRootParam.Descriptor.ShaderRegister = 1;//定数バッファ番号
	newRootParam.Descriptor.RegisterSpace = 0;//デフォルト値
	newRootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//全てのシェーダーから見える

	rootParam_.push_back(newRootParam);

	newRootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;//定数バッファビュー
	newRootParam.Descriptor.ShaderRegister = 2;//定数バッファ番号
	newRootParam.Descriptor.RegisterSpace = 0;//デフォルト値
	newRootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//全てのシェーダーから見える

	rootParam_.push_back(newRootParam);


#pragma endregion 定数バッファを増やしたら増やすところがある
}

void Object3D::textureSamplerGeneration()
{
#pragma region テクスチャサンプラーの設定

	//テクスチャサンプラーの設定
	sampleDesc_.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;//横繰り返し(タイリング)
	sampleDesc_.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;//縦繰り返し(タイリング)
	sampleDesc_.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;//奥行繰り返し(タイリング)
	sampleDesc_.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;//ボーダーの時は黒
	sampleDesc_.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;//全てリニア補間
	sampleDesc_.MaxLOD = D3D12_FLOAT32_MAX;//ミップマップ最大値
	sampleDesc_.MinLOD = 0.0f;//ミップマップ最小値
	sampleDesc_.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	sampleDesc_.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;//ピクセルシェーダからのみ使用可能

#pragma endregion
}

void Object3D::rootsignatureGeneration()
{
#pragma region ルートシグネチャ設定

	//ルートシグネチャの設定
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParam_.data();//ルートパラメータの先頭アドレス
	rootSignatureDesc.NumParameters = (uint32_t)rootParam_.size();//ルートパラメータ数
	rootSignatureDesc.pStaticSamplers = &sampleDesc_;
	rootSignatureDesc.NumStaticSamplers = 1;

	//ルートシグネチャの生成
	Microsoft::WRL::ComPtr<ID3DBlob> rootSigBlob = nullptr;
	result_ = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob_);
	result_ = DirectXInit::GetInstance()->Getdev()->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootsignature_));

	PipeLineSeting seting1 = PipeLine::defCreatePipeLineSeting(vs, ps, inputLayout_, rootsignature_.Get());
	PipeLineSeting seting2 = PipeLine::defCreatePipeLineSeting(vs, ps, inputLayout_, rootsignature_.Get());

	
	seting2.rasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;

	pipeline_ = PipeLine::CreatePipeLine("Def"+vs.name_+ps.name_+"ObjPipeLine",seting1);
	gpipeline2_ = gpipeline2_.CreatePipeLine("Wireframe" + vs.name_ + ps.name_ + "ObjPipeLine",seting2);

	

#pragma endregion 定数バッファを増やしたらルートパラメータを書き換えパラメータ数を書き換える

}

void Object3D::constantBuffGeneration()
{
#pragma region 定数バッファ

	//定数バッファの生成用の設定

	cbHeapProp_.Type = D3D12_HEAP_TYPE_UPLOAD;//GPUへの転送用


	cbResourceDesc_ = constBuffResourceGeneration(sizeof(ConstBufferDataTransform));

	result_ = DirectXInit::GetInstance()->Getdev()->CreateCommittedResource(
		&cbHeapProp_,
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc_,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffTransform_));
	assert(SUCCEEDED(result_));

	result_ = constBuffTransform_->Map(0, nullptr, (void**)&constMapTransform_);
	assert(SUCCEEDED(result_));

	constMapTransform_->color = { 1,1,1,1 };

	cbResourceDesc_ = constBuffResourceGeneration(sizeof(ConstBufferDataB1));

	result_ = DirectXInit::GetInstance()->Getdev()->CreateCommittedResource(
		&cbHeapProp_,
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc_,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffB1_));
	assert(SUCCEEDED(result_));

	result_ = constBuffB1_->Map(0, nullptr, (void**)&constMapB1_);
	assert(SUCCEEDED(result_));
	constMapB1_->ambient_ = material_.ambient_;
	constMapB1_->diffuse_ = material_.diffuse_;
	constMapB1_->specular_ = material_.specular_;
	constMapB1_->alpha_ = material_.alpha_;


	////定数バッファのマッピング
	//

	//マッピングをするとGPUのVRSMがCPUと連動する
	//Unmapをするとつながりが解除されるが定数バッファは書き換えることが多いので
	//しなくても大丈夫

	//値を書き込むと自動的に転送される

#pragma endregion
}

void Object3D::constantBuffFBXGeneration()
{
#pragma region 定数バッファ

	//定数バッファの生成用の設定

	cbHeapProp_.Type = D3D12_HEAP_TYPE_UPLOAD;//GPUへの転送用

	cbResourceDesc_ = constBuffResourceGeneration(sizeof(ConstBufferDataTransform));

	result_ = DirectXInit::GetInstance()->Getdev()->CreateCommittedResource(
		&cbHeapProp_,
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc_,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffTransform_));
	assert(SUCCEEDED(result_));

	result_ = constBuffTransform_->Map(0, nullptr, (void**)&constMapTransform_);
	assert(SUCCEEDED(result_));

	constMapTransform_->color = { 1,1,1,1 };

	if (material_.name_ != "")
	{
		cbResourceDesc_ = constBuffResourceGeneration(sizeof(ConstBufferDataB1));

		result_ = DirectXInit::GetInstance()->Getdev()->CreateCommittedResource(
			&cbHeapProp_,
			D3D12_HEAP_FLAG_NONE,
			&cbResourceDesc_,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&constBuffB1_));
		assert(SUCCEEDED(result_));

		result_ = constBuffB1_->Map(0, nullptr, (void**)&constMapB1_);
		assert(SUCCEEDED(result_));
		constMapB1_->ambient_ = material_.ambient_;
		constMapB1_->diffuse_ = material_.diffuse_;
		constMapB1_->specular_ = material_.specular_;
		constMapB1_->alpha_ = material_.alpha_;
	}

#pragma endregion
}

D3D12_RESOURCE_DESC Object3D::constBuffResourceGeneration(uint32_t size)
{
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (size + 0xff) & ~0xff;//256バイトアラインメント
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	return cbResourceDesc;
}

void Object3D::indicesBuffGeneration()
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





void Object3D::deleteTexture()
{
	
}

void Object3D::loadMaterial(const std::string& filename, const std::string& directoryPath)
{

	//ファイルストリーム
	std::ifstream file;

	//マテリアルファイルを開く
	file.open(filename);

	//ファイルオープン失敗をチェック
	if (file.fail())
	{
		assert(0);
	}

	//1行ずつ読み込む
	std::string line;
	while (std::getline(file, line))
	{

		//1行分の文字列をストリームに変換
		std::istringstream lineStream(line);

		//半角スペース区切りで行の先頭文字列を取得
		std::string key;
		std::getline(lineStream, key, ' ');

		//先頭のタブ文字は無視する
		if (key[0] == '\t')
		{
			key.erase(key.begin());
		}

		//先頭文字列がnewmtlならマテリアル名
		if (key == "newmtl")
		{

			lineStream >> material_.name_;

		}

		//先頭文字列がKaならアンビエント色
		if (key == "Ka")
		{

			lineStream >> material_.ambient_.x;
			lineStream >> material_.ambient_.y;
			lineStream >> material_.ambient_.z;

		}

		//先頭文字列がKdならディフューズ色
		if (key == "Kd")
		{

			lineStream >> material_.diffuse_.x;
			lineStream >> material_.diffuse_.y;
			lineStream >> material_.diffuse_.z;

		}

		//先頭文字列がKsならスペキュラー色
		if (key == "Ks")
		{

			lineStream >> material_.specular_.x;
			lineStream >> material_.specular_.y;
			lineStream >> material_.specular_.z;

		}
		//先頭文字列がmap_Kdならテクスチャファイル名
		if (key == "map_Kd")
		{
			//テクスチャのファイル名読み込み
			lineStream >> material_.textureFilename_;
			std::string texpath = directoryPath + material_.textureFilename_;

			//テクスチャ読み込み
			LoadMaterialTexture(texpath, material_.textureFilename_);
		}

	}

	//ファイルを閉じる
	file.close();

}

bool Object3D::LoadMaterialTexture(const std::string& filename, std::string handle)
{

	materialTextureNum_ = Texture::GetInstance()->loadTexture(filename, handle);
	return true;

}