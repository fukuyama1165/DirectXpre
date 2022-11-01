#include "DrawingObj.h"



using namespace Microsoft::WRL;





DrawingObj::DrawingObj(const float windowWidth,const float windowHeight)
{
	/*Win_width = windowWidth;
	Win_height = windowHeight;

	texture = Texture::GetInstance();*/
	

	//���_�f�[�^(�l�_���̍��W)
					//  x     y    z   �@��  u    v
	//�O
	//vertices[0] = { { -50.0f,-50.0f,50.0f },{},{0.0f,1.0f}};//����
	//vertices[1] = { {  50.0f,-50.0f,50.0f },{},{0.0f,0.0f} };//����
	//vertices[2] = { { -50.0f, 50.0f,50.0f },{},{1.0f,1.0f} };//�E��
	//vertices[3] = { {  50.0f, 50.0f,50.0f },{},{1.0f,0.0f} };//�E��

	////��
	//vertices[4] = { { -50.0f,-50.0f,-50.0f },{},{0.0f,1.0f} };//����
	//vertices[5] = { { -50.0f, 50.0f,-50.0f },{},{0.0f,0.0f} };//����
	//vertices[6] = { {  50.0f,-50.0f,-50.0f },{},{1.0f,1.0f} };//�E��
	//vertices[7] = { {  50.0f, 50.0f,-50.0f },{},{1.0f,0.0f} };//�E��

	////��
	//vertices[8]  = { { -50.0f,-50.0f,-50.0f },{},{0.0f,1.0f} };//����
	//vertices[9]  = { { -50.0f,-50.0f, 50.0f },{},{0.0f,0.0f} };//����
	//vertices[10] = { { -50.0f, 50.0f,-50.0f },{},{1.0f,1.0f} };//�E��
	//vertices[11] = { { -50.0f, 50.0f, 50.0f },{},{1.0f,0.0f} };//�E��

	////�E
	//vertices[12] = { { 50.0f,-50.0f,-50.0f },{},{0.0f,1.0f} };//����
	//vertices[13] = { { 50.0f, 50.0f,-50.0f },{},{0.0f,0.0f} };//����
	//vertices[14] = { { 50.0f,-50.0f, 50.0f },{},{1.0f,1.0f} };//�E��
	//vertices[15] = { { 50.0f, 50.0f, 50.0f },{},{1.0f,0.0f} };//�E��

	////��
	//vertices[16] = { { -50.0f,50.0f,-50.0f },{},{0.0f,1.0f} };//����
	//vertices[17] = { { -50.0f,50.0f, 50.0f },{},{0.0f,0.0f} };//����
	//vertices[18] = { {  50.0f,50.0f,-50.0f },{},{1.0f,1.0f} };//�E��
	//vertices[19] = { {  50.0f,50.0f, 50.0f },{},{1.0f,0.0f} };//�E��

	////��
	//vertices[20] = { { -50.0f,-50.0f,-50.0f },{},{0.0f,1.0f} };//����
	//vertices[21] = { {  50.0f,-50.0f,-50.0f },{},{0.0f,0.0f} };//����
	//vertices[22] = { { -50.0f,-50.0f, 50.0f },{},{1.0f,1.0f} };//�E��
	//vertices[23] = { {  50.0f,-50.0f, 50.0f },{},{1.0f,0.0f} };//�E��

	////�C���f�b�N�X�f�[�^
	////�O
	//indices[0] = 0;
	//indices[1] = 1;
	//indices[2] = 2;
	//indices[3] = 2;
	//indices[4] = 1;
	//indices[5] = 3;

	////��
	//indices[6] = 4;
	//indices[7] = 5;
	//indices[8] = 6;
	//indices[9] = 6;
	//indices[10] = 5;
	//indices[11] = 7;

	////��
	//indices[12] = 8;
	//indices[13] = 9;
	//indices[14] = 10;
	//indices[15] = 10;
	//indices[16] = 9;
	//indices[17] = 11;

	////�E
	//indices[18] = 12;
	//indices[19] = 13;
	//indices[20] = 14;
	//indices[21] = 14;
	//indices[22] = 13;
	//indices[23] = 15;

	////��
	//indices[24] = 16;
	//indices[25] = 17;
	//indices[26] = 18;
	//indices[27] = 18;
	//indices[28] = 17;
	//indices[29] = 19;

	////��
	//indices[30] = 20;
	//indices[31] = 21;
	//indices[32] = 22;
	//indices[33] = 22;
	//indices[34] = 21;
	//indices[35] = 23;
	//
	//
	////���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
	//sizeVB = static_cast<UINT>(sizeof(vertices[0]) * _countof(vertices));
	////�C���f�b�N�X�f�[�^�S�̂̃T�C�Y
	//sizeIB = static_cast<UINT>(sizeof(uint16_t) * _countof(indices));

	////�r���[�ϊ��s��
	//eye_ = { 0, 0, -400.0f };//���_���W
	//target_ = { 0, 0, 0 };//�����_���W
	//up_ = { 0, 1, 0 };//������x�N�g��
}

