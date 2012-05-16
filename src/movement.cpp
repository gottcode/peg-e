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

#include "movement.h"

#include "board.h"
#include "hole.h"
#include "peg.h"

#include <QSettings>

/*****************************************************************************/

Movement::Movement(const QPoint& start_hole, const QPoint& end_hole, Board* board)
: m_start_hole(start_hole),
  m_jumped_hole(start_hole + ((end_hole - start_hole) / 2)),
  m_end_hole(end_hole),
  m_board(board) {
	Q_ASSERT(m_board->isPeg(m_start_hole));
	Q_ASSERT(m_board->isPeg(m_jumped_hole));
	Q_ASSERT(m_board->isHole(m_end_hole));

	m_peg = m_board->hole(start_hole)->peg();
	m_jumped_peg = m_board->hole(m_jumped_hole)->peg();
}

/*****************************************************************************/

void Movement::redo() {
	m_board->hole(m_start_hole)->setPeg(0);
	m_board->hole(m_jumped_hole)->setPeg(0);
	m_board->hole(m_end_hole)->setPeg(m_peg);

	m_jumped_peg->hide();
	m_peg->move(m_end_hole);

	QStringList moves = QSettings().value("Current/Moves").toStringList();
	moves += QString("%1x%2 to %3x%4").arg(m_start_hole.x()).arg(m_start_hole.y()).arg(m_end_hole.x()).arg(m_end_hole.y());
	QSettings().setValue("Current/Moves", moves);
}

/*****************************************************************************/

void Movement::undo() {
	m_board->hole(m_start_hole)->setPeg(m_peg);
	m_board->hole(m_jumped_hole)->setPeg(m_jumped_peg);
	m_board->hole(m_end_hole)->setPeg(0);

	m_peg->move(m_start_hole);
	m_jumped_peg->show();

	QStringList moves = QSettings().value("Current/Moves").toStringList();
	moves.removeLast();
	QSettings().setValue("Current/Moves", moves);
}

/*****************************************************************************/
