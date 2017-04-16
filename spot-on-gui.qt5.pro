cache()
include(spot-on-gui-source.pro)
libntl.target = libntl.so
libntl.commands = cd ../../libNTL/unix.d/src && ./configure && $(MAKE)
libntl.depends =
libntru.target = libntru.so
libntru.commands = $(MAKE) -C ../../libNTRU
libntru.depends =
libspoton.target = libspoton.so
libspoton.commands = $(MAKE) -C ../../libSpotOn library
libspoton.depends =

TEMPLATE	= app
LANGUAGE	= C++
QT		+= bluetooth concurrent gui multimedia network printsupport \
		   sql widgets
CONFIG		+= qt release warn_on

# The function gcry_kdf_derive() is available in version
# 1.5.0 of the gcrypt library.

DEFINES	+= SPOTON_BLUETOOTH_ENABLED \
           SPOTON_LINKED_WITH_LIBGEOIP \
	   SPOTON_LINKED_WITH_LIBNTRU \
           SPOTON_LINKED_WITH_LIBPTHREAD \
           SPOTON_MCELIECE_ENABLED \
	   SPOTON_SCTP_ENABLED

# Unfortunately, the clean target assumes too much knowledge
# about the internals of libNTL, libNTRU, and libSpotOn.

QMAKE_CLEAN     += Spot-On ../../libNTL/unix.d/src/*.o \
                   ../../libNTL/unix.d/src/*.lo \
		   ../../libNTRU/*.so ../../libNTRU/src/*.o \
                   ../../libNTRU/src/*.s \
		   ../../libSpotOn/*.o ../../libSpotOn/*.so \
		   ../../libSpotOn/test
QMAKE_DISTCLEAN += -r temp .qmake.cache .qmake.stash
QMAKE_CXXFLAGS_RELEASE += -fPIE -fstack-protector-all -fwrapv \
                          -mtune=native -pie \
			  -Wall -Wcast-align -Wcast-qual \
			  -Werror -Wextra \
			  -Woverloaded-virtual -Wpointer-arith \
                          -Wstack-protector -Wstrict-overflow=5
QMAKE_LFLAGS_RELEASE += -Wl,-rpath,/usr/local/spot-on/Lib
QMAKE_EXTRA_TARGETS = libntl libntru libspoton purge
INCLUDEPATH	+= . ../../. ../../libNTL/unix.d/include \
                   GUI /usr/include/postgresql
LIBS		+= -L../../libNTL/unix.d/src/.libs \
                   -L../../libNTRU -L../../libSpotOn \
		   -lGeoIP -lcrypto -lcurl -lgcrypt \
		   -lgpg-error -lntl -lntru -lpq -lspoton -lssl
PRE_TARGETDEPS = libntl.so libntru.so libspoton.so
OBJECTS_DIR = temp/obj
UI_DIR = temp/ui
MOC_DIR = temp/moc
RCC_DIR = temp/rcc

TARGET		= Spot-On
PROJECTNAME	= Spot-On

# Prevent qmake from stripping everything.

QMAKE_STRIP	= echo
