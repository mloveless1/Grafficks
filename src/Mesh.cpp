#include <mesh.h>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

/// Shape Types:
/// 1: Torus
/// 2: 
/// 3: Sphere
/// 4: Tapered Cylinder
/// 0: All other shapes



/* Shapes that use indices */
Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<uint32_t>& elements, int shapeType)
{
	init(vertices, elements, shapeType);

}

/* For lights */
Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<uint32_t>& elements, const glm::vec3 &color, int shapeType)
{
	for (auto& vertex : vertices) {
		vertex.Color = color;
	}
	init(vertices, elements, shapeType);

}
/* For shapes that don't use indices */
Mesh::Mesh(std::vector<Vertex>& vertices, GLuint size, int shapeType)
{	//Torus
	if (shapeType == 1) {
		_nVertices = size;

		// Create VAO
		glGenVertexArrays(1, &_vertexArrayObject); // we can also generate multiple VAOs or buffers at the same time
		glBindVertexArray(_vertexArrayObject);

		// Create VBOs
		glGenBuffers(1, &_vertexBufferObject);
		glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferObject); // Activates the buffer
		glBufferData(GL_ARRAY_BUFFER, size * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU

		

		//define vertex attributes
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Position)));
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Color)));
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Normal)));
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Uv)));

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);



		_shapeType = shapeType;
		_elementCount = 0;
	}
	// Tapered Cylinder
	if (shapeType == 4) {
		_nVertices = size;

		// Create VAO
		glGenVertexArrays(1, &_vertexArrayObject); // we can also generate multiple VAOs or buffers at the same time
		glBindVertexArray(_vertexArrayObject);

		// Create VBOs
		glGenBuffers(1, &_vertexBufferObject);
		glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferObject); // Activates the buffer
		glBufferData(GL_ARRAY_BUFFER, size * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU



		//define vertex attributes
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Position)));
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Color)));
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Normal)));
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Uv)));

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);



		_shapeType = shapeType;
		_elementCount = 0;
	}
}



/* For shapes that use indices */
void Mesh::init(std::vector<Vertex>& vertices, std::vector<uint32_t>& elements, int shapeType) {
	if (shapeType == 0) {
		for (auto i = 0; i < elements.size(); i += 3) {
			auto p1Index = elements[i];
			auto p2Index = elements[i + 1];
			auto p3Index = elements[i + 2];
			Shapes::UpdateNormals(vertices[p1Index], vertices[p2Index], vertices[p3Index]);
		}
		// Gen buffers 
		glGenVertexArrays(1, &_vertexArrayObject);
		glGenBuffers(1, &_vertexBufferObject);
		glGenBuffers(1, &_elementBufferObject);

		// Bind them
		glBindVertexArray(_vertexArrayObject);
		glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferObject);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementBufferObject);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(elements.size() * sizeof(uint32_t)), elements.data(), GL_STATIC_DRAW);
		//define vertex attributes
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Position)));
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Color)));
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Normal)));
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Uv)));

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);



		_shapeType = shapeType;
		_elementCount = elements.size();
	}
	if (shapeType == 3) {
		// Gen buffers 
		glGenVertexArrays(1, &_vertexArrayObject);
		glGenBuffers(1, &_vertexBufferObject);
		glGenBuffers(1, &_elementBufferObject);

		// Bind them
		glBindVertexArray(_vertexArrayObject);
		glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferObject);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementBufferObject);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(elements.size() * sizeof(uint32_t)), elements.data(), GL_STATIC_DRAW);
		//define vertex attributes
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Position)));
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Color)));
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Normal)));
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Uv)));

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);



		_shapeType = shapeType;
		_elementCount = elements.size();
		_nVertices = _elementCount;
	}
}

void Mesh::Draw(){

	switch (_shapeType) {
	case 0: // reg shapes
		// Bind vertex array
		glBindVertexArray(_vertexArrayObject);
		//draw calls
		glDrawElements(GL_TRIANGLES, _elementCount, GL_UNSIGNED_INT, nullptr);
		break;
	case 1:  // torus
		glBindVertexArray(_vertexArrayObject);
		glDrawArrays(GL_TRIANGLES, 0, _nVertices);
		break;
	case 3: //sphere
		glBindVertexArray(_vertexArrayObject);
		glDrawElements(GL_TRIANGLES, _elementCount, GL_UNSIGNED_INT, (void*)0);
		break;
	case 4: // tapered cylinder
		glDrawArrays(GL_TRIANGLE_FAN, 0, 36);		//bottom
		glDrawArrays(GL_TRIANGLE_FAN, 36, 72);		//top
		glDrawArrays(GL_TRIANGLE_STRIP, 72, 146);	//sides
		break;
	}
} 