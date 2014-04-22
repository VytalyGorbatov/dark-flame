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
    int width = 640;
    int height = 480;

    WINDOW_X window_x;
    WINDOW* window = &window_x;
    window->configure("Accept test window", width, height);
    window->make_current();

    /* parameters for drawing objects */
    P3D view_point(0, 0, 1);
    P3D cube_origin(0, 0, 0);
    int cube_size(1);

    /* setting up camera */
    VIEWPORT viewport(0, 0, width, height);
    PCAMERA camera(view_point, cube_origin, 90, (float)width / height, 1, 100);

    /* drawing cycle */
    while (1) {
        viewport.apply();
        camera.apply();

        PRIMITIVES::draw_cube(cube_origin, cube_size);

        window->swap_buffers();
    }

    return 0;
}
