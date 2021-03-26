#if 0
#include "Serial.hpp" //< Emteq::Serial//< TEMPEMPEMPMEPME

extern "C" void yield(void);
extern "C" void delay(unsigned long ms);

static void dbg_where(const char* tag, const bool endLine)
{
#if 0
    endLine ? Emteq::Serial.println(tag) : Emteq::Serial.print(tag);
    for (int i = 0; i < 100; ++i)
    {
        ::yield(); // yield run usb background task
        ::delay(10);
    }
#endif
}
static void dbg_start(const char* tag)
{
    dbg_where(tag, false);
}
static void dbg_end()
{
    dbg_where("=DONE", true);
}

class ScopeDebug
{
public:
    ScopeDebug(const char* tag)
    {
        dbg_start(tag);
    }

    void tag()
    {
        dbg_where(pos, false);
        ++pos[1];
    }

    ~ScopeDebug()
    {
        dbg_end();
    }

private:
    char pos[3] = { '.', '0', '\0' };
};

class TagDebug
{
public:
    TagDebug(const char* tag)
    {
        dbg_where(tag, true);
    }
};


#endif