/***********************************************************************
 *
 * Copyright (C) 2009, 2012, 2013 Graeme Gott <graeme@gottcode.org>
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

#ifndef PEGE_WINDOW_H
#define PEGE_WINDOW_H

class Board;

#include <QMainWindow>
class QAction;
class QActionGroup;

/**
 * Main window of the game.
 *
 * This is the main window. It defines the menubar and toolbar. It also
 * controls starting a game, as well as fetching details of the running game.
 */
class Window : public QMainWindow
{
	Q_OBJECT

public:
	/** Constructs the main window. */
	Window();

protected:
	/** Override parent function to save window geometry. */
	void closeEvent(QCloseEvent* event);

private slots:
	/**
	 * Start a new game.
	 *
	 * Shows a dialog of options that the player can adjust.
	 */
	void newGame();

	/** Restart the current game. */
	void restartGame();

	/** Show the current game details. */
	void showDetails();

	/**
	 * Changes the appearance of the board.
	 *
	 * @param action the color to set the pegs
	 */
	void changeAppearance(QAction* action);

	/**
	 * Changes the appearance of the board.
	 *
	 * Shows a dialog that allows the player to choose the peg color.
	 */
	void changeAppearanceCustom();

	/** Allows the player to change the application language. */
	void setLocale();

	/** Show the program details. */
	void about();

private:
	/** Starts the previous game. */
	void loadGame();

	/**
	 * Starts a new game.
	 *
	 * @param seed the seed passed to the random number generator
	 * @param difficulty how hard of a layout to create
	 * @param algorithm which Puzzle to use for the layout
	 */
	void startGame(int seed, int difficulty, int algorithm);

private:
	Board* m_board; /**< game board */
	QActionGroup* m_colors; /**< actions to set peg color */
	int m_seed; /**< seed passed to the random number generator */
	int m_difficulty; /**< how hard of a layout was created */
	int m_algorithm; /**< which Puzzle was used for the layout */
};

#endif // PEGE_WINDOW_H
