/* TE_Sensor_Widget Header.
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

#ifndef NPT_TE_SENSOR_WIDGET_H
#define NPT_TE_SENSOR_WIDGET_H

#include <QWidget>
#include <QDateTimeEdit>
#include <QTimer>
#include <QPointer>
#include <QQueue>

#include <boost/shared_ptr.hpp>
#include "rtd_calculator.h"
#include "thermo_couple_calculator.h"
#include "nseng_info_db.h"

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
class QSplitter;

namespace npt
{
	class XY_Curve_View;
	class XY_Curve_Data;
	class XY_Curve_Ctrl;

	class TE_Sensor_Widget : public QWidget
	{
		Q_OBJECT
	public:
		TE_Sensor_Widget(QWidget *parent = 0);
		~TE_Sensor_Widget();

	public slots:
	    void update_calc_method_combobox();

		void do_source2temp_calc();

		void do_temp2source_calc();

		void update_source_signal_unit();

		void update_temperature_unit();

		void disp_msg(const QString& msg);

	protected:
		void resizeEvent(QResizeEvent *event);

	private:
		void setup_ui();

		// Temperature Calculator
		QPointer<QHBoxLayout> temp_calc_main_layout_;
		QPointer<QSplitter> main_splitter_;
		QPointer<QFrame> temp_calc_info_frame_;
		QPointer<QVBoxLayout> temp_calc_info_layout_;
		QPointer<QGridLayout> temp_calc_select_panel_layout_;
		QPointer<QLabel> temp_element_type_lbl_;
		QPointer<QComboBox> temp_element_type_cb_;
		QPointer<QLabel> temp_scale_lbl_;
		QPointer<QComboBox> temp_scale_cb_;
		QPointer<QLabel> cjc_temp_lbl_;
		QPointer<QLineEdit> cjc_temp_txt_;
		QPointer<QLabel> temp_source_signal_lbl_;
		QPointer<QLineEdit> temp_source_signal_txt_;
		QPointer<QLabel> temp_source_signal_unit_lbl_;
		QPointer<QLabel> temp_measure_lbl_;
		QPointer<QLineEdit> temp_measure_txt_;
		QPointer<QLabel> temp_unit_lbl_;
		QPointer<QLabel> temp_method_lbl_;
		QPointer<QComboBox> temp_method_cb_;
		QPointer<QPushButton> source2temp_calc_btn_;
		QPointer<QPushButton> temp2source_calc_btn_;
		QPointer<QTextEdit> temp_calc_log_;
		QPointer<XY_Curve_View> xy_curve_view_;

		QPointer<QPushButton> do_btn_;

		QString ftemp_unit_;
		QString ctemp_unit_;
		RTD_Calculator rtd_calc_;
		Thermo_Couple_Calculator tc_calc_;

		boost::shared_ptr<XY_Curve_Data> xy_curve_data_;
		boost::shared_ptr<XY_Curve_Ctrl> xy_curve_ctrl_;
	};
}

#endif
