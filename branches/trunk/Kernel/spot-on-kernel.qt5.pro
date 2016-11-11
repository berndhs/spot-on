cache()
include(spot-on-kernel-source.pro)
libntl.target = libntl.so
libntl.commands = cd ../../../libNTL/unix.d/src && ./configure && $(MAKE)
libntl.depends =
#libntru.target = libntru.so
#libntru.commands = $(MAKE) -C ../../../libNTRU
#libntru.depends =
libspoton.target = libspoton.so
libspoton.commands = $(MAKE) -C ../../../libSpotOn library
libspoton.depends = libntru libntru-devel
purge.commands = rm -f *~

TEMPLATE	= app
LANGUAGE	= C++
QT		+= bluetooth concurrent network sql
CONFIG		+= qt release warn_on

# The function gcry_kdf_derive() is available in version
# 1.5.0 of the gcrypt library.

DEFINES += SPOTON_BLUETOOTH_ENABLED \
           SPOTON_LINKED_WITH_LIBGEOIP \
           SPOTON_LINKED_WITH_LIBPTHREAD \
           SPOTON_MCELIECE_ENABLED \
           SPOTON_SCTP_ENABLED \
#           SPOTON_LINKED_WITH_LIBNTRU \

# Unfortunately, the clean target assumes too much knowledge
# about the internals of libNTL, libNTRU, and libSpotOn.

QMAKE_CLEAN     += ../Spot-On-Kernel \
		   ../../../libSpotOn/*.o \
		   ../../../libSpotOn/*.so ../../../libSpotOn/test \
                   ../../../libNTL/unix.d/src/*.o \
                   ../../../libNTL/unix.d/src/*.lo \
#		   ../../../libNTRU/*.so \
#		   ../../../libNTRU/src/*.o ../../../libNTRU/src/*.s \


QMAKE_DISTCLEAN += -r temp .qmake.cache .qmake.stash
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -fPIE -fstack-protector-all -fwrapv \
			  -lntru -L/usr/local/lib64 -L/usr/lib64 \
			  -mtune=native -pie -O3 \
			  -Wall -Wcast-align -Wcast-qual \
			  -Werror -Wextra -Wl,-z,relro \
			  -Woverloaded-virtual -Wpointer-arith \
                          -Wstack-protector -Wstrict-overflow=5

QMAKE_LFLAGS_RELEASE += -Wl,-rpath,/usr/local/spot-on/Lib
QMAKE_EXTRA_TARGETS = libntl libspoton purge
QMAKE_EXTRA_TARGETS = libntl libspoton purge
INCLUDEPATH	+= . ../. ../../../.  \
		   /usr/include/postgresql \
		   /usr/include/libntru
LIBS		+= \
		   -L../../../libSpotOn \
		   -lGeoIP -lntru \
		   -lcrypto -lcurl -lgcrypt -lgpg-error -lntl \
		   -lpq -lspoton -lssl \


# PRE_TARGETDEPS = libntl.so libntru.so libspoton.so
PRE_TARGETDEPS =  libntru.so libspoton.so
OBJECTS_DIR = temp/obj
UI_DIR = temp/ui
MOC_DIR = temp/moc
RCC_DIR = temp/rcc

TARGET		= ../Spot-On-Kernel
PROJECTNAME	= Spot-On-Kernel

# Prevent qmake from stripping everything.

QMAKE_STRIP	= echo
