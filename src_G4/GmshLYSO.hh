#ifndef GMSHLYSO_HH
#define GMSHLYSO_HH

#include <set>
#include <gmsh.h>
#include <iostream>

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
#include "G4Trap.hh"
#include "G4UnionSolid.hh"
#include "G4TriangularFacet.hh"
#include "G4TessellatedSolid.hh"
#include "G4QuadrangularFacet.hh"

#include "G4Tet.hh"
#include <G4UIcommand.hh>
#include "G4Args.hh"
#include "util.hh"

class GmshLYSO 
{
public:
    GmshLYSO(MyG4Args *MainArgs);
    ~GmshLYSO();
    
    void MakeBar(MyG4Args *MainArgs);
	void MakeTile();
    void CreateG4LYSO(G4Material*, G4LogicalVolume*);
    void CreateG4LYSO_GC3(G4Material*, G4LogicalVolume*, G4double);
	void SurfaceCoating(G4VPhysicalVolume* , G4OpticalSurface* );
	void SurfaceCoating_GC3(G4VPhysicalVolume* , G4OpticalSurface* );

	std::vector<G4LogicalVolume*> GetScoringVolumeVec() const {return fScoringVolumeVec;}

    double* GetNodeTags() const {return mesh_nodetags;}
    double* GetNodeCoords() const {return mesh_nodecoords;}
    int GetNumberOfNodes() const {return mesh_totalnodes;}
    double GetVolume() const {return Volume;}

	bool IsVolumeInList(const G4LogicalVolume* volume);

    
private:

int mesh_totalnodes;
double *mesh_nodetags, *mesh_nodecoords, Volume=0;
G4Tet *LYSOTet;

std::vector<std::size_t> tags;
std::vector<double> coord, param;
std::vector<std::vector<std::size_t> > elemTags, elemNodeTags;

	G4double Ztot,Xtot,*ptsY,*ptsYF,*ptsYFb;
	G4int Znode;
	G4String modelname;

int Nelem=0;

G4int YSymm=0;
G4int def=0;
G4double VolTet=0;
G4ThreeVector *p1,*p2,*p3,*p4;
G4Tet *LYSOTet_Solid;
G4LogicalVolume *LYSOTet_Logic; 
G4PVPlacement *LYSOTet_Phys;
G4LogicalBorderSurface *LYSO_Air_Border;

std::vector<G4PVPlacement*> lstPhysTet;
std::vector<G4LogicalVolume*> fScoringVolumeVec;

MyG4Args *LocalArgs;

			
};    

#endif
