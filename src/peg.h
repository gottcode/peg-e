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

#ifndef PEG_H
#define PEG_H

#include <QGraphicsEllipseItem>
class QColor;
class Board;

class Peg : public QGraphicsEllipseItem {
public:
	Peg(const QPoint& hole, Board* board, QGraphicsItem* parent = 0);

	bool canMove() {
		findHoles();
		return !m_holes.isEmpty();
	}

	void move(QPoint hole);

	void setAppearance(const QColor& color);

	enum {
		Type = UserType + 1
	};

	int type() const {
		return Type;
	}

protected:
	virtual void mousePressEvent(QGraphicsSceneMouseEvent* event);
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);

private:
	void findHoles();

private:
	QPoint m_hole;
	QList<QPoint> m_holes;
	Board* m_board;
};

#endif
