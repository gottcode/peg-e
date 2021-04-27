cmake_minimum_required(VERSION 3.16.0)

# Configure project
project(peg-e VERSION 1.2.8 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui LinguistTools Widgets)
if(UNIX AND NOT APPLE)
	include(GNUInstallDirs)
endif()

add_compile_definitions(
	QT_STRICT_ITERATORS
	QT_NO_NARROWING_CONVERSIONS_IN_CONNECT
	QT_DISABLE_DEPRECATED_BEFORE=0x060000
)

# Create program
file(GLOB translations_SRCS translations/*.ts)
qt6_add_translation(translations_QM ${translations_SRCS})

qt6_add_executable(peg-e
	src/board.cpp
	src/hole.cpp
	src/locale_dialog.cpp
	src/main.cpp
	src/movement.cpp
	src/peg.cpp
	src/puzzle.cpp
	src/window.cpp
	${translations_QM}
)

target_link_libraries(peg-e PRIVATE
	Qt6::Core
	Qt6::Gui
	Qt6::Widgets
)

target_compile_definitions(peg-e PRIVATE VERSIONSTR="${PROJECT_VERSION}")

# Install
if(UNIX AND NOT APPLE)
	target_sources(peg-e PRIVATE icons/icon.qrc)

	install(TARGETS peg-e RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
	install(FILES ${translations_QM} DESTINATION ${CMAKE_INSTALL_DATADIR}/peg-e/translations)
	install(FILES doc/peg-e.6 DESTINATION ${CMAKE_INSTALL_MANDIR}/man6 COMPONENT doc)
	install(DIRECTORY icons/hicolor DESTINATION ${CMAKE_INSTALL_DATADIR}/icons)
	install(DIRECTORY icons/oxygen/hicolor DESTINATION ${CMAKE_INSTALL_DATADIR}/peg-e/icons)
	install(FILES icons/peg-e.xpm DESTINATION ${CMAKE_INSTALL_DATADIR}/pixmaps)
	install(FILES icons/peg-e.appdata.xml DESTINATION ${CMAKE_INSTALL_DATADIR}/metainfo)
	install(FILES icons/peg-e.desktop DESTINATION ${CMAKE_INSTALL_DATADIR}/applications)
elseif(WIN32)
	# Use Qt6 macro until CMake provides something
	# https://bugreports.qt.io/browse/QTBUG-87618
	set_target_properties(peg-e PROPERTIES
		WIN32_EXECUTABLE TRUE
		QT_TARGET_VERSION "${PROJECT_VERSION}"
		QT_TARGET_COMPANY_NAME "Graeme Gott"
		QT_TARGET_DESCRIPTION "Peg elimination game"
		QT_TARGET_COPYRIGHT "Copyright (C) 2021 Graeme Gott"
		QT_TARGET_PRODUCT_NAME "Peg-E"
		QT_TARGET_RC_ICONS ${CMAKE_SOURCE_DIR}/icons/peg-e.ico
	)
	_qt_internal_generate_win32_rc_file(peg-e)
endif()
