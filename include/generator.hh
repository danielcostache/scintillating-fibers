#ifndef GENERATOR_HH
#define GENERATOR_HH

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "G4GeneralParticleSource.hh"
#include "G4GeneralParticleSourceData.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleTable.hh"
#include "G4ChargedGeantino.hh"
#include "G4Geantino.hh"
#include "G4IonTable.hh"

class myPrimaryGenerator : public G4VUserPrimaryGeneratorAction {
    public:
        myPrimaryGenerator();
        ~myPrimaryGenerator();

        virtual void GeneratePrimaries(G4Event*);
    
    private:
        G4ParticleGun *fParticleGun;
        G4GeneralParticleSource *gParticleSource;
};

#endif