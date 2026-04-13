
#define GLM_ENABLE_EXPERIMENTAL 

#include <iostream>
#include "cube.h"
#include "cow.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include "HalfEdge.h"


cow::cow()
{

	setup();
}

void cow::calculateNormal()
{
	const uint32_t ntris = 3156;

	normals.resize(1732);

	for (uint32_t i = 0; i < ntris; ++i) {

		const glm::vec3& v0 = vertices[nvertices[i * 3]];
		const glm::vec3& v1 = vertices[nvertices[i * 3 + 1]];
		const glm::vec3& v2 = vertices[nvertices[i * 3 + 2]];


		glm::vec3 vv1 = v1 - v0;
		glm::vec3 vv2 = v2 - v0;

		glm::vec3 n = glm::cross(vv1, vv2);
		n = glm::normalize(n);

		normals[nvertices[i * 3]] = n;
		normals[nvertices[i * 3+1]] = n;
		normals[nvertices[i * 3+2]] = n;
	}

}

void cow::findingBoundaryEdge(HalfEdgeMesh& mesh)
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

void cow::laplacianSmoothing(HalfEdgeMesh& mesh)
{
	// 1. 새로운 위치 저장용 버퍼 (동적 크기 할당)
	std::vector<glm::vec3> newPositions(mesh._vertices.size());

	for (auto v : mesh._vertices) {

		if (v->edge == nullptr) {
			newPositions[v->id] = vertices[v->id];
			continue;
		}

		std::vector<int> neighborIds;
		HalfEdge* start = v->edge;
		HalfEdge* curr = start;
		bool isBoundary = false;

		// 정점 v를 중심으로 시계방향 혹은 반시계방향 순회
		do {
			// HalfEdge가 '시작점'을 가리키므로, 인접점은 curr->next->vertex입니다.
			if (curr->next != nullptr) {
				neighborIds.push_back(curr->next->vertex->id);
			}

			// 다음 outgoing edge로 이동: twin->next
			if (curr->twin != nullptr) {
				curr = curr->twin->next;
			}
			else {
				// twin이 없으면 경계면(Boundary)에 도달한 것임
				isBoundary = true;
				break;
			}
		} while (curr != start);

		// 경계면 정점일 경우: 반대 방향으로도 끝까지 순회해야 모든 인접점을 찾음
		if (isBoundary) {
			// 다시 시작점에서 반대 방향으로 순환
			// (prev가 없으므로 v로 들어오는 edge를 찾기 위해 Face를 한바퀴 돌아야 함)
			HalfEdge* currBack = start;

			// v로 들어오는 edge(currBack->??? == v)를 찾기 위해 현재 Face 순회
			// 이 로직은 복잡하므로, 보통 경계면 정점은 수축 방지를 위해 고정합니다.
			// 여기서는 안전하게 neighbors가 수집된 만큼만 사용하거나 lambda를 0으로 둡니다.
		}

		// 2. 평균 계산
		if (neighborIds.empty()) {
			newPositions[v->id] = vertices[v->id];
			continue;
		}

		glm::vec3 avgPos(0.0f);
		for (int id : neighborIds) {
			avgPos += vertices[id];
		}
		avgPos /= static_cast<float>(neighborIds.size());

		// 3. 가중치 적용 (경계면 정점은 위치를 유지시켜 매쉬 변형을 막음)
		float lambda = isBoundary ? 0.0f : 0.5f;
		newPositions[v->id] = vertices[v->id] + lambda * (avgPos - vertices[v->id]);
	}

	// 4. 데이터 업데이트
	for (size_t i = 0; i < mesh._vertices.size(); ++i) {
		vertices[i] = newPositions[i];
	}
}


// Check which vertices have no edge

void cow::setup()
{

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


	calculateNormal();


	const uint32_t ntris = 9468/3.0f; //number of triangles
	
	std::vector<std::vector<int>> triangles;
	for (uint32_t i = 0; i < ntris; i++) {
		std::vector<int> tri;

		tri.push_back(nvertices[i*3]);
		tri.push_back(nvertices[i*3 + 1]);
		tri.push_back(nvertices[i*3 + 2]);

		triangles.push_back(tri);

	}


	HalfEdgeMesh mesh(triangles);

	//two operations

//	findingBoundaryEdge(mesh);
	laplacianSmoothing(mesh);
//	laplacianSmoothing(mesh);
	

	glCreateVertexArrays(1, &vaoHandle);
	glCreateBuffers(1, &vbo_cow_vertices); // VBO 두 개 생성
	glCreateBuffers(1, &vbo_cow_normals); // VBO 두 개 생성
	glCreateBuffers(1, &ibo_cow_elements); // VBO 두 개 생성


	// 정점 데이터 VBO 설정
	glNamedBufferData(vbo_cow_vertices, sizeof(glm::vec3)*1732, vertices, GL_STATIC_DRAW);
	glVertexArrayVertexBuffer(vaoHandle, 0, vbo_cow_vertices, 0, sizeof(glm::vec3));


	glNamedBufferData(vbo_cow_normals, sizeof(glm::vec3) * 1732, normals.data(), GL_STATIC_DRAW);
    glVertexArrayVertexBuffer(vaoHandle, 1, vbo_cow_normals, 0, sizeof(glm::vec3));


	// 인덱스 데이터 IBO 설정
	glNamedBufferData(ibo_cow_elements, sizeof(nvertices), nvertices, GL_STATIC_DRAW);
	glVertexArrayElementBuffer(vaoHandle, ibo_cow_elements);


	// 위치 속성 설정
	glVertexArrayAttribFormat(vaoHandle, 0, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(vaoHandle, 0, 0);
	glEnableVertexArrayAttrib(vaoHandle, 0);
	

	glVertexArrayAttribFormat(vaoHandle, 1, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(vaoHandle, 1, 1);
	glEnableVertexArrayAttrib(vaoHandle, 1);



}

void cow::draw()
{

	
	glBindVertexArray(vaoHandle);
//	glDrawArrays(GL_TRIANGLES, 0, 3);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cube_elements);
	int size;
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	size_t tt = size / sizeof(uint32_t);
	glDrawElements(GL_TRIANGLES, size / sizeof(uint32_t), GL_UNSIGNED_INT, 0);



}

/////////////

