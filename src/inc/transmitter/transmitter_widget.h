/* Transmitter_Widget Header.
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

#ifndef NPT_TRANSMITTER_WIDGET_H
#define NPT_TRANSMITTER_WIDGET_H

#include <QWidget>
#include <QDateTimeEdit>
#include <QTimer>
#include <QPointer>
#include <QQueue>

#include <boost/shared_ptr.hpp>
#include "transmitter/transmitter_calculator.h"

class QLCDNumber;
class QTextEdit;
class QVBoxLayout;
class QHBoxLayout;
class QPushButton;
class QFrame;
class QLabel;
class QComboBox;
class QLineEdit;
class QTabWidget;
class QProcess;
class QGridLayout;
class QMenu;

namespace npt
{
	class XY_Curve_View;
	class XY_Curve_Data;
	class XY_Curve_Ctrl;

	class Transmitter_Widget : public QWidget
	{
		Q_OBJECT
	public:
		Transmitter_Widget(QWidget *parent = 0);
		~Transmitter_Widget();

	public slots:
		void trans_calc_output_with_hl_limit();

		void trans_calc_source_sigal();

		void trans_calc_output_with_gain_bias();

		void disp_msg(const QString& msg);

	protected:
		void resizeEvent(QResizeEvent *event);

	private:
		void setup_ui();

		QPointer<QHBoxLayout> trans_calc_main_layout_;
		QPointer<QVBoxLayout> trans_calc_info_layout_;
		QPointer<QGridLayout> trans_calc_select_panel_layout_;
		QPointer<QLabel> trans_type_lbl_;
		QPointer<QComboBox> trans_type_cb_;
		QPointer<QLabel> low_limit_lbl_;
		QPointer<QLineEdit> low_limit_txt_;
		QPointer<QLabel> high_limit_lbl_;
		QPointer<QLineEdit> high_limit_txt_;
		QPointer<QLabel> source_signal_lbl_;
		QPointer<QLineEdit> source_signal_txt_;
		QPointer<QLabel> source_signal_unit_lbl_;
		QPointer<QLabel> screen_value_lbl_;
		QPointer<QLineEdit> screen_value_txt_;
		QPointer<QLabel> gain_lbl_;
		QPointer<QLineEdit> gain_txt_;
		QPointer<QLabel> bias_lbl_;
		QPointer<QLineEdit> bias_txt_;
		QPointer<QLabel> method_lbl_;
		QPointer<QComboBox> method_cb_;
		QPointer<QPushButton> trans_screen_value_calc_btn_;
		QPointer<QPushButton> trans_source_signal_calc_btn_;
		QPointer<QPushButton> trans_gain_bias_calc_btn_;
		QPointer<XY_Curve_View> xy_curve_view_;

		QPointer<QTextEdit> trans_calc_log_;

		QPointer<QPushButton> do_btn_;

		Transmitter_Calculator trans_calc_;

		boost::shared_ptr<XY_Curve_Data> xy_curve_data_;
		boost::shared_ptr<XY_Curve_Ctrl> xy_curve_ctrl_;
	};
}

#endif
