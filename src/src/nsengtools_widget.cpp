/* NSEngTools_Widget.
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

#include "nsengtools_widget.h"
#include "te_sensor/temperature_scale_convertor.h"
#include "transmitter/transmitter_widget.h"
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
#include <QMenu>
#include <QMenuBar>

#include <iostream>
#include <cstdio>

using namespace std;

namespace npt
{
	NSEngTools_Widget::NSEngTools_Widget(QWidget *parent)
		: QMainWindow(parent)
	{
		this->init_env();
		this->setup_ui();
		this->resize(800, 600);
	}

	void NSEngTools_Widget::setup_ui()
	{
		//is->statusBar()->showMessage(tr("Ready"));
		this->setWindowTitle(tr("NSEngTools"));
		this->resize(480, 320);
		main_frame_ = new QFrame();
		this->setCentralWidget(main_frame_);
		main_layout_ = new QVBoxLayout();
		main_frame_->setLayout(main_layout_);

		// Temperature Calculator
		do_btn_ = new QPushButton(tr("DO"));
		connect(do_btn_, SIGNAL(clicked()), this, SLOT(do_something()));

		tab_container_ = new QTabWidget();
		main_layout_->addWidget(tab_container_);
		te_sensor_widget_ = new TE_Sensor_Widget(this);
		tab_container_->addTab(te_sensor_widget_, "Temperature Calc");
		transmitter_widget_ = new Transmitter_Widget(this);
		tab_container_->addTab(transmitter_widget_, "Transmitter Calc");
		qDebug() << "tab size:" << tab_container_->width() << "," << tab_container_->height();

		this->create_actions();
		this->create_menus();
		//this->statusBar()->addWidget(timestamp_lbl_);
	}

	void NSEngTools_Widget::init_env()
	{
		QString py_cfg_file_name = "py_exe_path.txt";
		QFile py_cfg_file(py_cfg_file_name);
		if (py_cfg_file.open(QFile::ReadOnly))
		{
			QTextStream py_cfg_stream(&py_cfg_file);
			//py_exe_path_ = py_cfg_stream.readLine();
			//disp_msg(QString("The Python executable path is: %1\n").arg(py_exe_path_));
			//this->get_weather_data_cmd_ = QString("%1 py_scripts/open_weather_map.py").arg(py_exe_path_);
			//qDebug() << get_weather_data_cmd_;
		}
	}

	void NSEngTools_Widget::disp_msg(const QString& msg)
	{
		//++msg_count_;
		/*this->txt_log_->append(msg);
		if (msg_count_ >= 100)
		{
			this->txt_log_->clear();
			msg_count_ = 0;
		}*/
	}

	void NSEngTools_Widget::resizeEvent(QResizeEvent *event)
	{
		QMainWindow::resizeEvent(event);
	}
	
	void NSEngTools_Widget::do_something()
	{
		qDebug() << this->tab_container_->width() << "," << this->tab_container_->height();
		this->resize(800, 600);
	}

	void NSEngTools_Widget::create_menus()
	{
		this->file_menu_ = this->menuBar()->addMenu(tr("&File"));
		file_menu_->addSeparator();
		this->file_menu_->addAction(this->exit_action_);
		//update_recent_file_actions();

		this->help_menu_ = this->menuBar()->addMenu(tr("&Help"));
		this->help_menu_->addAction(this->about_action_);
	}

	void NSEngTools_Widget::create_actions()
	{
		this->exit_action_ = new QAction(tr("E&xit"), this);
		this->exit_action_->setShortcut(QKeySequence("CTRL+Q"));
		this->exit_action_->setStatusTip(tr("Exit the application"));
		connect(exit_action_, SIGNAL(triggered()), qApp, SLOT(quit()));


		// Help menu action
		this->about_action_ = new QAction(tr("About &NSEngTools"), this);
		this->about_action_->setStatusTip(tr("About NSEngTools"));
		connect(about_action_, SIGNAL(triggered()), this, SLOT(about()));

		quit_action_ = new QAction(tr("&Quit"), this);
		connect(quit_action_, SIGNAL(triggered()), qApp, SLOT(quit()));
	}

	void NSEngTools_Widget::about()
	{
		QMessageBox::about(this, "About", "NSEngTools v1.0.0\nnewsuppy@outlook.com\nQt5.9.6");
	}

	NSEngTools_Widget::~NSEngTools_Widget()
	{
	}
}