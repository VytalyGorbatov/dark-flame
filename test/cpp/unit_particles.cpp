#include "utils.hpp"
#include "particle.hpp"

static void test_particle_ttl(void)
{
    P3D p(0, 0, 0);
    P3D v(0, 0, 0);
    PARTICLE a(p, 0, 0, v, 0, 10);

    ASSERT_TRUE(a.update(1));
    ASSERT_TRUE(a.is_alive());

    ASSERT_TRUE(a.update(7));
    ASSERT_TRUE(a.is_alive());

    ASSERT_TRUE(a.update(1));
    ASSERT_TRUE(a.is_alive());

    ASSERT_TRUE(!a.update(2));
    ASSERT_TRUE(!a.is_alive());

    ASSERT_TRUE(!a.update(4));
    ASSERT_TRUE(!a.is_alive());
}

static void test_emitter(void)
{
    P3D p(1, 2, 3);
    EMITTER e;

    e.init(p, 100, 10000);
    e.p_ttl = 1000;

    e.update(10);
    e.update(10);
    e.update(10);

    e.start_emission();
    e.update(50);
    e.update(50);
    e.update(50);

    e.stop_emission();
    e.update(50);
    e.update(50);
    e.update(50);
}

void unit_particles(void)
{
    std::cout << __FUNCTION__ << std::endl;

    test_particle_ttl();
    test_emitter();
}
