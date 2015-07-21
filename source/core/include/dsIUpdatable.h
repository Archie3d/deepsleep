#ifndef DS_IUPDATABLE_H
#define DS_IUPDATABLE_H

namespace ds {

/**
 * Interface for updatable objects.
 */
class IUpdatable
{
public:

    virtual void update(int deltaMs) = 0;

    ~IUpdatable() {}
};

} // namespace ds

#endif // DS_IUPDATABLE_H
