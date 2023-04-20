/***
 * Ryan Hecht
 * Professor Brandt
 * CS 3388 Computer Graphics I
 * Due Date: 10 April 2023
 * 
 * Created: 17 March 2023
 * Last Edited: 25 March 2023
 * 
 * Assignment 5
 * 
 * @file HelperFunctions.hpp
 * 
 * @brief Contains various utility functions used by the main program.
 * This file contains utility functions for converting spherical to Cartesian
 * coordinates, computing vector normals, running the Marching Cubes algorithm
 * asynchronously, writing the PLY output file, and defining scalar field
 * generating functions.
 * 
 */


#ifndef HELPERFUNCTIONS_HPP
#define HELPERFUNCTIONS_HPP

#include <glm/glm.hpp>
#include <fstream>
#include <string>
#include <vector>

typedef float (*scalar_field_3d)(float, float, float);

/***
 * @brief Convert spherical coordinates to Cartesian coordinates.
 * @param r Radius
 * @param theta Azimuth angle
 * @param phi Polar angle
 * @return glm::vec3 Cartesian coordinates (x, y, z)
*/
glm::vec3 sphericalToCartesian(float r, float theta, float phi) {
    float x = r * sin(phi) * cos(theta);
    float y = r * cos(phi);
    float z = r * sin(phi) * sin(theta);
    return glm::vec3(x, y, z);
}


/***
 * @brief Compute normals for each vertex in a given list of vertices.
 * @param vertices A list of vertices
 * @return std::vector<float> A list of computed normals for each vertex
*/
std::vector<float> compute_normals(const std::vector<float>& vertices) {
    const size_t num_vertices = vertices.size() / 3;
    std::vector<float> normals(num_vertices * 3);
    const size_t num_triangles = num_vertices / 3;

    for (size_t i = 0; i < num_triangles; ++i) {
        size_t idx = i * 9;

        // Due to winding order, the direction of the normal is set by the order of vertices
        glm::vec3 v1 = {vertices[idx], vertices[idx + 1], vertices[idx + 2]};
        glm::vec3 v2 = {vertices[idx + 3], vertices[idx + 4], vertices[idx + 5]};
        glm::vec3 v3 = {vertices[idx + 6], vertices[idx + 7], vertices[idx + 8]};

        glm::vec3 diff1 = v1 - v2;
        glm::vec3 diff2 = v1 - v3;
        glm::vec3 norm = glm::normalize(glm::cross(diff1, diff2));

        // Store the computed normal for each vertex of the triangle
        for (size_t j = 0; j < 3; ++j) {
            size_t normal_idx = i * 9 + j * 3;
            normals[normal_idx] = norm.x;
            normals[normal_idx + 1] = norm.y;
            normals[normal_idx + 2] = norm.z;
        }
    }

    return normals;
}


