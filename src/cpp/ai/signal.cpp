// TODO: use limits.h
#include "signal.hpp"

using namespace ai;

const int SIGNAL::max = 0x7fffffff;         // maximum value
const int SIGNAL::min = (1 << 31);          // minimum value

const float LEVEL::max = 1.0f;              // ceil value
const float LEVEL::min = 0.0f;              // floor value
