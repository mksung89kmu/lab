
#include "GL/gl3w.h"
#include "vboplane.h"

#include <cstdio>
#include <cmath>



VBOPlane::VBOPlane(float xsize, float zsize, int xdivs, int zdivs, float smax, float tmax)
{
    
    faces = xdivs * zdivs;
    float * v = new float[3 * (xdivs + 1) * (zdivs + 1)];
	float * n = new float[3 * (xdivs + 1) * (zdivs + 1)];
    float * tex = new float[2 * (xdivs + 1) * (zdivs + 1)];
    unsigned int * el = new unsigned int[6 * xdivs * zdivs];

    float x2 = xsize / 2.0f;
    float z2 = zsize / 2.0f;
    float iFactor = (float)zsize / zdivs;
    float jFactor = (float)xsize / xdivs;

    float texi = smax / zdivs;
    float texj = tmax / xdivs;
    
    float x, z;
    int vidx = 0, tidx = 0;
    
    for( int i = 0; i <= zdivs; i++ ) {
        z = iFactor * i - z2;
        for( int j = 0; j <= xdivs; j++ ) {
            x = jFactor * j - x2;
            v[vidx] = x;
            v[vidx+1] = 0.0f;
            v[vidx+2] = z;

			n[vidx] = 0.0f;
			n[vidx+1] = 1.0f;
			n[vidx+2] = 0.0f;
            
            vidx += 3;
            tex[tidx] = j * texj;
            tex[tidx+1] = i * texi;
            tidx += 2;
        }
    }

    unsigned int rowStart, nextRowStart;
    int idx = 0;
    for( int i = 0; i < zdivs; i++ ) {
        rowStart = i * (xdivs+1);
        nextRowStart = (i+1) * (xdivs+1);
        for( int j = 0; j < xdivs; j++ ) {
            el[idx] = rowStart + j;
            el[idx+1] = nextRowStart + j;
            el[idx+2] = nextRowStart + j + 1;
            el[idx+3] = rowStart + j;
            el[idx+4] = nextRowStart + j + 1;
            el[idx+5] = rowStart + j + 1;
            idx += 6;
        }
    }

    /*
    glCreateVertexArrays(1, &vaoHandle);
    GLuint handle[2];

    glCreateBuffers(2, handle); // VBO 두 개 생성

    // 정점 데이터 VBO 설정
    glNamedBufferData(handle[0], 3161 * sizeof(ModelVertex), modelVertices, GL_STATIC_DRAW);
    glVertexArrayVertexBuffer(vaoHandle, 0, handle[0], 0, sizeof(float) * 8);

    // 위치 속성 설정
    glVertexArrayAttribFormat(vaoHandle, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(vaoHandle, 0, 0);
    glEnableVertexArrayAttrib(vaoHandle, 0);
    // 노말 속성 설정
    glVertexArrayAttribFormat(vaoHandle, 1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3);
    glVertexArrayAttribBinding(vaoHandle, 1, 0);
    glEnableVertexArrayAttrib(vaoHandle, 1);
    // 텍스처 속성 설정
    glVertexArrayAttribFormat(vaoHandle, 2, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6);
    glVertexArrayAttribBinding(vaoHandle, 2, 0);
    glEnableVertexArrayAttrib(vaoHandle, 2);

    //인덱스
    glNamedBufferData(handle[1], 17088 * sizeof(unsigned int), modelIndices, GL_STATIC_DRAW);
    glVertexArrayElementBuffer(vaoHandle, handle[1]);
    */

   
    glCreateVertexArrays(1, &vaoHandle);
    GLuint handle[4];

    glCreateBuffers(4, handle); // VBO 두 개 생성

    glNamedBufferData(handle[0], 3 * (xdivs + 1) * (zdivs + 1) * sizeof(float), v, GL_STATIC_DRAW);
    glVertexArrayVertexBuffer(vaoHandle, 0, handle[0], 0, sizeof(float) * 3);

    glNamedBufferData(handle[1], 3 * (xdivs + 1) * (zdivs + 1) * sizeof(float), n, GL_STATIC_DRAW);
    glVertexArrayVertexBuffer(vaoHandle, 1, handle[1], 0, sizeof(float) * 3);

    glNamedBufferData(handle[2], 2 * (xdivs + 1) * (zdivs + 1) * sizeof(float), tex, GL_STATIC_DRAW);
    glVertexArrayVertexBuffer(vaoHandle, 2, handle[2], 0, sizeof(float) * 2);

    glNamedBufferData(handle[3], 6 * xdivs * zdivs * sizeof(unsigned int), el, GL_STATIC_DRAW);
    glVertexArrayElementBuffer(vaoHandle, handle[3]);


    glVertexArrayAttribFormat(vaoHandle,
        0, // attribute index
        3, //data per vertex
        GL_FLOAT, //type	
        GL_FALSE, //normalized
        0);  //상대적인 offset : binding index에서부터 상대적인 offset, 0은 바로 시작한다는 의미
    glVertexArrayAttribBinding(vaoHandle, 0, 0);  //attribute index, binding index을 연결  (attribute 0은 binding 0번 VBO에서 데이터를 가져온다)
    glEnableVertexArrayAttrib(vaoHandle, 0);


    glVertexArrayAttribFormat(vaoHandle,
        1, // attribute index
        3, //data per vertex
        GL_FLOAT, //type	
        GL_FALSE, //normalized
        0);  //상대적인 offset : binding index에서부터 상대적인 offset, 0은 바로 시작한다는 의미
    glVertexArrayAttribBinding(vaoHandle, 1, 1);  //attribute index, binding index을 연결  (attribute 1은 binding 1번 VBO에서 데이터를 가져온다)
    glEnableVertexArrayAttrib(vaoHandle, 1);


    glVertexArrayAttribFormat(vaoHandle,
        2, // attribute index
        2, //data per vertex
        GL_FLOAT, //type	
        GL_FALSE, //normalized
        0);  //상대적인 offset : binding index에서부터 상대적인 offset, 0은 바로 시작한다는 의미
    glVertexArrayAttribBinding(vaoHandle, 2, 2);  //attribute index, binding index을 연결  (attribute 2은 binding 2번 VBO에서 데이터를 가져온다)
    glEnableVertexArrayAttrib(vaoHandle, 2);    




    /*
	glGenVertexArrays( 1, &vaoHandle );
    glBindVertexArray(vaoHandle);
    
    unsigned int handle[4];
    glGenBuffers(4, handle);

    glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
    glBufferData(GL_ARRAY_BUFFER, 3 * (xdivs+1) * (zdivs+1) * sizeof(float), v, GL_STATIC_DRAW);
    glVertexAttribPointer( (GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)) );
    glEnableVertexAttribArray(0);  // Vertex position

    glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
    glBufferData(GL_ARRAY_BUFFER, 3 * (xdivs + 1) * (zdivs + 1) * sizeof(float), n, GL_STATIC_DRAW);
    glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte*)NULL + (0)));
    glEnableVertexAttribArray(1);  // Vertex normal

    glBindBuffer(GL_ARRAY_BUFFER, handle[2]);
    glBufferData(GL_ARRAY_BUFFER, 2 * (xdivs + 1) * (zdivs + 1) * sizeof(float), tex, GL_STATIC_DRAW);
    glVertexAttribPointer((GLuint)2, 2, GL_FLOAT, GL_FALSE, 0, ((GLubyte*)NULL + (0)));
    glEnableVertexAttribArray(2);  // Texture coords

   
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle[3]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * xdivs * zdivs * sizeof(unsigned int), el, GL_STATIC_DRAW);

    glBindVertexArray(0);
    */

    delete [] v;
	delete [] n;
    delete [] tex;
    delete [] el;
}



void VBOPlane::draw() const {
    
    glBindVertexArray(vaoHandle);
    glDrawElements(GL_TRIANGLES, 6 * faces, GL_UNSIGNED_INT, ((GLubyte *)NULL + (0)));
    
}
