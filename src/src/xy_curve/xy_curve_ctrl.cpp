/* Pt_Graphics_Ctrl Body.
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

#include "xy_curve/xy_curve_ctrl.h"
#include "transmitter/transmitter_calculator.h"
#include "te_sensor/rtd_calculator.h"
#include "te_sensor/thermo_couple_calculator.h"
#include "xy_curve/xy_curve_data.h"
#include <QDebug>
#include <QString>

namespace npt
{
	XY_Curve_Ctrl::XY_Curve_Ctrl()
	{}

	void XY_Curve_Ctrl::gen_transmitter_data(boost::shared_ptr<XY_Curve_Data> data, Transmitter_Calculator& trans_calc, double current_input)
	{
		double transmitter_low = 4;
		double transmitter_high = 20;
		double low_margin_pct = 0.1;
		double high_margin_pct = 0.1;
		double full_step = transmitter_high - transmitter_low;
		double range_low = transmitter_low - full_step * low_margin_pct;
		double current_val = transmitter_low - full_step * low_margin_pct;
		double range_high = transmitter_high + full_step * high_margin_pct;
		double end_val = transmitter_high + full_step * high_margin_pct;
		unsigned int pt_cnt = 100;// .0 * (1 + low_margin_pct + high_margin_pct);
		double inc_val = (range_high - range_low) / pt_cnt;
		data->enlarge_pt_data_series_1(pt_cnt);
		for (unsigned int pos=0;pos <= pt_cnt; current_val += inc_val,++pos)
		{
			data->update_pt_data_1_val(current_val, trans_calc.calc_output(current_val, trans_calc.get_low_limit(), trans_calc.get_high_limit()), pos);
			//qDebug() << "calc:" << current_val << "," << trans_calc.calc_output(current_val, trans_calc.get_low_limit(), trans_calc.get_high_limit()) << "," << pos;
		}
		data->update_current_pt_val(current_input, trans_calc.calc_output(current_input, trans_calc.get_low_limit(), trans_calc.get_high_limit()));
		
		data->set_x_axis_range(range_low, range_high);
		double low_limit = trans_calc.get_low_limit();
		double high_limit = trans_calc.get_high_limit();
		double output_range = high_limit - low_limit;
		double output_low = trans_calc.calc_output(range_low, trans_calc.get_low_limit(), trans_calc.get_high_limit());
		double output_high = trans_calc.calc_output(range_high, trans_calc.get_low_limit(), trans_calc.get_high_limit());
		if (output_low > output_high)
		{
			data->set_y_axis_range(output_high, output_low);
		}
		else
		{
			data->set_y_axis_range(output_low, output_high);
		}
	}
	
	void XY_Curve_Ctrl::gen_rtd_ohm2te_data(boost::shared_ptr<XY_Curve_Data> data, RTD_Calculator& rtd_calc, double current_input)
	{
		data->set_x_axis_label(tr("Source Signal(ohm)"));
		QString y_label = QString(tr("Temperature-"));
		y_label += rtd_calc.get_temperature_scale();
		data->set_y_axis_label("Temperature");

		double ohm_low = 0;
		double ohm_high = 500;
		if (current_input < ohm_low)
			ohm_low = current_input;
		if (current_input > ohm_high)
			ohm_high = current_input;

		double low_margin_pct = 0.1;
		double high_margin_pct = 0.1;
		double full_step = ohm_high - ohm_low;
		double range_low = ohm_low - full_step * low_margin_pct;
		double current_val = ohm_low - full_step * low_margin_pct;
		double range_high = ohm_high + full_step * high_margin_pct;
		double end_val = ohm_high + full_step * high_margin_pct;
		unsigned int pt_cnt = 100;// .0 * (1 + low_margin_pct + high_margin_pct);
		double inc_val = (range_high - range_low) / pt_cnt;
		data->enlarge_pt_data_series_1(pt_cnt);
		for (unsigned int pos = 0; pos <= pt_cnt; current_val += inc_val, ++pos)
		{
			data->update_pt_data_1_val(current_val, rtd_calc.calc_temperature(current_val),pos);
			//qDebug() << "calc:" << current_val << "," << trans_calc.calc_output(current_val, trans_calc.get_low_limit(), trans_calc.get_high_limit()) << "," << pos;
		}
		data->update_current_pt_val(current_input, rtd_calc.calc_temperature(current_input));

		data->enlarge_pt_data_series_2(pt_cnt);
		current_val = ohm_low - full_step * low_margin_pct;
		data->set_x_axis_range(range_low, range_high);
		double low_limit = rtd_calc.calc_temperature(ohm_low);
		double high_limit = rtd_calc.calc_temperature(ohm_high);
		double te_range = high_limit - low_limit;
		double te_low = rtd_calc.calc_temperature(range_low);
		double te_high = rtd_calc.calc_temperature(range_high);
		data->set_y_axis_range(te_low, te_high);

		qDebug() << "Current Calc Method is: " << rtd_calc.get_current_calc_method();
		data->set_title("RTD Ohm -> Temperature");
		data->set_pt_data_series_1_name(rtd_calc.get_current_calc_method());
		rtd_calc.switch_calc_method();
		data->set_pt_data_series_2_name(rtd_calc.get_current_calc_method());
		data->show_pt_data_series_2();
		for (unsigned int pos = 0; pos <= pt_cnt; current_val += inc_val, ++pos)
		{
			data->update_pt_data_2_val(current_val, rtd_calc.calc_temperature(current_val), pos);
			//qDebug() << "calc:" << current_val << "," << trans_calc.calc_output(current_val, trans_calc.get_low_limit(), trans_calc.get_high_limit()) << "," << pos;
		}
	}

	void XY_Curve_Ctrl::gen_rtd_te2ohm_data(boost::shared_ptr<XY_Curve_Data> data, RTD_Calculator& rtd_calc, double current_te)
	{
		QString x_label = QString(tr("Temperature-"));
		x_label += rtd_calc.get_temperature_scale();
		data->set_x_axis_label("Temperature");
		data->set_y_axis_label(tr("Source Signal(ohm)"));

		double te_low = -273.15;
		double te_high = 600;
		if (current_te < te_low)
			te_low = current_te;
		if (current_te > te_high)
			te_high = current_te;

		double low_margin_pct = 0.1;
		double high_margin_pct = 0.1;
		double full_step = te_high - te_low;
		double range_low = te_low - full_step * low_margin_pct;
		double current_val = te_low - full_step * low_margin_pct;
		double range_high = te_high + full_step * high_margin_pct;
		double end_val = te_high + full_step * high_margin_pct;
		unsigned int pt_cnt = 100;// .0 * (1 + low_margin_pct + high_margin_pct);
		double inc_val = (range_high - range_low) / pt_cnt;
		data->enlarge_pt_data_series_1(pt_cnt);
		for (unsigned int pos = 0; pos <= pt_cnt; current_val += inc_val, ++pos)
		{
			data->update_pt_data_1_val(current_val, rtd_calc.calc_source_ohm(current_val), pos);
		}
		data->update_current_pt_val(current_te, rtd_calc.calc_source_ohm(current_te));	
		current_val = te_low - full_step * low_margin_pct;
		data->set_x_axis_range(range_low, range_high);
		double low_limit = rtd_calc.calc_source_ohm(te_low);
		double high_limit = rtd_calc.calc_source_ohm(te_high);
		double te_range = high_limit - low_limit;
		double ohm_low = rtd_calc.calc_source_ohm(range_low);
		double ohm_high = rtd_calc.calc_source_ohm(range_high);
		data->set_y_axis_range(ohm_low, ohm_high);
		data->set_title("Temperature -> RTD Ohm");
		data->set_pt_data_series_1_name("IEC60751");
		data->set_pt_data_series_2_name("Nothing");
		data->hide_pt_data_series_2();
		/*data->enlarge_pt_data_series_2(pt_cnt);
		rtd_calc.switch_calc_method();
		for (unsigned int pos = 0; pos <= pt_cnt; current_val += inc_val, ++pos)
		{
			data->update_pt_data_2_val(current_val, rtd_calc.calc_source_ohm(current_val), pos);
		}*/
	}

	void XY_Curve_Ctrl::gen_tc_mv2te_data(boost::shared_ptr<XY_Curve_Data> data, Thermo_Couple_Calculator& tc_calc, double current_input)
	{
		data->set_x_axis_label(tr("Source Signal(mV)"));
		QString y_label = QString(tr("Temperature-"));
		y_label += tc_calc.get_temperature_scale();
		data->set_y_axis_label(y_label);
		double tc_low = tc_calc.get_low_signal_limit();
		double tc_high = tc_calc.get_high_signal_limit();
		if (current_input < tc_low)
			tc_low = current_input;
		if (current_input > tc_high)
			tc_high = current_input;

		double low_margin_pct = 0.01;
		double high_margin_pct = 0.01;
		double full_step = tc_high - tc_low;
		double range_low = tc_low - full_step * low_margin_pct;
		double current_val = tc_low - full_step * low_margin_pct;
		double range_high = tc_high + full_step * high_margin_pct;
		double end_val = tc_high + full_step * high_margin_pct;
		unsigned int pt_cnt = 100;// .0 * (1 + low_margin_pct + high_margin_pct);
		double inc_val = (range_high - range_low) / pt_cnt;
		data->enlarge_pt_data_series_1(pt_cnt);
		for (unsigned int pos = 0; pos <= pt_cnt; current_val += inc_val, ++pos)
		{
			data->update_pt_data_1_val(current_val, tc_calc.calc_temperature(current_val), pos);
			//qDebug() << "calc:" << current_val << "," << trans_calc.calc_output(current_val, trans_calc.get_low_limit(), trans_calc.get_high_limit()) << "," << pos;
		}
		data->update_current_pt_val(current_input, tc_calc.calc_temperature(current_input));
		
		current_val = tc_low - full_step * low_margin_pct;
		data->set_x_axis_range(range_low, range_high);
		double low_limit = tc_calc.calc_temperature(tc_low);
		double high_limit = tc_calc.calc_temperature(tc_high);
		double te_range = high_limit - low_limit;
		double te_low = tc_calc.calc_temperature(range_low);
		double te_high = tc_calc.calc_temperature(range_high);
		data->set_y_axis_range(te_low, te_high);

		data->enlarge_pt_data_series_2(pt_cnt);
		data->set_title("TC mv -> Temperature");
		data->set_pt_data_series_1_name(tc_calc.get_current_calc_method());
		tc_calc.switch_calc_method();
		data->set_pt_data_series_2_name(tc_calc.get_current_calc_method());
		data->show_pt_data_series_2();
		for (unsigned int pos = 0; pos <= pt_cnt; current_val += inc_val, ++pos)
		{
			data->update_pt_data_2_val(current_val, tc_calc.calc_temperature(current_val), pos);
			//qDebug() << "calc:" << current_val << "," << trans_calc.calc_output(current_val, trans_calc.get_low_limit(), trans_calc.get_high_limit()) << "," << pos;
		}
		//data->update_current_pt_val(current_input, tc_calc.calc_temperature_by_last_method(current_input,"5thOrderPoly"));	
	}

	void XY_Curve_Ctrl::gen_tc_te2mv_data(boost::shared_ptr<XY_Curve_Data> data, Thermo_Couple_Calculator& tc_calc, double current_te)
	{
		QString x_label = QString(tr("Temperature-"));
		x_label += tc_calc.get_temperature_scale();
		data->set_x_axis_label(x_label);
		data->set_y_axis_label(tr("Source Signal(mV)"));

		double te_low = tc_calc.get_low_temperature_limit();
		double te_high = tc_calc.get_high_temperature_limit();
		if (current_te < te_low)
			te_low = current_te;
		if (current_te > te_high)
			te_high = current_te;

		double low_margin_pct = 0.01;
		double high_margin_pct = 0.01;
		double full_step = te_high - te_low;
		double range_low = te_low - full_step * low_margin_pct;
		double current_val = te_low - full_step * low_margin_pct;
		double range_high = te_high + full_step * high_margin_pct;
		double end_val = te_high + full_step * high_margin_pct;
		unsigned int pt_cnt = 100;// .0 * (1 + low_margin_pct + high_margin_pct);
		double inc_val = (range_high - range_low) / pt_cnt;
		data->enlarge_pt_data_series_1(pt_cnt);
		for (unsigned int pos = 0; pos <= pt_cnt; current_val += inc_val, ++pos)
		{
			data->update_pt_data_1_val(current_val, tc_calc.calc_source_signal(current_val), pos);
			//qDebug() << "calc:" << current_val << "," << tc_calc.calc_source_signal(current_val) << "," << pos;
		}
		data->update_current_pt_val(current_te, tc_calc.calc_source_signal(current_te));
		
		current_val = te_low - full_step * low_margin_pct;
		data->set_x_axis_range(range_low, range_high);
		double low_limit = tc_calc.calc_source_signal(te_low);
		double high_limit = tc_calc.calc_source_signal(te_high);
		double te_range = high_limit - low_limit;
		double mv_low = tc_calc.calc_source_signal(range_low);
		double mv_high = tc_calc.calc_source_signal(range_high);
		data->set_y_axis_range(mv_low, mv_high);

		data->set_title("TC Temperature -> mv");
		data->set_pt_data_series_1_name("ITS-90Coeff");	
		data->set_pt_data_series_2_name("Nothing");
		data->hide_pt_data_series_2();
		//data->enlarge_pt_data_series_2(pt_cnt);
		//tc_calc.update_calc_method("5thOrderPoly");
		//for (unsigned int pos = 0; pos <= pt_cnt; current_val += inc_val, ++pos)
		{
		//	data->update_pt_data_2_val(current_val, tc_calc.calc_source_signal(current_val), pos);
			//qDebug() << "calc:" << current_val << "," << trans_calc.calc_output(current_val, trans_calc.get_low_limit(), trans_calc.get_high_limit()) << "," << pos;
		}
		//data->update_current_pt_val(current_input, tc_calc.calc_temperature_by_last_method(current_input,"5thOrderPoly"));	
	}
}