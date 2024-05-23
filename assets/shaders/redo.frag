#version 460 core

// Vertex Shader -> Fragment Shader
in vec3 fragment_position;
in vec3 world_space_normal;
in vec2 texture_coordinates;

// CPP -> Fragment Shader
uniform vec3 u_camera_position;
uniform float u_ambient_alpha;
uniform float u_diffuse_alpha;

// Fragment Shader
out vec4 frag_color;

// Material
struct Material {
    vec3 ambient;
    sampler2D textura; // texture unit
    vec3 specular;
    float shininess;
};
uniform Material u_material;

//Directional light
struct DirectionalLight {
    vec3 direction;
    vec3 diffuse;
    vec3 specular;
};
uniform DirectionalLight u_directional_light;

vec4 calcDirectionalLightColor(DirectionalLight directional_light, vec3 normal, vec3 frag2camera) {
    vec3 frag2light = normalize(-directional_light.direction);
    vec4 diffuse = vec4(directional_light.diffuse * max(dot(normal, frag2light), 0.0f), u_diffuse_alpha) * texture(u_material.textura, texture_coordinates);
    vec3 specular = directional_light.specular * u_material.specular * pow(max(dot(normal, normalize(frag2light + frag2camera)), 0.0f), u_material.shininess);
    return diffuse + vec4(specular, 0.0f);
}

//Point lights
#define MAX_POINT_LIGHTS 1
struct PointLight {
    vec3 position;
    vec3 diffuse;
    vec3 specular;
    int on;
    float constant;
    float linear;
    float exponent;
};
uniform PointLight u_point_lights[MAX_POINT_LIGHTS];

vec4 calcPointLightColor(PointLight point_light, vec3 normal, vec3 fragment_position, vec3 frag2camera) {
    vec3 frag2light = normalize(point_light.position - fragment_position);
    vec4 diffuse = vec4(point_light.diffuse * max(dot(normal, frag2light), 0.0f), u_diffuse_alpha) * texture(u_material.textura, texture_coordinates);
    vec3 specular = point_light.specular * u_material.specular * pow(max(dot(normal, normalize(frag2light + frag2camera)), 0.0f), u_material.shininess);
    float d = length(point_light.position - fragment_position);
    float attenuation = 1.0f / (point_light.constant + point_light.linear * d + point_light.exponent * (d * d));
    diffuse *= attenuation;
    specular *= attenuation;
    return diffuse + vec4(specular, 0.0f);
}

//flashlight
struct Spotlight {
    vec3 position;
    vec3 direction;
    float cos_inner_cone;
    float cos_outer_cone;
    vec3 diffuse;
    vec3 specular;
    bool on;
    float constant;
    float linear;
    float exponent;
};
uniform Spotlight u_spotlight;

vec4 calcSpotLightColor(Spotlight spotlight, vec3 normal, vec3 fragment_position, vec3 frag2camera) {
    vec3 frag2light = normalize(spotlight.position - fragment_position);
    vec4 diffuse = vec4(spotlight.diffuse * max(dot(normal, frag2light), 0.0f), u_diffuse_alpha) * texture(u_material.textura, texture_coordinates);
    vec3 specular = spotlight.specular * u_material.specular * pow(max(dot(normal, normalize(frag2light + frag2camera)), 0.0f), u_material.shininess);

    float d = length(spotlight.position - fragment_position);
    float attenuation = 1.0f / (spotlight.constant + spotlight.linear * d + spotlight.exponent * (d * d));
    float spotIntensity = smoothstep(spotlight.cos_outer_cone, spotlight.cos_inner_cone, dot(-frag2light, normalize(spotlight.direction)));

    diffuse *= attenuation * spotIntensity;
    specular *= attenuation * spotIntensity;    
    return diffuse + vec4(specular, 0.0f);
}

void main() {
    vec3 normal = normalize(world_space_normal);
    vec3 frag2camera = normalize(u_camera_position - fragment_position);
    vec4 out_color = vec4(0.0f);

    // Ambient light
    vec4 ambient = vec4(u_material.ambient, u_ambient_alpha) * texture(u_material.textura, texture_coordinates);

    // Directional light
    out_color += calcDirectionalLightColor(u_directional_light, normal, frag2camera);

    // Point lights
    for (int i = 0; i < MAX_POINT_LIGHTS; i++) {
        if (u_point_lights[i].on == 1) {
            out_color += calcPointLightColor(u_point_lights[i], normal, fragment_position, frag2camera);
        }
    }

    // Spotlight
    if (u_spotlight.on) {
        out_color += calcSpotLightColor(u_spotlight, normal, fragment_position, frag2camera);
    }

    // Output
    frag_color = ambient + out_color;
}
