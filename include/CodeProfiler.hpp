

/** @todo REFACTOR: Only tested in early Atmega platform wihtout newer interfaces etc
*/
struct AutoProfiler
{
    inline AutoProfiler(const Char* msg) :
        m_msg(msg),
        m_start(micros())
    {}
    inline ~AutoProfiler()
    {
        unsigned long end = micros();

        Serial.print(m_msg);
        Serial.println(end - m_start);
    }

    const Char* m_msg;
    unsigned long m_start;
};