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

#include "hole.h"

#include <QBrush>
#include <QPen>
#include <QRadialGradient>

/*****************************************************************************/

Hole::Hole(const QPoint& position, QGraphicsItem* parent)
: QGraphicsEllipseItem(0, 0, 16, 16, parent),
  m_peg(0) {
	QRadialGradient gradient(QPointF(8,8), 8);
	gradient.setColorAt(0, QColor(0, 0, 0, 0));
	gradient.setColorAt(1, QColor(0, 0, 0, 64));
	setBrush(gradient);
	setPen(Qt::NoPen);

	setZValue(1);
	setPos(position.x() * 20 + 2, position.y() * 20 + 2);
	setFlag(QGraphicsItem::ItemIsMovable, false);
}

/*****************************************************************************/

void Hole::setHighlight(bool highlight) {
	setPen(!highlight ? Qt::NoPen : QPen(Qt::yellow, 2));
}

/*****************************************************************************/
