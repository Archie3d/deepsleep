#ifndef DS_VARIANT_H
#define DS_VARIANT_H

#include <string>
#include <vector>
#include <map>


namespace ds {

class Variant;

typedef std::vector<Variant> VariantList;
typedef std::map<std::string, Variant> VariantMap;

class Variant
{
public:

    typedef enum {
        Type_Invalid    = 0,
        Type_Boolean    = 1,
        Type_Int        = 2,
        Type_Long       = 3,
        Type_Float      = 4,
        Type_String     = 5,
        Type_List       = 6,
        Type_Map        = 7,

        Type_Max = Type_Map + 1
    } Type;

    Variant();
    Variant(Type t);
    Variant(bool v);
    Variant(int v);
    Variant(long long v);
    Variant(float v);
    Variant(double v);
    Variant(const char *p);
    Variant(const std::string &v);
    Variant(const VariantList &v);
    Variant(const VariantMap &v);
    Variant(const Variant &v);
    Variant& operator =(const Variant &v);
    Variant& operator =(bool v);
    Variant& operator =(int v);
    Variant& operator =(long long v);
    Variant& operator =(float v);
    Variant& operator =(double v);
    Variant& operator =(const char *p);
    Variant& operator =(const std::string &v);
    Variant& operator =(const VariantList &v);
    Variant& operator =(const VariantMap &v);

    ~Variant();

    Type type() const { return m_type; }
    bool isValid() const { return m_type != Type_Invalid; }
    bool toBoolean(bool def = false) const;
    int toInteger(int def = 0) const;
    long long toLong(long long def = 0L) const;
    float toFloat(float def = 0.0f) const;
    double toDouble(double def = 0.0) const;
    std::string toString() const;

protected:

    void initFrom(const Variant &v);
    void clear();

private:

    Type m_type;

    union Data {
        bool b;
        char c;
        int i;
        long long l;
        double f;
        void *ptr;
    } m_data;
};

} // namespace ds

#endif // DS_VARIANT_H
