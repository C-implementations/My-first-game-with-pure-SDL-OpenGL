// Third party libraries
#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <glm/glm.hpp>

// own libraries
#include "display/display.h"

// C++ standard template library (STL)
#include <vector>
#include <iostream>
#include <fstream>

/* Test of glm */
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective
#include <glm/ext/scalar_constants.hpp> // glm::pi

glm::mat4 camera(float Translate, glm::vec2 const& Rotate)
{
	glm::mat4 Projection = glm::perspective(glm::pi<float>() * 0.25f, 4.0f / 3.0f, 0.1f, 100.f);
	glm::mat4 View = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -Translate));
	View = glm::rotate(View, Rotate.y, glm::vec3(-1.0f, 0.0f, 0.0f));
	View = glm::rotate(View, Rotate.x, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 Model = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
	return Projection * View * Model;
}

/* end of glm */

/* globals */
std::string LoadShaderAsString(const std::string& fileName)
{
    /* Resulting shader program loaded as a single string */
    std::string result = "";

    std::string line = "";
    std::ifstream myFile(fileName.c_str());

    if (myFile.is_open())
    {
        while (std::getline(myFile, line)) {
            result += line + '\n';
        }

        myFile.close();
    }

    return result;
}

// shader
// The following stores the a unique id for the graphics pipeline
// program object that will be used for our OpenGL draw calls.
GLuint gGraphicsPipelineShaderProgram = 0;

// OpenGL Objects
// Vertex Array Object (VAO)
// Vertex array objects encapsulate all of the items needed to render an object.
// For example, we may have multiple vertex buffer objects (VBO) related to rendering one
// object. The VAO allows us to setup the OpenGL state to render that object using
// the correct layout and correct buffers with one call after being setup.

// Vertex Buffer Object (VBO)
// Vertex Buffer Objects store information relating to vertices (e.g. positions,
// normals, textures)
// VBOs are our mechanism for arranging geometry on the GPU.
GLuint gVertexArrayObject = 0; // VAO
GLuint gVertexBufferObject = 0; // VBO
GLuint gIndexBufferObject = 0; // IBO (EBO)

/* Error handling routines */
static void GLClearAllErrors()
{
    while (glGetError() != GL_NO_ERROR)
    {

    }
}

static bool GLCheckErrorStatus(const char* function, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "OpenGL Error: " << error 
        << "\tLine: " << line 
        << "\tFunction: " << function << std::endl;
        return true;
    }

    return false;
}

#define GLCheck(x) GLClearAllErrors(); x; GLCheckErrorStatus(#x, __LINE__);

// Shaders
// Here we setup two shaders, a vertex shader and a fragment shader.
// At a minimum, every Modern OpenGL program needs a vertex and a fragment shader.

// OpenGl provides functions that will compile the shader source code
// (which are simply stored as strings) at run-time.

// Vertex Shader
// Vertex shader executes once per vertex, and will be in charge of the
// final position of the vertex


// Fragment shader
// The fragment shader executes once per fragment (i.e roughly for every pixel
// at will be rasterized),
// and in part determines the final color that will be sent to the screen.

/*

Initialization of the graphics application. Typically this will involve setting
up a window.

and the OpenGL context (with the appropriate version)

@return void

*/
void PreDraw(Display* display)
{
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    glViewport(0,0, display->getScreenWidth(), display->getScreenHeight());
    glClearColor(1.0f, 0.984f, 0.0f, 1.f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    glUseProgram(gGraphicsPipelineShaderProgram);
    
    // Model transformation by translating the object to world space
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, display->getUOffset()));
    // Rotate it
    model = glm::rotate(model, glm::radians(display->getGRotate()), glm::vec3(1.0f, 1.0f, 1.0f));
    model = glm::scale(model, glm::vec3(display->getGScale(), display->getGScale(), display->getGScale()));

    // Retrieve our location of our model matrix
    GLint u_ModelMatrixLocation = glGetUniformLocation(gGraphicsPipelineShaderProgram, "u_ModelMatrix");

    if (u_ModelMatrixLocation >= 0) {
        glUniformMatrix4fv(u_ModelMatrixLocation, 1, GL_FALSE, &model[0][0]);
    } else {
        std::cout << "Could not find model matrix uniform(s), maybe a mispelling?\n" << std::endl;
    }

    // Projection matrix (in perspective)
    glm::mat4 projection = glm::perspective(
        glm::radians(45.0f),
        (float) display->getScreenWidth()/(float)display->getScreenHeight(),
        0.1f, // how close can we see things, if something is closer then we cannot see it
        10.0f
    );

    // Retrieve our location of our perspective matrix
    GLint u_ProjectionLocation = glGetUniformLocation(gGraphicsPipelineShaderProgram, "u_Projection");

    if (u_ProjectionLocation >= 0) {
        glUniformMatrix4fv(u_ProjectionLocation, 1, GL_FALSE, &projection[0][0]);
    } else {
        std::cout << "Could not find projection uniform, maybe a mispelling?\n" << std::endl;
    }
}

