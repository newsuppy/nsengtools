#include "te_sensor/thermo_couple_calculator.h"
#include "te_sensor/temperature_scale_convertor.h"
#include <QVector>
#include <QDebug>
#include <cmath>

namespace npt
{
	Thermo_Couple_Calculator::Thermo_Couple_Calculator()
	{
		eng_info_db_.fill_temp_element_coeff_map(coefficient_map_, "TC%");
		eng_info_db_.load_its90_coeff_map(its90_coeff_map_, "TC%");
		eng_info_db_.load_te_limit_table(te_limit_tbl_);

		static const QChar ctemp_unicode_str[] = { 0x2103 };
		ctemp_unit_ = QString::fromRawData(ctemp_unicode_str, 1);
		static const QChar ftemp_unicode_str[] = { 0x2109 };
		ftemp_unit_ = QString::fromRawData(ftemp_unicode_str, 1);
	}

	QString Thermo_Couple_Calculator::gen_tc_calc_type(const QString& temperature_element, const QString& temperature_scale, const QString& calc_method)
	{
		qDebug() << temperature_element << "|" << temperature_scale;
		if ((temperature_element == ("B")) && (temperature_scale == ("Celsius")) && (calc_method == ("5thOrderPoly")))
		{
			return "TC_B_5thOrder_C";
		}
		else if ((temperature_element == ("B")) && (temperature_scale == ("Fahrenheit")) && (calc_method == ("5thOrderPoly")))
		{
			return "TC_B_5thOrder_F";
		}
		if ((temperature_element == ("E")) && (temperature_scale == ("Celsius")) && (calc_method == ("5thOrderPoly")))
		{
			return "TC_E_5thOrder_C";
		}
		else if ((temperature_element == ("E")) && (temperature_scale == ("Fahrenheit")) && (calc_method == ("5thOrderPoly")))
		{
			return "TC_E_5thOrder_F";
		}
		if ((temperature_element == ("J")) && (temperature_scale == ("Celsius")) && (calc_method == ("5thOrderPoly")))
		{
			return "TC_J_5thOrder_C";
		}
		else if ((temperature_element == ("J")) && (temperature_scale == ("Fahrenheit")) && (calc_method == ("5thOrderPoly")))
		{
			return "TC_J_5thOrder_F";
		}
		if ((temperature_element == ("K")) && (temperature_scale == ("Celsius")) && (calc_method == ("5thOrderPoly")))
		{
			return "TC_K_5thOrder_C";
		}
		else if ((temperature_element == ("K")) && (temperature_scale == ("Fahrenheit")) && (calc_method == ("5thOrderPoly")))
		{
			return "TC_K_5thOrder_F";
		}
		if ((temperature_element == ("R")) && (temperature_scale == ("Celsius")) && (calc_method == ("5thOrderPoly")))
		{
			return "TC_R_5thOrder_C";
		}
		else if ((temperature_element == ("R")) && (temperature_scale == ("Fahrenheit")) && (calc_method == ("5thOrderPoly")))
		{
			return "TC_R_5thOrder_F";
		}
		if ((temperature_element == ("S")) && (temperature_scale == ("Celsius")) && (calc_method == ("5thOrderPoly")))
		{
			return "TC_S_5thOrder_C";
		}
		else if ((temperature_element == ("S")) && (temperature_scale == ("Fahrenheit")) && (calc_method == ("5thOrderPoly")))
		{
			return "TC_S_5thOrder_F";
		}
		if ((temperature_element == ("T")) && (temperature_scale == ("Celsius")) && (calc_method == ("5thOrderPoly")))
		{
			return "TC_T_5thOrder_C";
		}
		else if ((temperature_element == ("T")) && (temperature_scale == ("Fahrenheit")) && (calc_method == ("5thOrderPoly")))
		{
			return "TC_T_5thOrder_F";
		}
		else if (calc_method == ("ITS-90Coeff"))
		{
			return "ITS-90Coeff";
		}
		else
		{
			return "UNKNOWN";
		}
	}

	void Thermo_Couple_Calculator::update_calc_method(const QString& calc_method)
	{
		calc_method_ = calc_method;
		this->tc_calc_type_ = this->gen_tc_calc_type(this->temperature_element_, this->temperature_scale_, this->calc_method_);
	}

	const QString& Thermo_Couple_Calculator::get_current_calc_method() const
	{
		return this->calc_method_;
	}

