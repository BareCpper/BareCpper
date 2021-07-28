
#ifndef BARECPPER_SWI_HPP_
#  error "Include <BareCpper/Swi.hpp> instead of this file."
#endif

#if !__SAMD51__
#  error "SAMD library error, please check and update accordingly."
#endif

#include <cstring> //< std::memcpy

#include "sam.h"

//#define STANDARD_SPEED/* Disable Interrupts */

#define swi_enable_interrupts  __enable_irq //enable interrupts
#define swi_disable_interrupts __disable_irq //disable interrupts

//define Reset and Discovery Timing
#define tDSCHG				200 //Reset Low Time, Device in Inactive State: min spec = 150us
#define tRESET				500 //Discharge Low Time, Device in Active Write Cycle(tWR): min spec = 480us (STD Speed), 48us (High Speed)
#define tRRT				10 //Reset Recovery Time: min spec = 8us
#define tDRR				1 //Discovery Response Request: min spec = 1us; max spec = 2us - t_PUP
#define tDACK_MIN			8 //Discovery Response Acknowledge Time: min spec = 8us; max spec = 24
#define tDACK_MAX			24 // ''    ''
#define tMSDR_MIN			3 //Master Strobe Discovery Response Time: min spec = 2us; max spec = 6us
#define tMSDR_MAX			6 // '' ''
#define tHTSS				200 //min spec = 150us

#define tDRESET_DLY			delayUs(tRESET) ///< Unused
#define tHTSS_DLY           delayUs(tHTSS)


//define High-Speed Mode Communication Timing

#define tLOW0_MIN			6000
#define tLOW0_MAX			16000
#define tLOW1_MIN			1000
#define tLOW1_MAX			2000
#define tBIT_MIN			8000												//tLOW0 + tPUP + tRCV
#define tBIT_MAX			25000
#define tRCV_MIN			2000

#define tLWO_TYPICAL		(tLOW0_MIN +((tLOW0_MAX-tLOW0_MIN)/2))				//creates typical data timing (AVG of Min and Max)
#define tLW1_TYPICAL	    (tLOW1_MIN +((tLOW1_MAX-tLOW1_MIN)/2))				//creates typical data timing (AVG of Min and Max)
#define tBIT_TYPICAL		(tBIT_MIN +((tBIT_MAX-tBIT_MIN)/2))					//creates typical data timing (AVG of Min and Max)
							
#define tLOW0_HDLY			delayNs(11000)									//min spec = 6us; max spec = 16us
#define tRD_HDLY			delayNs(1200)										//min spec = 1us; max spec = 2us
#define tLOW1_HDLY			delayNs(1500)										//min spec = 1us; max spec = 2us
#define tRCV0_HDLY			delayNs(11000)
#define tRCV1_HDLY			delayNs(14000)
							
#define tRD_DLY				delayNs(1200)										//min spec = 1us; max spec = 2us
#define tSWIN_DLY           delayNs(1500)										//delay to put master strobe within sample window
							
#define tLOW0_DLY			delayNs(tLWO_TYPICAL)
#define tLOW1_DLY			delayNs(tLW1_TYPICAL)

#define tBIT_DLY			delayNs(tBIT_TYPICAL)
#define tRCV0_DLY			delayNs(tBIT_TYPICAL - tLWO_TYPICAL)
#define tRCV1_DLY			delayNs(tBIT_TYPICAL - tLW1_TYPICAL)

#include "../Delay.hpp" //< BareCpper::delayNs/Ms/Us

namespace BareCpper
{
	/** Set pin as output low
	@note ATmega: {PORT_DDR |= si_pin; PORT_OUT &= ~si_pin;}
	*/
	inline void drive_si_low(const uint8_t iPort, const uint8_t iPin)
	{
		using ::Port;
		PORT->Group[iPort].PINCFG[iPin].reg = (uint8_t)(PORT_PINCFG_INEN);
		PORT->Group[iPort].DIRSET.reg = (1 << iPin); // Set pin to output mode
		PORT->Group[iPort].OUTCLR.reg = (1 << iPin); // Drive low
	}

