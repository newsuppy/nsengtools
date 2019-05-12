/* XY_Curve_Ctrl Header.
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
along with NSPCTools; see the file COPYING.  If not, write to
the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
Boston, MA 02110-1301, USA.  */

#ifndef NPT_XY_CURVE_CTRL_H
#define NPT_XY_CURVE_CTRL_H

#include <boost/shared_ptr.hpp>
#include <QList>
#include <QObject>

namespace npt
{
	class XY_Curve_Data;
	class Transmitter_Calculator;
	class RTD_Calculator;
	class Thermo_Couple_Calculator;
	class XY_Curve_Ctrl : public QObject
	{
	public:
		XY_Curve_Ctrl();

		void gen_transmitter_data(boost::shared_ptr<XY_Curve_Data> data, Transmitter_Calculator& trans_calc, double current_input);

		void gen_rtd_ohm2te_data(boost::shared_ptr<XY_Curve_Data> data, RTD_Calculator& rtd_calc, double current_input);
		void gen_rtd_te2ohm_data(boost::shared_ptr<XY_Curve_Data> data, RTD_Calculator& rtd_calc, double current_te);

		void gen_tc_mv2te_data(boost::shared_ptr<XY_Curve_Data> data, Thermo_Couple_Calculator& tc_calc, double current_input);
		void gen_tc_te2mv_data(boost::shared_ptr<XY_Curve_Data> data, Thermo_Couple_Calculator& tc_calc, double current_te);

	private:

	};
}

#endif
