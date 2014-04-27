#include "renderer.hpp"
#include "viewport.hpp"

using namespace renderer;

VIEWPORT::VIEWPORT(int x, int y, int w, int h)
{
    this->x = x > 0 ? x : 0;
    this->y = y > 0 ? y : 0;
    this->w = w > 0 ? w : 0;
    this->h = h > 0 ? h : 0;
}

void VIEWPORT::apply() const
{
    glViewport(x, y, w, h);
}

void VIEWPORT::clear()
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