	/** Release SI/O pin and set as input
	@note ATmega: {PORT_DDR &= ~si_pin;}
	*/
	inline void release_si(const uint8_t iPort, const uint8_t iPin)
	{
		using ::Port;
		// @note External pull-up is in place so we don't need to pull high
		PORT->Group[iPort].PINCFG[iPin].reg = (uint8_t)(PORT_PINCFG_INEN);// Set pin to input
		PORT->Group[iPort].DIRCLR.reg = (1 << iPin); // Set pin to input mode
	}

	/** Read input
	@note ATmega: {PORT_IN & devicePin_}
	*/
	inline bool readIn(const uint8_t iPort, const uint8_t iPin)
	{
		using ::Port;
		return (PORT->Group[iPort].IN.reg & (1 << iPin)) != 0;
	}

	/** Enable pin as output
	@note ATmega: {PORT_OUT |= devicePin_;	 PORT_DDR |= devicePin_;}
	*/
	inline void enableOutput(const uint8_t iPort, const uint8_t iPin)
	{
		using ::Port;
		// Set pin to input mode
		PORT->Group[iPort].PINCFG[iPin].reg = (uint8_t)(PORT_PINCFG_INEN);
		drive_si_low(iPort, iPin);
	}

	/** Enable pin as input
	@note ATmega: PORT_DDR &= ~devicePin_;
	*/
	inline void enableInput(const uint8_t iPort, const uint8_t iPin)
	{
		using ::Port;
		PORT->Group[iPort].PINCFG[iPin].reg = (uint8_t)(PORT_PINCFG_INEN);
		release_si(iPort, iPin);
	}

	/** defines high-speed mode master read bit frame where delay is tRD
	*/
	inline void initDeviceWriteHi(const uint8_t iPort, const uint8_t iPin)
	{
		drive_si_low(iPort, iPin);
		BareCpper::delayUs(1);
		release_si(iPort, iPin);
	}

	/** defines standard speed mode master read bit frame where delay is tRD
	*/
	inline void initDeviceWriteStd(const uint8_t iPort, const uint8_t iPin)
	{
		drive_si_low(iPort, iPin);
		BareCpper::delayUs(6);
		release_si(iPort, iPin);
	}

	//uint16_t tlow0;	 //defines variable for SI/O low time for a logic '0' depending on communication speed

	inline Swi::Status Swi::write(const Swi::Package_t* packet)
	{
		uint8_t index = 0; //variable used to determine position within the writeBuffer array
		uint8_t writeSize = 1; //set initial value to writeSize variable

		uint8_t cmdWriteBuffer[64];	 //defines array for write buffer
		cmdWriteBuffer[index++] = (((packet->opcode << 4) | packet->deviceAddress) & ~0x01); //load device address w/ opcode and set R/W = 0
		if (packet->memoryAddressLength) //get memory address
		{
			cmdWriteBuffer[index++] = packet->memoryAddress; //memory address
			writeSize++; //increment the writeSize variable by 1
		}

		std::memcpy(&cmdWriteBuffer[index], packet->buffer, packet->wlen); //load data to be written
		writeSize += packet->wlen; //evaluate writeSize + wlen variable
 
		Swi::start_stop_cond(); //start condition --> GO TO Swi::start_stop_cond()
		Status sendRet = Swi::send_bytes(writeSize, &cmdWriteBuffer[0]); //transmit data bytes to the EEPROM --> GO TO Swi::send_bytes()
		Swi::start_stop_cond();	
		//stop condition --> GO TO Swi::start_stop_cond()

		return sendRet; //return status code
	}

