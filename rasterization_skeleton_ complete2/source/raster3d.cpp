#define _USE_MATH_DEFINES 
#define GLM_ENABLE_EXPERIMENTAL


#define COW 1

#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <fstream>
#include <chrono>

//두 개 중에 골라야 함
#include "cow.h"
//#include "teapot.h"
#include "ppm_io.h"


void convertNDCtoImage(const glm::vec4 vertexNDC, glm::vec4 vertexView, const uint32_t& imageWidth, const uint32_t& imageHeight, glm::vec3& vertexRaster)
{
    //code here
    vertexRaster.x = (vertexNDC.x + 1) / 2 * imageWidth;
    // in raster space y is down so invert direction
    vertexRaster.y = (1 - vertexNDC.y) / 2 * imageHeight;
    vertexRaster.z = -vertexView.z;

}


float min3(const float& a, const float& b, const float& c)
{
    return std::min(a, std::min(b, c));
}

float max3(const float& a, const float& b, const float& c)
{
    return std::max(a, std::max(b, c));
}

float area(const glm::vec3 a, const glm::vec3 b, const glm::vec3 c)
{
    return fabs(a.x * (b.y-c.y) + b.x * (c.y-a.y) + c.x * (a.y-b.y));
}


bool edge(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c) 
{
    // a->b로 가는 벡터에 대해 c가 오른쪽인지 왼쪽인지 확인 {    return ((c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x) >= 0); } 
    return ((c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x) >= 0);
}



glm::mat4 lookAt(glm::vec3 pos, glm::vec3 look, glm::vec3 up)
{
    glm::mat4 viewMatrix(1.0f);

    return glm::lookAt(pos, look, up);
   // return viewMatrix;

}

glm::mat4 perspective(float fovy, float aspect, float near, float far)
{

    glm::mat4 projection(1.0f);
    return glm::perspective(glm::radians(fovy), aspect, near, far);
}

const uint32_t imageWidth = 640;
const uint32_t imageHeight = 480;

 
//const unsigned int modelIndices[47112] =  //struct ModelVertex const modelVertices[6066] = {
//struct ModelVertex const modelVertices[8334]    const unsigned int modelIndices[17412] = {

#ifdef COW 
    const uint32_t ntris = 17412 /3.0;
#else
    const uint32_t ntris = 47112/3.0;
#endif


const float nearClippingPlane = 1;
const float farClippingPLane = 1000;


