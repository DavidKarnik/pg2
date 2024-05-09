#version 420 core

// = FINAL OUTPUT =
out vec4 color;

// = UNIFORM VARIABLES =
// Material properties
uniform vec3 ambient_material, diffuse_material, specular_material;
uniform float specular_shinines;
// Texture file from cpp
uniform sampler2D uTexture;

// = INPUT FROM VERTEX SHADER =
in VS_OUT
{
	vec3 N;
	vec3 L;
	vec3 V;
	vec2 texCoord;
} fs_in;

// Freetype Text
uniform vec3 font_colour;
uniform sampler2D alphabet_texture;
 
in vec2 texture_coordinates;
//in vec3 texcoords_anim;
 
out vec4 fragment_colour;

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
	
	color = vec4(ambient + diffuse + specular, 1.0) * texture(uTexture, fs_in.texCoord);

	// Text setup
	float texture_value = texture(alphabet_texture, texture_coordinates).r;
 
	// Enable this if-statement for 2D window-positioned text
	// -------------------------------------------------------------------------
	 if (texture_value == 1) // Fully opaque character pixels.
	 {
		 fragment_colour = vec4(font_colour / 255, texture_value);
			 // fragment_colour = vec4(255 / 255, 255 / 255, 255 / 255, 1.0);
	 }
	 else if (texture_value == 0) 
     // Fully transparent, i.e. background pixels (Note: you cannot use the 	"discard" method as used for 3D text further down, if colouring in the font's background)
	 {
		 fragment_colour = vec4(font_colour / 255, texture_value);
			 // fragment_colour = vec4(85.0 / 255, 160.0 / 255, 155.0 / 255, 1.0);
	 }
	 else // Anti-aliased character pixels.
	 {
		 fragment_colour = vec4(font_colour / 255, texture_value);
			 // fragment_colour = vec4(255 / 255, 255 / 255, 255 / 255, 1.0);
	 }
 
	// Enable these three lines instead for 3D animated text
	// ----------------------------------------------------------------------
	// fragment_colour = vec4((texcoords_anim * 2) + vec3(0.25, 0.25, 0.25), texture_value);
 
	// if (texture_value == 0)
		//  discard;
}