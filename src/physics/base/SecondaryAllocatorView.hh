//----------------------------------*-C++-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file SecondaryAllocatorView.hh
//---------------------------------------------------------------------------//
#pragma once

#include "Secondary.hh"
#include "base/StackAllocatorView.hh"

namespace celeritas
{
//---------------------------------------------------------------------------//
//!@{
//! Type aliases for secondary allocation
using SecondaryAllocatorView = StackAllocatorView<Secondary>;
//!@}

//---------------------------------------------------------------------------//
} // namespace celeritas
