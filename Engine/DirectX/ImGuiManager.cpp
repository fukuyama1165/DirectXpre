#include "ImGuiManager.h"
#include <cstdint>
#include "WinApp.h"



ImGuiManager::~ImGuiManager()
{

}

ImGuiManager* ImGuiManager::GetInstance()
{
	static ImGuiManager instance;
	return &instance;
}

void ImGuiManager::Init()
{

	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	dsvHeapDesc.NumDescriptors = 1;//深度ビューは1つ
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;//デプスステンシルビュー
	dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	result_ = DirectXInit::GetInstance()->Getdev().Get()->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvHeap_));
	assert(SUCCEEDED(result_));

	ImGui::CreateContext();

	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(WinApp::GetInstance()->getHwnd());
	ImGui_ImplDX12_Init(DirectXInit::GetInstance()->Getdev().Get(), (uint32_t)DirectXInit::GetInstance()->GetBackBufferCount(),
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, dsvHeap_.Get(),
		dsvHeap_->GetCPUDescriptorHandleForHeapStart(),
		dsvHeap_->GetGPUDescriptorHandleForHeapStart());


	ImGuiIO& io = ImGui::GetIO();
	ImFontConfig config;
	config.MergeMode = true;
	io.Fonts->AddFontDefault();
	io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\meiryo.ttc", 18.0f, &config, io.Fonts->GetGlyphRangesJapanese());

	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
}

void ImGuiManager::Finalize()
{

	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	//デスクリプタヒープを解放
	dsvHeap_.Reset();

}

void ImGuiManager::Begin()
{

	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
#ifdef _DEBUG
	ImGuizmo::BeginFrame();
	ImGuizmo::Enable(true);
#endif

}

void ImGuiManager::End()
{

	ImGui::Render();

}

void ImGuiManager::Draw()
{

	ID3D12DescriptorHeap* ppHeaps[] = { dsvHeap_.Get() };

	DirectXInit::GetInstance()->GetcmdList()->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), DirectXInit::GetInstance()->GetcmdList().Get());


}