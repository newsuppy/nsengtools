/* Pt_Data Body.
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
//#pragma execution_character_set("utf-8")
#include <QDateTimeAxis>
#include <QDateTime>
#include <QLineSeries>
#include <QScatterSeries>
#include <QValueAxis>
#include <QPointF>
#include <QDebug>
#include <qmath.h>
#include "xy_curve/xy_curve_data.h"

namespace npt
{
	XY_Curve_Data::XY_Curve_Data()
		: pt_data_series_1_(0), pt_data_series_2_(0), current_pt_(0), x_axis_(0), y_axis_(0)
	{
		this->setTitle(tr("XY Curve Data View"));
	}

	void XY_Curve_Data::create_xy_axis(const QString& x_label, const QString& y_label)
	{
		x_axis_ = new QValueAxis();
		x_axis_->setTitleText(x_label);
		x_axis_->setRange(-100, 100);
		//x_axis_->setFormat("yyyy-MM-dd h:mm:ss");
		this->addAxis(x_axis_, Qt::AlignBottom);
		y_axis_ = new QValueAxis();
		y_axis_->setTitleText(y_label);
		y_axis_->setRange(-100, 100);
		//x_axis_->setFormat("yyyy-MM-dd h:mm:ss");
		this->addAxis(y_axis_, Qt::AlignLeft);	
	}

	void XY_Curve_Data::create_pt_data_series_1(const QString& curve_label)
	{
		pt_data_series_1_ = new QLineSeries();
		this->addSeries(pt_data_series_1_);
		//pt_data_series_->attachAxis(x_axis_);
		//pt_data_series_->attachAxis(y_axis_);
		this->setAxisX(x_axis_, pt_data_series_1_);
		this->setAxisY(y_axis_, pt_data_series_1_);
		//pt_data_series_->setPointLabelsVisible(true);
		pt_data_series_1_->setName(curve_label);
	}

	void XY_Curve_Data::create_pt_data_series_2(const QString& curve_label)
	{
		pt_data_series_2_ = new QLineSeries();
		this->addSeries(pt_data_series_2_);
		//pt_data_series_->attachAxis(x_axis_);
		//pt_data_series_->attachAxis(y_axis_);
		this->setAxisX(x_axis_, pt_data_series_2_);
		this->setAxisY(y_axis_, pt_data_series_2_);
		//pt_data_series_->setPointLabelsVisible(true);
		pt_data_series_2_->setName(curve_label);
	}

	void XY_Curve_Data::create_current_pt()
	{
		current_pt_ = new QScatterSeries();
		current_pt_->append(0, 0);
		this->addSeries(current_pt_);
		//pt_data_series_->attachAxis(x_axis_);
		//pt_data_series_->attachAxis(y_axis_);
		this->setAxisX(x_axis_, current_pt_);
		this->setAxisY(y_axis_, current_pt_);
		current_pt_->setName(tr("Current Point"));
		current_pt_->setPointLabelsVisible(true);
	}

	void XY_Curve_Data::set_x_axis_range(double low, double high)
	{
		x_axis_->setRange(low, high);
	}

	void XY_Curve_Data::set_y_axis_range(double low, double high)
	{
		y_axis_->setRange(low, high);
	}

	void XY_Curve_Data::set_x_axis_label(const QString& label_str)
	{
		x_axis_->setTitleText(label_str);
	}

	void XY_Curve_Data::set_y_axis_label(const QString& label_str)
	{
		y_axis_->setTitleText(label_str);
	}

	void XY_Curve_Data::update_pt_data_1_val(double x, double y, unsigned int pos)
	{
		if (pt_data_series_1_ != 0)
		{
			if (pos < pt_data_series_1_->count())
			{
				pt_data_series_1_->replace(pos, x, y);
				//qDebug() << "replace pt pos: " << x << "," << y << "," << pos;
			}
		}
	}

	void XY_Curve_Data::update_pt_data_2_val(double x, double y, unsigned int pos)
	{
		if (pt_data_series_2_ != 0)
		{
			if (pos < pt_data_series_2_->count())
			{
				pt_data_series_2_->replace(pos, x, y);
				//qDebug() << "replace pt pos: " << x << "," << y << "," << pos;
			}
		}
	}

	void XY_Curve_Data::update_current_pt_val(double x, double y)
	{
		if (current_pt_ != 0)
		{
			current_pt_->replace(0, x, y);
		}
	}

	void XY_Curve_Data::enlarge_pt_data_series_1(unsigned int size)
	{
		if (pt_data_series_1_ != 0 )
		{
			if (pt_data_series_1_->count() < size)
			{
				for (unsigned int append_cnt = 0; append_cnt <= size; ++append_cnt)
				{
					pt_data_series_1_->append(0, 0);
				}
			}
			//qDebug() << "enlarge Series Size: " << pt_data_series_->pointsVector().size();
		}
	}

	void XY_Curve_Data::enlarge_pt_data_series_2(unsigned int size)
	{
		if (pt_data_series_2_ != 0)
		{
			if (pt_data_series_2_->count() < size)
			{
				for (unsigned int append_cnt = 0; append_cnt <= size; ++append_cnt)
				{
					pt_data_series_2_->append(0, 0);
				}
			}
			//qDebug() << "enlarge Series Size: " << pt_data_series_->pointsVector().size();
		}
	}

	void XY_Curve_Data::set_title(const QString& title)
	{
		this->setTitle(title);
	}

	void XY_Curve_Data::set_pt_data_series_1_name(const QString& name)
	{
		if (pt_data_series_1_)
			pt_data_series_1_->setName(name);
	}

	void XY_Curve_Data::set_pt_data_series_2_name(const QString& name)
	{
		if (pt_data_series_2_)
			pt_data_series_2_->setName(name);
	}

	void XY_Curve_Data::set_pt_data_series_2_value(double x_val, double y_val)
	{
		;
	}

	void XY_Curve_Data::show_pt_data_series_2()
	{
		if (pt_data_series_2_)
			pt_data_series_2_->setVisible(true);
	}

	void XY_Curve_Data::hide_pt_data_series_2()
	{
		if (pt_data_series_2_)
			pt_data_series_2_->setVisible(false);
	}

	XY_Curve_Data::~XY_Curve_Data()
	{
		if (pt_data_series_1_)
			delete pt_data_series_1_;
		if (pt_data_series_2_)
			delete pt_data_series_2_;
		if (current_pt_)
			delete current_pt_;
		if (x_axis_)
			delete x_axis_;
		if (y_axis_)
			delete y_axis_;
	
		//qDebug() << "Trend_Data dtor";
	}
}