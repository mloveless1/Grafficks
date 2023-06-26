#pragma once
#include <glm/glm.hpp>
#include <vector>


constexpr uint8_t MAX_POINT_LIGHTS = 4;

struct Vertex {
	glm::vec3 Position {0.f, 0.f, 0.f};
	glm::vec3 Color {1.f, 1.f, 1.f};
	glm::vec3 Normal {0.f, 0.f, 0.f}; 
	glm::vec2 Uv {1.f, 1.f}; 
};
struct DirectionalLight {
	glm::vec3 Direction;

	glm::vec3 AmbientColor{};
	glm::vec3 DiffuseColor{};
	glm::vec3 SpecularColor{};
};
struct PointLightStruct {
	glm::vec3 Position;
	glm::vec3 AmbientColor{};
	glm::vec3 DiffuseColor{};
	glm::vec3 SpecularColor{};

	float Constant{ 1.f };
	float Linear{ 0.f };
	float Quadratic{ 0.f };
};
struct SceneParameters {
	glm::mat4 ProjectionMatrix {1.f};
	glm::mat4 ViewMatrix {1.f};
	glm::vec3 CameraPosition {};
	std::vector<PointLightStruct> Lights {};

	DirectionalLight DirLight {};

};
/*	For easy copy and paste 
	{
	.Position = { 0.0f, 0.0f, 0.0f }
	.Color = {0.0f, 0.0f, 0.0f}
	}, */
struct Shapes {
	// Automate normal calculation so we don't have to calc by hand anymore
	static inline void UpdateNormals(Vertex& p1, Vertex& p2, Vertex& p3) {
		// Normals calc
		glm::vec3 U = p2.Position - p1.Position;
		glm::vec3 V = p3.Position - p1.Position;
		auto normal = glm::cross(U, V);

		p1.Normal = normal;
		p2.Normal = normal;
		p3.Normal = normal;
	}
	// Used provided meshes as clue to build pyramid (and cube for module 3-5 complex object)
	// changed it to something that was easier for me to understand and build
	
