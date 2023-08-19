/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright (C) 2023 Emily "TTG" Banerjee <prs.ttg+afeirsa@pm.me>
 */

#include <afeirsa/afeirsa.h>

#include <afeirsa/afgl.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#undef AF_CHK
#define AF_CHK(c) \
	do { \
		if(c) { \
			fprintf(stderr, "%i\n", c); \
			abort(); \
		} \
	} while(0)

void on_glfw_error(int error_code, const char* description);
GLFWwindow* make_glfw(struct af_gl_ver* gl_ver);

struct vertex {
	float pos[3];
	float col[4];
	float uv[2];
	float norm[3];
} __attribute__((packed));

int main(void) {
	struct af_gl_ver req_ver = { 1, 0 };

	struct vertex vertices[] = {
		{
			{ -1.0f,  1.0f, 0.0f },
			{  1.0f,  0.0f, 0.0f, 0.7f },
			{  0.0f,  1.0f },
			{  0.0f,  1.0f, 0.0f }
		},
		{
			{  1.0f,  1.0f, 0.0f },
			{  0.0f,  1.0f, 0.0f, 0.7f },
			{  1.0f,  1.0f },
			{  0.0f,  1.0f, 0.0f }
		},
		{
			{  1.0f, -1.0f, 0.0f },
			{  1.0f,  1.0f, 0.0f, 0.7f },
			{  1.0f,  0.0f },
			{  0.0f,  1.0f, 0.0f }
		},
		{
			{ -1.0f, -1.0f, 0.0f },
			{  0.0f,  0.0f, 1.0f, 0.7f },
			{  0.0f,  0.0f },
			{  0.0f,  1.0f, 0.0f }
		}
	};

	struct af_vert_element vert_elements[] = {
		{ AF_MEMBSIZE(struct vertex, pos ), AF_VERT_POS  },
		{ AF_MEMBSIZE(struct vertex, col ), AF_VERT_COL  },
		{ AF_MEMBSIZE(struct vertex, uv  ), AF_VERT_UV   },
		{ AF_MEMBSIZE(struct vertex, norm), AF_VERT_NORM },
	};

	struct af_ctx ctx;
	GLFWwindow* window = make_glfw(&req_ver);

	struct af_drawlist drawlist;
	struct af_buf buf;
	struct af_vert vert;
	struct af_buf tex;

	af_uchar_t texdata[64 * 64 * 4];
	af_size_t i;

	struct af_drawop drawops[] = {
		{ AF_SETTEX, { 0 } },
		{ AF_DRAWBUF, { 0 } }
	};
	drawops[0].data.settex.tex = &tex;
	drawops[0].data.settex.width = 64;

	drawops[1].data.drawbuf.vert = &vert;
	drawops[1].data.drawbuf.buf = &buf;
	drawops[1].data.drawbuf.primitive = AF_TRIANGLE_FAN;

	srand(time(0));

	for(i = 0; i < AF_ARRLEN(texdata); ++i) {
		texdata[i] = rand();
	}

	AF_CHK(af_mkctx(&ctx, AF_FIDELITY_FAST));
	printf("%u.%u\n", ctx.gl_ver.major, ctx.gl_ver.minor);

	AF_CHK(af_mkvert(&ctx, &vert, vert_elements, AF_ARRLEN(vert_elements)));

	AF_CHK(af_mkbuf(&ctx, &buf, AF_BUF_VERTEX));
	AF_CHK(af_upload(&ctx, &buf, vertices, sizeof(vertices)));

	AF_CHK(af_mkbuf(&ctx, &tex, AF_BUF_TEXTURE));
	AF_CHK(af_upload(&ctx, &tex, texdata, sizeof(texdata)));

	AF_CHK(af_mkdrawlist(&ctx, &drawlist, drawops, AF_ARRLEN(drawops)));

	AF_CHK(af_setview(&ctx, 640, 480));

	glMatrixMode(GL_PROJECTION);
	AF_GL_CHK;
		glLoadIdentity();
		AF_GL_CHK;
		gluPerspective(60.0f, 3.0 / 2.0, 0.01, 100.0);
		AF_GL_CHK;

	/*
	glEnable(GL_LIGHTING);
	AF_GL_CHK;
	{
		float pos[] = { 0.0f, -1.0f, 0.0f, 1.0f };
		glLightfv(GL_LIGHT0, GL_POSITION, pos);
		glEnable(GL_LIGHT0);
	}
	*/

	while(!glfwWindowShouldClose(window)) {
		float y_rot = 0.0f;
		float z_trans = 0.0f;
		if(glfwGetKey(window, GLFW_KEY_D)) y_rot =  0.5f;
		if(glfwGetKey(window, GLFW_KEY_A)) y_rot = -0.5f;
		if(glfwGetKey(window, GLFW_KEY_W)) z_trans =  0.5f;
		if(glfwGetKey(window, GLFW_KEY_S)) z_trans = -0.5f;
		glMatrixMode(GL_PROJECTION);
		AF_GL_CHK;
			glRotatef(y_rot, 0.0f, 1.0f, 0.0f);
			AF_GL_CHK;
			glTranslatef(0.0f, 0.0f, z_trans);
			AF_GL_CHK;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		AF_GL_CHK;

		glMatrixMode(GL_MODELVIEW);
		AF_GL_CHK;
			glLoadIdentity();
			AF_GL_CHK;
			glScalef(15.0f, 1.0f, 15.0f);
			AF_GL_CHK;
			glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
			AF_GL_CHK;
			glTranslatef(0.0f, 0.0f, 2.0f);
			AF_GL_CHK;

		AF_CHK(af_draw(&ctx, &drawlist));

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	AF_CHK(af_killdrawlist(&ctx, &drawlist));
	AF_CHK(af_killbuf(&ctx, &tex));
	AF_CHK(af_killvert(&ctx, &vert));
	AF_CHK(af_killbuf(&ctx, &buf));
	AF_CHK(af_killctx(&ctx));
}

void on_glfw_error(int error_code, const char* description) {
	fprintf(stderr, "glfw error %i: %s\n", error_code, description);
}

GLFWwindow* make_glfw(struct af_gl_ver* ver) {
	GLFWwindow* window;

	glfwSetErrorCallback(on_glfw_error);
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, (int) ver->major);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, (int) ver->minor);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	window = glfwCreateWindow(640, 480, "Afeirsa Test", NULL, NULL);
	glfwMakeContextCurrent(window);

	return window;
}