void Draw()
{
    /* Enable our attributes */
    glBindVertexArray(gVertexArrayObject);
    
    /* Select the vertex buffer object we want to enable */
    glBindBuffer(GL_ARRAY_BUFFER, gVertexBufferObject);
    
    /* Render data */
    //glDrawArrays(GL_TRIANGLES, 0, 6);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    /* Stop using our current graphics pipeline */
    /* Note: This is not necessary if we only have one graphics pipeline. */
    glUseProgram(0);
}

void MainLoop(Display* display)
{
    while (!display->getGQuit())
    {
        display->Input();
        PreDraw(display);
        Draw();
        SDL_GL_SwapWindow(display->getGraphicsApplicationWindow());
    }
}

/*

Setup your geometry during the vertex specification step
@return void

*/
void VertexSpecification()
{
    /*
    
    Geometry data
    here we are going to store x, y and z position attributes within vertexPositions
    for the data.
    
    For now, this information is just stored in the CPU, and we are going to store
    this data on the GPU shortly, in a call to glBufferData which will store
    the information into a vertex buffer object.

    Note: That I have segregated the data from the OpenGL calls this function.

    */

    // Lives on the cpu
    const std::vector<GLfloat> vertexData {
        // 0 - Vertex
        -0.5f, -0.5f, 0.0f, // position
        1.0f, 0.0f, 0.0f, // color
        // 1 - Vertex
        0.5f, -0.5f, 0.0f, // position
        0.0f, 1.0f, 0.0f, // color
        // 2 - Vertex
        -0.5f, 0.5f, 0.0f, // position
        0.0f, 0.0f, 1.0f, // color
        // 3 - Vertex
        0.5f, 0.5f, 0.0f, // position
        1.0f, 0.0f, 0.0f, // color
    };

    /*
    
    Vertex Array Object (VAO) Setup
    Note: We can think of the VAO as a 'wrapper around' all of the vertex buffer
    objects, in the sense that it encapsulates all vbo state that we are setting up
    Thus, it is also important that we glBindVertexArray (i. e. select the vao we
    want to use) before our vertex buffer object operations.

    We bind (i.e. select) to the vertex array object (vao) that we want to work
    with.
    
    */

/*
        We are working with gl_array_buffer or gl_element_array_buffer
        size of the data in bytes
        raw array of data
        how we intend to use the data


        For our given vertex array object, we need to tell OpenGL
        how the information our buffer will be used.

        for the specific attribute in our vertex specification, we use
        glVertexAttribPointer to figure out how we are going to move through
        the data

        attribute 0 corresponds to the enabled glEnableVertexAttribArray

        In the future, you will see in our vertex shader this also correspond to (layout=0)
        which selects these attributes

        the number of components (e.g. x, y, z = 3 components)
    */

   /*
    
    Vertex Buffer Object (VBO) creation
    Create a new vertex buffer object
    Note: We'll see this pattern of code often in OpenGL of creating and
    binding to a buffer.

    Next we will do glBindBuffer
    Bind is equivalent to 'selecting the active buffer object' that we want to
    work with in OpenGL.

    Now, in our currently binded buffer, we populate the data from our
    'vertexPositions' which is on the CPU, onto a buffer that will store
    on the GPU.

    */
    // Start generating our VBO

    // We start setting things up on the GPU
    glGenVertexArrays(1, &gVertexArrayObject);
    glBindVertexArray(gVertexArrayObject);

    /* Vertex coords buffer */
    glGenBuffers(1, &gVertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, gVertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(GLfloat), 
                vertexData.data(), 
                GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0, // layout=0
        3, // number of components
        GL_FLOAT, // type
        GL_FALSE, // is the data normalized
        sizeof(GL_FLOAT) * 6, // Stride (how to get the next component)
        (GLvoid*)0 // Offset
    );

    /* Setup the index buffer object (IBO) or EBO(Element Array Object Buffer)  */
    const std::vector<GLuint> indexBufferData {
        2, 0, 1, 3, 2, 1
    };

    glGenBuffers(1, &gIndexBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIndexBufferObject);
    /* Populate our Index Buffer */
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        indexBufferData.size() * sizeof(GLuint),
        indexBufferData.data(),
        GL_STATIC_DRAW
    );

    /*Color information*/
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(GL_FLOAT) * 6,
        (void*) (sizeof(GL_FLOAT) * 3)
    );
    
    glBindVertexArray(0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

/*
    CompileShader will compile any valid vertex, fragment, geometry, tesselation or
    compute shader.

    @param type We use the 'type' field to determine which shader we
    are going to compile.

    @param source: The shader source code.
    @return id of the shaderObject.
*/
GLuint CompileShader(GLuint type, const std::string& source)
{
    GLuint shaderObject;

    if (type == GL_VERTEX_SHADER)
    {
        shaderObject = glCreateShader(GL_VERTEX_SHADER);
    } else if (type == GL_FRAGMENT_SHADER)
    {
        shaderObject = glCreateShader(GL_FRAGMENT_SHADER);
    }

    const char* src = source.c_str();
    glShaderSource(shaderObject, 1, &src, nullptr);
    glCompileShader(shaderObject);

    /* Retrieve the result of our compilation */
    int result;

    /* Our goal with glGetShaderiv is to retrieve the compilation status */
    glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(shaderObject, GL_INFO_LOG_LENGTH, &length);
        char* errorMessages = new char[length];
        glGetShaderInfoLog(shaderObject, length, &length, errorMessages);

        if (type == GL_VERTEX_SHADER)
        {
            std::cout << "ERROR: GL_VERTEX_SHADER compilation failed!\n" << errorMessages << std::endl;
        }
        else if (type == GL_FRAGMENT_SHADER) 
        {
            std::cout << "ERROR: GL_FRAGMENT_SHADER compilation failed!" << errorMessages << std::endl;
        }

        /* Reclaim our memory */
        delete[] errorMessages;

        /* Delete our broken shader */
        glDeleteShader(shaderObject);

        return 0;
    }

    return shaderObject;
}