int main(int argc, char** argv)
{
  
   // glm::mat4 worldToCamera = glm::lookAt(glm::vec3(20, 10, 20), glm::vec3(0, 5, 0), glm::vec3(0, 1, 0));

    float t, b, l, r;

    
    std::string imagePath = std::string(DATA_DIR) + "/source/cow2.ppm";

	PPM texture(imagePath);

    PPM ppmOut;
    ppmOut.setBinary(true);



    struct rgb
    {
        uint8_t r, g, b;
    };

    std::vector <rgb>  frameBuffer;
    frameBuffer.resize(imageWidth * imageHeight);


    for (uint32_t i = 0; i < imageWidth * imageHeight; ++i) {
        frameBuffer[i].r = 0.3;
        frameBuffer[i].g = 0.3;
        frameBuffer[i].b = 0.3;
    }

    float* depthBuffer = new float[imageWidth * imageHeight];

    for (uint32_t i = 0; i < imageWidth * imageHeight; ++i) {
        depthBuffer[i] = farClippingPLane;
    }

    auto t_start = std::chrono::high_resolution_clock::now();

#ifdef COW
    int n = 6066;
#else
    int n = 8334;
#endif

	std::vector<glm::vec3> vRasters;
	vRasters.resize(n);

    std::vector<glm::vec3> ves;
    ves.resize(n);

    std::vector <glm::vec2>  textureCoords;
	textureCoords.resize(n);

	// Pre-compute all vertex positions in raster space

    for (uint32_t i = 0; i < n; ++i) {

#ifdef COW
        glm::vec3 v = modelVertices[i].position;
        glm::vec2 uv = modelVertices[i].uv;

#else
        glm::vec3 v = modelVertices[i].position;
        glm::vec2 uv = modelVertices[i].uv;

#endif
             
        

        glm::mat4 modelMatrix(1.0f);

#ifdef COW   
        glm::mat4 viewMatrix = lookAt(glm::vec3(10, 10, 10), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
#else
        glm::mat4 viewMatrix = lookAt(glm::vec3(200, 200, 200), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
#endif

        glm::mat4 modelViewMatrix = viewMatrix * modelMatrix;

        //Camera(view) coordinates
        glm::vec4 ve;

        //Code here (calculate v0e, v1e, v2e here)
        glm::mat4 projection = perspective((45.0f), imageWidth / (float)imageHeight, nearClippingPlane, farClippingPLane);

        //Clip coodinates
        glm::vec4 vc;

		//camera(view) coordinates
        ve = modelViewMatrix * glm::vec4(v, 1.0f);
        ves[i] = ve;

		//clip coordinates
        vc = projection * ve;

		//vc -> gl_Position


		//perspective division (divide v0c.x, v0c.y, v0c.z by v0c.w   (same to v1c, v2c)  : NDC
        vc.x = vc.x / vc.w;
        vc.y = vc.y / vc.w;
        vc.z = vc.z / vc.w;

		//how can I transform the texture coordinates st to the raster space?  (Hint : you can use the same way as vertex positions)
        textureCoords[i] = uv;

		//convert NDC to raster space
        glm::vec3 vRaster;
        convertNDCtoImage(vc, ve, imageWidth, imageHeight, vRaster);

        vRasters[i] = vRaster;
    }



    for (uint32_t i = 0; i < ntris; ++i) {

		//get triangle vertex indices : This is the primitive assembly step

#ifdef COW
        int index0 = modelIndices[i * 3];
        int index1 = modelIndices[i * 3 + 1];
        int index2 = modelIndices[i * 3 + 2];

#else
        int index0 = modelIndices[i * 3];
        int index1 = modelIndices[i * 3 + 1];
        int index2 = modelIndices[i * 3 + 2];

#endif

        glm::vec3 v0Raster, v1Raster, v2Raster;
        glm::vec3 v0e, v1e, v2e;

		v0Raster = vRasters[index0];
		v1Raster = vRasters[index1];
		v2Raster = vRasters[index2];


               
		v0e = ves[index0];
		v1e = ves[index1];
		v2e = ves[index2];  

        float xmin = min3(v0Raster.x, v1Raster.x, v2Raster.x);
        float ymin = min3(v0Raster.y, v1Raster.y, v2Raster.y);
        float xmax = max3(v0Raster.x, v1Raster.x, v2Raster.x);
        float ymax = max3(v0Raster.y, v1Raster.y, v2Raster.y);

        if (xmin > imageWidth - 1 || xmax < 0 || ymin > imageHeight - 1 || ymax < 0) continue;

        uint32_t x0 = std::max(int32_t(0), (int32_t)(std::floor(xmin)));
        uint32_t x1 = std::min(int32_t(imageWidth) - 1, (int32_t)(std::floor(xmax)));
        uint32_t y0 = std::max(int32_t(0), (int32_t)(std::floor(ymin)));
        uint32_t y1 = std::min(int32_t(imageHeight) - 1, (int32_t)(std::floor(ymax)));



        //calculat the area of triangle  (area)
       // float area = edge(v0Raster, v1Raster, v2Raster);
		float total = area(v0Raster, v1Raster, v2Raster);

        //calculat the area of triangle  (area)
       // float area = edge2(v0Raster, v1Raster, v2Raster);

        v0Raster.z = 1.0f / v0Raster.z,
        v1Raster.z = 1.0f / v1Raster.z,
        v2Raster.z = 1.0f / v2Raster.z;


        for (uint32_t y = y0; y <= y1; ++y) {
            for (uint32_t x = x0; x <= x1; ++x) {
                                
                glm::vec3 pixelSample(x + 0.5, y + 0.5, 0);

                
                //calculate the areas of  three suvdivided triangles

                bool w0 = edge(v0Raster, v1Raster, pixelSample); //w0
                bool w1 = edge(v1Raster, v2Raster, pixelSample); //w1
                bool w2 = edge(v2Raster, v0Raster, pixelSample); //w2
             

                if (w0 == true && w1 == true && w2 == true) {  //inside

                                                    
                   float a2 = area(v0Raster, v1Raster, pixelSample); //w2
                   float a0 = area(v1Raster, v2Raster, pixelSample); //w0
                   float a1 = area(v2Raster, v0Raster, pixelSample); //w1

                    //calculate the ratio here
                    a0 /= total;
                    a1 /= total;
                    a2 /= total;

             

                    //calculate the z of pixelSample
                    float z;
                    float oneOverZ = v0Raster.z * a0 + v1Raster.z * a1 + v2Raster.z * a2;

					glm::vec3 color = glm::vec3(1, 0, 0) * a0 + glm::vec3(0, 1, 0) * a1 + glm::vec3(0, 0, 1) * a2;

                    z = 1 / oneOverZ;
                    
                    //code here
                                        
                    if (z < depthBuffer[y * imageWidth + x]) {

                        depthBuffer[y * imageWidth + x] = z;


                        //calculate normal vector from v0e, v1e, v2e

                        glm::vec3 n;
                        

                        n = glm::cross((glm::vec3(v1e) - glm::vec3(v0e)), (glm::vec3(v2e) - glm::vec3(v0e)));
                        n = glm::normalize(n);
                        n = (n + glm::vec3(1, 1, 1)) / 2.0f;

						//following codes are for texture mapping
						glm::vec2 uv = (textureCoords[index0] * v0Raster.z) * a0 + 
                                       (textureCoords[index1] * v1Raster.z) * a1 + 
                                       (textureCoords[index2] * v2Raster.z) * a2;

						uv = uv / oneOverZ;
                                               
						//get a texel color from texture using the uv coordinate
                        
						//is it perspective correct interpolation? (Hint : you can use the same way as z interpolation)
						glm::vec3 texelColor = glm::vec3(texture.getImageHandler()[(int)(uv.y * texture.getH()) * texture.getW() * 3 + (int)(uv.x * texture.getW()) * 3],   
                                                         texture.getImageHandler()[(int)(uv.y * texture.getH()) * texture.getW() * 3 + (int)(uv.x * texture.getW()) * 3 + 1],
                                                         texture.getImageHandler()[(int)(uv.y * texture.getH()) * texture.getW() * 3 + (int)(uv.x * texture.getW()) * 3 + 2]);


                        frameBuffer[y * imageWidth + x].r = unsigned char(texelColor.r);
                        frameBuffer[y * imageWidth + x].g = unsigned char(texelColor.g);
                        frameBuffer[y * imageWidth + x].b = unsigned char(texelColor.b);

                    }
                }
                


               // frameBuffer[y * imageWidth + x].r = 255;
               // frameBuffer[y * imageWidth + x].g = 0;
               // frameBuffer[y * imageWidth + x].b = 0;
            

            }// inner loop
		} //outer loop
    }

    auto t_end = std::chrono::high_resolution_clock::now();
    auto passedTime = std::chrono::duration<double, std::milli>(t_end - t_start).count();
    std::cerr << "Wall passed time: " << passedTime << "ms" << std::endl;


    rgb* p = frameBuffer.data();

    ppmOut.load(&p[0].r, imageHeight, imageWidth, 255, "P6");
    ppmOut.write("../../output.ppm");

    delete[] depthBuffer;

    return 0;
}