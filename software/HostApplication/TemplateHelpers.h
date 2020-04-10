#ifndef TEMPLATEHELPERS_H
#define TEMPLATEHELPERS_H

#include <iterator>
#include <type_traits>

namespace DIYV
{
    template <typename R>
    typename R::iterator_type make_iterator(R reverse_iterator)
    {
      static_assert(std::is_same<R, typename std::reverse_iterator<typename R::iterator_type>>::value,
        "Passed argument is not a reverse_iterator!");
      return --reverse_iterator.base();
    }
}

#endif // TEMPLATEHELPERS_H
