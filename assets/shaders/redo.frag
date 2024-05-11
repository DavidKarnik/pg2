#version 460 core

// Inspired by "lighting_dir_point_spot.frag" by Steve Jones, Game Institute

// VS -> FS
in vec3 o_fragment_position;
in vec3 o_normal;
in vec2 o_texture_coordinate;

// CPP -> FS
uniform vec3 u_camera_position;
uniform float u_ambient_alpha;
uniform float u_diffuse_alpha;

// FS ->
out vec4 frag_color;

// Material
struct Material 
{
    vec3 ambient;
    sampler2D textura; // texture unit
    vec3 specular;
    float shininess;
};
uniform Material u_material;

// === Directional light - Sun ===
struct DirectionalLight
{
	vec3 direction;
	vec3 diffuse;
	vec3 specular;
};

uniform DirectionalLight u_directional_light;

vec4 calcDirectionalLightColor(DirectionalLight directional_light, vec3 normal, vec3 frag2camera)
{
	vec3 frag2light = normalize(-directional_light.direction);
    vec4 diffuse = vec4(directional_light.diffuse * max(dot(normal, frag2light), 0.0f), u_diffuse_alpha) * texture(u_material.textura, o_texture_coordinate);
	vec3 specular = directional_light.specular * u_material.specular * pow(max(dot(normal, normalize(frag2light + frag2camera)), 0.0f), u_material.shininess);
	return (diffuse + vec4(specular, 0.0f));
}


// === Spotlight - Flashlight ===
struct Spotlight
{
	vec3 position;
	vec3 direction;
	float cos_inner_cone;
	float cos_outer_cone;
	vec3 diffuse;
	vec3 specular;
	int on;

	float constant;
	float linear;
	float exponent;
};

uniform Spotlight u_spotlight;

vec4 calcSpotLightColor(Spotlight spotlight, vec3 normal, vec3 fragment_position, vec3 frag2camera)
{
	vec3 frag2light = normalize(spotlight.position - fragment_position);
        vec4 diffuse = vec4(u_spotlight.diffuse * max(dot(normal, frag2light), 0.0f), u_diffuse_alpha) * texture(u_material.textura, o_texture_coordinate);
	vec3 specular = u_spotlight.specular * u_material.specular * pow(max(dot(normal, normalize(frag2light + frag2camera)), 0.0f), u_material.shininess);

	float d = length(spotlight.position - fragment_position);
	float attenuation = 1.0f / (spotlight.constant + spotlight.linear * d + spotlight.exponent * (d * d));
	float spotIntensity = smoothstep(spotlight.cos_outer_cone, spotlight.cos_inner_cone, dot(-frag2light, normalize(spotlight.direction)));

	diffuse *= attenuation * spotIntensity;
	specular *= attenuation * spotIntensity;	
	return (diffuse + vec4(specular, 0.0f));
}

// === Main ===
void main()
{
	vec3 normal = normalize(o_normal);
	vec3 frag2camera = normalize(u_camera_position - o_fragment_position);
	vec4 out_color = vec4(0.0f);

	// Ambient light
	vec4 ambient = vec4(u_material.ambient, u_ambient_alpha) * texture(u_material.textura, o_texture_coordinate);

	// Directional light
	out_color += calcDirectionalLightColor(u_directional_light, normal, frag2camera);
 
	// Spotlight
	if (u_spotlight.on == 1) out_color += calcSpotLightColor(u_spotlight, normal, o_fragment_position, frag2camera);

	// Output
	frag_color = ambient + out_color;
}