DrawingObj::DrawingObj(const float windowWidth, const float windowHeight,XMFLOAT3 vertexPos1, XMFLOAT3 vertexPos2, XMFLOAT3 vertexPos3, XMFLOAT3 vertexPos4, XMFLOAT2 vertexUv1, XMFLOAT2 vertexUv2, XMFLOAT2 vertexUv3, XMFLOAT2 vertexUv4)
{
	/*Win_width = windowWidth;
	Win_height = windowHeight;

	texture = Texture::GetInstance();;*/

	//���_�f�[�^(�l�_���̍��W)
					//  x     y    z      u    v
	//vertices[0] = { vertexPos1,{},vertexUv1 };//����
	//vertices[1] = { vertexPos2,{},vertexUv2 };//����
	//vertices[2] = { vertexPos3,{},vertexUv3 };//�E��
	//vertices[3] = { vertexPos4,{},vertexUv4 };//�E��

	////�C���f�b�N�X�f�[�^
	//indices[0] = 0;
	//indices[1] = 1;
	//indices[2] = 2;
	//indices[3] = 1;
	//indices[4] = 2;
	//indices[5] = 3;



	//���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
	//sizeVB = static_cast<UINT>(sizeof(vertices[0]) * _countof(vertices));
	////�C���f�b�N�X�f�[�^�S�̂̃T�C�Y
	//sizeIB = static_cast<UINT>(sizeof(uint16_t) * _countof(indices));

	////�r���[�ϊ��s��
	//eye_ = { 0, 0, -100 };//���_���W
	//target_ = { 0, 0, 0 };//�����_���W
	//up_ = { 0, 1, 0 };//������x�N�g��
}


DrawingObj::~DrawingObj()
{
	
}




//void DrawingObj::constantBuffGeneration1(ID3D12Device* dev)
//{
//#pragma region �萔�o�b�t�@
//
//	//�萔�o�b�t�@�̐����p�̐ݒ�
//	//�q�[�v�ݒ�
//	//D3D12_HEAP_PROPERTIES cbHeapProp{};
//	//cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;//GPU�ւ̓]���p
//
//
//	////���\�[�X�ݒ�
//	//D3D12_RESOURCE_DESC cbResourceDesc{};
//
//	//cbResourceDesc = constBuffResourceGeneration(sizeof(ConstBufferDataTransform));
//
//	////�萔�o�b�t�@�̐���
//	//result = dev->CreateCommittedResource(
//	//	&cbHeapProp,//�q�[�v�ݒ�
//	//	D3D12_HEAP_FLAG_NONE,
//	//	&cbResourceDesc,//���\�[�X�ݒ�
//	//	D3D12_RESOURCE_STATE_GENERIC_READ,
//	//	nullptr,
//	//	IID_PPV_ARGS(&constBuffTransform1)
//	//);
//	//assert(SUCCEEDED(result));
//
//
//	////�萔�o�b�t�@�̃}�b�s���O
//
//	//result = constBuffTransform1->Map(0, nullptr, (void**)&constMapTransform1);//�}�b�s���O
//	//assert(SUCCEEDED(result));
//
//	////�s��ɒP�ʍs�����
//	///*matWorld.IdentityMatrix();
//
//	//matScale.IdentityMatrix();
//
//	//matRotate.IdentityMatrix();
//
//	//matTrans.IdentityMatrix();
//
//	//constMapTransform->mat.IdentityMatrix();*/
//
//	///*constMapTransform->mat.r[0].m128_f32[0] = 2.0f / Win_width;
//	//constMapTransform->mat.r[1].m128_f32[1] = -2.0f / Win_height;
//	//constMapTransform->mat.r[3].m128_f32[0] = -1.0f;
//	//constMapTransform->mat.r[3].m128_f32[1] = 1.0f;*/
//
//	////���s���ˍs��̌v�Z
//	//constMapTransform->mat = XMMatrixOrthographicOffCenterLH(0.0f,Win_width,Win_height, 0.0f, 0.0f, 1.0f);
//
//	//matWorldUpdata1();
//
//	//constTransformMatUpdata1();
//
//#pragma endregion
//}