	void Thermo_Couple_Calculator::switch_calc_method()
	{
		if (this->calc_method_ == "ITS-90Coeff")
			this->update_calc_method("5thOrderPoly");//"ITS-90Coeff" "IEC60751"
		else
			this->update_calc_method("ITS-90Coeff");
	}

	double Thermo_Couple_Calculator::calc_temperature(double source_signal, double cjc_temp, const QString& tc_calc_type,
		const QString& temperature_element, const QString& temperature_scale, const QString& calc_method)
	{
		cjc_temp_ = cjc_temp;
		tc_calc_type_ = tc_calc_type;
		temperature_element_ = temperature_element;
		temperature_scale_ = temperature_scale;
		calc_method_ = calc_method;

		mv2t_calc_info_.clear();
		mv2t_calc_info_.append("mV -> temperature.\n");
		mv2t_calc_info_.append("TC Type: ");
		mv2t_calc_info_.append(temperature_element);
		mv2t_calc_info_.append("; ");
		mv2t_calc_info_.append(temperature_scale);
		mv2t_calc_info_.append("; \nsignal: ");
		mv2t_calc_info_.append(QString("%1").arg(source_signal));
		mv2t_calc_info_.append(" mV; \nCJC Temp: ");
		mv2t_calc_info_.append(QString("%1").arg(cjc_temp));
		QString element_tag = ("TC_");
		element_tag.append(temperature_element);
		double result = 0;
		QVector<double> coeff_array;

		if (calc_method == ("5thOrderPoly")) 
		{
			mv2t_calc_info_.append("\n5thOrderPoly method.\n");
			coeff_array = coefficient_map_[tc_calc_type];
			double cjc_value = (cjc_temp * coeff_array[7]) + coeff_array[6];
			mv2t_calc_info_.append("CJC mV: ");
			mv2t_calc_info_.append(QString("%1").arg(cjc_value));
			double compensated_value = source_signal / 1000.0 + cjc_value;
			result = coeff_array[0] + coeff_array[1] * compensated_value + coeff_array[2] * compensated_value * compensated_value
				+ coeff_array[3] * pow(compensated_value, 3) + coeff_array[4] * pow(compensated_value, 4) + coeff_array[5] * pow(compensated_value, 5);
			mv2t_calc_info_.append("\nT=sum(i=0->n)c_i*t^i\n");
			int c_num_coeff = 6;
			for (int c_idx = 0; c_idx < c_num_coeff; ++c_idx) {
				mv2t_calc_info_.append("c");
				mv2t_calc_info_.append(QString("%1").arg(c_idx));
				mv2t_calc_info_.append("=");
				mv2t_calc_info_.append(QString("%1").arg(coeff_array[c_idx]));
				mv2t_calc_info_.append("\n");
			}
			mv2t_calc_info_.append("CJC(mv) = CJC(T)*c7 + c6\n");
			mv2t_calc_info_.append("c6");
			mv2t_calc_info_.append("=");
			mv2t_calc_info_.append(QString("%1").arg(coeff_array[6]));
			mv2t_calc_info_.append("\n");
			mv2t_calc_info_.append("c7");
			mv2t_calc_info_.append("=");
			mv2t_calc_info_.append(QString("%1").arg(coeff_array[7]));
			mv2t_calc_info_.append("\n");
		}
		else
		{
			mv2t_calc_info_.append("\nITS-90 coefficient method.\n");
			element_tag.append("_ITS90_MV2C_");

			double cjc_temp_fix = cjc_temp;
			if (temperature_scale.contains("Fahrenheit"))
			{
				cjc_temp_fix = Temperature_Scale_Converter::convert_fahrenheit2celsius(result);
				mv2t_calc_info_.append(QString("\nConvert %2->%1, The CJC Temp(%1) is:").arg(ctemp_unit_).arg(ftemp_unit_));
				mv2t_calc_info_.append(QString("%1").arg(cjc_temp_fix));
				mv2t_calc_info_.append("\n");
			}

			int max_suffix = 5; //multiple coeff arry for different source range
			QString search_tag = "Empty";
			for (int suffix = 1; suffix < max_suffix; ++suffix)
			{
				search_tag = element_tag + QString("%1").arg(suffix);

				coeff_array = its90_coeff_map_[search_tag];
				if (coeff_array.size() < 1) {
					// find previous coeff_array
					--suffix;
					search_tag = element_tag + QString("%1").arg(suffix);
					coeff_array = its90_coeff_map_[search_tag];
					break;
				}
				else {
					double low_signal_limit = coeff_array[its90_low_signal_limit_idx];
					double high_signal_limit = coeff_array[its90_high_signal_limit_idx];
					if (source_signal < low_signal_limit) {
						break;
					}

					if ((source_signal >= low_signal_limit) && (source_signal <= high_signal_limit)) {
						break;
					}
				}
			}
			if (coeff_array.size() > 1) {
				int c_num_coeff = (int)coeff_array[its90_c_coeff_num_idx];
				int a_num_coeff = (int)coeff_array[its90_a_coeff_num_idx];

				double cjc_value = calc_source_signal_for_cjc(cjc_temp_fix, temperature_element, "Celsius", calc_method);
				double compensated_value = source_signal + cjc_value;
				mv2t_calc_info_.append("CJC mV: ");
				mv2t_calc_info_.append(QString("%1").arg(cjc_value));
				for (int c_idx = 0; c_idx < c_num_coeff; ++c_idx) {
					result += (coeff_array[c_idx] * pow(compensated_value, c_idx));
				}

				mv2t_calc_info_.append("\nT=sum(i=0->n)c_i*t^i\n");
				for (int c_idx = 0; c_idx < c_num_coeff; ++c_idx) {
					mv2t_calc_info_.append("c");
					mv2t_calc_info_.append(QString("%1").arg(c_idx));
					mv2t_calc_info_.append("=");
					mv2t_calc_info_.append(QString("%1").arg(coeff_array[c_idx]));
					mv2t_calc_info_.append("\n");
				}
			}
			else
			{
				mv2t_calc_info_.append("Could not find corrsponding coeff array! ");
			}

			if (temperature_scale.contains("Fahrenheit"))
			{
				mv2t_calc_info_.append(QString("Convert Celsius %1C to Fahrenheit.").arg(result));
				result = Temperature_Scale_Converter::convert_celsius2fahrenheit(result);
			}
		}
		mv2t_calc_info_.append("Result:");
		mv2t_calc_info_.append(QString("%1").arg(result));
		
		return result;
	}

