/***********************************************************************
 *
 * Copyright (C) 2009, 2012, 2013, 2014, 2015, 2018 Graeme Gott <graeme@gottcode.org>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 ***********************************************************************/

#include "locale_dialog.h"
#include "window.h"

#include <QApplication>

/**
 * Program entry point.
 *
 * @param argc amount of command line arguments
 * @param argv command line arguments
 * @return @c 0 on successful exit
 */
int main(int argc, char** argv)
{
#if !defined(Q_OS_MAC) && (QT_VERSION >= QT_VERSION_CHECK(5,6,0))
	if (!qEnvironmentVariableIsSet("QT_DEVICE_PIXEL_RATIO")
			&& !qEnvironmentVariableIsSet("QT_AUTO_SCREEN_SCALE_FACTOR")
			&& !qEnvironmentVariableIsSet("QT_SCALE_FACTOR")
			&& !qEnvironmentVariableIsSet("QT_SCREEN_SCALE_FACTORS")) {
		QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	}
#endif
	QApplication app(argc, argv);
	app.setApplicationName("Peg-E");
	app.setApplicationVersion(VERSIONSTR);
	app.setApplicationDisplayName(Window::tr("Peg-E"));
	app.setOrganizationDomain("gottcode.org");
	app.setOrganizationName("GottCode");
#if !defined(Q_OS_WIN) && !defined(Q_OS_MAC)
	app.setWindowIcon(QIcon::fromTheme("peg-e", QIcon(":/peg-e.png")));
#endif
	app.setAttribute(Qt::AA_DontShowIconsInMenus, true);
	app.setAttribute(Qt::AA_UseHighDpiPixmaps, true);

	LocaleDialog::loadTranslator("pege_");

	// Set location of fallback icons
	{
		QString appdir = app.applicationDirPath();
		QString datadir;
#if defined(Q_OS_MAC)
		datadir = appdir + "/../Resources";
#elif defined(Q_OS_UNIX)
		datadir = appdir + "/../share/peg-e";
#else
		datadir = appdir;
#endif

		QStringList paths = QIcon::themeSearchPaths();
		paths.prepend(datadir + "/icons");
		QIcon::setThemeSearchPaths(paths);
	}

	// Set up icons
	if (QIcon::themeName().isEmpty()) {
		QIcon::setThemeName("hicolor");
	}

	Window window;
	window.show();

	return app.exec();
}
