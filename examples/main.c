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

#define WIDTH_F (640.0f)
#define HEIGHT_F (480.0f)

#define WIDTH_I ((int) WIDTH_F)
#define HEIGHT_I ((int) HEIGHT_F)

GLFWwindow* make_glfw(struct af_gl_ver* gl_ver);

struct vertex {
	float pos[3];
	float col[4];
	float uv[2];
	float norm[3];
} __attribute__((packed));

struct transform {
	float pos[3];
	float rot[3];
	float scale[3];
};

typedef float pos_t[2];

static int trans(struct transform* trans) {
	glScalef(trans->scale[0], trans->scale[1], trans->scale[2]);
	AF_GL_CHK;
	glTranslatef(trans->pos[0], trans->pos[1], trans->pos[2]);
	AF_GL_CHK;
	glRotatef(trans->rot[0], 1.0f, 0.0f, 0.0f);
	AF_GL_CHK;
	glRotatef(trans->rot[1], 0.0f, 1.0f, 0.0f);
	AF_GL_CHK;
	glRotatef(trans->rot[2], 0.0f, 0.0f, 1.0f);
	AF_GL_CHK;

	return AF_ERR_NONE;
}

static int draw(
		struct af_ctx* ctx, struct af_drawlist* drawlist,
		struct transform* t) {

	glMatrixMode(GL_MODELVIEW);
	AF_GL_CHK;
		glLoadIdentity();
		AF_GL_CHK;
		AF_CHK(trans(t));

	AF_CHK(af_draw(ctx, drawlist));

	return AF_ERR_NONE;
}

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

	pos_t mouse = { 0 };

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

	struct transform cam = { 0 };

	drawops[0].data.settex.tex = &tex;
	drawops[0].data.settex.width = 64;

	drawops[1].data.drawbuf.vert = &vert;
	drawops[1].data.drawbuf.buf = &buf;
	drawops[1].data.drawbuf.primitive = AF_TRIANGLE_FAN;

	cam.scale[0] = 1.0f;
	cam.scale[1] = 1.0f;
	cam.scale[2] = 1.0f;

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

	AF_CHK(af_setview(&ctx, WIDTH_I, HEIGHT_I));

	glEnable(GL_DEPTH_TEST);

	while(!glfwWindowShouldClose(window)) {
		pos_t mouse_delta;

		{
			double x, y;
			glfwGetCursorPos(window, &x, &y);

			mouse_delta[0] = ((float) x) - mouse[0];
			mouse_delta[1] = ((float) y) - mouse[1];

			mouse[0] = (float) x;
			mouse[1] = (float) y;
		}

		cam.rot[1] += mouse_delta[0];
		cam.rot[0] += mouse_delta[1];

		glMatrixMode(GL_PROJECTION);
		AF_GL_CHK;
			glLoadIdentity();
			AF_GL_CHK;
			gluPerspective(60.0f, WIDTH_F / HEIGHT_F, 0.01, 100.0);
			AF_GL_CHK;
			AF_CHK(trans(&cam));

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		AF_GL_CHK;

		{
			struct transform t = {
				{  0.0f, -2.0f,  0.0f },
				{ 90.0f,  0.0f,  0.0f },
				{ 15.0f,  1.0f, 15.0f }
			};
			AF_CHK(draw(&ctx, &drawlist, &t));
		}

		{
			struct transform t = {
				{ -2.0f,  0.0f, -5.0f },
				{  0.0f,  0.0f,  0.0f },
				{  2.0f,  2.0f,  2.0f }
			};
			AF_CHK(draw(&ctx, &drawlist, &t));
		}

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	AF_CHK(af_killdrawlist(&ctx, &drawlist));
	AF_CHK(af_killbuf(&ctx, &tex));
	AF_CHK(af_killvert(&ctx, &vert));
	AF_CHK(af_killbuf(&ctx, &buf));
	AF_CHK(af_killctx(&ctx));
}

GLFWwindow* make_glfw(struct af_gl_ver* ver) {
	GLFWwindow* window;

	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, (int) ver->major);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, (int) ver->minor);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	window = glfwCreateWindow(WIDTH_I, HEIGHT_I, "Afeirsa Test", NULL, NULL);
	glfwMakeContextCurrent(window);

	return window;
}
