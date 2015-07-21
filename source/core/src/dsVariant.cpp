#include "dsGlobal.h"
#include "dsVariant.h"

namespace ds {

static char typeSignature[Variant::Type_Max] = {
    'X',
    'B',
    'I',
    'L',
    'F',
    'S',
    '[',    // list
    '{'     // map
};

template <typename T>
std::string numberToString(T number)
{
    std::ostringstream ss;
    ss << number;
    return ss.str();
}

template <typename T>
T stringToNumber(const std::string &s)
{
    std::istringstream ss(s);
    T res;
    ss >> res;
    return res;
}

Variant::Variant()
    : m_type(Type_Invalid)
{
    m_data.ptr = nullptr;
}

Variant::Variant(Type t)
    : m_type(t)
{
    m_data.ptr = nullptr;
    switch (t) {
    case Type_String:
        m_data.ptr = new std::string();
        break;
    case Type_List:
        m_data.ptr = new VariantList();
        break;
    case Type_Map:
        m_data.ptr = new VariantMap();
        break;
    default:
        break;
    }
}

Variant::Variant(bool v)
    : m_type(Type_Boolean)
{
    m_data.b = v;
}

Variant::Variant(int v)
    : m_type(Type_Int)
{
    m_data.i = v;
}

Variant::Variant(long long v)
    : m_type(Type_Long)
{
    m_data.l = v;
}

Variant::Variant(float v)
    : m_type(Type_Float)
{
    m_data.f = v;
}

Variant::Variant(double v)
    : m_type(Type_Float)
{
    m_data.f = v;
}

Variant::Variant(const char *p)
    : m_type(Type_String)
{
    m_data.ptr = new std::string(p);
}

Variant::Variant(const std::string &v)
    : m_type(Type_String)
{
    m_data.ptr = new std::string(v);
}

Variant::Variant(const VariantList &v)
    : m_type(Type_List)
{
    m_data.ptr = new VariantList(v);
}

Variant::Variant(const VariantMap &v)
    : m_type(Type_Map)
{
    m_data.ptr = new VariantMap(v);
}

Variant::Variant(const Variant &v)
    : m_type(v.m_type)
{
    initFrom(v);
}

Variant& Variant::operator =(const Variant &v)
{
    if (this != &v) {
        clear();
        m_type = v.m_type;
        initFrom(v);
    }
    return *this;
}

Variant& Variant::operator =(bool v)
{
    if (m_type != Type_Boolean) {
        clear();
        m_type = Type_Boolean;
    }
    m_data.b = v;
    return *this;
}

Variant& Variant::operator =(int v)
{
    if (m_type != Type_Int) {
        clear();
        m_type = Type_Int;
    }
    m_data.i = v;
    return *this;
}

Variant& Variant::operator =(long long v)
{
    if (m_type != Type_Long) {
        clear();
        m_type = Type_Long;
    }
    m_data.l = v;
    return *this;
}

Variant& Variant::operator =(float v)
{
    if (m_type != Type_Float) {
        clear();
        m_type = Type_Float;
    }
    m_data.f = v;
    return *this;
}

Variant& Variant::operator =(double v)
{
    if (m_type != Type_Float) {
        clear();
        m_type = Type_Float;
    }
    m_data.f = v;
    return *this;
}

Variant& Variant::operator =(const char *p)
{
    return operator =(std::string(p));
}

Variant& Variant::operator =(const std::string &v)
{
    if (m_type != Type_String) {
        clear();
        m_type = Type_String;
        m_data.ptr = new std::string(v);
    } else {
        std::string *pStr = static_cast<std::string*>(m_data.ptr);
        *pStr = v;
    }
    return *this;
}

Variant& Variant::operator =(const VariantList &v)
{
    if (m_type != Type_List) {
        clear();
        m_type = Type_List;
        m_data.ptr = new VariantList(v);
    } else {
        VariantList *pList = static_cast<VariantList*>(m_data.ptr);
        *pList = v;
    }
    return *this;
}

Variant& Variant::operator =(const VariantMap &v)
{
    if (m_type != Type_Map) {
        clear();
        m_type = Type_Map;
        m_data.ptr = new VariantMap(v);
    } else {
        VariantMap *pMap = static_cast<VariantMap*>(m_data.ptr);
        *pMap = v;
    }
    return *this;
}

Variant::~Variant()
{
    clear();
}

bool Variant::toBoolean(bool def) const
{
    bool res = def;
    switch (m_type) {
    case Type_Boolean:
        res = m_data.b;
        break;
    case Type_Int:
        res = m_data.i != 0;
        break;
    case Type_Long:
        res = m_data.l != 0L;
        break;
    case Type_String: {
        std::string *pString = static_cast<std::string*>(m_data.ptr);
        res = *pString == "true";
        break;
    }
    default:
        break;
    }
    return res;
}

int Variant::toInteger(int def) const
{
    int res = def;
    switch (m_type) {
    case Type_Boolean:
        res = m_data.b ? 1 : 0;
        break;
    case Type_Int:
        res = m_data.i;
        break;
    case Type_Long:
        res = static_cast<int>(m_data.l);
        break;
    case Type_Float:
        res = static_cast<int>(m_data.f);
        break;
    case Type_String: {
        std::string *pString = static_cast<std::string*>(m_data.ptr);
        res = stringToNumber<int>(*pString);
        break;
    }
    default:
        break;
    }
    return res;
}

void Variant::initFrom(const Variant &v)
{
    switch (m_type) {
    case Type_String: {
        std::string *pStr = static_cast<std::string*>(v.m_data.ptr);
        m_data.ptr = new std::string(*pStr);
        break;
    }
    case Type_List: {
        VariantList *pList = static_cast<VariantList*>(v.m_data.ptr);
        m_data.ptr = new VariantList(*pList);
        break;
    }
    case Type_Map: {
        VariantMap *pMap = static_cast<VariantMap*>(v.m_data.ptr);
        m_data.ptr = new VariantMap(*pMap);
        break;
    }
    default:
        memcpy(&m_data, &v.m_data, sizeof(Data));
        break;
    }
}

void Variant::clear()
{
    switch (m_type) {
    case Type_String: {
        std::string *pStr = static_cast<std::string*>(m_data.ptr);
        delete pStr;
        break;
    }
    case Type_List: {
        VariantList *pList = static_cast<VariantList*>(m_data.ptr);
        delete pList;
        break;
    }
    case Type_Map: {
        VariantMap *pMap = static_cast<VariantMap*>(m_data.ptr);
        delete pMap;
        break;
    }
    default:
        break;
    }
}

} // namespace
