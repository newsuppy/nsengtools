/* Thermo_Couple_Calculator Header.
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

#ifndef NPT_THERMO_COUPLE_CALCULATOR_H
#define NPT_THERMO_COUPLE_CALCULATOR_H

#include <QString>
#include <QHash>
#include <QVector>
#include "nseng_info_db.h"

namespace npt
{
	class Thermo_Couple_Calculator 
	{
	public:
		Thermo_Couple_Calculator();

		QString gen_tc_calc_type(const QString& temperature_element, const QString& temperature_scale, const QString& calc_method);
		void update_calc_method(const QString& calc_method);
		const QString& get_current_calc_method() const;
		void switch_calc_method();

		double calc_temperature(double source_signal, double cjc_temp, const QString& tc_calc_type,
			const QString& temperature_element, const QString& temperature_scale, const QString& calc_method);
		double calc_temperature(double source_signal);

		double calc_source_signal(double temperature, double cjc_temp,
			const QString& temperature_element, const QString& temperature_scale, const QString& calc_method);
		double calc_source_signal(double temperature);

		double calc_source_signal_for_cjc(double temperature,
			const QString& temperature_element, const QString& temperature_scale, const QString& calc_method);

		const QString& get_mv2t_calc_info() const;
		const QString& get_t2mv_calc_info() const;

		double get_low_signal_limit() const;
		double get_high_signal_limit() const;

		double get_low_temperature_limit() const;
		double get_high_temperature_limit() const;

		const QString& get_temperature_scale() const;

	private:
		QHash<QString, QVector<double> > coefficient_map_;
		QHash<QString, QVector<double> > its90_coeff_map_;
		QHash<QString, QVector<double> > te_limit_tbl_;
		NSEng_Info_DB eng_info_db_;

		QString ftemp_unit_;
		QString ctemp_unit_;
		QString mv2t_calc_info_;
		QString t2mv_calc_info_;

		double c1, c2, c3, c4, c5, c6;

		const int its90_c0_idx = 0;
		const int its90_c1_idx = 1;
		const int its90_c2_idx = 2;
		const int its90_c3_idx = 3;
		const int its90_c4_idx = 4;
		const int its90_c5_idx = 5;
		const int its90_c6_idx = 6;
		const int its90_c7_idx = 7;
		const int its90_c8_idx = 8;
		const int its90_c9_idx = 9;
		const int its90_c10_idx = 10;
		const int its90_c11_idx = 11;
		const int its90_c12_idx = 12;
		const int its90_c13_idx = 13;
		const int its90_c14_idx = 14;
		const int its90_c15_idx = 15;
		const int its90_a0_idx = 16;
		const int its90_a1_idx = 17;
		const int its90_a2_idx = 18;
		const int its90_a3_idx = 19;
		const int its90_a4_idx = 20;
		const int its90_a5_idx = 21;
		const int its90_low_te_limit_idx = 22;
		const int its90_high_te_limit_idx = 23;
		const int its90_low_signal_limit_idx = 24;
		const int its90_high_signal_limit_idx = 25;
		const int its90_c_coeff_num_idx = 26;
		const int its90_a_coeff_num_idx = 27;

		double cjc_temp_;
		QString tc_calc_type_;
		QString temperature_element_;
		QString temperature_scale_;
		QString calc_method_;
	};
}

#endif