#pragma once
#ifndef __MYTUPLE_HELPERS_H__
#define __MYTUPLE_HELPERS_H__

#include <memory>
#include <tuple>

namespace otus_hw4{

	template<typename SomeT, typename TupleT>
	struct TupleIndexHelper_t;
	
	template<typename SomeT>
	struct TupleIndexHelper_t<SomeT, std::tuple<>>
	{
		using type = SomeT;
		static constexpr size_t value = 0;
	};

	template<typename SomeT, class... TailT>
	struct TupleIndexHelper_t<SomeT, std::tuple<SomeT, TailT...>>
	{
		using type = SomeT;
		static constexpr size_t value = 0;
		using tail_tuple_t = std::tuple<TailT...>;
		static_assert( 
			TupleIndexHelper_t<SomeT, tail_tuple_t>::value == std::tuple_size<tail_tuple_t>::value,
			"not is the same element type");
	};

	template<typename SomeT, typename FirstT, class... TailT>
	struct TupleIndexHelper_t<SomeT, std::tuple<FirstT, TailT...>>
	{
		using type = SomeT;
		using first_type = FirstT;
		using tail_tuple_t = std::tuple<TailT...>;
		static constexpr size_t value = 1 + TupleIndexHelper_t<SomeT, tail_tuple_t>::value;
	};

	template<typename SomeT, typename TupleT>
	struct TupleIndex_t
	{
		static constexpr size_t value = TupleIndexHelper_t<SomeT, TupleT>::value;
		static_assert(value < std::tuple_size<TupleT>::value, "tuple does not have type");
	};

	template<size_t elem_idx, typename TupleT>
	struct TupleElemTypeHelper
	{
		using type = typename std::tuple_element<elem_idx - 1, TupleT>::type;
		static constexpr const bool value = std::is_same<type, typename TupleElemTypeHelper<elem_idx - 1, TupleT>::type>::value;
	};

	template<typename TupleT>
	struct TupleElemTypeHelper<1, TupleT>
	{
		using type = typename std::tuple_element<0, TupleT>::type;
		static constexpr const bool value = true;
	};

	template<typename TupleT>
	struct TupleElemTypeHelper<0, TupleT>
	{
		using type = void;
		static constexpr const bool value = true;
	};

	template<typename TupleT>
	struct TupleElemSameType
	{
		static constexpr const bool value = TupleElemTypeHelper<std::tuple_size<TupleT>::value, TupleT>::value;
	};

} // otus_hw4

#endif //__MYTUPLE_HELPERS_H__
