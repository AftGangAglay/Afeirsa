# SPDX-License-Identifier: LGPL-3.0-or-later
# Copyright (C) 2023 Emily "TTG" Banerjee <prs.ttg+afeirsa@pm.me>

ifdef APPLE
else
	ifdef WINDOWS
	else
		GLXABI = 1
	endif
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
	ifdef WINDOWS
		GLABI += -DAF_WGL
		LDLIBS += -lopengl32 -lglu32
	endif
endif
