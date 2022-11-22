#pragma once
#include<d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

#include <fstream>
#include <sstream>
#include <vector>
#include <string>

#include<d3d12.h>
#include<dxgi1_6.h>

//数学ライブラリ(最適化されすぎて使いにくいらしいのであとで自作しろって)
#include<DirectXMath.h>
using namespace DirectX;

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

#include "matrix4x4.h"
#include "Float3.h"

//ComPtr用インクルード
#include <wrl.h>

#include "SpriteCommon.h"

class Sprite
{
public:
	void initialize(ID3D12Device* dev);

	void vertexBuffGeneration(ID3D12Device* dev);

	void Draw(ID3D12GraphicsCommandList* cmdList,SpriteCommon& spriteCommon, bool PipeLineRuleFlag);

private:

	XMFLOAT3 vertices[3];

	UINT sizeVB;

	HRESULT result;

	//頂点バッファ用変数
	D3D12_HEAP_PROPERTIES heapprop{};//ヒープ設定
	D3D12_RESOURCE_DESC resDesc{};//リソース設定
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff = nullptr;
	XMFLOAT3* vertMap = nullptr;
	//頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView{};

};