/***
 * @brief Run the Marching Cubes algorithm asynchronously.
 * @param[out] vertices Output vertices
 * @param[out] normals Output normals
 * @param f Scalar field generating function
 * @param isoval Isovalue
 * @param minx Minimum x value
 * @param maxx Maximum x value
 * @param miny Minimum y value
 * @param maxy Maximum y value
 * @param minz Minimum z value
 * @param maxz Maximum z value
 * @param stepsize Step size for the algorithm
 * @return bool Returns true if the algorithm executed successfully
*/
bool marching_cubes_async(std::vector<float>& vertices, std::vector<float>& normals, scalar_field_3d f, float isoval, float minx, float maxx, float miny, float maxy, float minz, float maxz, float stepsize) {
    
    std::vector<float> tempVertices;
    std::vector<float> tempNormals;

	float x = minx;
	float y = miny;
	float z = minz;
	float ftl, ftr, fbr, fbl, ntl, ntr, nbr, nbl;
	int which = 0;
	int* verts;

	for ( ; x < maxx; x += stepsize) {
        for (y = miny; y < maxy; y += stepsize) {
            float ystep = y + stepsize;
            for (z = minz; z < maxz; z += stepsize) {
                float zstep = z + stepsize;

                // Test the cube
                ntl = (*f)(x, ystep, zstep);
                ntr = (*f)(x + stepsize, ystep, zstep);
                nbr = (*f)(x + stepsize, y, zstep);
                nbl = (*f)(x, y, zstep);
                ftl = (*f)(x, ystep, z);
                ftr = (*f)(x + stepsize, ystep, z);
                fbr = (*f)(x + stepsize, y, z);
                fbl = (*f)(x, y, z);

				which = 0;

                if (ntl < isoval) which |= NEAR_TOP_LEFT;
                if (ntr < isoval) which |= NEAR_TOP_RIGHT;
                if (nbr < isoval) which |= NEAR_BOTTOM_RIGHT;
                if (nbl < isoval) which |= NEAR_BOTTOM_LEFT;
                if (ftl < isoval) which |= FAR_TOP_LEFT;
                if (ftr < isoval) which |= FAR_TOP_RIGHT;
                if (fbr < isoval) which |= FAR_BOTTOM_RIGHT;
                if (fbl < isoval) which |= FAR_BOTTOM_LEFT;

				verts = marching_cubes_lut[which];
				
                // Generate vertices
                for (int i = 0; verts[i] >= 0; i += 3) {
                    for (int j = 0; j < 3; ++j) {
                        tempVertices.emplace_back(x + stepsize * vertTable[verts[i + j]][0]);
						tempVertices.emplace_back(y + stepsize * vertTable[verts[i + j]][1]);
						tempVertices.emplace_back(z + stepsize * vertTable[verts[i + j]][2]);
                    }
                    
                }
			
			}
		}
        tempNormals = compute_normals(tempVertices);
		vertices.insert(vertices.end(), tempVertices.begin(), tempVertices.end());
		normals.insert(normals.end(), tempNormals.begin(), tempNormals.end());

        // Clear our temporary holders
        tempVertices.clear();
        tempNormals.clear();
	}

	return true;
}


/***
 * @brief Write the vertices and normals to a PLY file.
 * @param vertices A list of vertices
 * @param normals A list of normals
 * @param fileName The output PLY file name
*/
void writePLY(const std::vector<float>& vertices, const std::vector<float>& normals, const std::string& fileName) {
    assert(vertices.size() == normals.size());
    assert(vertices.size() % 9 == 0); // Ensure that vertices can form complete triangles

    std::string outputPath = "../OutputPLYFiles/" + fileName; // Add the directory path
    printf("Writing to PLY file : %s\n", fileName.c_str());

    std::ofstream outputFile(outputPath);

    // Write PLY header
    outputFile << "ply\n";
    outputFile << "format ascii 1.0\n";
    outputFile << "element vertex " << vertices.size() / 3 << "\n";
    outputFile << "property float x\n";
    outputFile << "property float y\n";
    outputFile << "property float z\n";
    outputFile << "property float nx\n";
    outputFile << "property float ny\n";
    outputFile << "property float nz\n";
    outputFile << "element face " << vertices.size() / 9 << "\n";
    outputFile << "property list uchar int vertex_indices\n";
    outputFile << "end_header\n";

    // Write vertices and normals
    for (size_t i = 0; i < vertices.size(); i += 3) {
        outputFile << vertices[i] << " " << vertices[i + 1] << " " << vertices[i + 2] << " ";
        outputFile << normals[i] << " " << normals[i + 1] << " " << normals[i + 2] << "\n";
    }

    // Write faces
    for (size_t i = 0; i < vertices.size() / 3; i += 3) {
        outputFile << "3 " << i << " " << (i + 1) << " " << (i + 2) << "\n";
    }

	printf("Finished : %s\n", fileName.c_str());

    outputFile.close();
}


