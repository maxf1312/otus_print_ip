/**
 * @file myprintip.h
 * @author @MaximF
 * @brief Реализация печати условного ip адреса в рамках ДЗ4
 * @version 0.1
 * @date 2025-04-09
 * 
 * @copyright Copyright (c) 2025 
 * 
 * @details print_ip0 - шаблоны функции реализации для вывода в указанный поток. 
 * 
 */
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

	/// @brief Шаблон вывода значения, обрамленного префиксом и суффиксом
	/// @tparam OStreamT - тип потока
	/// @tparam T  - тип значения
	/// @tparam PrefixStrCharT  - тип префискса
	/// @tparam PostfixStrCharT - тип суффикса 
	/// @param os - поток
	/// @param v - значение
	/// @param postfix - суффикс
	/// @param prefix  - префикс
	/// @return - поток после вывода в него значения v
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

	/// @brief шаблон для одиночного не целочисленного значения 
	/// @tparam T тип
	/// @tparam анонимный тип для SFINAE 
	/// @param os поток для вывода
	/// @param v  значение для вывода
	/// @return поток после вывода в него
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

	/// @brief специализация для uint8_t, выводит как беззнаковое целое  [0,255]
	/// @tparam  bool = true для SFINAE
	/// @param os поток для вывода
	/// @param v  значение для вывода
	/// @return поток после вывода в него
	template<std::enable_if_t<std::is_same<uint8_t, uint8_t>::value, bool> = true>
	auto& print_ip0(std::ostream& os, uint8_t v)
	{
	 	DBG_TRACE( "print_ip0_uint8_", v );
	 	return print_val(os, static_cast<int16_t>(v) );
	}

	/// @brief перегрузка шаблона для контейнеров vector<> list<>
	/// @tparam T тип контейнера
	/// @tparam  bool = true для SFINAE
	/// @param os поток для вывода
	/// @param v  значение для вывода
	/// @return поток после вывода в него
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

	/// @brief перегрузка шаблона для целочисленного типа
	/// @tparam T 
	/// @tparam  bool = true для SFINAE
	/// @param os поток для вывода
	/// @param v  значение для вывода
	/// @return поток после вывода в него
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

	/// @brief перегрузка шаблона для класса, но не list<> или vector<>
	/// @tparam T 
	/// @tparam  bool = true для SFINAE
	/// @param os поток для вывода
	/// @param v  значение для вывода
	/// @return поток после вывода в него
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
		
	/// @brief перегрузка для варианта вывода нескольких значений
	/// @tparam T первое занчение из списка для вывода
	/// @tparam ...Args - типы остальных значений 
	/// @tparam  bool = true для SFINAE
	/// @param os поток для вывода
	/// @param v  значение для вывода
	/// @param ...tail_args - остальные значения
	/// @return поток после вывода в него
	template<typename T, class... Args>
	auto& print_ip0(std::ostream& os, T v, Args...tail_args )
	{
		DBG_TRACE( "print_ip0_", v );
		
		print_ip0(os, v);
		print_ip0(os, ' ');
		return print_ip0(os, tail_args... );
	}

	/// @brief общий случай шаблона вспомогательной структуры для печати кортежа  
	/// @tparam TupleT 
	/// @tparam elem_idx 
	template<size_t elem_idx, class TupleT>
	struct TuplePrintHelper_t
	{
		static std::ostream&  print_tuple(std::ostream& os, TupleT const& t)
		{
			TuplePrintHelper_t<elem_idx - 1, TupleT>::print_tuple(os, t);
			return print_val(os, std::get<elem_idx - 1>(t), "", ".");
		}
	};

	/// @brief специализация шаблона  вспомогательной структуры для печати кортежа для первого элемента кортежа
	/// @tparam TupleT 
	template<class TupleT>
	struct TuplePrintHelper_t<1, TupleT>
	{
		static std::ostream& print_tuple(std::ostream& os, TupleT const& t)
		{
			return print_val(os, std::get<0>(t), "", "");
		}
	};

	/// @brief общий случай шаблона печати кортежа по списку типов, кортеж не пустой и все элементы одного типа
	/// @tparam ...TupleTypesT 
	/// @tparam  
	/// @tparam  
	/// @param os 
	/// @param tuple 
	/// @return 
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

	/// @brief перегрузка шаблона для пустого кортежа - ничего не печатает
	/// @tparam ...TupleTypesT 
	/// @tparam  
	/// @param os 
	/// @param  
	/// @return 
	template<class... TupleTypesT
			,std::enable_if_t<sizeof...(TupleTypesT) == 0, int> = 0
	>
	auto& print_ip0(std::ostream& os, std::tuple<TupleTypesT...> )
	{
		 DBG_TRACE( "print_ip0_empty_tuple", "tuple" );
		 return os;
	}
 
	/// @brief перегрузка шаблона печати для вывода одного значения в std::cout
	/// @tparam T 
	/// @param v 
	/// @return 
	template<typename T>
	auto& print_ip(T&& v)
	{
		 DBG_TRACE( "print_ip_end_", v );
		 return print_ip0(std::cout, std::forward<T&&>(v) );
	}

	/// @brief перегрузка шаблона для списка значений 
	/// @tparam ...Args 
	/// @tparam T 
	/// @param v 
	/// @param ...tail_args 
	/// @return 
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
