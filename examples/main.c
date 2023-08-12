#include <afeirsa/afeirsa.h>

#define GL_SILENCE_DEPRECATION
#include <OpenGL/GL.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <assert.h>

#define AF_CHK(c) assert(c == AF_ERR_NONE)

void on_glfw_error(int error_code, const char* description);
GLFWwindow* make_glfw(struct af_gl_ver*);

struct vertex {
    float pos[2];
} __attribute__((packed));

int main(void) {
	struct af_gl_ver gl_ver = { 1, 0 };

    struct vertex vertices[] = {
            {  0.0f, 0.5f },
            { -0.5f, 0.0f },
            {  0.5f, 0.0f }
    };

    GLFWwindow* window = make_glfw(&gl_ver);

    struct af_ctx ctx;
    struct af_buf vbuf;

    AF_CHK(af_mkctx(&ctx, &gl_ver));
    AF_CHK(af_mkbuf(&ctx, &vbuf, AF_BUF_VERTEX));

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	while(!glfwWindowShouldClose(window)) {
        af_size_t i;

		glClear(GL_COLOR_BUFFER_BIT);

		glBegin(GL_TRIANGLES);
			glColor3f(1.0f, 0.0f, 0.0f);
			for(i = 0; i < AF_ARRLEN(vertices); ++i)
				glVertex2f(vertices[i].pos[0], vertices[i].pos[1]);
		glEnd();
		
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

    AF_CHK(af_killbuf(&ctx, &vbuf));
    AF_CHK(af_killctx(&ctx));
}

void on_glfw_error(int error_code, const char* description) {
        fprintf(stderr, "glfw error %i: %s\n", error_code, description);
}

GLFWwindow* make_glfw(struct af_gl_ver* ver) {
    GLFWwindow* window;

    glfwSetErrorCallback(on_glfw_error);
    glfwInit();

    /*
    glfwWindowHint(GLFW_CONTEXT_ROBUSTNESS, GLFW_LOSE_CONTEXT_ON_RESET);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
    */

    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, ver->major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, ver->minor);

    window = glfwCreateWindow(640, 480, "Afeirsa Test", NULL, NULL);
	glfwMakeContextCurrent(window);

	return window;
}
