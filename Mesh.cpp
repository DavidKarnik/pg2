#include "Mesh.h"

Mesh::Mesh(GLenum primitive_type, std::vector<Vertex>& vertices, std::vector<GLuint>& indices, GLuint texture_id) : 
	primitive_type(primitive_type),
	vertices(vertices),
	indices(indices),
	texture_id(texture_id)
{
	vertices = vertices;
	indices = indices;

	// Get the Vertex Array Object
	glGenVertexArrays(1, &VAO);
	// Get the Vertex Buffer Object
	glGenBuffers(1, &VBO);
	// Get the Element Buffer Object
	glGenBuffers(1, &EBO);
	// Set Vertex Array Buffer as the current Vertex Array Object
	glBindVertexArray(VAO);
	// Set the Vertex Buffer Object as the current buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Send the vertices data to vertex buffer
	// Allocate buffer as number of vertices * size of one vertex
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
	
	// Set Element Buffer as current buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	// Send data into EBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

	//GLint attrib_location;
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(0 + offsetof(Vertex, Position)));
	// Enable the Vertex Attribute for position
	glEnableVertexAttribArray(0);

	// Set end enable Vertex Attribute for Normal
	//attrib_location = glGetAttribLocation(shader_prog_ID, "aNormal"); //name in shader src
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(0 + offsetof(Vertex, Normal)));
	glEnableVertexAttribArray(1);
	
	// Set end enable Vertex Attribute for Texture Coordinates
	//attrib_location = glGetAttribLocation(shader_prog_ID, "aTex"); //name in shader src
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(0 + offsetof(Vertex, UVs)));
	glEnableVertexAttribArray(2);

	// Bind VBO and VAO to 0 to prevent unintended modification of VAO/VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::draw(Shader& shader)
{
	shader.activate();
	//TODO: draw mesh: bind vertex array object, draw all elements with selected primitive type, unbind vertex array object 
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

}

void Mesh::draw(Shader& shader, glm::mat4 mx_model)
{
	if (texture_id > 0) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture_id);
		//shader.setUniform("uTexture", 0);
		shader.setUniform("u_material.textura", 0); // We're only using texturing unit no. 0
	}
	shader.setUniform("uMx_model", mx_model);
	glBindVertexArray(VAO);
	glDrawElements(primitive_type, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Mesh::clear(void)
{
    vertices.clear();
    indices.clear();
    //texture_id = 0;
    primitive_type = GL_POINTS;

    // delete all allocations 
    //glDeleteBuffers... //VBO a EBO
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    //glDeleteVertexArrays... // VAO
    //glDeleteVertexArrays(1, &VAO);

    if (VAO) { glDeleteVertexArrays(1, &VAO); VAO = 0; }
    if (texture_id) { glDeleteTextures(1, &texture_id); texture_id = 0; }

    // Destruktor ne-e
};