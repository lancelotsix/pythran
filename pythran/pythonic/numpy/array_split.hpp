#ifndef PYTHONIC_NUMPY_ARRAYSPLIT_HPP
#define PYTHONIC_NUMPY_ARRAYSPLIT_HPP

#include "pythonic/include/numpy/array_split.hpp"

#include "pythonic/utils/functor.hpp"
#include "pythonic/types/ndarray.hpp"

PYTHONIC_NS_BEGIN

namespace numpy
{
  template <class T, class pS>
  types::list<types::ndarray<T, types::array<long, std::tuple_size<pS>::value>>>
  array_split(types::ndarray<T, pS> const &a, long nb_split)
  {
    long sz = std::distance(a.begin(), a.end());
    long n = (sz + nb_split - 1) / nb_split;
    long end = n * nb_split;
    long nb_full_split = nb_split;
    if (end != sz)
      nb_full_split -= (end - sz);
    types::list<types::ndarray<
        T, types::array<long, std::tuple_size<pS>::value>>> out(nb_split);
    long index = 0;
    for (long i = 0; i < nb_full_split; ++i, index += n)
      out[i] = a[types::contiguous_slice(index, index + n)];
    for (long i = nb_full_split; i < nb_split; ++i, index += (n - 1))
      out[i] = a[types::contiguous_slice(index, index + n - 1)];

    return out;
  }

  template <class T, class pS, class I>
  typename std::enable_if<
      types::is_iterable<I>::value,
      types::list<types::ndarray<
          T, types::array<long, std::tuple_size<pS>::value>>>>::type
  array_split(types::ndarray<T, pS> const &a, I const &split_mask)
  {
    long sz = std::distance(a.begin(), a.end());
    types::list<
        types::ndarray<T, types::array<long, std::tuple_size<pS>::value>>>
        out(1 + split_mask.flat_size());
    long index = 0;
    auto inserter = out.begin();
    for (auto next_index : split_mask) {
      *inserter++ = a[types::contiguous_slice(index, next_index)];
      index = next_index;
    }
    *inserter = a[types::contiguous_slice(index, sz)];
    return out;
  }

  NUMPY_EXPR_TO_NDARRAY0_IMPL(array_split);
}
PYTHONIC_NS_END

#endif