	double Thermo_Couple_Calculator::calc_temperature(double source_signal)
	{
		QString element_tag = ("TC_");
		element_tag.append(temperature_element_);
		double result = 0;
		QVector<double> coeff_array;

		if (calc_method_ == ("5thOrderPoly"))
		{
			coeff_array = coefficient_map_[tc_calc_type_];
			if (coeff_array.size() < 1)
			{
				qDebug() << "Null coeff_array";
				double const NAN_d = std::numeric_limits<double>::quiet_NaN();
				return NAN_d;
			}
			//qDebug() << "tc_calc_type_:" << tc_calc_type_ << " temperature_element_:" << temperature_element_ << " temperature_scale_:" << temperature_scale_ << " calc_method_:" << calc_method_;
			double cjc_value = (cjc_temp_ * coeff_array[7]) + coeff_array[6];
			double compensated_value = source_signal / 1000.0 + cjc_value;
			result = coeff_array[0] + coeff_array[1] * compensated_value + coeff_array[2] * compensated_value * compensated_value
				+ coeff_array[3] * pow(compensated_value, 3) + coeff_array[4] * pow(compensated_value, 4) + coeff_array[5] * pow(compensated_value, 5);
		}
		else
		{
			element_tag.append("_ITS90_MV2C_");

			double cjc_temp_fix = cjc_temp_;
			if (temperature_scale_.contains("Fahrenheit"))
			{
				cjc_temp_fix = Temperature_Scale_Converter::convert_fahrenheit2celsius(result);
			}

			int max_suffix = 5; //multiple coeff arry for different source range
			QString search_tag = "Empty";
			for (int suffix = 1; suffix < max_suffix; ++suffix)
			{
				search_tag = element_tag + QString("%1").arg(suffix);

				coeff_array = its90_coeff_map_[search_tag];
				if (coeff_array.size() < 1) {
					// find previous coeff_array
					--suffix;
					search_tag = element_tag + QString("%1").arg(suffix);
					coeff_array = its90_coeff_map_[search_tag];
					break;
				}
				else {
					double low_signal_limit = coeff_array[its90_low_signal_limit_idx];
					double high_signal_limit = coeff_array[its90_high_signal_limit_idx];
					if (source_signal < low_signal_limit) {
						break;
					}

					if ((source_signal >= low_signal_limit) && (source_signal <= high_signal_limit)) {
						break;
					}
				}
			}
			if (coeff_array.size() > 1) {
				int c_num_coeff = (int)coeff_array[its90_c_coeff_num_idx];
				int a_num_coeff = (int)coeff_array[its90_a_coeff_num_idx];

				double cjc_value = calc_source_signal_for_cjc(cjc_temp_fix, temperature_element_, "Celsius", calc_method_);
				double compensated_value = source_signal + cjc_value;
				mv2t_calc_info_.append("CJC mV: ");
				mv2t_calc_info_.append(QString("%1").arg(cjc_value));
				for (int c_idx = 0; c_idx < c_num_coeff; ++c_idx) {
					result += (coeff_array[c_idx] * pow(compensated_value, c_idx));
				}
			}
			else
			{
				//
			}

			if (temperature_scale_.contains("Fahrenheit"))
			{
				result = Temperature_Scale_Converter::convert_celsius2fahrenheit(result);
			}
		}

		return result;
	}

