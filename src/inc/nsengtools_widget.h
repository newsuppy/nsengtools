/* NSEngTools_Widget Header.
Copyright (C) 2018
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

#ifndef NPT_NSENGTOOLS_WIDGET_H
#define NPT_NSENGTOOLS_WIDGET_H

#include <QMainWindow>
#include <QDateTimeEdit>
#include <QTimer>
#include <QPointer>
#include <QQueue>

#include <boost/shared_ptr.hpp>
#include "te_sensor/rtd_calculator.h"
#include "te_sensor/thermo_couple_calculator.h"
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

namespace npt
{
	class XY_Curve_View;
	class XY_Curve_Data;
	class XY_Curve_Ctrl;
	class Transmitter_Widget;
	class TE_Sensor_Widget;

	class NSEngTools_Widget : public QMainWindow
	{
		Q_OBJECT
	public:
		NSEngTools_Widget(QWidget *parent = 0);
		~NSEngTools_Widget();

	public slots:
		void do_something();

		void disp_msg(const QString& msg);

		void about();

	protected:
		void resizeEvent(QResizeEvent *event);

	private:
		void setup_ui();
		void init_env();

		void create_menus();
		void create_actions();


		// Main Window Menus
		QMenu* file_menu_;
		QMenu* help_menu_;

		// Main Window Actions
		QAction* exit_action_;
		QAction* about_action_;
		QAction* quit_action_;

		QPointer<QFrame> main_frame_;

		QPointer<QVBoxLayout> main_layout_;
		QPointer<QTabWidget> tab_container_;

		// Temperature Calculator
		QPointer<TE_Sensor_Widget> te_sensor_widget_;

		// Transmitter Calculator
		QPointer<Transmitter_Widget> transmitter_widget_;

		QPointer<QPushButton> do_btn_;
	};
}

#endif
