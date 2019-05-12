/* XY_Curve_Data Header.
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

#ifndef NPT_XY_CURVE_DATA_H
#define NPT_XY_CURVE_DATA_H

#include <boost/shared_ptr.hpp>
#include <QVector>
#include <QChart>
#include <QPointF>
#include <QString>
using namespace QtCharts;

namespace QtCharts
{
	class QLineSeries;
	class QScatterSeries;
	class QDateTimeAxis;
	class QValueAxis;
}

namespace npt
{
	class XY_Curve_Data : public QChart
	{
	public:
		XY_Curve_Data();
		~XY_Curve_Data();

		void create_xy_axis(const QString& x_label, const QString& y_label);
		void create_pt_data_series_1(const QString& curve_label);
		void create_pt_data_series_2(const QString& curve_label);
		void create_current_pt();

		void set_x_axis_range(double low, double high);
		void set_y_axis_range(double low, double high);
		void set_x_axis_label(const QString& label_str);
		void set_y_axis_label(const QString& label_str);
		void update_pt_data_1_val(double x, double y, unsigned int pos);
		void update_pt_data_2_val(double x, double y, unsigned int pos);
		void update_current_pt_val(double x, double y);
		void enlarge_pt_data_series_1(unsigned int size);
		void enlarge_pt_data_series_2(unsigned int size);

		void set_title(const QString& title);
		void set_pt_data_series_1_name(const QString& name);
		void set_pt_data_series_2_name(const QString& name);
		void set_pt_data_series_2_value(double x_val, double y_val);
		void show_pt_data_series_2();
		void hide_pt_data_series_2();

	private:
		QtCharts::QLineSeries *pt_data_series_1_;
		QtCharts::QLineSeries *pt_data_series_2_;
		QtCharts::QScatterSeries *current_pt_;
		QtCharts::QValueAxis *x_axis_;
		QtCharts::QValueAxis *y_axis_;
	};
}

#endif
