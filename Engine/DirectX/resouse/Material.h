#pragma once

/**
 * @file Material.h
 * @brief マテリアルデータを保存するためのクラス
 * @author フクヤマ
 * @date 2023_11/10
 */
//D3Dコンパイラ
#include<d3dcompiler.h>

#include<d3d12.h>


#include <vector>

#include "DirectXInit.h"

#include "Vector4.h"
#include "Vector3.h"
#include "Vector2.h"
#include "matrix4x4.h"
#include "Texture.h"

#include <unordered_map>


class Material
{
public:
	Material();
	~Material();

	void MaterialConstBuffInit();

	void Update();

	void SetMaterialTiring(Vector2 tiring) { material_.tile_ = tiring; };

	struct ConstBufferDataB1
	{
		Vector3 ambient_;//アンビエント影響度
		float pad1_;//パディング
		Vector3 diffuse_;//ディフューズ影響度
		float pad2_;//パディング
		Vector3 specular_;//スペキュラー影響度
		float alpha_;//アルファ
		Vector2 tile;
		float pad3_;
		float pad4_;
	};

	struct MaterialObj
	{
		std::string name_;//マテリアル名
		Vector3 ambient_ = {};//アンビエント影響度
		Vector3 diffuse_ = {};//ディフューズ影響度
		Vector3 specular_ = {};//スペキュラー影響度
		float alpha_ = 1.0f;//アルファ
		Vector2 tile_ = {1,1};
		std::string textureFilename_;//テクスチャファイル名

		//コンストラクタ
		MaterialObj()
		{
			ambient_ = { 0.3f, 0.3f, 0.3f };
			diffuse_ = { 0.0f, 0.0f, 0.0f };
			specular_ = { 0.0f, 0.0f, 0.0f };
			alpha_ = 1.0f;
		}

	};

	//定数バッファ用
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffB1_ = nullptr;

	ConstBufferDataB1* constMapB1_ = nullptr;

	D3D12_HEAP_PROPERTIES cbHeapProp_{};//ヒープ設定

	MaterialObj material_;

	

private:

	HRESULT result_ = S_OK;

};

