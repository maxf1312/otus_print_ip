#pragma once
#ifndef __MYPRINTIP_H__
#define __MYPRINTIP_H__

#ifndef __PRETTY_FUNCTION__
#include "pretty.h"
#endif

#include <memory>
#include <utility>
#include <bitset>

#ifdef USE_DBG_TRACE
#ifndef USE_PRETTY
#define DBG_TRACE( func, trace_statements )\
		std::cout << func << trace_statements << std::endl;
#else
#define DBG_TRACE( func, trace_statements )\
		std::cout << __PRETTY_FUNCTION__ << trace_statements << std::endl;
#endif
#else
#define DBG_TRACE( func, trace_statements )   
#endif // USE_DBG_TRACE

namespace otus_hw4{


} // otus_hw4

#endif //__MYALLOCATOR_H__
