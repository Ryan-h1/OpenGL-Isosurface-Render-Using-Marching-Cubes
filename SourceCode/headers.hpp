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
 * @file headers.hpp
 * 
 * @brief A collection of all the necessary headers and libraries for the main program.
 * This file includes all the required libraries, headers, and global variables
 * for the main program. It aggregates all dependencies in a single file for
 * easier management and maintainability.
 * 
 * Included libraries and headers:
 * 
 * Standard C++ libraries (stdio, stdlib, cmath, functional, fstream)
 * OpenGL libraries (GLEW, GLFW)
 * GLM (OpenGL Mathematics) for 3D math operations
 * C++ Standard Library features (iostream, vector, future, thread, chrono)
 * Project-specific headers (TriTable, Constants, shader, Worldboundaries,
 * HelperFunctions, UserConstants)
 */

#ifndef HEADERS_HPP
#define HEADERS_HPP

#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <functional>
#include <fstream>

// Include glew
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

#include <iostream>
#include <vector>
#include <future>
#include <thread>
#include <chrono>

#include "TriTable.hpp"
#include "Constants.hpp"
#include "shader.hpp"
#include "Worldboundaries.hpp"
#include "HelperFunctions.hpp"
#include "UserConstants.hpp"

#endif // HEADERS_HPP