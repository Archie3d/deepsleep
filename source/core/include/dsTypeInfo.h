#ifndef DS_TYPEINFO_H
#define DS_TYPEINFO_H

#include <typeinfo>

namespace ds {

class TypeInfo
{
public:
    explicit TypeInfo(const std::type_info &info)
        : m_typeInfo(info)
    {
    }

    bool operator < (const TypeInfo &info) const
    {
        return m_typeInfo.before(info.m_typeInfo) != 0;
    }

private:
    const std::type_info &m_typeInfo;
};

} // namespace ds

#endif // DS_TYPEINFO_H
