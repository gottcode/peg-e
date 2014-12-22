/***********************************************************************
 *
 * Copyright (C) 2009, 2013, 2014 Graeme Gott <graeme@gottcode.org>
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

#include "puzzle.h"

#include <algorithm>

//-----------------------------------------------------------------------------

inline uint qHash(const QPoint& key)
{
	return (key.x() << 16) + key.y();
}

//-----------------------------------------------------------------------------

Puzzle::Puzzle()
{
	m_directions = QList<QPoint>()
		<< QPoint(-1,0)
		<< QPoint(-1,-1)
		<< QPoint(0,-1)
		<< QPoint(1,-1)
		<< QPoint(1,0)
		<< QPoint(1,1)
		<< QPoint(0,1)
		<< QPoint(-1,1);
}

//-----------------------------------------------------------------------------

Puzzle::~Puzzle()
{
}

//-----------------------------------------------------------------------------

QHash<QPoint, bool> Puzzle::holes() const
{
	return m_holes;
}

//-----------------------------------------------------------------------------

QPoint Puzzle::position() const
{
	return m_top_left;
}

//-----------------------------------------------------------------------------

QSize Puzzle::size() const
{
	return QSize((m_bottom_right.x() - m_top_left.x()) + 1, (m_bottom_right.y() - m_top_left.y()) + 1);
}

//-----------------------------------------------------------------------------

void Puzzle::generate(int seed, int difficulty)
{
	m_random.seed(seed);

	difficulty = qBound(1, difficulty, 101);
	difficulty += 5;

	generate(difficulty);
}

//-----------------------------------------------------------------------------

void Puzzle::generate(int pegs)
{
	QPoint start(0, 0);
	setHasPeg(start, true);
	findMoves(start, pegs);
}

//-----------------------------------------------------------------------------

bool Puzzle::isAvailable(const QPoint& hole) const
{
	return hasPeg(hole) != 1;
}

//-----------------------------------------------------------------------------

void Puzzle::shuffle(QList<QPoint>& pegs)
{
	std::shuffle(pegs.begin(), pegs.end(), m_random);
}

//-----------------------------------------------------------------------------

bool Puzzle::findNextMove(const QPoint& start_hole, QPoint& jumped_hole, QPoint& end_hole)
{
	std::shuffle(m_directions.begin(), m_directions.end(), m_random);
	for (const QPoint& direction : m_directions) {
		jumped_hole = direction + start_hole;
		end_hole = (direction * 2) + start_hole;
		if (isAvailable(jumped_hole) && isAvailable(end_hole)) {
			setHasPeg(start_hole, false);
			setHasPeg(jumped_hole, true);
			setHasPeg(end_hole, true);
			return true;
		}
	}
	return false;
}

//-----------------------------------------------------------------------------

QPoint Puzzle::findMoves(const QPoint& start, int loops)
{
	QList<QPoint> pegs;
	pegs.append(start);

	QPoint jumped_hole, end_hole;
	for (int i = 0; i < loops; ++i) {
		shuffle(pegs);
		for (const QPoint& start_hole : pegs) {
			if (findNextMove(start_hole, jumped_hole, end_hole)) {
				pegs.removeOne(start_hole);
				pegs.append(jumped_hole);
				pegs.append(end_hole);
				break;
			}
		}
	}

	return pegs.last();
}

//-----------------------------------------------------------------------------

int Puzzle::hasPeg(const QPoint& hole) const
{
	return m_holes.contains(hole) ? m_holes.value(hole) : -1;
}

//-----------------------------------------------------------------------------

void Puzzle::setHasPeg(const QPoint& hole, bool value)
{
	m_top_left.setX(std::min(m_top_left.x(), hole.x()));
	m_top_left.setY(std::min(m_top_left.y(), hole.y()));
	m_bottom_right.setX(std::max(m_bottom_right.x(), hole.x()));
	m_bottom_right.setY(std::max(m_bottom_right.y(), hole.y()));
	m_holes[hole] = value;
}

//-----------------------------------------------------------------------------

void PuzzleBranch::generate(int pegs)
{
	QPoint start_hole(0, 0);
	setHasPeg(start_hole, true);

	int loops = pegs;
	while (loops > 0) {
		int group_loops = std::min(10, loops);
		start_hole = findMoves(start_hole, group_loops);
		loops -= group_loops;

		QPoint jumped_hole, end_hole;
		int branch_loops = std::min(10, loops);
		for (int i = 0; i < branch_loops; ++i) {
			if (findNextMove(start_hole, jumped_hole, end_hole)) {
				start_hole = end_hole;
				loops--;
			}
		}
	}
}

//-----------------------------------------------------------------------------

bool PuzzleLine::isAvailable(const QPoint& hole) const
{
	return hasPeg(hole) == -1;
}

//-----------------------------------------------------------------------------

void PuzzleLine::shuffle(QList<QPoint>&)
{
}

//-----------------------------------------------------------------------------
