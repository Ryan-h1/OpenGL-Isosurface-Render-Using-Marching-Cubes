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
 * @file Constants.hpp
 * 
 * @brief Definitions of constants used in the Marching Cubes Algorithm.
 * This file contains the constants representing the 8 vertices of a cube
 * for the Marching Cubes Algorithm. Each constant is assigned a specific
 * binary value, which is used in the computation of the cube configuration
 * and vertex generation. It also contains some graphics constants, such as
 * shininess.
 * 
 * Constants:
 * 
 * NEAR_TOP_LEFT: 128
 * NEAR_TOP_RIGHT: 64
 * FAR_TOP_RIGHT: 32
 * FAR_TOP_LEFT: 16
 * NEAR_BOTTOM_LEFT: 8
 * NEAR_BOTTOM_RIGHT: 4
 * FAR_BOTTOM_RIGHT: 2
 * FAR_BOTTOM_LEFT: 1
 */

#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

// MARCHING CUBE ALGORITHM CONSTANTS
const int NEAR_TOP_LEFT = 128;
const int NEAR_TOP_RIGHT = 64;
const int FAR_TOP_RIGHT = 32;
const int FAR_TOP_LEFT = 16;
const int NEAR_BOTTOM_LEFT = 8;
const int NEAR_BOTTOM_RIGHT = 4;
const int FAR_BOTTOM_RIGHT = 2;
const int FAR_BOTTOM_LEFT = 1;

// GRAPHICS CONSTANTS
const float SHININESS = 64.0;


#endif // CONSTANTS_HPP