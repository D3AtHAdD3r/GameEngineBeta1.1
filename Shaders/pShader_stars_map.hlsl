

Texture2D Texture : register(t0);
sampler TextureSampler : register(s0);


struct PS_INPUT
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD0;
    float3 normal : NORMAL0;
    float3 world_pos : TEXCOORD1; //needed for pointlight
    float3 direction_to_camera : TEXCOORD2; //needed for directional light
    row_major float3x3 tbn : TEXCOORD3;
};


cbuffer constant : register(b0)
{
    row_major float4x4 m_world;
    row_major float4x4 m_view;
    row_major float4x4 m_proj;
    float4 m_light_direction;
    float4 m_camera_position;
    float4 m_light_position;
    float4 TerrainSize;
    float m_light_radius;
    float m_time;
    float distortion_level;
    float sizeHeightMap;
};

float4 psmain(PS_INPUT input) : SV_Target
{
    return Texture.Sample(TextureSampler, input.texcoord);
}