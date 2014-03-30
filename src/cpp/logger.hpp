#ifndef __LOGGER_HPP__
#define __LOGGER_HPP__

#include <fstream>

class LFILE
{
private:
    static unsigned int note;

    char file_name[1024];
    std::fstream* fhdl;

public:
    LFILE();
    LFILE(const char* name_output);
    LFILE(const char* name_output, const char* text);
    ~LFILE();

    LFILE(const LFILE&);
    LFILE& operator =(const LFILE&);

    void addf(const char* text, ...);
    void add(const char* text);
    void deny();
};

extern LFILE DFLOG;

#endif  // __LOGGER_HPP__
