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

#include "peg.h"

#include "board.h"

#include <QBrush>
#include <QPen>

/*****************************************************************************/

Peg::Peg(const QPoint& hole, Board* board, QGraphicsItem* parent)
: QGraphicsEllipseItem(0, 0, 20, 20, parent),
  m_hole(hole),
  m_board(board) {
	setPen(Qt::NoPen);

	QGraphicsEllipseItem* gloss = new QGraphicsEllipseItem(3, 1.5, 14, 9, this);
	QLinearGradient gradient2(0, 0, 0, 9);
	gradient2.setColorAt(0, QColor(255, 255, 255));
	gradient2.setColorAt(1, QColor(255, 255, 255, 0));
	gloss->setBrush(gradient2);
	gloss->setPen(Qt::NoPen);

	setZValue(2);
	setPos(hole.x() * 20, hole.y() * 20);
	setFlag(QGraphicsItem::ItemIsMovable, true);
	setCursor(Qt::OpenHandCursor);
}

/*****************************************************************************/

void Peg::mousePressEvent(QGraphicsSceneMouseEvent* event) {
	findHoles();
	m_board->setHighlight(m_holes, true);

	setZValue(3);
	setCursor(Qt::ClosedHandCursor);
	QGraphicsEllipseItem::mousePressEvent(event);
}

/*****************************************************************************/

void Peg::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
	m_board->setHighlight(m_holes, false);

	QPoint hole = ((mapToScene(boundingRect().center()) / 20.f) - QPointF(0.5f, 0.5f)).toPoint();
	if (m_holes.contains(hole)) {
		m_board->move(m_hole, hole);
	}
	move(m_hole);

	setZValue(2);
	if (!m_board->isFinished()) {
		setCursor(Qt::OpenHandCursor);
	} else {
		unsetCursor();
	}
	QGraphicsEllipseItem::mouseReleaseEvent(event);
}

/*****************************************************************************/

void Peg::move(QPoint hole) {
	m_hole = hole;
	setPos(m_hole.x() * 20, m_hole.y() * 20);
}

/*****************************************************************************/

void Peg::setAppearance(const QColor& color) {
	QRadialGradient gradient(QPointF(10,10), 10, QPoint(10,13));
	gradient.setColorAt(0, color);
	gradient.setColorAt(1, color.darker());
	setBrush(gradient);
}

/*****************************************************************************/

void Peg::findHoles() {
	m_holes.clear();
	for (int r = -1; r < 2; ++r) {
		for (int c = -1; c < 2; ++c) {
			QPoint direction(c, r);
			QPoint hole = m_hole + (direction * 2);
			if (m_board->isHole(hole) && m_board->isPeg(m_hole + direction)) {
				m_holes.append(hole);
			}
		}
	}
}

/*****************************************************************************/
