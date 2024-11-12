#include "construction.hh"

myDetectorConstruction::myDetectorConstruction() {
    xWorld = 0.5*m;
    yWorld = 0.5*m;
    zWorld = 0.5*m;

    nCols = 8;
    nRows = 8;

    DefineMaterial();
}

myDetectorConstruction::~myDetectorConstruction() {

}

G4LogicalVolume *myDetectorConstruction::GetScoringVolume() const {
    return fScoringVolume;
}

void myDetectorConstruction::DefineMaterial() {
    G4NistManager *nist = G4NistManager::Instance();
    std::vector<G4double> energy = {1.239841939*eV/0.9, 1.239841939*eV/0.2};


    // Defining the world material
    worldMat = nist->FindOrBuildMaterial("G4_AIR");

    G4MaterialPropertiesTable *mptWorld = new G4MaterialPropertiesTable();
    std::vector<G4double> rindexAir = {1.0, 1.0};
    mptWorld->AddProperty("RINDEX", energy, rindexAir);
    worldMat->SetMaterialPropertiesTable(mptWorld);


    // Defining the scintillator material
    detMat = new G4Material("detMat", 1.06*g/cm3, 2);

    detMat->AddElement(nist->FindOrBuildElement("C"), 50.00*perCent);
    detMat->AddElement(nist->FindOrBuildElement("H"), 50.00*perCent);

    std::vector<G4double> scintEnergydetMat = {1.239841939*eV/0.9, 1.239841939*eV/0.425, 1.239841939*eV/0.2};
    std::vector<G4double> rindexdetMat = {1.60, 1.60, 1.60};
    std::vector<G4double> scintFracdetMat = {0.01, 1.0, 0.01};

    G4MaterialPropertiesTable *mptdetMat = new G4MaterialPropertiesTable();
    mptdetMat->AddProperty("RINDEX", scintEnergydetMat, rindexdetMat);
    mptdetMat->AddProperty("SCINTILLATIONCOMPONENT1", scintEnergydetMat, scintFracdetMat);
    mptdetMat->AddConstProperty("SCINTILLATIONYIELD", 10.44/keV);
    mptdetMat->AddConstProperty("RESOLUTIONSCALE", 1.0);
    mptdetMat->AddConstProperty("SCINTILLATIONTIMECONSTANT1", 3.3*ns);
    mptdetMat->AddConstProperty("SCINTILLATIONYIELD1", 1.);

    detMat->SetMaterialPropertiesTable(mptdetMat);


    // Defining the case material
    caseMat = new G4Material("caseMat", 0.855*g/cm3, 2);

    caseMat->AddElement(nist->FindOrBuildElement("C"), 33.33*perCent);
    caseMat->AddElement(nist->FindOrBuildElement("H"), 66.67*perCent);


    Si = nist->FindOrBuildMaterial("G4_Si");
}

void myDetectorConstruction::ConstructScintillator() {
    G4double radScint = 0.5*mm, lenScint = 10.*cm, posScint = 12.*cm, sideSIPM = 0.5*mm, lenSIPM = 0.1*mm, widthCase = 0.25*cm;

    // Constructing the scintillating fibers
    solidScintillator = new G4Tubs("solidScintillator", 0.*cm, radScint, lenScint, 0.*deg, 360.*deg);
    logicScintillator = new G4LogicalVolume(solidScintillator, detMat, "logicalScintillator");
    for(G4int i = 0; i < nRows; i++) {
        for(G4int j = 0; j < nCols; j++) {
            physScintillator = new G4PVPlacement(0, G4ThreeVector(-radScint*nRows+(i+0.5)*mm, -radScint*nCols+(j+0.5)*mm, posScint), logicScintillator, "physScintillator", logicWorld, false, j+i*nCols, true);
        }
    }
    

    // Constructing the SIPM matrices
    solidSIPM = new G4Box("solidSIPM", sideSIPM, sideSIPM, lenSIPM);
    logicSIPM = new G4LogicalVolume(solidSIPM, Si, "logicSIPM");
    for(G4int i = 0; i < nRows; i++) {
        for(G4int j = 0; j < nCols; j++) {
            physSIPM1 = new G4PVPlacement(0, G4ThreeVector(-sideSIPM*nRows+(i+0.5)*mm, -sideSIPM*nCols+(j+0.5)*mm, posScint+lenScint+lenSIPM), logicSIPM, "physSIPM1", logicWorld, false, j+i*nCols, true);
            physSIPM2 = new G4PVPlacement(0, G4ThreeVector(-sideSIPM*nRows+(i+0.5)*mm, -sideSIPM*nCols+(j+0.5)*mm, posScint-lenScint-lenSIPM), logicSIPM, "physSIPM2", logicWorld, false, j+i*nCols, true);
        }
    }


    // Constructing the casing of the detector
    auto              *solidCase1  = new G4Box("solidCase1", nRows*radScint, widthCase, lenScint+2*lenSIPM); 
    G4LogicalVolume   *logicCase1  = new G4LogicalVolume(solidCase1, caseMat, "logicalCase1");
    G4VPhysicalVolume *physCase11  = new G4PVPlacement(0, G4ThreeVector(0., nCols*radScint+widthCase, posScint), logicCase1, "physCase11", logicWorld, false, 0, true);
    G4VPhysicalVolume *physCase12  = new G4PVPlacement(0, G4ThreeVector(0., -nCols*radScint-widthCase, posScint), logicCase1, "physCase12", logicWorld, false, 0, true);
    auto              *solidCase2  = new G4Box("solidCase2", widthCase, nCols*radScint+2*widthCase, lenScint+2*lenSIPM); 
    G4LogicalVolume   *logicCase2  = new G4LogicalVolume(solidCase2, caseMat, "logicalCase2");
    G4VPhysicalVolume *physCase21  = new G4PVPlacement(0, G4ThreeVector(nRows*radScint+widthCase, 0., posScint), logicCase2, "physCase21", logicWorld, false, 0, true);
    G4VPhysicalVolume *physCase22  = new G4PVPlacement(0, G4ThreeVector(-nRows*radScint-widthCase, 0., posScint), logicCase2, "physCase22", logicWorld, false, 0, true);
    auto              *solidCase3  = new G4Box("solidCase3", nRows*radScint+2*widthCase, nCols*radScint+2*widthCase, widthCase); 
    G4LogicalVolume   *logicCase3  = new G4LogicalVolume(solidCase3, caseMat, "logicalCase3");
    G4VPhysicalVolume *physCase31  = new G4PVPlacement(0, G4ThreeVector(0., 0., posScint-lenScint-2*lenSIPM-widthCase), logicCase3, "physCase31", logicWorld, false, 0, true);
    G4VPhysicalVolume *physCase32  = new G4PVPlacement(0, G4ThreeVector(0., 0., posScint+lenScint+2*lenSIPM+widthCase), logicCase3, "physCase32", logicWorld, false, 0, true);
    
    fScoringVolume = logicScintillator;
}

G4VPhysicalVolume *myDetectorConstruction::Construct() {
    // Defining the world
    solidWorld = new G4Box("solidWorld", xWorld, yWorld, zWorld);
    logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");
    physWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "physWorld", 0, false, 0, true);

    ConstructScintillator();

    return physWorld;
}

void myDetectorConstruction::ConstructSDandField() {
    mySensitiveDetector *sensDet = new mySensitiveDetector("SensitiveDetector");

    logicScintillator->SetSensitiveDetector(sensDet);
}