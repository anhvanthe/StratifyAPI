//Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#include <cstdlib>
#include <cstring>
#include "fmt/Son.hpp"

using namespace fmt;

Son::Son(u16 max_depth, son_stack_t * stack){
	memset(&m_son, 0, sizeof(m_son));
	m_stack_size = max_depth;
	if( stack == 0 ){
		m_is_stack_needs_free = true;
		m_stack = (son_stack_t*)malloc(max_depth * sizeof(son_stack_t));
	} else {
		m_is_stack_needs_free = false;
		m_stack = stack;
	}

}


Son::~Son(){
	if( m_is_stack_needs_free ){
		free(m_stack);
	}
}