	double Thermo_Couple_Calculator::calc_source_signal(double temperature, double cjc_temp,
		const QString& temperature_element, const QString& temperature_scale, const QString& calc_method)
	{
		cjc_temp_ = cjc_temp;
		temperature_element_ = temperature_element;
		temperature_scale_ = temperature_scale;
		calc_method_ = calc_method;

		t2mv_calc_info_.clear();
		t2mv_calc_info_.append("temperature -> mV.\n");
		t2mv_calc_info_.append("TC Type: ");
		t2mv_calc_info_.append(temperature_element);
		t2mv_calc_info_.append("; ");
		t2mv_calc_info_.append(temperature_scale);
		t2mv_calc_info_.append("; temperature: ");
		t2mv_calc_info_.append(QString("%1").arg(temperature));

		double temperature_fix = temperature;
		if (temperature_scale.contains("Fahrenheit"))
		{
			temperature_fix = Temperature_Scale_Converter::convert_fahrenheit2celsius(temperature);
			t2mv_calc_info_.append(QString("\nConvert %2->%1, The T(%1) is:").arg(ctemp_unit_).arg(ftemp_unit_));
			t2mv_calc_info_.append(QString("%1").arg(temperature_fix));
		}

		QString element_tag = ("TC_");
		element_tag.append(temperature_element);
		double result = 0;
		QVector<double> coeff_array;

		t2mv_calc_info_.append("\nITS-90 coefficient method only.");
		element_tag.append("_ITS90_C2MV_");

		int max_suffix = 5;
		QString search_tag = "Empty";
		for (int suffix = 1; suffix < max_suffix; ++suffix)
		{
			search_tag = element_tag + QString("%1").arg(suffix);

			coeff_array = its90_coeff_map_[search_tag];
			if (coeff_array.size() < 1) {
				// find previous coeff_array
				--suffix;
				search_tag = element_tag + QString("%1").arg(suffix);
				coeff_array = its90_coeff_map_[search_tag];
				break;
			}
			else {
				double low_te_limit = coeff_array[its90_low_te_limit_idx];
				double high_te_limit = coeff_array[its90_high_te_limit_idx];
				if (temperature_fix < low_te_limit) {
					break;
				}

				if ((temperature_fix >= low_te_limit) && (temperature_fix <= high_te_limit)) {
					break;
				}
			}
		}
		if (coeff_array.size() > 1) {
			int c_num_coeff = (int)coeff_array[its90_c_coeff_num_idx];
			int a_num_coeff = (int)coeff_array[its90_a_coeff_num_idx];
			for (int c_idx = 0; c_idx < c_num_coeff; ++c_idx) {
				result += (coeff_array[c_idx] * pow(temperature_fix, c_idx));
			}
			int fix_a_num = 3;
			if (a_num_coeff == fix_a_num)
			{
				result += coeff_array[its90_a0_idx] * exp(coeff_array[its90_a1_idx] *
					(temperature_fix - coeff_array[its90_a2_idx]) * (temperature_fix - coeff_array[its90_a2_idx]));
				t2mv_calc_info_.append("\nE=sum(i=0->n)c_i*E^i+a0*exp(a1*(T-a2)^2)\n");
			}
			else
			{
				t2mv_calc_info_.append("\nE=sum(i=0->n)c_i*E^i\n");
			}
			for (int c_idx = 0; c_idx < c_num_coeff; ++c_idx) {
				t2mv_calc_info_.append("c");
				t2mv_calc_info_.append(QString("%1").arg(c_idx));
				t2mv_calc_info_.append("=");
				t2mv_calc_info_.append(QString("%1").arg(coeff_array[c_idx]));
				t2mv_calc_info_.append("\n");
			}
			for (int a_idx = 0; a_idx < a_num_coeff; ++a_idx) {
				t2mv_calc_info_.append("a");
				t2mv_calc_info_.append(QString("%1").arg(a_idx));
				t2mv_calc_info_.append("=");
				t2mv_calc_info_.append(QString("%1").arg(coeff_array[its90_a0_idx + a_idx]));
				t2mv_calc_info_.append("\n");
			}
		}

		t2mv_calc_info_.append("Result(uncompensatedValue):");
		t2mv_calc_info_.append(QString("%1").arg(result));
		//Log.d("ITS90", searchTag.toString());

		double cjc_temp_fix = cjc_temp;
		if (temperature_scale.contains("Fahrenheit"))
		{
			cjc_temp_fix = Temperature_Scale_Converter::convert_fahrenheit2celsius(cjc_temp);
			t2mv_calc_info_.append(QString("\nConvert %2->%1, The CJC Temp(%1) is: ").arg(ctemp_unit_).arg(ftemp_unit_));
			t2mv_calc_info_.append(QString("%1").arg(cjc_temp_fix));
		}

		double cjc_value = calc_source_signal_for_cjc(cjc_temp_fix, temperature_element, "Celsius", calc_method);
		result = result - cjc_value;
		t2mv_calc_info_.append("\nCJC mV: ");
		t2mv_calc_info_.append(QString("%1").arg(cjc_value));

		t2mv_calc_info_.append("\nResult(compensatedValue):");
		t2mv_calc_info_.append(QString("%1").arg(result));
		//Log.d("ITS90", searchTag.toString());

		return result;
	}

