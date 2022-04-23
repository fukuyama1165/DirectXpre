cbuffer ConstBufferDataMaterial2:register(b1)//定数バッファの番号をbの後ろにつける
{
	float4 posm;//位置(XYZ)
}

float4 main( float4 pos : POSITION ) : SV_POSITION
{
	return pos+posm;
}