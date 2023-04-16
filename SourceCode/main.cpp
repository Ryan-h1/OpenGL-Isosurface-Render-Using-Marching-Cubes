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
 * @file main.cpp
 * 
 * @brief A C++ openGL program that renders an isosurface using the marching cubes algorithm.
 * 
 * @details This program takes command line arguments for screen width, screen height,
 * step size, minimum and maximum x, y, and z values, iso value, and function selection.
 * It initializes GLFW, GLEW, sets up shaders, and creates a window. The program
 * sets up VAOs and VBOs, camera, and lighting. The marching cubes algorithm runs
 * asynchronously, updating vertices and normals as it processes. Once the marching cubes
 * algorithm is complete, the program writes the resulting isosurface data to a PLY file.
 * The isosurface is rendered in the window with user interaction for camera control.
 * The program handles cleanup and termination upon completion.
 * 
 * @section ProcessInput
 * The processInput function is responsible for handling user inputs for controlling
 * the camera view. This function is not included in the HelperFunctions.hpp file
 * due to its dependence on numerous local variables within the main program. It
 * processes input for zooming in and out, and updates the camera and light position
 * based on the new spherical coordinates. It also handles cursor controls for modifying
 * theta and phi values, which affect the camera's orientation.
 * 
 * @note Program Structure
 * The overall structure of the program includes a central headers.hpp file which
 * incorporates several other header files, such as:
 * Constants.hpp: General constants used throughout the program
 * HelperFunctions.hpp: Utility functions, including marching cubes algorithm
 * shader.hpp: Shader loading and management functions
 * TriTable.hpp: Lookup table for the marching cubes algorithm
 * UserConstants.hpp: User-defined constants for customization
 * Worldboundaries.hpp: Class definition for the world boundaries and axes
 * 
 * @note How to Run
 * To run the program, run the make file by typing make (if using Ubuntu 22.04.2) 
 * or compile it with the appropriate libraries if on a different system,
 * and provide command line arguments for screen width, screen height,
 * step size, min x,y,z, max x,y,z, iso value, and function selection (1 - 8).
 * For example: ./assign5 1920 1080 0.1 -5.0 5.0 1.0 1
 * This will run the program with a 1920 x 1080 pixel window, step size of 0.1,
 * coordinate boundaries of (-5, -5, -5) to (5, 5, 5) with an iso value of 1.0
 * and the first of eight generating functions selected. 
 */


#include "headers.hpp"


// Camera related variables
float r = sqrt(3 * (5 * 5));
float theta = M_PI / 4.0;
float phi = atan(sqrt(50.0) / 5);

glm::vec3 eye = sphericalToCartesian(r, theta, phi);
glm::vec3 lightPos = sphericalToCartesian(r, theta, phi);
glm::vec3 origin = {0, 0, 0};
glm::vec3 up = {0, 1, 0};

// TIMING DEFAULTS
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

// VAO and VBO IDs for global use
GLuint vaoID;
GLuint vboVertexID;
GLuint vboNormalID;

// Forward declarations
typedef float (*scalar_field_3d)(float, float, float);
void processInput(GLFWwindow *window);



