
#ifndef BARECPPER_SWI_HPP_
#  error "Include <BareCpper/Swi.hpp> instead of this file."
#endif

#if !__SAMD51__
#  error "SAMD library error, please check and update accordingly."
#endif

#include <cstring> //< std::memcpy


///////////////////////////////////////////////////

#define swi_enable_interrupts  sei													//enable interrupts
#define swi_disable_interrupts cli													//disable interrupts

#define INPUT	0																	//define input
#define OUTPUT	1																	//define output


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//define MCU port registers

#define PORT_DDR        //TODO: (DDRD)													//direction register for device id 0
#define PORT_OUT        //TODO: (PORTD)												//output port register for device id 0
#define PORT_IN         //TODO: (PIND)													//input port register for device id 0


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//define Reset and Discovery Timing

#define tDSCHG				200000												//min spec = 150us
#define tRESET				500000												//min spec = 480us (STD Speed)
#define tRRT				10000												//min spec = 8us
#define tDRR				1000												//min spec = 1us; max spec = 2us
#define tMSDR				2000												//min spec = 2us; max spec = 6us
#define tHTSS				200000												//min spec = 150us

#define tDACK_DLY			//TODO: _delay_ns(8000)
#define tRRT_DLY			//TODO: _delay_ns(tRRT)
#define tDRR_DLY			//TODO: _delay_ns(tDRR)
#define tMSDR_DLY			//TODO: _delay_ns(tMSDR)
#define tDSCHG_DLY			//TODO: _delay_ns(tDSCHG)
#define tDRESET_DLY			//TODO: _delay_ns(tRESET)
#define tHTSS_DLY           //TODO: _delay_ns(tHTSS)


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
							
#define tLOW0_HDLY			//TODO: _delay_ns(11000)									//min spec = 6us; max spec = 16us
#define tRD_HDLY			//TODO: _delay_ns(1200)										//min spec = 1us; max spec = 2us
#define tLOW1_HDLY			//TODO: _delay_ns(1500)										//min spec = 1us; max spec = 2us
#define tRCV0_HDLY			//TODO: _delay_ns(11000)
#define tRCV1_HDLY			//TODO: _delay_ns(14000)
							
#define tRD_DLY				//TODO: _delay_ns(1200)										//min spec = 1us; max spec = 2us
#define tSWIN_DLY           //TODO: _delay_ns(1500)										//delay to put master strobe within sample window
							
#define tLOW0_DLY			//TODO: _delay_ns(tLWO_TYPICAL)
#define tLOW1_DLY			//TODO: _delay_ns(tLW1_TYPICAL)

#define tBIT_DLY			//TODO: _delay_ns(tBIT_TYPICAL)
#define tRCV0_DLY			//TODO: _delay_ns(tBIT_TYPICAL - tLWO_TYPICAL)
#define tRCV1_DLY			//TODO: _delay_ns(tBIT_TYPICAL - tLW1_TYPICAL)

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//define MASTER releasing and driving SI/O

#define drive_si_low(si_pin)   //TODO:  {PORT_DDR |= si_pin; PORT_OUT &= ~si_pin;}
#define release_si(si_pin)     //TODO:  {PORT_DDR &= ~si_pin;}



///////////////////////////////////////

#define low			0x00															//defines "low"
#define high		0x01															//defines "high"
#define ACK			0x00															//defines "ACK" or acknowledge
#define device_pin  0x02															//defines the SI/O pin location

#define initDeviceWriteHi(pin)	{drive_si_low(pin); _delay_us(1); release_si(pin);}	//defines high-speed mode master read bit frame where delay is tRD
#define initDeviceWriteStd(pin) {drive_si_low(pin); _delay_us(6); release_si(pin);}	//defines standard speed mode master read bit frame where delay is tRD


namespace BareCpper
{

	uint8_t cmdWriteBuffer[64];	 //defines array for write buffer
	uint16_t tlow0;	 //defines variable for SI/O low time for a logic '0' depending on communication speed

