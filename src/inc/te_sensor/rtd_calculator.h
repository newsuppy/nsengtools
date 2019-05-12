/* RTD_Calculator Header.
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

#ifndef NPT_RTD_CALCULATOR_H
#define NPT_RTD_CALCULATOR_H

#include <QString>
#include <QHash>
#include <QVector>
#include "nseng_info_db.h"

namespace npt
{
	class RTD_Calculator
	{
	public:
		RTD_Calculator();

		QString gen_rtd_calc_type(const QString& temperature_element, const QString& temperature_scale, const QString& calc_method);
		void update_calc_method(const QString& calc_method);
		const QString& get_current_calc_method() const;
		void switch_calc_method();

		double calc_temperature(double source_ohm, const QString& rtd_calc_type,
			const QString& temperature_element, const QString& temperature_scale, const QString& calc_method);
		double calc_temperature(double source_ohm);

		double calc_source_ohm(double temperature, const QString& temperature_element, 
			const QString& temperature_scale, const QString& calc_method);
		double calc_source_ohm(double temperature);

		const QString& get_ohm2t_calc_info() const;
		const QString& get_t2ohm_calc_info() const;

		const QString& get_temperature_scale() const;

	private:
		QHash<QString, QVector<double> > coefficient_map_;
		NSEng_Info_DB eng_info_db_;

		QString ftemp_unit_;
		QString ctemp_unit_;
		QString ohm2t_calc_info_;
		QString t2ohm_calc_info_;

		QString rtd_calc_type_;
		QString temperature_element_;
		QString temperature_scale_;
		QString calc_method_;
	};
}

#endif