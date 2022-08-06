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


#include "detector.hh"

class MyDetectorConstruction : public G4VUserDetectorConstruction
{
public:
    MyDetectorConstruction();
    ~MyDetectorConstruction();

    G4LogicalVolume *GetScoringVolume() const {return fScoringVolume;}
    G4LogicalVolume *GetDetectorVolume() const {return fDetectorVolume;}
    G4double GetLYSOL() const {return LYSO_L;}
    G4double GetLYSOT() const {return LYSO_thick;}
    G4double GetGLUEL() const {return GLUE_L;}
    G4double GetRESINL() const {return RESIN_L;}
    G4double GetXPOS() const {return XposTol;}
    G4double GetYPOS() const {return YposTol;}
    G4double GetGC() const {return GeomConfig;}
    void Reset_LYSO();


    //static void RandGLUEL()  { GLUE_L=0.15*mm+0.1*mm*G4UniformRand();}
    /*G4AnyMethod RandGLUEL()  { GLUE_L=0.15*mm+0.1*mm*G4UniformRand();return GLUE_L}
    void RandRESINL()  { RESIN_L=0.3*mm+0.4*mm*G4UniformRand();}
    virtual void RandXPOS()  { XposTol=-0.05*mm+G4UniformRand()*0.1*mm;}
    virtual void RandYPOS()  { YposTol=-0.05*mm+G4UniformRand()*0.1*mm;}*/
    virtual G4VPhysicalVolume *Construct(); // call to detector construction function
private: // it is not accessed from outside
    
    G4LogicalVolume *logicDetector; // We need to refer to this volume. Needs to be outside of the construction.

    G4int nCols, nRows, GeomConfig, ESRtrue;
    G4double LYSO_L, LYSO_YIELD, LYSO_SCALERESOLUTION, Vovcon, LYSO_thick, perincr, decay_time;
    virtual void ConstructSDandField(); // SD==Sensitive Detector
    G4GenericMessenger *fMessenger,*fMessenger_thick, *fMessenger_SR, *fMessenger_YIELD, *fMessenger_vov, *fMessenger_GlueL, *fMessenger_ResinL, *fMessenger_XPos, *fMessenger_YPos, *fMessenger_GC, *fMessenger_pi, *fMessenger_ESR, *fMessenger_reset_LYSO, *fMessenger_decay_time;

    G4Box *solidWorld, *solidDetector, *solidGlue, *solidResin, *solidFR4;
    G4UnionSolid *solidLYSO;

    G4LogicalVolume *logicWorld, *logicLYSO, *logicGlue, *logicResin, *logicResin_Sub, *logicFR4;

    G4VPhysicalVolume *physWorld, *physLYSO, *physDetector, *physGlue1, *physGlue2, *physResin1, *physResin2, *physFR41, *physFR42;

    G4Material *worldMat, *SiO2, *H2O, *Aerogel, *prelude, *scintillator,*NaI, *EPOXY, *RTV3145;
    G4Element *C,*Na,*I,*H,*O,*Si,*N,*Al;

    G4OpticalSurface *mirrorSurface, *groundSurface, *SurfFR4, *Tyvek_Surface, *lamb_air_surface;
    G4UserLimits* fStepLimit;  
    
    G4LogicalVolume *fScoringVolume , *fDetectorVolume;
    
    void DefineMaterial();

    G4double GLUE_L;
    G4double RESIN_L;
    G4double XposTol;
    G4double YposTol;
    
};

#endif
