#include <cmath>
#include <climits>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 800

#define MAX_VERTEX_BUFFER 512 * 1024
#define MAX_ELEMENT_BUFFER 128 * 1024

using namespace std;

/* ===============================================================
 *
 *                              GUI
 *
 * ===============================================================*/
/* This are some code examples to provide a small overview of what can be
 * done with this library. To try out an example uncomment the defines */

/* ===============================================================
 *
 *                          Vertices
 *
 * ===============================================================*/
static unsigned int compile_shader(unsigned int type, const string& source)
{
    unsigned int id = glCreateShader(type);
    const char * src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    // TODO: Error handling
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char *message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        cout << "Failed to compile" << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "shader!" << endl;
        cout << message << endl;
        glDeleteShader(id);

        return 0;
    }
    

    return id;
}
static unsigned int create_shader(const string& vetex_shader, const string& frag_shader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = compile_shader(GL_VERTEX_SHADER, vetex_shader);
    unsigned int fs = compile_shader(GL_FRAGMENT_SHADER, frag_shader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}


/* ===============================================================
 *
 *                      Callback Functions
 *
 * ===============================================================*/
static void error_callback(int e, const char *d)
{cerr << "Error " << e << ": " << d << endl;}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        //control_points.push_back({xpos, ypos});
    }
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        // if (control_points.size() != 0) {
        //     control_points.pop_back();
        // }
    }
}

/* ===============================================================
 *
 *                             DEMO
 *
 * ===============================================================*/
int main(void)
{
    /* Platform */
    static GLFWwindow *win;
    int width = 0, height = 0;

    /* GLFW */
    glfwSetErrorCallback(error_callback);
    if (!glfwInit()) {
        cout << "[GFLW] failed to init!" << endl;
        exit(1);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    win = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Hull Modeling Methods for Offshore Structures", NULL, NULL);
    glfwMakeContextCurrent(win);
    glfwGetWindowSize(win, &width, &height);

    /* OpenGL */
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glewExperimental = 1;
    if (glewInit() != GLEW_OK) {
        cerr << "Failed to setup [GLEW]!" << endl;
        exit(1);
    }

    /* Context */

    /* Set Fonts */

    /* Set callback functions */
    glfwSetMouseButtonCallback(win, mouse_button_callback);

    float positions[6] = {
        -0.5f, -0.5f,
        0.0f,  0.0f,
        0.5f, -0.5f
    };

    /* Set buffer */
    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GL_FLOAT), (const void*)0);

    string vetex_shader = 
        "#version 450 core\n"
        "\n"
        "layout(location = 0) in vec4 position;"
        "\n"
        "void main()\n"
        "{\n"
        "   gl_Position = position;\n"
        "}\n";
    string fragment_shader = 
        "#version 450 core\n"
        "\n"
        "layout(location = 0) out vec4 color;"
        "\n"
        "void main()\n"
        "{\n"
        "   color = vec4(1.0, 0.0, 0.0, 1.0);\n"
        "}\n";

    unsigned int shader = create_shader(vetex_shader, fragment_shader);
    glUseProgram(shader);

    cout << glGetString(GL_VERSION) << endl;
    cout << sizeof(float) << endl;
    cout << 6 * sizeof(float) << endl;
    cout << sizeof(positions) << endl;

    while (!glfwWindowShouldClose(win))
    {

        /* ------------------ GUI ------------------ */
        /* ----------------------------------------- */

        /* Draw */
        glfwGetWindowSize(win, &width, &height);
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(win);

        /* Input */
        glfwPollEvents();

    }

    glDeleteShader(shader);
    glfwTerminate();
    return 0;
}