/***
 * @brief Main entry point for the OpenGL C++ program.
 * This function sets up the OpenGL context, initializes GLEW, processes command
 * line arguments, and manages the rendering loop. It also calls the marching cubes
 * algorithm asynchronously and writes the generated mesh to a PLY file. The
 * rendering loop updates the camera and light position based on user input, and
 * renders the isosurface and world boundaries.
 * @param argc Number of command-line arguments
 * @param argv Array of command-line arguments
 * @return int 0 if the program executes successfully, -1 otherwise
*/
int main(int argc, char* argv[]) {
    // variables
    float screenW = 1400;
    float screenH = 900;
    float stepsize = 0.1f;

    float xmin = -5;
    float xmax = 5;
    float isoval = 1;
    scalar_field_3d selectedF = f1;

    // Check commandline arguments
    if (argc > 1) screenW = atoi(argv[1]);
    if (argc > 2) screenH = atoi(argv[2]);
    if (argc > 3) stepsize = atof(argv[3]);
    if (argc > 4) xmin = atof(argv[4]);
    if (argc > 5) xmax = atof(argv[5]);
    if (argc > 6) isoval = atof(argv[6]);
    if (argc > 7) {
        int func = atoi(argv[7]);
        if (func == 2) selectedF = f2;
        else if (func == 3) selectedF = f3;
        else if (func == 4) selectedF = f4;
        else if (func == 5) selectedF = f5;
        else if (func == 6) selectedF = f6;
        else if (func == 7) selectedF = f7;
        else if (func == 8) selectedF = f8;
        // if not [2, 8]then defaults to f1
    }

    // Set y,z min/max to x min/max since it's just a box
    float ymin = xmin;
	float ymax = xmax;
	float zmin = xmin;
	float zmax = xmax;

    // Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow( screenW, screenH, "Ryan Hecht Assignment 5", NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version.\n" );
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

    // Enable sticky keys
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    // Set dark blue background
	glClearColor(0.2f, 0.2f, 0.3f, 0.0f);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

    // Enable depth test and blending
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Load shaders
    GLuint ProgramID = LoadShaders("../Shaders/PhongTexture.vertexshader", "../Shaders/PhongTexture.fragmentshader" );
    

    // Setup matrix
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glm::mat4 Projection = glm::perspective(glm::radians(45.0f), screenW / screenH, 0.001f, 1000.0f);
    glLoadMatrixf(glm::value_ptr(Projection));

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();


    glm::vec4 color(0.f, 0.8f, 0.8f, 1.0f); // colour of the rendered triangles

    glm::mat4 V = glm::lookAt(eye, origin, up);
    glm::mat4 M = glm::mat4(1.0f);
    glm::mat4 MV = V * M;
    glLoadMatrixf(glm::value_ptr(V));
    glm::mat4 MVP = Projection * V * M;

    // get Uniform Location of shader attributes
    GLuint MVPID = glGetUniformLocation(ProgramID, "MVP");
    GLuint MID = glGetUniformLocation(ProgramID, "M");
    GLuint VID = glGetUniformLocation(ProgramID, "V");
    GLuint LightPosID = glGetUniformLocation(ProgramID, "LightPosition_worldspace");
    GLuint colorID = glGetUniformLocation(ProgramID, "modelColor");
    GLuint shininessID = glGetUniformLocation(ProgramID, "shininess");

    glUseProgram(ProgramID);
    glUniformMatrix4fv(MID, 1, GL_FALSE, &M[0][0]); //model matrix always identity.
    glUniformMatrix4fv(MVPID, 1, GL_FALSE, &MVP[0][0]);
    glUniform1f(shininessID, SHININESS);

    // VAO, VBO setup
	glGenVertexArrays(1, &vaoID);
	glGenBuffers(1, &vboVertexID);
	glGenBuffers(1, &vboNormalID);

    // Axes and box
	Worldboundaries wb(origin, glm::vec3(xmin, ymin, zmin), glm::vec3(xmax, ymax, zmax));\

    // vertices from marching cubes algo and their normals
    std::vector<float> marchingVerts;
    std::vector<float> normals;

    // Calculate the expected number of vertices according to the stepsize and reserve memory for that
    GLuint expectedVertCount = ceil((xmax - xmin) / stepsize) * ceil((ymax - ymin) / stepsize) * ceil((zmax - zmin) / stepsize) * 12;
    marchingVerts.reserve(expectedVertCount);
    normals.reserve(expectedVertCount);


    std::future<bool> marching_cubes_future = std::async(std::launch::async, marching_cubes_async, std::ref(marchingVerts), std::ref(normals), selectedF, isoval, xmin, xmax, ymin, ymax, zmin, zmax, stepsize);
    std::future<void> writePLY_future;

    // Rendering loop
    bool writtenPLY = false;
    do {

        // if the async function is finished:
        if (marching_cubes_future.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {

            // Set VAO and VBOs one last time and write the PLY file if it's not already written
            if (!writtenPLY) {

                // Bind VAO
                glBindVertexArray(vaoID);
        
                glBindBuffer(GL_ARRAY_BUFFER, vboVertexID);
                glBufferData(GL_ARRAY_BUFFER, marchingVerts.size()*sizeof(marchingVerts[0]), &marchingVerts[0], GL_STATIC_DRAW);
                // 1st attribute buffer : vertices
                glEnableVertexAttribArray(0);
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
                
                glBindBuffer(GL_ARRAY_BUFFER, vboNormalID);
                glBufferData(GL_ARRAY_BUFFER, normals.size()*sizeof(normals[0]), &normals[0], GL_STATIC_DRAW);
                // 2nd attribute buffer : normals
                glEnableVertexAttribArray(1);
                glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

                glBindVertexArray(0); // unbind vao

                // Call writePLY asynchronously
                writePLY_future = std::async(std::launch::async, [&]() {
                    writePLY(marchingVerts, normals, FILE_NAME);
                });
                writtenPLY = true;
            }

        } else {
            // The async function is still running, do other things here
            glBindVertexArray(vaoID);
    
            glBindBuffer(GL_ARRAY_BUFFER, vboVertexID);
            glBufferData(GL_ARRAY_BUFFER, marchingVerts.size()*sizeof(marchingVerts[0]), &marchingVerts[0], GL_STATIC_DRAW);
            // 1st attribute buffer : vertices
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

            glBindBuffer(GL_ARRAY_BUFFER, vboNormalID);
            glBufferData(GL_ARRAY_BUFFER, normals.size()*sizeof(normals[0]), &normals[0], GL_STATIC_DRAW);
            // 2nd attribute buffer : normals
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

    
            glBindVertexArray(0); // unbind vao
        }

        // Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use the program
        glUseProgram(ProgramID);

		// Recalculate matrix
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		Projection = glm::perspective(glm::radians(45.0f), screenW/screenH, 0.001f, 1000.0f);
		glLoadMatrixf(glm::value_ptr(Projection));

		glMatrixMode( GL_MODELVIEW );
		glPushMatrix();

        // Calculate per-frame time logic
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Handle input:
		processInput(window);

        // Recalculate projection matrix
        V = glm::lookAt(eye, origin, up);
        M = glm::mat4(1.0f);
        MV = M * V;
        glLoadMatrixf(glm::value_ptr(V));
        MVP = Projection * V;
        
        // Update uniform variables
        glUniformMatrix4fv(MVPID, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(VID, 1, GL_FALSE, &V[0][0]);
		glUniform3f(LightPosID, lightPos.x, lightPos.y, lightPos.z);
		glUniform4fv(colorID, 1, &color[0]);
	
		// Draw from server side array
		glBindVertexArray(vaoID);
		glDrawArrays(GL_TRIANGLES, 0, marchingVerts.size()); 	
		glBindVertexArray(0);

        // Unuse the program
        glUseProgram(0);
        wb.draw();

        // GLFW event processing and buffer swapping
        glfwSwapBuffers(window);
        glfwPollEvents();

    } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
             glfwWindowShouldClose(window) == 0);

    // Cleanup and terminate
    glDeleteBuffers(1, &vboVertexID);
    glDeleteBuffers(1, &vboNormalID);
    glfwTerminate();

    return 0;
}


/***
 * @brief Processes user input for controlling the camera and light position.
 * This function is responsible for handling user inputs such as zooming in and
 * out, updating the camera and light position based on new spherical coordinates,
 * and managing cursor controls for modifying theta and phi values affecting the
 * camera's orientation.
 * @param window Pointer to the GLFWwindow object
*/
void processInput(GLFWwindow *window) {
    // Camera controls
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        r -= ZOOM_SPEED * deltaTime;
        if (r < 0.1f) r = 0.1f; // Prevent r from becoming too small
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        r += ZOOM_SPEED * deltaTime;
    }

    // Cursor controls
    static double lastMouseX, lastMouseY;
    double mouseX, mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        // Calculate cursor movement deltas
        float deltaX = mouseX - lastMouseX;
        float deltaY = mouseY - lastMouseY;

        // Modify theta and phi
        theta += MOUSE_SPEED * deltaX * deltaTime;
        phi -= MOUSE_SPEED * deltaY * deltaTime;

        // Clamp phi between a small epsilon and PI - epsilon
        const float epsilon = 0.01f;
        phi = glm::clamp(phi, epsilon, glm::pi<float>() - epsilon);
    }

    lastMouseX = mouseX;
    lastMouseY = mouseY;

    // Update camera and light position based on the new spherical coordinates
    eye = sphericalToCartesian(r, theta, phi);
    lightPos = sphericalToCartesian(r, theta, phi);
}