#ifndef CONSTRUCTION_HH
#define CONSTRUCTION_HH

#include "G4SystemOfUnits.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "G4Material.hh"
#include "G4GenericMessenger.hh"

#include "detector.hh"

class myDetectorConstruction : public G4VUserDetectorConstruction {
    public: 
        myDetectorConstruction();
        ~myDetectorConstruction();

        G4LogicalVolume *GetScoringVolume() const;

        virtual G4VPhysicalVolume *Construct();   

    private:
        G4LogicalVolume *fScoringVolume;

        G4GenericMessenger *fMessenger;
        
        G4double xWorld, yWorld, zWorld;
        G4int nCols, nRows;
        
        G4Tubs *solidScintillator;
        G4Box *solidWorld, *solidSIPM;
        G4LogicalVolume *logicWorld, *logicScintillator, *logicSIPM;
        G4VPhysicalVolume *physWorld, *physScintillator, *physSIPM1, *physSIPM2;

        G4Material *worldMat, *detMat, *caseMat, *Si;
        G4Element *C, *H;

        void DefineMaterial();
        void ConstructScintillator();

        G4bool isScintillator;

        virtual void ConstructSDandField();
};

#endif