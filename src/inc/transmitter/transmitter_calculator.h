/* Transmitter_Calculator Header.
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

#ifndef NPT_TRANSMITTER_CALCULATOR_H
#define NPT_TRANSMITTER_CALCULATOR_H

namespace npt
{
	class Transmitter_Calculator {
		
	public:
		Transmitter_Calculator();

		double calc_output(double source_signal, double low_limit, double high_limit);

		double calc_source_signal(double output, double low_limit, double high_limit);

		double set_gain_bias(double source_signal, double gain, double bias);

		double get_gain();

		double get_bias();

		double get_low_limit();

		double get_high_limit();

	private:
		double low_limit_;
		double high_limit_;
		double gain_;
		double bias_;
	};
}

#endif