/***********************************************************************
 *
 * Copyright (C) 2009, 2013 Graeme Gott <graeme@gottcode.org>
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

#ifndef PEGE_PEG_H
#define PEGE_PEG_H

class Board;

#include <QGraphicsEllipseItem>
class QColor;

/**
 * %Peg movable by the player.
 *
 * This class represents a peg on a game board. It can be dragged by the
 * player, or moved between holes by calling move(), but only if the peg
 * determines that it is a valid jump. It tracks what hole it is currently
 * resting in to make it faster to determine if it can move.
 */
class Peg : public QGraphicsEllipseItem
{
public:
	/**
	 * Constructs a peg.
	 *
	 * @param hole the hole containing the peg
	 * @param board the board containing the peg
	 * @param parent the parent item of the peg
	 */
	Peg(const QPoint& hole, Board* board, QGraphicsItem* parent = 0);

	/** Returns @c true if the peg can move; @c false otherwise */
	bool canMove()
	{
		findHoles();
		return !m_holes.isEmpty();
	}

	/**
	 * Moves the peg.
	 *
	 * @param hole where to move the peg
	 */
	void move(QPoint hole);

	/**
	 * Change the peg appearance.
	 *
	 * @param color what color is used to draw peg
	 */
	void setAppearance(const QColor& color);

protected:
	/** Override parent function to check if there are valid moves. */
	void mousePressEvent(QGraphicsSceneMouseEvent* event);

	/** Override parent function to handle completing a jump. */
	void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);

private:
	/** Search game board for valid jumps to make. */
	void findHoles();

private:
	QPoint m_hole; /**< position of hole containing peg */
	QList<QPoint> m_holes; /**< holes that can complete valid jumps */
	Board* m_board; /**< game board */
};

#endif // PEGE_PEG_H
