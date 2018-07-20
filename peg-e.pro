lessThan(QT_MAJOR_VERSION, 5) {
	error("Peg-E requires Qt 5.2 or greater")
}
equals(QT_MAJOR_VERSION, 5):lessThan(QT_MINOR_VERSION, 2) {
	error("Peg-E requires Qt 5.2 or greater")
}

TEMPLATE = app
QT += widgets
CONFIG += c++11

CONFIG(debug, debug|release) {
	CONFIG += warn_on
	DEFINES += QT_DEPRECATED_WARNINGS
	DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x051100
	DEFINES += QT_NO_NARROWING_CONVERSIONS_IN_CONNECT
}

# Allow in-tree builds
!win32 {
	MOC_DIR = build
	OBJECTS_DIR = build
	RCC_DIR = build
}

# Set program version
VERSION = 1.2.6
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

# Generate translations
TRANSLATIONS = $$files(translations/pege_*.ts)
qtPrepareTool(LRELEASE, lrelease)
updateqm.input = TRANSLATIONS
updateqm.output = ${QMAKE_FILE_PATH}/${QMAKE_FILE_BASE}.qm
updateqm.commands = $$LRELEASE -silent ${QMAKE_FILE_IN} -qm ${QMAKE_FILE_OUT}
updateqm.CONFIG += no_link target_predeps
QMAKE_EXTRA_COMPILERS += updateqm

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
	appdata.path = $$PREFIX/share/metainfo/

	qm.files = $$replace(TRANSLATIONS, .ts, .qm)
	qm.path = $$PREFIX/share/peg-e/translations
	qm.CONFIG += no_check_exist

	man.files = doc/peg-e.6
	man.path = $$PREFIX/share/man/man6

	INSTALLS += target icon pixmap desktop appdata icons qm man
}
