#pragma once
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
	/// �w�肵�ēǂݍ���
	/// </summary>
	/// <param name="pipeLineSeting">��肽���ݒ�</param>
	PipeLine(PipeLineSeting pipeLineSeting);

	//�p�C�v���C�������
	static PipeLine CreatePipeLine(std::string id, PipeLineSeting pipeLineSeting);

	//�w�肵��id�̃p�C�v���C����T��(�Ȃ���΋�̃p�C�v���C����Ԃ��̂Œ���)
	static PipeLine SearchPipeLines(std::string id);

	//�o�^
	static void RegisterPipeLine(std::string id, PipeLine pipeLine);

	static PipeLineSeting defCreatePipeLineSeting(const Shader& vs, const Shader& ps, const std::vector<D3D12_INPUT_ELEMENT_DESC>& inputLayout,ID3D12RootSignature* rootsignature);

private:



	class PipeLines
	{
	public:

		static PipeLines* GetInstance() {
			static PipeLines instance;
			return &instance;
		};

		//�p�C�v���C���̃f�[�^�̘A�z�z��
		std::map<std::string, PipeLine> pipeLines_;


	private:
		PipeLines() = default;
		~PipeLines() {};

		PipeLines(const PipeLines&) = delete;
		PipeLines& operator=(const PipeLines&) = delete;


	};




};