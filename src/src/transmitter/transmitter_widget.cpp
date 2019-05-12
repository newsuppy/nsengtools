/* Transmitter_Widget.
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

#include "transmitter/transmitter_widget.h"
#include "te_sensor/temperature_scale_convertor.h"
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

#include <iostream>
#include <cstdio>

using namespace std;

namespace npt
{
	Transmitter_Widget::Transmitter_Widget(QWidget *parent)
		: QWidget(parent)
	{
		this->setup_ui();
		this->resize(800, 600);
	}

	void Transmitter_Widget::setup_ui()
	{
		this->setWindowTitle(tr("TransmitterCalc"));
		this->resize(480, 320);
		
		trans_calc_main_layout_ = new QHBoxLayout();
		trans_calc_info_layout_ = new QVBoxLayout();
		trans_calc_select_panel_layout_ = new QGridLayout();
		this->setLayout(trans_calc_main_layout_);
		trans_calc_main_layout_->addLayout(trans_calc_info_layout_);
		trans_calc_info_layout_->addLayout(trans_calc_select_panel_layout_);
		trans_type_lbl_ = new QLabel(tr("Type"));
		trans_type_cb_ = new QComboBox();
		trans_type_cb_->addItem(tr("4-20mA"));
		trans_type_cb_->setCurrentText(tr("4-20mA"));
		trans_calc_select_panel_layout_->addWidget(trans_type_lbl_, 0, 0);
		trans_calc_select_panel_layout_->addWidget(trans_type_cb_, 0, 1);
		low_limit_lbl_ = new QLabel(tr("Low Limit"));
		low_limit_txt_ = new QLineEdit(tr("0"));
		trans_calc_select_panel_layout_->addWidget(low_limit_lbl_, 1, 0);
		trans_calc_select_panel_layout_->addWidget(low_limit_txt_, 1, 1);
		high_limit_lbl_ = new QLabel(tr("High Limit"));
		high_limit_txt_ = new QLineEdit(tr("100"));
		trans_calc_select_panel_layout_->addWidget(high_limit_lbl_, 2, 0);
		trans_calc_select_panel_layout_->addWidget(high_limit_txt_, 2, 1);
		source_signal_lbl_ = new QLabel(tr("Source Signal"));
		source_signal_txt_ = new QLineEdit(tr("4"));
		source_signal_unit_lbl_ = new QLabel(tr("mA"));
		trans_calc_select_panel_layout_->addWidget(source_signal_lbl_, 3, 0);
		trans_calc_select_panel_layout_->addWidget(source_signal_txt_, 3, 1);
		trans_calc_select_panel_layout_->addWidget(source_signal_unit_lbl_, 3, 2);
		screen_value_lbl_ = new QLabel(tr("Screen Value"));
		screen_value_txt_ = new QLineEdit(tr("0"));
		trans_calc_select_panel_layout_->addWidget(screen_value_lbl_, 4, 0);
		trans_calc_select_panel_layout_->addWidget(screen_value_txt_, 4, 1);
		gain_lbl_ = new QLabel(tr("Gain"));
		gain_txt_ = new QLineEdit(tr("6250"));
		trans_calc_select_panel_layout_->addWidget(gain_lbl_, 5, 0);
		trans_calc_select_panel_layout_->addWidget(gain_txt_, 5, 1);
		bias_lbl_ = new QLabel(tr("Bias"));
		bias_txt_ = new QLineEdit(tr("-25"));
		trans_calc_select_panel_layout_->addWidget(bias_lbl_, 6, 0);
		trans_calc_select_panel_layout_->addWidget(bias_txt_, 6, 1);
		method_lbl_ = new QLabel(tr("Type"));
		method_cb_ = new QComboBox();
		method_cb_->addItem(tr("linear"));
		method_cb_->setCurrentText(tr("linear"));
		trans_calc_select_panel_layout_->addWidget(method_lbl_, 7, 0);
		trans_calc_select_panel_layout_->addWidget(method_cb_, 7, 1);
		trans_screen_value_calc_btn_ = new QPushButton(tr("Calculate Output with Low/High Limit"));
		connect(trans_screen_value_calc_btn_, SIGNAL(clicked()), this, SLOT(trans_calc_output_with_hl_limit()));
		trans_calc_info_layout_->addWidget(trans_screen_value_calc_btn_);

		trans_source_signal_calc_btn_ = new QPushButton(tr("Calculate Source Signal"));
		connect(trans_source_signal_calc_btn_, SIGNAL(clicked()), this, SLOT(trans_calc_source_sigal()));
		trans_calc_info_layout_->addWidget(trans_source_signal_calc_btn_);

		trans_gain_bias_calc_btn_ = new QPushButton(tr("Calculate Output with Gain/Bias"));
		connect(trans_gain_bias_calc_btn_, SIGNAL(clicked()), this, SLOT(trans_calc_output_with_gain_bias()));
		trans_calc_info_layout_->addWidget(trans_gain_bias_calc_btn_);

		trans_calc_log_ = new QTextEdit();
		trans_calc_info_layout_->addWidget(trans_calc_log_);
		do_btn_ = new QPushButton(tr("DO"));
		connect(do_btn_, SIGNAL(clicked()), this, SLOT(do_something()));

		xy_curve_view_ = new XY_Curve_View();
		trans_calc_main_layout_->addWidget(xy_curve_view_);

		//this->statusBar()->addWidget(timestamp_lbl_);

		xy_curve_data_ = boost::shared_ptr<XY_Curve_Data>(new XY_Curve_Data());
		xy_curve_view_->setChart(xy_curve_data_.get());

		xy_curve_ctrl_ = boost::shared_ptr<XY_Curve_Ctrl>(new XY_Curve_Ctrl());
		xy_curve_data_->create_xy_axis("Source Signal(mA)", "Screen Value");
		xy_curve_data_->create_pt_data_series_1("Transmitter Curve");
		xy_curve_data_->create_current_pt();

	}

	void Transmitter_Widget::disp_msg(const QString& msg)
	{
		//++msg_count_;
		/*this->txt_log_->append(msg);
		if (msg_count_ >= 100)
		{
		this->txt_log_->clear();
		msg_count_ = 0;
		}*/
	}

	void Transmitter_Widget::resizeEvent(QResizeEvent *event)
	{
		QWidget::resizeEvent(event);
	}

	void Transmitter_Widget::trans_calc_output_with_hl_limit()
	{
		double low_limit = low_limit_txt_->text().toDouble();
		double high_limit = high_limit_txt_->text().toDouble();
		double source_signal = source_signal_txt_->text().toDouble();
		double screen_value = trans_calc_.calc_output(source_signal, low_limit, high_limit);
		screen_value_txt_->setText(QString("%1").arg(screen_value));
		gain_txt_->setText(QString("%1").arg(trans_calc_.get_gain()));
		bias_txt_->setText(QString("%1").arg(trans_calc_.get_bias()));
		QString calc_info = QString("%1%2Output: %3\n").arg("Source Signal -> Screen Value(Output)\n").arg("Output = Gain * Input + Bias\n").arg(screen_value);
		calc_info.append(QString("Gain: %1\nBias: %2").arg(trans_calc_.get_gain()).arg(trans_calc_.get_bias()));
		trans_calc_log_->setText(calc_info);

		xy_curve_ctrl_->gen_transmitter_data(xy_curve_data_, trans_calc_, source_signal);
	}

	void Transmitter_Widget::trans_calc_source_sigal()
	{
		double low_limit = low_limit_txt_->text().toDouble();
		double high_limit = high_limit_txt_->text().toDouble();
		double screen_value = screen_value_txt_->text().toDouble();
		double source_signal = trans_calc_.calc_source_signal(screen_value, low_limit, high_limit);
		source_signal_txt_->setText(QString("%1").arg(source_signal));
		gain_txt_->setText(QString("%1").arg(trans_calc_.get_gain()));
		bias_txt_->setText(QString("%1").arg(trans_calc_.get_bias()));
		QString calc_info = QString("%1%2Souce Signal: %3 mA\n").arg("Screen Value(Output) -> Source Signal\n").arg("Input = (Output - Bias) / Gain\n").arg(source_signal);
		calc_info.append(QString("Gain: %1\nBias: %2").arg(trans_calc_.get_gain()).arg(trans_calc_.get_bias()));
		trans_calc_log_->setText(calc_info);

		xy_curve_ctrl_->gen_transmitter_data(xy_curve_data_, trans_calc_, source_signal);
	}

	void Transmitter_Widget::trans_calc_output_with_gain_bias()
	{
		double gain = gain_txt_->text().toDouble();
		double bias = bias_txt_->text().toDouble();
		double source_signal = source_signal_txt_->text().toDouble();

		double output = trans_calc_.set_gain_bias(source_signal, gain, bias);
		screen_value_txt_->setText(QString("%1").arg(output));
		low_limit_txt_->setText(QString("%1").arg(trans_calc_.get_low_limit()));
		high_limit_txt_->setText(QString("%1").arg(trans_calc_.get_high_limit()));
		QString calc_info = QString("%1").arg("Change Gain or Bias\n");
		calc_info.append(QString("Gain: %1 Bias: %2\n").arg(trans_calc_.get_gain()).arg(trans_calc_.get_bias()));
		calc_info.append(QString("Low Limit: %1 High Limit: %2\n").arg(trans_calc_.get_low_limit()).arg(trans_calc_.get_high_limit()));
		calc_info.append(QString("Source Signal: %1 mA\nOutput(Screen Value): %2").arg(source_signal).arg(output));
		trans_calc_log_->setText(calc_info);

		xy_curve_ctrl_->gen_transmitter_data(xy_curve_data_, trans_calc_, source_signal);
	}

	Transmitter_Widget::~Transmitter_Widget()
	{
	}
}