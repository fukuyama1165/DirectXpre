#include<Windows.h>

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

//#pragma commentとは、オブジェクトファイルに、
//リンカでリンクするライブラリの名前を記述するもの
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

//キーボードやコントローラーなどの入力するヘッダとライブラリのリンク
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")


#pragma region ウィンドウプロシージャ


LRESULT windowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_DESTROY://ウィンドウが破棄された
		PostQuitMessage(0);//OSに対して、終わったことを伝える
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);//標準の処理をする
}

#pragma endregion


//デバイス発見時の実行される
BOOL CALLBACK DeviceFindCallBack(LPCDIDEVICEINSTANCE ipddi, LPVOID pvRef)
{
	return DIENUM_CONTINUE;
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{

#pragma region ウィンドウの初期化部分
	const int window_width = 1280;
	const int window_height = 720;


	WNDCLASSEX w = {};//ウィンドウクラスの設定

	w.cbSize = sizeof(WNDCLASSEX);
	w.lpfnWndProc = (WNDPROC)windowProc;//ウィンドウプロシージャ
	w.lpszClassName = L"DirectXGame";//ウィンドウクラス名
	w.hInstance = GetModuleHandle(nullptr);//ウィンドウハンドル
	w.hCursor = LoadCursor(NULL, IDC_ARROW);//カーソル指定

	//ウィンドウクラスをOSに登録
	RegisterClassEx(&w);

	//ウィンドウのサイズの構造体{x座標,y座標,横幅,縦幅}
	RECT wrc = { 0,0,window_width,window_height };
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);//自動でサイズ補正

	//ウィンドウの構成要素？
	HWND hwnd = CreateWindow(w.lpszClassName,//クラス名
		L"DirectXGame",//タイトルバーの名前
		WS_OVERLAPPEDWINDOW,//標準的なウインドウスタイル
		CW_USEDEFAULT,//x座標(OSに任せる)
		CW_USEDEFAULT,//y座標(OSに任せる)
		wrc.right - wrc.left,//ウィンドウ横幅
		wrc.bottom - wrc.top,//ウィンドウ縦幅
		nullptr,//親ウィンドウハンドル
		nullptr,//メニューハンドル
		w.hInstance,//呼び出しアプリケーションハンドル
		nullptr);//オプション

	//ウィンドウ表示部分
	ShowWindow(hwnd, SW_SHOW);

	//これを書かないとウィンドウが一瞬で消えるため記述
	MSG msg{};

#pragma endregion


#pragma region DirectX初期化処理
//ここから	
#pragma region デバックレイヤーの有効化



#ifdef _DEBUG

	//デバックレイヤーをオンに
	ID3D12Debug* debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{
		debugController->EnableDebugLayer();
	}

#endif // DEBUG

#pragma endregion

#pragma region 初期化用変数宣言

	HRESULT result;
	ID3D12Device* dev = nullptr;
	IDXGIFactory6* dxgiFactory = nullptr;
	IDXGISwapChain4* swapchain = nullptr;
	ID3D12CommandAllocator* cmdAllocator = nullptr;
	ID3D12GraphicsCommandList* cmdList = nullptr;
	ID3D12CommandQueue* cmdQueue = nullptr;
	ID3D12DescriptorHeap* rtvHeaps = nullptr;

#pragma endregion

#pragma region グラフィックアダプター設定


	
	//DXGIファクトリーの生成
	result = CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory));

	//アダプターの列挙用
	std::vector<IDXGIAdapter1*> adapters;

	//ここに特定の名前を持つアダプターオブジェクトが入る
	IDXGIAdapter1* tmpAdapter = nullptr;
	for (int i = 0; dxgiFactory->EnumAdapters1(i, &tmpAdapter) != DXGI_ERROR_NOT_FOUND; i++)
	{
		adapters.push_back(tmpAdapter);//動的配列に追加する
	}

	for (int i = 0; i < adapters.size(); i++)
	{
		DXGI_ADAPTER_DESC1 adesc;
		adapters[i]->GetDesc1(&adesc);//アダプターの情報を取得

		//ソフトウェアデバイスを回避
		if (adesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
		{
			continue;
		}

		std::wstring strDesc = adesc.Description;//アダプター名

		//Intel UHD Graphice(オンボードグラフィック)を回避
		if (strDesc.find(L"Intel") == std::wstring::npos)
		{
			tmpAdapter = adapters[i];//採用
			break;
		}

	}

	//採用したグラフィックデバイスを操作するためのオブジェクトを生成
	//※普通は一つしか作らないのでクラス化等する際に複数生成しないように

	//対応レベルの配列
	D3D_FEATURE_LEVEL levels[] =
	{
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};

	D3D_FEATURE_LEVEL featureLevel;

	for (int i = 0; i < _countof(levels); i++)
	{
		//採用したアダプターでデバイスを生成
		result = D3D12CreateDevice(tmpAdapter, levels[i], IID_PPV_ARGS(&dev));
		if (result == S_OK)//ちょくちょくresultの中身がS_OKであることを確認すること
		{
			//デバイスを生成できた時点でループを抜ける
			featureLevel = levels[i];
			break;
		}
	}

#pragma endregion これのクラス化で複数生成に注意

#pragma region コマンドリスト辺

	//コマンドリストを生成するにはコマンドアロケータが必要
	//なので先にコマンドアロケータを生成

	//コマンドアロケータを生成
	result = dev->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&cmdAllocator)
	);

	//コマンドリストを生成
	result = dev->CreateCommandList(
		0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		cmdAllocator,
		nullptr,
		IID_PPV_ARGS(&cmdList)
	);