	double Thermo_Couple_Calculator::calc_source_signal(double temperature)
	{
		double temperature_fix = temperature;
		if (temperature_scale_.contains("Fahrenheit"))
		{
			temperature_fix = Temperature_Scale_Converter::convert_fahrenheit2celsius(temperature);
		}

		QString element_tag = ("TC_");
		element_tag.append(temperature_element_);
		double result = 0;
		QVector<double> coeff_array;

		element_tag.append("_ITS90_C2MV_");

		int max_suffix = 5;
		QString search_tag = "Empty";
		for (int suffix = 1; suffix < max_suffix; ++suffix)
		{
			search_tag = element_tag + QString("%1").arg(suffix);

			coeff_array = its90_coeff_map_[search_tag];
			if (coeff_array.size() < 1) {
				// find previous coeff_array
				--suffix;
				search_tag = element_tag + QString("%1").arg(suffix);
				coeff_array = its90_coeff_map_[search_tag];
				break;
			}
			else {
				double low_te_limit = coeff_array[its90_low_te_limit_idx];
				double high_te_limit = coeff_array[its90_high_te_limit_idx];
				if (temperature_fix < low_te_limit) {
					break;
				}

				if ((temperature_fix >= low_te_limit) && (temperature_fix <= high_te_limit)) {
					break;
				}
			}
		}
		if (coeff_array.size() > 1) {
			int c_num_coeff = (int)coeff_array[its90_c_coeff_num_idx];
			int a_num_coeff = (int)coeff_array[its90_a_coeff_num_idx];
			for (int c_idx = 0; c_idx < c_num_coeff; ++c_idx) {
				result += (coeff_array[c_idx] * pow(temperature_fix, c_idx));
			}
			int fix_a_num = 3;
			if (a_num_coeff == fix_a_num)
			{
				result += coeff_array[its90_a0_idx] * exp(coeff_array[its90_a1_idx] *
					(temperature_fix - coeff_array[its90_a2_idx]) * (temperature_fix - coeff_array[its90_a2_idx]));
			}
			else
			{
				// Nothing
			}
		}

		double cjc_temp_fix = cjc_temp_;
		if (temperature_scale_.contains("Fahrenheit"))
		{
			cjc_temp_fix = Temperature_Scale_Converter::convert_fahrenheit2celsius(cjc_temp_);
		}

		double cjc_value = calc_source_signal_for_cjc(cjc_temp_fix, temperature_element_, "Celsius", calc_method_);
		result = result - cjc_value;

		return result;
	}

