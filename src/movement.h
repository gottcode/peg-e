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

#ifndef PEGE_MOVEMENT_H
#define PEGE_MOVEMENT_H

class Board;
class Hole;
class Peg;

#include <QHash>
#include <QPoint>
#include <QUndoCommand>

/**
 * %Movement of a Peg.
 *
 * This class represents a move that has been made. It tracks references to
 * the pegs that were moved or removed, as well as their positions on a game
 * board.
 */
class Movement : public QUndoCommand
{
public:
	/**
	 * Constructs a movement.
	 *
	 * @param start_hole the hole the peg jumped from
	 * @param end_hole the hole the peg jumped to
	 * @param board the board that contains the holes
	 */
	Movement(const QPoint& start_hole, const QPoint& end_hole, Board* board);

	/** Move the peg from the start hole to the end hole, removing the peg in the hole between them. */
	void redo();

	/** Move the peg from the end hole to the start hole, restoring the peg in the hole between them. */
	void undo();

private:
	Peg* m_peg; /**< peg that was moved */
	Peg* m_jumped_peg; /**< peg that was removed */
	QPoint m_start_hole; /**< start position of moved peg */
	QPoint m_jumped_hole; /**< position of removed peg */
	QPoint m_end_hole; /**< end position of moved peg */
	Board* m_board; /**< game board */
};

#endif // PEGE_MOVEMENT_H