#pragma endregion

#pragma region コマンドキュー辺

	//標準設定でコマンドキューを生成
	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc{};//構造体の中身を{}で０でクリアしている(microsoftの標準が0のため)

	dev->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&cmdQueue));

#pragma endregion

#pragma region スワップチェーン辺

	//各種設定をしてスワップチェーンを生成
	DXGI_SWAP_CHAIN_DESC1 swapchainDesc{};
	swapchainDesc.Width = 1280;
	swapchainDesc.Height = 720;
	swapchainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;//色情報の書式
	swapchainDesc.SampleDesc.Count = 1;//マルチサンプルしない
	swapchainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;//バックバッファ用
	swapchainDesc.BufferCount = 2;//バッファ数を2つに設定
	swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;//フリップ後は破棄
	swapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	dxgiFactory->CreateSwapChainForHwnd(
		cmdQueue,
		hwnd,
		&swapchainDesc,
		nullptr,
		nullptr,
		(IDXGISwapChain1**)&swapchain
	);

#pragma endregion

#pragma region レンダーターゲットビュー辺

	//レンダーターゲットビューはデスクリプタヒープから生成されるので
	//デスクリプタヒープを先に生成する

	//各種設定をしてデスクリプタヒープを生成
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;//レンダーターゲットビュー
	heapDesc.NumDescriptors = 2;//裏表の２つ
	dev->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&rtvHeaps));

	//レンダーターゲットビューの生成部分

	//裏表の2つ分について
	std::vector<ID3D12Resource*> backBuffers(2);
	for (int i = 0; i < 2; i++)
	{
		//スワップチェーンからバッファを取得
		result = swapchain->GetBuffer(i, IID_PPV_ARGS(&backBuffers[i]));
		//デスクリプタヒープのハンドルを取得
		D3D12_CPU_DESCRIPTOR_HANDLE handle = rtvHeaps->GetCPUDescriptorHandleForHeapStart();
		//裏か表かでアドレスがずれる
		handle.ptr += i * dev->GetDescriptorHandleIncrementSize(heapDesc.Type);
		//レンダーターゲットビューの生成
		dev->CreateRenderTargetView(
			backBuffers[i],
			nullptr,
			handle
		);

	}

#pragma endregion バックバッファを描画キャンバスとして扱う為のオブジェクト

#pragma region フェンス辺

	//フェンスを生成
	ID3D12Fence* fence = nullptr;
	UINT64 fenceVel = 0;

	result = dev->CreateFence(fenceVel, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));

#pragma endregion CPUとGPUで同期をとるためのDirectXの仕組み

#pragma region DirectInoutオブジェクトの生成

	//DirectInputの初期化
	IDirectInput8* directInput = nullptr;
	result = DirectInput8Create(
		w.hInstance,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&directInput,
		nullptr
	);
	assert(SUCCEEDED(result));

#pragma endregion 全体の初期化であり入力デバイスが増えてもこのオブジェクトは増やさなくてもいい

