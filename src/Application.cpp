#include <GL/glew.h>  /* must be the 1st #include BEFORE ANY other #includes */
#include <GLFW/glfw3.h>
#include <iostream>

/* Vid#8 add includes to read, parse C++ external file: Basic.shader, and add to each Shader buffer */
#include <fstream> 
#include <string>
#include <sstream>

/* Vid#13: (8:00) add include "Renderer.h" */
#include "Renderer.h"

/* Vid#13: (18:30) Add #includes for two new classes */
#include "VertexBuffer.h"
#include "IndexBuffer.h"

/* Vid#14: (23:50) Add #includes for new VertexArray class */
#include "VertexArray.h"

/* Vid#15: (15:40) Add #includes for new Shader class */
#include "Shader.h"

/* Vid#16: (10:20) #include "VertexBufferLayout.h" - removes the infinite include looping error */
#include "VertexBufferLayout.h"

/* Vid#17: (19:30) #include "Texture.h" */
#include "Texture.h"

/* Vid#19: (9:30) #include "glm/glm.hpp" and #include "glm/gtc/matrix_transform.hpp" */
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Vid#12 (8:10) OpenGL GLFW  Version 3.3 create an Open Context and Window with the Core Profile 
        GLFW_OPENGL_CORE_PROFILE 
        Note:  ONLY (GLFW_CONTEXT_VERSION_MAJOR, 2) and (GLFW_CONTEXT_VERSION_MINOR, 1) WORKS!!!
        All other combinations of ints (e.g. 2, 3) of later major/minor versions Fails 
        with the following console output msg:

        C:\Dev\Cherno\OpenGL\bin\Win32\Debug\OpenGL.exe (process 4936) exited with code -1.
        To automatically close the console when debugging stops, 
        enable Tools->Options->Debugging->Automatically close the console when debugging stops.
        Press any key to close this window . . .
     */

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    glfwWindowHint(GLFW_OPENGL_ANY_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    //glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);



/****** Create a windowed mode window and its OpenGL context 
    glfwCreateWindow MUST BE PERFORMED AFTER ANY glfwWindowHint(s) ******/

    /* Vid#20: (13:00) Change window from 640x480 to */
    //window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);

    window = glfwCreateWindow(960, 540, "Hello Projection Matrices", NULL, NULL);

    if (window==NULL)
    {
        return -1;
    }

    /*** Make the window's context current - this MUST BE PEFORMED BEFORE glewInit() !!! ***/
    glfwMakeContextCurrent(window);

    /* Vid#11 (9:00) - should sync our Swap with the monitor's refresh rate
        and produce a smooth color change transition */
    GLCall(glfwSwapInterval(1));

    /*** Vid#3: JT Added Modern OpenGL code here - MUST FOLLOW glfwMakeContextCurrent(window) ***/
    if (glewInit() != GLEW_OK)
    {
        std::cout << "glewInit() Error!" << std::endl;
    }

    /*** Vid#3: JT Added Print Modern OpenGL Version code here ***/
    std::cout << glGetString(GL_VERSION) << std::endl;
    
    /* Vid#9B: Vertex Buffer - remove 2 duplicate vertices of the 6 vertices in position[] to implement
            an Index Buffer */
    /* Create a New Scope {...} FROM Here TO BEFORE glfwTerminate(); */
    {
        /* Vid#17: (24:10) add another vec2 to each vertice which makes each vertice a vec4 */
        /*
        float positions[] = {
            -0.5f, -0.5f, 0.0f, 0.0f,       // vertex 0 bottom-left vertex
             0.5f, -0.5f, 1.0f, 0.0f,       // vertex 1 right most edge of rectangle
             0.5f,  0.5f, 1.0f, 1.0f,       // vertex 2 top-right vertex
            -0.5f,  0.5f, 0.0f, 1.0f        // vertex 3 left most edge of rectangle
        };
        */

        /* Vid#20: (14:15) hardcode test window pixel positions, 
            small white square corner bottom-left corner of window */
        /*
        float positions[] = {
            -10.5f, -10.5f, 0.0f, 0.0f,       // vertex 0 bottom-left vertex
             10.5f, -10.5f, 1.0f, 0.0f,       // vertex 1 right most edge of rectangle
             10.5f,  10.5f, 1.0f, 1.0f,       // vertex 2 top-right vertex
            -10.5f,  10.5f, 0.0f, 1.0f        // vertex 3 left most edge of rectangle
        };
        */

        /* Vid#20: (15:00) hardcode 2nd and 3rd test window pixel positions,
            white square towards corner bottom-left corner of window */
        float positions[] = {
            100.0f, 100.0f, 0.0f, 0.0f,       // vertex 0 bottom-left vertex
            200.0f, 100.0f, 1.0f, 0.0f,       // vertex 1 right most edge of rectangle
            200.0f, 200.0f, 1.0f, 1.0f,       // vertex 2 top-right vertex
            100.0f, 200.0f, 0.0f, 1.0f        // vertex 3 left most edge of rectangle
        };


        /* Vid9B: create Index Buffer using new indices[] array
            note: must be unsigned but can use char, short, int, etc. */

        unsigned int indices[] = {
            0, 1, 2,        // 1st right triangle drawn CCW
            2, 3, 0         // 2nd inverted right triangle drawn CCW
        };
     
    /* Vid#17: (29:15) Enable Blender and Setup Blender in Application.cpp that defines how OpenGL will Blend alpha pixels 
        void glBlendFunc(GLenum sfactor, GLenum dfactor);  
        1st param: sfactor - Specifies how the red, green, blue, and alpha source blending factors are computed.
            GL_SRC_ALPHA:	(As/kA, As/kA, As/kA, As/kA)
        2nd param: dfactor - Specifies how the red, green, blue, and alpha destination blending factors are computed.
            GL_ONE_MINUS_SRC_COLOR:	(1, 1, 1, 1) − (Rs/kR, Gs/kG, Bs/kB, As/kA)
        */

        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        /* Vid#14: (5:00) create VertexArray va and VertexBuffer vb AFTER creating Vertex Array Object (vao) */
        VertexArray va;

        // Vid#17 (25:05) VertexBuffer vb(positions, 4 * 4 * sizeof(float));

        VertexBuffer vb(positions, 4 * 4 * sizeof(float));
        //VertexBuffer vb(positions, sizeof(positions));

        /* Vid#14: (24:00) create VertexBufferLayout */

        VertexBufferLayout layout;
        layout.Push<float>(2);

        /* Vid#17: (24:50) add 2nd layout.Push for vec4 positions[]*/
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);

        /* Vid#13: (19:35) Delete OR Comment Out IndexBuffer creation code,
            move to the new IndexBuffer Class, and replace with: */

        IndexBuffer ib(indices, 6);

    /* Vid#19 (10:00) create 4x4 instance (proj) of an Orthographic Projection Matrix to map all
    *  of our vertice coordinates onto a 2D plane where objects that are further away DO NOT get smaller,
        2D Rendering.
    *  (as opposed to a Perspective Projection like a photograph from a camera where objects further
        away appear actually smaller, 3D Rendering)
    * 
        glm::highp_mat4 glm::ortho<float>(float left, float right, 
        float bottom, float top, float zNear, float zFar)
        the absolute magnitude of left + right : bottom + top provides a 4:3 aspect ratio
        to fix our Vertex positioning using it in our Shader.
     */
        //glm::mat4 proj = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, -1.0f, 1.0f);

        /* Vid#20: (13:30) hardcode a Projection Matrix, test window pixel positions */
        glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);

        /* Vid#21: (8:10) add a View Matrix to position (translate/move, rotate/angle) 
            our object(s) to the left effectively moving our camera to the right */
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-100, 0, 0));

        /* Vid#21: (11:55) add a Model Matrix to position (translate/move, rotate/angle) 
            our model x: +200 pixels to the right and y: +200 pixels up so the 
            ChernoLogo.png ends up closer to the center of our window screen */
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(200, 200, 0));


        /* Vid#21: (10:40) create MVP Matrix with Projection x View (No Model) only */
        //glm::mat4 mvp = proj * view;
        /* Vid#21: (12:20) create MVP Matrix with Projection x View x Model
            The MVP is actually in reverse order (PVM) for OpenGL b/c the geometry
            is in column major (instead of row major) for matrix memory multiplication */
        glm::mat4 mvp = proj * view * model;

        /* Vid#20: (16:40) create glm::vec4 Vertex Position 100x100, 0x1  */
        //glm::vec4 vp(100.0f, 100.0f, 0.0f, 1.0f);

        /* Vid#20: (17:01) create glm::vec4 result = proj * vpset breakpoint, 
            Run & Test(F5) test 4, (F11) and check result the original x=100, y=100 changed to fit in -1 to 1  
            result.x =-0.791666627 and result.y = -0.629629612*/
        //glm::vec4 result = proj * vp;




    /* Vid#15: (12:15) Moved ParseShader to Shader.cpp constructor */

        /* Vid#15: (15:50) create Shader class instance and Bind it */

        Shader shader("res/shaders/Basic.shader");
        shader.Bind();

        /* Vid#15: (16:00) call shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f) */
        shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

        /* Vid#19 (13:20) call shader matrix 4f passing in new Projection Matrix (u_MVP) 
            and glm::mat4 proj */
        //shader.SetUniformMat4f("u_MVP", proj);

        /* Vid#21: (10:50) change shader.SetUniformMat4f from proj to mvp,
            the result is the ChernoLogo.png translated(moved) 100 pixels to the left
            b/c of the glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-100, 0, 0))
            moved our camera 100 pixels to the right */
        shader.SetUniformMat4f("u_MVP", mvp);


    /* Vid#17: (19:30) Create instance of Texture Class: texture with
        the path: "res/textures/ChernoLogo.png" 
        then Bind() the texture with no param = 0 by default,
     */

        Texture texture("res/textures/ChernoLogo.png");
        texture.Bind(0);

    /* Vid#17: (22:00) Call Shader SetUniformi passing in "u_Texture", bind to slot0*/

        shader.SetUniform1i("u_Texture", 0);




    /* Vid#15: (17:00) Unbind VertexArray, the Unbind Shader, Unbind VertexBuffer , and Unbind IndexBuffer
       by setting each = 0 and re-bind all (3) inside the Rendering while loop before the glDraw cmd */

        va.Unbind();

        /* Vid#15: (16:50) delete glBindBuffers and add vb.UnBind() and ib.Unbind() */
        //GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
        //GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

        vb.Unbind(); 
        ib.Unbind();

        /* Vid#15: (16:15) changed GLCall(glUseProgram(0) to shader.Unbind() */
        shader.Unbind();

        /* Vid#16: (8:20) create instance of Renderer */
        Renderer renderer;

        /* Vid#11 (8:00) - Animate Loop: 1st define 4 float variables: r, g, b, and i */
        float r = 0.0f;             // red color float var initially set to zero
        float increment = 0.05f;    // color animation float increment var initially set to 0.05

        /* Games Render Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Vid#16: (10:30) Move glClear from Application.cpp to Renderer.h class Renderer public method
                and replace with renderer.Clear() Call */
            //GLCall(glClear(GL_COLOR_BUFFER_BIT))

            renderer.Clear();

            /* Vid#12: (4:45) Bind Shader (shader), Uniform (location), Vertex Buffer (buffer)
               and Index Buffer (ibo) BEFORE calling glDrawElements... */

            /* Vid#15: (16:20) changed GLCall(glUseProgram(shader) to shader.Bind() */
            shader.Bind();      // bind our shader
             
            /* Vid#15: (16:35) changed GLCall(glUniform4f(location, r, 0.3f, 0.8f, 1.0f))
                to hader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f)  */
            shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);    // setup uniform(s)

            /* Vid#16: (8:30) renderer.Draw() call passing in:
                VertexArray va, IndexBuffer ib, and Shader shader */

            renderer.Draw(va, ib, shader);

            /* Vid#16: (8:35) delete OR comment out glDrawElements b/c it's now inside the Renderer class */
            //GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));  // Draw Elements call

            /* Vid#11 (8:30) check if r value > 1.0f --> set increment = -0.05f
                else if r value < 0.0f --> set increment = 0.05f */

            if (r > 1.0f)
                increment = -0.05f;
            else if (r < 0.0f)
                increment = 0.05f;

            r += increment;

            /* Swap front and back buffers */
            GLCall(glfwSwapBuffers(window));

            /* Poll for and process events */
            GLCall(glfwPollEvents());
        }

        /* Vid#15: (16:45) removed delete Shader b/c when the code hits end of scope,
            it will automatically be deleted by the ~destructor of the Shader class */
        //GLCall(glDeleteProgram(shader));
    }

    /* OpenGL GLFW Terminate destroys OpenGL context BEFORE IndexBuffer ~destructor is Called
        We now DO NOT have an OpenGL Context */
    glfwTerminate();
    return 0;
}