/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright (C) 2023 Emily "TTG" Banerjee <prs.ttg+afeirsa@pm.me>
 */

#include <afeirsa/afeirsa.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#define WIDTH_F (640.0f)
#define HEIGHT_F (480.0f)

#define WIDTH_I ((int) WIDTH_F)
#define HEIGHT_I ((int) HEIGHT_F)

GLFWwindow* mkglfw(void);
void killglfw(GLFWwindow* window);

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

	float mouse[2] = { 0 };

	struct af_vert_element vert_elements[] = {
		{ AF_MEMBSIZE(struct vertex, pos ), AF_VERT_POS  },
		{ AF_MEMBSIZE(struct vertex, col ), AF_VERT_NONE },
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

	struct af_drawop drawops[] = {
		{ AF_SETTEX, { 0 } },
		{ AF_DRAWBUF, { 0 } }
	};

	struct transform cam = { 0 };

	float clear[] = { 0.0f, 0.0f, 0.0f, 1.0f };

	drawops[0].data.settex = &tex;

	drawops[1].data.drawbuf.vert = &vert;
	drawops[1].data.drawbuf.buf = &buf;
	drawops[1].data.drawbuf.primitive = AF_TRIANGLE_FAN;

	cam.scale[0] = 1.0f;
	cam.scale[1] = 1.0f;
	cam.scale[2] = 1.0f;

	for(i = 0; i < AF_ARRLEN(texdata); ++i) {
		texdata[i] = pattern();
	}

	AF_CHK(af_mkctx(&ctx, AF_FIDELITY_FAST));

	AF_CHK(af_mkvert(&ctx, &vert, vert_elements, AF_ARRLEN(vert_elements)));

	AF_CHK(af_mkbuf(&ctx, &buf, AF_BUF_VERTEX));
	AF_CHK(af_upload(&ctx, &buf, vertices, sizeof(vertices)));

	AF_CHK(af_mkbuf(&ctx, &tex, AF_BUF_TEXTURE));
	tex.tex_width = 64;
	AF_CHK(af_upload(&ctx, &tex, texdata, sizeof(texdata)));

	AF_CHK(af_mkdrawlist(&ctx, &drawlist, drawops, AF_ARRLEN(drawops)));

	AF_CHK(af_setview(&ctx, WIDTH_I, HEIGHT_I));

	while(!glfwWindowShouldClose(window)) {
		float mouse_delta[2];

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

		{
			float proj[] = {
				1.609476f,     0.0f,  0.0f,  0.0f,
				     0.0f, 2.414213,  0.0f,  0.0f,
				     0.0f,     0.0f, -1.0f, -1.0f,
				     0.0f,     0.0f, -0.2f,  0.0f
			};
			AF_CHK(af_setparam(&ctx, "u_projection", &projection, proj));
		}

		AF_CHK(af_clear(&ctx, clear));

		{
			float mv[] = {
				15.0f,  0.0f,  0.0f, 0.0f,
				 0.0f,  0.0f, 15.0f, 0.0f,
				 0.0f, -1.0f,  0.0f, 0.0f,
				 0.0f, -2.0f,  0.0f, 1.0f
			};

			AF_CHK(af_setparam(&ctx, "u_model_view", &modelview, mv));
			AF_CHK(af_draw(&ctx, &drawlist));
		}

		{
			float mv[] = {
				 1.0f, 0.0f,  0.0f, 0.0f,
				 0.0f, 1.0f,  0.0f, 0.0f,
				 0.0f, 0.0f,  1.0f, 0.0f,
				-2.0f, 0.0f, -4.0f, 1.0f
			};

			AF_CHK(af_setparam(&ctx, "u_model_view", &modelview, mv));
			AF_CHK(af_draw(&ctx, &drawlist));
		}

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	AF_CHK(af_killdrawlist(&ctx, &drawlist));
	AF_CHK(af_killbuf(&ctx, &tex));
	AF_CHK(af_killvert(&ctx, &vert));
	AF_CHK(af_killbuf(&ctx, &buf));
	AF_CHK(af_killctx(&ctx));

	killglfw(window);
}

GLFWwindow* mkglfw(void) {
	GLFWwindow* window;

	glfwInit();

	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	window = glfwCreateWindow(WIDTH_I, HEIGHT_I, "Afeirsa Test", NULL, NULL);
	glfwMakeContextCurrent(window);

	return window;
}

void killglfw(GLFWwindow* window) {
	glfwDestroyWindow(window);
	glfwTerminate();
}
