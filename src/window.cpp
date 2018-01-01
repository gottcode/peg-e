/***********************************************************************
 *
 * Copyright (C) 2009, 2012, 2013, 2014, 2015, 2016, 2017, 2018 Graeme Gott <graeme@gottcode.org>
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

#include "window.h"

#include "board.h"
#include "locale_dialog.h"

#include <QActionGroup>
#include <QApplication>
#include <QColorDialog>
#include <QComboBox>
#include <QDialog>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QMenuBar>
#include <QMessageBox>
#include <QSettings>
#include <QSpinBox>
#include <QToolBar>
#include <QUndoStack>
#include <QVBoxLayout>

#include <ctime>
#include <random>

//-----------------------------------------------------------------------------

Window::Window() :
	m_seed(0),
	m_difficulty(QSettings().value("Difficulty", 5).toInt()),
	m_algorithm(QSettings().value("Algorithm", 1).toInt())
{
	// Create board
	QUndoStack* moves = new QUndoStack(this);
	m_board = new Board(moves, this);
	m_seed = QSettings().value("Current/Seed").toInt();
	setCentralWidget(m_board);

	// Create menubar
	QMenu* game_menu = menuBar()->addMenu(tr("&Game"));

	QAction* new_action = game_menu->addAction(QIcon::fromTheme("document-new"), tr("&New"), this, SLOT(newGame()), QKeySequence::New);
	QAction* restart_action = game_menu->addAction(QIcon::fromTheme("view-refresh"), tr("&Restart Game"), this, SLOT(restartGame()), QKeySequence::Refresh);
	game_menu->addAction(tr("&Details"), this, SLOT(showDetails()));
	game_menu->addSeparator();
	QAction* quit_action = game_menu->addAction(QIcon::fromTheme("application-exit"), tr("Quit"), this, SLOT(close()), QKeySequence::Quit);
	quit_action->setMenuRole(QAction::QuitRole);

	QMenu* move_menu = menuBar()->addMenu(tr("&Move"));

	QAction* undo_action = move_menu->addAction(QIcon::fromTheme("edit-undo"), tr("&Undo"), moves, SLOT(undo()), QKeySequence::Undo);
	undo_action->setEnabled(false);
	connect(moves, &QUndoStack::canUndoChanged, undo_action, &QAction::setEnabled);

	QAction* redo_action = move_menu->addAction(QIcon::fromTheme("edit-redo"), tr("&Redo"), moves, SLOT(redo()), QKeySequence::Redo);
	redo_action->setEnabled(false);
	connect(moves, &QUndoStack::canRedoChanged, redo_action, &QAction::setEnabled);

	QMenu* appearance_menu = menuBar()->addMenu(tr("&Appearance"));

	QMap<QString, QString> colors;
	colors.insert(tr("Blue"), "#0055ff");
	colors.insert(tr("Charcoal"), "#3d3d3d");
	colors.insert(tr("Green"), "#00aa00");
	colors.insert(tr("Midnight"), "#00007f");
	colors.insert(tr("Plum"), "#aa007f");
	colors.insert(tr("Red"), "#ff0000");
	colors.insert(tr("Yellow"), "#ffff00");
	QString selected_color = QSettings().value("Appearance", "#0055ff").toString();

	m_colors = new QActionGroup(this);
	QMapIterator<QString, QString> i(colors);
	while (i.hasNext()) {
		i.next();
		QAction* color_action = appearance_menu->addAction(i.key());
		color_action->setData(i.value());
		color_action->setCheckable(true);
		m_colors->addAction(color_action);
		if (i.value() == selected_color) {
			color_action->setChecked(true);
		}
	}

	connect(m_colors, &QActionGroup::triggered, this, &Window::changeAppearance);

	appearance_menu->addAction(tr("Custom..."), this, SLOT(changeAppearanceCustom()));

	QMenu* settings_menu = menuBar()->addMenu(tr("&Settings"));
	settings_menu->addAction(tr("Application &Language..."), this, SLOT(setLocale()));

	QMenu* help_menu = menuBar()->addMenu(tr("&Help"));
	QAction* about_action = help_menu->addAction(tr("&About"), this, SLOT(about()));
	about_action->setMenuRole(QAction::AboutRole);
	QAction* about_qt_action = help_menu->addAction(tr("About &Qt"), qApp, SLOT(aboutQt()));
	about_qt_action->setMenuRole(QAction::AboutQtRole);

	// Create toolbar
	QToolBar* toolbar = new QToolBar(this);
	toolbar->setIconSize(QSize(22, 22));
	toolbar->setFloatable(false);
	toolbar->setMovable(false);
	toolbar->setToolButtonStyle(Qt::ToolButtonFollowStyle);
	toolbar->addAction(new_action);
	toolbar->addAction(restart_action);
	toolbar->addSeparator();
	toolbar->addAction(undo_action);
	toolbar->addAction(redo_action);
	addToolBar(toolbar);
	setContextMenuPolicy(Qt::NoContextMenu);

	// Restore size and position
	resize(400,400);
	restoreGeometry(QSettings().value("Geometry").toByteArray());

	// Restore current game
	loadGame();
}

//-----------------------------------------------------------------------------

void Window::closeEvent(QCloseEvent* event)
{
	QSettings().setValue("Geometry", saveGeometry());
	QMainWindow::closeEvent(event);
}

//-----------------------------------------------------------------------------

void Window::newGame()
{
	QDialog dialog(this);
	dialog.setWindowTitle(tr("New Game"));

	// Create options
	QComboBox* algorithms_box = new QComboBox(&dialog);
	algorithms_box->addItem(tr("Original"), 1);
	algorithms_box->addItem(tr("Branch"), 2);
	algorithms_box->addItem(tr("Line"), 3);
	algorithms_box->setCurrentIndex(algorithms_box->findData(m_algorithm));

	QSpinBox* difficulty_box = new QSpinBox(&dialog);
	difficulty_box->setRange(1, 100);
	difficulty_box->setValue(m_difficulty);

	QSpinBox* seed_box = new QSpinBox(&dialog);
	seed_box->setRange(0, INT_MAX);
	seed_box->setValue(0);
	seed_box->setSpecialValueText(tr("Random"));

	// Create dialog buttons
	QDialogButtonBox* buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);
	connect(buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
	connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

	// Lay out options
	QFormLayout* options_layout = new QFormLayout;
	options_layout->addRow(tr("Algorithm:"), algorithms_box);
	options_layout->addRow(tr("Difficulty:"), difficulty_box);
	options_layout->addRow(tr("Seed:"), seed_box);

	// Lay out dialog
	QVBoxLayout* layout = new QVBoxLayout(&dialog);
	layout->addLayout(options_layout);
	layout->addSpacing(18);
	layout->addWidget(buttons);

	if (dialog.exec() == QDialog::Accepted) {
		startGame(seed_box->value(), difficulty_box->value(), algorithms_box->itemData(algorithms_box->currentIndex()).toInt());
	}
}

//-----------------------------------------------------------------------------

void Window::restartGame()
{
	if (m_board->isFinished() || QMessageBox::question(this, tr("Question"), tr("Do you want to restart?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes) {
		m_board->generate(m_seed, m_difficulty, m_algorithm);
		QSettings().remove("Current/Moves");
	}
}

//-----------------------------------------------------------------------------

void Window::showDetails()
{
	QString algorithm;
	switch (m_algorithm) {
	case 2:
		algorithm = tr("Branch");
		break;
	case 3:
		algorithm = tr("Line");
		break;
	case 1:
	default:
		algorithm = tr("Original");
		break;
	}
	QMessageBox::information(this, tr("Details"), tr("<p><b>Algorithm:</b> %1<br><b>Difficulty:</b> %2<br><b>Seed:</b> %L3</p>").arg(algorithm).arg(m_difficulty).arg(m_seed));
}

//-----------------------------------------------------------------------------

void Window::setLocale()
{
	LocaleDialog dialog;
	dialog.exec();
}

//-----------------------------------------------------------------------------

void Window::about()
{
	QMessageBox::about(this, tr("About Peg-E"), QString(
		"<p align='center'><big><b>%1 %2</b></big><br/>%3<br/><small>%4<br/>%5</small></p>"
		"<p align='center'>%6<br/><small>%7</small></p>")
		.arg(tr("Peg-E"), QCoreApplication::applicationVersion(),
			tr("Peg elimination game"),
			tr("Copyright &copy; 2009-%1 Graeme Gott").arg("2018"),
			tr("Released under the <a href=%1>GPL 3</a> license").arg("\"http://www.gnu.org/licenses/gpl.html\""),
			tr("Uses icons from the <a href=%1>Oxygen</a> icon theme").arg("\"http://www.oxygen-icons.org/\""),
			tr("Used under the <a href=%1>LGPL 3</a> license").arg("\"http://www.gnu.org/licenses/lgpl.html\""))
	);
}

//-----------------------------------------------------------------------------

void Window::changeAppearance(QAction* action)
{
	m_board->setAppearance(action->data().toString());
}

//-----------------------------------------------------------------------------

void Window::changeAppearanceCustom()
{
	QColor color = QColorDialog::getColor(QSettings().value("Appearance").toString(), this);
	if (!color.isValid()) {
		return;
	}

	QString selected_color = color.name();
	QList<QAction*> actions = m_colors->actions();
	for (QAction* action : actions) {
		action->setChecked(action->data() == selected_color);
	}

	m_board->setAppearance(color);
}

//-----------------------------------------------------------------------------

void Window::loadGame()
{
	// Load board
	QSettings settings;
	int seed = settings.value("Current/Seed", m_seed).toInt();
	int difficulty = settings.value("Current/Difficulty", m_difficulty).toInt();
	int algorithm = settings.value("Current/Algorithm", m_algorithm).toInt();
	QStringList moves = settings.value("Current/Moves").toStringList();
	if (settings.value("Current/Version").toInt() != 2) {
		moves.clear();
	}
	startGame(seed, difficulty, algorithm);

	// Load moves
	QRegExp parse("(-?\\d+)x(-?\\d+) to (-?\\d+)x(-?\\d+)");
	for (const QString& move : moves) {
		if (!parse.exactMatch(move)) {
			continue;
		}

		QPoint old_hole(parse.cap(1).toInt(), parse.cap(2).toInt());
		QPoint new_hole(parse.cap(3).toInt(), parse.cap(4).toInt());
		if (m_board->isPeg(old_hole) && m_board->isHole(new_hole)) {
			m_board->move(old_hole, new_hole);
		} else {
			qWarning("Invalid move: %dx%d to %dx%d", old_hole.x(), old_hole.y(), new_hole.x(), new_hole.y());
		}
	}
}

//-----------------------------------------------------------------------------

void Window::startGame(int seed, int difficulty, int algorithm)
{
	if (seed == 0) {
#ifndef Q_OS_WIN
		std::random_device rd;
		std::mt19937 gen(rd());
#else
		std::mt19937 gen(time(0));
#endif
		std::uniform_int_distribution<int> dis;
		seed = dis(gen);
	}
	m_seed = seed;
	m_difficulty = difficulty;
	m_algorithm = algorithm;

	m_board->generate(m_seed, m_difficulty, m_algorithm);

	QSettings settings;
	settings.setValue("Difficulty", m_difficulty);
	settings.setValue("Algorithm", m_algorithm);
	settings.setValue("Current/Version", 2);
	settings.setValue("Current/Seed", m_seed);
	settings.setValue("Current/Difficulty", m_difficulty);
	settings.setValue("Current/Algorithm", m_algorithm);
	settings.remove("Current/Moves");
}

//-----------------------------------------------------------------------------
