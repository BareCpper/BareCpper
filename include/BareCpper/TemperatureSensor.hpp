#pragma once

namespace BareCpper
{
	/**
	 * @brief Abstract class for temperature sensor.
	 * @tparam TemperatureT The temperature value type.
	*/
	template <typename TemperatureT>
	class TemperatureSensor
	{
	public:
		/**
		 * @brief Get the current temperature reading
		 * from the sensor.
		 * 
		 * @return TemperatureT The temperature in deg C.
		 */
		virtual TemperatureT getTemperature() = 0;
	};
}