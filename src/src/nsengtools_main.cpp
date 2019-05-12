/* NSEngTools Main.
Copyright (C) 2018
newsuppy

This file is part of NSPCTools

*/

#include <QApplication>
#include <QSystemTrayIcon>
#include <QMessageBox>
#include <QDir>

#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/trivial.hpp>

#include <exception>

#include "nsengtools_widget.h"

using namespace std;
using namespace boost::log;

int main(int argc, char *argv[])
{
	int rtn_val = 0;
	try
	{
		auto file_sink = boost::log::add_file_log(keywords::file_name = "nsengtools%N.log",
			keywords::rotation_size = 8 * 1024 * 1024,
			keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0),
			keywords::format = "[%TimeStamp%]: %Message%");
		boost::log::add_console_log();
		boost::log::add_common_attributes();
		file_sink->locked_backend()->set_file_collector(boost::log::sinks::file::make_collector(
			keywords::target = "logs",
			keywords::max_size = 64 * 1024 * 1024,
			keywords::min_free_space = 128 * 1024 * 1024
		));
		file_sink->locked_backend()->scan_for_files();
		file_sink->locked_backend()->auto_flush(true);

		QApplication app(argc, argv);

		QString app_dir = QApplication::applicationDirPath();
		BOOST_LOG_TRIVIAL(info) << "The NSEngTools path is: " << app_dir.toStdWString();
		QDir dir;
		dir.cd("C:\npt");
		if (!QSystemTrayIcon::isSystemTrayAvailable())
		{
			QMessageBox::critical(0, QObject::tr("Systray"), QObject::tr("I couldn't detect any system tray on this system."));
			return 1;
		}
		QApplication::setQuitOnLastWindowClosed(true);

		npt::NSEngTools_Widget main_window;
		main_window.showNormal();
#ifndef DEBUG
		//main_window.close();
#endif
		rtn_val = app.exec();

		BOOST_LOG_TRIVIAL(info) << "Remove all Sinks!\n";
		boost::log::core::get()->remove_all_sinks();
	}
	catch (const std::exception& ex)
	{
		QMessageBox::about(0, "Main Error!", ex.what());
	}
	return rtn_val;
}
