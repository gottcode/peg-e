/*
	SPDX-FileCopyrightText: 2009-2022 Graeme Gott <graeme@gottcode.org>

	SPDX-License-Identifier: GPL-3.0-or-later
*/

#include "locale_dialog.h"
#include "window.h"

#include <QApplication>
#include <QCommandLineParser>
#include <QFileInfo>
#include <QSettings>

/**
 * Program entry point.
 *
 * @param argc amount of command line arguments
 * @param argv command line arguments
 * @return @c 0 on successful exit
 */
int main(int argc, char** argv)
{
	QApplication app(argc, argv);
	app.setApplicationName("Peg-E");
	app.setApplicationVersion(VERSIONSTR);
	app.setApplicationDisplayName(Window::tr("Peg-E"));
	app.setOrganizationDomain("gottcode.org");
	app.setOrganizationName("GottCode");
#if !defined(Q_OS_WIN) && !defined(Q_OS_MAC)
	app.setWindowIcon(QIcon::fromTheme("peg-e", QIcon(":/peg-e.png")));
	app.setDesktopFileName("peg-e");
#endif
	app.setAttribute(Qt::AA_DontShowIconsInMenus, true);

	// Find application data
	const QString appdir = app.applicationDirPath();
	const QStringList datadirs{
#if defined(Q_OS_MAC)
		appdir + "/../Resources"
#elif defined(Q_OS_UNIX)
		DATADIR,
		appdir + "/../share/peg-e"
#else
		appdir
#endif
	};

	// Handle portability
#ifdef Q_OS_MAC
	const QFileInfo portable(appdir + "/../../../Data");
#else
	const QFileInfo portable(appdir + "/Data");
#endif
	if (portable.exists() && portable.isWritable()) {
		QSettings::setDefaultFormat(QSettings::IniFormat);
		QSettings::setPath(QSettings::IniFormat, QSettings::UserScope, portable.absoluteFilePath() + "/Settings");
	}

	// Load application language
	LocaleDialog::loadTranslator("pege_", datadirs);

	// Handle commandline
	QCommandLineParser parser;
	parser.setApplicationDescription(Window::tr("Peg elimination game"));
	parser.addHelpOption();
	parser.addVersionOption();
	parser.process(app);

	// Set location of fallback icons
	QStringList paths = QIcon::themeSearchPaths();
	for (const QString& datadir : datadirs) {
		paths.prepend(datadir + "/icons");
	}
	QIcon::setThemeSearchPaths(paths);

	// Set up icons
#if defined(Q_OS_MAC) || defined(Q_OS_WIN)
	QIcon::setThemeName("hicolor");
#else
	if (QIcon::themeName() == "hicolor") {
		QIcon::setThemeName("Hicolor");
	}
	QIcon::setFallbackThemeName("hicolor");
#endif

	// Create main window
	Window window;
	window.show();

	return app.exec();
}