void DrawingObj::imageDataGeneration()
{
#pragma region �摜�C���[�W�f�[�^�̍쐬

	//���삵���C���[�W�f�[�^
	////�������s�N�Z����
	//const size_t textureWidth = 256;
	////�c�����s�N�Z����
	//const size_t textureHeight = 256;
	////�z��̗v�f��
	//const size_t imageDataCount = textureWidth * textureHeight;
	////�摜�C���[�W�f�[�^�z��
	//XMFLOAT4* imageData = new XMFLOAT4[imageDataCount];//�K����ŉ������

	//for (size_t i = 0; i < imageDataCount; i++)
	//{
	//	imageData[i].x = 0.0f;//R
	//	imageData[i].y = 1.0f;//G
	//	imageData[i].z = 0.0f;//B
	//	imageData[i].w = 1.0f;//A

	//}

	//�摜�ǂݍ��݂��ĉ摜�C���[�W�f�[�^�𐶐�
	

	////WIC�e�N�X�`���̃��[�h
	//result = LoadFromWICFile(
	//	L"Resources/hokehoke.png",
	//	WIC_FLAGS_NONE,
	//	&metadata,
	//	scratchImg
	//);

	//ScratchImage mipChain{};
	////�~�b�v�}�b�v�̐���
	//result = GenerateMipMaps(
	//	scratchImg.GetImages(),
	//	scratchImg.GetImageCount(),
	//	scratchImg.GetMetadata(),
	//	TEX_FILTER_DEFAULT,
	//	0,
	//	mipChain
	//);

	//if (SUCCEEDED(result))
	//{
	//	scratchImg = std::move(mipChain);
	//	metadata = scratchImg.GetMetadata();
	//}

	////�ǂݍ��񂾃f�B�t���[�Y�e�N�X�`����SRGB�Ƃ��Ĉ���
	//metadata.format = MakeSRGB(metadata.format);

#pragma endregion
}

void DrawingObj::textureBuffGeneraion(ID3D12Device* dev)
{
#pragma region �e�N�X�`���o�b�t�@�ݒ�

	//�q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	////���\�[�X�ݒ�
	//D3D12_RESOURCE_DESC textureResourceDesc{};
	//textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	//textureResourceDesc.Format = metadata.format;
	//textureResourceDesc.Width = metadata.width;//��
	//textureResourceDesc.Height = (UINT)metadata.height;//����
	//textureResourceDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
	//textureResourceDesc.MipLevels = (UINT16)metadata.mipLevels;
	//textureResourceDesc.SampleDesc.Count = 1;

#pragma endregion

#pragma region �e�N�X�`���o�b�t�@�̐���

	//�e�N�X�`���o�b�t�@�̐���
	//
	//result = dev->CreateCommittedResource(
	//	&textureHeapProp,
	//	D3D12_HEAP_FLAG_NONE,
	//	&textureResourceDesc,
	//	D3D12_RESOURCE_STATE_GENERIC_READ,
	//	nullptr,
	//	IID_PPV_ARGS(&texBuff)
	//);

	////�~�b�v�}�b�v�Œu��������
	////�S�~�b�v�}�b�v�ɂ���
	//for (size_t i = 0; i < metadata.mipLevels; i++)
	//{
	//	//�~�b�v�}�b�v���x�����w�肵�ăC���[�W���擾
	//	const Image* img = scratchImg.GetImage(i, 0, 0);
	//	//�e�N�X�`���o�b�t�@�Ƀf�[�^�]��
	//	result = texBuff->WriteToSubresource(
	//		(UINT)i,
	//		nullptr,//�S�̈�փR�s�[
	//		img->pixels,//���f�[�^�A�h���X
	//		(UINT)img->rowPitch,//1���C���A�h���X
	//		(UINT)img->slicePitch//1���T�C�Y
	//	);
	//	assert(SUCCEEDED(result));
	//}

	//result = texBuff->WriteToSubresource(
	//	0,
	//	nullptr,//�S�̈�փR�s�[
	//	imageData,//���f�[�^�A�h���X
	//	sizeof(XMFLOAT4) * textureWidth,//1���C���T�C�Y
	//	sizeof(XMFLOAT4) * imageDataCount//�S�T�C�Y
	//);

	//delete[] imageData;

#pragma endregion �����ŉ摜�C���[�W�f�[�^��delete���Ă���(�R�����g�A�E�g��)
}

