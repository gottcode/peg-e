/*
	SPDX-FileCopyrightText: 2009-2013 Graeme Gott <graeme@gottcode.org>

	SPDX-License-Identifier: GPL-3.0-or-later
*/

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
	Peg(const QPoint& hole, Board* board, QGraphicsItem* parent = nullptr);

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
	void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

	/** Override parent function to handle completing a jump. */
	void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

private:
	/** Search game board for valid jumps to make. */
	void findHoles();

private:
	QPoint m_hole; /**< position of hole containing peg */
	QList<QPoint> m_holes; /**< holes that can complete valid jumps */
	Board* m_board; /**< game board */
};

#endif // PEGE_PEG_H
