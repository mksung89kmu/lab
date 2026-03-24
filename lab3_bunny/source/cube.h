#ifndef __CUBE
#define __CUBE

#include "GL/gl3w.h"
#include <glm/mat4x4.hpp>

#include <vector>


#include <iostream>
#include <vector>
#include <unordered_map>
#include <tuple>

struct HalfEdge;
struct Face;
struct Vertex;

struct Vertex {
    int id;
    HalfEdge* edge; // outgoing half-edge
};

struct Face {
    int id;
    HalfEdge* edge; // one of its half-edges
};

struct HalfEdge {
    Vertex* vertex;   // start vertex
    Face* face;       // face it belongs to
    HalfEdge* next;   // next half-edge in the face
    HalfEdge* twin;   // opposite half-edge
};

// Key for edge lookup
struct EdgeKey {
    int v1, v2;
    EdgeKey(int a, int b) {
        v1 = std::min(a, b);
        v2 = std::max(a, b);
    }
    bool operator==(const EdgeKey& other) const {
        return v1 == other.v1 && v2 == other.v2;
    }
};

struct EdgeKeyHash {
    std::size_t operator()(const EdgeKey& k) const {
        return std::hash<int>()(k.v1) ^ (std::hash<int>()(k.v2) << 1);
    }
};

class HalfEdgeMesh {
public:
    std::vector<Vertex*> vertices;
    std::vector<Face*> faces;
    std::vector<HalfEdge*> halfEdges;

    HalfEdgeMesh(const std::vector<std::vector<int>>& triangles) {
        // Create vertices
        int maxVertex = 0;
        for (auto& tri : triangles)
            for (int v : tri)
                if (v > maxVertex) maxVertex = v;
        for (int i = 0; i <= maxVertex; i++) {
            Vertex* v = new Vertex{ i, nullptr };
            vertices.push_back(v);
        }

        std::unordered_map<EdgeKey, HalfEdge*, EdgeKeyHash> edgeMap;

        // Create faces and half-edges
        int f_id = 0;
        for (auto& tri : triangles) {
            Face* f = new Face{ f_id++, nullptr };
            faces.push_back(f);

            HalfEdge* hes[3];
            for (int i = 0; i < 3; i++) {
                hes[i] = new HalfEdge();
                hes[i]->vertex = vertices[tri[i]];
                hes[i]->face = f;
                halfEdges.push_back(hes[i]);
            }
            // link next pointers
            hes[0]->next = hes[1];
            hes[1]->next = hes[2];
            hes[2]->next = hes[0];
            f->edge = hes[0];

            // register edges for twin linking
            for (int i = 0; i < 3; i++) {
                int v_start = tri[i];
                int v_end = tri[(i + 1) % 3];
                EdgeKey key(v_start, v_end);
                if (edgeMap.find(key) == edgeMap.end()) {
                    edgeMap[key] = hes[i];
                }
                else {
                    HalfEdge* twinCandidate = edgeMap[key];
                    hes[i]->twin = twinCandidate;
                    twinCandidate->twin = hes[i];
                }
                vertices[v_start]->edge = hes[i];
            }
        }
    }
};

class bunny
{
public:
	bunny();
	void setup();
	void draw();
	

	GLuint vaoHandle;
	GLuint vbo_cow_vertices, ibo_cow_elements;
	GLuint vbo_cow_normals;

};

#endif