	static inline std::vector<Vertex> pyramidVertices{
		// Triangle 1 Front
		{
			.Position = {0.0f, 0.5f, 0.0f}, // 0 Top
			//.Color = {0.5f, 0.0f, 0.0f},
			.Normal = {0.0f, 0.4472f, 0.8944f},
			.Uv = {0.5f, 1.f}
		},
		{
			.Position = {-0.5f, -0.5f, 0.5f}, // 1 Front left
			//.Color = { 0.0f, 0.5f, 0.0f },
			.Normal = {0.0f, 0.4472f, 0.8944f},
			.Uv = { 0.f, 0.f }
		},
		{
			.Position = {0.5f, -0.5f, 0.5f}, // 2 Front right
			//.Color = {0.0f, 0.0f, 0.5f},
			.Normal = {0.0f, 0.4472f, 0.8944f},
			.Uv = {1.f, 0.f}

		},
	// Triangle 2 left
		{
			.Position = {0.0f, 0.5f, 0.0f}, // 3 
			//.Color = {0.5f, 0.0f, 0.0f},
			.Normal = {-0.8944f, 0.4472f, 0.0f},
			.Uv = {0.5f, 1.f}

		},
		{
			.Position = {0.5f, -0.5f, 0.5f}, // 4
			//.Color = {0.0f, 0.5f, 0.0f},
			.Normal = {-0.8944f, 0.4472f, 0.0f},
			.Uv = {0.f, 0.f}

		},
		{
			.Position = {0.5f, -0.5f, -0.5f}, // 5
			//.Color = {0.0f, 0.0f, 0.5f},
			.Normal = {-0.8944f, 0.4472f, 0.0f},
			.Uv = {1.f, 0.f}


		},
	// Triangle 3 right
		{
			.Position = {0.0f, 0.5f, 0.0f}, // 6
			//.Color = {0.5f, 0.0f, 0.0f},
			.Normal = {0.0f, 0.4472f, -0.8944f},
			
			.Uv = {0.5f, 1.f}

		},
		{
			.Position = {0.5f, -0.5f, -0.5f}, // 7
			//.Color = {0.0f, 0.5f, 0.0f},
			.Normal = {0.0f, 0.4472f, -0.8944f},
			
			.Uv = {0.f, 0.f}

		},
		{
			.Position = {-0.5f, -0.5f, -0.5f}, // 8
			//.Color = {0.0f, 0.0f, 0.5f},
			.Normal = {0.0f, 0.4472f, -0.8944f},
			
			.Uv = {1.f, 0.f}

		},
	// Triangle 4 back
		{
			.Position = {0.0f, 0.5f, 0.0f}, // 9
			//.Color = {0.5f, 0.0f, 0.0f},
			.Normal = {0.8944f, 0.4472f, 0.0f},
			.Uv = {0.5f, 1.f}

		},
		{
			.Position = {-0.5f, -0.5f, -0.5f}, // 10
			//.Color = {0.0f, 0.5f, 0.0f},
			.Normal = {0.8944f, 0.4472f, 0.0f},
			.Uv = {0.f, 0.f}

		},
		{
			.Position = {-0.5f, -0.5f, 0.5f}, // 11
			//.Color = {0.5f, 0.0f, 0.5f},
			.Normal = {0.8944f, 0.4472f, 0.0f},
			.Uv = {1.f, 0.f}

		},
	// Base; switched vertices to match cube face (easier tex coords)
		{
			.Position = {0.5f, -0.5f, 0.5f}, // 12 front left
			//.Color = {1.0f, 0.0f, 0.0f}
			.Normal = {0.0f, -1.0f, 0.0f},
			.Uv = {0.f, 1.f}

		},
		{
			.Position = {0.5f, -0.5f, -0.5f}, // 13 front right
			//.Color = {0.0f, 1.0f, 0.0f}
			.Normal = {0.0f, -1.0f, 0.0f},
			.Uv = {0.f, 0.f}

		},
		{
			.Position = {-0.5f, -0.5f, -0.5f}, // 14 back right
			//.Color = {1.0f, 0.0f, 1.0f}
			.Normal = {0.0f, -1.0f, 0.0f},
			.Uv = {1.f, 0.f}

		},
		{
			.Position = {-0.5f, -0.5f, 0.5f}, // 15 
			//.Color = {0.0f, 0.0f, 0.0f}
			.Normal = {0.0f, -1.0f, 0.0f},
			.Uv = {1.f, 1.f}

		},
		
	};
	static inline std::vector<uint32_t> pyramidElements{
		0, 1, 2,
		3, 4, 5, 
		6, 7, 8,
		9, 10, 11,
		12, 13, 15,
		13, 14, 15
	};

