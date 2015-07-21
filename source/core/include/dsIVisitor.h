#ifndef DS_IVISITOR_H
#define DS_IVISITOR_H

namespace ds {

/**
 * Generic interface for visitor template.
 */
template <typename T>
class IVisitor
{
public:
    virtual void visit(T *pT) = 0;
    virtual ~IVisitor() {}
};

} // namespace ds

#endif // DS_IVISITOR_H
