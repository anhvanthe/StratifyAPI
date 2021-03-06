#include "hal/FFifo.hpp"

using namespace hal;

FFifo::FFifo(){}


int FFifo::init(const char * path, const FFifoAttr & attr, int o_flags){
    int ret;
    ret = set_error_number_if_error(open(path, o_flags | FFifo::READWRITE));
    if( ret < 0 ){
        return ret;
    }

    ret = set_attr(attr);
    if( ret < 0 ){
        return ret;
    }

    return set_error_number_if_error(ioctl(I_FFIFO_INIT));
}

int FFifo::flush() const {
    return set_error_number_if_error(ioctl(I_FFIFO_FLUSH));
}


int FFifo::set_attr(const FFifoAttr & attr) const {
    return set_error_number_if_error( ioctl(I_FFIFO_SETATTR, &attr.m_ffifo_attr) );
}

FFifoInfo FFifo::get_info() const {
    FFifoInfo info;
    set_error_number_if_error( ioctl(I_FFIFO_GETINFO, &info.m_ffifo_info) );
    return info;
}

int FFifo::get_info(FFifoInfo & info) const {
    return set_error_number_if_error( ioctl(I_FFIFO_GETINFO, &info.m_ffifo_info) );
}
