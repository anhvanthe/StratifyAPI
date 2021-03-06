//Copyright 2011-2018 Tyler Gilbert; All Rights Reserved


#include <errno.h>
#include <cctype>
#include <cstdio>
#include <cstdarg>
#include <cstring>
#include "var/String.hpp"
#include "sys.hpp"

using namespace var;

String::String(){
    //set up the minimum size
    set_capacity(minimum_size()-1);
    clear();
}

String::String(u32 capacity){
    if( (capacity > 0) && (capacity != npos) ){
        set_capacity(capacity);
        clear();
    }
}

String::String(const char * s){
    assign(s);
}

String::String(const char * s, u32 len){
    assign(s, len);
}


String::String(char * mem, u32 capacity, bool readonly) : Data((void*)mem, capacity, readonly){
    if( !readonly ){
        clear();
    }
}

u32 String::capacity() const {
    if( Data::capacity() ){
        return Data::capacity() - 1;
    }
    return 0;
}

//assignment

float String::atoff() const {
#ifndef __link
    return ::atoff(c_str());
#else
    return ::atof(c_str());
#endif
}

int String::sprintf(const char * format, ...){
    int ret;
    va_list args;
    va_start (args, format);
    ret = vsnprintf(cdata(), capacity(), format, args);
    if( ret > (int)capacity() ){ //if the data did not fit, make the buffer bigger
        if( set_capacity(ret+1) >= 0 ){
            vsnprintf(cdata(), capacity(), format, args);
        }
    }
    va_end (args);
    return ret;
}

int String::set_capacity(u32 s){
    if( Data::set_capacity(s+1) < 0 ){
        return -1;
    }
    //ensure zero termination on resized string
    return 0;
}

char String::at(u32 pos) const {
    const char * s = c_str();
    if (pos < strlen(s)){
        return s[pos];
    }
    return 0;
}

int String::assign(const char * a){
    if( a == 0 ){
        clear();
        return 0;
    }
    u32 len = strlen(a);
    return assign(a, len);
}

int String::assign(const char * a, u32 n){
    if( a != 0 ){
        if( n == (u32)npos ){
            n = strlen(a);
        }
        if( set_capacity(n) < 0 ){
            return -1;
        }
        clear();
        strncpy(cdata(), a, n);
    } else {
        clear();
    }
    return 0;
}

int String::append(const char * a){
    if( a == 0 ){ return 0; }
    u32 len = size();
    u32 alen = strlen(a);
    set_capacity(len + alen); //try to make min capacity
    if( len == 0 ){
        clear(); //previous length was zero -- ensure string is valid
    }
    if( cdata() == 0 ){ return -1; }
    strncat(cdata(), a, capacity() - len);
    return 0;
}

int String::append(char c){
    u32 len = size();
    u32 alen = 1;
    set_capacity(len + alen + 1); //try to make min capacity
    if( cdata() == 0 ){ return -1; }
    strncat(cdata(),&c,1);
    return 0;
}

bool String::get_delimited_data(String & dest, int n, char sep, char term){
    dest.clear();
    int element = 0;
    int start = 0;
    int end = 0;
    char c;
    for(u32 i=0; i < size(); i++){
        c = at(i);
        if( (c == sep) || (c == term) ){
            end = i;
            if( n == element ){
                copy(dest.cdata(), end - start, start);
                return true;
            }
            element++;
            start = i+1;
        }
    }
    return false;
}

int String::calc_delimited_data_size(char sep, char term){
    int elements = 1;
    //return the total number of elements
    for(u32 i=0; i < size(); i++){
        if( at(i) == sep ){
            elements++;
        }
        if( at(i) == term ){
            break;
        }
    }
    return elements;
}


String& String::insert(u32 pos, const char * str){

    if( str == 0 ){
        return *this;
    }

    if( cdata() == 0 ){
        assign(str);
        return *this;
    }

    u32 s;
    u32 len = length();

    if( pos > len ){
        set_error_number(EINVAL);
        return *this;
    } else if( pos == len ){
        append(str);
    } else {

        //this needs a limit
        s = strlen(str);

        char buffer[len+1];

        if( set_capacity( len + s ) < 0 ){
            exit_fatal("failed to alloc for insert");
            return *this;
        }

        strncpy(buffer, cdata()+pos, len+1); //copy the existing string to buffer
        strncpy(cdata()+pos, str, capacity() - pos);
        strncat(cdata(), buffer, capacity());
    }


    return *this;
}

