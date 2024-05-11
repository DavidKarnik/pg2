#version 460 core

// Inspired by "lighting_dir_point_spot.vert" by Steve Jones, Game Institute

// Vertex attributes
layout (location = 0) in vec4 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

// Matrices
uniform mat4 uMx_projection = mat4(1.0); // Camera space -> Screen
uniform mat4 uMx_model      = mat4(1.0); // Object local coor space -> World space
uniform mat4 uMx_view       = mat4(1.0); // World space -> Camera space

// VS -> FS
out vec3 o_fragment_position;
out vec3 o_normal;
out vec2 o_texture_coordinate;

void main()
{
    o_fragment_position = vec3(uMx_model * aPosition);

    // https://computergraphics.stackexchange.com/questions/1502/why-is-the-transposed-inverse-of-the-model-view-matrix-used-to-transform-the-nor
    o_normal = mat3(transpose(inverse(uMx_model))) * aNormal;

    o_texture_coordinate = aTexCoord;

    gl_Position = uMx_projection * uMx_view * uMx_model * aPosition;
}