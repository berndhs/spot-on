cache()
purge.commands = del /F *\\*~ && del /F *~

CONFIG		+=	ordered
QMAKE_EXTRA_TARGETS = purge
SUBDIRS		=	goldbug-gui.win.qt5.pro \
			Kernel\\spot-on-kernel.win.qt5.pro
TEMPLATE	=	subdirs