#pragma region キーボードデバイスの生成

	//キーボードデバイスの生成
	IDirectInputDevice8* keyboard = nullptr;
	result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	assert(SUCCEEDED(result));

#pragma endregion

#pragma region ゲームパッドの入力

	//DIDEVICEINSTANCE* gamePadDID = {};
	//result = directInput->EnumDevices(
	//	DI8DEVTYPE_JOYSTICK,//JOYSTICKだと認識しないことがあるらしい(確認できるなら確認すること)
	//	DeviceFindCallBack,
	//	&gamePadDID,
	//	DIEDFL_ATTACHEDONLY
	//);
	//assert(SUCCEEDED(result));

	//IDirectInputDevice8* gamePadDirct = nullptr;
	//result = directInput->CreateDevice(gamePadDID->guidInstance, &gamePadDirct, NULL);
	//assert(SUCCEEDED(result));


#pragma endregion(中止)

#pragma region 入力データ形式のセット

	result = keyboard->SetDataFormat(&c_dfDIKeyboard);//標準形式
	assert(SUCCEEDED(result));

#pragma endregion


#pragma region 排他制御レベルのセット

	result = keyboard->SetCooperativeLevel(
		hwnd,
		DISCL_FOREGROUND/*画面が手前にある場合のみ入力を受け付ける*/ |
		DISCL_NONEXCLUSIVE/*デバイスをこのアプリだけで占有しない*/ |
		DISCL_NOWINKEY/*windowキーを無効にする*/);
	assert(SUCCEEDED(result));

#pragma endregion


	//DirectX初期化処理　ここまで

#pragma endregion
	
#pragma region 描画初期化処理

	//描画初期化処理ここから
	
#pragma region 頂点データ変数の宣言

	//頂点データ(三点分の座標)
	XMFLOAT3 vertices[] = {
		{-0.5f,-0.5f,0.0f},//左下
		{-0.5f,+0.5f,0.0f},//左上
		{+0.5f,-0.5f,0.0f},//右下
	};

	//頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
	UINT sizeVB = static_cast<UINT>(sizeof(XMFLOAT3) * _countof(vertices));

#pragma endregion

#pragma region 頂点バッファの確保

	//頂点バッファの確保辺
	D3D12_HEAP_PROPERTIES heapprop{};//ヒープ設定
	heapprop.Type = D3D12_HEAP_TYPE_UPLOAD;//GPUへの転送用

	D3D12_RESOURCE_DESC resdesc{};//リソース設定
	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resdesc.Width = sizeVB;//頂点データ全体のサイズ
	resdesc.Height = 1;
	resdesc.DepthOrArraySize = 1;
	resdesc.MipLevels = 1;
	resdesc.SampleDesc.Count = 1;
	resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

#pragma endregion

#pragma region 頂点バッファの生成

	//頂点バッファの生成
	ID3D12Resource* vertBuff = nullptr;
	result = dev->CreateCommittedResource(
		&heapprop,//ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resdesc,//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff)
	);

#pragma endregion

#pragma region 頂点バッファへのデータ転送

	//頂点バッファへのデータ転送

	//GPU上のバッファに対応した仮想メモリを取得
	XMFLOAT3* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	
	//全頂点に対して
	for (int i = 0; i < _countof(vertices); i++)
	{
		vertMap[i] = vertices[i];//座標をコピー
	}


	//つながりを解除
	vertBuff->Unmap(0, nullptr);

#pragma endregion

#pragma region 頂点バッファビューの生成

	//頂点バッファビューの作成(GPUへの道しるべ)辺

	//頂点バッファビューの作成
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeVB;
	vbView.StrideInBytes = sizeof(XMFLOAT3);

#pragma endregion

#pragma region 頂点シェーダファイルの読み込みとコンパイル

	//頂点シェーダファイルの読み込み辺
	ID3DBlob* vsBlob = nullptr;//頂点シェーダオブジェクト
	ID3DBlob* psBlob = nullptr;//ピクセルシェーダオブジェクト
	ID3DBlob* errorBlob = nullptr;//エラーオブジェクト

	//頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"BasicVS.hlsl",//シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,//インクルード可能にする
		"main",//エントリーポイント名
		"vs_5_0",//シェーダモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,//デバック用設定
		0,
		&vsBlob,
		&errorBlob
	);

