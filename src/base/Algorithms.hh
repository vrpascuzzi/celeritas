//----------------------------------*-C++-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file Algorithms.hh
//---------------------------------------------------------------------------//
#pragma once

#include "Macros.hh"

namespace celeritas
{
//---------------------------------------------------------------------------//
/*!
 * Return the lower of two values.
 */
template<class T>
CELER_CONSTEXPR_FUNCTION const T& min(const T& a, const T& b)
{
    return (b < a) ? b : a;
}

//---------------------------------------------------------------------------//
/*!
 * Return the higher of two values.
 */
template<class T>
CELER_CONSTEXPR_FUNCTION const T& max(const T& a, const T& b)
{
    return (b > a) ? b : a;
}

//---------------------------------------------------------------------------//
/*!
 * Return an integer power of the input value.
 *
 * Example: \code
  assert(9.0 == ipow<2>(3.0));
  assert(256 == ipow<8>(2));
 \endcode
 */
template<unsigned int N, class T>
CELER_CONSTEXPR_FUNCTION T ipow(T v)
{
    return (N == 0)       ? 1
           : (N % 2 == 0) ? ipow<N / 2>(v) * ipow<N / 2>(v)
                          : v * ipow<(N - 1) / 2>(v) * ipow<(N - 1) / 2>(v);
}

//---------------------------------------------------------------------------//
/*!
 * Find the insertion point for a value.
 *
 * \todo Define an iterator adapter that dereferences using `__ldg` in
 * device code.
 * \todo Add a template on comparator if needed.
 * \todo Add a "lower_bound_index" that will use the native pointer difference
 * type instead of iterator arithmetic, for potential speedup on CUDA.
 */
template<class ForwardIt, class T>
inline CELER_FUNCTION ForwardIt lower_bound(ForwardIt first,
                                            ForwardIt last,
                                            const T&  value)
{
    auto count = last - first;
    while (count > 0)
    {
        auto      step   = count / 2;
        ForwardIt middle = first + step;
        if (*middle < value)
        {
            first = middle + 1;
            count -= step + 1;
        }
        else
        {
            count = step;
        }
    }
    return first;
}

//---------------------------------------------------------------------------//
} // namespace celeritas
