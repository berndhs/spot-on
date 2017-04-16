purge.commands = rm -f */*~ *~

CONFIG		+=	ordered
QMAKE_EXTRA_TARGETS = purge
QT += concurrent
SUBDIRS		=	spot-on-gui.pro \
			Kernel/spot-on-kernel.pro
TEMPLATE	=	subdirs
