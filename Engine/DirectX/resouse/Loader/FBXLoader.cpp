#include "FBXLoader.h"

AnimationModel::~AnimationModel()
{
}

bool AnimationModel::Load(std::string filename, std::string fileType, std::string materialName, std::string materialType)
{
	//インスタンスを取得
	Assimp::Importer importer;

	//ファイル名を保存
	filename_ = filename;

	//マテリアル名を保存
	if (materialName != "")
	{
		materialName_ = "Resources\\obj\\" + filename + "\\" + materialName + "." + materialType;
	}
	else
	{
		materialName_ = materialName;
	}

	//ファイルパスをつくるよ～
	std::string baseDirectory = "Resources\\obj\\";
	std::string extend = "." + fileType;
	filename = baseDirectory + filename + "\\" + filename + extend;

	//指定したファイルの読み込みらしい
	const aiScene* scene = importer.ReadFile(filename,
		aiProcess_CalcTangentSpace |//インポートしたメッシュのタンジェントとかを計算してくれるらしい
		aiProcess_Triangulate |//三角面化してくれる
		aiProcess_JoinIdenticalVertices |//インポートしたメッシュの同一の頂点データセットを識別、結合する
		aiProcess_SortByPType |//2つ以上のプリミティブタイプを持つメッシュを均質なサブメッシュに分割する
		aiProcess_MakeLeftHanded |//左手座標系に
		aiProcess_FlipUVs |//UV座標を左上を原点にする
		aiProcess_FlipWindingOrder//CCWカリングに適した時計回りの面の順序にする
	);

	//読み込みに失敗したらエラーをはいて止まる?
	if (nullptr == scene)
	{
		//DoTheImportThing(importer.GetErrorString());
		return false;
	}



	//情報を入れる
	if (scene != nullptr)
	{
		CopyNodeMesh(scene->mRootNode, scene);
	}

	//ここからアニメーションの予定


	return true;
}

void AnimationModel::Draw()const
{

	for (auto& itr : nodes_)
	{
		for (auto& jtr : itr->meshes_)
		{
			DirectXInit::GetInstance()->GetcmdList()->IASetVertexBuffers(0, 1, &jtr->vbView_);

			DirectXInit::GetInstance()->GetcmdList()->IASetIndexBuffer(&jtr->ibView_);

			for (uint16_t i = 0; i < jtr->textureHandle.size(); i++)
			{
				Texture::GetInstance()->Draw(jtr->textureHandle[i]);
			}

			DirectXInit::GetInstance()->GetcmdList()->DrawIndexedInstanced((uint32_t)jtr->indices_.size(), 1, 0, 0, 0);



		}
	}

}


void AnimationModel::CopyNodeMesh(const aiNode* node, const aiScene* scene, Node* targetParent)
{
	Node* parent;

	std::unique_ptr<Node> newNode = std::make_unique<Node>();

	//部位の名前を入れる
	newNode->name_ = node->mName.C_Str();

	//メッシュの数だけ回す
	for (uint16_t i = 0; i < node->mNumMeshes; i++)
	{
		//情報を入れる入れ物を作る
		std::unique_ptr<AnimationMesh> model = std::make_unique<AnimationMesh>();

		//メッシュの情報を書き込み
		ProcessMesh(scene->mMeshes[node->mMeshes[i]], scene, *model.get());
		newNode->meshes_.emplace_back(std::move(model));
	}

	//持ってる位置を入れる
	newNode->transform_.m[0][0] = node->mTransformation.a1;
	newNode->transform_.m[0][1] = node->mTransformation.a2;
	newNode->transform_.m[0][2] = node->mTransformation.a3;
	newNode->transform_.m[0][3] = node->mTransformation.a4;

	newNode->transform_.m[1][0] = node->mTransformation.b1;
	newNode->transform_.m[1][1] = node->mTransformation.b2;
	newNode->transform_.m[1][2] = node->mTransformation.b3;
	newNode->transform_.m[1][3] = node->mTransformation.b4;

	newNode->transform_.m[2][0] = node->mTransformation.c1;
	newNode->transform_.m[2][1] = node->mTransformation.c2;
	newNode->transform_.m[2][2] = node->mTransformation.c3;
	newNode->transform_.m[2][3] = node->mTransformation.c4;

	newNode->transform_.m[3][0] = node->mTransformation.d1;
	newNode->transform_.m[3][1] = node->mTransformation.d2;
	newNode->transform_.m[3][2] = node->mTransformation.d3;
	newNode->transform_.m[3][3] = node->mTransformation.d4;

	newNode->transform_ = newNode->transform_.TransposeMatrix();
	newNode->globalTransform_ = newNode->transform_;
	newNode->globalInverseTransform_ = Matrix4x4::Inverse(newNode->transform_);

	nodes_.emplace_back(std::move(newNode));
	parent = nodes_.back().get();

	//親が設定されているなら
	if (targetParent)
	{

		parent->parent_ = targetParent;
		targetParent->globalTransform_ *= parent->globalTransform_;

	}
	for (uint16_t i = 0; i < node->mNumChildren; i++)
	{
		CopyNodeMesh(node->mChildren[i], scene, parent);
	}

}

