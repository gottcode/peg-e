TEMPLATE = app
VERSION = 1.1.0
CONFIG += warn_on release
macx {
	# Uncomment the following line to compile on PowerPC Macs
	# QMAKE_MAC_SDK = /Developer/SDKs/MacOSX10.4u.sdk
	CONFIG += x86 ppc
}

MOC_DIR = build
OBJECTS_DIR = build
RCC_DIR = build

unix: !macx {
	TARGET = peg-e
} else {
	TARGET = Peg-E
}

HEADERS = src/board.h \
	src/hole.h \
	src/movement.h \
	src/peg.h \
	src/puzzle.h \
	src/window.h

SOURCES = src/board.cpp \
	src/hole.cpp \
	src/main.cpp \
	src/movement.cpp \
	src/peg.cpp \
	src/puzzle.cpp \
	src/window.cpp

RESOURCES = icons/icon.qrc
macx {
	ICON = icons/peg-e.icns
} else:win32 {
	RC_FILE = icons/icon.rc
}

unix: !macx {
	isEmpty(PREFIX) {
		PREFIX = /usr/local
	}

	target.path = $$PREFIX/bin/

	icon.path = $$PREFIX/share/icons/hicolor/48x48/apps
	icon.files = icons/peg-e.png

	desktop.path = $$PREFIX/share/applications/
	desktop.files = icons/peg-e.desktop

	INSTALLS += target icon desktop
}
