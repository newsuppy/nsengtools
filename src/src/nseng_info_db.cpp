#include "nseng_info_db.h"
#include <QDebug>
#include <QUuid>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <iostream>
using namespace std;

namespace npt
{
	NSEng_Info_DB::NSEng_Info_DB(QString db_name)
		: db_name_(db_name)
	{
		this->conn_name_ = QUuid::createUuid().toString();
		//qDebug() << "DB UUID: " << conn_name_;
		db_ = QSqlDatabase::addDatabase("QSQLITE", conn_name_);
		db_.setDatabaseName(db_name_);
		//qDebug() << "DB: " << db_name_ << "|" << conn_name_ << " added!";
	}

	void NSEng_Info_DB::init()
	{
		;
	}

	QString NSEng_Info_DB::query_value(QString table_name, QString column_name) const
	{
		QString row_value;

		/*try
		{
			if (db_.open())
			{
				QSqlQuery query(db_);
				QString query_str = QString("SELECT %1 FROM %2 where %1 = :setting_name");
				query.prepare(query_str);
				query.bindValue(":setting_name", setting_name);
				if (query.exec())
				{
					if (query.next())
					{
						setting_value = query.value(0).toString();
						qDebug() << "setting_value = " << setting_value << "\n";
					}
				}
				else
				{
					qDebug() << "get_app_setting: db query failed!\n";
				}
			}
			else
			{
				qDebug() << "get_app_setting: db open failed!\n";
			}

			if (db_.isOpen())
			{
				db_.close();
			}
			//qDebug() << "Get- setting name: " << setting_name << "setting value: " << setting_value << endl;
		}
		catch (const std::exception& exp)
		{
			cerr << "select failed: " << exp.what() << "\n";
			cerr << exp.what();
		}

		return setting_value;
		/*String rowValue = "";
		Cursor cursor = db.query(tableName, null, null, null, null, null, null);
		if (cursor.moveToFirst()) {
			do {
				rowValue = cursor.getString(cursor.getColumnIndex(columnName));
			} while (cursor.moveToNext());
		}*/

		return "rowValue";
	}

	void NSEng_Info_DB::fill_temp_element_coeff_map(QHash<QString, QVector<double> > &coeff_map, QString elemnt_tag)
	{
		try
		{
			if (db_.open())
			{
				QSqlQuery query(db_);
				QString query_str = QString("SELECT * FROM temperature_element_tbl where te_element_tag like :elemnt_tag");
				query.prepare(query_str);
				query.bindValue(":elemnt_tag", elemnt_tag);
				if (query.exec())
				{
					int id = 0;
					while (query.next())
					{
						QString elem_type = query.value("te_element_tag").toString();
						QVector<double> coeff_vec;
						coeff_vec.push_back(query.value("c1").toDouble());
						coeff_vec.push_back(query.value("c2").toDouble());
						coeff_vec.push_back(query.value("c3").toDouble());
						coeff_vec.push_back(query.value("c4").toDouble());
						coeff_vec.push_back(query.value("c5").toDouble());
						coeff_vec.push_back(query.value("c6").toDouble());
						coeff_vec.push_back(query.value("c7").toDouble());
						coeff_vec.push_back(query.value("c8").toDouble());
						coeff_map.insert(elem_type, coeff_vec);
						//qDebug() << "value = " << elem_type << ":" << coeff_vec;
					}
					//qDebug() << coeff_map;
				}
				else
				{
					qDebug() << "fill_temp_element_coeff_map: db query failed!\n";
				}
			}
			else
			{
				qDebug() << "fill_temp_element_coeff_map: db open failed!\n";
			}

			if (db_.isOpen())
			{
				db_.close();
			}
			//qDebug() << "Get- setting name: " << setting_name << "setting value: " << setting_value << endl;
		}
		catch (const std::exception& exp)
		{
			cerr << "fill_temp_element_coeff_map failed: " << exp.what() << "\n";
		}
		//Toast.makeText(NSApplication.getContext(), "queryTCCoeffMap", Toast.LENGTH_SHORT).show();
		//Toast.makeText(NSApplication.getContext(), String.valueOf(count), Toast.LENGTH_SHORT).show();
	}

