#include "PipeLine.h"



PipeLine::PipeLine(PipeLineSeting pipeLineSeting)
{
#pragma region グラフィックスパイプライン設定

	//グラフィックスパイプライン辺

	//頂点シェーダ、ピクセルシェーダをパイプラインに設定
	gpipeline_.VS.pShaderBytecode = pipeLineSeting.vShader.blob_->GetBufferPointer();
	gpipeline_.VS.BytecodeLength = pipeLineSeting.vShader.blob_->GetBufferSize();
	gpipeline_.PS.pShaderBytecode = pipeLineSeting.pShader.blob_->GetBufferPointer();
	gpipeline_.PS.BytecodeLength = pipeLineSeting.pShader.blob_->GetBufferSize();

	//サンプルマスクとラスタライザステートの設定
	gpipeline_.SampleMask = pipeLineSeting.sampleMask;//標準設定
	gpipeline_.RasterizerState = pipeLineSeting.rasterizerState;


	//ブレンドステートの設定
	for (uint16_t i = 0; i < pipeLineSeting.blendDescs.size(); i++)
	{
		gpipeline_.BlendState.RenderTarget[i] = pipeLineSeting.blendDescs[i];
	}
	
	//頂点レイアウトの設定
	gpipeline_.InputLayout.pInputElementDescs = pipeLineSeting.inputLayouts.data();
	gpipeline_.InputLayout.NumElements = (uint32_t)pipeLineSeting.inputLayouts.size();

	//図形の形状を設定
	gpipeline_.PrimitiveTopologyType = pipeLineSeting.PrimitiveTopologyType;

	//その他の設定
	//描画対象の数を設定
	gpipeline_.NumRenderTargets = (uint32_t)pipeLineSeting.RTVFormats.size();

	//描画設定を画面の数だけ用意?
	for (uint16_t i = 0; i < pipeLineSeting.RTVFormats.size(); i++)
	{
		gpipeline_.RTVFormats[i] = pipeLineSeting.RTVFormats[i];
	}

	//１ピクセルにつき何回サンプリングするか
	gpipeline_.SampleDesc.Count = pipeLineSeting.SampleDesc.Count;

#pragma endregion 
}

PipeLine PipeLine::CreatePipeLine(std::string id, PipeLineSeting pipeLineSeting)
{
	PipeLine newPipeLine = SearchPipeLines(id);

	if (!newPipeLine.Roadsuccess)
	{
		newPipeLine = PipeLine(pipeLineSeting);
	}

	RegisterPipeLine(id, newPipeLine);

	return newPipeLine;
}

PipeLine PipeLine::SearchPipeLines(std::string id)
{
	//一回読み込んだことがあるファイルはそのまま返す
	auto itr = std::find_if(PipeLines::GetInstance()->pipeLines_.begin(), PipeLines::GetInstance()->pipeLines_.end(), [&](const std::pair<std::string, PipeLine>& p) {
		return p.first == id;//条件
		});
	//見つかったらそれを返す
	if (itr != PipeLines::GetInstance()->pipeLines_.end()) {
		return PipeLines::GetInstance()->pipeLines_[id];
	}

	return PipeLine();
}

void PipeLine::RegisterPipeLine(std::string id, PipeLine pipeLine)
{
	PipeLines::GetInstance()->pipeLines_[id] = pipeLine;
}