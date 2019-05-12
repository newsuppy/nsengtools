/* NSEng_Info_DB Header.
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

#ifndef NPT_NSENG_INFO_DB_H
#define NPT_NSENG_INFO_DB_H

#include <QString>
#include <QSqlDatabase>

namespace npt
{
	class NSEng_Info_DB
	{
	public:
		NSEng_Info_DB(QString db_name = "db/nseng_info_v4.db");

		void init();

		QString query_value(QString table_name, QString column_name) const;

		void fill_temp_element_coeff_map(QHash<QString, QVector<double> > &coeff_map, QString elemnt_tag);
		void load_its90_coeff_map(QHash<QString, QVector<double> > &coeff_map, QString elemnt_tag);
		void load_te_limit_table(QHash<QString, QVector<double> > &te_limit_table);
	
	private:
		QString dabasePath = "./db/";
		QString databaseName = "nseng_info_v4.db";
		QString databaseFileName;
	
		QString conn_name_;
		QString db_name_;
		QSqlDatabase db_;
	};
}

#endif