#pragma endregion ピクセルシェーダオブジェクトとエラーオブジェクトの宣言もここ

#pragma region 頂点シェーダの読み込み時のエラーを表示する場所

	//頂点シェーダの読み込み時のエラーを表示する場所
	if (FAILED(result))
	{
		//errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		//エラー内容をウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

#pragma endregion

#pragma region ピクセルシェーダの読み込みとコンパイル

	//ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"BasicPS.hlsl",//シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,//インクルード可能にする
		"main",//エントリーポイント名
		"ps_5_0",//シェーダモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,//デバック用設定
		0,
		&psBlob,
		&errorBlob
	);

#pragma endregion

#pragma region ピクセルシェーダの読み込み時のエラーを表示する場所

	//ピクセルシェーダの読み込み時のエラーを表示する場所
	if (FAILED(result))
	{
		//errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		//エラー内容をウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

#pragma endregion

#pragma region 頂点レイアウトの設定

	//頂点レイアウト
	//グラフィックパイプラインで頂点一つ分のデータに何を持たせるか決める
	//これは最低限の3D座標だけ持たせているらしい
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{
			"POSITION",//セマンティック名
			0,//同じセマンティック名が複数あるときのインデックス
			DXGI_FORMAT_R32G32B32_FLOAT,//要素数とビット数を返す(XYZの３つでfloat型なのでR32G32B32_FLOAT)
			0,//入力スロットインデックス
			D3D12_APPEND_ALIGNED_ELEMENT,//データのオフセット値(D3D12_APPEND_ALIGNED_ELEMENTだと自動設定)
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,//入力データ識別(標準はD3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA)
			0,//一度に描画するインスタンス数
		},
		//座標以外に　色、テクスチャUVなどを渡す場合はさらに続ける
	};

#pragma endregion

#pragma region グラフィックスパイプライン設定

	//グラフィックスパイプライン辺
	
	//グラフィックスパイプラインの各ステージの設定をする構造体を用意
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};

	//頂点シェーダ、ピクセルシェーダをパイプラインに設定
	gpipeline.VS.pShaderBytecode = vsBlob->GetBufferPointer();
	gpipeline.VS.BytecodeLength = vsBlob->GetBufferSize();
	gpipeline.PS.pShaderBytecode = psBlob->GetBufferPointer();
	gpipeline.PS.BytecodeLength = psBlob->GetBufferSize();

	//サンプルマスクとラスタライザステートの設定
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;//標準設定
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;//カリングしない
	gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;//ポリゴン内塗りつぶし
	gpipeline.RasterizerState.DepthClipEnable = true;//深度クリッピングを有効に

	//ブレンドステートの設定
	gpipeline.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;//RGBA全てのチャンネルを描画

	//頂点レイアウトの設定
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	//図形の形状を三角形に設定
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	//その他の設定
	gpipeline.NumRenderTargets = 1;//描画対象は１つ
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;//0～255指定のRGBA
	gpipeline.SampleDesc.Count = 1;//１ピクセルにつき１回サンプリング

#pragma endregion

#pragma region ルートシグネチャ設定

	//ルートシグネチャの生成
	ID3D12RootSignature* rootsignature;

	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	ID3DBlob* rootSigBlob = nullptr;
	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	result = dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootsignature));
	rootSigBlob->Release();

	//パイプラインにルートシグネチャをセット
	gpipeline.pRootSignature = rootsignature;

#pragma endregion

#pragma region パイプラインステートの生成

	//パイプラインステートの生成
	ID3D12PipelineState* pipelinestate = nullptr;
	result = dev->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelinestate));

#pragma endregion

	//描画初期化処理ここまで

#pragma endregion


	float clearColor[] = { 0.1f,0.25f,0.5f,0.0f };//青っぽい色(画面クリアするときの色)

	//ゲームループ
	while (true)
	{
		//メッセージがある？
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);//キー入力メッセージの処理
			DispatchMessage(&msg);//プロシージャにメッセージを送る
		}

		//×ボタンで終了メッセージが来たらゲームループを抜ける
		if (msg.message == WM_QUIT)
		{
			break;
		}

#pragma region DirectX毎フレーム処理

		//DirectX毎フレーム処理　ここから
		
