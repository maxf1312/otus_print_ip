#pragma once
#ifndef __MYPRINTIP_H__
#define __MYPRINTIP_H__

#include <memory>
#include <utility>
#include <bitset>
#include <vector>
#include <list>
#include <algorithm>
#include "mytuple_helpers.h"
#include "mydbgtrace.h"

namespace otus_hw4{


	template<typename OStreamT, typename T, typename PostfixStrCharT = typename OStreamT::char_type, typename PrefixStrCharT = typename OStreamT::char_type>
	OStreamT& print_val(OStreamT& os, T v, PostfixStrCharT postfix = {}, PrefixStrCharT prefix = {} ) 
	{
		if (PrefixStrCharT() != prefix)
			os << prefix;
		
		os << v ; 
		
		if (PostfixStrCharT() != postfix)
			os << postfix;
		return os;
	}

	template<typename T
			,typename = std::enable_if_t<!std::is_class<T>::value && 
			                             (!std::is_integral<std::decay_t<T>>::value || 
										   std::is_same<std::decay_t<T>, char>::value), bool>
			>
	auto& print_ip0(std::ostream& os, T v)
	{
		DBG_TRACE( "print_ip0_end_not_int", v );
		return print_val(os, v, "" );
	}

	template<std::enable_if_t<std::is_same<uint8_t, uint8_t>::value, bool> = true>
	auto& print_ip0(std::ostream& os, uint8_t v)
	{
	 	DBG_TRACE( "print_ip0_uint8_", v );
	 	return print_val(os, static_cast<int16_t>(v) );
	}

	template<typename T
			,std::enable_if_t<(std::is_class<std::decay_t<T>>::value 
							   && (std::is_same<std::decay_t<T>, std::vector<typename T::value_type>>::value
							       || std::is_same<std::decay_t<T>, std::list<typename T::value_type>>::value)), bool> = true 
			>
	auto& print_ip0(std::ostream& os, const T& v)
	{
		DBG_TRACE( "print_ip0_vec_", v.size() );
		size_t el_idx = 0;
		for( const auto& el: v )
		{
			print_val(os, el, "", (el_idx++ ? "." : ""));
		}
		return os;
	}

	template<typename T
			,std::enable_if_t<!std::is_class<std::decay_t<T>>::value && std::is_integral<std::decay_t<T>>::value && 
						      !std::is_same<std::decay_t<T>, uint8_t>::value && 
						      !std::is_same<std::decay_t<T>, char>::value, bool> = true
			
			>
	auto& print_ip0(std::ostream& os, T v)
	{
		static_assert(!std::is_class<T>::value, "is class");
		std::vector<uint8_t> bytes;
		bytes.reserve(8);
		constexpr const std::decay_t<T> mask = 0xFF; 
		for( size_t N = sizeof(T)/sizeof(uint8_t); N-- > 0; v >>= 8 )
			bytes.push_back(static_cast<uint8_t>(v & mask));
		std::reverse(std::begin(bytes), std::end(bytes));
		
		for(size_t el_idx = 0; el_idx < bytes.size(); ++el_idx) 
		{ 
			if( el_idx )	print_ip0(os, ".");
			print_ip0(os, bytes.at(el_idx));
		} 
					 	
		return os;
	}

	template<typename T
			,std::enable_if_t<not (std::is_class<std::decay_t<T>>::value 
									&& (std::is_same<std::decay_t<T>, std::vector<typename T::value_type>>::value
									|| std::is_same<std::decay_t<T>, std::list<typename T::value_type>>::value)), bool> = true 
			>
	auto& print_ip0(std::ostream& os, T v)
	{
		 DBG_TRACE( "print_ip0_end_", v );
		 return print_val(os, v, "" );
	}
		
	template<typename T, class... Args>
	auto& print_ip0(std::ostream& os, T v, Args...tail_args )
	{
		DBG_TRACE( "print_ip0_", v );
		
		print_ip0(os, v);
		print_ip0(os, ' ');
		return print_ip0(os, tail_args... );
	}

	template<size_t elem_idx, class TupleT>
	struct TuplePrintHelper_t
	{
		static std::ostream&  print_tuple(std::ostream& os, TupleT const& t)
		{
			TuplePrintHelper_t<elem_idx - 1, TupleT>::print_tuple(os, t);
			return print_val(os, std::get<elem_idx - 1>(t), "", ".");
		}
	};

	template<class TupleT>
	struct TuplePrintHelper_t<1, TupleT>
	{
		static std::ostream& print_tuple(std::ostream& os, TupleT const& t)
		{
			return print_val(os, std::get<0>(t), "", "");
		}
	};

	template<class... TupleTypesT
			,std::enable_if_t<sizeof...(TupleTypesT) != 0, int> = 0
		    ,std::enable_if_t<TupleElemSameType<std::tuple<TupleTypesT...>>::value == true, int> = 0
	>
	auto& print_ip0(std::ostream& os, std::tuple<TupleTypesT...> tuple)
	{
		 DBG_TRACE( "print_ip0_tuple", "tuple" );
		 using tuple_t = std::tuple<TupleTypesT...>;
		 return TuplePrintHelper_t<std::tuple_size<tuple_t>::value, tuple_t>::print_tuple(os, tuple);
	}

	template<class... TupleTypesT
			,std::enable_if_t<sizeof...(TupleTypesT) == 0, int> = 0
	>
	auto& print_ip0(std::ostream& os, std::tuple<TupleTypesT...> )
	{
		 DBG_TRACE( "print_ip0_empty_tuple", "tuple" );
		 return os;
	}
 
	template<typename T>
	auto& print_ip(T&& v)
	{
		 DBG_TRACE( "print_ip_end_", v );
		 return print_ip0(std::cout, std::forward<T&&>(v) );
	}

	template<typename T, class... Args>
	auto& print_ip(T&& v, Args&&...tail_args )
	{
		DBG_TRACE( "print_ip_", v );
		
		print_ip(std::forward<T&&>(v));
		std::cout << std::endl;
		return print_ip( std::forward<Args&&...>(tail_args...) );
	}

} // otus_hw4

#endif //__MYALLOCATOR_H__
