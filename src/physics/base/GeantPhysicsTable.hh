//----------------------------------*-C++-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file GeantPhysicsTable.hh
//---------------------------------------------------------------------------//
#pragma once

#include <vector>

namespace celeritas
{
//---------------------------------------------------------------------------//
/*!
 * Struct to store Geant4 asc physics tables.
 */
struct GeantPhysicsTable
{
    int                              tableSize_;
    std::vector<double>              edgeMin_;
    std::vector<double>              edgeMax_;
    std::vector<int>                 numberOfNodes_;
    std::vector<int>                 vectorType_;
    std::vector<std::vector<double>> binVector_;
    std::vector<std::vector<double>> dataVector_;
};

//---------------------------------------------------------------------------//
} // namespace celeritas
