Texture2D grass : register(t0);
sampler Sampler : register(s0);

Texture2D cliff : register(t1);


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
    float sizeHeightMap;
    int Material_id;
    float m_light_radius;
    float m_time;
    float distortion_level;
    
};


float4 psmain(PS_INPUT input) : SV_Target
{
    float4 grassColor = grass.Sample(Sampler, input.texcoord * 30);
    float4 cliffColor = cliff.Sample(Sampler, input.texcoord * 30);
    
    float4 texColor = grassColor;
    
    float angle = abs(dot(input.normal, float3(0, 1, 0)));
    
    float minAngle = 0.5;
    float maxAngle = 0.6;
    
    if(angle >= minAngle && angle <=maxAngle)
        texColor = lerp(cliffColor, grassColor, (angle - minAngle) * (1.0 / (maxAngle - minAngle)));
    
    if(angle < minAngle)
        texColor = cliffColor;
    
    //Ambient light
    float ka = 1.0; //ambient light coefficient
    float3 ia = float3(1.0, 1.0, 1.0); //surrounding color approx rgb value
    ia *= (texColor.rgb);
    float3 ambient_light = ka * ia;
    
    //Diffuse light
    float kd = 0.7;
    float3 id = float3(1.0, 1.0, 1.0);
    id *= (texColor.rgb);
    float ammount_diffuse_light = max(0.0, dot(m_light_direction.xyz, input.normal));
    float3 diffuse_light = kd * ammount_diffuse_light * id;
    
    
    //Specular light
    float ks = 0;
    float3 is = float3(1.0, 1.0, 1.0);
    //is *= (texColor.rgb);
    float shininess = 30.0;
    float3 reflected_light = reflect(m_light_direction.xyz, input.normal);
    
    float amount_specular_light = pow(max(0.0, dot(reflected_light, input.direction_to_camera)), shininess);
    float3 specular_light = ks * amount_specular_light * is;
    
    
    float3 final_light = ambient_light + diffuse_light + specular_light;
    return float4(final_light, 1.0);

}