void AnimationModel::ProcessMesh(aiMesh* mesh, const aiScene* scene, AnimationMesh& model)
{

	for (uint16_t i = 0; i < mesh->mNumVertices; i++)
	{

		AnimationVertex vertex;

		vertex.pos_.x = mesh->mVertices[i].x;
		vertex.pos_.y = mesh->mVertices[i].y;
		vertex.pos_.z = mesh->mVertices[i].z;

		vertex.normal_.x = mesh->mNormals[i].x;
		vertex.normal_.y = mesh->mNormals[i].y;
		vertex.normal_.z = mesh->mNormals[i].z;

		if (mesh->mTextureCoords[0])
		{
			vertex.uv_.x = (float)mesh->mTextureCoords[0][i].x;
			vertex.uv_.y = (float)mesh->mTextureCoords[0][i].y;
		}

		model.vertices_.emplace_back(vertex);

	}

	//おそらく最小単位の形がfaceに該当してその中にインデックスがある
	for (uint16_t i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];

		//インデックスを入れる
		for (uint16_t j = 0; j < face.mNumIndices; j++)
		{
			model.indices_.emplace_back((unsigned short)face.mIndices[j]);
		}
	}

	//マテリアルを入れる
	if (mesh->mMaterialIndex >= 0)
	{
		aiColor3D color;
		Material material;
		aiString name;

		//diffuse
		scene->mMaterials[mesh->mMaterialIndex]->Get(AI_MATKEY_COLOR_DIFFUSE, color);
		material.material_.diffuse_.x = color.r;
		material.material_.diffuse_.y = color.g;
		material.material_.diffuse_.z = color.b;

		//specular
		scene->mMaterials[mesh->mMaterialIndex]->Get(AI_MATKEY_COLOR_SPECULAR, color);
		material.material_.specular_.x = color.r;
		material.material_.specular_.y = color.g;
		material.material_.specular_.z = color.b;

		//ambient
		scene->mMaterials[mesh->mMaterialIndex]->Get(AI_MATKEY_COLOR_AMBIENT, color);
		material.material_.ambient_.x = color.r;
		material.material_.ambient_.y = color.g;
		material.material_.ambient_.z = color.b;

		//テストで読み込んだやつのアンビエントがなくてテクスチャが見えなくなってしまっていたので追加
		if (material.material_.ambient_.x == 0 && material.material_.ambient_.y == 0 && material.material_.ambient_.z == 0)
		{
			material.material_.ambient_.x = 1;
			material.material_.ambient_.y = 1;
			material.material_.ambient_.z = 1;
		}

		//ファイル名
		scene->mMaterials[mesh->mMaterialIndex]->Get(AI_MATKEY_NAME, name);
		material.material_.name_ = name.C_Str();

		//α値
		scene->mMaterials[mesh->mMaterialIndex]->Get(AI_MATKEY_OPACITY, material.material_.alpha_);

		material.MaterialConstBuffInit();

		model.material_.emplace_back(material);

		LoadMaterialTextures(scene->mMaterials[mesh->mMaterialIndex], aiTextureType_DIFFUSE, model);

	}


	//ウエイトの保存場所
	std::vector<std::list<SetWeight>> weightList(model.vertices_.size());
	//ボーンの読み込み
	for (uint16_t i = 0; i < mesh->mNumBones; i++)
	{

		Bone temp;
		//名前
		temp.name_ = mesh->mBones[i]->mName.C_Str();

		//ボーンの位置を代入
		temp.offsetMatrix_.m[0][0] = mesh->mBones[i]->mOffsetMatrix.a1;
		temp.offsetMatrix_.m[0][1] = mesh->mBones[i]->mOffsetMatrix.a2;
		temp.offsetMatrix_.m[0][2] = mesh->mBones[i]->mOffsetMatrix.a3;
		temp.offsetMatrix_.m[0][3] = mesh->mBones[i]->mOffsetMatrix.a4;

		temp.offsetMatrix_.m[1][0] = mesh->mBones[i]->mOffsetMatrix.b1;
		temp.offsetMatrix_.m[1][1] = mesh->mBones[i]->mOffsetMatrix.b2;
		temp.offsetMatrix_.m[1][2] = mesh->mBones[i]->mOffsetMatrix.b3;
		temp.offsetMatrix_.m[1][3] = mesh->mBones[i]->mOffsetMatrix.b4;

		temp.offsetMatrix_.m[2][0] = mesh->mBones[i]->mOffsetMatrix.c1;
		temp.offsetMatrix_.m[2][1] = mesh->mBones[i]->mOffsetMatrix.c2;
		temp.offsetMatrix_.m[2][2] = mesh->mBones[i]->mOffsetMatrix.c3;
		temp.offsetMatrix_.m[2][3] = mesh->mBones[i]->mOffsetMatrix.c4;

		temp.offsetMatrix_.m[3][0] = mesh->mBones[i]->mOffsetMatrix.d1;
		temp.offsetMatrix_.m[3][1] = mesh->mBones[i]->mOffsetMatrix.d2;
		temp.offsetMatrix_.m[3][2] = mesh->mBones[i]->mOffsetMatrix.d3;
		temp.offsetMatrix_.m[3][3] = mesh->mBones[i]->mOffsetMatrix.d4;

		//逆行列として使いたいが転置しないと使えない？
		//転置させる
		temp.offsetMatrix_ = temp.offsetMatrix_.TransposeMatrix();
		temp.finalMatrix_ = temp.offsetMatrix_;

		//ウエイトを入れる
		for (uint16_t j = 0; j < mesh->mBones[i]->mNumWeights; j++)
		{

			SetWeight tempVer{};
			tempVer.id_ = i;
			tempVer.weight_ = mesh->mBones[i]->mWeights[j].mWeight;
			//特定の頂点の位置に入れる感じだと思う
			weightList[mesh->mBones[i]->mWeights[j].mVertexId].emplace_back(tempVer);

		}

		bones.emplace_back(temp);

	}

	//ウエイトの本格代入
	for (uint16_t i = 0; i < model.vertices_.size(); i++)
	{
		//頂点のウエイトから最も大きい4つを選択
		auto& weightL = weightList[i];

		//順番にする
		weightL.sort([](auto const& lhs, auto const& rhs) { return lhs.weight_ > rhs.weight_; });

		uint32_t weightArrayIndex = 0;
		for (auto& weightSet : weightL)
		{
			//頂点にウエイトとidを渡す
			model.vertices_[i].ids_[weightArrayIndex] = weightSet.id_;
			model.vertices_[i].weights_[weightArrayIndex] = weightSet.weight_;

			//もしインデックスを増やした際に最大値を超えてしまった場合
			if (++weightArrayIndex >= SNUM_BONES_PER_VERTEX)
			{
				//この頂点に対して影響を受けるボーンの数が最大値(SNUM_BONES_PER_VERTEX)を超えてしまった場合
				//普通のやり方だとウエイトの合計が100%にならなくなってしまうため
				//最初の値にオーバーした分のウエイトを足すことで100%になるように
				float weight = 0.0f;

				//残す方のウエイトの合計を作る
				for (uint16_t j = 1; j < SNUM_BONES_PER_VERTEX; j++)
				{
					//全部まとめる
					weight += model.vertices_[i].weights_[j];
				}

				//最大値から残す方のウエイトを引くことで 最も大きいウエイト+あふれた分のウエイトになる
				model.vertices_[i].weights_[0] = 1.0f - weight;
				break;

			}


		}

	}

	model.Init();

}

void AnimationModel::LoadMaterialTextures(aiMaterial* material, aiTextureType type, AnimationMesh& model)
{
	//条件に該当するテクスチャ分回す?
	for (uint16_t i = 0; i < material->GetTextureCount(type); i++)
	{

		aiString str;
		std::string path;

		//パスを取得
		material->GetTexture(type, i, &str);

		path = str.C_Str();

		if (path.size() < 5)
		{
			if (materialName_ != "")
			{
				//読み込んでハンドルを保存
				model.textureHandle.emplace_back(Texture::GetInstance()->loadTexture(materialName_));
			}
			else
			{
				//読み込んでハンドルを保存
				model.textureHandle.emplace_back(Texture::GetInstance()->loadTexture("Resources\\white1x1.png"));
			}
		}
		else
		{

			while (path.find("\\") != std::string::npos)
			{
				//末尾の部分を取得
				path = path.substr(path.find("\\") + 1);

			}

			//合体させてパスを完成させる
			path = "Resources\\obj" + filename_ + "\\" + path;


			//読み込んでハンドルを保存
			model.textureHandle.emplace_back(Texture::GetInstance()->loadTexture(path));
		}

	}
}