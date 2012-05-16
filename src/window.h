/***********************************************************************
 *
 * Copyright (C) 2009 Graeme Gott <graeme@gottcode.org>
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

#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>
class QAction;
class QActionGroup;
class Board;

class Window : public QMainWindow {
	Q_OBJECT
public:
	Window();

protected:
	virtual void closeEvent(QCloseEvent* event);

private slots:
	void newGame();
	void restartGame();
	void showDetails();
	void changeAppearance(QAction* action);
	void changeAppearanceCustom();
	void about();

private:
	void loadGame();
	void startGame(int seed, int difficulty, int algorithm);

private:
	Board* m_board;
	QActionGroup* m_colors;
	int m_seed;
	int m_difficulty;
	int m_algorithm;
};

#endif
