#ifndef __LOGGER_HPP__
#define __LOGGER_HPP__

class LFILE
{
private:
    static unsigned int note;

    char logname[1024];

public:
    LFILE();
    LFILE(const char* name_output, const char* text = NULL);
    ~LFILE();

    void addf(const char* text, ...);
    void add(const char* text);
};

extern LFILE DFLOG;

#endif  // __LOGGER_HPP__