String& String::erase(u32 pos, u32 len){
    char * p = cdata();
    u32 s = size();
    if( p == 0 ){ return *this; }
    if( (len != npos) && (pos + len < s) ){
        int remaining;
        remaining = s - pos - len;
        if( remaining > 0 ){
            memcpy(p + pos, p + pos + len, remaining);
            p[pos+remaining] = 0;
        } else {
            p[pos] = 0;
        }
    } else if (pos < s ){
        p[pos] = 0;
    }
    return *this;
}


u32 String::copy(char * s, u32 len, u32 pos) const {
    const char * p = c_str();
    u32 siz = size();
    u32 n;
    if( pos < siz ){
        n = siz - pos;
        if( len > n ){
            len = n;
        }
        memcpy(s,p+pos,len);
        return len;
    }
    return 0;
}

String String::substr(u32 pos, u32 len) const {
    if( pos >= size() ){
        return String();
    }

    String ret;
    ret.assign(c_str() + pos, len);
    ret.set_transfer_ownership();
    return ret;
}


void String::to_upper(){
    u32 s = size();
    char * p = cdata();
    for(u32 i = 0; i < s; i++){
        p[i] = ::toupper(p[i]);
    }
}

void String::to_lower(){
    u32 s = size();
    char * p = cdata();
    for(u32 i = 0; i < s; i++){
        p[i] = ::tolower(p[i]);
    }
}

u32 String::find(const String & str, u32 pos) const{
    return find(str.c_str(), pos, str.size());
}

u32 String::find(const char * str, u32 pos) const {
    if( str == 0 ){
        return npos;
    }
    int len = strlen(str);
    return find(str, pos, len);
}

u32 String::find(const char c, u32 pos) const{
    char str[2];
    str[0] = c;
    str[1] = 0;
    return find(str, pos);
}

u32 String::find(const char * s, u32 pos, u32 n) const {
    //find s (length n) starting at pos
    if( s != 0 ){
        u32 i;
        for(i=pos; i < size(); i++){
            if( strncmp(c_str() + i, s, n) == 0 ){
                return i;
            }
        }
    }
    return npos;
}

u32 String::rfind(const String & str, u32 pos) const{
    return rfind(str.c_str(), pos, str.size());
}

u32 String::rfind(const char * str, u32 pos) const {
    if( str == 0 ){
        return npos;
    }
    int len = strlen(str);
    return rfind(str, pos, len);
}

u32 String::rfind(const char c, u32 pos) const{
    char str[2];
    str[0] = c;
    str[1] = 0;
    return rfind(str, pos);
}

u32 String::rfind(const char * s, u32 pos, u32 n) const {
    //find s (length n) starting at pos
    if( s != 0 ){
        s32 i;
        u32 len = strlen(s);
        if( n > len ){
            n = len;
        }
        for(i=size()-n; i >= (s32)pos; i--){
            if( strncmp(c_str() + i, s, n) == 0 ){
                return i;
            }
        }
    }
    return npos;
}

int String::compare(const String & str) const {
    return compare(str.c_str());
}

int String::compare(u32 pos, u32 len, const String & str) const{
    return strncmp(&(c_str()[pos]), str.c_str(), len);
}

int String::compare(u32 pos, u32 len, const String & str, u32 subpos, u32 sublen) const{
    int l_compared;
    int l_comparing;

    const char * compared = &(c_str()[pos]);
    const char * comparing = &(str.c_str()[subpos]);

    l_compared = strnlen(compared, len);
    l_comparing = strnlen(comparing, sublen);

    if( l_compared != l_comparing ){
        return l_comparing - l_compared;
    }

    return strncmp(compared, comparing, l_compared);
}

int String::compare(const char * s) const{
    if( s == 0 ){
        return npos;
    }
    return strcmp(s, c_str());
}

int String::compare(u32 pos, u32 len, const char * s){
    if( s == 0 ){
        return npos;
    }
    return strncmp(&(c_str()[pos]), s, len);
}

int String::compare(u32 pos, u32 len, const char * s, u32 n) const {
    u32 l;
    const char * str_at_position;
    if( s == 0 ){
        return npos;
    }

    str_at_position = &(c_str()[pos]);

    l = strnlen(str_at_position, n);
    if( l != n ){
        return l - n;
    }

    return strncmp(str_at_position, s, n);
}

void PathString::strip_suffix(){
    u32 dot;
    dot = rfind('.');
    if( dot != npos ){
        cdata()[dot] = 0;
    }
}

const char * PathString::file_name() const{
    u32 slash;
    slash = rfind('/');
    if( slash != npos ){
        return c_str() + slash + 1;
    }
    return c_str();
}




