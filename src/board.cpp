/*
	SPDX-FileCopyrightText: 2009 Graeme Gott <graeme@gottcode.org>

	SPDX-License-Identifier: GPL-3.0-or-later
*/

#include "board.h"

#include "hole.h"
#include "movement.h"
#include "peg.h"
#include "puzzle.h"

#include <QGridLayout>
#include <QLabel>
#include <QLinearGradient>
#include <QResizeEvent>
#include <QSettings>
#include <QUndoStack>

#include <algorithm>

//-----------------------------------------------------------------------------

Board::Board(QUndoStack* moves, QWidget* parent)
	: QGraphicsView(parent)
	, m_color(QSettings().value("Appearance", "#0055ff").toString())
	, m_status(0)
	, m_moves(moves)
{
	QGraphicsScene* scene = new QGraphicsScene(this);
	setScene(scene);

	m_message = new QLabel(this);
	m_message->setFont(QFont("Sans", 24));
	m_message->setStyleSheet(
		"QLabel {"
			"background-color: rgba(0, 0, 0, 200);"
			"color: white;"
			"margin: 0;"
			"padding: 1em;"
			"border-radius: 10px;"
		"}");
	m_message->hide();

	QGridLayout* layout = new QGridLayout(this);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(0);
	layout->addWidget(m_message, 0, 0, Qt::AlignCenter);

	// Configure view
	setCacheMode(QGraphicsView::CacheBackground);
	setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
	setFrameStyle(QFrame::NoFrame);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setMinimumSize(300, 300);
}

//-----------------------------------------------------------------------------

bool Board::isHole(const QPoint& hole) const
{
	return m_holes.contains(hole) ? !m_holes[hole]->hasPeg() : false;
}

//-----------------------------------------------------------------------------

bool Board::isPeg(const QPoint& hole) const
{
	return m_holes.contains(hole) ? m_holes[hole]->hasPeg() : false;
}

//-----------------------------------------------------------------------------

Hole* Board::hole(const QPoint& hole) const
{
	return m_holes.contains(hole) ? m_holes[hole] : nullptr;
}

//-----------------------------------------------------------------------------

void Board::generate(int seed, int difficulty, int algorithm)
{
	// Remove old board
	m_status = 0;
	m_message->hide();
	m_moves->clear();
	m_holes.clear();
	scene()->clear();
	setInteractive(true);

	// Create puzzle
	Puzzle* puzzle = nullptr;
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

	const QHash<QPoint, bool> holes = puzzle->holes();
	for (auto i = holes.cbegin(); i != holes.cend(); ++i) {
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

	for (Hole* hole : std::as_const(m_holes)) {
		Peg* peg = hole->peg();
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

void Board::resizeEvent(QResizeEvent* event)
{
	fitInView(sceneRect(), Qt::KeepAspectRatio);
	QGraphicsView::resizeEvent(event);
}

//-----------------------------------------------------------------------------

bool Board::checkFinished()
{
	int pegs = 0;
	for (Hole* hole : std::as_const(m_holes)) {
		if (hole->hasPeg()) {
			pegs++;
			if (hole->peg()->canMove()) {
				return false;
			}
		}
	}

	if (pegs == 1) {
		m_status = 2;
		m_message->setText(tr("Success"));
	} else {
		m_status = 1;
		m_message->setText(tr("Game Over"));
	}
	m_message->show();

	return true;
}

//-----------------------------------------------------------------------------
