#version 330

#define MAX_LIGHTS 100

// Input
in vec2 texcoord;
in vec3 vertex_color;
in vec3 world_position;
in vec3 world_normal;

// Uniform properties
uniform sampler2D texture_1;
uniform vec2 ciclare;

uniform int textureActive;
uniform int ciclareActiva;

// Lighting
struct light_source
{
   int      type;
   vec3     position;
   vec3     color;
   vec3     direction;
   float    cutOff;
   float    intensity;
};

uniform vec3 eye_position;

uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

uniform int nrOfLightSources;
uniform light_source LightSources[MAX_LIGHTS];

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    // Calculare culoare pe baza vertexilor sau a textureii
	vec4 finalColor = vec4(vertex_color, 1);
	if (textureActive == 1)
	{
		vec2 finalTextCoord = texcoord;
		if(ciclareActiva == 1)
			finalTextCoord += ciclare;

		finalColor = texture2D(texture_1, finalTextCoord);

		if(finalColor.w < 0.9f)
		{
			discard;
		}
	}

    // Procesare lumini
    vec3 ambient_light = vec3(0);

    vec3 semiFinalColor = vec3(0);
    vec3 culoareObiect = vec3(0);

//    for(int i = 0; i < nrOfLightSources; i++)
//    {
//        vec3 diffuse_light = vec3(0);
//        vec3 specular_light = vec3(0);
//
//        vec3 N = normalize( world_normal );
//        vec3 L = normalize( LightSources[i].position - world_position );
//        vec3 V = normalize( eye_position - world_position );
//        vec3 H = normalize( L + V );
//
//        float factorAtenuare = 1;
//
//        switch(LightSources[i].type)
//        {
//        // Calculations for directional light
//        case 1:
//            L = LightSources[i].direction;
//            H = normalize( L + V );
//
//            diffuse_light = material_kd * LightSources[i].color * abs(dot(N, L));
//            if (dot(N, L) > 0)
//            {
//                specular_light = material_ks * LightSources[i].color * pow(max(dot(N, H), 0), material_shininess);
//            }
//            break;
//
//        // Calculations for the spot light
//        case 2:
//            float spot_light = dot(-L, LightSources[i].direction);
//            float spot_light_limit = cos(LightSources[i].cutOff);
//
//            if(spot_light > cos(LightSources[i].cutOff))
//            {
//                float linear_att = (spot_light - spot_light_limit) / (1.0f - spot_light_limit);
//                factorAtenuare = pow(linear_att, 2);
//                factorAtenuare *= 1 / (0.1 * pow(distance(world_position, LightSources[i].position), 2) + 1);
//
//                diffuse_light = material_kd * LightSources[i].color * max(dot(N, L), 0);
//                if (dot(N, L) > 0)
//                {
//                    specular_light = material_ks * LightSources[i].color * pow(max(dot(N, H), 0), material_shininess);
//                }
//            }
//            break;
//
//        // Calculations for the point light
//        case 3:
//            diffuse_light = material_kd * LightSources[i].color * max(dot(N, L), 0);
//            if (dot(N, L) > 0)
//            {
//                specular_light = material_ks * LightSources[i].color * pow(max(dot(N, H), 0), material_shininess);
//            }
//            factorAtenuare = 1 / (0.1 * pow(distance(world_position, LightSources[i].position), 2) + 1);
//            break;
//
//        // Cazul default
//         default:
//            factorAtenuare = 0;
//            break;
//        }
//        semiFinalColor += factorAtenuare * ( diffuse_light + specular_light ) * LightSources[i].intensity;
//    }
//
//    culoareObiect = finalColor.xyz * (ambient_light + semiFinalColor);

    out_color = vec4(finalColor.xyz, 1);

}