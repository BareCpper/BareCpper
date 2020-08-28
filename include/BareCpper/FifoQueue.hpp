#ifndef BARECPPER_FIFOQUEUE_H_
#define BARECPPER_FIFOQUEUE_H_

#include <cstdint>
#include "Common.hpp"

namespace BareCpper
{
    
    /** Simple Single-Producer Single-Consumer FIFO queue
    * @note FIFO queues offer strict first-in-first-out delivery so the order in which messages are sent and received is strictly preserved
    * @todo
    */
    template< typename DataT, size_t Capacity >
    class FifoQueueSPSC
    {
    public:
        typedef uint8_t Index;

        inline static Index next(Index index)
        {
            return (index < (Capacity - 1u) ? ++index : 0);
        }

    public:
        FifoQueueSPSC() 
            : front_(0)
            , back_(0)
            , data_{}
        {}

        constexpr uint8_t capacity() const
        { return Capacity; }

        uint8_t size() const
        { return back_ - front_; }

        bool isEmpty() const
        { return front_ == back_; }

        bool isFull() const
        { return  size() == capacity(); }

        const DataT& peek() const
        { return data_[front_ % Capacity]; } ///< @todo modulo % is slow on some platforms if capacity is not power-2

        /**
         * @note Return-value optimisation shall remove local temporary
        */
        DataT pop()
        {
            const DataT retVal = peek();
            front_++;
            return retVal;
        }

        void push(const DataT& value)
        { data_[back_++ % Capacity] = value; } ///< @todo modulo % is slow on some platforms if capacity is not power-2

    private:
        volatile Index front_;
        volatile Index back_;
        DataT data_[Capacity];
    };

} //END: BareCpper



    #if 0 //< @todo TEST codes!
    BareCpper::FifoQueueSPSC<uint32_t, 8U> eventQueue_ = {};
    static bool done = false;
    if(done)
    return;

    while(!Emteq::Serial);

    const uint32_t capacity = 8;
    const uint32_t testValues[capacity] = {12345678, 2345678, 345678, 45678, 5678, 678, 78, 8};

    Emteq::Serial.printf("%s capacity (%u == %u)\n", (eventQueue_.capacity() == capacity ? "pass" : "FAIL"), capacity, eventQueue_.capacity());
    Emteq::Serial.printf("%s size (%u == %u)\n", (0 == eventQueue_.size() ? "pass" : "FAIL"), 0, eventQueue_.size());
    Emteq::Serial.printf("%s isEmpty (%u == %u)\n", (eventQueue_.isEmpty() ? "pass" : "FAIL"), true, eventQueue_.isEmpty());
    Emteq::Serial.printf("%s isFull (%u == %u)\n", (!eventQueue_.isFull() ? "pass" : "FAIL"), false, eventQueue_.isFull());

    uint32_t count = 0;
    for(uint32_t sampleCount = capacity; sampleCount >= 1; --sampleCount)
    {
        const uint32_t indexOffset = (capacity - sampleCount); //< Offset so 0-index is not the same value every time!
        for(uint32_t index = 0; index < sampleCount; ++index)
        {
            const uint32_t testValue = testValues[count + indexOffset];

            eventQueue_.push(testValue);
            ++count;
            Emteq::Serial.printf("push(%u)\n", testValue);

            Emteq::Serial.printf("%s size (%u == %u)\n", (count == eventQueue_.size() ? "pass" : "FAIL"), count, eventQueue_.size());
            Emteq::Serial.printf("%s peek (%u == %u)\n", (testValues[indexOffset] == eventQueue_.peek() ? "pass" : "FAIL"), testValues[indexOffset], eventQueue_.peek());
            Emteq::Serial.printf("%s isEmpty (%u == %u)\n", (!eventQueue_.isEmpty() ? "pass" : "FAIL"), false, eventQueue_.isEmpty());
            Emteq::Serial.printf("%s isFull (%u == %u)\n", (eventQueue_.isFull() == (count == eventQueue_.capacity()) ? "pass" : "FAIL"), count == eventQueue_.capacity(), eventQueue_.isFull());
        }

        for(uint32_t index = 0; index < sampleCount; ++index)
        {
            const uint32_t testValue = testValues[index + indexOffset];

            Emteq::Serial.printf("%s peek (%u == %u)\n", (testValue == eventQueue_.peek() ? "pass" : "FAIL"), testValue, eventQueue_.peek());

            const uint32_t popped = eventQueue_.pop();
            --count;

            Emteq::Serial.printf("%s pop (%u == %u)\n", (popped == testValue ? "pass" : "FAIL"), testValue, popped);
            Emteq::Serial.printf("%s size (%u == %u)\n", (count == eventQueue_.size() ? "pass" : "FAIL"), count, eventQueue_.size());
            Emteq::Serial.printf("%s isFull (%u == %u)\n", (!eventQueue_.isFull() ? "pass" : "FAIL"), false, eventQueue_.isFull());
            Emteq::Serial.printf("%s isEmpty (%u == %u)\n", (eventQueue_.isEmpty() == (count == 0) ? "pass" : "FAIL"), count == 0, eventQueue_.isEmpty());
        }
    }

    done = true;
    #endif

#endif