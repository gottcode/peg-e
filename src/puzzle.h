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

#ifndef PEGE_PUZZLE_H
#define PEGE_PUZZLE_H

#include <QHash>
#include <QPoint>
#include <QSize>

#include <random>

/**
 * Contains the layout of holes and pegs.
 *
 * This class generates and contains a board layout. When it generates a
 * layout it tracks which holes are the furthest left and up, right and down
 * so that the size of the layout can be quickly retrieved.
 */
class Puzzle
{
public:
	/** Constructs an empty layout. */
	Puzzle();

	/** Destroys the layout. */
	virtual ~Puzzle();

	/** Returns the layout as a hash of points describing the holes. */
	QHash<QPoint, bool> holes() const;

	/** Returns the top left corner of the layout. */
	QPoint position() const;

	/** Returns the size of the layout. */
	QSize size() const;

	/**
	 * Creates a layout.
	 *
	 * @param seed the seed passed to the random number generator
	 * @param difficulty how hard of a layout to create
	 */
	void generate(int seed, int difficulty);

protected:
	/**
	 * Creates a layout with a single group of pegs.
	 *
	 * @param pegs how many pegs to create
	 */
	virtual void generate(int pegs);

	/**
	 * Returns @c true if @p hole has no peg.
	 *
	 * @param hole the hole to check
	 */
	virtual bool isAvailable(const QPoint& hole) const;

	/**
	 * Randomly shuffles a list of pegs.
	 *
	 * @param pegs the pegs to shuffle
	 */
	virtual void shuffle(QList<QPoint>& pegs);

	/**
	 * Determines where to move next.
	 *
	 * @param start_hole the hole that is moved from
	 * @param jumped_hole the hole between the start and end holes
	 * @param end_hole the hole that is moved to
	 */
	bool findNextMove(const QPoint& start_hole, QPoint& jumped_hole, QPoint& end_hole);

	/**
	 * Creates a group of pegs.
	 *
	 * @param start initial peg
	 * @param loops how many pegs to create
	 */
	QPoint findMoves(const QPoint& start, int loops);

	/**
	 * Returns whether or not the @p hole has a peg.
	 *
	 * @param hole the hole to check
	 * @return @li @c 0 if the hole is empty
	 *         @li @c 1 if the hole has a peg
	 *         @li @c -1 if the hole doesn't exist
	 */
	int hasPeg(const QPoint& hole) const;

	/**
	 * Sets if the @p hole has a peg.
	 *
	 * @param hole the hole to specify
	 * @param value what to put at the hole
	 */
	void setHasPeg(const QPoint& hole, bool value);

private:
	QHash<QPoint, bool> m_holes; /**< hash of holes */
	QList<QPoint> m_directions; /**< list of directions to check */
	QPoint m_top_left; /**< top left hole */
	QPoint m_bottom_right; /**< bottom right hole */
	std::mt19937 m_random; /**< random number generator */
};

/** Puzzle that has branches between groups of pegs. */
class PuzzleBranch : public Puzzle
{
protected:
	/**
	 * Creates a layout with branches between the groups.
	 *
	 * @param pegs how many pegs to create
	 */
	void generate(int pegs);
};

/** Puzzle that doesn't reuse or shuffle any holes. */
class PuzzleLine : public Puzzle
{
protected:
	/**
	 * Returns true if @p hole does not exist yet.
	 *
	 * @param hole the hole to check
	 */
	bool isAvailable(const QPoint& hole) const;

	/**
	 * Override parent function to prevent shuffling the pegs.
	 *
	 * @param pegs the pegs to shuffle
	 */
	void shuffle(QList<QPoint>& pegs);
};

#endif // PEGE_PUZZLE_H
