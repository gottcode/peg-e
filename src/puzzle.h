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

#ifndef PUZZLE_H
#define PUZZLE_H

#include <QHash>
#include <QPoint>
#include <QSize>

class Puzzle {
public:
	Puzzle();
	virtual ~Puzzle();

	QHash<QPoint, bool> holes() const;
	QPoint position() const;
	QSize size() const;
	void generate(int seed, int difficulty);

protected:
	virtual void generate(int pegs);
	virtual bool isAvailable(const QPoint& hole) const;
	virtual void shuffle(QList<QPoint>& pegs) const;
	bool findNextMove(const QPoint& start_hole, QPoint& jumped_hole, QPoint& end_hole);
	QPoint findMoves(const QPoint& start, int loops);
	int hasPeg(const QPoint& hole) const;
	void setHasPeg(const QPoint& hole, bool value);

private:
	QHash<QPoint, bool> m_holes;
	QList<QPoint> m_directions;
	QPoint m_top_left;
	QPoint m_bottom_right;
};


class PuzzleBranch : public Puzzle {
protected:
	virtual void generate(int pegs);
};


class PuzzleLine : public Puzzle {
protected:
	virtual bool isAvailable(const QPoint& hole) const;
	virtual void shuffle(QList<QPoint>& pegs) const;
};

#endif
