cbuffer cbPerObject
{
    float4x4 WVP;
};

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float4 Colour : COLOUR;
};

VS_OUTPUT main(float4 inPos : POSITION, float4 inColour : COLOUR)
{
    VS_OUTPUT output;

    output.Pos = mul(inPos, WVP);
    output.Colour = inColour;

    return output;
}