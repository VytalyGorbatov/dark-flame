#include "utils.hpp"
#include "vector.hpp"

static void test_add_points(void)
{
    P3D a(1, 1, 1);
    P3D b(-1, 2, 3);
    P3D exp(0, 3, 4);
    P3D act;

    act = a + b;

    ASSERT_EQUALS(&exp, &act);
}

static void test_sub_points(void)
{
    P3D a(1, 1, 1);
    P3D b(-1, 2, 3);
    P3D exp(2, -1, -2);
    P3D act;

    act = a - b;

    ASSERT_EQUALS(&exp, &act);
}

static void test_mult_point(void)
{
    P3D a(1, 2, -3);
    P3D exp(3, 6, -9);
    P3D act;

    act = a.mult_by(3);

    ASSERT_EQUALS(&exp, &act);
}


static void test_add(void)
{
    V3D a(1, 1, -1);
    V3D b(2, 0, 1);
    V3D exp(3, 1, 0);
    V3D act;

    act = a + b;

    ASSERT_EQUALS(&exp, &act);
}

static void test_sub(void)
{
    V3D a(1, 1, -1);
    V3D b(2, 0, 1);
    V3D exp(-1, 1, -2);
    V3D act;

    act = a - b;

    ASSERT_EQUALS(&exp, &act);
}

static void test_mult_by(void)
{
    V3D a(1, 2, -3);
    V3D exp(-2, -4, 6);
    V3D act;

    act = a.mult_by(-2);

    ASSERT_EQUALS(&exp, &act);
}

static void test_mult(void)
{
    V3D a(2, 0, 0);
    V3D b(0, 1, 0);
    V3D exp(0, 0, 2);
    V3D act;

    act = a * b;

    ASSERT_EQUALS(&exp, &act);
}

static void test_abs_mult(void)
{
    V3D a(2, 0, 0);
    V3D b(0, 3, 0);

    ASSERT_TRUE(0 == a.abs_mult(b));

    V3D c(2, 0, 0);
    V3D d(-2, 0, 0);

    ASSERT_TRUE(-4 == c.abs_mult(d));
}

static void test_full_mult(void)
{
    V3D a(2, 0, 0);
    V3D b(0, 3, 0);
    V3D c(0, 0, 4);

    ASSERT_TRUE(24 == a.full_mult(b ,c));

    V3D d(2, 0, 0);
    V3D e(0, 0, 3);
    V3D f(4, 0, 7);

    ASSERT_TRUE(0 == d.full_mult(e ,f));
}

static void test_get_length(void)
{
    V3D a(2, 0, 0);

    ASSERT_TRUE(2 == a.get_length());
}

static void test_set_length(void)
{
    V3D a(0, 0, 1);

    ASSERT_TRUE(1 == a.get_length());
    ASSERT_TRUE(a.set_length(4));
    ASSERT_TRUE(4 == a.get_length());
}

static void test_projection(void)
{
    V3D a(4, 0, 0);
    V3D act(1, 1, 0);
    V3D exp(1, 0, 0);

    ASSERT_TRUE(act.projection(a));

    ASSERT_EQUALS(&exp, &act);
}

void unit_vector(void)
{
    std::cout << __FUNCTION__ << std::endl;

    test_add_points();
    test_sub_points();
    test_mult_point();

    test_add();
    test_sub();
    test_mult_by();
    test_mult();
    test_abs_mult();
    test_full_mult();
    test_get_length();
    test_set_length();
    test_projection();
}