	inline Swi::Status Swi::read(const Swi::Package_t* packet)
	{
		uint8_t index = 0; //variable used to determine position within the writeBuffer array
		uint8_t writeSize = 1; //set initial value to writeSize variable

		uint8_t cmdWriteBuffer[64];	 //defines array for write buffer
		cmdWriteBuffer[index++] = ((packet->opcode << 4) | packet->deviceAddress); //load device address w/ opcode
		if (packet->memoryAddressLength) //get memory word address
		{
			cmdWriteBuffer[index++] = packet->memoryAddress; //set write buffer to memory word address
			writeSize++; //increment the writeSize variable by 1
		}

		Swi::start_stop_cond(); //send start condition --> GO TO Swi::start_stop_cond()

		if (packet->memoryAddressLength) //perform dummy write
		{
			cmdWriteBuffer[0] &= 0xFE; //bitwiseAND cmdWriteBuffer array with 0xFEh
			const Status addrRet = Swi::send_bytes(writeSize, &cmdWriteBuffer[0]); //send device address byte with R/W bit = 1 --> GO TO Swi::send_bytes()
			Swi::start_stop_cond(); //start condition --> GO TO Swi::start_stop_cond()

			if(addrRet != Status::Success)
				return addrRet;
		}

		cmdWriteBuffer[0] |= 0x01; //cmdWriteBuffer ORed with 0x01 or set the R/W bit = 1 in the device address byte
		const Status reqRet = Swi::send_bytes((writeSize - packet->memoryAddressLength), &cmdWriteBuffer[0]); //send device address byte --> GO TO Swi::send_bytes()

		if(reqRet != Status::Success)
			return reqRet;

		Status recieveRet;
		if (!packet->chk_ack_only_flag) //if the EEPROM ACKs the device address byte
		{
			recieveRet = Swi::receive_bytes(packet->rlen, packet->buffer); //perform read operation --> GO TO Swi::receive_bytes()
		}
		else
		{
			recieveRet = Status::Success;
		}

		Swi::start_stop_cond(); //stop condition --> GO TO Swi::start_stop_cond() 
		return recieveRet; //return success code
	}

	inline Swi::Status Swi::send_bytes(uint8_t count, uint8_t* buffer)
	{
		uint8_t bit_mask; //declares variable for bit mask for data to be transmitted
		Status retCode; //declares variable for the returned code if a NACK is received
		swi_disable_interrupts(); //disable interrupts while sending
		
		enableOutput(iPort_, iPin_);

		for (uint8_t ii = 0; ii < count; ii++) //for loop for number of byte to be written 
		{
			for (bit_mask = 0x80; bit_mask >= 1; bit_mask >>= 1) //for loop for bit mask
			{
				//if the next bit transmitted is a logic '1'	
				if (bit_mask & buffer[ii]) //send a logic '1'
				{
					drive_si_low( iPort_, iPin_ ); //drive SI/O pin low
					tLOW1_DLY; //SI/O low time, logic '1' delay
					release_si( iPort_, iPin_ ); //release SI/O pin and set as input
					tRCV1_DLY; //slave recovery time delay for logic '1'
				}
				//if the next bit transmitted is a logic '0'	
				else																	//send a logic '0'
				{
					drive_si_low( iPort_, iPin_ ); //drive SI/O pin low
					tLOW0_DLY; //SI/O low time, logic '0' delay
					release_si( iPort_, iPin_ ); //release SI/O pin and set as input											
					tRCV0_DLY; //slave recovery time delay for logic'0'
				}
			}
			//this checks for an ACK or NACK from EEPROM
			drive_si_low( iPort_, iPin_ ); //drive SI/O pin low
			tRD_DLY; //SI/O low time during read delay
			release_si( iPort_, iPin_ ); //release SI/O pin and set as input	
			tLOW1_DLY; //master read strobe time (same as SI/O low time, logic '1') delay
			//check for ACK/NACK
			if (readIn( iPort_, iPin_ )) //if a NAK is detected													
			{
				release_si( iPort_, iPin_ ); //release SI/O pin and set as input
				swi_enable_interrupts(); //enable interrupts after sending

				if (ii == 0) //EEPROM failed to send ACK during device address byte	
				{ 
					retCode = Status::AddressAckFail; 
				}										 						
				else //EEPROM failed to send ACK during word address byte
				{ 
					retCode = Status::DataAckFail;
				}									
				return retCode; //return which byte the EEPROM did not ACK 
			}

			tRCV0_DLY; //slave recovery time delay (same for logic'0' and logic '1')
		}

		release_si( iPort_, iPin_ ); //release SI/O pin and set as input
		swi_enable_interrupts(); //enable interrupts after sending

		return Status::Success; //return success code
	}