void DrawingObj::SRVGeneraion(ID3D12Device* dev)
{
#pragma region �V�F�[�_�[���\�[�X�r���[�ׂ̈̃f�X�N���v�^�q�[�v����

	//SRV�̍ő��
	const size_t kMaxSRVCount = 2056;

	//�f�X�N���v�^�q�[�v�̐ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//�V�F�[�_���猩����悤��
	srvHeapDesc.NumDescriptors = kMaxSRVCount;

	//�ݒ�����Ƃ�SRV�p�f�X�N���v�^�q�[�v�𐶐�
	
	//result = dev->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap));
	//assert(SUCCEEDED(result));

	////�f�X�N���v�^�n���h��(�q�[�v���̑��삷��ꏊ�w��Ɏg��)
	////SRV�q�[�v�̐擪�n���h�����擾
	//D3D12_CPU_DESCRIPTOR_HANDLE srvHandle = srvHeap->GetCPUDescriptorHandleForHeapStart();

#pragma endregion

#pragma region �V�F�[�_���\�[�X�r���[

	//�V�F�[�_���\�[�X�r���[
	//D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};//�ݒ�\����
	//srvDesc.Format = resDesc.Format;//RGBA float
	//srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	//srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2D�e�N�X�`��
	//srvDesc.Texture2D.MipLevels = resDesc.MipLevels;

	//�n���h���̎w���ʒu�ɃV�F�[�_���\�[�X�r���[�쐬
	//dev->CreateShaderResourceView(texBuff.Get(), &srvDesc, srvHandle);

#pragma endregion
}

void DrawingObj::vertexMap()
{

	//vertBuff->Map(0, nullptr, (void**)&vertMap);
	////�S���_�ɑ΂���
	//for (int i = 0; i < _countof(vertices); i++)
	//{
	//	vertMap[i] = vertices[i];//���W���R�s�[
	//}

	////�Ȃ��������
	//vertBuff->Unmap(0, nullptr);
}

