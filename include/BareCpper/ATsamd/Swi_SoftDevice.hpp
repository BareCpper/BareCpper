
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
#define tDSCHG				200/*000ns*/												//min spec = 150us
#define tRESET				500/*000nsn*/												//min spec = 480us (STD Speed)
#define tRRT				10/*000ns*/												//min spec = 8us
#define tDRR				1/*000ns*/												//min spec = 1us; max spec = 2us
#define tMSDR				2/*000ns*/												//min spec = 2us; max spec = 6us
#define tHTSS				200/*000ns*/												//min spec = 150us

#define tDACK_DLY			delayUs(8/*000ns*/)
#define tRRT_DLY			delayUs(tRRT)
#define tDRR_DLY			delayUs(tDRR)
#define tMSDR_DLY			delayUs(tMSDR) //< unused
#define tDSCHG_DLY			delayUs(tDSCHG)
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
		PORT->Group[iPort].PINCFG[iPin].reg = (uint8_t)(PORT_PINCFG_INEN);
		PORT->Group[iPort].DIRSET.reg = (1 << iPin); // Set pin to output mode
		PORT->Group[iPort].OUTCLR.reg = (1 << iPin); // Drive low
	}

	/** Release SI/O pin and set as input
	@note ATmega: {PORT_DDR &= ~si_pin;}
	*/
	inline void release_si(const uint8_t iPort, const uint8_t iPin)
	{
		// @note External pull-up is in place so we don't need to pull high
		PORT->Group[iPort].PINCFG[iPin].reg = (uint8_t)(PORT_PINCFG_INEN);// Set pin to input
		PORT->Group[iPort].DIRCLR.reg = (1 << iPin); // Set pin to input mode
	}

	/** Read input
	@note ATmega: {PORT_IN & devicePin_}
	*/
	inline bool readIn(const uint8_t iPort, const uint8_t iPin)
	{
		return (PORT->Group[iPort].IN.reg & (1 << iPin)) != 0;
	}

	/** Enable pin as output
	@note ATmega: {PORT_OUT |= devicePin_;	 PORT_DDR |= devicePin_;}
	*/
	inline void enableOutput(const uint8_t iPort, const uint8_t iPin)
	{
		// Set pin to input mode
		PORT->Group[iPort].PINCFG[iPin].reg = (uint8_t)(PORT_PINCFG_INEN);
		drive_si_low(iPort, iPin);
	}

	/** Enable pin as input
	@note ATmega: PORT_DDR &= ~devicePin_;
	*/
	inline void enableInput(const uint8_t iPort, const uint8_t iPin)
	{
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
		Swi::send_bytes(writeSize, &cmdWriteBuffer[0]); //transmit data bytes to the EEPROM --> GO TO Swi::send_bytes()
		Swi::start_stop_cond();	
		//stop condition --> GO TO Swi::start_stop_cond()
		return Status::Success; //return success code
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
			Swi::send_bytes(writeSize, &cmdWriteBuffer[0]); //send device address byte with R/W bit = 1 --> GO TO Swi::send_bytes()
			Swi::start_stop_cond(); //start condition --> GO TO Swi::start_stop_cond()
		}

		cmdWriteBuffer[0] |= 0x01; //cmdWriteBuffer ORed with 0x01 or set the R/W bit = 1 in the device address byte
		Swi::send_bytes((writeSize - packet->memoryAddressLength), &cmdWriteBuffer[0]); //send device address byte --> GO TO Swi::send_bytes()

		if (!packet->chk_ack_only_flag) //if the EEPROM ACKs the device address byte
		{
			Swi::receive_bytes(packet->rlen, packet->buffer); //perform read operation --> GO TO Swi::receive_bytes()
		}

		Swi::start_stop_cond(); //stop condition --> GO TO Swi::start_stop_cond() 
		return Status::Success; //return success code
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
		drive_si_low( iPort_, iPin_ ); //drive SI/O pin low
		tDSCHG_DLY; //discharge low time delay
		release_si( iPort_, iPin_ ); //release SI/O pin and set as input;
		tRRT_DLY; //reset recovery time delay
		drive_si_low( iPort_, iPin_ ); //drive SI/O pin low
		tDRR_DLY; //discovery response request time delay
		release_si( iPort_, iPin_ ); //release SI/O pin and set as input
		tDACK_DLY; //master strobe
		return (readIn( iPort_, iPin_ )); //return value of SI/O
	}

	inline void Swi::start_stop_cond()
	{
		release_si( iPort_, iPin_ ); //release SI/O pin and set as input;
		tHTSS_DLY; //SI/O high time before start/stop conditions
	}

} //END: BareCpper