	uint8_t Swi::write(const Swi::Package_t* packet)
	{
		uint8_t index = 0;																//variable used to determine position within the writeBuffer array
		uint8_t writeSize = 1;															//set initial value to writeSize variable

		cmdWriteBuffer[index++] = (((packet->opcode << 4) | packet->dev_addr) & ~0x01);		//load device address w/ opcode and set R/W = 0
		if (packet->mem_addr_length)														//get memory address
		{
			cmdWriteBuffer[index++] = packet->mem_addr;									//memory address
			writeSize++;																//increment the writeSize variable by 1
		}

		std::memcpy(&cmdWriteBuffer[index], packet->buffer, packet->wlen);						//load data to be written
		writeSize += packet->wlen;														//evaluate writeSize + wlen variable

		Swi::start_stop_cond();															//start condition --> GO TO Swi::start_stop_cond()
		Swi::send_bytes(writeSize, &cmdWriteBuffer[0]);									//transmit data bytes to the EEPROM --> GO TO Swi::send_bytes()
		Swi::start_stop_cond();	
		//stop condition --> GO TO Swi::start_stop_cond()
		return Status::Success;																//return success code
	}

	uint8_t Swi::read(const Swi::Package_t* packet)
	{
		uint8_t index = 0;																//variable used to determine position within the writeBuffer array
		uint8_t writeSize = 1;															//set initial value to writeSize variable

		cmdWriteBuffer[index++] = ((packet->opcode << 4) | packet->dev_addr);				//load device address w/ opcode
		if (packet->mem_addr_length)														//get memory word address
		{
			cmdWriteBuffer[index++] = packet->mem_addr;									//set write buffer to memory word address
			writeSize++;																//increment the writeSize variable by 1
		}

		Swi::start_stop_cond();															//send start condition --> GO TO Swi::start_stop_cond()

		if (packet->mem_addr_length)														//perform dummy write
		{
			cmdWriteBuffer[0] &= 0xFE;													//bitwiseAND cmdWriteBuffer array with 0xFEh
			Swi::send_bytes(writeSize, &cmdWriteBuffer[0]);								//send device address byte with R/W bit = 1 --> GO TO Swi::send_bytes()
			Swi::start_stop_cond();														//start condition --> GO TO Swi::start_stop_cond()
		}

		cmdWriteBuffer[0] |= 0x01;														//cmdWriteBuffer ORed with 0x01 or set the R/W bit = 1 in the device address byte
		Swi::send_bytes((writeSize - packet->mem_addr_length), &cmdWriteBuffer[0]);		//send device address byte --> GO TO Swi::send_bytes()

		if (!packet->chk_ack_only_flag)													//if the EEPROM ACKs the device address byte
		{
			Swi::receive_bytes(packet->rlen, packet->buffer);							//perform read operation --> GO TO Swi::receive_bytes()
		}

		Swi::start_stop_cond();															//stop condition --> GO TO Swi::start_stop_cond() 
		return Status::Success;																//return success code
	}