#pragma region キーボード情報の取得

		//キーボード情報の取得開始
		keyboard->Acquire();

		//全キーの入力情報を取得する
		BYTE key[256] = {};
		keyboard->GetDeviceState(sizeof(key), key);

#pragma endregion

		



		//リソースバリア辺

		//バックバッファの番号を取得(２つなので0番か1番)
		UINT bbIndex = swapchain->GetCurrentBackBufferIndex();

		// 1.リソースバリアで書き込み可能に変更
		D3D12_RESOURCE_BARRIER barrierDesc{};
		barrierDesc.Transition.pResource = backBuffers[bbIndex];//バックバッファを指定
		barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;//表示から
		barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;//描画
		cmdList->ResourceBarrier(1, &barrierDesc);

		// 2.描画先指定
		//レンダーターゲットビュー用デスクリプタヒープのハンドルを取得
		D3D12_CPU_DESCRIPTOR_HANDLE rtvH = rtvHeaps->GetCPUDescriptorHandleForHeapStart();
		rtvH.ptr += bbIndex * dev->GetDescriptorHandleIncrementSize(heapDesc.Type);
		cmdList->OMSetRenderTargets(1, &rtvH, false, nullptr);

		// 3.画面クリア
		
		cmdList->ClearRenderTargetView(rtvH, clearColor, 0, nullptr);

#pragma region 描画コマンド

		// 4.描画コマンドここから
		
		//ビューポート設定
		D3D12_VIEWPORT viewport{};

		viewport.Width = window_width;
		viewport.Height = window_height;
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.MinDepth = 0.1f;
		viewport.MaxDepth = 1.0f;

		cmdList->RSSetViewports(1, &viewport);

		//シザー矩形設定
		D3D12_RECT scissorrect{};

		scissorrect.left = 0;//切り抜き座標左
		scissorrect.right = scissorrect.left + window_width;//切り抜き座標右
		scissorrect.top = 0;//切り抜き座標上
		scissorrect.bottom = scissorrect.top + window_height;//切り抜き座標下

		cmdList->RSSetScissorRects(1, &scissorrect);

		//プリミティブ形状(三角形リスト)
		cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//パイプラインステートとルートシグネチャの設定
		//作ったパイプラインステートとルートシグネチャをセットする
		//決めたルールで描画をお願いするところ
		cmdList->SetPipelineState(pipelinestate);
		cmdList->SetGraphicsRootSignature(rootsignature);

		//頂点バッファビューの設定
		cmdList->IASetVertexBuffers(0, 1, &vbView);

		//描画コマンド
		cmdList->DrawInstanced(3, 1, 0, 0);

		// 4.描画コマンドここまで

#pragma endregion

		// 5.リソースバリアを戻す(書き込める状態だと描画できないので描画できるようにする)
		barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;//描画
		barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;//表示に
		cmdList->ResourceBarrier(1, &barrierDesc);

		//命令のクローズ(ここで描画の命令はそろったので実行にうつす)
		cmdList->Close();
		//コマンドリストの実行
		ID3D12CommandList* cmdLists[] = { cmdList };
		cmdQueue->ExecuteCommandLists(1, cmdLists);

		//画面に表示するバッファをフリップ(裏表の入れ替え)
		swapchain->Present(1, 0);

		//コマンド完了待ち(これを書かないとすぐ処理に移行してしまうため)
		//コマンドの実行完了を待つ
		cmdQueue->Signal(fence, ++fenceVel);
		if (fence->GetCompletedValue() != fenceVel)
		{
			HANDLE event = CreateEvent(nullptr, false, false, nullptr);
			fence->SetEventOnCompletion(fenceVel, event);
			WaitForSingleObject(event, INFINITE);
			CloseHandle(event);
		}

		cmdAllocator->Reset();
		cmdList->Reset(cmdAllocator, nullptr);


		//DirectX毎フレーム処理　ここまで

#pragma endregion

#pragma region 更新処理

		if (key[DIK_D])
		{
			OutputDebugStringA("Hit 0\n");
		}

#pragma endregion

#pragma region 描画処理

#pragma endregion



	}

	//ウィンドウクラスを登録解除
	UnregisterClass(w.lpszClassName, w.hInstance);

	return 0;
}