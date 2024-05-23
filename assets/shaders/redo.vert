#version 460 core

// Vertex attributes
layout (location = 0) in vec4 Position;
layout (location = 1) in vec3 Normal;
layout (location = 2) in vec2 TexCoord;

// Matrices
uniform mat4 uMx_projection = mat4(1.0); // Camera space to Screen
uniform mat4 uMx_model      = mat4(1.0); // Object local coor space to World space
uniform mat4 uMx_view       = mat4(1.0); // World space to Camera space

// Vertex Shader -> Fragment Shader
out vec3 fragment_position;
out vec3 world_space_normal ;
out vec2 texture_coordinates;

void main()
{
    // Compute the position of the fragment in world space
    fragment_position = vec3(uMx_model * Position);

    // Compute the normal in world space
    world_space_normal = normalize(mat3(transpose(inverse(uMx_model))) * Normal);

    // Pass the texture coordinates to the fragment shader
    texture_coordinates = TexCoord;

    // Compute the final position of the vertex in screen space
    gl_Position = uMx_projection * uMx_view * uMx_model * Position;
}
