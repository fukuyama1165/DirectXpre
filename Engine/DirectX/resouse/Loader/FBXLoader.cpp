#include "FBXLoader.h"

AnimationModel::~AnimationModel()
{
}

bool AnimationModel::Load(std::string filename, std::string fileType, std::string materialName, std::string materialType)
{
	//�C���X�^���X���擾
	Assimp::Importer importer;

	//�t�@�C������ۑ�
	filename_ = filename;

	//�}�e���A������ۑ�
	if (materialName != "")
	{
		materialName_ = "Resources\\obj\\" + filename + "\\" + materialName + "." + materialType;
	}
	else
	{
		materialName_ = materialName;
	}

	//�t�@�C���p�X�������`
	std::string baseDirectory = "Resources\\obj\\";
	std::string extend = "." + fileType;
	filename = baseDirectory + filename + "\\" + filename + extend;

	//�w�肵���t�@�C���̓ǂݍ��݂炵��
	const aiScene* scene = importer.ReadFile(filename,
		aiProcess_CalcTangentSpace |//�C���|�[�g�������b�V���̃^���W�F���g�Ƃ����v�Z���Ă����炵��
		aiProcess_Triangulate |//�O�p�ʉ����Ă����
		aiProcess_JoinIdenticalVertices |//�C���|�[�g�������b�V���̓���̒��_�f�[�^�Z�b�g�����ʁA��������
		aiProcess_SortByPType |//2�ȏ�̃v���~�e�B�u�^�C�v�������b�V�����ώ��ȃT�u���b�V���ɕ�������
		aiProcess_MakeLeftHanded |//������W�n��
		aiProcess_FlipUVs |//UV���W����������_�ɂ���
		aiProcess_FlipWindingOrder//CCW�J�����O�ɓK�������v���̖ʂ̏����ɂ���
	);

	//�ǂݍ��݂Ɏ��s������G���[���͂��Ď~�܂�?
	if (nullptr == scene)
	{
		//DoTheImportThing(importer.GetErrorString());
		return false;
	}


	
	//��������
	if (scene != nullptr)
	{
		CopyNodeMesh(scene->mRootNode, scene);
	}

	//��������A�j���[�V�����̗\��


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

	//���ʂ̖��O������
	newNode->name_ = node->mName.C_Str();

	//���b�V���̐�������
	for (uint16_t i = 0; i < node->mNumMeshes; i++)
	{
		//����������ꕨ�����
		std::unique_ptr<AnimationMesh> model = std::make_unique<AnimationMesh>();

		//���b�V���̏�����������
		ProcessMesh(scene->mMeshes[node->mMeshes[i]], scene, *model.get());
		newNode->meshes_.emplace_back(std::move(model));
	}

	//�����Ă�ʒu������
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

	//�e���ݒ肳��Ă���Ȃ�
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

	//�����炭�ŏ��P�ʂ̌`��face�ɊY�����Ă��̒��ɃC���f�b�N�X������
	for (uint16_t i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];

		//�C���f�b�N�X������
		for (uint16_t j = 0; j < face.mNumIndices; j++)
		{
			model.indices_.emplace_back((unsigned short)face.mIndices[j]);
		}
	}

	//�}�e���A��������
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

		//�t�@�C����
		scene->mMaterials[mesh->mMaterialIndex]->Get(AI_MATKEY_NAME, name);
		material.material_.name_ = name.C_Str();

		//���l
		scene->mMaterials[mesh->mMaterialIndex]->Get(AI_MATKEY_OPACITY, material.material_.alpha_);
		
		material.MaterialConstBuffInit();

		model.material_.emplace_back(material);

		LoadMaterialTextures(scene->mMaterials[mesh->mMaterialIndex], aiTextureType_DIFFUSE, model);

	}


	//�E�G�C�g�̕ۑ��ꏊ
	std::vector<std::list<SetWeight>> weightList(model.vertices_.size());
	//�{�[���̓ǂݍ���
	for (uint16_t i = 0; i < mesh->mNumBones; i++)
	{

		Bone temp;
		//���O
		temp.name_ = mesh->mBones[i]->mName.C_Str();

		//�{�[���̈ʒu����
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

		//�t�s��Ƃ��Ďg���������]�u���Ȃ��Ǝg���Ȃ��H
		//�]�u������
		temp.offsetMatrix_ = temp.offsetMatrix_.TransposeMatrix();
		temp.finalMatrix_ = temp.offsetMatrix_;

		//�E�G�C�g������
		for (uint16_t j = 0; j < mesh->mBones[i]->mNumWeights; j++)
		{

			SetWeight tempVer;
			tempVer.id_ = i;
			tempVer.weight_ = mesh->mBones[i]->mWeights[j].mWeight;
			//����̒��_�̈ʒu�ɓ���銴�����Ǝv��
			weightList[mesh->mBones[i]->mWeights[j].mVertexId].emplace_back(tempVer);

		}

		bones.emplace_back(temp);

	}

	//�E�G�C�g�̖{�i���
	for (uint16_t i = 0; i < model.vertices_.size(); i++)
	{
		//���_�̃E�G�C�g����ł��傫��4��I��
		auto& weightL = weightList[i];

		//���Ԃɂ���
		weightL.sort([](auto const& lhs, auto const& rhs){ return lhs.weight_ > rhs.weight_; });

		uint32_t weightArrayIndex = 0;
		for (auto& weightSet : weightL)
		{
			//���_�ɃE�G�C�g��id��n��
			model.vertices_[i].ids_[weightArrayIndex] = weightSet.id_;
			model.vertices_[i].weights_[weightArrayIndex] = weightSet.weight_;

			//�����C���f�b�N�X�𑝂₵���ۂɍő�l�𒴂��Ă��܂����ꍇ
			if (++weightArrayIndex >= SNUM_BONES_PER_VERTEX)
			{
				//���̒��_�ɑ΂��ĉe�����󂯂�{�[���̐����ő�l(SNUM_BONES_PER_VERTEX)�𒴂��Ă��܂����ꍇ
				//���ʂ̂������ƃE�G�C�g�̍��v��100%�ɂȂ�Ȃ��Ȃ��Ă��܂�����
				//�ŏ��̒l�ɃI�[�o�[�������̃E�G�C�g�𑫂����Ƃ�100%�ɂȂ�悤��
				float weight = 0.0f;

				//�c�����̃E�G�C�g�̍��v�����
				for (uint16_t j = 1; j < SNUM_BONES_PER_VERTEX; j++)
				{
					//�S���܂Ƃ߂�
					weight += model.vertices_[i].weights_[j];
				}

				//�ő�l����c�����̃E�G�C�g���������Ƃ� �ł��傫���E�G�C�g+���ӂꂽ���̃E�G�C�g�ɂȂ�
				model.vertices_[i].weights_[0] = 1.0f - weight;
				break;

			}


		}

	}

	model.Init();

}