	static inline std::vector<Vertex> cubeVertices{
		// Front
		{
			.Position = { -0.5f, 0.5f, 0.5f },
			//.Color = {0.5f, 1.0f, 0.5f},
			.Uv = {0.f, 1.f}
		},
		{
			.Position = { -0.5f, -0.5f, 0.5f },
			//.Color = {0.5f, 1.0f, 0.5f},
			.Uv = {0.f, 0.f}

		},
		{
			.Position = { 0.5f, -0.5f,0.5f },
			//.Color = {0.5f, 1.0f, 0.5f},
			.Uv = {1.f, 0.f}

		},
		{
			.Position = { 0.5f, 0.5f, 0.5f },
			//.Color = {0.5f, 1.0f, 0.5f}, 
			.Uv = {1.f, 1.f}

		},
		// Right
		{
			.Position = {0.5f, 0.5f,0.5f },
			//.Color = {0.5f, 0.0f, 0.5f}
		},
		{
			.Position = { 0.5f, -0.5, 0.5f },
			//Color = {0.5f, 0.0f, 0.5f}
		},
		{
			.Position = { 0.5f, -0.5f, -0.5f },
			//.Color = {0.5f, 0.0f, 0.5f}
		},
		{
			.Position = { 0.5f, 0.5f, -0.5f },
			//.Color = {0.5f, 0.0f, 0.5f}
		},
		// Back
		{
			.Position = { 0.5f, 0.5f, -0.5f },
			//.Color = {0.2f, 0.2f, 0.3f}
		},
		{
			.Position = { 0.5f, -0.5f, -0.5f },
			//.Color = {0.2f, 0.2f, 0.3f}
		},
		{
			.Position = { -0.5f, -0.5f, -0.5f },
			//.Color = {0.2f, 0.2f, 0.3f}
		},
		{
			.Position = { -0.5f, 0.5f, -0.5f },
			//.Color = {0.2f, 0.2f, 0.3f}
		},
		// Left
		{
			.Position = { -0.5f, 0.5f, -0.5f },
			//.Color = {0.5f, 0.4f, 0.5f}
		},
		{
			.Position = { -0.5f, -0.5f, -0.5f },
			//.Color = {0.5f, 0.4f, 0.5f}
		},
		{
			.Position = { -0.5f, -0.5f, 0.5f },
			//.Color = {0.5f, 0.4f, 0.5f}
		},
		{
			.Position = { -0.5f, 0.5f, 0.5f },
			//.Color = {0.5f, 0.4f, 0.5f}
		},
		// Top
		{
			.Position = { -0.5f, 0.5f, -0.5f },
			//.Color = {0.5f, 0.0f, 0.5f}
		},
		{
			.Position = { -0.5f, 0.5f,0.5f },
			//.Color = {0.5f, 0.0f, 0.5f}
		},
		{
			.Position = { 0.5f, 0.5f, 0.5f },
			//.Color = {0.5f, 0.0f, 0.5f}
		},
		{
			.Position = { 0.5f,0.5f, -0.5f },
			//.Color = {0.5f, 0.0f, 0.5f}
		},
		// Bottom
		{
			.Position = { 0.5f, -0.5f, 0.5f },
			//.Color = {0.5f, 0.0f, 0.5f}
		},
		{
			.Position = {0.5f, -0.5f, -0.5f },
			//.Color = {0.5f, 0.0f, 0.5f}
		},
		{
			.Position = { -0.5f, -0.5f, -0.5f },
			//.Color = {0.5f, 0.0f, 0.5f}
		},
		{
			.Position = { -0.5f, -0.5f, 0.5f },
			//.Color = {0.5f, 0.0f, 0.5f}
		},
	};
	static inline std::vector<uint32_t> cubeElements{
		0, 1, 3, 1, 2, 3, // front
		4, 5, 7, 5, 6, 7, // right
		8, 9, 11, 9, 10, 11, // back
		12, 13, 15, 13, 14, 15, // left
		16, 17, 19, 17, 18, 19, // top
		20, 21, 23, 21, 22, 23, // bottom
	};

	static inline std::vector<Vertex> planeVertices{
		{
			.Position = {-4.0f, 0.0f, 4.0f}, // front left
			// .Color = {1.f, 0.f, 1.f},
			.Uv = {0.f, 1.f}
		},
		{
			.Position = {4.0f, 0.0f, 4.0f}, // front right
			//.Color = {1.f, 0.f, 1.f},
			.Uv = {0.f, 0.f}

		},
		{
			.Position = {4.0f,  0.0f, -4.0f},  // back right
			// .Color = {1.f, 0.f, 1.f},
			.Uv = {1.f, 0.f}

		},
		{
			.Position = {-4.0f, 0.0f, -4.0f}, // back left
			// .Color = {1.f, 0.f, 1.f},
			.Uv = {1.f, 1.f}

		},
	};
	static inline std::vector<uint32_t> planeElements{
		0, 1, 2,
		0, 2, 3
	
	};

	static inline std::vector<Vertex> soilPlaneVertices{
			{
				.Position = {-4.0f, 0.0f, 4.0f}, // front left
				// .Color = {1.f, 0.f, 1.f},
				.Uv = {0.f, 4.f}
			},
			{
				.Position = {4.0f, 0.0f, 4.0f}, // front right
				//.Color = {1.f, 0.f, 1.f},
				.Uv = {0.f, 0.f}

			},
			{
				.Position = {4.0f,  0.0f, -4.0f},  // back right
				// .Color = {1.f, 0.f, 1.f},
				.Uv = {4.f, 0.f}

			},
			{
				.Position = {-4.0f, 0.0f, -4.0f}, // back left
				// .Color = {1.f, 0.f, 1.f},
				.Uv = {4.f, 4.f}

			},
	};
	static inline std::vector<uint32_t> soilPlaneElements{
		0, 1, 2,
		0, 2, 3

	};

