Texture2D HeightMap : register(t0);
sampler HeightMapSampler : register(s0);
sampler HeightMapSampler1 : register(s01);



struct VS_INPUT
{
    float3 position : POSITION0;
    float2 texcoord : TEXCOORD0;
    float3 normal : NORMAL0;
    float3 tangent : TANGENT0;
    float3 binormal : BINORMAL0;
};


struct VS_OUTPUT
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
}



VS_OUTPUT vsmain(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    
    //Displace vertices through the usage  of height map
    float texelSize = 1.0 / sizeHeightMap;
    
    //Get Y(Height) of current vertex 
    //float height = HeightMap.SampleLevel(HeightMapSampler, float2(input.texcoord.x, 1.0f - input.texcoord.y), 0).r;
    float height = 0;
    
    //Update new world pos
    output.position = mul(float4(input.position.x * TerrainSize.x, height * TerrainSize.y, input.position.z * TerrainSize.z, 1), m_world);
    //output.position.x = input.position.x * TerrainSize.x;
    //output.position.y = height * TerrainSize.y;
    //output.position.z = input.position.z * (TerrainSize.z);
    //output.position.w = 1;
    
    
    //WORLD SPACE
    //output.position = mul(input.position, m_world);
    output.world_pos = output.position.xyz;
    output.direction_to_camera = normalize(output.position.xyz - m_camera_position.xyz);
    
    //VIEW SPACE
    output.position = mul(output.position, m_view);
    //SCREEN SPACE
    output.position = mul(output.position, m_proj);
    
    output.texcoord = input.texcoord;
    //output.normal = normalize(mul(float4(input.normal, 0), m_world));
    //output.normal = normalize(mul(float4(input.normal, 1), m_world));
    output.normal = normalize(mul(input.normal, /*(float3x3)*/m_world));
    
    
    //output.tbn[0] = normalize(mul(input.tangent, m_world));
    //output.tbn[1] = normalize(mul(input.binormal, m_world));
    //output.tbn[2] = normalize(mul(input.normal, m_world));
    
    
    //check
    float m_lightradius = m_light_radius;
    float distortionlevel = distortion_level;
    float4 Terrain_Size = TerrainSize;
    
    
    return output;
}