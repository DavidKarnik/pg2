#version 460 core

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
}