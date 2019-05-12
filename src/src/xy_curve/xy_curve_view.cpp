/* Pt_Graphics_View Body.
Copyright (C) 2019
newsuppy

This file is part of NSPCTools.

NSPCTools is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

NSPCTools is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with NSPCTools; see the file COPYING.  If not, write to
the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
Boston, MA 02110-1301, USA.  */

#include "xy_curve/xy_curve_view.h"
#include <QDebug>

namespace npt
{
	XY_Curve_View::XY_Curve_View()
		: width_scale_(0.99), height_scale_(0.99),
		scene_width_(800), scene_height_(600)
	{
		this->setDragMode(QGraphicsView::RubberBandDrag);
		this->setMouseTracking(true);
	}

	void XY_Curve_View::mousePressEvent(QMouseEvent *event)
	{
		if (event->button() == Qt::LeftButton)
		{
			begin_point_ = event->pos();
			qDebug() << event->pos();
		}
		QChartView::mousePressEvent(event);
	}

	void XY_Curve_View::mouseMoveEvent(QMouseEvent *event)
	{
		QPoint point = event->pos();
		//qDebug() << "Move:" << event->pos();
		emit mouse_move(point);
		QChartView::mouseMoveEvent(event);
	}

	void XY_Curve_View::mouseReleaseEvent(QMouseEvent *event)
	{
		if (event->button() == Qt::LeftButton)
		{
			end_point_ = event->pos();
			QRectF rect;
			rect.setTopLeft(this->begin_point_);
			rect.setBottomRight(this->end_point_);
			this->chart()->zoomIn(rect);
		}
		else if (event->button() == Qt::RightButton)
		{
			this->chart()->zoomReset();
		}

		QChartView::mouseReleaseEvent(event);
	}

	void XY_Curve_View::keyPressEvent(QKeyEvent *event)
	{
		switch (event->key())
		{
		case Qt::Key_Plus:
			chart()->zoom(1.2);
			break;
		case Qt::Key_Minus:
			chart()->zoom(0.8);
			break;
		case Qt::Key_Left:
			chart()->scroll(10, 0);
			break;
		case Qt::Key_Right:
			chart()->scroll(-10, 0);
			break;
		case Qt::Key_Up:
			chart()->scroll(0, 10);
			break;
		case Qt::Key_Down:
			chart()->scroll(0, -10);
			break;
		case Qt::Key_PageUp:
			chart()->scroll(0, 50);
			break;
		case Qt::Key_PageDown:
			chart()->scroll(0, -50);
			break;
		case Qt::Key_Home:
			chart()->zoomReset();
			break;
		default:
			QChartView::keyPressEvent(event);
		}

	}

	void XY_Curve_View::resizeEvent(QResizeEvent *event)
	{
		qDebug() << "Trend View:" << this->width() << "," << this->height();
		scene_width_ = this->width() * width_scale_;
		scene_height_ = this->height() * height_scale_;
		QChartView::resizeEvent(event);
	}

	void XY_Curve_View::draw_frame()
	{
	}

	XY_Curve_View::~XY_Curve_View()
	{
	}
}