	void NSEng_Info_DB::load_its90_coeff_map(QHash<QString, QVector<double> > &coeff_map, QString elemnt_tag)
	{
		try
		{
			if (db_.open())
			{
				QSqlQuery query(db_);
				QString query_str = QString("SELECT * FROM its90_te_tbl where te_element_tag like :elemnt_tag");
				query.prepare(query_str);
				query.bindValue(":elemnt_tag", elemnt_tag);
				if (query.exec())
				{
					int id = 0;
					while (query.next())
					{
						QString elem_type = query.value("te_element_tag").toString();
						QVector<double> coeff_vec;
						coeff_vec.push_back(query.value("c0").toDouble());
						coeff_vec.push_back(query.value("c1").toDouble());
						coeff_vec.push_back(query.value("c2").toDouble());
						coeff_vec.push_back(query.value("c3").toDouble());
						coeff_vec.push_back(query.value("c4").toDouble());
						coeff_vec.push_back(query.value("c5").toDouble());
						coeff_vec.push_back(query.value("c6").toDouble());
						coeff_vec.push_back(query.value("c7").toDouble());
						coeff_vec.push_back(query.value("c8").toDouble());
						coeff_vec.push_back(query.value("c9").toDouble());
						coeff_vec.push_back(query.value("c10").toDouble());
						coeff_vec.push_back(query.value("c11").toDouble());
						coeff_vec.push_back(query.value("c12").toDouble());
						coeff_vec.push_back(query.value("c13").toDouble());
						coeff_vec.push_back(query.value("c14").toDouble());
						coeff_vec.push_back(query.value("c15").toDouble());
						coeff_vec.push_back(query.value("a0").toDouble());
						coeff_vec.push_back(query.value("a1").toDouble());
						coeff_vec.push_back(query.value("a2").toDouble());
						coeff_vec.push_back(query.value("a3").toDouble());
						coeff_vec.push_back(query.value("a4").toDouble());
						coeff_vec.push_back(query.value("a5").toDouble());
						coeff_vec.push_back(query.value("low_te_limit").toDouble());
						coeff_vec.push_back(query.value("high_te_limit").toDouble());
						coeff_vec.push_back(query.value("low_signal_limit").toDouble());
						coeff_vec.push_back(query.value("high_signal_limit").toDouble());
						coeff_vec.push_back(query.value("c_coeff_num").toDouble());
						coeff_vec.push_back(query.value("a_coeff_num").toDouble());
						coeff_map.insert(elem_type, coeff_vec);
						//qDebug() << "value = " << elem_type << ":" << coeff_vec;
					}
					//qDebug() << coeff_map;
				}
				else
				{
					qDebug() << "fill_temp_element_coeff_map: db query failed!\n";
				}
			}
			else
			{
				qDebug() << "fill_temp_element_coeff_map: db open failed!\n";
			}

			if (db_.isOpen())
			{
				db_.close();
			}
		}
		catch (const std::exception& exp)
		{
			cerr << "load_its90_coeff_map failed: " << exp.what() << "\n";
		}
	}

	void NSEng_Info_DB::load_te_limit_table(QHash<QString, QVector<double> > &te_limit_table)
	{
		try
		{
			if (db_.open())
			{
				QSqlQuery query(db_);
				QString query_str = QString("SELECT * FROM te_limit_tbl");
				query.prepare(query_str);
				if (query.exec())
				{
					int id = 0;
					while (query.next())
					{
						QString te_type = query.value("te_type").toString();
						QVector<double> limit_vec;
						limit_vec.push_back(query.value("low_te_limit").toDouble());
						limit_vec.push_back(query.value("high_te_limit").toDouble());
						limit_vec.push_back(query.value("low_signal_limit").toDouble());
						limit_vec.push_back(query.value("high_signal_limit").toDouble());
						te_limit_table.insert(te_type, limit_vec);
						//qDebug() << "value = " << te_type << ":" << limit_vec;
					}
					//qDebug() << te_limit_table;
				}
				else
				{
					qDebug() << "fill_te_limit_table: db query failed!\n";
				}
			}
			else
			{
				qDebug() << "fill_te_limit_table: db open failed!\n";
			}

			if (db_.isOpen())
			{
				db_.close();
			}
		}
		catch (const std::exception& exp)
		{
			cerr << "load_its90_coeff_map failed: " << exp.what() << "\n";
		}
	}
}