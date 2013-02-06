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

#ifndef PEGE_HOLE_H
#define PEGE_HOLE_H

class Peg;

#include <QGraphicsEllipseItem>

/**
 * %Hole that can contain a Peg.
 *
 * This class represents a hole on a game board. It stores a reference to any
 * peg that rests in it. It also handles drawing itself as well as a highlight
 * if it is the target for a valid jump.
 */
class Hole : public QGraphicsEllipseItem
{
public:
	/**
	 * Constructs a hole.
	 *
	 * @param position the location of the hole
	 * @param parent the parent item of the hole
	 */
	Hole(const QPoint& position, QGraphicsItem* parent = 0);

	/** Returns @c true if the hole has a peg. */
	bool hasPeg() const
	{
		return m_peg != 0;
	}

	/** Returns the peg contained by the hole. */
	Peg* peg() const
	{
		return m_peg;
	}

	/** Sets the peg contained by the hole. */
	void setPeg(Peg* peg)
	{
		m_peg = peg;
	}

	/** Sets the highlighted status of the hole. */
	void setHighlight(bool highlight);

private:
	Peg* m_peg; /**< peg contained by the hole */
};

#endif // PEGE_HOLE_H
