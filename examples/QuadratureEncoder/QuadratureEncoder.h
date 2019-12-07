#ifndef QUADRATUREENCODER_H
#define QUADRATUREENCODER_H

#include "barecpper/assert.h"
#include "QueueSPSC.h"

namespace Encoder
{
	static const int8_t cTwoStep = 0u; //< Not implemented

	/** Quadrature sequence code direction indexed by Previous an Current pin states packed in a 4-bit integer(nibble)
	 00 01 11 10 (Right)
	 01 00 10 11 (Left)
	 e.g.
	  transition from 00 to 01 is binary 0001 which is index 1 or cQuadratureStepTable[1] which is a +1 i.e. Right transition
	  transition form 00 to 00 is binary 0000 which is index 0 or cQuadratureStepTable[0] which is a 0 i.e. No transition
	 */
	static const int8_t cQuadratureStepTable[16u] =
	{
		0, // No-Step[0000]
		+1, //00 01
		-1, //00 10
		cTwoStep, //@TODO: +/-2
		-1, //01 00
		cTwoStep, //@TODO: +/-2
		cTwoStep, //@TODO: +/-2
		+1, //01 11
		+1, //10 00
		cTwoStep, //@TODO: +/-2
		cTwoStep, //@TODO: +/-2
		-1, //10 11
		cTwoStep, //@TODO: +/-2 
		-1, //11 01
		+1, //11 10
		0, // No-Step[1111]
	};

	/** Relative quadrature/step encoder
	*/
	template<typename Counter = uint16_t >
	class Quadrature
	{
	public:   
		friend void PCINT0_vect();

		typedef Counter Counter_t; ///< Step counter type

	public:
		/** 
		@param[in] currentStep  The initial step count. Can be used to restore a known position between power cycles etc.
		*/
		Quadrature( Counter_t currentStep = 0 )
		: currentStep_(currentStep)
		, previousState_(0) //<@todo Needs init to current state
		{
			const uint8_t cPinMask = 0b11; //< Arduino pins 8 & 9			
			cli();    // switch interrupts off while messing with their settings
			PCICR |= (1 << PCIE0);    // set PCIE0 to enable PCMSK0 scan
			PCMSK0 |= cPinMask; // set Pins to trigger a PCINT0_vect interrupt on state change
			sei();    // turn interrupts back on
		}

		/** Update quadrature encoder with new pin state
		@return The new step counter
		*/
		Counter_t update( uint8_t portState )
		{
			int8_t stepChange = calculateStepChange( portState );
		    currentStep_ += stepChange;
			return currentStep_;
		}

		/** Return the current step count of the encoder 
		*/
		Counter_t getStep() const
		{	
			return currentStep_;
		}

	private:
		/** Updates the step change from one state to the next and updates the previous stored state.
		@return Step change where 0 is no change, -1 is a left change, and +1 is a right change. 
		@note +/-2 may be returned in future if a step was missed
		*/
		int8_t calculateStepChange( uint8_t portState )
		{
			//assert( !(portState & ~0b11) ); //< Important: masked to only first 2 pins of interest
			const uint8_t stepIndex =  previousState_ | portState ;
			previousState_ = portState << 2u; //< Store for next update with shift applied as quicker
			return cQuadratureStepTable[stepIndex];
		}

	private:
		Counter_t currentStep_; ///< Current step count
		uint8_t previousState_; ///< Previous port state at last update masked to only pins of interest
	};

} //END: Encoder

#endif  

