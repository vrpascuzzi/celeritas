//----------------------------------*-C++-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file KleinNishinaSampleSecondaries.i.hh
//---------------------------------------------------------------------------//

#include <cmath>

#include "base/Assert.hh"
#include "physics/base/ConstantsAndUnits.hh"
#include "KleinNishinaSampleSecondaries.hh"


namespace celeritas {
//---------------------------------------------------------------------------//
/*!
 * Construct with defaults.
 */
KleinNishinaSampleSecondaries::KleinNishinaSampleSecondaries()
:
{}

// Need to correct the operator() input parameters
void KleinNishinaSampleSecondaries::operator()(double parameter) const;

    // The scattered gamma energy is sampled according to Klein - Nishina formula.
    // The random number techniques of Butcher & Messel are used
    // (Nuc Phys 20(1960),15).
    // Note : Effects due to binding of atomic electrons are negliged.
    
    // Placeholder. To be replaced by a particle->GetEnergy() kind of function
    double gamma_initial_energy = 10;
    double lowest_secondary_energy = 100 * constants::electron_volt;
    
    // Placeholder. To be replaced by a GetLowEnergy() kind of function
    double low_energy_limit = 10;
    
    // If below energy limit, stop
    if (gamma_initial_energy <= low_energy_limit)
    {
        return;
    }
    
    double gamma_initial_energy_per_mc2 =
    gamma_initial_energy / constants::electron_mass_c2;
    
    //
    //G4ThreeVector gamma_initial_direction = aDynamicGamma->GetMomentumDirection();
    
    //
    // sample the energy rate of the scattered gamma
    //
    
    double epsilon, epsilon_squared;
    double one_minus_cosTheta, sinTheta_squared;
    double g_rejection_function;
    
    double epsilon_0 = 1. / (1. + 2. * gamma_initial_energy_per_mc2);
    double epsilon_0_squared = epsilon_0 * epsilon_0;
    double alpha_1 = - std::log(epsilon_0);
    double alpha_2 = alpha_1 + 0.5*(1.- epsilon_0_squared);
    
    double random_array[3];
    
    static const int loop_limit = 1000;
    
    // False interaction if there are too many interations
    for (int loop = 0; loop < loop_limit; loop++)
    {
        // Selecting 3 random numbers to sample scattering
        // Need to use Celeritas' Random, but that is not ready
        // Adding fixed numbers as placeholders
        // Random numbers must be of type random.Uniform(0, 1)
        
        random_array[3] = {0.2, 0.4, 0.8}; // FIX RANDOM NUMBER
        
        if (alpha_1 > alpha_2 * random_array[0])
        {
            epsilon = std::exp(-alpha_1 * random_array[1]); // epsilon_0^r
            epsilon_squared = epsilon * epsilon;
        }
        
        else
        {
            epsilon_squared =
            epsilon_0_squared + (1. - epsilon_0_squared) * random_array[1];
            epsilon = std::sqrt(epsilon_squared);
        }
        
        one_minus_cosTheta =
        (1. - epsilon) / (epsilon * gamma_initial_energy_per_mc2);
        
        sinTheta_squared = one_minus_cosTheta * (2. - one_minus_cosTheta);
        
        g_rejection_function =
        1. - epsilon * sinTheta_squared / (1. + epsilon_squared);
        
        if (g_rejection_function >= random_array[2])
        {
            break;
        }
    }
    
    // scattered gamma angles (z-axis along the parent gamma)
    if (sinTheta_squared < 0.0)
    {
        sinTheta_squared = 0.0;
    }
    
    // Placeholder for a random.Uniform() number for Phi.
    double a_random_uniform = 0.5;
    
    double cosTheta = 1. - one_minus_cosTheta;
    double sinTheta = std::sqrt(sinTheta_squared);
    double Phi = constants::two_pi * a_random_uniform; // FIX RANDOM NUMBER
    
    
    
    //------------ Update particle info for the scattered gamma -------------//
    /*
    // USE THREEVECTOR CLASS
    G4ThreeVector gamma_final_direction(sinTheta*cos(Phi),
                                        sinTheta*sin(Phi),
                                        cosTheta);

    gamma_final_direction.rotateUz(gamma_initial_direction);
    
    double gamma_final_energy = epsilon * gamma_initial_energy;
    double energy_deposited = 0.0;
    
    if (gamma_final_energy > lowest_secondary_energy)
    {
        fParticleChange->ProposeMomentumDirection(gamma_final_direction);
        fParticleChange->SetProposedKineticEnergy(gamma_final_energy);
    }
    
    else
    {
        fParticleChange->ProposeTrackStatus(fStopAndKill);
        fParticleChange->SetProposedKineticEnergy(0.0);
        energy_deposited = gamma_final_energy;
    }
    
    //
    // kinematic of the scattered electron
    //
    
    double electron_kinetic_energy = gamma_initial_energy - gamma_final_energy;
    
    if (electron_kinetic_energy > lowest_secondary_energy)
    {
        G4ThreeVector eDirection = gamma_initial_energy*gamma_initial_direction - gamma_final_energy*gamma_final_direction;
        eDirection = eDirection.unit();
        
        // create G4DynamicParticle object for the electron.
        G4DynamicParticle* dp = new G4DynamicParticle(theElectron,eDirection,electron_kinetic_energy);
        fvect->push_back(dp);
    }
    
    else
    {
        energy_deposited += electron_kinetic_energy;
    }
    
    // energy balance
    if (energy_deposited > 0.0)
    {
        fParticleChange->ProposeLocalEnergyDeposit(energy_deposited);
    }
     */
}


//---------------------------------------------------------------------------//
}  // namespace celeritas
