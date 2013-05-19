//
//  Displayer.h
//  GameDemo
//
//  Created by qiuhelin on 13-5-18.
//
//

#ifndef GameDemo_Displayer_h
#define GameDemo_Displayer_h
#include <iostream>
#include <cassert>
#include <vector>
#include <map>
#include <string>
#include <stdexcept>
#include <stdint.h>
#include <string.h>
#include <limits.h>

//////////////////////////////////////////////////////////////////////
/// 用于输出
class CDisplayObj
{
public:
    virtual void display(std::ostream& os, int level = 0) const = 0;
public:
    std::string m_sClassName;
};

class Displayer
{
    std::ostream&   _os;
    int             _level;
    
    void ps(const char * fieldName)
    {
        for(int i = 0; i < _level; ++i)
            _os << '\t';
        if(fieldName != NULL)
            _os << fieldName << ": ";
    }
public:
    explicit Displayer(std::ostream& os, int level = 0)
    : _os(os)
    , _level(level)
    {}
    
    Displayer& display(bool b, const char * fieldName)
    {
        ps(fieldName);
        _os << (b ? 'T' : 'F') << std::endl;
        return *this;
    }
    
    Displayer& display(char n, const char * fieldName)
    {
        ps(fieldName);
        _os << n << std::endl;
        return *this;
    }
    
    Displayer& display(unsigned char n, const char * fieldName)
    {
        ps(fieldName);
        _os << n << std::endl;
        return *this;
    }
    
    Displayer& display(short n, const char * fieldName)
    {
        ps(fieldName);
        _os << n << std::endl;
        return *this;
    }
    
    Displayer& display(unsigned short n, const char * fieldName)
    {
        ps(fieldName);
        _os << n << std::endl;
        return *this;
    }
    
    
    Displayer& display(int n, const char * fieldName)
    {
        ps(fieldName);
        _os << n << std::endl;
        return *this;
    }
    
    Displayer& display(unsigned int n, const char * fieldName)
    {
        ps(fieldName);
        _os << n << std::endl;
        return *this;
    }
    
    Displayer& display(long n, const char * fieldName)
    {
        ps(fieldName);
        _os << n << std::endl;
        return *this;
    }
    
    Displayer& display(unsigned long n, const char * fieldName)
    {
        ps(fieldName);
        _os << n << std::endl;
        return *this;
    }
    
    Displayer& display(float n, const char * fieldName)
    {
        ps(fieldName);
        _os << n << std::endl;
        return *this;
    }
    
    Displayer& display(double n, const char * fieldName)
    {
        ps(fieldName);
        _os << n << std::endl;
        return *this;
    }
    
    Displayer& display(const std::string& s, const char * fieldName)
    {
        ps(fieldName);
        _os << s << std::endl;
        return *this;
    }
    
    Displayer& display(const char *s, const size_t len, const char * fieldName)
    {
        ps(fieldName);
        for(unsigned i=0;i< len; i++) {
            _os << s[i];
        }
        _os<<std::endl;
        return *this;
    }
    
    Displayer& display(const CDisplayObj& obj, const char * fieldName)
    {
        ps(fieldName);
        _os << " {" << std::endl;
        obj.display(_os, _level + 1);
        _os << " }" << std::endl;
        return *this;
    }
    
    template <typename K, typename V, typename Cmp, typename Alloc >
    Displayer& display(const std::map<K, V, Cmp, Alloc>& m, const char * fieldName)
    {
        ps(fieldName);
        if(m.empty()){
            _os << m.size() << ", {}" << std::endl;
            return *this;
        }
        _os << m.size() << ", {" << std::endl;
        Displayer jd1(_os, _level + 1);
        Displayer jd(_os, _level + 2);
        typedef typename std::map<K, V, Cmp, Alloc>::const_iterator IT;
        IT f = m.begin(), l = m.end();
        for(; f != l; ++f){
            jd1.display('(', NULL);
            jd.display(f->first, NULL);
            jd.display(f->second, NULL);
            jd1.display(')', NULL);
        }
        display('}', NULL);
        return *this;
    }
    
    template < typename T, typename Alloc >
    Displayer& display(const std::vector<T, Alloc>& v, const char * fieldName)
    {
        ps(fieldName);
        if(v.empty()){
            _os << v.size() << ", []" << std::endl;
            return *this;
        }
        _os << v.size() << ", [" << std::endl;
        Displayer jd(_os, _level + 1);
        typedef typename std::vector<T, Alloc>::const_iterator  IT;
        IT f = v.begin(), l = v.end();
        for(; f != l; ++f)
            jd.display(*f, NULL);
        display(']', NULL);
        return *this;
    }
    
