/*
 * SPDX-License-Identifier: LGPL-3.0-or-later
 * Copyright (C) 2023 Emily "TTG" Banerjee <prs.ttg+afeirsa@pm.me>
 */

#include <afeirsa/afeirsa.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

static void echk(const char* proc, enum af_err e) {
	const char* n;
	switch(e) {
		default:
			AF_FALLTHROUGH;
			/* FALLTHRU */
		case AF_ERR_NONE: return;

		case AF_ERR_UNKNOWN: n = "unknown"; break;
   		case AF_ERR_BAD_PARAM: n = "bad parameter"; break;
		case AF_ERR_BAD_CTX: n = "bad context"; break;
		case AF_ERR_BAD_OP: n = "bad operation"; break;
		case AF_ERR_NO_GL: n = "no opengl"; break;
		case AF_ERR_MEM: n = "out of memory"; break;
	}

	fprintf(stderr, "%s: %s\n", proc, n);
	abort();
}

GLFWwindow* mkglfw(void);
void killglfw(GLFWwindow* window);
static void glfwchk(const char* proc);

struct vertex {
	float pos[3];
	float col[4];
	float uv[2];
	float norm[3];
};

static af_uchar_t pattern(void) {
	static af_uchar_t state = 1;
	static af_uchar_t count = 0;

	af_uchar_t res = state;
	{
		af_uchar_t p = (res & 3);
		res *= state % (p ? p : 17);

		p += state;
		{
			af_uchar_t q = 41;
			res -= (p % q);
			res += (p + q);
		}
		res += !(count % p) * p;
	}
	state += res;

	count++;
	return res;
}

int main(void) {
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

	struct af_param modelview = { AF_PARAM_MODELVIEW, AF_FLOAT, 16 };
	struct af_param projection = { AF_PARAM_PROJECTION, AF_FLOAT, 16 };

	struct af_ctx ctx;
	GLFWwindow* window = mkglfw();

	struct af_drawlist drawlist;
	struct af_buf buf;
	struct af_vert vert;
	struct af_buf tex;

	af_uchar_t texdata[64 * 64 * 4];
	af_size_t i;

	float clear[] = { 0.0f, 0.0f, 0.0f, 1.0f };

	struct af_drawop drawops[2];

	drawops[0].type = AF_SETTEX;
	drawops[0].data.settex = &tex;

	drawops[1].type = AF_DRAWBUF;
	drawops[1].data.drawbuf.vert = &vert;
	drawops[1].data.drawbuf.buf = &buf;
	drawops[1].data.drawbuf.primitive = AF_TRIANGLE_FAN;

	for(i = 0; i < AF_ARRLEN(texdata); ++i) {
		texdata[i] = pattern();
	}

	echk("af_mkctx", af_mkctx(&ctx, AF_FIDELITY_FAST));

	echk(
		"af_mkvert",
		af_mkvert(&ctx, &vert, vert_elements, AF_ARRLEN(vert_elements)));

	echk("af_mkbuf", af_mkbuf(&ctx, &buf, AF_BUF_VERT));
	echk("af_upload", af_upload(&ctx, &buf, vertices, sizeof(vertices)));

	echk("af_mkbuf", af_mkbuf(&ctx, &tex, AF_BUF_TEX));
	tex.tex_width = 64;
	echk("af_upload", af_upload(&ctx, &tex, texdata, sizeof(texdata)));

	echk(
		"af_mkdrawlist",
		af_mkdrawlist(&ctx, &drawlist, drawops, AF_ARRLEN(drawops)));

	while(!glfwWindowShouldClose(window)) {
		float proj[] = {
			1.5f, 0.0f,  0.0f,  0.0f,
			0.0f, 2.5f,  0.0f,  0.0f,
			0.0f, 0.0f, -1.0f, -1.0f,
			0.0f, 0.0f, -0.5f,  0.0f
		};
		echk(
			"af_setparam",
			af_setparam(&ctx, "u_projection", &projection, proj));

		echk("af_clear", af_clear(&ctx, clear));

		{
			float mv[] = {
				15.0f,  0.0f,  0.0f, 0.0f,
				 0.0f,  0.0f, 15.0f, 0.0f,
				 0.0f, -1.0f,  0.0f, 0.0f,
				 0.0f, -2.0f,  0.0f, 1.0f
			};
			echk(
				"af_setparam",
				af_setparam(&ctx, "u_model_view", &modelview, mv));
			echk("af_draw", af_draw(&ctx, &drawlist));
		}

		{
			float mv[] = {
				 1.0f, 0.0f,  0.0f, 0.0f,
				 0.0f, 1.0f,  0.0f, 0.0f,
				 0.0f, 0.0f,  1.0f, 0.0f,
				-1.0f, 0.0f, -4.0f, 1.0f
			};
			float p;

			double t = glfwGetTime();
			glfwchk("glfwGetTime");

			p = (float) sin(t);
			mv[12] += p;

			echk(
				"af_setparam",
				af_setparam(&ctx, "u_model_view", &modelview, mv));
			echk("af_draw", af_draw(&ctx, &drawlist));
		}

		echk("af_drawbuf", af_drawbuf(&ctx, &buf, &vert, AF_TRIANGLE_FAN));

		glfwPollEvents();
		glfwchk("glfwPollEvents");

		glfwSwapBuffers(window);
		glfwchk("glfwSwapBuffers");
	}

	echk("af_killdrawlist", af_killdrawlist(&ctx, &drawlist));
	echk("af_killbuf", af_killbuf(&ctx, &tex));
	echk("af_killvert", af_killvert(&ctx, &vert));
	echk("af_killbuf", af_killbuf(&ctx, &buf));
	echk("af_killctx", af_killctx(&ctx));

	killglfw(window);

	return 0;
}

void glfwchk(const char* proc) {
	const char* n;
	if(!glfwGetError(&n)) return;
	fprintf(stderr, "%s: %s\n", proc, n);
	abort();
}

GLFWwindow* mkglfw(void) {
	GLFWwindow* window;

	glfwInit();
	glfwchk("glfwInit");

	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	glfwchk("glfwWindowHint");

	window = glfwCreateWindow(640, 480, "Afeirsa Test", NULL, NULL);
	glfwchk("glfwCreateWindow");

	glfwMakeContextCurrent(window);
	glfwchk("glfwMakeContextCurrent");

	return window;
}

void killglfw(GLFWwindow* window) {
	glfwDestroyWindow(window);
	glfwchk("glfwDestroyWindow");

	glfwTerminate();
	glfwchk("glfwTerminate");
}