void AnimationModel::LoadMaterialTextures(aiMaterial* material, aiTextureType type, AnimationMesh& model)
{
	uint32_t hoge = material->GetTextureCount(type);
	hoge;
	//�����ɊY������e�N�X�`������?
	for (uint16_t i = 0; i < material->GetTextureCount(type); i++)
	{

		aiString str;
		std::string path;

		//�p�X���擾
		material->GetTexture(type, i, &str);

		path = str.C_Str();

		if (path.size()<5)
		{
			if (materialName_ != "")
			{
				//�ǂݍ���Ńn���h����ۑ�
				model.textureHandle.emplace_back(Texture::GetInstance()->loadTexture(materialName_));
			}
			else
			{
				//�ǂݍ���Ńn���h����ۑ�
				model.textureHandle.emplace_back(Texture::GetInstance()->loadTexture("Resources\\white1x1.png"));
			}
		}
		else
		{

			while (path.find("\\") != std::string::npos)
			{
				//�����̕������擾
				path = path.substr(path.find("\\") + 1);

			}

			//���̂����ăp�X������������
			path = "Resources\\obj" + filename_ + "\\" + path;


			//�ǂݍ���Ńn���h����ۑ�
			model.textureHandle.emplace_back(Texture::GetInstance()->loadTexture(path));
		}

	}
}