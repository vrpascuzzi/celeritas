//----------------------------------*-C++-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file GeantImporter.cc
//---------------------------------------------------------------------------//
#include "GeantImporter.hh"

#include <iomanip>

// ROOT
#include "TTree.h"
#include "TBranch.h"
#include "TLeaf.h"

namespace celeritas
{
//---------------------------------------------------------------------------//
// PUBLIC
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
/*!
 * Construct with defaults
 */
GeantImporter::GeantImporter()
    : rootFile_particleDef(nullptr), rootFile_physicsTable(nullptr)
{
}

//---------------------------------------------------------------------------//
/*!
 * Destructor
 */
GeantImporter::~GeantImporter()
{
    this->rootFile_particleDef->Close();
    this->rootFile_physicsTable->Close();

    delete rootFile_particleDef;
    delete rootFile_physicsTable;
}

//---------------------------------------------------------------------------//
/*!
 * Loads the data from the particleData.root file into memory as a vector
 */
void GeantImporter::loadParticleDefRootFile(const std::string filename)
{
    this->rootFile_particleDef = new TFile(filename.c_str(), "open");

    // Silly safeguard
    if (!rootFile_particleDef)
        return;

    buildObjectsList(this->rootFile_particleDef);
    loadParticleDefsIntoMemory();
}

//---------------------------------------------------------------------------//
/*!
 * Loads the data from the physicsTables.root file into memory as a map
 */
void GeantImporter::loadPhysicsTableRootFile(const std::string filename)
{
    this->rootFile_physicsTable = new TFile(filename.c_str(), "open");

    // Silly safeguard
    if (!rootFile_physicsTable)
        return;

    buildObjectsList(this->rootFile_physicsTable);
    loadPhysicsTablesIntoMemory();
}

//---------------------------------------------------------------------------//
/*!
 * Copies a particle from the vector into a G4ParticleDef
 */
bool GeantImporter::copyParticleDef(int pdg, G4ParticleDef& g4Particle)
{
    for (auto aParticle : this->particleVector)
    {
        if (aParticle.pdg() == pdg)
        {
            g4Particle = aParticle;
            return true;
        }
    }

    return false;
}

//---------------------------------------------------------------------------//
/*!
 * Copies a physics table from the map into a G4PhysicsTable
 */
bool GeantImporter::copyPhysicsTable(std::string     physTableName,
                                     G4PhysicsTable& physTable)
{
    for (auto thisPair : physTableMap)
    {
        if (thisPair.first == physTableName)
        {
            physTable = thisPair.second;
            return true;
        }
    }

    return false;
}

//---------------------------------------------------------------------------//
/*!
 * Prints the loaded objects list, useful to check the physics tables names
 */
void GeantImporter::printObjectsList()
{
    for (auto name : this->objectsList)
    {
        std::cout << name << std::endl;
    }
}

//---------------------------------------------------------------------------//
/*!
 * Finds the particle using its PDG and prints all its G4ParticleDef data
 */
void GeantImporter::printParticleInfo(int pdg)
{
    G4ParticleDef particle;

    if (!copyParticleDef(pdg, particle))
    {
        std::cout << "Particle not found" << std::endl;
        return;
    }

    std::cout << "-----------------------" << std::endl;
    std::cout << particle.name() << std::endl;
    std::cout << "-----------------------" << std::endl;
    std::cout << "pdg      : " << particle.pdg() << std::endl;
    std::cout << "mass     : " << particle.mass() << std::endl;
    std::cout << "charge   : " << particle.charge() << std::endl;
    std::cout << "spin     : " << particle.spin() << std::endl;
    std::cout << "lifetime : " << particle.lifetime() << std::endl;
    std::cout << "isStable : " << particle.isStable() << std::endl;
    std::cout << "-----------------------" << std::endl;
}

//---------------------------------------------------------------------------//
/*!
 * Prints all the data from a given G4PhysicsTable by providing its name
 */
void GeantImporter::printPhysicsTable(std::string physTableName)
{
    std::cout << std::setprecision(3);
    std::cout << std::fixed;
    std::cout << std::scientific;

    G4PhysicsTable aTable;

    if (!copyPhysicsTable(physTableName, aTable))
    {
        std::cout << "Physics table not found" << std::endl;
        return;
    }

    std::cout << physTableName << std::endl;
    std::cout << " | tableSize: " << aTable.tableSize_ << std::endl;

    for (int i = 0; i < aTable.tableSize_; i++)
    {
        std::cout << " |" << std::endl;
        std::cout << " | --------------------------" << std::endl;
        std::cout << " | ENTRY " << i << std::endl;
        std::cout << " | --------------------------" << std::endl;
        std::cout << " | edgeMin       : ";
        std::cout << aTable.edgeMin_.at(i) << std::endl;
        std::cout << " | edgeMax       : ";
        std::cout << aTable.edgeMax_.at(i) << std::endl;
        std::cout << " | numberOfNodes : ";
        std::cout << aTable.numberOfNodes_.at(i) << std::endl;
        std::cout << " | vectorType    : ";
        std::cout << aTable.vectorType_.at(i) << std::endl;
        std::cout << " | binVector      dataVector" << std::endl;

        for (int j = 0; j < aTable.binVector_.at(i).size(); j++)
        {
            std::cout << " | | ";
            std::cout << std::left << std::setw(12);
            std::cout << std::setfill(' ');
            std::cout << aTable.binVector_.at(i).at(j);
            std::cout << " | ";
            std::cout << aTable.dataVector_.at(i).at(j);
            std::cout << std::endl;
        }
    }
}

//---------------------------------------------------------------------------//
/*!
 * Prints all the physics table names found in the ROOT file
 */
void GeantImporter::printPhysTableNames()
{
    for (auto aTable : this->physTableMap)
    {
        std::cout << aTable.first << std::endl;
    }
}

//---------------------------------------------------------------------------//
// PRIVATE
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
/*!
 * Creates a list of all the object names found in the ROOT file
 */
void GeantImporter::buildObjectsList(TFile* rootFile)
{
    // Getting list of keys -- i.e. list of elements in the root input
    TList* list = rootFile->GetListOfKeys();

    // Getting list iterator
    TIter iter(list->MakeIterator());

    // Cleaning vector
    this->objectsList.clear();

    // Looping over the objects found in rootInput
    while (TObject* object = iter())
    {
        // Getting the object's name and type
        TKey*       key     = (TKey*)object;
        std::string keyName = key->GetName();
        std::string keyType = key->GetClassName();

        // Safeguard to avoid reading a non-tree object
        // Just in case we add something else to the file in the future
        if (keyType != "TTree")
            continue;

        this->objectsList.push_back(keyName);
    }
}

//---------------------------------------------------------------------------//
/*!
 * Loops over the objects list created by buildObjectsList() to create a
 * vector<G4ParticleDef>.
 */
void GeantImporter::loadParticleDefsIntoMemory()
{
    this->physTableMap.clear();
    this->particleVector.clear();

    G4ParticleDef thisParticle;
    std::string*  branchName = new std::string;

    for (auto particleName : objectsList)
    {
        TTree* treeParticle
            = (TTree*)this->rootFile_particleDef->Get(particleName.c_str());
        treeParticle->SetBranchAddress("name", &branchName);

        treeParticle->GetEntry(0);

        std::string thisName   = *branchName;
        int         thisPdg    = treeParticle->GetLeaf("pdg")->GetValue();
        double      thisMass   = treeParticle->GetLeaf("mass")->GetValue();
        double      thisCharge = treeParticle->GetLeaf("charge")->GetValue();
        double      thisSpin   = treeParticle->GetLeaf("spin")->GetValue();
        double thisLifetime    = treeParticle->GetLeaf("lifetime")->GetValue();
        bool   thisIsStable    = treeParticle->GetLeaf("isStable")->GetValue();

        thisParticle(thisName,
                     thisPdg,
                     thisMass,
                     thisCharge,
                     thisSpin,
                     thisLifetime,
                     thisIsStable);

        this->particleVector.push_back(thisParticle);
    }
}

//---------------------------------------------------------------------------//
/*!
 * Loops over the objects list created by buildObjectsList() to create a
 * vector<G4ParticleDef>.
 */
void GeantImporter::loadPhysicsTablesIntoMemory()
{
    G4PhysicsTable pTable;

    for (auto tableName : objectsList)
    {
        // Creating a tree pointer and getting the tree
        TTree* tree
            = (TTree*)this->rootFile_physicsTable->Get(tableName.c_str());

        // For accessing the tree members
        std::vector<double>* readBinVector  = new std::vector<double>;
        std::vector<double>* readDataVector = new std::vector<double>;

        tree->SetBranchAddress("binVector", &readBinVector);
        tree->SetBranchAddress("dataVector", &readDataVector);

        // For writing to g4PhysTable
        std::vector<double> writeBinVector;
        std::vector<double> writeDataVector;

        // Looping over the tree entries
        pTable.tableSize_ = tree->GetEntries();

        for (int i = 0; i < pTable.tableSize_; i++)
        {
            // Clearing writing vectors
            writeBinVector.clear();
            writeDataVector.clear();

            // Fetching tree entry
            tree->GetEntry(i);

            // Fetching the values of each leaf
            pTable.edgeMin_.push_back(tree->GetLeaf("edgeMin")->GetValue());
            pTable.edgeMax_.push_back(tree->GetLeaf("edgeMax")->GetValue());
            pTable.numberOfNodes_.push_back(
                tree->GetLeaf("numberOfNodes")->GetValue());
            pTable.vectorType_.push_back(
                tree->GetLeaf("vectorType")->GetValue());

            // Looping over binVector and dataVector
            for (int j = 0; j < readBinVector->size(); j++)
            {
                writeBinVector.push_back(readBinVector->at(j));
                writeDataVector.push_back(readDataVector->at(j));
            }

            pTable.binVector_.push_back(writeBinVector);
            pTable.dataVector_.push_back(writeDataVector);
        }

        this->physTableMap.emplace(std::make_pair(tableName, pTable));
    }
}

//---------------------------------------------------------------------------//
} // namespace celeritas