void DrawingObj::Draw(ID3D12GraphicsCommandList* cmdList,bool PipeLineRuleFlag, bool ChangeSquareFlag,bool ChangeTexure)
{

	//�p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐ݒ�
	//������p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`�����Z�b�g����
	//���߂����[���ŕ`������肢����Ƃ���
	//D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle;

	//if (PipeLineRuleFlag)
	//{
	//	cmdList->SetPipelineState(pipelinestate.Get());
	//}
	//else
	//{
	//	cmdList->SetPipelineState(pipelinestate2.Get());
	//}

	//cmdList->SetGraphicsRootSignature(rootsignature.Get());

	////���_�o�b�t�@�r���[�̐ݒ�
	//cmdList->IASetVertexBuffers(0, 1, &vbView);



	////�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
	//cmdList->SetGraphicsRootConstantBufferView(0, constBuffMaterial->GetGPUVirtualAddress());
	//cmdList->SetGraphicsRootConstantBufferView(1, constBuffMaterial2->GetGPUVirtualAddress());


	//SRV�q�[�v�̐ݒ�R�}���h
	//cmdList->SetDescriptorHeaps(1, &texture.srvHeap);
	////SRV�q�[�v�̐擪�n���h�����擾(SRV���w���Ă���͂�)
	//srvGpuHandle = srvHeap->GetGPUDescriptorHandleForHeapStart();
	////SRV�q�[�v�̐擪�ɂ���SRV�����[�g�p�����[�^�Q�Ԃɐݒ�
	//cmdList->SetGraphicsRootDescriptorTable(2, srvGpuHandle);

	/*texture->Draw(cmdList, ChangeTexure);

	cmdList->IASetIndexBuffer(&ibView);

	for (int i = 0; i < _countof(object3Ds); i++)
	{
		object3Ds[0].Draw(cmdList, vbView, ibView, _countof(indices),ChangeSquareFlag);
	}*/

	////�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h(��ԍŏ��̈�����"���[�g�p�����[�^"�̗v�f�ԍ��ł���)
	//cmdList->SetGraphicsRootConstantBufferView(3, constBuffTransform0->GetGPUVirtualAddress());

	////�`��R�}���h
	//if (ChangeSquareFlag)
	//{
	//	//�l�p�`�ɕ`��
	//	cmdList->DrawIndexedInstanced(_countof(indices), 1, 0, 0, 0);
	//}
	//else
	//{
	//	cmdList->DrawInstanced(3, 1, 0, 0);
	//}

	////�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h(��ԍŏ��̈�����"���[�g�p�����[�^"�̗v�f�ԍ��ł���)
	//cmdList->SetGraphicsRootConstantBufferView(3, constBuffTransform1->GetGPUVirtualAddress());

	////�`��R�}���h
	//if (ChangeSquareFlag)
	//{
	//	//�l�p�`�ɕ`��
	//	cmdList->DrawIndexedInstanced(_countof(indices), 1, 0, 0, 0);
	//}
	//else
	//{
	//	cmdList->DrawInstanced(3, 1, 0, 0);
	//}
}

XMFLOAT3 DrawingObj::Afin(XMFLOAT3 box, float moveX, float moveY, float rotate, float scaleX, float scaleY)
{
	//float ansBuff[3] = {};
	//float ansBuff2[3] = {};
	//float ansBuff3[3] = {};
	//float ansBuff4[3] = {};
	//float ans[3] = {};

	//float moveA[3][3] = {
	//	{1.0f,0.0f, moveX},
	//	{0.0f,1.0f, moveY},
	//	{0.0f,0.0f, 1.0f}
	//};
	//float rotateA[3][3] = {
	//	{cosf(rotate * (PI / 180)),-sinf(rotate * (PI / 180)), 0.0f},
	//	{sinf(rotate * (PI / 180)),cosf(rotate * (PI / 180)), 0.0f},
	//	{0.0f,0.0f, 1.0f}
	//};

	//float scaleA[3][3] = {
	//	{scaleX,0.0f, 0.0f},
	//	{0.0f,scaleY, 0.0f},
	//	{0.0f,0.0f, 1.0f}
	//};

	//ans[0] = box.x;
	//ans[1] = box.y;
	//ans[2] = 1.0f;

	///*ansBuff[0] = moveA[0][0] * vertices[0].pos.x + moveA[0][1] * vertices[0].pos.y + -ans[0] * 1.0f;
	//ansBuff[1] = moveA[1][0] * vertices[0].pos.x + moveA[1][1] * vertices[0].pos.y + -ans[1] * 1.0f;
	//ansBuff[2] = moveA[2][0] * vertices[0].pos.x + moveA[2][1] * vertices[0].pos.y + ans[2] * 1.0f;*/


	//ansBuff2[0] = rotateA[0][0] * ansBuff[0] + rotateA[0][1] * ansBuff[1] + rotateA[0][2] * ansBuff[2];
	//ansBuff2[1] = rotateA[1][0] * ansBuff[0] + rotateA[1][1] * ansBuff[1] + rotateA[1][2] * ansBuff[2];
	//ansBuff2[2] = rotateA[2][0] * ansBuff[0] + rotateA[2][1] * ansBuff[1] + rotateA[2][2] * ansBuff[2];

	//ansBuff3[0] = scaleA[0][0] * ansBuff2[0] + scaleA[0][1] * ansBuff2[1] + scaleA[0][2] * ansBuff2[2];
	//ansBuff3[1] = scaleA[1][0] * ansBuff2[0] + scaleA[1][1] * ansBuff2[1] + scaleA[1][2] * ansBuff2[2];
	//ansBuff3[2] = scaleA[2][0] * ansBuff2[0] + scaleA[2][1] * ansBuff2[1] + scaleA[2][2] * ansBuff2[2];

	//ansBuff4[0] = moveA[0][0] * ansBuff3[0] + moveA[0][1] * ansBuff3[1] + ans[0] * ansBuff3[2];
	//ansBuff4[1] = moveA[1][0] * ansBuff3[0] + moveA[1][1] * ansBuff3[1] + ans[1] * ansBuff3[2];
	//ansBuff4[2] = moveA[2][0] * ansBuff3[0] + moveA[2][1] * ansBuff3[1] + ans[2] * ansBuff3[2];

	//box.x = moveA[0][0] * ansBuff4[0] + moveA[0][1] * ansBuff4[1] + moveA[0][2] * ansBuff4[2];
	//box.y = moveA[1][0] * ansBuff4[0] + moveA[1][1] * ansBuff4[1] + moveA[1][2] * ansBuff4[2];

	return box;

}

