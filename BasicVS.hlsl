cbuffer ConstBufferDataMaterial2:register(b1)
{
	float4 posm;//ˆÊ’u(XYZ)
}

float4 main( float4 pos : POSITION ) : SV_POSITION
{
	return pos+posm;
}