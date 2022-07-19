#ifndef CONSTRUCTION_HH
#define CONSTRUCTION_HH

/*Geant4 Packages*/
#include "G4VUserDetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4GenericMessenger.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4UserLimits.hh"
#include "G4SubtractionSolid.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"
#include "event.hh"
#include "G4Trap.hh"
#include "G4UnionSolid.hh"
#include "G4Args.hh"

#include "detector.hh"

class MyDetectorConstruction : public G4VUserDetectorConstruction
{
public:
    MyDetectorConstruction(MyG4Args*);
    ~MyDetectorConstruction();

    G4LogicalVolume *GetScoringVolume() const {return fScoringVolume;}
    G4LogicalVolume *GetDetectorVolume() const {return fDetectorVolume;}

    // Return Parameter Values
    G4double GetLYSOL() const {return LYSO_L;}
    G4double GetLYSOT() const {return LYSO_thick;}
    G4double GetGLUEL() const {return GLUE_L;}
    G4double GetRESINL() const {return RESIN_L;}
    G4double GetXPOS() const {return XposTol;}
    G4double GetYPOS() const {return YposTol;}
    G4double GetXPOS2() const {return XposTol2;}
    G4double GetYPOS2() const {return YposTol2;}
    G4double GetGC() const {return GeomConfig;}


    virtual G4VPhysicalVolume *Construct(); // call to detector construction function
private: // it is not accessed from outside
    
    // Default Values
    G4int nCols, nRows, GeomConfig, ESRtrue;
    G4double LYSO_L, LYSO_YIELD, LYSO_SCALERESOLUTION, Vovcon, LYSO_thick, perincr;
   G4double GLUE_L, RESIN_L, XposTol, YposTol,XposTol2, YposTol2, XYTol, LYSO_SC1, LYSO_RT1, RESIN_W, RESIN_LNOM, RESIN_LTol;
    
    // Messengers
    G4GenericMessenger *fMessenger,*fMessenger_thick, *fMessenger_SR, *fMessenger_YIELD, *fMessenger_vov, *fMessenger_GlueL, *fMessenger_ResinL, *fMessenger_XPos, *fMessenger_YPos, *fMessenger_GC, *fMessenger_pi, *fMessenger_ESR, *fMessenger_SR1, *fMessenger_RT1, *fMessenger_XPos2, *fMessenger_YPos2;

    // Geometry
    G4Box *solidWorld, *solidDetector, *solidGlue, *solidResin, *solidFR4;
    G4UnionSolid *solidLYSO;

    G4SubtractionSolid *Resin_Sub, *LYSOCover_Sub;

    G4UnionSolid *LYSOAll_Add;

    G4LogicalVolume *logicDetector; // We need to refer to this volume. Needs to be outside of the construction.
    G4LogicalVolume *logicWorld, *logicLYSO, *logicGlue, *logicResin, *logicResin_Sub, *logicFR4, *logicLYSOCover;
    G4LogicalVolume *fScoringVolume , *fDetectorVolume;

    G4VPhysicalVolume *physWorld, *physLYSO, *physDetector, *physGlue1, *physGlue2, *physResin1, *physResin2, *physFR41, *physFR42, *physLYSOCover;

    // Materials
    G4Material *worldMat, *SiO2, *H2O, *Aerogel, *prelude, *scintillator,*NaI, *EPOXY, *RTV3145;
    G4Element *C,*Na,*I,*H,*O,*Si,*N,*Al;

    // Surfaces
    G4OpticalSurface *mirrorSurface, *groundSurface, *SurfFR4;
    G4UserLimits* fStepLimit;  

    // classes
    MyG4Args* ArgsPass;
    
    // Functions
    virtual void ConstructSDandField(); // SD==Sensitive Detector
    void DefineMaterial();
    void DefaultValues();
    void DefineMessengers();
};

#endif
