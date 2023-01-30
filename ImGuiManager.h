#pragma once
#include "WinApp.h"
#include "DirectXInit.h"

class ImGuiManager
{
public:
	//初期化
	void Init(WinApp* winApp);

	//終了
	void Finalize();

	void Begin();

	void End();

	void Draw();

private:

	HRESULT result;

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvHeap = nullptr;

};


//サンプルコード
//bool flag = true;
//
////Vector4 color;
//
//float color[4];

//タブの名前
//ImGui::Begin("My First Tool", &flag, ImGuiWindowFlags_MenuBar);
// ウィンドウの中のタブ
//if (ImGui::BeginMenuBar())
//{
//	タブ名
//	if (ImGui::BeginMenu("File"))
//	{
//		押したときの中身							処理
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

