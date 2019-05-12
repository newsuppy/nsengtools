/* TE_Sensor_Widget.
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
along with TurbineTools; see the file COPYING.  If not, write to
the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
Boston, MA 02110-1301, USA.  */

#include "te_sensor/temperature_scale_convertor.h"
#include "te_sensor/te_sensor_widget.h"
#include "xy_curve/xy_curve_view.h"
#include "xy_curve/xy_curve_data.h"
#include "xy_curve/xy_curve_ctrl.h"
//#include <boost/thread/thread.hpp>
#include <boost/log/trivial.hpp>

#include <QDateTimeEdit>
#include <QTimer>
#include <QStatusBar>
#include <QDebug>
#include <QMessageBox>
#include <QtNetwork>
#include <QStringList>
#include <QTextCodec>
#include <QLCDNumber>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QPushButton>
#include <QFrame>
#include <QDateTime>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QTabWidget>
#include <QComboBox>
#include <QProcess>
#include <QGridLayout>
#include <QSplitter>
#include <iostream>
#include <cstdio>

using namespace std;

namespace npt
{
	TE_Sensor_Widget::TE_Sensor_Widget(QWidget *parent)
		: QWidget(parent)
	{
		this->setup_ui();
		this->resize(800, 600);
	}

	void TE_Sensor_Widget::setup_ui()
	{
		//is->statusBar()->showMessage(tr("Ready"));
		static const QChar ctemp_unicode_str[] = { 0x2103 };
		ctemp_unit_ = QString::fromRawData(ctemp_unicode_str, 1);
		static const QChar ftemp_unicode_str[] = { 0x2109 };
		ftemp_unit_ = QString::fromRawData(ftemp_unicode_str, 1);

		this->setWindowTitle(tr("TemperatureCalc"));
		this->resize(480, 320);

		// Temperature Calculator
		temp_calc_main_layout_ = new QHBoxLayout();
		main_splitter_ = new QSplitter();
		temp_calc_info_layout_ = new QVBoxLayout();
		temp_calc_select_panel_layout_ = new QGridLayout();
		this->setLayout(temp_calc_main_layout_);
		temp_calc_main_layout_->addWidget(main_splitter_);
		temp_calc_info_frame_ = new QFrame();
		temp_calc_info_frame_->setLayout(temp_calc_info_layout_);
		main_splitter_->addWidget(temp_calc_info_frame_);
		temp_calc_info_layout_->addLayout(temp_calc_select_panel_layout_);
		temp_element_type_lbl_ = new QLabel(tr("Type"));
		temp_element_type_cb_ = new QComboBox();
		temp_element_type_cb_->addItem(tr("B")); temp_element_type_cb_->addItem(tr("E"));
		temp_element_type_cb_->addItem(tr("J")); temp_element_type_cb_->addItem(tr("K"));
		temp_element_type_cb_->addItem(tr("R")); temp_element_type_cb_->addItem(tr("S"));
		temp_element_type_cb_->addItem(tr("T")); temp_element_type_cb_->addItem(tr("RTD(Pt100)"));
		temp_element_type_cb_->setCurrentText(tr("K"));
		temp_calc_select_panel_layout_->addWidget(temp_element_type_lbl_, 0, 0);
		temp_calc_select_panel_layout_->addWidget(temp_element_type_cb_, 0, 1);
		temp_scale_lbl_ = new QLabel(QString("%1/%2").arg(ctemp_unit_).arg(ftemp_unit_));
		temp_scale_cb_ = new QComboBox();
		temp_scale_cb_->addItem(tr("Celsius"));
		temp_scale_cb_->addItem(tr("Fahrenheit"));
		temp_scale_cb_->setCurrentText(tr("Celsius"));
		temp_calc_select_panel_layout_->addWidget(temp_scale_lbl_, 1, 0);
		temp_calc_select_panel_layout_->addWidget(temp_scale_cb_, 1, 1);
		cjc_temp_lbl_ = new QLabel(tr("CJCTemp"));
		cjc_temp_txt_ = new QLineEdit(tr("20"));
		temp_calc_select_panel_layout_->addWidget(cjc_temp_lbl_, 2, 0);
		temp_calc_select_panel_layout_->addWidget(cjc_temp_txt_, 2, 1);
		temp_source_signal_lbl_ = new QLabel(tr("Source Signal"));
		temp_source_signal_txt_ = new QLineEdit(tr("0"));
		temp_source_signal_unit_lbl_ = new QLabel(tr("mV"));
		temp_calc_select_panel_layout_->addWidget(temp_source_signal_lbl_, 3, 0);
		temp_calc_select_panel_layout_->addWidget(temp_source_signal_txt_, 3, 1);
		temp_calc_select_panel_layout_->addWidget(temp_source_signal_unit_lbl_, 3, 2);
		temp_measure_lbl_ = new QLabel(tr("Temperature"));
		temp_measure_txt_ = new QLineEdit(tr("0"));
		temp_unit_lbl_ = new QLabel();
		temp_unit_lbl_->setText(ctemp_unit_);
		temp_calc_select_panel_layout_->addWidget(temp_measure_lbl_, 4, 0);
		temp_calc_select_panel_layout_->addWidget(temp_measure_txt_, 4, 1);
		temp_calc_select_panel_layout_->addWidget(temp_unit_lbl_, 4, 2);
		temp_method_lbl_ = new QLabel(tr("Method"));
		temp_method_cb_ = new QComboBox();
		//temp_method_cb_->addItem(tr("ITS-90Coeff"));
		//temp_method_cb_->addItem(tr("5thOrderPoly"));
		//temp_method_cb_->addItem(tr("IEC60751"));
		this->update_calc_method_combobox();
		temp_method_cb_->setCurrentText(tr("ITS-90Coeff"));
		temp_calc_select_panel_layout_->addWidget(temp_method_lbl_, 7, 0);
		temp_calc_select_panel_layout_->addWidget(temp_method_cb_, 7, 1);
		source2temp_calc_btn_ = new QPushButton(tr("Source Signal --> Temperature"));
		connect(source2temp_calc_btn_, SIGNAL(clicked()), this, SLOT(do_source2temp_calc()));
		temp_calc_info_layout_->addWidget(source2temp_calc_btn_);
		temp2source_calc_btn_ = new QPushButton(tr("Temperature --> Source Signal"));
		connect(temp2source_calc_btn_, SIGNAL(clicked()), this, SLOT(do_temp2source_calc()));
		temp_calc_info_layout_->addWidget(temp2source_calc_btn_);
		temp_calc_log_ = new QTextEdit();
		temp_calc_info_layout_->addWidget(temp_calc_log_);

		connect(temp_element_type_cb_, SIGNAL(currentTextChanged(const QString&)), this, SLOT(update_source_signal_unit()));
		connect(temp_scale_cb_, SIGNAL(currentTextChanged(const QString&)), this, SLOT(update_temperature_unit()));
		connect(temp_element_type_cb_, SIGNAL(currentTextChanged(const QString&)), this, SLOT(update_calc_method_combobox()));
		do_btn_ = new QPushButton(tr("DO"));
		connect(do_btn_, SIGNAL(clicked()), this, SLOT(do_something()));

		xy_curve_view_ = new XY_Curve_View();
		main_splitter_->addWidget(xy_curve_view_);
		main_splitter_->setStretchFactor(0, 1);
		main_splitter_->setStretchFactor(1, 6);
		main_splitter_->setStretchFactor(2, 1);
		xy_curve_data_ = boost::shared_ptr<XY_Curve_Data>(new XY_Curve_Data());
		xy_curve_view_->setChart(xy_curve_data_.get());

		xy_curve_ctrl_ = boost::shared_ptr<XY_Curve_Ctrl>(new XY_Curve_Ctrl());
		xy_curve_data_->create_xy_axis("Source Signal", "Temperature");
		xy_curve_data_->create_pt_data_series_1("ITS-90 Curve");
		xy_curve_data_->create_pt_data_series_2("Ovation 5thOrder");
		xy_curve_data_->create_current_pt();

	}

