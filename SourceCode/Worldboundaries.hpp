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
 * @file Worldboundaries.hpp
 * 
 * @brief A class for drawing the boundaries of the world. This file defines
 * the Worldboundaries class, which is used to draw the boundaries of the
 * world with OpenGL. The boundaries are defined by an origin point and the
 * positive and negative bounds in each of the three axes. The class contains
 * a draw() function that draws the coordinate axes and a box that encloses
 * the world boundaries.
 * 
 * Class:
 * 
 * Worldboundaries: A class for drawing the boundaries of the world. This class
 * has a constructor that takes three glm::vec3 objects as input to define the
 * world boundaries. It also contains a draw() function that draws the coordinate
 * axes and a box that encloses the world boundaries using OpenGL.
 * 
 */

#pragma once

#include <glm/glm.hpp>
#include <GL/glew.h>


/***
 * Worldboundaries
*/
class Worldboundaries {

private:

	glm::vec3 origin;
	glm::vec3 posBound;
	glm::vec3 negBound;

public:

	/**
	 * @brief Constructs a Worldboundaries object with the given origin and 
	 * positive and negative bounds in each of the three axes.
	 * 
	 * @param origin The origin point for the world boundaries.
	 * @param posBound The positive bounds in each of the three axes.
	 * @param negBound The negative bounds in each of the three axes.
	 */
	Worldboundaries(glm::vec3 origin, glm::vec3 posBound, glm::vec3 negBound) : 
	origin(origin), posBound(posBound), negBound(negBound) {}

	/**
	 * @brief Draws the coordinate axes and box that encloses the world 
	 * boundaries using OpenGL.
	 */
	void draw() {

		glMatrixMode( GL_MODELVIEW );
		glPushMatrix();

		// DRAW THE COORDINATE AXES
		glLineWidth(2.0f);
		glBegin(GL_LINES);

		// X-axis
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(origin.x, origin.y, origin.z);
		glVertex3f(origin.x + posBound.x, origin.y, origin.z);
		glVertex3f(origin.x, origin.y, origin.z);
		glVertex3f(origin.x + negBound.x, origin.y, origin.z);

		// Y-axis
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(origin.x, origin.y, origin.z);
		glVertex3f(origin.x, origin.y + posBound.y, origin.z);
		glVertex3f(origin.x, origin.y, origin.z);
		glVertex3f(origin.x, origin.y + negBound.y, origin.z);

		// Z-axis
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(origin.x, origin.y, origin.z);
		glVertex3f(origin.x, origin.y, origin.z + posBound.z);
		glVertex3f(origin.x, origin.y, origin.z);
		glVertex3f(origin.x, origin.y, origin.z + negBound.z);
		
		
		glEnd();

		// DRAW THE BOX
		glColor3f(0.8f, 0.8f, 0.0f);
		glLineWidth(2.0f);
		glBegin(GL_LINE_STRIP);
			glVertex3f(negBound.x, negBound.y, negBound.z);
			glVertex3f(posBound.x, negBound.y, negBound.z); 
			glVertex3f(posBound.x, negBound.y, posBound.z);
			glVertex3f(negBound.x, negBound.y, posBound.z);

			glVertex3f(negBound.x, negBound.y, negBound.z);
			glVertex3f(negBound.x, posBound.y, negBound.z);
			glVertex3f(posBound.x, posBound.y, negBound.z);
			glVertex3f(posBound.x, negBound.y, negBound.z);	

			glVertex3f(posBound.x, posBound.y, negBound.z);
			glVertex3f(posBound.x, posBound.y, posBound.z);
			glVertex3f(posBound.x, negBound.y, posBound.z);
			glVertex3f(posBound.x, posBound.y, posBound.z);

			glVertex3f(negBound.x, posBound.y, posBound.z);
			glVertex3f(negBound.x, negBound.y, posBound.z);
			glVertex3f(negBound.x, posBound.y, posBound.z);
			glVertex3f(negBound.x, posBound.y, negBound.z);
			
		glEnd();

		glPopMatrix();
	}

};