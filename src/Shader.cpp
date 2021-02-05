#include <GL/glew.h>  /* must be the 1st #include BEFORE ANY other #includes */
#include <GLFW/glfw3.h>

#include "Shader.h"

/*** Vid#15: (10:00) Added required include libraries ***/
#include <iostream>
#include <fstream> 
#include <string>
#include <sstream>
#include "Renderer.h"

/* Shader constructor */
Shader::Shader(const std::string& filepath)
    /* m_FilePath(filepath) is used for debugging purposes */
	: m_FilePath(filepath), m_RendererID(0)
{
    /* Vid#15: (12:15) Moved to Shader.cpp constructor */
    ShaderProgramSource source = ParseShader(filepath);

    /* test print VertexShader and FragmentShader to console */
    //std::cout << "VERTEX" << std::endl;
    //std::cout << source.VertexSource << std::endl;
    //std::cout << "FRAGMENT" << std::endl;
    //std::cout << source.FragmentSource << std::endl;

    /* Vid#15 (12:30) Call to create vertexShader and fragmentShader above now using m_RendererID  */
    m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);

}

/* Shader destructor */
Shader::~Shader()
{
    /* Vid#15 (12:30) destroys */
    GLCall(glDeleteProgram(m_RendererID));
}

void Shader::Bind() const
{
    /* Vid#15 (13:00) Shader Bind method */
    GLCall(glUseProgram(m_RendererID));
    //glUseProgram(m_RendererID);

}

void Shader::Unbind() const
{
    /* Vid#15 (13:00) Shader Unind method */
    GLCall(glUseProgram(0));
}

/* Vid#17: (21:45) Add Uniform1f definition with string& name, int value */
void Shader::SetUniform1i(const std::string& name, int value)
{
    /* Vid#17: (21:45) pass in location and (1) integer value of the Uniform */
    GLCall(glUniform1i(GetUniformLocation(name), value));
}


/* Vid#15: (20:15) Add Uniform1f definition with string& name, float value */
void Shader::SetUniform1f(const std::string& name, float value)
{
    /* Vid#15: (20:20) pass in location and (1) float value of the Uniform */
    GLCall(glUniform1f(GetUniformLocation(name), value));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    /* Vid#15: (13:15) pass in location and (4) float values of the Uniform */
    GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
    //glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);

}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
    /* glUniformMatrix4fv
        1st param:  Get Uniform Location (name),
        2nd param:  count or # matrices,
        3rd param:  Transpose = GL_FALSE b/c we are using glm 
        4th param:  pointer to the float array or memory address of matrix[col0][element0] */
    
    GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

/* Vid#17: (20:00) fix the Shader caching GetUniformLocation FCN by
    changing unsigned int to int for testing location == -1 */
int Shader::GetUniformLocation(const std::string& name)
{
    /* Vid#15: (18:00) Check if UniformLocationCache contains the name
        and if it does, return the location, else GetUniformLocation() */
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
        return m_UniformLocationCache[name];

    /* Vid#15: (13:45) should be int location like in Application.cpp */
    GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));

    /* test for valid (-1) for Shader location when a Uniform is declared but not used yet */
    if (location == -1)
        std::cout << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;

    /* cache the location improves the performance with multiple uniforms */
    m_UniformLocationCache[name] = location;

    /* return location */
    return location;
}

