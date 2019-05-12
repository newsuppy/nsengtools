/* RTD_Calculator.
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

#include "te_sensor/rtd_calculator.h"
#include "te_sensor/temperature_scale_convertor.h"
#include <QVector>
#include <QDebug>

namespace npt
{
	RTD_Calculator::RTD_Calculator()
	{
		eng_info_db_.fill_temp_element_coeff_map(coefficient_map_, "RTD%");
		// c1,c2,c3,c4,c5,c6,c7,c8,temp_low,temp_high,ohm_low,ohm_high
		/*double[] RTD_PT100_5thOrder_C = {-242.36601,2.240231,0.0024929789,-8.8930401E-006,2.4698643E-008,-2.3161646E-011,0,0,-16,561,0,400};
		coefficientMap.put("RTD_PT100_5thOrder_C", RTD_PT100_5thOrder_C);
		double[] RTD_PT100_5thOrder_F = {-406.03751,4.1006289,0.0035849023,-0.000010650798,0.000000029814572,-0.000000000026676971,0,0,-4,1040,0,400};
		coefficientMap.put("RTD_PT100_5thOrder_F", RTD_PT100_5thOrder_F);*/
		static const QChar ctemp_unicode_str[] = { 0x2103 };
		ctemp_unit_ = QString::fromRawData(ctemp_unicode_str, 1);
		static const QChar ftemp_unicode_str[] = { 0x2109 };
		ftemp_unit_ = QString::fromRawData(ftemp_unicode_str, 1);
	}

	QString RTD_Calculator::gen_rtd_calc_type(const QString& temperature_element, const QString& temperature_scale, const QString& calc_method)
	{
		qDebug() << temperature_element << "|" << temperature_scale;
		if ((temperature_element == "RTD(Pt100)") && (temperature_scale == "Celsius"))
			//if ((temperatureElement.equals("RTD(Pt100)")) && (temperatureScale.equals("Celsius")) && (calcMethod.equals("5thOrderPoly")))
		{
			return "RTD_PT100_5thOrder_C";
		}
		else if ((temperature_element == "RTD(Pt100)") && (temperature_scale == "Fahrenheit"))
			//else if ((temperatureElement.equals("RTD(Pt100)")) && (temperatureScale.equals("Fahrenheit")) && (calcMethod.equals("5thOrderPoly")))
		{
			return "RTD_PT100_5thOrder_F";
		}
		else
		{
			return "UNKNOWN";
		}
	}

	void RTD_Calculator::update_calc_method(const QString& calc_method)
	{
		calc_method_ = calc_method;
		this->rtd_calc_type_ = this->gen_rtd_calc_type(this->temperature_element_, this->temperature_scale_, this->calc_method_);
	}

	const QString& RTD_Calculator::get_current_calc_method() const
	{
		return this->calc_method_;
	}

	void RTD_Calculator::switch_calc_method()
	{
		if (this->calc_method_ == "IEC60751")
			this->update_calc_method("5thOrderPoly");//"ITS-90Coeff" "IEC60751"
		else
			this->update_calc_method("IEC60751");
	}

	double RTD_Calculator::calc_temperature(double source_ohm, const QString& rtd_calc_type,
		const QString& temperature_element, const QString& temperature_scale, const QString& calc_method)
	{
		rtd_calc_type_ = rtd_calc_type;
		temperature_element_ = temperature_element;
		temperature_scale_ = temperature_scale;
		calc_method_ = calc_method;

		ohm2t_calc_info_.clear();
		ohm2t_calc_info_.append("Ohm -> temperature.\n");
		ohm2t_calc_info_.append("Type: ");
		ohm2t_calc_info_.append(temperature_element);
		ohm2t_calc_info_.append("; ");
		ohm2t_calc_info_.append(temperature_scale);
		ohm2t_calc_info_.append("\nResistance:");
		ohm2t_calc_info_.append(QString("%1").arg(source_ohm));
		double result = 0;
		if (calc_method == "5thOrderPoly")
		{
			ohm2t_calc_info_.append("\nUse 5thOrder method.\n");
			if (coefficient_map_.contains("RTD_PT100_5thOrder_C"))
			{
				QVector<double> coeff_array = coefficient_map_["RTD_PT100_5thOrder_C"];
				ohm2t_calc_info_.append(QString("%1").arg(coeff_array.size()));
				if (coeff_array.size() < 7) {
					qDebug() << "RTD-null Array:" << rtd_calc_type;
					return -1;
				}
				result = coeff_array[0] + coeff_array[1] * source_ohm + coeff_array[2] * source_ohm * source_ohm
					+ coeff_array[3] * pow(source_ohm, 3) + coeff_array[4] * pow(source_ohm, 4) + coeff_array[5] * pow(source_ohm, 5);
				ohm2t_calc_info_.append("\nT=sum(i=0->n)c_i*R^i");
				ohm2t_calc_info_.append("\n");
				int c_num_cnt = 6;
				for (int c_idx = 0; c_idx < c_num_cnt; ++c_idx) {
					ohm2t_calc_info_.append("c");
					ohm2t_calc_info_.append(QString("%1").arg(c_idx));
					ohm2t_calc_info_.append("=");
					ohm2t_calc_info_.append(QString("%1").arg(coeff_array[c_idx]));
					ohm2t_calc_info_.append("\n");
				}
			}
		}
		else // IEC60751
		{
			ohm2t_calc_info_.append("\nUse IEC60751 method.\n");
			QVector<double> coeff_array = coefficient_map_["RTD_PT100_IEC60751_R2T_C"];
			if (coeff_array.size() < 7) {
				qDebug() << "RTD-null Array:" << rtd_calc_type;
				return -1;
			}
			ohm2t_calc_info_.append(QString("%1").arg(coeff_array.size()));
			double numerator = source_ohm * (coeff_array[1] + source_ohm * (coeff_array[2] + source_ohm * (coeff_array[3] + coeff_array[4] * source_ohm)));
			double denominator = 1 + source_ohm * (coeff_array[5] + source_ohm * (coeff_array[6] + coeff_array[7] * source_ohm));
			ohm2t_calc_info_.append(QString("%1").arg(denominator));
			result = coeff_array[0] + numerator / denominator;
			ohm2t_calc_info_.append("\nT=c0+{R(c1+R(c2+R(c3+c4*R)))}/{1+R(c5+R(c6+c7*R))}");
			ohm2t_calc_info_.append("\n");
			int c_num_cnt = 8;
			for (int c_idx = 0; c_idx < c_num_cnt; ++c_idx) {
				ohm2t_calc_info_.append("c");
				ohm2t_calc_info_.append(QString("%1").arg(c_idx));
				ohm2t_calc_info_.append("=");
				ohm2t_calc_info_.append(QString("%1").arg(coeff_array[c_idx]));
				ohm2t_calc_info_.append("\n");
			}
		}
		qDebug() << "result:" << result;

		if (temperature_scale.contains("Fahrenheit"))
		{
			ohm2t_calc_info_.append(QString("Convert Celsius %1C to Fahrenheit.").arg(result));
			result = Temperature_Scale_Converter::convert_celsius2fahrenheit(result);
		}

		ohm2t_calc_info_.append(QString("Result: %1F").arg(result));
		
		return result;
	}

	double RTD_Calculator::calc_temperature(double source_ohm)
	{
		double result = 0;
		if (calc_method_ == "5thOrderPoly")
		{
			if (coefficient_map_.contains("RTD_PT100_5thOrder_C"))
			{
				QVector<double> coeff_array = coefficient_map_["RTD_PT100_5thOrder_C"];
				if (coeff_array.size() < 7) {
					qDebug() << "RTD-null Array:" << rtd_calc_type_;
					return -1;
				}
				result = coeff_array[0] + coeff_array[1] * source_ohm + coeff_array[2] * source_ohm * source_ohm
					+ coeff_array[3] * pow(source_ohm, 3) + coeff_array[4] * pow(source_ohm, 4) + coeff_array[5] * pow(source_ohm, 5);			
			}
		}
		else // IEC60751
		{
			QVector<double> coeff_array = coefficient_map_["RTD_PT100_IEC60751_R2T_C"];
			if (coeff_array.size() < 7) {
				qDebug() << "RTD-null Array:" << rtd_calc_type_;
				return -1;
			}
			double numerator = source_ohm * (coeff_array[1] + source_ohm * (coeff_array[2] + source_ohm * (coeff_array[3] + coeff_array[4] * source_ohm)));
			double denominator = 1 + source_ohm * (coeff_array[5] + source_ohm * (coeff_array[6] + coeff_array[7] * source_ohm));
			result = coeff_array[0] + numerator / denominator;
		}

		if (temperature_scale_.contains("Fahrenheit"))
		{
			result = Temperature_Scale_Converter::convert_celsius2fahrenheit(result);
		}

		return result;
	}

	double RTD_Calculator::calc_source_ohm(double temperature, const QString& temperature_element,
		const QString& temperature_scale, const QString& calc_method)
	{
		temperature_element_ = temperature_element;
		temperature_scale_ = temperature_scale;
		calc_method_ = calc_method;

		t2ohm_calc_info_.clear();
		t2ohm_calc_info_.append("temperature -> Ohm.\n");
		t2ohm_calc_info_.append("\nUse IEC60751 method.\n");
		QVector<double> coeff_array = coefficient_map_["RTD_PT100_IEC60751_T2R_C"];
		if (coeff_array.size() < 7) {
			qDebug() << "RTD-null Array:" << "RTD_PT100_IEC60751_T2R_C";
			return -1;
		}
		double result = 0;
		t2ohm_calc_info_.append(QString("%1").arg(coeff_array.size()));
		if (temperature <= 0)
		{
			result = coeff_array[0] * (1 + coeff_array[1] * temperature + coeff_array[2] * temperature*temperature
				+ coeff_array[3] * (temperature - 100.0)*temperature*temperature*temperature);//R(t) = R0 * (1+A*t+B*t^2+C*(t-100)*t^3) -200 ~ 0
		}
		else
		{
			result = coeff_array[0] * (1 + coeff_array[1] * temperature + coeff_array[2] * temperature*temperature);//R(t)=R0*(1+A*t+B*t^2) 0~850
		}

		t2ohm_calc_info_.append("\nR=R0*(1+A*T+B*T^2+C*(T-100)*T^3) -200C ~ 0C");
		t2ohm_calc_info_.append("\nR=R0*(1+A*T+B*T^2) 0C~850C");
		t2ohm_calc_info_.append("\n");
		int c_num_cnt = 8;
		for (int c_idx = 0; c_idx < c_num_cnt; ++c_idx) {
			t2ohm_calc_info_.append("c");
			t2ohm_calc_info_.append(QString("%1").arg(c_idx));
			t2ohm_calc_info_.append("=");
			t2ohm_calc_info_.append(QString("%1").arg(coeff_array[c_idx]));
			t2ohm_calc_info_.append("\n");
		}

		if (temperature_scale.contains("Fahrenheit"))
		{
			t2ohm_calc_info_.append(QString("Convert Celsius %1%2 to Fahrenheit.").arg(result).arg(ctemp_unit_));
			result = Temperature_Scale_Converter::convert_celsius2fahrenheit(result);;
		}

		t2ohm_calc_info_.append(QString("Result: %1%2").arg(result).arg(ftemp_unit_));
		
		return result;
	}

	double RTD_Calculator::calc_source_ohm(double temperature)
	{
		QVector<double> coeff_array = coefficient_map_["RTD_PT100_IEC60751_T2R_C"];
		if (coeff_array.size() < 7) {
			qDebug() << "RTD-null Array:" << "RTD_PT100_IEC60751_T2R_C";
			return -1;
		}
		double result = 0;
		if (temperature <= 0)
		{
			result = coeff_array[0] * (1 + coeff_array[1] * temperature + coeff_array[2] * temperature*temperature
				+ coeff_array[3] * (temperature - 100.0)*temperature*temperature*temperature);//R(t) = R0 * (1+A*t+B*t^2+C*(t-100)*t^3) -200 ~ 0
		}
		else
		{
			result = coeff_array[0] * (1 + coeff_array[1] * temperature + coeff_array[2] * temperature*temperature);//R(t)=R0*(1+A*t+B*t^2) 0~850
		}

		if (temperature_scale_.contains("Fahrenheit"))
		{
			result = Temperature_Scale_Converter::convert_celsius2fahrenheit(result);;
		}

		return result;
	}

	const QString& RTD_Calculator::get_ohm2t_calc_info() const
	{
		return ohm2t_calc_info_;
	}

	const QString& RTD_Calculator::get_t2ohm_calc_info() const
	{
		return t2ohm_calc_info_;
	}

	const QString& RTD_Calculator::get_temperature_scale() const
	{
		return this->temperature_scale_;
	}
}