	static inline std::vector<Vertex> taperedCylinderVertices{
		// Bottom
		{
			.Position = {1.0f, 0.0f, 0.0f},
		},
		{
			.Position = {0.98f, 0.0f, -0.17f},
		},
		{
			.Position = {0.94f, 0.0f, -0.34f},
		},
		{
			.Position = {0.87f, 0.0f, -0.5f},
		},
		{
			.Position = {0.77f, 0.0f, -0.64f},
		},
		{
			.Position = {0.64f, 0.0f, -0.77f},
		},
		{
			.Position = {0.5f, 0.0f, -0.87f},
		},
		{
			.Position = {0.34f, 0.0f, -0.94f},
		},
		{
			.Position = {0.17f, 0.0f, -0.98f},
		},
		{
			.Position = {0.0f, 0.0f, -1.0f},
		},
		{
			.Position = {-0.17f, 0.0f, -0.98f},
		},
		{
			.Position = {-0.34f, 0.0f, -0.94f},
		},
		{
			.Position = {-0.5f, 0.0f, -0.87f},
		},
		{
			.Position = {-0.64f, 0.0f, -0.77f},
		},
		{
			.Position = {-0.77f, 0.0f, -0.64f},
		},
		{
			.Position = {-0.87f, 0.0f, -0.5f},
		},
		{
			.Position = {-0.94f, 0.0f, -0.34f},
		},
		{
			.Position = {-0.98f, 0.0f, -0.17f},
		},
		{
			.Position = {-1.0f, 0.0f, 0.0f},
		},
		{
			.Position = {-0.98f, 0.0f, 0.17f},
		},
		{
			.Position = {-0.94f, 0.0f, 0.34f},
		},
		{
			.Position = {-0.87f, 0.0f, 0.5f},
		},
		{
			.Position = {-0.77f, 0.0f, 0.64f},
		},
		{
			.Position = {-0.64f, 0.0f, 0.77f},
		},
		{
			.Position = {-0.5f, 0.0f, 0.87f},
		},
		{
			.Position = {-0.34f, 0.0f, 0.94f},
		},
		{
			.Position = {-0.17f, 0.0f, 0.98f},
		},
		{
			.Position = {0.0f, 0.0f, 1.0f},
		},
		{
			.Position = {0.17f, 0.0f, 0.98f},
		},
		{
			.Position = {0.34f, 0.0f, 0.94f},
		},
		{
			.Position = {0.5f, 0.0f, 0.87f},
		},
		{
			.Position = {0.64f, 0.0f, 0.77f},
		},
		{
			.Position = {0.77f, 0.0f, 0.64f},
		},
		{
			.Position = {0.87f, 0.0f, 0.5f},
		},
		{
			.Position = {0.94f, 0.0f, 0.34f},
		},
		{
			.Position = {0.98f, 0.0f, 0.17f},
		},
		// Top
		{
			.Position = {0.5f, 1.0f, 0.0f},
		},
		{
			.Position = {0.49f, 1.0f, -0.085f},
		},
		{
			.Position = {0.47f, 1.0f, -0.17f},
		},
		{
			.Position = {0.435f, 1.0f, -0.25f},
		},
		{
			.Position = {0.385f, 1.0f, -0.32f},
		},
		{
			.Position = {0.32f, 1.0f, -0.385f},
		},
		{
			.Position = {0.25f, 1.0f, -0.435f},
		},
		{
			.Position = {0.17f, 1.0f, -0.47f},
		},
		{
			.Position = {0.085f, 1.0f, -0.49f},
		},
		{
			.Position = {0.0f, 1.0f, -0.5f},
		},
		{
			.Position = {-0.085f, 1.0f, -0.49f},
		},
		{
			.Position = {-0.17f, 1.0f, -0.47f},
		},
		{
			.Position = {-0.25f, 1.0f, -0.435f},
		},
		{
			.Position = {-0.32f, 1.0f, -0.385f},
		},
		{
			.Position = {-0.385f, 1.0f, -0.32f},
		},
		{
			.Position = {-0.435f, 1.0f, -0.25f},
		},
		{
			.Position = {-0.47f, 1.0f, -0.17f},
		},
		{
			.Position = {-0.49f, 1.0f, -0.085f},
		},
		{
			.Position = {-0.5f, 1.0f, 0.0f},
		},
		{
			.Position = {-0.49f, 1.0f, 0.085f},
		},
		{
			.Position = {-0.47f, 1.0f, 0.17f},
		},
		{
			.Position = {-0.435f, 1.0f, 0.25f},
		},
		{
			.Position = {-0.385f, 1.0f, 0.32f},
		},
		{
			.Position = {-0.32f, 1.0f, 0.385f},
		},
		{
			.Position = {-0.25f, 1.0f, 0.435f},
		},
		{
			.Position = {-0.17f, 1.0f, 0.47f},
		},
		{
			.Position = {-0.085f, 1.0f, 0.49f},
		},
		{
			.Position = {0.0f, 1.0f, 0.5f},
		},
		{
			.Position = {0.085f, 1.0f, 0.49f},
		},
		{
			.Position = {0.17f, 1.0f, 0.47f},
		},
		{
			.Position = {0.25f, 1.0f, 0.435f},
		},
		{
			.Position = {0.32f, 1.0f, 0.385f},
		},
		{
			.Position = {0.385f, 1.0f, 0.32f},
		},
		{
			.Position = {0.435f, 1.0f, 0.25f},
		},
		{
			.Position = {0.47f, 1.0f, 0.17f},
		},
		{
			.Position = {0.49f, 1.0f, 0.085f},
		},
		//body
		{
			.Position = { 0.5f, 1.0f, 0.0f },
		},
		{
			.Position = {0.49f, 1.0f, -0.085f},

		},
		{
			.Position = { 0.47f, 1.0f, -0.17f },

		},
		{
			.Position = {0.435f, 1.0f, -0.25f},

		},
		{
			.Position = {0.385f, 1.0f, -0.32f},

		},
		{
			.Position = {0.32f, 1.0f, -0.385f},

		},
		{
			.Position = {0.25f, 1.0f, -0.435f},

		},
		{
			.Position = {0.17f, 1.0f, -0.47f},

		},
		{
			.Position = {0.085f, 1.0f, -0.49f},

		},
		{
			.Position = {0.0f, 1.0f, -0.5f},

		},
		{
			.Position = {-0.085f, 1.0f, -0.49f},

		},
		{
			.Position = {-0.17f, 1.0f, -0.47f},

		},
		{
			.Position = {-0.25f, 1.0f, -0.435f},

		},
		{
			.Position = {-0.32f, 1.0f, -0.385f},

		},
		{
			.Position = {-0.385f, 1.0f, -0.32f},

		},
		{
			.Position = {-0.435f, 1.0f, -0.25f},

		},
		{
			.Position = {-0.47f, 1.0f, -0.17f},

		},
		{
			.Position = {-0.49f, 1.0f, -0.085f},

		},
		{
			.Position = {-0.5f, 1.0f, 0.0f},

		},
		{
			.Position = {-0.49f, 1.0f, 0.085f},

		},
		{
			.Position = {-0.47f, 1.0f, 0.17f},

		},
		{
			.Position = {-0.435f, 1.0f, 0.25f},

		},
		{
			.Position = {-0.385f, 1.0f, 0.32f},

		},
		{
			.Position = {-0.385f, 1.0f, 0.32f},

		},
		{
			.Position = {-0.32f, 1.0f, 0.385f},

		},
		{
			.Position = {-0.25f, 1.0f, 0.435f},

		},
		{
			.Position = {-0.17f, 1.0f, 0.47f},

		},
		{
			.Position = {-0.085f, 1.0f, 0.49f},

		},
		{
			.Position = {0.0f, 1.0f, 0.5f},

		},
		{
			.Position = {0.085f, 1.0f, 0.49f},

		},
		{
			.Position = {0.17f, 1.0f, 0.47f},
		},
		{
			.Position = {0.25f, 1.0f, 0.435f},

		},
		{
			.Position = {0.32f, 1.0f, 0.385f},

		},
		{
			.Position = {0.385f, 1.0f, 0.32f},

		},
		{
			.Position = {0.435f, 1.0f, 0.25f},
		},
		{
			.Position = {0.47f, 1.0f, 0.17f},

		},
		{
			.Position = {0.49f, 1.0f, 0.085f},
		}
	};
	static inline std::vector<uint32_t> taperedCylinderElements{

	};

	static inline std::vector<Vertex> cylinderVertices{

	};
	static inline std::vector<uint32_t> cylinderElements{

	};

};