    template < typename T >
    Displayer& display(const T * v, const size_t len ,const char * fieldName)
    {
        ps(fieldName);
        if(len == 0){
            _os << len << ", []" << std::endl;
            return *this;
        }
        _os << len << ", [" << std::endl;
        Displayer jd(_os, _level + 1);
        for(size_t i=0; i< len; ++i)
            jd.display(v[i], NULL);
        display(']', NULL);
        return *this;
    }
    
    Displayer& displaySimple(bool b, bool bSep)
    {
        _os << (b ? 'T' : 'F') << (bSep ? "|" : "");
        return *this;
    }
    
    Displayer& displaySimple(char n, bool bSep)
    {
        _os << n << (bSep ? "|" : "");
        return *this;
    }
    
    Displayer& displaySimple(unsigned char n, bool bSep)
    {
        _os << n << (bSep ? "|" : "");
        return *this;
    }
    
    Displayer& displaySimple(short n, bool bSep)
    {
        _os << n << (bSep ? "|" : "");
        return *this;
    }
    
    Displayer& displaySimple(unsigned short n, bool bSep)
    {
        _os << n << (bSep ? "|" : "");
        return *this;
    }
    
    Displayer& displaySimple(int n, bool bSep)
    {
        _os << n << (bSep ? "|" : "");
        return *this;
    }
    
    Displayer& displaySimple(unsigned int n, bool bSep)
    {
        _os << n << (bSep ? "|" : "");
        return *this;
    }
    
    Displayer& displaySimple(long n, bool bSep)
    {
        _os << n << (bSep ? "|" : "");
        return *this;
    }
    
    Displayer& displaySimple(unsigned long n, bool bSep)
    {
        _os << n << (bSep ? "|" : "");
        return *this;
    }
    
    Displayer& displaySimple(float n, bool bSep)
    {
        _os << n << (bSep ? "|" : "");
        return *this;
    }
    
    Displayer& displaySimple(double n, bool bSep)
    {
        _os << n << (bSep ? "|" : "");
        return *this;
    }
    
    Displayer& displaySimple(const std::string& n, bool bSep)
    {
        _os << n << (bSep ? "|" : "");
        return *this;
    }
    
    Displayer& displaySimple(const char * n, const size_t len, bool bSep)
    {
        for(unsigned i=0;i< len; i++) {
            _os << n[i] ;
        }
        _os<<(bSep ? "|" : "");
        return *this;
    }

    template <typename K, typename V, typename Cmp, typename Alloc >
    Displayer& displaySimple(const std::map<K, V, Cmp, Alloc>& m, bool bSep)
    {
        if(m.empty()){
            _os << m.size() << "{}";
            return *this;
        }
        _os << m.size() << "{";
        Displayer jd1(_os, _level + 1);
        Displayer jd(_os, _level + 2);
        typedef typename std::map<K, V, Cmp, Alloc>::const_iterator IT;
        IT f = m.begin(), l = m.end();
        for(; f != l; ++f){
            if(f != m.begin()) _os << ',';
            jd.displaySimple(f->first, true);
            jd.displaySimple(f->second, false);
        }
        _os << '}' << (bSep ? "|" : "");
        return *this;
    }
    
    template < typename T, typename Alloc >
    Displayer& displaySimple(const std::vector<T, Alloc>& v, bool bSep)
    {
        if(v.empty()){
            _os << v.size() << "{}";
            return *this;
        }
        _os << v.size() << '{';
        Displayer jd(_os, _level + 1);
        typedef typename std::vector<T, Alloc>::const_iterator  IT;
        IT f = v.begin(), l = v.end();
        for(; f != l; ++f)
        {
            if(f != v.begin()) _os << "|";
            jd.displaySimple(*f, false);
        }
        _os << '}' << (bSep ? "|" : "");
        return *this;
    }
    
    template < typename T>
    Displayer& displaySimple(const T* v, const size_t len, bool bSep)
    {
        if(len == 0){
            _os << len << "{}";
            return *this;
        }
        _os << len << '{';
        Displayer jd(_os, _level + 1);
        for(size_t i=0; i <len ; ++i)
        {
            if(i != 0) _os << "|";
            jd.displaySimple(v[i], false);
        }
        _os << '}' << (bSep ? "|" : "");
        return *this;
    }
};
    ////////////////////////////////////////////////////////////////////////////////////////////////////
#endif
