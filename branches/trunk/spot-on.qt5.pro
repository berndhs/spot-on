cache()
doxygen.commands = doxygen spot-on.doxygen
purge.commands = rm -f */*~ *~
TOP_DIR = $$PWD
message ("TOP_DIR is $$TOP_DIR")

CONFIG		+=	ordered
QMAKE_EXTRA_TARGETS = doxygen purge
SUBDIRS		=	spot-on-gui.qt5.pro \
			Kernel/spot-on-kernel.qt5.pro
TEMPLATE	=	subdirs
