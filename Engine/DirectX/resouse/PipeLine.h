#pragma once
#include "Shader.h"
#include<d3d12.h>
#include<dxgi1_6.h>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

#include <dxgiformat.h>

struct PipeLineSeting
{
	Shader vShader;
	Shader pShader;

	uint32_t sampleMask;

	D3D12_RASTERIZER_DESC rasterizerState;

	std::vector<D3D12_RENDER_TARGET_BLEND_DESC> blendDescs;

	std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayouts;

	D3D12_PRIMITIVE_TOPOLOGY_TYPE PrimitiveTopologyType;

	uint32_t NumRenderTargets;

	std::vector <DXGI_FORMAT> RTVFormats;

	DXGI_SAMPLE_DESC SampleDesc;

};

class PipeLine
{
public:

	PipeLineSeting seting;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline_;

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