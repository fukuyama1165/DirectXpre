#include "ImGuiManager.h"
#include <imgui.h>
#include "imgui_impl_win32.h"
#include "imgui_impl_dx12.h"


void ImGuiManager::Init(WinApp* winApp)
{
	DirectXInit* directXInit = DirectXInit::GetInstance();

	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	dsvHeapDesc.NumDescriptors = 1;//�[�x�r���[��1��
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;//�f�v�X�X�e���V���r���[
	dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	result_ = directXInit->Getdev().Get()->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvHeap_));
	assert(SUCCEEDED(result_));

	ImGui::CreateContext();

	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(winApp->getHwnd());
	ImGui_ImplDX12_Init(directXInit->Getdev().Get(), (int)directXInit->GetBackBufferCount(),
		DXGI_FORMAT_R8G8B8A8_UNORM, dsvHeap_.Get(),
		dsvHeap_->GetCPUDescriptorHandleForHeapStart(),
		dsvHeap_->GetGPUDescriptorHandleForHeapStart());
	


}

void ImGuiManager::Finalize()
{

	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	//�f�X�N���v�^�q�[�v�����
	dsvHeap_.Reset();

}

void ImGuiManager::Begin()
{

	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

}

void ImGuiManager::End()
{

	ImGui::Render();

}

void ImGuiManager::Draw()
{
	DirectXInit* directXInit = DirectXInit::GetInstance();

	ID3D12DescriptorHeap* ppHeaps[] = { dsvHeap_.Get() };

	directXInit->GetcmdList()->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), directXInit->GetcmdList().Get());


}