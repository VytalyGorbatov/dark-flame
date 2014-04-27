#include "logger.hpp"
#include "window_x.hpp"
#include "primitives.hpp"
#include "viewport.hpp"
#include "vector.hpp"

using namespace math;
using namespace window;
using namespace renderer;

int main()
{

    /* comment next line to logging */
    DFLOG.deny();

    /* create window (pseudo code to choose OS-dependent type) */
    int width = 600;
    int height = 600;

    WINDOW_X window_x;
    WINDOW* window = &window_x;
    window->configure("Accept test window", width, height);
    window->make_current();

    /* parameters for drawing objects */
    P3D view_point(-0.1f, 1, 1);
    P3D cube_origin(0, 0, 0);
    int cube_size(1);

    /* setting up camera */
    VIEWPORT viewport(0, 0, width, height);
    camera::MCAMERA* camera = new OCAMERA(view_point, cube_origin, -5, 5, 5, -5, 5, -5);

    /* drawing cycle */
    while (1) {
        viewport.apply();
        camera->apply();

        PRIMITIVES::draw_cube(cube_origin, cube_size);

        window->swap_buffers();
    }

    return 0;
}
