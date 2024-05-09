#version 420 core

// Vertex attributes
layout (location = 0) in vec4 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

// Matrices
uniform mat4 uMx_projection = mat4(1.0); // Camera space -> Screen
uniform mat4 uMx_model      = mat4(1.0); // Object local coor space -> World space
uniform mat4 uMx_view       = mat4(1.0); // World space -> Camera space

// Light properties
uniform vec3 light_position;

// Outputs to the fragment shader
out VS_OUT
{
    vec3 N;         // normal vector
    vec3 L;         // vector from point on object (vertex or rasterised point) towards light source
    vec3 V;         // vector towards viewer
    vec2 texCoord;  // texture coordinates
} vs_out;

// Freetype Text
layout (location = 3) in vec4 vertex;
 
out vec2 texture_coordinates;
// out vec3 texcoords_anim;


void main(void)
{
    // Create Model-View matrix
    mat4 mx_modelView = uMx_view * uMx_model;
    // Calculate view-space coordinate - in P point we are computing the color
    vec4 P = mx_modelView * aPosition;
    // Calculate normal in view space
    vs_out.N = mat3(mx_modelView) * aNormal;
    // Calculate view-space light vector
    vs_out.L = light_position - P.xyz;
    // Calculate view vector (negative of the view-space position)
    vs_out.V = -P.xyz;
    // Calculate the clip-space position of each vertex
    gl_Position = uMx_projection * P;

    // Texture coordinates
    vs_out.texCoord = aTexCoord;

    // Text setup
    texture_coordinates = vec2(vertex[2], vertex[3]); // Same as using z and w... i.e. vertex.zw
 
	// Enable this line for 2D window-positioned text
	// -------------------------------------------------------------
    gl_Position = vec4(vertex.xy, 0.0, 1.0);
 
	// Enable these two lines instead for 3D animated text
	// --------------------------------------------------------------------
	// texcoords_anim = vec3(animate * vec4(vertex.xy, 0.0, 1)); // Used simply to animate the colours.
	// gl_Position = projection * view * animate * vec4(vertex.xy, 0.0, 1.0);
}