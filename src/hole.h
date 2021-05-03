/*
	SPDX-FileCopyrightText: 2009-2013 Graeme Gott <graeme@gottcode.org>

	SPDX-License-Identifier: GPL-3.0-or-later
*/

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
	explicit Hole(const QPoint& position, QGraphicsItem* parent = nullptr);

	/** Returns @c true if the hole has a peg. */
	bool hasPeg() const
	{
		return m_peg;
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
