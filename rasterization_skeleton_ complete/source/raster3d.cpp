
#define _USE_MATH_DEFINES 
#define GLM_ENABLE_EXPERIMENTAL

#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <fstream>
#include <chrono>

#include "cow.h"
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


float edge(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c)
{
    return (c[0] - a[0]) * (b[1] - a[1]) - (c[1] - a[1]) * (b[0] - a[0]);
    //   return ((b.x-a.x) * (c.y-a.y) - (b.y-a.y)*(c.x-a.x));
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


const uint32_t ntris = 3156;
const float nearClippingPlane = 1;
const float farClippingPLane = 1000;



int main(int argc, char** argv)
{
  
    glm::mat4 worldToCamera = glm::lookAt(glm::vec3(20, 10, 20), glm::vec3(0, 5, 0), glm::vec3(0, 1, 0));

    float t, b, l, r;

    PPM ppmOut;
    ppmOut.setBinary(true);



    struct rgb
    {
        uint8_t r, g, b;
    };

    std::vector <rgb>  frameBuffer;
    frameBuffer.resize(imageWidth * imageHeight);


    for (uint32_t i = 0; i < imageWidth * imageHeight; ++i) {
        frameBuffer[i].r = 0;
        frameBuffer[i].g = 0;
        frameBuffer[i].b = 0;
    }

    float* depthBuffer = new float[imageWidth * imageHeight];

    for (uint32_t i = 0; i < imageWidth * imageHeight; ++i) {
        depthBuffer[i] = farClippingPLane;
    }

    auto t_start = std::chrono::high_resolution_clock::now();

    for (uint32_t i = 0; i < ntris; ++i) {

        const glm::vec3& v0 = vertices[nvertices[i * 3]];
        const glm::vec3& v1 = vertices[nvertices[i * 3 + 1]];
        const glm::vec3& v2 = vertices[nvertices[i * 3 + 2]];


        glm::mat4 modelMatrix(1.0f);

        glm::mat4 viewMatrix = lookAt(glm::vec3(20, 10, 20), glm::vec3(0, 5, 0), glm::vec3(0, 1, 0));

        glm::mat4 modelViewMatrix = viewMatrix * modelMatrix;

        //Camera(view) coordinates
        glm::vec4 v0e;
        glm::vec4 v1e;
        glm::vec4 v2e;

        //Code here (calculate v0e, v1e, v2e here)


        glm::mat4 projection = perspective((45.0f), imageWidth / (float)imageHeight, nearClippingPlane, farClippingPLane);



        //Clip coodinates
        glm::vec4 v0c, v1c, v2c;
        
        v0e = modelViewMatrix * glm::vec4(v0, 1.0f);
        v1e = modelViewMatrix * glm::vec4(v1, 1.0f);
        v2e = modelViewMatrix * glm::vec4(v2, 1.0f);


        //Code here (calculate v0c, v1c, v2c here)


        v0c = projection * v0e;
        v1c = projection * v1e;
        v2c = projection * v2e;


        //Perspective division (divide v0c.x, v0c.y, v0c.z by v0c.w   (same to v1c, v2c)
        v0c.x = v0c.x / v0c.w;
        v0c.y = v0c.y / v0c.w;
        v0c.z = v0c.z / v0c.w;

        v1c.x = v1c.x / v1c.w;
        v1c.y = v1c.y / v1c.w;
        v1c.z = v1c.z / v1c.w;

        v2c.x = v2c.x / v2c.w;
        v2c.y = v2c.y / v2c.w;
        v2c.z = v2c.z / v2c.w;


        //Code here


        glm::vec3 v0Raster, v1Raster, v2Raster;
        convertNDCtoImage(v0c, v0e, imageWidth, imageHeight, v0Raster);
        convertNDCtoImage(v1c, v1e, imageWidth, imageHeight, v1Raster);
        convertNDCtoImage(v2c, v2e, imageWidth, imageHeight, v2Raster);

      

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
        float area = edge(v0Raster, v1Raster, v2Raster);

        for (uint32_t y = y0; y <= y1; ++y) {
            for (uint32_t x = x0; x <= x1; ++x) {
                                
                glm::vec3 pixelSample(x + 0.5, y + 0.5, 0);

                /*
                //calculate the areas of  three suvdivided triangles

                float w0 = edge(v1Raster, v2Raster, pixelSample); //w0
                float w1 = edge(v2Raster, v0Raster, pixelSample); //w1
                float w2 = edge(v0Raster, v1Raster, pixelSample); //w2


                if (w0 >= 0 && w1 >= 0 && w2 >= 0) { //inside

                    
                    //calculate the ratio here
                    w0 /= area;
                    w1 /= area;
                    w2 /= area;

                    //calculate the z of pixelSample
                    float z;
                    float oneOverZ = v0Raster.z * w0 + v1Raster.z * w1 + v2Raster.z * w2;
                    z = 1 / oneOverZ;
                    
                    //code here
                                        
                    if (z < depthBuffer[y * imageWidth + x]) {

                        depthBuffer[y * imageWidth + x] = z;


                        //calculate normal vector from v0e, v1e, v2e

                        glm::vec3 n;
                        

                        n = glm::cross((glm::vec3(v1e) - glm::vec3(v0e)), (glm::vec3(v2e) - glm::vec3(v0e)));
                        n = glm::normalize(n);
                        n = (n + glm::vec3(1, 1, 1)) / 2.0f;



                        frameBuffer[y * imageWidth + x].r = n.x * 255;
                        frameBuffer[y * imageWidth + x].g = n.y * 255;
                        frameBuffer[y * imageWidth + x].b = n.z * 255;
                    }
                }
                */


                frameBuffer[y * imageWidth + x].r = 255;
                frameBuffer[y * imageWidth + x].g = 0;
                frameBuffer[y * imageWidth + x].b = 0;
            

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