#ifndef __VIEWPORT_HPP__
#define __VIEWPORT_HPP__

namespace renderer
{

class VIEWPORT
{
private:
    int x;
    int y;
    int w;
    int h;

public:
    VIEWPORT(int x_left_bot, int y_left_bot, int width, int height);

    void apply() const;

    static void clear();
};

} // namespace renderer

#endif  // __VIEWPORT_HPP__
