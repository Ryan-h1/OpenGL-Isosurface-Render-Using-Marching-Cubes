# OpenGL-Isosurface-Render-Using-Marching-Cubes
This is a repository for a C++ OpenGL application I made for my CS 3388 Computer Graphics I class at Western University.

## Assignment 5 Header

Ryan Hecht

Professor Brandt

CS 3388 Computer Graphics I

Due Date: 10 April 2023

Created: 17 March 2023

Last Edited: 25 March 2023

## Description

    This program is a C++ openGL application that renders an isosurface using the marching cubes 
    algorithm. It takes command line arguments for screen width, screen height, step size, 
    minimum and maximum x, y, and z values, iso value, and function selection. The program 
    initializes GLFW, GLEW, sets up shaders, and creates a window. It also sets up VAOs and V
    BOs, camera, and lighting. The marching cubes algorithm runs asynchronously, updating 
    vertices and normals as it processes. Once the marching cubes algorithm is complete, 
    the program writes the resulting isosurface data to a PLY file. The isosurface is rendered 
    in the window with user interaction for camera control. The program handles cleanup and 
    termination upon completion.

## Requirements

    1. C++ Standard Library
    2. OpenGL
    3. GLEW (OpenGL Extension Wrangler Library)
    4. GLFW (OpenGL Graphics Library Framework)
    5. GLM (OpenGL Mathematics)
    6. Support for GLSL 3.30.

## Program Structure

### "ExampleScreenshots" Folder
    Contains screenshots of the first three scalar functions

### "ExamplePLYFiles" Folder
    Contains PLY files of the first two scalar functions

### "OutputPLYFiles" Folder
    Contains the PLY files generated and output by this program.

### "Shaders" Folder
    Contains the Phong-like shaders used in the shader program.

### "SourceCode" Folder
    headers.hpp: Central header file including:
    Constants.hpp: General constants used throughout the program
    HelperFunctions.hpp: Utility functions, including marching cubes algorithm
    shader.hpp: Shader loading and management functions
    TriTable.hpp: Lookup table for the marching cubes algorithm
    UserConstants.hpp: User-defined constants for customization
    WorldBoundaries.hpp: Class definition for the world boundaries and axes
    Running the Program:

## How to Run the Program

    Compile the program using the provided makefile (if using Ubuntu 22.04.2) with the 
    appropriate libraries on your system. Use your own form of compilation if using an
    alternative operating system. Provide command line arguments for the
    screen width, screen height, step size, min x,y,z, max x,y,z, iso value, and function 
    selection [1-8].

    For Example: './assign5 1920 1080 0.1 -5.0 5.0 1.0 1'

    This example will run the program with a 1920 x 1080 pixel window, step size of 0.1, 
    coordinate boundaries of (-5, -5, -5) to (5, 5, 5) with an iso value of 1.0 and the 
    first of eight generating functions selected.

## A Note on Compatibility

    This file uses the standard dev tools from the OpenGL library. It also uses GLEW and GLM. Even if you have the standard
    OpenGL dev tools downloaded, you must still download GLEW and GLM to your machine seperately. This program was
    made on Ubuntu 22.04.2, with an Intel CPU and Nvidia GPU. Please note there may be compatibility issues with different
    operating systems and hardware, particularly Apple Silicone and Intel GPUs. There may also be compatibility issues with
    high retina displays.

## Additional Notes

    Please note that small stepsizes use A LOT of memory. If you are getting memory issues, please
    try using the default stepsize of 0.1.

## Acknowledgments
    
    This program was completed as an assignment for CS 3388 Computer Graphics I at 
    Western University and uses some files provided by Professor Alexandar Brandt, such
    as shader.hpp.

## Example Screenshots
![Alt text](/ExampleScreenshots/scalar1.png?raw=true "Scalar Function 1")

![Alt text](/ExampleScreenshots/scalar2.png?raw=true "Scalar Function 2")

![Alt text](/ExampleScreenshots/scalar3.png?raw=true "Scalar Function 3")

