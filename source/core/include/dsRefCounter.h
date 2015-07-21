#ifndef DS_REFCOUNTER_H
#define DS_REFCOUNTER_H

namespace ds {

/**
 * Simple reference counter
 */
class RefCounter
{
public:
    RefCounter(int count = 0) : m_count(count) {}

    void acquire() { ++m_count; }
    int release() { return --m_count; }
    int count() const { return m_count; }
private:
    /// Number of references
    int m_count;
};

} // namespace ds

#endif // DS_REFCOUNTER_H
