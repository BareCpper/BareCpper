#ifndef TEMPERATURE_SENSOR_HPP
#define TEMPERATURE_SENSOR_HPP

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

		/** Start an async read returning true on subsequent call after completion
		 * @remark The return value must be polled and will only be `true` after async read completes. Subsequenct call will initiate a new async read. 
		 * @param temperature Reading when return==true
		 * @return True when async read completes, otherwise false
		*/
		virtual bool getTemperatureAsync(TemperatureT& temperature) = 0;
	};
}

#endif //TEMPERATURE_SENSOR_HPP