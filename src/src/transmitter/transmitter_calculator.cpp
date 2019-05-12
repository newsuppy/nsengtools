#include "transmitter/transmitter_calculator.h"

namespace npt
{
	Transmitter_Calculator::Transmitter_Calculator() 
	{
		low_limit_ = 0.0;
		high_limit_ = 100.0;
		gain_ = 1.0;
		bias_ = 0.0;
	}

	// output = gain*input + bias
	double Transmitter_Calculator::calc_output(double source_signal, double low_limit, double high_limit) 
	{
		this->low_limit_ = low_limit;
		this->high_limit_ = high_limit;
		gain_ = (high_limit - low_limit) / 0.016;   // 20ma-4ma
		bias_ = low_limit - 0.004 * gain_;
		double output = gain_ * source_signal / 1000.0 + bias_;
		return output;
	}

	double Transmitter_Calculator::calc_source_signal(double output, double low_limit, double high_limit)
	{
		this->low_limit_ = low_limit;
		this->high_limit_ = high_limit;
		gain_ = (high_limit - low_limit) / 0.016;   // 20ma-4ma
		bias_ = low_limit - 0.004 * gain_;
		double sourceSignal = (output - bias_) / gain_ * 1000.0;
		return sourceSignal;
	}

	double Transmitter_Calculator::set_gain_bias(double source_signal, double gain, double bias) 
	{
		gain_ = gain;
		bias_ = bias;
		low_limit_ = gain_ * 0.004 + bias_;
		high_limit_ = gain_ * 0.02 + bias_;
		
		double output = gain_ * source_signal / 1000.0 + bias_;

		return output;
	}

	double Transmitter_Calculator::get_gain()
	{
		return gain_;
	}

	double Transmitter_Calculator::get_bias() 
	{
		return bias_;
	}

	double Transmitter_Calculator::get_low_limit()
	{ 
		return low_limit_; 
	}

	double Transmitter_Calculator::get_high_limit()
	{ 
		return high_limit_;
	}

}