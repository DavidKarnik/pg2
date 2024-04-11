#version 430 core

// Input vertex attributes
in vec3 aPos;    // Position: MUST exist
in vec3 aColor;  // Color attribute
in vec3 aNormal; // Normal vector, assuming this attribute is now available

// Outputs to fragment shader
out vec3 FragColor; // Output color attribute
out vec3 Normal;    // Normal vector passed to fragment shader
out vec3 FragPos;   // Fragment position in world space

// Uniform matrices
uniform mat4 transform;  // Transformation matrix for vertex position (previously called model)
uniform mat4 view;       // View matrix
uniform mat4 projection; // Projection matrix

void main()
{
    FragPos = vec3(transform * vec4(aPos, 1.0)); // Transform vertex into world space
    Normal = mat3(transpose(inverse(transform))) * aNormal; // Transform normals to world space using the transformation matrix

    gl_Position = projection * view * transform * vec4(aPos, 1.0); // Compute final vertex position

    FragColor = aColor; // Pass color to fragment shader
}
