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

#ifndef BOARD_H
#define BOARD_H

#include <QColor>
#include <QGraphicsView>
#include <QHash>
class QUndoStack;
class Hole;
class Peg;

class Board : public QGraphicsView {
public:
	Board(QUndoStack* moves, QWidget* parent = 0);

	bool isFinished() const {
		return m_status;
	}

	bool isHole(const QPoint& hole) const;
	bool isPeg(const QPoint& hole) const;
	Hole* hole(const QPoint& hole) const;

	void generate(int seed, int difficutly, int algorithm);
	void move(const QPoint& old_hole, const QPoint& new_hole);
	void setHighlight(const QList<QPoint>& holes, bool highlight = false);
	void setAppearance(const QColor& color);

protected:
	virtual void drawBackground(QPainter* painter, const QRectF& rect);
	virtual void drawForeground(QPainter* painter, const QRectF& rect);
	virtual void resizeEvent(QResizeEvent* event);

private:
	bool checkFinished();

private:
	QHash<QPoint, Hole*> m_holes;
	QColor m_color;
	int m_status;
	QUndoStack* m_moves;
};

#endif
