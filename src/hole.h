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

#ifndef HOLE_H
#define HOLE_H

#include <QGraphicsEllipseItem>
class Peg;

class Hole : public QGraphicsEllipseItem {
public:
	Hole(const QPoint& position, QGraphicsItem* parent = 0);

	bool hasPeg() const {
		return m_peg != 0;
	}

	Peg* peg() const {
		return m_peg;
	}

	void setPeg(Peg* peg) {
		m_peg = peg;
	}

	void setHighlight(bool highlight);

private:
	Peg* m_peg;
};

#endif
