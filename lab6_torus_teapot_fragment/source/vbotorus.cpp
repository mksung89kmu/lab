#include "vbotorus.h"


#include <GL/gl.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>



#include <cstdio>
#include <cmath>

VBOTorus::VBOTorus(float outerRadius, float innerRadius, int nsides, int nrings) :
        rings(nrings), sides(nsides)
{
    faces = sides * rings;
    int nVerts  = sides * (rings+1);   // One extra ring to duplicate first ring

    // Verts
	GLfloat * v = new GLfloat[3 * nVerts];
    // Normals
	GLfloat * n = new GLfloat[3 * nVerts];
//     // Tex coords
    float * tex = new float[2 * nVerts];
    // Elements
    unsigned int * el = new unsigned int[6 * faces];

    // Generate the vertex data
    generateVerts(v, n, tex,  el, outerRadius, innerRadius);


	//create vao, vbo, ibo here

     //DSA 
    glCreateVertexArrays(1, &vaoHandle);
    GLuint handle[4];
    glCreateBuffers(4, handle); // VBO 두 개 생성

    // 정점 데이터 VBO 설정
    glNamedBufferData(handle[0], nVerts * 3 * sizeof(float), v, GL_STATIC_DRAW);
    glVertexArrayVertexBuffer(vaoHandle, 0, handle[0], 0, sizeof(glm::vec3));

    // 색상 데이터 VBO 설정
    glNamedBufferData(handle[1], nVerts * 3 * sizeof(float), n, GL_STATIC_DRAW);
    glVertexArrayVertexBuffer(vaoHandle, 1, handle[1], 0, sizeof(glm::vec3));

	//texture coord 데이터 VBO 설정
    glNamedBufferData(handle[2], nVerts * 2 * sizeof(float), tex, GL_STATIC_DRAW);
    glVertexArrayVertexBuffer(vaoHandle, 2, handle[2], 0, sizeof(glm::vec3));

    // 인덱스 데이터 IBO 설정
    glNamedBufferData(handle[3], sizeof(unsigned int) * faces * 6, el, GL_STATIC_DRAW);
    glVertexArrayElementBuffer(vaoHandle, handle[3]);


    // 위치 속성 설정
    glVertexArrayAttribFormat(vaoHandle, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(vaoHandle, 0, 0);
    glEnableVertexArrayAttrib(vaoHandle, 0);

    // 색상 속성 설정
    glVertexArrayAttribFormat(vaoHandle, 1, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(vaoHandle, 1, 1);
    glEnableVertexArrayAttrib(vaoHandle, 1);

    // 텍스처 좌표 속성 설정
    glVertexArrayAttribFormat(vaoHandle, 2, 2, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(vaoHandle, 2, 2);
    glEnableVertexArrayAttrib(vaoHandle, 2);

    delete [] v;
    delete [] n;
    delete [] el;


    glBindVertexArray(0);
}


VBOTorus::~VBOTorus()
{
    	
}

void VBOTorus::draw() const 
{

    glBindVertexArray(vaoHandle);
    int size;
    glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
    glDrawElements(GL_TRIANGLES, size / sizeof(unsigned int), GL_UNSIGNED_INT, 0);


    glBindVertexArray(0);

}

void VBOTorus::generateVerts(GLfloat * verts, GLfloat * norms, GLfloat * tex, unsigned int * el, 
                             float outerRadius, float innerRadius)
{
	float TWOPI = 2 * glm::pi<float>();

    float ringFactor  = (float)(TWOPI / rings);
    float sideFactor = (float)(TWOPI / sides);
    int idx = 0, tidx = 0;
    for( int ring = 0; ring <= rings; ring++ ) {
        float u = ring * ringFactor;
        float cu = cos(u);
        float su = sin(u);
        for( int side = 0; side < sides; side++ ) {
            float v = side * sideFactor;
            float cv = cos(v);
            float sv = sin(v);
            float r = (outerRadius + innerRadius * cv);
            verts[idx] = r * cu;
            verts[idx + 1] = r * su;
            verts[idx + 2] = innerRadius * sv;
		
            norms[idx] = cv * cu * r;
            norms[idx + 1] = cv * su * r;
            norms[idx + 2] = sv * r;

            tex[tidx] = (float)(u / TWOPI);
            tex[tidx+1] = (float)(v / TWOPI);
            tidx += 2;

            // Normalize
            float len = sqrt( norms[idx] * norms[idx] +
                              norms[idx+1] * norms[idx+1] +
                              norms[idx+2] * norms[idx+2] );
            norms[idx] /= len;
            norms[idx+1] /= len;
            norms[idx+2] /= len;
            idx += 3;
        }
    }

    idx = 0;
    for( int ring = 0; ring < rings; ring++ ) {
        int ringStart = ring * sides;
        int nextRingStart = (ring + 1) * sides;
        for( int side = 0; side < sides; side++ ) {
            int nextSide = (side+1) % sides;
            // The quad
            el[idx] = (ringStart + side);
            el[idx+1] = (nextRingStart + side);
            el[idx+2] = (nextRingStart + nextSide);
            el[idx+3] = ringStart + side;
            el[idx+4] = nextRingStart + nextSide;
            el[idx+5] = (ringStart + nextSide);
            idx += 6;
        }
    }


}


