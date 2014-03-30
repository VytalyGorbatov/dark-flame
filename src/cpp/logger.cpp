#include <string.h>
#include <stdarg.h>
#include <fstream>
#include "logger.hpp"

using namespace std;

unsigned int LFILE::note = 0;

LFILE::LFILE()
{

}

LFILE::LFILE(const char* name, const char* text)
{
    fstream* handler = new fstream;

    handler->open(name, ios::binary | ios::out | ios::trunc);
    sprintf((char*)logname, "%s", name);
    if(handler->fail()) {
        handler->close();
        return;
    }

    if (text) {
        handler->write((char*)text, strlen(text));
    }

    handler->close();
    delete handler;
}

LFILE::~LFILE()
{

}

void LFILE::addf(const char* text, ...)
{
    va_list argptr;
    char tmp1[1024];
    char tmp2[1024];

    va_start(argptr, text);
    vsprintf(tmp1, text, argptr);
    va_end(argptr);

    sprintf(tmp2,"%s", tmp1);
    add(tmp2);
}

void LFILE::add(const char* text)
{
    char line[1024];
    char nnote[128];

    fstream* handler = new fstream;
    handler->open((char*)logname, ios::binary | ios::out | ios::app);
    if(handler->fail()) {
        handler->close();
        return;
    }

    memset(nnote, 0, 128);
    sprintf(nnote, " <%d> ", note);
    handler->write((char*)nnote, strlen(nnote));
    note++;
    memset(line, 0, 1024);
    sprintf(line, "%s", text);
    handler->write((char*)line, strlen(line));
    handler->close();

    delete handler;
}

LFILE DFLOG("log.txt", "PROJECT\tDark Flame\n\n");
