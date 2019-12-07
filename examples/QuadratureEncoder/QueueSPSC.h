#ifndef CROG_QUEUESPSC_ASSERT_H
#define CROG_QUEUESPSC_ASSERT_H

#include <stdint.h>

/** Single-Produce Single-Consumer Queue
	*  @remark Stores the queue in a fixed size ring-buffer where front is the insertion point and back is the red point
	*  @tparam Data      The data type the queue stores
	*  @tparam cCapacity  The number of Data elements the queue stores. Power-of-two is more optimal i.e. 32,64,128
	*/
template< typename Data, size_t cCapacity >
class QueueSPSC
{
public:
	/** Queue index type
	*/
	typedef uint8_t Index;

	/** Increments supplied index wrapping at cCapacity-1
	*  @note This function is optimized for power-of-2 cCapacity values i.e 32, 64, 128
	*  @return The next valid data buffer index wrapped at cCapacity-1
	*/
	inline static Index next( Index index )
	{	
		if ( (cCapacity & (cCapacity - 1u)) == 0 ) //< Optimize power-of-2
			return ++index & (cCapacity-1u); //< Mask lower N-bits based on capacity
		else
			return ( index < (cCapacity-1u) ? ++index : 0); 
	}
    
public:
	/** Construct empty queue 
	*/
	QueueSPSC() 
	: front_(0u)
	, back_(0u) 
	{}

	/** Tests whether the queue is empty
	*  @warning If the queue was full when a 'push' occurred then the queue will be in an undefined empty state
	*  @return True if the queue is empty, false if it has one or more entries
	*/
	bool empty() const
	{ 
		return front_ == back_; 
	}

	/** Tests whether the capacity of the queue has been reached
	*  @return True if the cCapacity has been reached, false if there is space available
	*/
	bool full() const
	{
		return next(back_) == front_; 
	}

	/** Clears the queue of all content
	*  @remark After clear() the return value of empty() will be true
	*/
	void clear()
	{ 
		front_ = back_ = 0u;
	}

	/** Reads the head of the queue
	* @note pop() must be subsequently called to remove the head
	* @warning Only call if !empty()
	* @return Reference to the top head value from the queue
	*/
	const Data& top() const
	{
		return data_[front_];
	}

	/**Removes the head of the queue returning a copy of the data
	* @warning Only call if !empty()
	* @return The top head value from the queue
	*/
	Data pop()
	{
		Data t = top();
		front_ = next(front_);
		return t;
	}

	/** Writes a new entry to the queue
	* @param value New value to insert
	* @warning Only call if !full()
	*/
	void push( const Data& value )
	{
		uint8_t back = back_; //< @note Cache locally as back_ is volatile
		data_[back] = value;    
		back_ = next(back);    
	}
  
private:  
	volatile Index front_; ///< Index of front write-index
	volatile Index back_; ///< index of back read-index
	Data data_[cCapacity]; ///< Data storage buffer
};

#endif
