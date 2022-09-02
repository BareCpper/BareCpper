#pragma once

#include <cstdint>
#include <functional>

namespace BareCpper
{
	/**
	 * Interface class for Timer/Counter (TC) peripheral.
	 * Provides basic functions for TC peripheral.
	*/
	class TimerCounter
	{
	public:
		virtual ~TimerCounter() { }

		/**
		 * Initialize the TC peripheral
		 * This function is needed instead of a constructor
		 * to be able to declare the object in global namespace
		 * to use it inside ISR, before other MCU initializations are done
		*/
		virtual void init(const uint32_t periodUs, const uint32_t ccUs = 0) = 0;

		/**
		 * Start the TC peripheral
		*/
		virtual void start() = 0;

		/**
		 * Stop the TC peripheral
		*/
		virtual void stop() = 0;

		/**
		 * @brief Read the current TC counter register value and convert it in us
		 * @return The current TC counter value, in us
		*/
		virtual double getCounterValue() = 0;

		/**
		 * @brief Change the TC period (overflow time)
		 * @param timeUs The period in us
		*/
		virtual void setPeriod(const uint32_t timeUs) = 0;

		/**
		 * @brief Set Capture/Compare match time
		 * @param timeUs The match time in us
		*/
		virtual void setCaptureCompare(const uint32_t timeUs) = 0;

		/**
		 * Enable period interrupt
		*/
		virtual void enablePeriodInterrupt() = 0;

		/**
		 * Enable interrupt on compare match
		*/
		virtual void enableCaptureCompareInterrupt() = 0;

		/**
		 * Disable period interrupt
		*/
		virtual void disablePeriodInterrupt() = 0;

		/**
		 * Disable interrupt on compare match
		*/
		virtual void disableCaptureCompareInterrupt() = 0;

		/**
		 * @brief Set callback function to be called when timer period interrupt fires
		 * @param callbackFunction The function to be called when period interrupt happens
		*/
		virtual void setPeriodCallback(const std::function<void()>& periodCallback) = 0;

		/**
		 * @brief Set callback function to be called when timer capture/compare interrupt fires
		 * @param ccCallback The function to be called when cc interrupt fires
		*/
		virtual void setCaptureCompareCallback(const std::function<void()>& ccCallback) = 0;
	};
}