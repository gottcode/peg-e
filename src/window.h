/*
	SPDX-FileCopyrightText: 2009-2013 Graeme Gott <graeme@gottcode.org>

	SPDX-License-Identifier: GPL-3.0-or-later
*/

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
	explicit Window();

protected:
	/** Override parent function to save window geometry. */
	void closeEvent(QCloseEvent* event) override;

private Q_SLOTS:
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