//Obj�ɑ΂��ăA�t�B���ϊ���������֐�
void DrawingObj::ObjAfin(float moveX, float moveY, float rotate, float scaleX, float scaleY)
{
#pragma region �A�t�B���ϊ�

	//���_�͍Ō�ɕϊ�����(�����W���ς���Ă��܂�����)
	/*vertices[1].pos = Afin(vertices[1].pos, moveX, moveY, rotate, scaleX, scaleY);
	vertices[2].pos = Afin(vertices[2].pos, moveX, moveY, rotate, scaleX, scaleY);
	vertices[3].pos = Afin(vertices[3].pos, moveX, moveY, rotate, scaleX, scaleY);
	vertices[0].pos = Afin(vertices[0].pos, moveX, moveY, rotate, scaleX, scaleY);*/

#pragma endregion

#pragma region �A�t�B���ϊ���̒��_�f�[�^���V�F�[�_�ɓ]��

	vertexMap();

#pragma endregion
}

//�萔�o�b�t�@��ύX����֐�
void DrawingObj::constBuffColorUpdata(float Red, float Green, float Blue)
{
	//constMapMaterial->color = XMFLOAT4(Red, Green, Blue, 1.0f);
}

void DrawingObj::constBuffPosMUpdata(float X, float Y, float Z)
{
	//constMapMaterial2->posM = XMFLOAT4(X, Y, Z, 0.0f);
}

void DrawingObj::matViewUpdata(Float3 eye, Float3 target, Float3 up)
{
	//eye_ = eye;
	//target_ = target;
	//up_ = up;

	///*matView = XMMatrixLookAtLH(XMLoadFloat3(&eye_), XMLoadFloat3(&target_), XMLoadFloat3(&up_));*/
	//matView = matViewGeneration(eye_, target_, up_);

	//for (size_t i = 0; i < _countof(object3Ds); i++)
	//{
	//	object3Ds[i].Update(matView, matProjection);
	//}
	
}

void DrawingObj::constTransformMatUpdata()
{
	//constMapTransform0->mat = matWorld * matView * matProjection;
}

void DrawingObj::constTransformMatUpdata1()
{
	//constMapTransform1->mat = matWorld1 * matView * matProjection;
}

void DrawingObj::matWorldUpdata()
{

	////�X�P�[���s��X�V
	//matScale = matScaleGeneration(Scale_);

	////��]�s��X�V
	//matRotate = matRotateGeneration(Rotate_);

	////���s�ړ��s��X�V
	//matTrans = matMoveGeneration(Trans_);

	////���[���h�s��X�V
	//matWorld.IdentityMatrix();
	//matWorld *= matScale*matRotate*matTrans;

	/*for (size_t i = 0; i < _countof(object3Ds); i++)
	{
		object3Ds[i].Update(matView, matProjection);
	}*/
	
}

void DrawingObj::matWorldUpdata1()
{

	

}


void DrawingObj::SetScale(Float3 scale)
{
	//object3Ds[0].SetScale(scale);
}

