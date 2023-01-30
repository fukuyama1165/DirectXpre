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
	result = directXInit->Getdev().Get()->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvHeap));
	assert(SUCCEEDED(result));

	ImGui::CreateContext();

	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(winApp->getHwnd());
	ImGui_ImplDX12_Init(directXInit->Getdev().Get(), directXInit->GetBackBufferCount(),
		DXGI_FORMAT_R8G8B8A8_UNORM, dsvHeap.Get(),
		dsvHeap->GetCPUDescriptorHandleForHeapStart(),
		dsvHeap->GetGPUDescriptorHandleForHeapStart());
	


}

void ImGuiManager::Finalize()
{

	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	//�f�X�N���v�^�q�[�v�����
	dsvHeap.Reset();

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

	ID3D12DescriptorHeap* ppHeaps[] = { dsvHeap.Get() };

	directXInit->GetcmdList()->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), directXInit->GetcmdList().Get());


}