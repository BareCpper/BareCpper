#if 1

#ifdef __cplusplus
extern "C" {
#endif
    void dbg_where(const char* tag, const bool endLine) __attribute__((weak)) {};
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
class ScopeDebug
{
public:
    ScopeDebug(const char* tag)
    {
        dbg_where(tag, false);
    }

    void tag()
    {
        dbg_where(pos, false);
        ++pos[1]; //< INcrement the counter-character i.e ".0", ".1", ".2" etc
    }

    ~ScopeDebug()
    {
        dbg_where("=DONE", true);
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

#endif