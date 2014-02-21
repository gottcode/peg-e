lessThan(QT_VERSION, 4.6) {
	error("Peg-E requires Qt 4.6 or greater")
}

TEMPLATE = app
greaterThan(QT_MAJOR_VERSION, 4) {
	QT += widgets
}
CONFIG += warn_on
macx {
	CONFIG += x86_64
}

MOC_DIR = build
OBJECTS_DIR = build
RCC_DIR = build

VERSION = 1.1.2
DEFINES += VERSIONSTR=\\\"$${VERSION}\\\"

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
	translations/pege_fr.ts \
	translations/pege_he.ts \
	translations/pege_nl.ts \
	translations/pege_ro.ts

RESOURCES = icons/icon.qrc
macx {
	ICON = icons/peg-e.icns

	ICONS.files = icons/oxygen/hicolor
	ICONS.path = Contents/Resources/icons

	QMAKE_BUNDLE_DATA += ICONS
} else:win32 {
	RC_FILE = icons/icon.rc
} else:unix {
	isEmpty(PREFIX) {
		PREFIX = /usr/local
	}
	isEmpty(BINDIR) {
		BINDIR = bin
	}

	target.path = $$PREFIX/$$BINDIR/

	icon.files = icons/hicolor/*
	icon.path = $$PREFIX/share/icons/hicolor

	pixmap.files = icons/peg-e.xpm
	pixmap.path = $$PREFIX/share/pixmaps

	icons.files = icons/oxygen/hicolor/*
	icons.path = $$PREFIX/share/peg-e/icons/hicolor

	desktop.files = icons/peg-e.desktop
	desktop.path = $$PREFIX/share/applications

	qm.files = translations/*.qm
	qm.path = $$PREFIX/share/peg-e/translations

	INSTALLS += target icon pixmap desktop icons qm
}