	inline Swi::Status Swi::receive_bytes(uint8_t count, uint8_t* buffer)
	{
		swi_disable_interrupts(); //disable interrupts while sending
		std::memset(&buffer[0], 0, count); //clear buffer before reading
																
		enableInput(iPort_, iPin_);//configure signal pin as input.

		//data phase,... Receive bits and store in buffer.
		for (uint8_t ii = 0; ii < count; ii++) //for loop for number of byte to be received 
		{
			for (uint8_t bit_mask = 0x80; bit_mask >= 1; bit_mask >>= 1) //for loop for bit mask
			{
	#ifdef STANDARD_SPEED //device is set for standard speed communication 
				initDeviceWriteStd( iPort_, iPin_ ); //standard speed mode master read bit frame where delay is tRD
	#else //device is set for high-speed communication
				initDeviceWriteHi( iPort_, iPin_ ); //high-speed mode master read bit frame where delay is tRD
	#endif

				tSWIN_DLY; //delay to put master read inside the master sampling window

				//check for SI/O state
				if ( readIn( iPort_, iPin_ ) ) //if a logic '1' is detected; received "one" bit
				{ 
					buffer[ii] |= bit_mask;
				}
				tBIT_DLY; //bit frame duration (tBIT) before reading the next bit
			}
			if (ii < (count - 1)) 
			{ 
				sendAck(); //send ACK except for last byte of read --> GO TO sendAck()
			} 
		}
		sendNack(); //send NACK to EEPROM signalling read is complete
		swi_enable_interrupts(); //enable interrupts after sending

		return Status::Success; //return success code
	}

	inline Swi::Status Swi::write_stdspeed_cmd(const Swi::Package_t* packet)
	{
		uint8_t index = 0; //variable used to determine position within the writeBuffer array
		uint8_t writeSize = 1; //set initial value to writeSize variable
		uint8_t bit_mask; //declares variable for bit mask for data to be transmitted

		uint8_t cmdWriteBuffer[64];	 //defines array for write buffer
		cmdWriteBuffer[index++] = (((packet->opcode << 4) | packet->deviceAddress) & ~0x01); //load device address w/ opcode and set R/W = 0
		if (packet->memoryAddressLength) //get memory address
		{
			cmdWriteBuffer[index++] = packet->memoryAddress; //memory address
			writeSize++; //increment the writeSize variable by 1
		}

		std::memcpy(&cmdWriteBuffer[index], packet->buffer, packet->wlen); //load data to be written
		writeSize += packet->wlen; //evaluate writeSize + wlen variable

		Swi::start_stop_cond(); //start condition --> GO TO Swi::start_stop_cond()

		swi_disable_interrupts(); //disable interrupts while sending
		enableOutput(iPort_, iPin_);

		for (uint8_t ii = 0; ii < writeSize; ii++) //for loop for number of byte to be written  
		{
			for (bit_mask = 0x80; bit_mask >= 1; bit_mask >>= 1) //for loop for bit mask 
			{
				//if the next bit transmitted is a logic '1'		
				if (bit_mask & cmdWriteBuffer[ii]) //send a logic '1' 
				{
					drive_si_low( iPort_, iPin_ ); //drive SI/O pin low
					tLOW1_HDLY; //SI/O low time, logic '1' delay
					release_si( iPort_, iPin_ );  //release SI/O pin and set as input
					tRCV1_HDLY; //slave recovery time delay (same for logic'0' and logic '1')
				}
				//if the next bit transmitted is a logic '0'	
				else //send a logic '0'
				{
					drive_si_low( iPort_, iPin_ ); //drive SI/O pin low
					tLOW0_HDLY; //SI/O low time, logic '0' delay
					release_si( iPort_, iPin_ ); //release SI/O pin and set as input
					tRCV0_HDLY; //slave recovery time delay (same for logic'0' and logic '1')
				}
			}
			//this checks for an ACK or NACK from EEPROM
			drive_si_low( iPort_, iPin_ ); //drive SI/O pin low
			tRD_HDLY; //SI/O low time during read delay
			release_si( iPort_, iPin_ ); //release SI/O pin and set as input
			tLOW1_HDLY; //master read strobe time (same as SI/O low time, logic '1') delay
		
			//check for ACK/NACK	
			if (readIn( iPort_, iPin_ )) //if a NAK is detected
			{
				release_si( iPort_, iPin_ ); //release SI/O pin and set as input
				swi_enable_interrupts(); //enable interrupts after sending

				Status retCode; //declares variable for the returned code if a NACK is received
				if (ii == 0) 
				{
					retCode = Status::AddressAckFail;//EEPROM failed to send ACK during device address byte
				}							
				else 
				{
					retCode = Status::DataAckFail; //EEPROM failed to send ACK during word address byte
				}			

				return retCode; //return which byte the EEPROM did not ACK
			}

			tRCV0_HDLY; //slave recovery time delay (same for logic'0' and logic '1')
		}

		release_si( iPort_, iPin_ ); //release SI/O pin and set as input
		swi_enable_interrupts(); //enable interrupts after sending
		
		return Status::Success; //return success code
	}

