#include "Object3D.h"
#include "DirectXInit.h"
#include "BaseCollider.h"

Texture* Object3D::texture_ = nullptr;

float Object3D::SWin_width_ =1280;
float Object3D::SWin_height_ =720;

LightGroup* Object3D::lightGroup_ = nullptr;

//float PI = 3.141592653589f;

//Object3D::Material Object3D::material;

Object3D::Object3D()
{
	Scale_ = { 1,1,1 };
	Rotate_ = {};
	Trans_ = {};

	matWorld_.IdentityMatrix();

}

Object3D::~Object3D()
{
	if (collider_)
	{
		CollisionManager::GetInstance()->RemoveCollider(collider_);
		delete collider_;
	}
}


void Object3D::Update(const Camera& camera)
{
	forward_ = { 0.0f,0.0f,1.0f };

	forward_ = VectorMat(forward_, matWorld_);

	matWorldGeneration();

	//matProjection = perspectiveProjectionGeneration((45.0f * (PI / 180)), 0.1f, 1000.0f);

	constTransformMatUpdata(camera.eye_,camera.matView_, camera.matProjection_);

	if (collider_)
	{
		collider_->Update();
	}

}

void Object3D::Draw(const uint32_t& ChangeTexure,const bool& PipeLineRuleFlag,const bool& ChangeSquareFlag)
{

	

	//パイプラインステートとルートシグネチャの設定
	//作ったパイプラインステートとルートシグネチャをセットする
	//決めたルールで描画をお願いするところ

	if (PipeLineRuleFlag)
	{
		DirectXInit::GetInstance()->GetcmdList().Get()->SetPipelineState(pipelinestate_.Get());
	}
	else
	{
		DirectXInit::GetInstance()->GetcmdList().Get()->SetPipelineState(pipelinestate2_.Get());
	}

	DirectXInit::GetInstance()->GetcmdList().Get()->SetGraphicsRootSignature(rootsignature_.Get());

	//頂点バッファビューの設定
	DirectXInit::GetInstance()->GetcmdList().Get()->IASetVertexBuffers(0, 1, &vbView_);


	//定数バッファビュー(CBV)の設定コマンド
	/*cmdList->SetGraphicsRootConstantBufferView(0, constBuffMaterial->GetGPUVirtualAddress());
	cmdList->SetGraphicsRootConstantBufferView(1, constBuffMaterial2->GetGPUVirtualAddress());*/

	if (materialTextureNum_ == uint32_t( - 1))
	{

		texture_->Draw(ChangeTexure);
	}
	else
	{
		texture_->Draw(materialTextureNum_);
	}

	//インデックスバッファの設定
	DirectXInit::GetInstance()->GetcmdList().Get()->IASetIndexBuffer(&ibView_);

	DirectXInit::GetInstance()->GetcmdList().Get()->IASetVertexBuffers(0, 1, &vbView_);

	//定数バッファビュー(CBV)の設定コマンド
	DirectXInit::GetInstance()->GetcmdList().Get()->SetGraphicsRootConstantBufferView(0, constBuffTransform_->GetGPUVirtualAddress());
	DirectXInit::GetInstance()->GetcmdList().Get()->SetGraphicsRootConstantBufferView(2, constBuffB1_->GetGPUVirtualAddress());
	
	lightGroup_->Draw(3);

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

void Object3D::constTransformMatUpdata(const Vector3& eye, const Matrix4x4& matView, const Matrix4x4& matProjection)
{
	//constMapTransform->mat = matWorld * matView * matProjection;

	constMapTransform_->viewProj_ = matView;
	constMapTransform_->viewProj_ *= matProjection;
	constMapTransform_->world_ = matWorld_;
	constMapTransform_->cameraPos_ = {eye.x,eye.y,eye.z};


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
	Trans_ = pos;
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

	collider_->SetObject(this);
	collider_ = Collider;

	//マネージャーに登録
	CollisionManager::GetInstance()->AddCollider(collider_);

	//更新しとくといいらしい
	collider_->Update();

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
	return Trans_;
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

Matrix4x4 Object3D::matScaleGeneration(const Vector3& scale)
{
	//スケーリング行列を宣言
	Matrix4x4 matScale;
	matScale.IdentityMatrix();

	//スケーリング倍率を行列に設定
	matScale.m[0][0] = scale.x;
	matScale.m[1][1] = scale.y;
	matScale.m[2][2] = scale.z;
	matScale.m[3][3] = 1;

	return matScale;
}

Matrix4x4 Object3D::matRotateXGeneration(const float& rotateX)
{
	//X軸回転行列を宣言
	Matrix4x4 matRotateX;
	matRotateX.IdentityMatrix();

	//回転角を行列に設定(ラジアン)
	matRotateX.m[0][0] = 1;
	matRotateX.m[1][1] = cosf(rotateX);
	matRotateX.m[1][2] = sinf(rotateX);
	matRotateX.m[2][1] = -sinf(rotateX);
	matRotateX.m[2][2] = cosf(rotateX);
	matRotateX.m[3][3] = 1;

	return matRotateX;
}

Matrix4x4 Object3D::matRotateYGeneration(const float& rotateY)
{
	//Y軸回転行列を宣言
	Matrix4x4 matRotateY;
	matRotateY.IdentityMatrix();

	//回転角を行列に設定(ラジアン)
	matRotateY.m[0][0] = cosf(rotateY);
	matRotateY.m[0][2] = -sinf(rotateY);
	matRotateY.m[1][1] = 1;
	matRotateY.m[2][0] = sinf(rotateY);
	matRotateY.m[2][2] = cosf(rotateY);
	matRotateY.m[3][3] = 1;

	return matRotateY;
}

Matrix4x4 Object3D::matRotateZGeneration(const float& rotateZ)
{
	//Z軸回転行列を宣言
	Matrix4x4 matRotateZ;
	matRotateZ.IdentityMatrix();

	//回転角を行列に設定(ラジアン)
	matRotateZ.m[0][0] = cosf(rotateZ);
	matRotateZ.m[0][1] = sinf(rotateZ);
	matRotateZ.m[1][0] = -sinf(rotateZ);
	matRotateZ.m[1][1] = cosf(rotateZ);
	matRotateZ.m[2][2] = 1;
	matRotateZ.m[3][3] = 1;

	return matRotateZ;
}

Matrix4x4 Object3D::matRotateGeneration(const Vector3& rotate)
{
	//X軸回転行列を宣言
	Matrix4x4 matRotateX = matRotateXGeneration(rotate.x);

	//Y軸回転行列を宣言
	Matrix4x4 matRotateY = matRotateYGeneration(rotate.y);

	//Z軸回転行列を宣言
	Matrix4x4 matRotateZ = matRotateZGeneration(rotate.z);

	//回転軸合成行列を宣言
	Matrix4x4 matRotate;
	matRotate.IdentityMatrix();

	//計算した角度を計算(順番は回転させるモデルによって変える)


	matRotate = matRotateZ * matRotateX * matRotateY;

	return matRotate;



}

Matrix4x4 Object3D::matMoveGeneration(const Vector3& translation)
{
	//移動するための行列を用意
	Matrix4x4 matMove;
	matMove.IdentityMatrix();

	//行列に移動量を代入
	matMove.m[3][0] = translation.x;
	matMove.m[3][1] = translation.y;
	matMove.m[3][2] = translation.z;

	return matMove;
}

void Object3D::matWorldGeneration()
{
	//スケール行列更新
	matScale_ = matScaleGeneration(Scale_);

	//回転行列更新
	matRotate_ = QuaternionMatRotateGeneration(Rotate_) /*matRotateGeneration(Rotate_)*/;

	//平行移動行列更新
	matTrans_ = matMoveGeneration(Trans_);

	//ワールド行列更新
	matWorld_.IdentityMatrix();
	matWorld_ = matScale_ * matRotate_ * matTrans_;



	if (parent_ != nullptr)
	{
		matWorld_ *= parent_->GetWorldMat();
	}
}

Matrix4x4 Object3D::QuaternionMatRotateGeneration(const Vector3& rotate)
{
	Quaternion ans;

	Quaternion rotaX = Quaternion::MakeAxisAngle({ 1.0f,0.0f,0.0f }, rotate.x);
	Quaternion rotaY = Quaternion::MakeAxisAngle({ 0.0f,1.0f,0.0f }, rotate.y);
	Quaternion rotaZ = Quaternion::MakeAxisAngle({ 0.0f,0.0f,1.0f }, rotate.z);

	ans = rotaX * rotaY * rotaZ;
	

	return Quaternion::MakeRotateMatrix(Quaternion::Normalize(ans));

}

uint32_t Object3D::loadTexture(const char filename[])
{
	return texture_->loadTexture(filename);
}

uint32_t Object3D::loadTexture(const std::string& filename)
{
	return texture_->loadTexture(filename);
}

void Object3D::basicInit()
{

	texture_ = Texture::GetInstance();
	
	name_ = typeid(*this).name();

	basicVertexInit();

	vertexBuffGeneration();

	vertexShaderGeneration();

	pixelShaderGeneration();

	vertexLayout();

	graphicPipelineGeneration();

	descriptorRangeGeneration();

	rootParamGeneration();

	textureSamplerGeneration();

	rootsignatureGeneration();

	constantBuffGeneration();


	indicesBuffGeneration();

	texture_->Init();

	texture_->loadTexture("Resources/basketballman2.png");

}

void Object3D::colorChangeInit()
{

	name_ = typeid(*this).name();

	colorChangeVertexInit();

	vertexBuffGeneration();

	vertexShaderGeneration2();

	pixelShaderGeneration2();

	vertexLayout();

	graphicPipelineGeneration();

	descriptorRangeGeneration();

	rootParamGeneration();

	textureSamplerGeneration();

	rootsignatureGeneration();

	constantBuffGeneration();

	indicesBuffGeneration();

	//texture->Init(dev);

}

void Object3D::objDrawInit(const std::string& directoryPath, const char filename[], const bool& smoothing)
{

	name_ = typeid(*this).name();

	texture_ = Texture::GetInstance();

	texture_->Init();
	
	objVertexBuffGeneration(directoryPath, filename,smoothing);

	vertexShaderGeneration3();

	pixelShaderGeneration3();

	vertexLayout();

	graphicPipelineGeneration();

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
	vert = { {-50.0f,-50.0f,50.0f }, {}, { 0.0f,1.0f } };//左下
	vertices_.emplace_back(vert);
	vert = { {  50.0f,-50.0f,50.0f },{}, {0.0f,0.0f} };//左上
	vertices_.emplace_back(vert);
	vert = { {-50.0f, 50.0f,50.0f }, {}, { 1.0f,1.0f } };//右下
	vertices_.emplace_back(vert);
	vert = { {  50.0f, 50.0f,50.0f },{}, {1.0f,0.0f} };//右上
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

	for (uint16_t i = 0; i < indices_.size() / 3; i++)
	{//三角形1つごとに計算していく

		//三角形のインデックスを取り出して,一時的な変数に入れる
		unsigned short indices0 = indices_[i * 3 + 0];
		unsigned short indices1 = indices_[i * 3 + 1];
		unsigned short indices2 = indices_[i * 3 + 2];

		//三角形を構成する頂点座標をベクトルに代入
		XMVECTOR p0 = XMLoadFloat3(&vertices_[indices0].pos_);
		XMVECTOR p1 = XMLoadFloat3(&vertices_[indices1].pos_);
		XMVECTOR p2 = XMLoadFloat3(&vertices_[indices2].pos_);

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

void Object3D::objVertexBuffGeneration(const std::string& directoryPath, const char filename[], const bool& smoothing)
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
#pragma region 頂点シェーダファイルの読み込みとコンパイル

	//頂点シェーダファイルの読み込み辺
	//頂点シェーダの読み込みとコンパイル
	result_ = D3DCompileFromFile(
		L"Resources/shaders/BasicVS.hlsl",//シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,//インクルード可能にする
		"main",//エントリーポイント名
		"vs_5_0",//シェーダモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,//デバック用設定
		0,
		&vsBlob_,
		&errorBlob_
	);

#pragma endregion 

#pragma region 頂点シェーダの読み込み時のエラーを表示する場所

	//頂点シェーダの読み込み時のエラーを表示する場所
	if (FAILED(result_))
	{
		//errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob_->GetBufferSize());

		std::copy_n((char*)errorBlob_->GetBufferPointer(),
			errorBlob_->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		//エラー内容をウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

#pragma endregion
}

void Object3D::vertexShaderGeneration2()
{
#pragma region 頂点シェーダファイルの読み込みとコンパイル

	//頂点シェーダファイルの読み込み辺
	//頂点シェーダの読み込みとコンパイル
	result_ = D3DCompileFromFile(
		L"Resources/shaders/vertexMoveVS.hlsl",//シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,//インクルード可能にする
		"main",//エントリーポイント名
		"vs_5_0",//シェーダモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,//デバック用設定
		0,
		&vsBlob_,
		&errorBlob_
	);

#pragma endregion 

#pragma region 頂点シェーダの読み込み時のエラーを表示する場所

	//頂点シェーダの読み込み時のエラーを表示する場所
	if (FAILED(result_))
	{
		//errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob_->GetBufferSize());

		std::copy_n((char*)errorBlob_->GetBufferPointer(),
			errorBlob_->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		//エラー内容をウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

#pragma endregion
}

void Object3D::vertexShaderGeneration3()
{
#pragma region 頂点シェーダファイルの読み込みとコンパイル

	//頂点シェーダファイルの読み込み辺
	//頂点シェーダの読み込みとコンパイル
	result_ = D3DCompileFromFile(
		L"Resources/shaders/OBJVS.hlsl",//シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,//インクルード可能にする
		"main",//エントリーポイント名
		"vs_5_0",//シェーダモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,//デバック用設定
		0,
		&vsBlob_,
		&errorBlob_
	);

#pragma endregion 

#pragma region 頂点シェーダの読み込み時のエラーを表示する場所

	//頂点シェーダの読み込み時のエラーを表示する場所
	if (FAILED(result_))
	{
		//errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob_->GetBufferSize());

		std::copy_n((char*)errorBlob_->GetBufferPointer(),
			errorBlob_->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		//エラー内容をウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

#pragma endregion
}

void Object3D::vertexShaderGeneration4()
{
#pragma region 頂点シェーダファイルの読み込みとコンパイル

	//頂点シェーダファイルの読み込み辺
	//頂点シェーダの読み込みとコンパイル
	result_ = D3DCompileFromFile(
		L"Resources/shaders/Phong_toon_rim_VS.hlsl",//シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,//インクルード可能にする
		"main",//エントリーポイント名
		"vs_5_0",//シェーダモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,//デバック用設定
		0,
		&vsBlob_,
		&errorBlob_
	);

#pragma endregion 

#pragma region 頂点シェーダの読み込み時のエラーを表示する場所

	//頂点シェーダの読み込み時のエラーを表示する場所
	if (FAILED(result_))
	{
		//errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob_->GetBufferSize());

		std::copy_n((char*)errorBlob_->GetBufferPointer(),
			errorBlob_->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		//エラー内容をウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

#pragma endregion
}

void Object3D::vertexShaderGeneration5()
{
#pragma region 頂点シェーダファイルの読み込みとコンパイル

	//頂点シェーダファイルの読み込み辺
	//頂点シェーダの読み込みとコンパイル
	result_ = D3DCompileFromFile(
		L"Resources/shaders/GaussianBlurVS.hlsl",//シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,//インクルード可能にする
		"main",//エントリーポイント名
		"vs_5_0",//シェーダモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,//デバック用設定
		0,
		&vsBlob_,
		&errorBlob_
	);

#pragma endregion 

#pragma region 頂点シェーダの読み込み時のエラーを表示する場所

	//頂点シェーダの読み込み時のエラーを表示する場所
	if (FAILED(result_))
	{
		//errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob_->GetBufferSize());

		std::copy_n((char*)errorBlob_->GetBufferPointer(),
			errorBlob_->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		//エラー内容をウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

#pragma endregion
}

void Object3D::pixelShaderGeneration()
{
#pragma region ピクセルシェーダの読み込みとコンパイル

	//ピクセルシェーダの読み込みとコンパイル
	result_ = D3DCompileFromFile(
		L"Resources/shaders/BasicPS.hlsl",//シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,//インクルード可能にする
		"main",//エントリーポイント名
		"ps_5_0",//シェーダモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,//デバック用設定
		0,
		&psBlob_,
		&errorBlob_
	);

#pragma endregion

#pragma region ピクセルシェーダの読み込み時のエラーを表示する場所

	//ピクセルシェーダの読み込み時のエラーを表示する場所
	if (FAILED(result_))
	{
		//errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob_->GetBufferSize());

		std::copy_n((char*)errorBlob_->GetBufferPointer(),
			errorBlob_->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		//エラー内容をウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

#pragma endregion
}

void Object3D::pixelShaderGeneration2()
{
#pragma region ピクセルシェーダの読み込みとコンパイル

	//ピクセルシェーダの読み込みとコンパイル
	result_ = D3DCompileFromFile(
		L"Resources/shaders/colorChangePS.hlsl",//シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,//インクルード可能にする
		"main",//エントリーポイント名
		"ps_5_0",//シェーダモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,//デバック用設定
		0,
		&psBlob_,
		&errorBlob_
	);

#pragma endregion

#pragma region ピクセルシェーダの読み込み時のエラーを表示する場所

	//ピクセルシェーダの読み込み時のエラーを表示する場所
	if (FAILED(result_))
	{
		//errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob_->GetBufferSize());

		std::copy_n((char*)errorBlob_->GetBufferPointer(),
			errorBlob_->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		//エラー内容をウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

#pragma endregion
}

void Object3D::pixelShaderGeneration3()
{
#pragma region ピクセルシェーダの読み込みとコンパイル

	//ピクセルシェーダの読み込みとコンパイル
	result_ = D3DCompileFromFile(
		L"Resources/shaders/OBJPS.hlsl",//シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,//インクルード可能にする
		"main",//エントリーポイント名
		"ps_5_0",//シェーダモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,//デバック用設定
		0,
		&psBlob_,
		&errorBlob_
	);

#pragma endregion

#pragma region ピクセルシェーダの読み込み時のエラーを表示する場所

	//ピクセルシェーダの読み込み時のエラーを表示する場所
	if (FAILED(result_))
	{
		//errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob_->GetBufferSize());

		std::copy_n((char*)errorBlob_->GetBufferPointer(),
			errorBlob_->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		//エラー内容をウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

#pragma endregion
}

void Object3D::pixelShaderGeneration4()
{
#pragma region ピクセルシェーダの読み込みとコンパイル

	//ピクセルシェーダの読み込みとコンパイル
	result_ = D3DCompileFromFile(
		L"Resources/shaders/Phong_toon_rim_PS.hlsl",//シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,//インクルード可能にする
		"main",//エントリーポイント名
		"ps_5_0",//シェーダモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,//デバック用設定
		0,
		&psBlob_,
		&errorBlob_
	);

#pragma endregion

#pragma region ピクセルシェーダの読み込み時のエラーを表示する場所

	//ピクセルシェーダの読み込み時のエラーを表示する場所
	if (FAILED(result_))
	{
		//errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob_->GetBufferSize());

		std::copy_n((char*)errorBlob_->GetBufferPointer(),
			errorBlob_->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		//エラー内容をウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

#pragma endregion
}

void Object3D::pixelShaderGeneration5()
{
#pragma region ピクセルシェーダの読み込みとコンパイル

	//ピクセルシェーダの読み込みとコンパイル
	result_ = D3DCompileFromFile(
		L"Resources/shaders/GaussianBlurPS.hlsl",//シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,//インクルード可能にする
		"main",//エントリーポイント名
		"ps_5_0",//シェーダモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,//デバック用設定
		0,
		&psBlob_,
		&errorBlob_
	);

#pragma endregion

#pragma region ピクセルシェーダの読み込み時のエラーを表示する場所

	//ピクセルシェーダの読み込み時のエラーを表示する場所
	if (FAILED(result_))
	{
		//errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob_->GetBufferSize());

		std::copy_n((char*)errorBlob_->GetBufferPointer(),
			errorBlob_->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		//エラー内容をウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

#pragma endregion
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

void Object3D::graphicPipelineGeneration()
{
#pragma region グラフィックスパイプライン設定

	//グラフィックスパイプライン辺

	//頂点シェーダ、ピクセルシェーダをパイプラインに設定
	gpipeline_.VS.pShaderBytecode = vsBlob_->GetBufferPointer();
	gpipeline_.VS.BytecodeLength = vsBlob_->GetBufferSize();
	gpipeline_.PS.pShaderBytecode = psBlob_->GetBufferPointer();
	gpipeline_.PS.BytecodeLength = psBlob_->GetBufferSize();

	//サンプルマスクとラスタライザステートの設定
	gpipeline_.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;//標準設定
	gpipeline_.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;//カリングしない
	gpipeline_.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;//ポリゴン内塗りつぶし
	gpipeline_.RasterizerState.DepthClipEnable = true;//深度クリッピングを有効に


	//ブレンドステートの設定
	//gpipeline.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;//RGBA全てのチャンネルを描画
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = gpipeline_.BlendState.RenderTarget[0];
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

#pragma region ブレンドステートの共通設定(アルファ値)

	blenddesc.BlendEnable = true;//ブレンドを有効にする
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;//加算
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;//ソースの値を100%使う
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;//デストの値を0%使う

#pragma endregion


#pragma region 加算合成

	//blenddesc.BlendOp = D3D12_BLEND_OP_ADD;//加算
	//blenddesc.SrcBlend = D3D12_BLEND_ONE;//ソースの値を100%使う
	//blenddesc.DestBlend = D3D12_BLEND_ONE;//デストの値を100%使う

#pragma endregion コメントアウト中

#pragma region 減算合成

	//blenddesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;//デストからソースを減算
	//blenddesc.SrcBlend = D3D12_BLEND_ONE;//ソースの値を100%使う
	//blenddesc.DestBlend = D3D12_BLEND_ONE;//デストの値を100%使う

#pragma endregion コメントアウト中

#pragma region 色反転

	//blenddesc.BlendOp = D3D12_BLEND_OP_ADD;//加算
	//blenddesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;//1.0f-デストカラーの値
	//blenddesc.DestBlend = D3D12_BLEND_ZERO;//使わない

#pragma endregion コメントアウト中

#pragma region 半透明合成

	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;//加算
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;//ソースのアルファ値
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;//1.0f-ソースのアルファ値

#pragma endregion



	//頂点レイアウトの設定
	gpipeline_.InputLayout.pInputElementDescs = inputLayout_.data();
	gpipeline_.InputLayout.NumElements = (uint32_t)inputLayout_.size();

	//図形の形状を三角形に設定
	gpipeline_.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	//その他の設定
	gpipeline_.NumRenderTargets = 1;//描画対象は１つ
	gpipeline_.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;//0～255指定のRGBA
	gpipeline_.SampleDesc.Count = 1;//１ピクセルにつき１回サンプリング

#pragma endregion 通常描画ポリゴン内を塗りつぶし(三角形)

#pragma region グラフィックスパイプライン２の設定

//グラフィックスパイプライン辺


	//頂点シェーダ、ピクセルシェーダをパイプラインに設定
	gpipeline2_.VS.pShaderBytecode = vsBlob_->GetBufferPointer();
	gpipeline2_.VS.BytecodeLength = vsBlob_->GetBufferSize();
	gpipeline2_.PS.pShaderBytecode = psBlob_->GetBufferPointer();
	gpipeline2_.PS.BytecodeLength = psBlob_->GetBufferSize();

	//サンプルマスクとラスタライザステートの設定
	gpipeline2_.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;//標準設定
	gpipeline2_.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;//カリングしない
	gpipeline2_.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;//ワイヤーフレーム描画
	gpipeline2_.RasterizerState.DepthClipEnable = true;//深度クリッピングを有効に

	//ブレンドステートの設定
	//gpipeline2.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;//RGBA全てのチャンネルを描画
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc2 = gpipeline2_.BlendState.RenderTarget[0];
	blenddesc2.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

#pragma region 共通設定(アルファ値)

	blenddesc2.BlendEnable = true;//ブレンドを有効にする
	blenddesc2.BlendOpAlpha = D3D12_BLEND_OP_ADD;//加算
	blenddesc2.SrcBlendAlpha = D3D12_BLEND_ONE;//ソースの値を100%使う
	blenddesc2.DestBlendAlpha = D3D12_BLEND_ZERO;//デストの値を0%使う

#pragma endregion

#pragma region 半透明合成

	blenddesc2.BlendOp = D3D12_BLEND_OP_ADD;//加算
	blenddesc2.SrcBlend = D3D12_BLEND_SRC_ALPHA;//ソースのアルファ値
	blenddesc2.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;//1.0f-ソースのアルファ値

#pragma endregion

	//頂点レイアウトの設定
	gpipeline2_.InputLayout.pInputElementDescs = inputLayout_.data();
	gpipeline2_.InputLayout.NumElements = (uint32_t)inputLayout_.size();

	//図形の形状を三角形に設定
	gpipeline2_.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	//その他の設定
	gpipeline2_.NumRenderTargets = 1;//描画対象は１つ
	gpipeline2_.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;//0～255指定のRGBA
	gpipeline2_.SampleDesc.Count = 1;//１ピクセルにつき１回サンプリング

#pragma endregion 通常描画ワイヤーフレーム描画(三角形)

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

	D3D12_ROOT_PARAMETER newRootParam;

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


	//パイプラインにルートシグネチャをセット
	gpipeline_.pRootSignature = rootsignature_.Get();

	//パイプライン2にもルートシグネチャをセット
	gpipeline2_.pRootSignature = rootsignature_.Get();

#pragma endregion 定数バッファを増やしたらルートパラメータを書き換えパラメータ数を書き換える

#pragma region デプスステンシルステート

	//デプスステンシルステートの設定
	gpipeline_.DepthStencilState.DepthEnable = true;//深度テストを行う
	gpipeline_.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;//書き込み許可
	gpipeline_.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;//小さければ合格
	gpipeline_.DSVFormat = DXGI_FORMAT_D32_FLOAT;//深度値フォーマット

	gpipeline2_.DepthStencilState.DepthEnable = true;//深度テストを行う
	gpipeline2_.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;//書き込み許可
	gpipeline2_.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;//小さければ合格
	gpipeline2_.DSVFormat = DXGI_FORMAT_D32_FLOAT;//深度値フォーマット



#pragma endregion


#pragma region パイプラインステートの生成

	//パイプラインステートの生成

	result_ = DirectXInit::GetInstance()->Getdev()->CreateGraphicsPipelineState(&gpipeline_, IID_PPV_ARGS(&pipelinestate_));

	//パイプラインステート２の生成

	result_ = DirectXInit::GetInstance()->Getdev()->CreateGraphicsPipelineState(&gpipeline2_, IID_PPV_ARGS(&pipelinestate2_));

#pragma endregion

}

void Object3D::constantBuffGeneration()
{
#pragma region 定数バッファ

	//定数バッファの生成用の設定

	cbHeapProp_.Type = D3D12_HEAP_TYPE_UPLOAD;//GPUへの転送用




	//cbResourceDesc = constBuffResourceGeneration(sizeof(ConstBufferDataMaterial));

	////定数バッファの生成
	//result = dev->CreateCommittedResource(
	//	&cbHeapProp,//ヒープ設定
	//	D3D12_HEAP_FLAG_NONE,
	//	&cbResourceDesc,//リソース設定
	//	D3D12_RESOURCE_STATE_GENERIC_READ,
	//	nullptr,
	//	IID_PPV_ARGS(&constBuffMaterial)
	//);
	//assert(SUCCEEDED(result));


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


	//cbResourceDesc = constBuffResourceGeneration(sizeof(ConstBufferDataTransform));

	////定数バッファの生成
	//result = dev->CreateCommittedResource(
	//	&cbHeapProp,//ヒープ設定
	//	D3D12_HEAP_FLAG_NONE,
	//	&cbResourceDesc,//リソース設定
	//	D3D12_RESOURCE_STATE_GENERIC_READ,
	//	nullptr,
	//	IID_PPV_ARGS(&constBuffTransform0)
	//);
	//assert(SUCCEEDED(result));


	////定数バッファのマッピング
	//

	//マッピングをするとGPUのVRSMがCPUと連動する
	//Unmapをするとつながりが解除されるが定数バッファは書き換えることが多いので
	//しなくても大丈夫

	//値を書き込むと自動的に転送される



	//result = constBuffMaterial2->Map(0, nullptr, (void**)&constMapMaterial2);//マッピング
	//assert(SUCCEEDED(result));



	//平行投射行列の計算
	//constMapTransform->mat = XMMatrixOrthographicOffCenterLH(0.0f,Win_width,Win_height, 0.0f, 0.0f, 1.0f);

	//透視投影行列の計算

	/*matProjection = perspectiveProjectionGeneration((45.0f * (PI / 180)), 0.1f, 1000.0f);

	matView = matViewGeneration(eye_, target_, up_);*/



	//ビュー変換行列	

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
	if (texture_ != nullptr)
	{
		texture_->instanceDelete();
	}
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
			LoadMaterialTexture(texpath);
		}

	}

	//ファイルを閉じる
	file.close();

}

bool Object3D::LoadMaterialTexture(const std::string& filename)
{

	materialTextureNum_ = texture_->loadTexture(filename);
	return true;

}