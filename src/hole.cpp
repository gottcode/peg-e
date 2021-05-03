/*
	SPDX-FileCopyrightText: 2009-2013 Graeme Gott <graeme@gottcode.org>

	SPDX-License-Identifier: GPL-3.0-or-later
*/

#include "hole.h"

#include <QBrush>
#include <QPen>
#include <QRadialGradient>

//-----------------------------------------------------------------------------

Hole::Hole(const QPoint& position, QGraphicsItem* parent)
	: QGraphicsEllipseItem(0, 0, 16, 16, parent)
	, m_peg(nullptr)
{
	QRadialGradient gradient(QPointF(8,8), 8);
	gradient.setColorAt(0, QColor(0, 0, 0, 0));
	gradient.setColorAt(1, QColor(0, 0, 0, 64));
	setBrush(gradient);
	setPen(Qt::NoPen);

	setZValue(1);
	setPos(position.x() * 20 + 2, position.y() * 20 + 2);
	setFlag(QGraphicsItem::ItemIsMovable, false);
}

//-----------------------------------------------------------------------------

void Hole::setHighlight(bool highlight)
{
	setPen(!highlight ? Qt::NoPen : QPen(Qt::yellow, 2));
}

//-----------------------------------------------------------------------------
