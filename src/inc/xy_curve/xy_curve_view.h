/* XY_Curve_View Header.
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

#ifndef NPT_XY_CURVE_VIEW_H
#define NPT_XY_CURVE_VIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QChartView>
#include <boost/shared_ptr.hpp>
using namespace QtCharts;

class QMouseEvent;

namespace npt
{
	//class Trend_Data;
	class XY_Curve_View : public QChartView
	{
	Q_OBJECT
	public:
		XY_Curve_View();
		~XY_Curve_View();

		void draw_frame();
		
	signals:
		void mouse_move(QPoint point);

	protected:
		void mousePressEvent(QMouseEvent *event);
		void mouseMoveEvent(QMouseEvent *event);
		void mouseReleaseEvent(QMouseEvent *event);
		void keyPressEvent(QKeyEvent *event);
		void resizeEvent(QResizeEvent *event);

	private:
		double width_scale_;
		double height_scale_;
		double scene_width_;
		double scene_height_;

		QPoint begin_point_;
		QPoint end_point_;
		//Trend_Data* trend_data_;
	};
}

#endif