/*** Vid#7: create static int CompileShader function with parameters:
    unsigned int type (used raw C++ type instead of OpenGL GLuint type to allow other non-OpenGL GPU driver implementations),
    const std::string& source
    returns a static unsigned int, takes in a type and a string ptr reference to a source ***/

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
    /* change GL_VERTEX_SHADER to type */
    unsigned int id = glCreateShader(type);

    /* returns a char ptr* src to a raw string (the beginning of our data)
        assigned to source which needs to exist before this code is executed
        pointer to  beginning of our data */
    const char* src = source.c_str();

    /* specify glShaderSource(Shader ID, source code count, ptr* to memory address of ptr*, length)
        as the source of our Shader */
    glShaderSource(id, 1, &src, nullptr);

    /* specify glCompileShader(Shader ID), then return the Shader ID */
    glCompileShader(id);

    /*error handling - query void glGetShaderiv(GLuint shader, GLenum pname, GLint *params);
        i - specifies an integer
        v - specifies a vector (array) */
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE)
    {
        /* query message - length and contents
           void glGetShaderiv(GLuint shader, GLenum pname, GLint *params); */
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

        /* construct char message[length] array allocated on the stack */
        char* message = (char*)alloca(length * sizeof(char));

        /* glGetShaderInfoLog — Returns the information log for a shader object
            void glGetShaderInfoLog(GLuint shader, GLsizei maxLength, GLsizei *length, GLchar *infoLog); */
        glGetShaderInfoLog(id, length, &length, message);

        /* print the message to the console using std::cout */
        std::cout << "Failed to Compile "
            << (type == GL_VERTEX_SHADER ? "vertex shader" : "fragment shader")
            << std::endl;
        std::cout << message << std::endl;

        /* delete Shader using id and return error code = 0 */
        glDeleteShader(id);
        return 0;
    }
    return id;
}

/*** Vid#7: create static int CreateShader function with parameters:
    const string pointer vertexShader(actual source code),
    const string pointer fragmentShader (actual source code)
    returns a static int, takes in the actual source code of these two Shader strings ***/
/* Vid#15: (9:20) Moved CreateShader from Application.cpp to Shader.cpp, then Refactored */

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    /* glCreateProgram() return an unsigned int program */
    GLCall(unsigned int program = glCreateProgram());

    /* create vertexShader object */
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);

    /* create fragmentShader object */
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    /* attach vs and fs Shader files, link and validate them to our program ID
       void glAttachShader(GLuint program, GLuint shader); */
    glAttachShader(program, vs);
    glAttachShader(program, fs);

    /* void glLinkProgram(GLuint program); */
    glLinkProgram(program);

    /* void glValidateProgram(	GLuint program); */
    glValidateProgram(program);

    /* finally, delete the intermediary *.obj files (objects vs and fs) of program ID
        and return an unsigned int program
        void glDeleteShader(GLuint shader); */
    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

/*** Vid#8 Add new function ParseShader to parse external Basic.shader file
    returns - struct ShaderProgramSource above which contains two strings (variables)
    note: C++ functions are normally capable of only returning one variable ***/

ShaderProgramSource Shader::ParseShader(const std::string& filepath)
{
    /*** Vid#15: (10:20) added - should have been there in Vid#14 ***/
    std::ifstream stream(filepath);

    /* create enum class for each Shader type */
    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    /* define buffers for 2 Shaders:  vertexShader and fragmentShader */
    std::stringstream ss[2];

    /* set initial ShaderType = NONE */
    ShaderType type = ShaderType::NONE;

    /* parse file line by line */
    std::string line;
    while (getline(stream, line))
    {
        /* find "#shader" keyword */
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                /* set mode to vertex */
                type = ShaderType::VERTEX;

            else if (line.find("fragment") != std::string::npos)
                /* set mode to fragment */
                type = ShaderType::FRAGMENT;
        }
        else if (type != ShaderType::NONE)
            /* add each line to the corresponding buffer after detecting the ShaderType */
        {
            /* type is an index to push data into the selected array buffer, casted to a Shader int type,
                to add each new line plus newline char */

            ss[(int)type] << line << '\n';
        }
        else
        {
            /* Got non-introductory line out of sequence! Don't know what type to use! Consider asserting,
                or throwing an exception, or something, depending on how defensive you
                want to be with respect to the input file format. */
        }
    }

    /* returns a struct comprised of two ss strings */
    return { ss[0].str(), ss[1].str() };
}
