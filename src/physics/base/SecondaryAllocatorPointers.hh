//----------------------------------*-C++-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file SecondaryAllocatorPointers.hh
//---------------------------------------------------------------------------//
#pragma once

#include "base/StackAllocatorPointers.hh"

namespace celeritas
{
//---------------------------------------------------------------------------//
/*!
 * View to data for allocating secondaries.
 */
struct SecondaryAllocatorPointers
{
    StackAllocatorPointers allocator;

    //! Check whether the pointers have been assigned
    explicit CELER_FUNCTION operator bool() const { return bool(allocator); }
};

//---------------------------------------------------------------------------//
} // namespace celeritas