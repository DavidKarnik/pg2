#version 460 core

// = FINAL OUTPUT =
out vec4 color;

// = UNIFORM VARIABLES =
// Material properties
uniform vec3 ambient_material, diffuse_material, specular_material;
uniform float specular_shinines;
// Texture file from cpp
uniform sampler2D uTexture;

// = FLASHLIGHT =
// === Spotlight ===
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

// = INPUT FROM VERTEX SHADER =
in VS_OUT
{
	vec3 N;
	vec3 L;
	vec3 V;
	vec2 texCoord;
} fs_in;

void main(void)
{
	// Normalize the incoming N, L and V vectors
	vec3 N = normalize(fs_in.N);
	vec3 L = normalize(fs_in.L);
	vec3 V = normalize(fs_in.V);
	
	// Calculate R by reflecting -L around the plane defined by N
	vec3 R = reflect(-L, N);
	
	// Calculate the ambient, diffuse and specular contributions
	vec3 ambient = ambient_material * vec3(1.0);
	vec3 diffuse = max(dot(N, L), 0.0) * diffuse_material;
	vec3 specular = pow(max(dot(R, V), 0.0), specular_shinines) * specular_material;

	// Apply flashlight effect if flashlight is on
	if (flashlightOn) {
		// Calculate distance from fragment to flashlight
		float distanceToFlashlight = distance(flashlightPosition, gl_FragCoord.xyz);

		// Calculate attenuation based on distance
		float attenuation = 1.0 / (1.0 + 0.1 * distanceToFlashlight + 0.01 * distanceToFlashlight * distanceToFlashlight);

		// Apply flashlight color and intensity
		vec3 flashlightEffect = flashlightColor * flashlightIntensity * attenuation;

		// Add flashlight effect to ambient, diffuse, and specular components
		ambient += flashlightEffect;
		diffuse += flashlightEffect;
		specular += flashlightEffect;
	}

	color = vec4(ambient + diffuse + specular, 1.0) * texture(uTexture, fs_in.texCoord);

	// Spotlight
	if (u_spotlight.on == 1) {
		vec4 help += calcSpotLightColor(u_spotlight, normal, o_fragment_position, frag2camera);
	}

	color = color + help;
}
