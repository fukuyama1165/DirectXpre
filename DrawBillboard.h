#pragma once
//D3Dコンパイラ
#include<d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

#include<vector>
#include<string>

#include<d3d12.h>
#include<dxgi1_6.h>

//数学ライブラリ(最適化されすぎて使いにくいらしいのであとで自作しろって)
#include<DirectXMath.h>
using namespace DirectX;

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

//キーボードやコントローラーなどの入力するヘッダとライブラリのリンク
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#pragma comment(lib,"dinput8.lib")

#pragma comment(lib,"dxguid.lib")

//const float PI = 3.141592653589;

#include <DirectXTex.h>

#include "matrix4x4.h"
#include "Float3.h"
#include "Object3D.h"
#include "Texture.h"
#include "Float4.h"

//ComPtr用インクルード
#include <wrl.h>


class DrawBillboard
{
};

