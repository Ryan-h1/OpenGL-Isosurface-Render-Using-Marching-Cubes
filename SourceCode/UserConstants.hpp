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
 * @file UserConstants.hpp
 * 
 * @brief User-defined constants for output file and user interaction parameters.
 * This file contains constants that can be modified by users to customize
 * the output file name and user interaction parameters such as zoom and mouse
 * speed.
 * 
 * Constants:
 * 
 * FILE_NAME: Default output file name for the generated PLY file (default: "output.ply").
 * ZOOM_SPEED: Zoom speed controlled by arrow keys (default: 6.0f).
 * MOUSE_SPEED: Mouse speed control by cursor and left (default: 0.5f).
 */

#ifndef USERCONSTANTS_HPP
#define USERCONSTANTS_HPP

std::string FILE_NAME = "output.ply"; // Choose a file name for the output PLY file
float ZOOM_SPEED = 6.0f;              // Zoom speed controlled by arrow keys
float MOUSE_SPEED = 0.5f;             // Mouse speed control by cursor and left


#endif // USERCONSTANTS_HPP