	double Thermo_Couple_Calculator::calc_source_signal_for_cjc(double temperature,
		const QString& temperature_element, const QString& temperature_scale, const QString& calc_method)
	{
		double temperature_fix = temperature;
		if (temperature_scale.contains("Fahrenheit"))
		{
			temperature_fix = Temperature_Scale_Converter::convert_fahrenheit2celsius(temperature);
		}

		QString element_tag = ("TC_");
		element_tag.append(temperature_element);
		double result = 0;
		QVector<double> coeff_array;

		element_tag.append("_ITS90_C2MV_");

		int max_suffix = 5;
		QString search_tag = "Empty";
		for (int suffix = 1; suffix < max_suffix; ++suffix)
		{
			search_tag = element_tag + QString("%1").arg(suffix);

			coeff_array = its90_coeff_map_[search_tag];
			if (coeff_array.size() < 1) {
				// find previous coeff_array
				--suffix;
				search_tag = element_tag + QString("%1").arg(suffix);
				coeff_array = its90_coeff_map_[search_tag];
				break;
			}
			else {
				double low_te_limit = coeff_array[its90_low_te_limit_idx];
				double high_te_limit = coeff_array[its90_high_te_limit_idx];
				if (temperature_fix < low_te_limit) {
					break;
				}

				if ((temperature_fix >= low_te_limit) && (temperature_fix <= high_te_limit)) {
					break;
				}
			}
		}
		if (coeff_array.size() > 1) {
			int c_num_coeff = (int)coeff_array[its90_c_coeff_num_idx];
			int a_num_coeff = (int)coeff_array[its90_a_coeff_num_idx];
			for (int c_idx = 0; c_idx < c_num_coeff; ++c_idx) {
				result += (coeff_array[c_idx] * pow(temperature_fix, c_idx));
			}
			int fix_a_num = 3;
			if (a_num_coeff == fix_a_num)
			{
				result += coeff_array[its90_a0_idx] * exp(coeff_array[its90_a1_idx] *
					(temperature_fix - coeff_array[its90_a2_idx]) * (temperature_fix - coeff_array[its90_a2_idx]));
			}
			else
			{
				//
			}
		}

		return result;
	}

	const QString& Thermo_Couple_Calculator::get_mv2t_calc_info() const
	{
		return mv2t_calc_info_;
	}

	const QString& Thermo_Couple_Calculator::get_t2mv_calc_info() const
	{
		return t2mv_calc_info_;
	}

	double Thermo_Couple_Calculator::get_low_signal_limit() const
	{
		QVector<double> limit_array;
		limit_array = te_limit_tbl_[temperature_element_];
		const int array_min_size = 4;
		//qDebug() << "limit_array: " << limit_array;
		if (limit_array.size() < array_min_size)
		{
			qDebug() << "Null coeff_array";
			double const NAN_d = std::numeric_limits<double>::quiet_NaN();
			return NAN_d;
		}
		const int low_signal_limit_pos = 2;
		return limit_array[low_signal_limit_pos];
	}

	double Thermo_Couple_Calculator::get_high_signal_limit() const
	{
		QVector<double> limit_array;
		limit_array = te_limit_tbl_[temperature_element_];
		const int array_min_size = 4;
		//qDebug() << "limit_array: " << limit_array;
		if (limit_array.size() < array_min_size)
		{
			qDebug() << "Null coeff_array";
			double const NAN_d = std::numeric_limits<double>::quiet_NaN();
			return NAN_d;
		}
		const int high_signal_limit_pos = 3;
		return limit_array[high_signal_limit_pos];
	}

	double Thermo_Couple_Calculator::get_low_temperature_limit() const
	{
		QVector<double> limit_array;
		limit_array = te_limit_tbl_[temperature_element_];
		const int array_min_size = 4;
		qDebug() << "limit_array: " << limit_array;
		if (limit_array.size() < array_min_size)
		{
			qDebug() << "Null coeff_array";
			double const NAN_d = std::numeric_limits<double>::quiet_NaN();
			return NAN_d;
		}
		const int low_te_limit_pos = 0;
		return limit_array[low_te_limit_pos];
	}

	double Thermo_Couple_Calculator::get_high_temperature_limit() const
	{
		QVector<double> limit_array;
		limit_array = te_limit_tbl_[temperature_element_];
		const int array_min_size = 4;
		//qDebug() << "limit_array: " << limit_array;
		if (limit_array.size() < array_min_size)
		{
			qDebug() << "Null coeff_array";
			double const NAN_d = std::numeric_limits<double>::quiet_NaN();
			return NAN_d;
		}
		const int high_te_limit_pos = 1;
		return limit_array[high_te_limit_pos];
	}

	const QString& Thermo_Couple_Calculator::get_temperature_scale() const
	{
		return this->temperature_scale_;
	}
}