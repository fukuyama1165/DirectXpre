#pragma once
#include "WinApp.h"
#include "DirectXInit.h"

class ImGuiManager
{
public:
	//������
	void Init(WinApp* winApp);

	//�I��
	void Finalize();

	void Begin();

	void End();

	void Draw();

private:

	HRESULT result_ =S_OK;

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvHeap_ = nullptr;

};


//�T���v���R�[�h
//bool flag = true;
//
////Vector4 color;
//
//float color[4];

//�^�u�̖��O
//ImGui::Begin("My First Tool", &flag, ImGuiWindowFlags_MenuBar);
// �E�B���h�E�̒��̃^�u
//if (ImGui::BeginMenuBar())
//{
//	�^�u��
//	if (ImGui::BeginMenu("File"))
//	{
//		�������Ƃ��̒��g							����
//		if (ImGui::MenuItem("Open..", "Ctrl+O")) { /* Do stuff */ }
//		if (ImGui::MenuItem("Save", "Ctrl+S")) { /* Do stuff */ }
//		if (ImGui::MenuItem("Close", "Ctrl+W")) { flag = false; }
//		ImGui::EndMenu();
//	}
//  
//	ImGui::EndMenuBar();
//}
//
//// Edit a color stored as 4 floats
//ImGui::ColorEdit4("Color", color);
//
//// Generate samples and plot them
//float samples[100];
//for (int n = 0; n < 100; n++)
//	samples[n] = sinf(n * 0.2f + ImGui::GetTime() * 1.5f);
//ImGui::PlotLines("Samples", samples, 100);
//
//// Display contents in a scrolling region
//ImGui::TextColored(ImVec4(1, 1, 0, 1), "Important Stuff");
//ImGui::BeginChild("Scrolling");
//for (int n = 0; n < 50; n++)
//	ImGui::Text("%04d: Some text", n);
//ImGui::EndChild();
//ImGui::End();

