#include <cmath>
#include <climits>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_GLFW_GL4_IMPLEMENTATION
#include "nuklear.h"
#include "nuklear_glfw_gl4.h"

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 800

#define MAX_VERTEX_BUFFER 512 * 1024
#define MAX_ELEMENT_BUFFER 128 * 1024

using namespace std;

/* ===============================================================
 *
 *                          Vertices
 *
 * ===============================================================*/
#include "ss_algorithm.h"
SS::Nurbs nurbs;

/* ===============================================================
 *
 *                              GUI
 *
 * ===============================================================*/
/* This are some code examples to provide a small overview of what can be
 * done with this library. To try out an example uncomment the defines */
#define INCLUDE_GUI_FEATURES
#ifdef INCLUDE_GUI_FEATURES
    #define INCLUDE_MENU
    #define INCLUDE_CANVAS
#endif /*INCLDUE_GUI_FEATURES */

#ifdef INCLUDE_MENU
    #include "./menu.cpp"
#endif
#ifdef INCLUDE_CANVAS
    #include "./canvas.cpp"
#endif

/* ===============================================================
 *
 *                      Callback Functions
 *
 * ===============================================================*/
static void error_callback(int e, const char *d)
{cerr << "Error " << e << ": " << d << endl;}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    bool is_in_canvas = false;

    if (xpos > 420) {
        is_in_canvas = true;
    }
    
    if (is_in_canvas && button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        nurbs.control_points.push_back({xpos, ypos});
        cout << xpos << " " << ypos << endl;
    }
    if (is_in_canvas && button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        if (nurbs.control_points.size() != 0) {
            nurbs.control_points.pop_back();
        }
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
    struct nk_context *ctx;
    struct nk_colorf bg;

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
    ctx = nk_glfw3_init(win, NK_GLFW3_INSTALL_CALLBACKS, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);

    /* Set Fonts */
    {struct nk_font_atlas *atlas;
    nk_glfw3_font_stash_begin(&atlas);
    struct nk_font *font = nk_font_atlas_add_from_file(atlas, "./font/OpenSans-SemiBold.ttf", 20, 0);
    nk_glfw3_font_stash_end();
    nk_style_set_font(ctx, &font->handle);}

    /* Set callback functions */
    glfwSetMouseButtonCallback(win, mouse_button_callback);
    
    cout << glGetString(GL_VERSION) << endl;

    bg.r = 0.10f, bg.g = 0.18f, bg.b = 0.24f, bg.a = 1.0f;
    while (!glfwWindowShouldClose(win))
    {
        /* Input */
        glfwPollEvents();
        nk_glfw3_new_frame();

        /* ------------------ GUI ------------------ */
        #ifdef INCLUDE_MENU
            menu(ctx, width, height);
        #endif
        #ifdef INCLUDE_CANVAS
            canvas(ctx, width, height, nurbs);
        #endif
        /* ----------------------------------------- */

        /* Draw */
        glfwGetWindowSize(win, &width, &height);
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(bg.r, bg.g, bg.b, bg.a);

        nk_glfw3_render(NK_ANTI_ALIASING_ON);
        glfwSwapBuffers(win);
    }
    nk_glfw3_shutdown();

    glfwTerminate();
    return 0;
}

