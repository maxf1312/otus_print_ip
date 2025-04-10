#pragma once
#ifndef __MYSFINAE_H__
#define __MYSFINAE_H__

#include <memory>
#include <utility>
#include <bitset>
#include <vector>
#include <list>
#include <algorithm>
#include "mydbgtrace.h"

/// 
/// @brief тестовые шаблоны для отладки SFINAE
/// 
/// @author @MaximF
/// @details отлаживаемся с помощью этих шаблонов, чтобы не тянуть реальный вывод
///          используем __PRETTY_FUNCTION__  и DBG_TRACE для трассировки 
///
namespace otus_hw4{

	/// @brief шаблон для простых не целочисленных типов 
	/// @tparam T тип значения
	/// @tparam условие 
	/// @param v значение
	template<typename T, 
			 std::enable_if_t<!std::is_class<T>::value && !std::is_integral<std::decay_t<T>>::value, bool> = true 
			>
	void func(T v)
	{
		static_assert(!std::is_class<T>::value, "is class");
		std::cout << __PRETTY_FUNCTION__ << ": " << v << std::endl;
	}

	/// @brief шаблон для uint8_t
	/// @tparam T 
	/// @tparam  
	/// @param v 
	template<typename T
			,std::enable_if_t<std::is_same<std::decay_t<T>, std::uint8_t>::value, bool> = true
	>
	void func(T v)
	{
		static_assert(!std::is_class<T>::value, "is class");
		std::cout << __PRETTY_FUNCTION__ << ": " << v << std::endl;
		std::cout << static_cast<int16_t>(v); 
	}

	/// @brief шаблон для классов, кроме вектора и списка
	/// @tparam T 
	/// @tparam  
	/// @param  
	template<typename T
			 ,std::enable_if_t<not (std::is_class<std::decay_t<T>>::value 
									&& (std::is_same<std::decay_t<T>, std::vector<typename T::value_type>>::value
									|| std::is_same<std::decay_t<T>, std::list<typename T::value_type>>::value)), bool> = true 
			>
	void func(T)
	{
		static_assert(std::is_class<std::decay_t<T>>::value, "not is class");
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}

	/// @brief шаблон для вектора
	/// @tparam T 
	/// @tparam выбор по условию для sfinae 
	/// @param  
	template<typename T, 
   			 std::enable_if_t<(std::is_class<std::decay_t<T>>::value && std::is_same<std::decay_t<T>, std::vector<typename T::value_type>>::value), bool> = true 
   	>
	void func(const T&)
	{
		static_assert(std::is_class<std::decay_t<T>>::value, "not is class");
		std::cout << __PRETTY_FUNCTION__ << std::endl;

	}

	/// @brief шаблон для списка
	/// @tparam T 
	/// @tparam  
	/// @param  
	template<typename T, 
			 std::enable_if_t<(std::is_class<std::decay_t<T>>::value && std::is_same<std::decay_t<T>, std::list<typename T::value_type>>::value), bool> = true 
   	>
	void func(const T&)
	{
		static_assert(std::is_class<std::decay_t<T>>::value, "not is class");
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}

	/// @brief шаблон для целочисленных
	/// @tparam T 
	/// @tparam  
	/// @param v 
	template<typename T
			,std::enable_if_t<!std::is_class<std::decay_t<T>>::value && std::is_integral<std::decay_t<T>>::value && 
						      !std::is_same<std::decay_t<T>, std::uint8_t>::value, bool> = true
			
	>
	void func(T v)
	{
		static_assert(!std::is_class<T>::value, "is class");
		std::cout << __PRETTY_FUNCTION__ << ": " << v << std::endl;
		std::vector<uint8_t> bytes;
		bytes.reserve(8);
		constexpr const std::decay_t<T> mask = 0xFF; 
		for( size_t N = sizeof(T)/sizeof(uint8_t); N-- > 0; v >>= 8 )
			bytes.push_back(static_cast<uint8_t>(v & mask));
		std::reverse(std::begin(bytes), std::end(bytes)); 	
		func( bytes );
	}

	/// @brief общий шаблон на список параметров
	/// @tparam T 
	/// @tparam ...Args 
	/// @param v 
	/// @param ...tail 
	template<typename T, class...Args>
	void func(T v, Args...tail)
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
		std::ignore = v;
		func( v );
		func( tail... );
	}

} // otus_hw4

#endif //__MYSFINAE_H__
