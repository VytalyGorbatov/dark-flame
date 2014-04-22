#ifndef __WINDOW_HPP__
#define __WINDOW_HPP__

namespace window
{

class WINDOW
{
public:
    virtual void configure(const char* name, int width, int height) = 0;
    virtual void make_current() = 0;
    virtual void swap_buffers() = 0;
};

} // namespace window

#endif  // __WINDOW_HPP__
