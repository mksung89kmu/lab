#ifndef VBOPLANE_H
#define VBOPLANE_H


class VBOPlane 
{
private:
    unsigned int vaoHandle;
    int faces;

public:
	//explain the parameters in the constructor
	//xsize and zsize are the size of the plane in x and z direction, respectively. The plane is centered at the origin.
	//xdivs and zdivs are the number of subdivisions in x and z direction, respectively. The total number of faces will be xdivs * zdivs.
	//smax and tmax are the maximum texture coordinates in s and t direction, respectively. The texture coordinates will be generated in the range [0, smax] and [0, tmax].
	//if we increase the number of subdivisions, the plane will be smoother and more detailed, but it will also require more memory and processing power to render. 
	// The texture coordinates can be used to map a texture image onto the plane, and the smax and tmax parameters allow you to control how the texture is repeated across the plane.
    VBOPlane(float, float, int, int, float smax = 1.0f, float tmax = 1.0f);

    void draw() const;
};

#endif // VBOPLANE_H
