#ifndef __SIGNAL_HPP__
#define __SIGNAL_HPP__

namespace ai
{

/**
 * Special type to represent signal, parameters value.
 */
class SIGNAL
{
public:
    static const int max;                   // maximum value
    static const int min;                   // minimum value
    int value;                              // value of signal or parameter

public:
    SIGNAL() : value(0) {}
    SIGNAL(int v) : value(v) {}

    SIGNAL operator -()
    {
        SIGNAL s(-value);
        return s;
    }
};

inline SIGNAL operator +(SIGNAL lh, const SIGNAL& rh)
{
    int r = lh.value + rh.value;
    lh.value = r > lh.value ? r : SIGNAL::max;
    return lh;
}

inline SIGNAL operator -(SIGNAL lh, const SIGNAL& rh)
{
    int r = lh.value - rh.value;
    lh.value = r < lh.value ? r : SIGNAL::min;
    return lh;
}

/**
 * Special type to represent potential, influence level.
 */
class LEVEL
{
public:
    static const float max;                 // ceil value
    static const float min;                 // floor value
    float value;                            // normalized value

public:
    LEVEL() : value(0) {}
    LEVEL(float v) : value(v) {}
};

} // namespace ai

#endif // __SIGNAL_HPP__
