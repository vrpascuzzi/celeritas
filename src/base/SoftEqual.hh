//----------------------------------*-C++-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file SoftEqual.hh
//---------------------------------------------------------------------------//
#ifndef base_SoftEqual_hh
#define base_SoftEqual_hh

#include "detail/SoftEqualTraits.hh"

namespace celeritas
{
//---------------------------------------------------------------------------//
/*!
 * Functor for floating point equality.
 *
 * \note This functor is *not commutative*: eq(a,b) will not always give the
 * same as eq(b,a).
 *
 * \param rel tolerance of relative error (default 1.0e-12 for doubles)
 *
 * \param abs threshold for absolute error when comparing to zero
 *           (default 1.0e-14 for doubles)
 */
template<typename T1, typename T2 = T1>
class SoftEqual
{
  public:
    //@{
    //! Type aliases
    using first_argument_type  = T1;
    using second_argument_type = T2;
    using value_type =
        typename detail::SoftPrecisionType<first_argument_type,
                                           second_argument_type>::type;
    //@}

  public:
    // Construct with default relative/absolute precision
    inline SoftEqual();

    // Construct with default absolute precision
    inline explicit SoftEqual(value_type rel);

    // Construct with both relative and absolute precision
    inline SoftEqual(value_type rel, value_type abs);

    // >>> COMPARISON

    // Compare two values (implicitly casting arguments)
    bool operator()(value_type expected, value_type actual) const;

    // >>> ACCESSORS

    //! Relative allowable error
    value_type rel() const { return d_rel; }

    //! Absolute tolerance
    value_type abs() const { return d_abs; }

  private:
    // >>> DATA

    value_type d_rel;
    value_type d_abs;

    using traits_t = detail::SoftEqualTraits<value_type>;
};

//---------------------------------------------------------------------------//
/*!
 * Functor for floating point equality.
 *
 * \param abs threshold for absolute error when comparing to zero
 *           (default 1.0e-14 for doubles)
 */
template<typename T>
class SoftZero
{
  public:
    //@{
    //! Type aliases
    using argument_type = T;
    using value_type    = T;
    using traits_t      = detail::SoftEqualTraits<value_type>;
    //@}

  private:
    // >>> DATA

    value_type d_abs;

  public:
    // >>> CONSTRUCTION

    // Construct with default relative/absolute precision
    inline SoftZero();

    // Construct with absolute precision
    inline explicit SoftZero(value_type abs);

    // >>> COMPARISON

    // Compare the given value to zero
    inline bool operator()(value_type actual) const;

    // >>> ACCESSORS

    //! Absolute tolerance
    value_type abs() const { return d_abs; }
};

//---------------------------------------------------------------------------//
} // namespace celeritas

#include "SoftEqual.i.hh"

#endif // base_SoftEqual_hh