


#include "cube.h"
#include "bunny.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "HalfEdge.h"

std::vector<glm::vec3> vertices(3161);
std::vector<glm::vec3> normals(3161);
std::vector<glm::vec2> texcoords(3161);


bunny::bunny()
{

	setup();
}



void bunny::findingBoundaryEdge(HalfEdgeMesh& mesh)
{

	std::cout << "Boundary edges:" << std::endl;
	for (auto he : mesh.halfEdges) {
		if (he->twin == nullptr) {
			int v_start = he->vertex->id;
			int v_end = he->next->vertex->id;
			//std::cout << "Edge from v" << v_start << " to v" << v_end << " is boundary." << std::endl;
			normals[v_start] = glm::vec3(0.0f, 0.0f, 0.0f); // 예시로 빨간색으로 표시
			normals[v_end] = glm::vec3(0.0f, 0.0f, 0.0f);   // 예시로 빨간색으로 표시
		}
	}
}


void bunny::laplacianSmoothing(HalfEdgeMesh& mesh)
{
	//Laplacian Smoothing

	// 새로운 위치를 저장할 버퍼
	std::vector<glm::vec3> newPositions(3161);

	// 각 vertex에 대해 인접 vertex 평균 계산
	for (auto v : mesh._vertices) {

		//if (v->edge == nullptr) continue; // ✓ Handle isolated vertices
		if (v->edge == nullptr) {
			newPositions[v->id] = vertices[v->id];
			continue;
		}

		std::vector<Vertex*> neighbors;
		HalfEdge* start = v->edge;
		HalfEdge* he = start;

		bool isBoundary = false;  // ✓ Detect boundary vertices

		do {
			
			neighbors.push_back(he->next->vertex);

			if (!he->twin) {
				isBoundary = true;  // ✓ Mark as boundary

				// Collect remaining neighbors on boundary
				HalfEdge* current = start;

				// what this wile loop does is to find all the neighbors of the boundary vertex by traversing along the boundary edges. It starts from the initial half-edge (start) ]
				// and keeps moving to the next half-edge's twin until it comes back to the starting half-edge. 
				// 
				// If it encounters a half-edge without a twin, it means it's at the end of the boundary, 
				// and it collects the neighbor vertex of that half-edge. 
				// This way, it ensures that all neighbors of a boundary vertex are included in the smoothing process, even if they are not connected through a twin half-edge.
				while (current->next->next->twin) {
					current = current->next->next->twin;
					if (current == start) 
						break;
				}
				if (current != start) {
					neighbors.push_back(current->vertex);
				}
				break;
			}
			he = he->twin->next;
		} while (he != start);


		if (neighbors.empty()) {
			newPositions[v->id] = vertices[v->id];
			continue;
		}

		// 평균 위치 계산
		glm::vec3 avgPos(0.0f);
		for (auto nb : neighbors)
		{
			avgPos += vertices[nb->id];
		}
		avgPos /= static_cast<float>(neighbors.size());

		float lambda = isBoundary ? 0.0f : 0.5f;
		newPositions[v->id] = vertices[v->id] + lambda * (avgPos - vertices[v->id]);

	}

	// 모든 vertex 위치 업데이트
	for (auto v : mesh._vertices) {
		//	std::cout << "Updating vertex " << v->id << " from " << glm::to_string(vertices[v->id]) 
		//		<< " to " << glm::to_string(newPositions[v->id]) << std::endl;
		vertices[v->id] = newPositions[v->id];

	}
}


void bunny::setup()
{
		
	//struct ModelVertex const modelVertices[3161]


	std::vector<uint32_t> nvertices;
	for (int i = 0; i < 3161; i++) {
		vertices[i] = modelVertices[i].position;
		normals[i] = modelVertices[i].normal;

	}
		
	std::vector<std::vector<int>> triangles;
	for (int i = 0; i < 17088; i+=3) { //is it correct?
	
		std::vector<int> tri;
		tri.push_back(modelIndices[i]);
		tri.push_back(modelIndices[i+1]);
		tri.push_back(modelIndices[i+2]);

		triangles.push_back(tri);
	}

	

	HalfEdgeMesh mesh(triangles);


//	findingBoundaryEdge(mesh);
	laplacianSmoothing(mesh);
	laplacianSmoothing(mesh);

	/*
	//create vao
	glGenVertexArrays(1, &vaoHandle);
	glBindVertexArray(vaoHandle);

		

	//create vbo for vertices
	glGenBuffers(1, &vbo_cube_vertices);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)* 3 * 8, &cube_vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(
		0, // attribute
		3,                 // number of elements per vertex, here (x,y,z,1)
		GL_FLOAT,          // the type of each element
		GL_FALSE,          // take our values as-is
		0,                 // no extra data between each position
		0                  // offset of first element
		);
	glEnableVertexAttribArray(0);


	//create vbo for colors
	glGenBuffers(1, &vbo_cube_colors);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_colors);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)* 3 * 8, &cube_colors, GL_STATIC_DRAW);
	glVertexAttribPointer(
		1, // attribute
		3,                 // number of elements per vertex, here (R,G,B)
		GL_FLOAT,          // the type of each element
		GL_FALSE,          // take our values as-is
		0,                 // no extra data between each position
		0                  // offset of first element
		);
	glEnableVertexAttribArray(1);


	glGenBuffers(1, &ibo_cube_elements);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cube_elements);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_elements), cube_elements, GL_STATIC_DRAW);

	

	glBindVertexArray(0);

	*/


	

	glCreateVertexArrays(1, &vaoHandle);
	glCreateBuffers(1, &vbo_cow_vertices); // VBO 두 개 생성
	glCreateBuffers(1, &vbo_cow_normals); // VBO 두 개 생성
	glCreateBuffers(1, &ibo_cow_elements); // VBO 두 개 생성


	// 정점 데이터 VBO 설정
	glNamedBufferData(vbo_cow_vertices, sizeof(glm::vec3)* 3161, vertices.data(), GL_STATIC_DRAW);
	glVertexArrayVertexBuffer(vaoHandle, 0, vbo_cow_vertices, 0, sizeof(glm::vec3));


	glNamedBufferData(vbo_cow_normals, sizeof(glm::vec3) * 3161, normals.data(), GL_STATIC_DRAW);
    glVertexArrayVertexBuffer(vaoHandle, 1, vbo_cow_normals, 0, sizeof(glm::vec3));


	// 인덱스 데이터 IBO 설정
	glNamedBufferData(ibo_cow_elements, sizeof(modelIndices), modelIndices, GL_STATIC_DRAW);
	glVertexArrayElementBuffer(vaoHandle, ibo_cow_elements);


	// 위치 속성 설정
	glVertexArrayAttribFormat(vaoHandle, 0, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(vaoHandle, 0, 0);
	glEnableVertexArrayAttrib(vaoHandle, 0);
	

	glVertexArrayAttribFormat(vaoHandle, 1, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(vaoHandle, 1, 1);
	glEnableVertexArrayAttrib(vaoHandle, 1);



}

void bunny::draw()
{

	glBindVertexArray(vaoHandle);
//	glDrawArrays(GL_TRIANGLES, 0, 3);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cube_elements);
	int size;
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	size_t tt = size / sizeof(unsigned int);
	glDrawElements(GL_TRIANGLES, size / sizeof(unsigned int), GL_UNSIGNED_INT, 0);

}

/////////////

