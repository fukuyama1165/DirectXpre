#pragma once

/**
 * @file PipeLine.h
 * @brief パイプラインを作成して登録し設定したキーで呼び出しを行える
 * @author フクヤマ
 * @date 2023_11/10
 */

#include "Shader.h"
#include<d3d12.h>
#include<dxgi1_6.h>

#include <dxgiformat.h>

struct PipeLineSeting
{
	Shader vShader{};
	Shader pShader{};

	uint32_t sampleMask=0;

	D3D12_RASTERIZER_DESC rasterizerState{};

	std::vector<D3D12_RENDER_TARGET_BLEND_DESC> blendDescs{};

	std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayouts{};

	D3D12_PRIMITIVE_TOPOLOGY_TYPE primitiveTopologyType{};

	uint32_t numRenderTargets=0;

	std::vector <DXGI_FORMAT> RTVFormats{};

	DXGI_SAMPLE_DESC sampleDesc{};

	D3D12_DEPTH_STENCIL_DESC depthDesc{};

	DXGI_FORMAT DSVFormat{};

	ID3D12RootSignature* rootsignature = nullptr;

};

class PipeLine
{
public:

	PipeLineSeting seting;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline_{};

	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelinestate_ = nullptr;

	bool Roadsuccess = false;

	PipeLine() {};
	/// <summary>
	/// 指定して読み込み
	/// </summary>
	/// <param name="pipeLineSeting">作りたい設定</param>
	PipeLine(PipeLineSeting pipeLineSeting);

	//パイプラインを作る
	static PipeLine CreatePipeLine(std::string id, PipeLineSeting pipeLineSeting);

	//指定したidのパイプラインを探す(なければ空のパイプラインを返すので注意)
	static PipeLine SearchPipeLines(std::string id);

	//登録
	static void RegisterPipeLine(std::string id, PipeLine pipeLine);

	//頂点シェーダとピクセルシェーダとインプットレイアウトとルートシグネチャ以外を既定の設定でパイプラインの設定を作成するところ
	static PipeLineSeting defCreatePipeLineSeting(const Shader& vs, const Shader& ps, const std::vector<D3D12_INPUT_ELEMENT_DESC>& inputLayout,ID3D12RootSignature* rootsignature);

private:



	class PipeLines
	{
	public:

		static PipeLines* GetInstance() {
			static PipeLines instance;
			return &instance;
		};

		//パイプラインのデータの連想配列
		std::map<std::string, PipeLine> pipeLines_;


	private:
		PipeLines() = default;
		~PipeLines() {};

		PipeLines(const PipeLines&) = delete;
		PipeLines& operator=(const PipeLines&) = delete;


	};




};