TEMPLATE = app
CONFIG += warn_on
macx {
	CONFIG += x86_64
}

MOC_DIR = build
OBJECTS_DIR = build
RCC_DIR = build

VERSION = $$system(git rev-parse --short HEAD)
isEmpty(VERSION) {
	VERSION = 0
}
DEFINES += VERSIONSTR=\\\"git.$${VERSION}\\\"

unix: !macx {
	TARGET = peg-e
} else {
	TARGET = Peg-E
}

HEADERS = src/board.h \
	src/hole.h \
	src/locale_dialog.h \
	src/movement.h \
	src/peg.h \
	src/puzzle.h \
	src/window.h

SOURCES = src/board.cpp \
	src/hole.cpp \
	src/locale_dialog.cpp \
	src/main.cpp \
	src/movement.cpp \
	src/peg.cpp \
	src/puzzle.cpp \
	src/window.cpp

TRANSLATIONS = translations/pege_en.ts \
	translations/pege_he_IL.ts

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
	isEmpty(BINDIR) {
		BINDIR = bin
	}

	target.path = $$PREFIX/$$BINDIR/

	icon.path = $$PREFIX/share/icons/hicolor/48x48/apps
	icon.files = icons/peg-e.png

	desktop.path = $$PREFIX/share/applications/
	desktop.files = icons/peg-e.desktop

	qm.files = translations/*.qm
	qm.path = $$PREFIX/share/peg-e/translations

	INSTALLS += target icon desktop qm
}