/***
 * Our generating functions
*/
float f1(float x, float y, float z) {
	return x*x+y*y+z*z;
}
float f2(float x, float y, float z) {
	return y-sin(x)*cos(z);
}
float f3(float x, float y, float z) {
	return x*x-y*y-z*z-z;
}
// Torus shape
float f4(float x, float y, float z) {
	float r1 = 3.0; // radius of torus
	float r2 = 1.0; // radius of tube
	float d = sqrt(x*x+y*y);
	return (r1 - d)*(r1 - d) + z*z - r2*r2;
}
// Saddle
float f5(float x, float y, float z) {
	return x*x - y*y - z*z*z;
}
// Sphere with a hole
float f6(float x, float y, float z) {
	float r1 = 3.0; // radius of outer sphere
	float r2 = 1.0; // radius of inner sphere
	float d = sqrt(x*x+y*y+z*z);
	return (d - r1)*(d - r2);
}
// Twisted Torus
float f7(float x, float y, float z) {
	float r1 = 3.0; // radius of torus
	float r2 = 1.0; // radius of tube
	float d = sqrt(x*x+y*y);
	float twist = sin(x+y+z);
	return (r1 - d)*(r1 - d) + (z - twist)*(z - twist) - r2*r2;
}
// Mobius strip
float f8(float x, float y, float z) {
	float a = 0.5; // width of strip
	float b = 2.0; // radius of loop
	float u = x*cos(z) + y*sin(z);
	float v = -x*sin(z) + y*cos(z);
	return v*v - b*b + a*u;
}


/***
 * Marching Cubes Algorithm ORIGINAL - FOR NON ASYNCHRONOUS CALLS
*/
/***
std::vector<float> marching_cubes(scalar_field_3d f, float isoval, float minx, float maxx, float miny, float maxy, float minz, float maxz, float stepsize) {
	std::vector<float> vertices;
	float x = minx;
	float y = miny;
	float z = minz;
	float ftl, ftr, fbr, fbl, ntl, ntr, nbr, nbl;
	int which = 0;
	int* verts;

	for ( ; x < maxx; x += stepsize) {
		for (y = miny ; y < maxy; y += stepsize) {
			for (z = minz ; z < maxz ; z += stepsize){
				//test the cube
				ntl = (*f)(x, y+stepsize, z+stepsize);
				ntr = (*f)(x+stepsize, y+stepsize, z+stepsize);
				nbr = (*f)(x+stepsize, y, z+stepsize);
				nbl = (*f)(x, y, z+stepsize);
				ftl = (*f)(x, y+stepsize, z);
				ftr = (*f)(x+stepsize, y+stepsize, z);
				fbr = (*f)(x+stepsize, y, z);
				fbl = (*f)(x, y, z);

				which = 0;
                if (ntl < isoval) which |= NEAR_TOP_LEFT;
                if (ntr < isoval) which |= NEAR_TOP_RIGHT;
                if (nbr < isoval) which |= NEAR_BOTTOM_RIGHT;
                if (nbl < isoval) which |= NEAR_BOTTOM_LEFT;
                if (ftl < isoval) which |= FAR_TOP_LEFT;
                if (ftr < isoval) which |= FAR_TOP_RIGHT;
                if (fbr < isoval) which |= FAR_BOTTOM_RIGHT;
                if (fbl < isoval) which |= FAR_BOTTOM_LEFT;

				verts = marching_cubes_lut[which];
				
                // Generate vertices
                for (int i = 0; verts[i] >= 0; i += 3) {
                    for (int j = 0; j < 3; ++j) {
                        vertices.emplace_back(x + stepsize * vertTable[verts[i + j]][0]);
                        vertices.emplace_back(y + stepsize * vertTable[verts[i + j]][1]);
                        vertices.emplace_back(z + stepsize * vertTable[verts[i + j]][2]);
                    }
                }
			
			}
		}
	}

	return vertices;
}
*/


#endif // HELPERFUNCTIONS_HPP