	inline void Swi::sendAck()
	{
		drive_si_low( iPort_, iPin_ ); //drive SI/O pin low
		tLOW0_HDLY; //SI/O low time, logic '0' delay
		release_si( iPort_, iPin_ ); //release SI/O pin and set as input
		tRCV0_DLY; //slave recovery time delay for logic'0'
	}

	inline void Swi::sendNack()
	{
		drive_si_low( iPort_, iPin_ ); //drive SI/O pin low
		tLOW1_DLY; //SI/O low time, logic '1' delay
		release_si( iPort_, iPin_ ); //release SI/O pin and set as input
		tRCV1_DLY; //slave recovery time delay for logic'1'
	}

	inline bool Swi::device_discovery()
	{
		static CycleCounter timer;

		drive_si_low( iPort_, iPin_ ); //drive SI/O pin low
		delay(timer, timer.usToTicks(tDSCHG) ); //discharge low time delay

		release_si( iPort_, iPin_ ); //release SI/O pin and set as input;
		delay(timer, timer.usToTicks(tRRT)); //reset recovery time delay

		/** @see "3.1.1.2 Device Response Upon Reset or Power-Up" 
		* The Discovery Response Acknowledge sequence begins by the master driving the SI/O line low which
		* will start the AT21CS01/11 internal timing circuits. The master must continue to drive the line low for tDRR.
		*/
		drive_si_low( iPort_, iPin_ ); //drive SI/O pin low
		const auto tDrrStart = timer.count();
		continueAt(timer, tDrrStart + timer.usToTicks(tDRR) ); //discovery response request time delay
		release_si( iPort_, iPin_ ); //release SI/O pin and set as input

		/** @see "3.1.1.2 Device Response Upon Reset or Power-Up" 
		* The master should sample the state of the SI/O line at
		* tMSDR past the initiation of tDRR.
		*/
		const auto tMsdrMinTime = tDrrStart + timer.usToTicks(tMSDR_MIN);
		const auto tMsdrMaxTime = tDrrStart + timer.usToTicks(tMSDR_MAX);
		continueAt(timer, tMsdrMinTime); // tMSDR past the initiation of tDRR.
		bool isDiscovered;
		while ( !(isDiscovered = readIn(iPort_, iPin_))
			&& (timer.count() < tMsdrMaxTime))
		{
			__NOP(); ///< @todo NOP or not-to NOP
		}

		// @todo If Slave hasn't responded wihtin MSDR window, it is not clear if it would be better to accept response upoto tDACK_MAX?
		if (!isDiscovered)
			return false;

		/** @see "3.1.1.2 Device Response Upon Reset or Power-Up" 
		* During the tDRR time, the AT21CS01/11 will respond by concurrently driving SI/O low. The device will
		* continue to drive SI/O low for a total time of tDACK */
		const auto tDackMinTime = tDrrStart + timer.usToTicks(tDACK_MIN);
		const auto tDackMaxTime = tDrrStart + timer.usToTicks(tDACK_MAX);
		continueAt(timer, tDackMinTime); // After the tDACK time has elapsed, the AT21CS01 / 11 will release SI / O
		bool isHolding;
		while ( (isHolding = readIn(iPort_, iPin_))
			&& (timer.count() < tDackMaxTime))
		{
			__NOP(); ///< @todo NOP or not-to NOP
		}
		const bool isReleased = !isHolding;

		//Device should have released line within time of tDACK_MAX
		return isReleased;
	}

	inline void Swi::start_stop_cond()
	{
		release_si( iPort_, iPin_ ); //release SI/O pin and set as input;
		tHTSS_DLY; //SI/O high time before start/stop conditions
	}

} //END: BareCpper