	void TE_Sensor_Widget::disp_msg(const QString& msg)
	{
		//++msg_count_;
		/*this->txt_log_->append(msg);
		if (msg_count_ >= 100)
		{
		this->txt_log_->clear();
		msg_count_ = 0;
		}*/
	}

	void TE_Sensor_Widget::resizeEvent(QResizeEvent *event)
	{
		QWidget::resizeEvent(event);
	}

	void TE_Sensor_Widget::update_calc_method_combobox()
	{
		if (this->temp_element_type_cb_->currentText().contains("RTD"))
		{
			temp_method_cb_->clear();
			temp_method_cb_->addItem("IEC60751");
			temp_method_cb_->addItem("5thOrderPoly");
		}
		else
		{
			temp_method_cb_->clear();
			temp_method_cb_->addItem("ITS-90Coeff");
			temp_method_cb_->addItem("5thOrderPoly");
		}
	}

	void TE_Sensor_Widget::do_source2temp_calc()
	{
		if (temp_element_type_cb_->currentText().contains("RTD"))
		{
			double source_signal = temp_source_signal_txt_->text().toDouble();
			qDebug() << "SOURCE:" << source_signal << "ohm";
			QString rtd_calc_type = rtd_calc_.gen_rtd_calc_type(temp_element_type_cb_->currentText(),
				temp_scale_cb_->currentText(), temp_method_cb_->currentText());
			qDebug() << "rtd_calc_type:" << rtd_calc_type;
			double temperature = rtd_calc_.calc_temperature(source_signal, rtd_calc_type,
				temp_element_type_cb_->currentText(), temp_scale_cb_->currentText(), temp_method_cb_->currentText());
			qDebug() << "temperature:" << temperature;
			temp_measure_txt_->setText(QString("%1").arg(temperature));
			this->temp_calc_log_->setText(rtd_calc_.get_ohm2t_calc_info());
			/*temperature = MathHelper.roundDoubleNumber(temperature, decimal_point);*/
			xy_curve_ctrl_->gen_rtd_ohm2te_data(xy_curve_data_, rtd_calc_, source_signal);
		}
		else
		{
			double source_signal = temp_source_signal_txt_->text().toDouble();
			QString tc_calc_type = tc_calc_.gen_tc_calc_type(temp_element_type_cb_->currentText(),
				temp_scale_cb_->currentText(), temp_method_cb_->currentText());
			double cjc_temp = cjc_temp_txt_->text().toDouble();
			double temperature = tc_calc_.calc_temperature(source_signal, cjc_temp, tc_calc_type,
				temp_element_type_cb_->currentText(),
				temp_scale_cb_->currentText(), temp_method_cb_->currentText());
			temp_measure_txt_->setText(QString("%1").arg(temperature));
			this->temp_calc_log_->setText(tc_calc_.get_mv2t_calc_info());
			/*
			temperature = MathHelper.roundDoubleNumber(temperature, decimal_point);
			//Toast.makeText(TemperatureActivity.this, "Enter New Signal, Calc TC Temperature", Toast.LENGTH_SHORT).show();*/

			xy_curve_ctrl_->gen_tc_mv2te_data(xy_curve_data_, tc_calc_, source_signal);
		}

	}

