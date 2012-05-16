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

#ifndef MOVEMENT_H
#define MOVEMENT_H

#include <QHash>
#include <QPoint>
#include <QUndoCommand>
class Board;
class Hole;
class Peg;

class Movement : public QUndoCommand {
public:
	Movement(const QPoint& start_hole, const QPoint& end_hole, Board* board);

	virtual void redo();
	virtual void undo();

private:
	Peg* m_peg;
	Peg* m_jumped_peg;
	QPoint m_start_hole;
	QPoint m_jumped_hole;
	QPoint m_end_hole;
	Board* m_board;
};

#endif