GLuint CreateShaderProgram(const std::string& vertexShaderSource, const std::string &fragmentShaderSource)
{
    GLuint programObject = glCreateProgram();

    GLuint myVertexShader = CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint myFragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    glAttachShader(programObject, myVertexShader);
    glAttachShader(programObject, myFragmentShader);
    glLinkProgram(programObject);

    /* Validate our program */
    glValidateProgram(programObject);

    glDetachShader(programObject, myVertexShader);
    glDetachShader(programObject, myFragmentShader);

    glDeleteShader(myVertexShader);
    glDeleteShader(myFragmentShader);

    return programObject;
}

void CreateGraphicsPipeline()
{
    std::string vertexShaderSource = LoadShaderAsString("./shaders/vertexShader.glsl");
    std::string fragmentShaderSource = LoadShaderAsString("./shaders/fragmentShader.glsl");

    gGraphicsPipelineShaderProgram = CreateShaderProgram(vertexShaderSource, fragmentShaderSource);
}

int main(int argc, char *argv[])
{
    // 1. setup the graphics program
    Display* display = new Display("First OpenGL", 1000, 900);

    // 2. setup our geometry
    VertexSpecification();

    // 3. Create our graphics pipeline
    // At a minimum, this means the vertex and fragment shader
    CreateGraphicsPipeline();

    // 4. Call the main application loop
    MainLoop(display);
    
    // 5. call the cleanup function when our program terminates
    display->CleanUp();

    return EXIT_SUCCESS;
}