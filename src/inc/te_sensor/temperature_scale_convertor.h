/* Temperature_Scale_Converter Header.
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

#ifndef NPT_TEMPERATURE_SCALE_CONVERTER_H
#define NPT_TEMPERATURE_SCALE_CONVERTER_H

namespace npt
{
	class Temperature_Scale_Converter
	{
	public:
		static double convert_celsius2fahrenheit(double celsius_degree)
		{
			return (celsius_degree) * 1.8 + 32.0;
		}

		static double convert_fahrenheit2celsius(double fahrenheit_degree)
		{
			return (fahrenheit_degree - 32.0) / 1.8;
		}
	};
}

#endif