	uint8_t Swi::send_bytes(uint8_t count, uint8_t* buffer)
	{
		uint8_t bit_mask;																//declares variable for bit mask for data to be transmitted
		uint8_t retCode;																//declares variable for the returned code if a NACK is received
		Swi::disable_interrupts();														//disable interrupts while sending
		PORT_OUT |= device_pin;															//set SI/O pin as output
		PORT_DDR |= device_pin;															//set direction of SI/O

		for (uint8_t ii = 0; ii < count; ii++)											//for loop for number of byte to be written 
		{
			for (bit_mask = 0x80; bit_mask >= 1; bit_mask >>= 1)						//for loop for bit mask
			{
				//if the next bit transmitted is a logic '1'	
				if (bit_mask & buffer[ii])												//send a logic '1'
				{
					drive_si_low(device_pin);											//drive SI/O pin low
					tLOW1_DLY;															//SI/O low time, logic '1' delay
					release_si(device_pin);												//release SI/O pin and set as input
					tRCV1_DLY;															//slave recovery time delay for logic '1'
				}
				//if the next bit transmitted is a logic '0'	
				else																	//send a logic '0'
				{
					drive_si_low(device_pin);											//drive SI/O pin low
					tLOW0_DLY;															//SI/O low time, logic '0' delay
					release_si(device_pin);												//release SI/O pin and set as input											
					tRCV0_DLY;															//slave recovery time delay for logic'0'
				}
			}
			//this checks for an ACK or NACK from EEPROM
			drive_si_low(device_pin);													//drive SI/O pin low
			tRD_DLY;																	//SI/O low time during read delay
			release_si(device_pin);														//release SI/O pin and set as input	
			tLOW1_DLY;																	//master read strobe time (same as SI/O low time, logic '1') delay
		//check for ACK/NACK
			if (PORT_IN & device_pin)													//if a NAK is detected													
			{
				release_si(device_pin);													//release SI/O pin and set as input
				Swi::enable_interrupts();												//enable interrupts after sending

				if (ii == 0) { retCode = Status::AddressAckFail; }								//EEPROM failed to send ACK during device address byte			 						
				else { retCode = Status::DataAckFail; }									//EEPROM failed to send ACK during word address byte
				return retCode;															//return which byte the EEPROM did not ACK 
			}

			tRCV0_DLY;																	//slave recovery time delay (same for logic'0' and logic '1')
		}

		release_si(device_pin);															//release SI/O pin and set as input
		Swi::enable_interrupts();														//enable interrupts after sending
		return Status::Success;											//return success code
	}

	uint8_t Swi::receive_bytes(uint8_t count, uint8_t* buffer)
	{
		uint8_t bit_mask;																//declares variable for bit mask for data to be transmitted
		Swi::disable_interrupts();														//disable interrupts while sending
		memset(&buffer[0], 0, count);														//clear buffer before reading
		PORT_DDR &= ~device_pin;														//configure signal pin as input.

		//data phase,... Receive bits and store in buffer.
		for (uint8_t ii = 0; ii < count; ii++)											//for loop for number of byte to be received 
		{
			for (bit_mask = 0x80; bit_mask >= 1; bit_mask >>= 1)						//for loop for bit mask
			{
	#ifdef STANDARD_SPEED													//device is set for standard speed communication 
				initDeviceWriteStd(device_pin);										//standard speed mode master read bit frame where delay is tRD
	#else																	//device is set for high-speed communication
				initDeviceWriteHi(device_pin);										//high-speed mode master read bit frame where delay is tRD
	#endif

				tSWIN_DLY;																//delay to put master read inside the master sampling window

			//check for SI/O state
				if (PORT_IN & device_pin) { buffer[ii] |= bit_mask; }					//if a logic '1' is detected; received "one" bit
				tBIT_DLY;																//bit frame duration (tBIT) before reading the next bit
			}
			if (ii < (count - 1)) { sendAck(); }									//send ACK except for last byte of read --> GO TO sendAck()
		}
		sendNack();																		//send NACK to EEPROM signaling read is complete
		Swi::enable_interrupts();														//enable interrupts after sending
		return Status::Success;											//return success code
	}

