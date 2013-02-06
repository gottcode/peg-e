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

#ifndef PEGE_BOARD_H
#define PEGE_BOARD_H

class Hole;
class Peg;

#include <QColor>
#include <QGraphicsView>
#include <QHash>
class QUndoStack;

/**
 * Game board.
 *
 * This class controls all of the game objects, as well as setting the color
 * of the pegs.
 */
class Board : public QGraphicsView
{
	Q_OBJECT

public:
	/**
	 * Constructs an empty game board.
	 *
	 * @param moves where to store the history of peg movements
	 * @param parent the game board's parent widget
	 */
	Board(QUndoStack* moves, QWidget* parent = 0);

	/** Returns @c true if there is only one peg left. */
	bool isFinished() const
	{
		return m_status;
	}

	/**
	 * Returns @c true if @p hole exists and is empty.
	 *
	 * @param hole the hole to check
	 */
	bool isHole(const QPoint& hole) const;

	/**
	 * Returns @c true if @p hole exists and has a peg.
	 *
	 * @param hole the hole to check
	 */
	bool isPeg(const QPoint& hole) const;

	/**
	 * Returns the Hole located at @p hole.
	 *
	 * @param hole the hole to fetch
	 */
	Hole* hole(const QPoint& hole) const;

	/**
	 * Creates a new puzzle layout.
	 *
	 * @param seed the seed passed to the random number generator
	 * @param difficulty how hard of a layout to create
	 * @param algorithm which Puzzle to use to lay out holes and pegs
	 */
	void generate(int seed, int difficulty, int algorithm);

	/**
	 * Moves a peg from @p old_hole to @p new_hole.
	 *
	 * @param old_hole the source hole
	 * @param new_hole the destination hole
	 */
	void move(const QPoint& old_hole, const QPoint& new_hole);

	/**
	 * Highlights holes that can be jumped to.
	 *
	 * @param holes which holes are valid moves
	 * @param highlight whether to add or remove highlight
	 */
	void setHighlight(const QList<QPoint>& holes, bool highlight = false);

	/**
	 * Changes the appearance of the pegs.
	 *
	 * @param color the color to use
	 */
	void setAppearance(const QColor& color);

protected:
	/** Override parent function to draw background gradient. */
	void drawBackground(QPainter* painter, const QRectF& rect);

	/** Override parent function to draw status message. */
	void drawForeground(QPainter* painter, const QRectF& rect);

	/** Override parent function to scale board when resized. */
	void resizeEvent(QResizeEvent* event);

private:
	/**
	 * Determine if there is only one peg left on the board.
	 *
	 * @return @c true if game is over; @c false otherwise
	 */
	bool checkFinished();

private:
	QHash<QPoint, Hole*> m_holes; /**< holes on board */
	QColor m_color; /**< peg color */
	int m_status; /**< status of game. @li @c 0 game is active @li @c 1 game is lost @li @c 2 game is won */
	QUndoStack* m_moves; /**< history of peg movements */
};

#endif // PEGE_BOARD_H
