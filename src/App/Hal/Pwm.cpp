//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

#include "Hal/Pwm.hpp"
using namespace Hal;

Pwm::Pwm(port_t port) : Periph(CORE_PERIPH_PWM, port){}

int Pwm::attr(pwm_attr_t * attr){
	return ioctl(I_PWM_GETATTR, attr);
}

int Pwm::setattr(const pwm_attr_t * attr){
	return ioctl(I_PWM_SETATTR, attr);
}

int Pwm::set(const pwm_reqattr_t * req){
	return ioctl(I_PWM_SET, req);
}