void DrawingObj::SetRotate(Float3 rotate)
{
	//object3Ds[0].SetRotate(rotate);
}

void DrawingObj::SetTrans(Float3 TransForm)
{
	//object3Ds[0].SetPos(TransForm);
}

void DrawingObj::obj3DUpdate()
{
	/*for (size_t i = 0; i < _countof(object3Ds); i++)
	{
		object3Ds[i].Update(matView, matProjection);
	}*/
}

Matrix4x4 DrawingObj::matScaleGeneration(Float3 scale)
{
	//�X�P�[�����O�s���錾
	Matrix4x4 matScale;
	matScale.IdentityMatrix();

	//�X�P�[�����O�{�����s��ɐݒ�
	matScale.m[0][0] = scale.x;
	matScale.m[1][1] = scale.y;
	matScale.m[2][2] = scale.z;
	matScale.m[3][3] = 1;

	return matScale;
}

Matrix4x4 DrawingObj::matRotateXGeneration(float rotateX)
{
	//X����]�s���錾
	Matrix4x4 matRotateX;
	matRotateX.IdentityMatrix();

	//��]�p���s��ɐݒ�(���W�A��)
	matRotateX.m[0][0] = 1;
	matRotateX.m[1][1] = cosf(rotateX);
	matRotateX.m[1][2] = sinf(rotateX);
	matRotateX.m[2][1] = -sinf(rotateX);
	matRotateX.m[2][2] = cosf(rotateX);
	matRotateX.m[3][3] = 1;

	return matRotateX;
}

Matrix4x4 DrawingObj::matRotateYGeneration(float rotateY)
{
	//Y����]�s���錾
	Matrix4x4 matRotateY;
	matRotateY.IdentityMatrix();

	//��]�p���s��ɐݒ�(���W�A��)
	matRotateY.m[0][0] = cosf(rotateY);
	matRotateY.m[0][2] = -sinf(rotateY);
	matRotateY.m[1][1] = 1;
	matRotateY.m[2][0] = sinf(rotateY);
	matRotateY.m[2][2] = cosf(rotateY);
	matRotateY.m[3][3] = 1;

	return matRotateY;
}

Matrix4x4 DrawingObj::matRotateZGeneration(float rotateZ)
{
	//Z����]�s���錾
	Matrix4x4 matRotateZ;
	matRotateZ.IdentityMatrix();

	//��]�p���s��ɐݒ�(���W�A��)
	matRotateZ.m[0][0] = cosf(rotateZ);
	matRotateZ.m[0][1] = sinf(rotateZ);
	matRotateZ.m[1][0] = -sinf(rotateZ);
	matRotateZ.m[1][1] = cosf(rotateZ);
	matRotateZ.m[2][2] = 1;
	matRotateZ.m[3][3] = 1;

	return matRotateZ;
}

Matrix4x4 DrawingObj::matRotateGeneration(Float3 rotate)
{
	//X����]�s���錾
	Matrix4x4 matRotateX = matRotateXGeneration(rotate.x);

	//Y����]�s���錾
	Matrix4x4 matRotateY = matRotateYGeneration(rotate.y);

	//Z����]�s���錾
	Matrix4x4 matRotateZ = matRotateZGeneration(rotate.z);

	//��]�������s���錾
	Matrix4x4 matRotate;
	matRotate.IdentityMatrix();

	//�v�Z�����p�x���v�Z(���Ԃ͉�]�����郂�f���ɂ���ĕς���)

	matRotateX *= matRotateY;

	matRotateZ *= matRotateX;

	matRotate = matRotateZ;

	return matRotate;

	

}

Matrix4x4 DrawingObj::matMoveGeneration(Float3 translation)
{
	//�ړ����邽�߂̍s���p��
	Matrix4x4 matMove;
	matMove.IdentityMatrix();

	//�s��Ɉړ��ʂ���
	matMove.m[3][0] = translation.x;
	matMove.m[3][1] = translation.y;
	matMove.m[3][2] = translation.z;

	return matMove;
}


void DrawingObj::colorMap(float R, float G, float B)
{
	//constMapMaterial->color = XMFLOAT4(R, G, B, 0.5f);
}