	void TE_Sensor_Widget::do_temp2source_calc()
	{
		if (temp_element_type_cb_->currentText().contains("RTD"))
		{
			double measure_value = temp_measure_txt_->text().toDouble();
			double source_signal = rtd_calc_.calc_source_ohm(measure_value, temp_element_type_cb_->currentText(),
				temp_scale_cb_->currentText(), temp_method_cb_->currentText());
			//sourceSignal = MathHelper.roundDoubleNumber(sourceSignal, decimal_point);
			temp_source_signal_txt_->setText(QString("%1").arg(source_signal));
			this->temp_calc_log_->setText(rtd_calc_.get_t2ohm_calc_info());

			xy_curve_ctrl_->gen_rtd_te2ohm_data(xy_curve_data_, rtd_calc_, measure_value);
		}
		else
		{
			double cjc_temp = cjc_temp_txt_->text().toDouble();
			double source_signal = tc_calc_.calc_source_signal(temp_measure_txt_->text().toDouble(), cjc_temp,
				temp_element_type_cb_->currentText(),
				temp_scale_cb_->currentText(), temp_method_cb_->currentText());
			//sourceSignal = MathHelper.roundDoubleNumber(sourceSignal, decimal_point);
			temp_source_signal_txt_->setText(QString("%1").arg(source_signal));
			this->temp_calc_log_->setText(tc_calc_.get_t2mv_calc_info());

			xy_curve_ctrl_->gen_tc_te2mv_data(xy_curve_data_, tc_calc_, temp_measure_txt_->text().toDouble());
			//Toast.makeText(TemperatureActivity.this, "Enter New Measure , Calc Signal", Toast.LENGTH_SHORT).show();
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	}

	void TE_Sensor_Widget::update_source_signal_unit()
	{
		if (temp_element_type_cb_->currentText().contains("RTD"))
		{
			temp_source_signal_unit_lbl_->setText(tr("ohm"));
		}
		else
		{
			temp_source_signal_unit_lbl_->setText(tr("mV"));
		}
	}

	void TE_Sensor_Widget::update_temperature_unit()
	{
		if (temp_scale_cb_->currentText() == "Celsius")
		{
			temp_unit_lbl_->setText(ctemp_unit_);
		}
		else if (temp_scale_cb_->currentText() == "Fahrenheit")
		{
			temp_unit_lbl_->setText(ftemp_unit_);;
		}
		else
		{
			qDebug() << "Unknown Temperature Unit!";
		}
	}

	TE_Sensor_Widget::~TE_Sensor_Widget()
	{
	}
}