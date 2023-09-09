# SPDX-License-Identifier: LGPL-3.0-or-later
# Copyright (C) 2023 Emily "TTG" Banerjee <prs.ttg+afeirsa@pm.me>

ifndef APPLE
	GLXABI = 1
endif

XQUARTZ_ROOT = /opt/X11
ifdef GLXABI
	GLABI += -DAF_GLXABI
	LDLIBS += -lGL -lglut -lGLU

	ifdef APPLE
		LDLIBS += -L$(XQUARTZ_ROOT)/lib
		PUBLIC_IFLAGS += -I$(XQUARTZ_ROOT)/include
	endif
else
	ifdef APPLE
		GLABI += -DAF_NSGL
		LDLIBS += -framework GLUT -framework OpenGL
	endif
endif
