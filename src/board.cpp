/***********************************************************************
 *
 * Copyright (C) 2009, 2013, 2018 Graeme Gott <graeme@gottcode.org>
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

#include "board.h"

#include "hole.h"
#include "movement.h"
#include "peg.h"
#include "puzzle.h"

#include <QLinearGradient>
#include <QRegExp>
#include <QResizeEvent>
#include <QSettings>
#include <QUndoStack>

#include <algorithm>

//-----------------------------------------------------------------------------

inline uint qHash(const QPoint& key)
{
	return (key.x() << 16) + key.y();
}

//-----------------------------------------------------------------------------

Board::Board(QUndoStack* moves, QWidget* parent) :
	QGraphicsView(parent),
	m_color(QSettings().value("Appearance", "#0055ff").toString()),
	m_status(0),
	m_moves(moves)
{
	QGraphicsScene* scene = new QGraphicsScene(this);
	setScene(scene);

	// Configure view
	setCacheMode(QGraphicsView::CacheNone);
	setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
	setFrameStyle(QFrame::NoFrame);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setMinimumSize(300, 300);
}

//-----------------------------------------------------------------------------

bool Board::isHole(const QPoint& hole) const
{
	return m_holes.contains(hole) ? !m_holes.value(hole)->hasPeg() : 0;
}

//-----------------------------------------------------------------------------

bool Board::isPeg(const QPoint& hole) const
{
	return m_holes.contains(hole) ? m_holes.value(hole)->hasPeg() : 0;
}

//-----------------------------------------------------------------------------

Hole* Board::hole(const QPoint& hole) const
{
	return m_holes.contains(hole) ? m_holes[hole] : 0;
}

//-----------------------------------------------------------------------------

void Board::generate(int seed, int difficulty, int algorithm)
{
	// Remove old board
	m_status = 0;
	m_moves->clear();
	m_holes.clear();
	scene()->clear();
	setInteractive(true);

	// Create puzzle
	Puzzle* puzzle = 0;
	switch (algorithm) {
	case 2:
		puzzle = new PuzzleBranch;
		break;
	case 3:
		puzzle = new PuzzleLine;
		break;
	case 1:
	default:
		puzzle = new Puzzle;
		break;
	}
	puzzle->generate(seed, difficulty);

	// Create scene
	setSceneRect(QRectF(puzzle->position() * 20, puzzle->size() * 20).adjusted(-10,-10,10,10));
	fitInView(sceneRect(), Qt::KeepAspectRatio);

	QHash<QPoint, bool> holes = puzzle->holes();
	QHashIterator<QPoint, bool> i(holes);
	while (i.hasNext()) {
		i.next();
		QPoint position = i.key();

		Hole* hole = new Hole(position);
		scene()->addItem(hole);
		m_holes.insert(position, hole);

		if (i.value()) {
			Peg* peg = new Peg(position, this);
			peg->setAppearance(m_color);
			hole->setPeg(peg);
			scene()->addItem(peg);
		}
	}

	delete puzzle;
}

//-----------------------------------------------------------------------------

void Board::move(const QPoint& old_hole, const QPoint& new_hole)
{
	// Move peg
	Movement* movement = new Movement(old_hole, new_hole, this);
	m_moves->push(movement);

	// Handle finishing the game
	if (checkFinished()) {
		setInteractive(false);
		scene()->update();
		m_moves->clear();
		QSettings().remove((m_status == 2) ? "Current" : "Current/Moves");
	}
}

//-----------------------------------------------------------------------------

void Board::setHighlight(const QList<QPoint>& holes, bool highlight)
{
	for (const QPoint& hole : holes) {
		Q_ASSERT(m_holes.contains(hole));
		m_holes[hole]->setHighlight(highlight);
	}
}

//-----------------------------------------------------------------------------

void Board::setAppearance(const QColor& color)
{
	m_color = color;
	QSettings().setValue("Appearance", m_color.name());

	QHashIterator<QPoint, Hole*> i(m_holes);
	while (i.hasNext()) {
		Peg* peg = i.next().value()->peg();
		if (peg) {
			peg->setAppearance(m_color);
		}
	}
}

//-----------------------------------------------------------------------------

void Board::drawBackground(QPainter* painter, const QRectF& rect)
{
	QLinearGradient gradient(sceneRect().topLeft(), sceneRect().bottomRight());
	gradient.setColorAt(0, QColor(241, 208, 178));
	gradient.setColorAt(1, QColor(193, 146, 70));
	painter->fillRect(rect, gradient);
}

//-----------------------------------------------------------------------------

void Board::drawForeground(QPainter* painter, const QRectF&)
{
	if (m_status) {
		QString message((m_status == 2) ? tr("Success") : tr("Game Over"));
		QFontMetrics metrics(QFont("Sans", 24));
		int w = metrics.boundingRect(message).width();
		int h = metrics.height();
		int ratio = devicePixelRatio();
		QPixmap pixmap(QSize(w + h, h * 2) * ratio);
		pixmap.setDevicePixelRatio(ratio);
		pixmap.fill(QColor(0, 0, 0, 0));
		{
			QPainter pixmap_painter(&pixmap);

			pixmap_painter.setPen(Qt::NoPen);
			pixmap_painter.setBrush(QColor(0, 0, 0, 200));
			pixmap_painter.setRenderHint(QPainter::Antialiasing, true);
			pixmap_painter.drawRoundedRect(0, 0, w + h, h * 2, 10, 10);

			pixmap_painter.setFont(QFont("Sans", 24));
			pixmap_painter.setPen(Qt::white);
			pixmap_painter.setRenderHint(QPainter::TextAntialiasing, true);
			pixmap_painter.drawText(h / 2, h / 2 + metrics.ascent(), message);
		}

		painter->save();
		painter->resetTransform();
		painter->drawPixmap((width() - (pixmap.width() / ratio)) / 2, (height() - (pixmap.height() / ratio)) / 2, pixmap);
		painter->restore();
	}
}

//-----------------------------------------------------------------------------

void Board::resizeEvent(QResizeEvent* event)
{
	fitInView(sceneRect(), Qt::KeepAspectRatio);
	QGraphicsView::resizeEvent(event);
}

//-----------------------------------------------------------------------------

bool Board::checkFinished()
{
	int pegs = 0;
	for (Hole* hole : m_holes) {
		if (hole->hasPeg()) {
			pegs++;
			if (hole->peg()->canMove()) {
				return false;
			}
		}
	}
	m_status = (pegs == 1) ? 2 : 1;
	return true;
}

//-----------------------------------------------------------------------------
