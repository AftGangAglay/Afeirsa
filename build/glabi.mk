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
	GLABI_CFLAGS += -DAF_GLXABI
	GLABI_LDLIBS += -lGL -lGLU
	ifdef WANT_GLUT
		GLABI_LDLIBS += -lglut
	endif

	ifdef APPLE
		GLABI_LDLIBS += -L$(XQUARTZ_ROOT)/lib
		GLABI_IFLAGS += -I$(XQUARTZ_ROOT)/include
	endif
else
	ifdef APPLE
		GLABI_CFLAGS += -DAF_NSGL
		GLABI_LDLIBS += -framework OpenGL
		ifdef WANT_GLUT
			GLABI_LDLIBS += -framework GLUT
		endif
	endif
	ifdef WINDOWS
		GLABI_CFLAGS += -DAF_WGL
		GLABI_LDLIBS += -lopengl32 -lglu32
	endif
endif
