lessThan(QT_VERSION, 5.2) {
	error("Peg-E requires Qt 5.2 or greater")
}

TEMPLATE = app
QT += widgets
CONFIG += warn_on c++11

# Allow in-tree builds
!win32 {
	MOC_DIR = build
	OBJECTS_DIR = build
	RCC_DIR = build
}

# Set program version
VERSION = 1.1.2
DEFINES += VERSIONSTR=\\\"$${VERSION}\\\"

# Set program name
unix: !macx {
	TARGET = peg-e
} else {
	TARGET = Peg-E
}

# Specify program sources
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

# Allow for updating translations
TRANSLATIONS = $$files(translations/pege_*.ts)

# Install program data
macx {
	ICON = icons/peg-e.icns

	ICONS.files = icons/oxygen/hicolor
	ICONS.path = Contents/Resources/icons

	QMAKE_BUNDLE_DATA += ICONS
} else:win32 {
	RC_FILE = icons/icon.rc
} else:unix {
	RESOURCES = icons/icon.qrc

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

	appdata.files = icons/peg-e.appdata.xml
	appdata.path = $$PREFIX/share/appdata/

	qm.files = translations/*.qm
	qm.path = $$PREFIX/share/peg-e/translations

	man.files = doc/peg-e.6
	man.path = $$PREFIX/share/man/man6

	INSTALLS += target icon pixmap desktop appdata icons qm man
}
