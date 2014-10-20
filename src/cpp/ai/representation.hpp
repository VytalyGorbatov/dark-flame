#ifndef __REPRESENTATION_HPP__
#define __REPRESENTATION_HPP__

namespace ai
{

/**
 * Inner Parameter.
 */
class PARAMETER
{
protected:
    int importance;
};

/**
 * Inner World, Representation.
 *
 * A set of parameters how they are known by AI,
 * active signals sended by analyzers.
 */
class REPRESENTATION
{
public:
    virtual ~REPRESENTATION() = 0;
};

} // namespace ai

#endif // __REPRESENTATION_HPP__