	uint8_t Swi::write_stdspeed_cmd(const Swi::Package_t* packet)
	{

		uint8_t index = 0;																//variable used to determine position within the writeBuffer array
		uint8_t writeSize = 1;															//set initial value to writeSize variable
		uint8_t bit_mask;																//declares variable for bit mask for data to be transmitted
		uint8_t retCode;																//declares variable for the returned code if a NACK is received


		cmdWriteBuffer[index++] = (((packet->opcode << 4) | packet->dev_addr) & ~0x01);		//load device address w/ opcode and set R/W = 0
		if (packet->mem_addr_length)														//get memory address
		{
			cmdWriteBuffer[index++] = packet->mem_addr;									//memory address
			writeSize++;																//increment the writeSize variable by 1
		}

		std::memcpy(&cmdWriteBuffer[index], packet->buffer, packet->wlen);						//load data to be written
		writeSize += packet->wlen;														//evaluate writeSize + wlen variable

		Swi::start_stop_cond();															//start condition --> GO TO Swi::start_stop_cond()

		Swi::disable_interrupts();														//disable interrupts while sending
		PORT_OUT |= device_pin;															//set SI/O pin as output
		PORT_DDR |= device_pin;															//set direction of SI/O

		for (uint8_t ii = 0; ii < writeSize; ii++)										//for loop for number of byte to be written  
		{
			for (bit_mask = 0x80; bit_mask >= 1; bit_mask >>= 1)						//for loop for bit mask 
			{
				//if the next bit transmitted is a logic '1'		
				if (bit_mask & cmdWriteBuffer[ii])										//send a logic '1' 
				{
					drive_si_low(device_pin);											//drive SI/O pin low
					tLOW1_HDLY;															//SI/O low time, logic '1' delay
					release_si(device_pin);												//release SI/O pin and set as input
					tRCV1_HDLY;															//slave recovery time delay (same for logic'0' and logic '1')
				}
				//if the next bit transmitted is a logic '0'	
				else																	//send a logic '0'
				{
					drive_si_low(device_pin);											//drive SI/O pin low
					tLOW0_HDLY;															//SI/O low time, logic '0' delay
					release_si(device_pin);												//release SI/O pin and set as input
					tRCV0_HDLY;															//slave recovery time delay (same for logic'0' and logic '1')
				}
			}
			//this checks for an ACK or NACK from EEPROM
			drive_si_low(device_pin);													//drive SI/O pin low
			tRD_HDLY;																	//SI/O low time during read delay
			release_si(device_pin);														//release SI/O pin and set as input
			tLOW1_HDLY;																	//master read strobe time (same as SI/O low time, logic '1') delay
		//check for ACK/NACK	
			if (PORT_IN & device_pin)													//if a NAK is detected
			{
				release_si(device_pin);													//release SI/O pin and set as input
				Swi::enable_interrupts();												//enable interrupts after sending

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

			tRCV0_HDLY;																	//slave recovery time delay (same for logic'0' and logic '1')
		}

		release_si(device_pin);															//release SI/O pin and set as input
		Swi::enable_interrupts();														//enable interrupts after sending
		return Status::Success;											//return success code
	}

	void Swi::sendAck()
	{
		drive_si_low(device_pin);														//drive SI/O pin low
		tLOW0_HDLY;																		//SI/O low time, logic '0' delay
		release_si(device_pin);															//release SI/O pin and set as input
		tRCV0_DLY;																		//slave recovery time delay for logic'0'
	}

	void Swi::sendNack()
	{
		drive_si_low(device_pin);														//drive SI/O pin low
		tLOW1_DLY;																		//SI/O low time, logic '1' delay
		release_si(device_pin);															//release SI/O pin and set as input
		tRCV1_DLY;																		//slave recovery time delay for logic'1'

	}

	uint8_t Swi::device_discovery()
	{
		drive_si_low(device_pin);														//drive SI/O pin low
		tDSCHG_DLY;																		//discharge low time delay
		release_si(device_pin);															//release SI/O pin and set as input;
		tRRT_DLY;																		//reset recovery time delay
		drive_si_low(device_pin);														//drive SI/O pin low
		tDRR_DLY;																		//discovery response request time delay
		release_si(device_pin);															//release SI/O pin and set as input
		tDACK_DLY;																		//master strobe
		return (PORT_IN & device_pin);													//return value of SI/O
	}

	void Swi::start_stop_cond()
	{
		release_si(device_pin);															//release SI/O pin and set as input;
		tHTSS_DLY;																		//SI/O high time before start/stop conditions
	}

} //END: BareCpper