#include "construction.hh"
#include "util.hh"
#include "materials.hh"

MyDetectorConstruction::MyDetectorConstruction(MyG4Args* MainArgs)
{// constructor
    ArgsPass=MainArgs;
    DefaultValues();
    DefineMessengers();
    DefineMaterial(); // Run material function in the constructor

}

MyDetectorConstruction::~MyDetectorConstruction()
{}

/* Function to define a single time the materials when parametrizing, materials
 * need to be defined in the class header! Definition of the function for the
 * class */
void MyDetectorConstruction::DefineMaterial()
{
    /////////////
    //Materials//
    /////////////

    // import nist material data
    G4NistManager *nist = G4NistManager::Instance();

    // Find material in G4 database: Air
    worldMat = nist->FindOrBuildMaterial("G4_AIR");
    /* Set the refractive index of air. The refractive index is defined by
     * giving a list of photon energies and their corresponding refractive
     * index. Here, we want to set a constant refractive index, so we just
     * define the array as starting from 200 nm (6.199 eV) to 900 nm (1.378 eV)
     * and the refractive index for both to 1. */
    G4double energyWorld[2] = {1.378*eV, 6.199*eV};
    G4double rindexWorld[2] = {1.0, 1.0};
        //Refer material properties to material 
    G4MaterialPropertiesTable *mptWorld = new G4MaterialPropertiesTable();
    /* Set the material properties table. The syntax is:
     *
     *     AddProperty("Mat Prop name", energy, values, num points)
     *
     */
    mptWorld->AddProperty("RINDEX", energyWorld, rindexWorld, 2);
    worldMat->SetMaterialPropertiesTable(mptWorld);

    /* Define SiO2 material. This is one of the material used to make printed
     * circuit boards (PCBs).
     *
     * G4Material is called like:
     *
     *     G4Material(name, density, number of components)
     *
     * In this case, the density is 2.201 g/cm^3 and it has two elements. */
    SiO2 = new G4Material("SiO2", 2.201*g/cm3, 2);
    SiO2->AddElement(nist->FindOrBuildElement("Si"),1);
    SiO2->AddElement(nist->FindOrBuildElement("O"),2);
    G4MaterialPropertiesTable *mptSiO2 = new G4MaterialPropertiesTable();
    G4double energySiO2[2] = {1.378*eV, 6.199*eV};
    G4double rindexSiO2[2] = {1.4585, 1.4585};
    /* Set the absorption length. Since optical light doesn't pass through
     * PCBs, we just set it to 10 microns. */
    G4double ABSSiO2[2] = {0.01*mm, 0.01*mm};
    mptSiO2->AddProperty("RINDEX", energySiO2, rindexSiO2, 2);
    mptSiO2->AddProperty("ABSLENGTH", energySiO2, ABSSiO2, 2);
    SiO2->SetMaterialPropertiesTable(mptSiO2);

    RTV3145 = get_rtv();
    scintillator = get_lyso(LYSO_YIELD,LYSO_RT1,LYSO_SCALERESOLUTION);

    H = nist->FindOrBuildElement("H");
    Si = nist->FindOrBuildElement("Si");
    O = nist->FindOrBuildElement("O");
    C = nist->FindOrBuildElement("C");

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Epoxy  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    N = nist -> FindOrBuildElement("N");
    EPOXY = new G4Material("EPOXY", 1.16*g/cm3,4);
    EPOXY->AddElement(H, 32);
    EPOXY->AddElement(N,2);
    EPOXY->AddElement(C,15);
    EPOXY->AddElement(O,4);

    const G4int numEP=30;
    G4double EPOXY_ene[numEP]   =  { 2.*eV ,  2.05*eV ,2.1*eV , 2.15*eV ,2.2*eV , 2.25*eV, 2.3*eV,  2.35*eV ,2.4*eV , 2.45*eV,2.5*eV , 2.55*eV ,2.6*eV , 2.65*eV, 2.7*eV , 2.75*eV, 2.8*eV , 2.85*eV ,2.9*eV ,2.95*eV ,3.*eV ,  3.05*eV, 3.1*eV , 3.15*eV ,3.2*eV , 3.25*eV, 3.3*eV , 3.35*eV ,3.4*eV , 3.45*eV};
    G4double EPOXY_RINDEX[numEP]   =  {1.54681164, 1.54728704, 1.54753043, 1.5486172 , 1.54841963, 1.54881503, 1.54919653, 1.54927211, 1.5508896 , 1.55264765,
       1.55430624, 1.55734675, 1.55627368, 1.5588548 , 1.56081691, 1.56419381, 1.56298843, 1.56578221, 1.56737946, 1.57147998,
       1.57392046, 1.57520827, 1.57817494, 1.58191818, 1.58438809, 1.58613128, 1.58577288, 1.58801395, 1.59180049, 1.59584015};
    G4double EPOXY_ABSLEN[numEP]   =  {38.01438753*cm, 35.75906054*cm, 33.61113008*cm, 31.56310336*cm, 29.60816877*cm, 27.74012015*cm, 25.95329104*cm, 24.24249722*cm, 22.60298646*cm, 21.03039452*cm,
       19.52070625*cm, 18.07022145*cm, 16.67552452*cm, 15.33345766*cm, 14.04109698*cm, 12.79573124*cm, 11.59484285*cm, 10.43609089*cm,  9.31729589*cm,  8.23642615*cm,
        7.1915854*cm ,  6.18100172*cm,  5.03496374*cm,  4.41427404*cm,  3.79572092*cm, 3.14544767*cm,  2.48008015*cm,  2.1255284*cm ,  1.83454881*cm,  1.5814501*cm};
    G4MaterialPropertiesTable *mptRESIN = new G4MaterialPropertiesTable();
    mptRESIN->AddProperty("RINDEX",    EPOXY_ene,    EPOXY_RINDEX,     numEP); // fraction of the light reflected (all=1)
    mptRESIN->AddProperty("ABSLENGTH", EPOXY_ene,    EPOXY_ABSLEN,     numEP); // fraction of the light reflected (all=1)
    EPOXY-> SetMaterialPropertiesTable(mptRESIN);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //FR4 (Glass + Epoxy)  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

 G4Material* FR4 = new G4Material("FR4" , 1.86*g/cm3, 2);
 FR4->AddMaterial(EPOXY, 47.2*perCent);
 FR4->AddMaterial(SiO2, 52.8*perCent);
    //G4MaterialPropertiesTable *mptFR4 = new G4MaterialPropertiesTable();
    //mptFR4->AddProperty("RINDEX",    energySiO2,    rindexSiO2,     numEP); 
    FR4-> SetMaterialPropertiesTable(mptSiO2);
//Alumina from
//http://geant4-hn.slac.stanford.edu:5090/HyperNews/public/get/emfields/29/1/1.html
//density = 3.75*g/cm3; //
Al = nist -> FindOrBuildElement("Al");
G4Material* Alumina = new G4Material("Alumina"  , 3.75*g/cm3, 2);
Alumina->AddElement(Al, 2);
Alumina->AddElement(O, 3);

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // MIRROR PROPERTIES  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


G4double energymirror0[34] ={1.387638658*eV,1.414514446*eV,1.442584622*eV,1.475671466*eV,1.511484607*eV,1.540077409*eV,1.57666106*eV,1.611195145*eV,1.650914734*eV,1.692642175*eV,1.736533665*eV,1.78129609*eV,1.831519006*eV,1.883017892*eV,1.952071279*eV,1.994518826*eV,2.056695744*eV,2.119145672*eV,2.191060487*eV,2.265172322*eV,2.344473291*eV,2.42373704*eV,2.522802173*eV,2.620592548*eV,2.726243339*eV,2.841937404*eV,2.962139009*eV,3.10551606*eV,3.198826301*eV,3.213681724*eV,3.235720977*eV,3.262160277*eV,3.303164908*eV,3.422984584*eV};



 G4double reflectivity0[34]={0.240441406,0.238504637,0.291823064,0.409637463,0.580529881,0.795137568,
0.959392257,0.98221903,0.992861287,0.981807802,
0.973183199,0.978334456,0.986066277,0.98714133,0.982715392,0.984215926,0.986816093,0.987184464,0.981927024
,0.983870632,0.983629712,0.984087367,0.974020118,0.971960262,0.972658815,0.977535487,0.975813698,0.9472273,
0.803353217, 0.630142605,0.488159981,0.333316034,0.174725627,0.127884496};

    const G4int numRTV=30;
    G4double RTV_ene[numRTV]   =  { 2.*eV ,  2.05*eV ,2.1*eV , 2.15*eV ,2.2*eV , 2.25*eV, 2.3*eV,  2.35*eV ,2.4*eV , 2.45*eV,2.5*eV , 2.55*eV ,2.6*eV , 2.65*eV, 2.7*eV , 2.75*eV, 2.8*eV , 2.85*eV ,2.9*eV ,2.95*eV ,3.*eV ,  3.05*eV, 3.1*eV , 3.15*eV ,3.2*eV , 3.25*eV, 3.3*eV , 3.35*eV ,3.4*eV , 3.45*eV};
    G4double RTV_RINDEX[numRTV]   =  {1.39539274, 1.39993242, 1.40181797, 1.40642231, 1.40942362,
       1.41301353, 1.41660416, 1.42133831, 1.42621651, 1.43028629,
       1.4380602 , 1.44295981, 1.44800143, 1.45426941, 1.46297174,
       1.47117145, 1.47716873, 1.48199179, 1.48986671, 1.49899325,
       1.50739242, 1.51507152, 1.52325557, 1.53162857, 1.53973068,
       1.54747239, 1.55620018, 1.56428621, 1.5715874 , 1.5823968 };
    G4double RTV_ABSLEN[numRTV]   =  {2.89352576*cm, 2.81870651*cm, 2.6356988*cm , 2.54633899*cm, 2.38804899*cm,
       2.33777894*cm, 2.20850964*cm, 2.13442529*cm, 1.99960048*cm, 1.91645445*cm,
       1.85130259*cm, 1.7937614*cm , 1.70213491*cm, 1.62087764*cm, 1.56593542*cm,
       1.52046865*cm, 1.47959493*cm, 1.42291287*cm, 1.35332923*cm, 1.29213138*cm,
       1.23859418*cm, 1.19172294*cm, 1.14473815*cm, 1.09502845*cm, 1.0599705*cm ,
       1.02676536*cm, 0.98029928*cm, 0.93696289*cm, 0.8959285*cm , 0.87300369*cm};

    G4MaterialPropertiesTable *mptMirror = new G4MaterialPropertiesTable();
    mptMirror->AddProperty("REFLECTIVITY", energymirror0, reflectivity0,34); // fraction of the light reflected (all=1)
    //mptMirror->AddProperty("SPECULARLOBECONSTANT", pp, specularlobe,2); 
    //mptMirror->AddProperty("SPECULARSPIKECONSTANT", pp, specularspike,2); 
    //mptMirror->AddProperty("BACKSCATTERCONSTANT", pp, backscatter,2); 
    mptMirror->AddProperty("RINDEX", RTV_ene, RTV_RINDEX,numRTV); // fraction of the light reflected (all=1)

    // Surface
    mirrorSurface = new G4OpticalSurface("mirrorSurface");
    mirrorSurface -> SetMaterialPropertiesTable(mptMirror);
    mirrorSurface -> SetType(dielectric_metal);
    mirrorSurface -> SetFinish(polished);//-backpainted
    mirrorSurface -> SetModel(unified);

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // FR4 Interface  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Surface
    G4double sigma_alpha = 0.1;
    SurfFR4 = new G4OpticalSurface("SurfFR4");
    SurfFR4 -> SetType(dielectric_dielectric);
    SurfFR4 -> SetFinish(ground);
    SurfFR4 -> SetModel(unified);
    SurfFR4 -> SetSigmaAlpha(sigma_alpha);

    //G4double energyFR4S[2] = {1.239841939*eV/0.9,1.239841939*eV/0.2};
    //G4double reflFR4S[2] ={0.5, 0.5};  
    G4MaterialPropertiesTable *mptFR4S = new G4MaterialPropertiesTable();
    //mptFR4S->AddProperty("TRANSMITTANCE", energyFR4S, reflFR4S,2); // fraction of the light reflected (all=1)
    SurfFR4 -> SetMaterialPropertiesTable(mptFR4S);

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // FR4 Interface  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Surface
    G4double sigma_alpha2 = 0.1;
    groundSurface = new G4OpticalSurface("LYSO_Glue_Surface");
    //mirrorSurface -> SetMaterialPropertiesTable(mptMirror);
    groundSurface -> SetType(dielectric_dielectric);
    groundSurface -> SetFinish(polished);
    groundSurface -> SetModel(unified);
    groundSurface -> SetSigmaAlpha(sigma_alpha2);
    //G4double energyground[2] = {1.239841939*eV/0.9,1.239841939*eV/0.2};
    //G4double reflground[2] ={0.88, 0.88};  
    G4MaterialPropertiesTable *mptground = new G4MaterialPropertiesTable();
    //mptground->AddProperty("TRANSMITTANCE", energyground, reflground,2); // fraction of the light reflected (all=1)
    groundSurface -> SetMaterialPropertiesTable(mptground);
}

G4VPhysicalVolume *MyDetectorConstruction::Construct()
{
////////////////////
// SOLID VOLUMES  // G4Box("var", width*m, lengtg*m, thickness*m);
////////////////////

    G4double xWorld = 0.1*m;    xWorld=xWorld/2.;
    G4double yWorld = 0.1*m;    yWorld=yWorld/2.;
    G4double zWorld = 0.1*m;    zWorld=zWorld/2.;
    G4double DET_L = 0.3*mm;    DET_L=DET_L/2.;    
    G4double RESIN_H =6.5*mm;   RESIN_H=RESIN_H/2.;
    //G4double RESIN_W =3.1*mm;RESIN_W=RESIN_W/2.;
    G4double FR4_L =0.8/2.*mm;     //FR4_L=FR4_L/2.;
    G4double DET_T =3.;DET_T=DET_T/2.;

    solidWorld = new G4Box("solidWorld", xWorld, yWorld, zWorld);
    if(GeomConfig==1 || GeomConfig==3){
        G4Trap* trapLYSO = new G4Trap("trapLYSO",       //G4String& pName,
                                LYSO_L/2.*mm,0.,   //G4double pDz, G4double pTheta,
                                0., LYSO_thick*mm,      //G4double pPhi, G4double pDy1,
                            LYSO_thick*mm, LYSO_thick*mm,   //G4double pDx1, G4double pDx2,
                            0.,LYSO_thick*mm*(1+perincr),               //G4double pAlp1, G4double pDy2,
                            LYSO_thick*mm,LYSO_thick*mm,    //G4double pDx3, G4double pDx4,
                            0.);            //G4double pAlp2)
        G4ThreeVector positionTrap(0., 0., LYSO_L*mm);// translate 1/4L in -Z, already set in Z trap
        G4RotationMatrix rotTrap  = G4RotationMatrix();// rotar 180 en Y
        rotTrap.rotateY(M_PI*rad);
        G4Transform3D transformTrap(rotTrap, positionTrap);
        solidLYSO = new G4UnionSolid("solidLYSO", trapLYSO, trapLYSO, transformTrap);
    solidGlue = new G4Box("solidGlue", RESIN_W*mm+0.2*mm*G4UniformRand(), LYSO_thick*mm+0.194*mm+0.2*mm*G4UniformRand(), GLUE_L*mm);
    }
    else if (GeomConfig==2)
    {
       G4Trap* trapLYSO1 = new G4Trap("trapLYSO",       //G4String& pName,
                                LYSO_L/2.*mm,0.,   //G4double pDz, G4double pTheta,
                                0., LYSO_thick*mm*(1+perincr),      //G4double pPhi, G4double pDy1,
                            LYSO_thick*mm, LYSO_thick*mm,   //G4double pDx1, G4double pDx2,
                            0.,LYSO_thick*mm,               //G4double pAlp1, G4double pDy2,
                            LYSO_thick*mm,LYSO_thick*mm,    //G4double pDx3, G4double pDx4,
                            0.);            //G4double pAlp2)
       G4Trap* trapLYSO2 = new G4Trap("trapLYSO",       //G4String& pName,
                                LYSO_L/2.*mm,0.,   //G4double pDz, G4double pTheta,
                                0., LYSO_thick*mm,      //G4double pPhi, G4double pDy1,
                            LYSO_thick*mm, LYSO_thick*mm,   //G4double pDx1, G4double pDx2,
                            0.,LYSO_thick*mm,               //G4double pAlp1, G4double pDy2,
                            LYSO_thick*mm,LYSO_thick*mm,    //G4double pDx3, G4double pDx4,
                            0.);            //G4double pAlp2)
        G4ThreeVector positionTrap(0., 0., LYSO_L*mm);// translate 1/4L in -Z, already set in Z trap
        G4RotationMatrix rotTrap  = G4RotationMatrix();// rotar 180 en Y
        rotTrap.rotateY(M_PI*rad);
        G4Transform3D transformTrap(rotTrap, positionTrap);
        solidLYSO = new G4UnionSolid("solidLYSO", trapLYSO1, trapLYSO2, transformTrap); 
    solidGlue = new G4Box("solidGlue", RESIN_W*mm, RESIN_H*mm+0.2*mm*G4UniformRand(), GLUE_L*mm);
    }


    solidResin = new G4Box("solidResin", RESIN_W*mm, RESIN_H*mm, RESIN_L*mm);

    solidDetector = new G4Box("solidDetector", DET_T*mm, DET_T*mm, DET_L);

    //G4ThreeVector position1(0, +0.5*mm+DET_T-RESIN_H, RESIN_L*mm);
    G4RotationMatrix rotm  = G4RotationMatrix();
    //G4Transform3D transform(rotm, position1);
    G4Transform3D tr = G4Translate3D(0, +0.5*mm+DET_T-RESIN_H, RESIN_L*mm) * G4Rotate3D(rotm) ;
    //G4Transform3D tr1 = G4Transform3D(rotm,position1);
    G4Box* box = new G4Box("Box_1",RESIN_W*mm, RESIN_H*mm, RESIN_L*mm+DET_L);
G4int nSiPM=16;
if(GeomConfig==1 || GeomConfig==2 ){
    Resin_Sub =new G4SubtractionSolid("Resin_Sub", box, solidDetector, tr);
} else if (GeomConfig==3){
    tr = G4Translate3D(-RESIN_W+DET_T+0.194*mm, +0.5*mm+DET_T-RESIN_H, RESIN_L*mm) * G4Rotate3D(rotm) ;
    Resin_Sub =new G4SubtractionSolid("Resin_Sub", box, solidDetector, tr);

    for(int i = 0; i < nSiPM; i++){
    tr = G4Translate3D(-RESIN_W+DET_T+0.194*(i+1)*mm+DET_T*2*i, +0.5*mm+DET_T-RESIN_H, RESIN_L*mm) * G4Rotate3D(rotm) ;
    Resin_Sub =new G4SubtractionSolid("Resin_Sub", Resin_Sub, solidDetector, tr);
    }
int i =1;
tr = G4Translate3D(-RESIN_W+DET_T+0.194*(i+1)*mm+DET_T*2*i,0.,0.) * G4Rotate3D(rotm) ;
    //LYSOAll_Add=new G4UnionSolid("LYSOAll_Add", solidLYSO , solidLYSO, tr);
    for(i = 0; i < nSiPM-1; i++){
    tr = G4Translate3D(LYSO_thick+DET_T+0.194*(i+1)*mm+DET_T*2*i,0.,0.) * G4Rotate3D(rotm) ;
    if(i==0){        LYSOAll_Add =new G4UnionSolid("LYSOAll_Add", solidLYSO , solidLYSO, tr);
            }else   {LYSOAll_Add =new G4UnionSolid("LYSOAll_Add", LYSOAll_Add, solidLYSO, tr);}
    }

}else if (GeomConfig==4){
    G4Box* LYSOBox = new G4Box("LYSOBox",LYSO_thick*mm, LYSO_thick*mm, LYSO_L);
    G4Box* BoxLYSOCover = new G4Box("Box_sideSurf",RESIN_W*mm, LYSO_thick*mm+0.194*mm, LYSO_L);

    for(int i = 0; i < nSiPM; i++){
    tr = G4Translate3D(-RESIN_W+DET_T+0.194*(i+1)*mm+DET_T*2*i,0.,0.) * G4Rotate3D(rotm) ;
    if(i==0){        LYSOCover_Sub =new G4SubtractionSolid("LYSOCover_Sub", BoxLYSOCover , LYSOBox, tr);
            }else   {LYSOCover_Sub =new G4SubtractionSolid("LYSOCover_Sub", LYSOCover_Sub, LYSOBox, tr);}
    }

}
    solidFR4 = new G4Box("solidFR4", RESIN_W*mm, RESIN_H*mm, FR4_L);


////////////////////
// LOGIC VOLUMES  // G4LogicalVolume(solid_, material, "name");
////////////////////
    logicDetector = new G4LogicalVolume(solidDetector, worldMat, "locigDetector"); // Defined outside in class

    if (GeomConfig==3){logicLYSO = new G4LogicalVolume(LYSOAll_Add, scintillator, "logicLYSO");}
    else{logicLYSO = new G4LogicalVolume(solidLYSO, scintillator, "logicLYSO");}

    if (GeomConfig==4){logicLYSOCover = new G4LogicalVolume(LYSOCover_Sub, scintillator, "logicLYSOCover");}

    logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");

    logicGlue = new G4LogicalVolume(solidGlue, RTV3145, "logicGlue");

    logicResin = new G4LogicalVolume(solidResin, EPOXY, "logicResin");

    logicResin_Sub = new G4LogicalVolume(Resin_Sub, EPOXY, "logicResin");

    logicFR4 = new G4LogicalVolume(solidFR4, SiO2, "logicFR4");

    //G4LogicalSkinSurface *skinGlue = new G4LogicalSkinSurface("skin",logicGlue,SurfGlue);

    //G4LogicalSkinSurface *skinResin = new G4LogicalSkinSurface("skin",logicResin,SurfResin);

    G4LogicalSkinSurface *skinFR4 = new G4LogicalSkinSurface("skin",logicFR4,SurfFR4);
   
 
    fScoringVolume = logicLYSO; fDetectorVolume=logicDetector;
//////////////////////
// PHYSICAL VOLUMES // G4PVPlacement(rotation,translation,logic_,"var",motherVolume,boolean,index if rep,check overlap);    
//////////////////////

    physWorld = new G4PVPlacement(0,G4ThreeVector(0.,0.,0.),logicWorld,"physWorld",0,false,0,true);

if(GeomConfig == 1 ){
    physLYSO = new G4PVPlacement(0,G4ThreeVector(0.,0.,-LYSO_L/2.*mm),logicLYSO,"physLYSO",logicWorld,false,0,true);       

    physGlue1 = new G4PVPlacement(0,G4ThreeVector(0.,0.,+1*(+LYSO_L*mm+GLUE_L*mm)),logicGlue,"physGlue1",logicWorld,false,0,true); 
    physGlue2 = new G4PVPlacement(0,G4ThreeVector(0.,0.,-1*(+LYSO_L*mm+GLUE_L*mm)),logicGlue,"physGlue2",logicWorld,false,0,true); 
G4RotationMatrix* rM = new G4RotationMatrix();
  rM->rotateY(180.*deg);
physResin1 = new G4PVPlacement(0     ,G4ThreeVector(XposTol*mm,YposTol*mm+(RESIN_H-0.5*mm-LYSO_thick),+1*(+LYSO_L*mm+GLUE_L*mm*2+RESIN_L*mm+DET_L)),logicResin_Sub,"physResin1",logicWorld,false,0,true); 
physResin2 = new G4PVPlacement(rM,G4ThreeVector(XposTol2*mm,YposTol2*mm+(RESIN_H-0.5*mm-LYSO_thick),-1*(+LYSO_L*mm+GLUE_L*mm*2+RESIN_L*mm+DET_L)),logicResin_Sub,"physResin2",logicWorld,false,0,true); 
    physDetector = new G4PVPlacement(0,G4ThreeVector(XposTol*mm,YposTol*mm,+1*(+LYSO_L*mm+RESIN_L*mm*2+2*GLUE_L*mm+DET_L)),logicDetector,"physDetector",logicWorld,false,0,true); 
    physDetector = new G4PVPlacement(0,G4ThreeVector(XposTol2*mm,YposTol2*mm,-1*(+LYSO_L*mm+RESIN_L*mm*2+2*GLUE_L*mm+DET_L)),logicDetector,"physDetector",logicWorld,false,1,true); 

physFR41 = new G4PVPlacement(0     ,G4ThreeVector(XposTol*mm,YposTol*mm+(RESIN_H-0.5*mm-LYSO_thick),+1*(+LYSO_L*mm+GLUE_L*mm*2+2*(RESIN_L*mm+DET_L)+FR4_L)),logicFR4,"physResin1",logicWorld,false,0,true); 
physFR42 = new G4PVPlacement(rM,G4ThreeVector(XposTol2*mm,YposTol2*mm+(RESIN_H-0.5*mm-LYSO_thick),-1*(+LYSO_L*mm+GLUE_L*mm*2+2*(RESIN_L*mm+DET_L)+FR4_L)),logicFR4,"physResin2",logicWorld,false,0,true);
//////////////
// GEOM TYPE 2
}else if (GeomConfig == 2) // GEOM TYPE 2
{

    physLYSO = new G4PVPlacement(0,G4ThreeVector(0.,0.,-LYSO_L/2.*mm),logicLYSO,"physLYSO",logicWorld,false,0,true);       

    physGlue1 = new G4PVPlacement   (0,G4ThreeVector(XposTol*mm,YposTol*mm+(RESIN_H-0.5*mm-DET_T),+1*(+LYSO_L*mm+GLUE_L*mm)),logicGlue,"physGlue1",logicWorld,false,0,true); 
    physResin1 = new G4PVPlacement  (0,G4ThreeVector(XposTol*mm,YposTol*mm+(RESIN_H-0.5*mm-DET_T),+1*(+LYSO_L*mm+GLUE_L*mm*2+RESIN_L*mm+DET_L)),logicResin_Sub,"physResin1",logicWorld,false,0,true); 
    physDetector = new G4PVPlacement(0,G4ThreeVector(XposTol*mm,YposTol*mm,+1*(+LYSO_L*mm+RESIN_L*mm*2+2*GLUE_L*mm+DET_L)),logicDetector,"physDetector",logicWorld,false,0,true); 
    physFR41 = new G4PVPlacement    (0,G4ThreeVector(XposTol*mm,YposTol*mm+(RESIN_H-0.5*mm-DET_T),+1*(+LYSO_L*mm+GLUE_L*mm*2+2*(RESIN_L*mm+DET_L)+FR4_L)),logicFR4,"physResin1",logicWorld,false,0,true); 
//////////////
// GEOM TYPE 3
} else if (GeomConfig == 3){// GEOM TYPE 3
    physGlue1 = new G4PVPlacement(0,G4ThreeVector(0.,0.,+1*(+LYSO_L*mm+GLUE_L*mm)),logicGlue,"physGlue1",logicWorld,false,0,true); 
    physGlue2 = new G4PVPlacement(0,G4ThreeVector(0.,0.,-1*(+LYSO_L*mm+GLUE_L*mm)),logicGlue,"physGlue2",logicWorld,false,0,true); 
G4RotationMatrix* rM = new G4RotationMatrix();
  rM->rotateY(180.*deg);
physResin1 = new G4PVPlacement(0     ,G4ThreeVector(XposTol*mm,YposTol*mm+(RESIN_H-0.5*mm-LYSO_thick),+1*(+LYSO_L*mm+GLUE_L*mm*2+RESIN_L*mm+DET_L)),logicResin_Sub,"physResin1",logicWorld,false,0,true); 
physResin2 = new G4PVPlacement(rM,G4ThreeVector(XposTol2*mm,YposTol2*mm+(RESIN_H-0.5*mm-LYSO_thick),-1*(+LYSO_L*mm+GLUE_L*mm*2+RESIN_L*mm+DET_L)),logicResin_Sub,"physResin2",logicWorld,false,0,true); 


for(int i = 0; i < nSiPM; i++){
    physDetector = new G4PVPlacement(0,G4ThreeVector(XposTol*mm-RESIN_W+DET_T+0.194*(i+1)*mm+DET_T*2*i,YposTol*mm,+1*(+LYSO_L*mm+RESIN_L*mm*2+2*GLUE_L*mm+DET_L)),logicDetector,"physDetector",logicWorld,false,i-1,true); 
}
for(int i = 0; i < nSiPM; i++){    
physDetector = new G4PVPlacement(0,G4ThreeVector(XposTol2*mm-RESIN_W+DET_T+0.194*(i+1)*mm+DET_T*2*i,YposTol2*mm,-1*(+LYSO_L*mm+RESIN_L*mm*2+2*GLUE_L*mm+DET_L)),logicDetector,"physDetector",logicWorld,false,i+nSiPM-1,true); 
}

physFR41 = new G4PVPlacement(0     ,G4ThreeVector(XposTol*mm,YposTol*mm+(RESIN_H-0.5*mm-LYSO_thick),+1*(+LYSO_L*mm+GLUE_L*mm*2+2*(RESIN_L*mm+DET_L)+FR4_L)),logicFR4,"physResin1",logicWorld,false,0,true); 
physFR42 = new G4PVPlacement(rM,G4ThreeVector(XposTol2*mm,YposTol2*mm+(RESIN_H-0.5*mm-LYSO_thick),-1*(+LYSO_L*mm+GLUE_L*mm*2+2*(RESIN_L*mm+DET_L)+FR4_L)),logicFR4,"physResin2",logicWorld,false,0,true);


    physLYSO = new G4PVPlacement(0,G4ThreeVector(-RESIN_W+LYSO_thick+0.194*mm,0.,-LYSO_L/2.*mm),logicLYSO,"physLYSO",logicWorld,false,0,true);   

} else if (GeomConfig == 4){
G4RotationMatrix* rM = new G4RotationMatrix();
physLYSOCover = new G4PVPlacement(0,G4ThreeVector(0.,0.,0.),logicLYSOCover,"physLYSO",logicWorld,false,0,true);    
  rM->rotateY(180.*deg);
for(int i = 0; i < nSiPM; i++){    
    physLYSO = new G4PVPlacement(0,G4ThreeVector(-RESIN_W+DET_T+0.194*(i+1)*mm+DET_T*2*i,0.,-LYSO_L/2.*mm),logicLYSO,"physLYSO",logicWorld,false,i-1,true);       
}
physFR41 = new G4PVPlacement(0     ,G4ThreeVector(XposTol*mm,YposTol*mm+(RESIN_H-0.5*mm-LYSO_thick),+1*(+LYSO_L*mm+GLUE_L*mm*2+2*(RESIN_L*mm+DET_L)+FR4_L)),logicFR4,"physResin1",logicWorld,false,0,true); 
physFR42 = new G4PVPlacement(rM,G4ThreeVector(XposTol*mm,YposTol*mm+(RESIN_H-0.5*mm-LYSO_thick),-1*(+LYSO_L*mm+GLUE_L*mm*2+2*(RESIN_L*mm+DET_L)+FR4_L)),logicFR4,"physResin2",logicWorld,false,0,true);

}
//////////////////////
// BORDER SURFACE //
//////////////////////
if( GeomConfig==1 || GeomConfig==3){
    if(ESRtrue==1){
    G4LogicalBorderSurface *LYSO_Air_Border = new G4LogicalBorderSurface("LYSO_Glue_Border",physLYSO,physWorld,mirrorSurface);   
    }
    /*G4LogicalBorderSurface *Glue_Air_Border1 = new G4LogicalBorderSurface("Glue_Air_Border1",physGlue1,physWorld,groundSurface);   
    G4LogicalBorderSurface *Glue_Air_Border2 = new G4LogicalBorderSurface("Glue_Air_Border2",physGlue2,physWorld,groundSurface);  
    G4LogicalBorderSurface *Resin_Air_Border1 = new G4LogicalBorderSurface("Glue_Air_Border1",physResin1,physWorld,groundSurface);   
    G4LogicalBorderSurface *Resin_Air_Border2 = new G4LogicalBorderSurface("Glue_Air_Border1",physResin2,physWorld,groundSurface);   
    G4LogicalBorderSurface *Glue_LYSO_Border1 = new G4LogicalBorderSurface("Glue_LYSO_Border1",physGlue1,physLYSO,groundSurface);   
    G4LogicalBorderSurface *Glue_LYSO_Border2 = new G4LogicalBorderSurface("Glue_LYSO_Border2",physGlue2,physLYSO,groundSurface);   
    G4LogicalBorderSurface *Glue_Resin_Border1 = new G4LogicalBorderSurface("Glue_LYSO_Border1",physGlue1,physResin1,groundSurface);   
    G4LogicalBorderSurface *Glue_Resin_Border2 = new G4LogicalBorderSurface("Glue_LYSO_Border2",physGlue2,physResin2,groundSurface); */
}
else if (GeomConfig == 2)
{
    if(ESRtrue==1){
    G4LogicalBorderSurface *LYSO_Air_Border = new G4LogicalBorderSurface("LYSO_Glue_Border",physLYSO,physWorld,mirrorSurface);   
    }
/*    G4LogicalBorderSurface *Glue_Air_Border1 = new G4LogicalBorderSurface("Glue_Air_Border1",physGlue1,physWorld,groundSurface);   
    G4LogicalBorderSurface *Resin_Air_Border1 = new G4LogicalBorderSurface("Glue_Air_Border1",physResin1,physWorld,groundSurface);   
    G4LogicalBorderSurface *Glue_LYSO_Border1 = new G4LogicalBorderSurface("Glue_LYSO_Border1",physGlue1,physLYSO,groundSurface);   
    G4LogicalBorderSurface *Glue_Resin_Border1 = new G4LogicalBorderSurface("Glue_LYSO_Border1",physGlue1,physResin1,groundSurface);   */

}else if (GeomConfig == 4){

}

//////////////////////
//  //
//////////////////////
    ConstructSDandField();

    if(ArgsPass->GetStepSize()>0){
  G4double maxStep = ArgsPass->GetStepSize()*LYSO_thick*mm;
  fStepLimit = new G4UserLimits(maxStep);
  logicWorld->SetUserLimits(fStepLimit);
  logicLYSO->SetUserLimits(fStepLimit);
    }

    G4cout<< "#####################" << G4endl;
    G4cout<< "#####################" << G4endl;
    G4cout<< "--Geometry Initialization-- " << G4endl;
    G4cout<< "-- Random values -- " << G4endl;
    G4cout<< "XPOS: " << XposTol << G4endl;
    G4cout<< "XPOS2: " << XposTol2 << G4endl;
    G4cout<< "YPOS: " << YposTol << G4endl;
    G4cout<< "YPOS2: " << YposTol2 << G4endl;
    G4cout<< "GLUE_L: " << GLUE_L*2 << G4endl;
    G4cout<< "RESIN_L: " << RESIN_L*2 << G4endl;
    G4cout<< "-- LYSO -- " << G4endl;
    G4cout<< "LYSO_L: " << LYSO_L*2 << G4endl;
    G4cout<< "RESIN_LTol: " << RESIN_LTol*2 << G4endl;
    G4cout<< "LYSO_thick: " << LYSO_thick*2 << G4endl;
    G4cout<< "LYSO_YIELD: " << LYSO_YIELD << G4endl;
    G4cout<< "#####################" << G4endl;
    G4cout<< "#####################" << G4endl;

//RETURN//
    return physWorld;
}
void MyDetectorConstruction::ConstructSDandField()
{
MySensitiveDetector *sensDet = new MySensitiveDetector("SensitiveDetector", ArgsPass);

logicDetector->SetSensitiveDetector(sensDet);

}

    void MyDetectorConstruction:: DefaultValues()
{
    LYSO_L = ArgsPass->GetGeom_LYSO_L();
    LYSO_thick=3./2.;
    RESIN_W=51.5/2.;
    LYSO_YIELD=ArgsPass->GetLYSO_Yield();
    LYSO_SCALERESOLUTION=ArgsPass->GetLYSO_ScaleResolution();
    XYTol=0.16;
     RESIN_LNOM=0.5;RESIN_LTol=0.1;
    if(ArgsPass->GetRnd_Geom()==1)
        {
        GLUE_L = 0.15+0.1*G4UniformRand();   GLUE_L=GLUE_L/2.;
        RESIN_L =(RESIN_LNOM-RESIN_LTol)+RESIN_LTol*2*G4UniformRand();   RESIN_L=RESIN_L/2.;
        XposTol = -0.05+G4UniformRand()*XYTol;
        YposTol = -0.05+G4UniformRand()*XYTol;
        XposTol2 = -0.05+G4UniformRand()*XYTol;
        YposTol2 = -0.05+G4UniformRand()*XYTol;
        }
    else{
        GLUE_L = 0.2;   GLUE_L=GLUE_L/2.;
        RESIN_L =0.5;   RESIN_L=RESIN_L/2.;
        XposTol = 0.;
        YposTol = 0.;
        XposTol2 = 0.;
        YposTol2 = 0.;
        }


    GeomConfig=ArgsPass->GetGeomConfig();
    perincr=ArgsPass->GetIncr();
    ESRtrue=1;
    LYSO_SC1=ArgsPass->GetLYSO_DecayT();
    LYSO_RT1=ArgsPass->GetLYSO_RiseT();
}
    void MyDetectorConstruction:: DefineMessengers()
{
    fMessenger = new G4GenericMessenger(this, "/detector/","Detector Construction");
    fMessenger->DeclareProperty("LYSO_L", LYSO_L, "Length of LYSO crystal in mm");

    fMessenger_thick = new G4GenericMessenger(this, "/detector/","Detector Construction");
    fMessenger_thick->DeclareProperty("LYSO_thick", LYSO_thick, "Section dimension for the LYSO in mm");

    fMessenger_ResinL = new G4GenericMessenger(this, "/detector/","Detector Construction");
    fMessenger_ResinL->DeclareProperty("GLUE_L", GLUE_L, "Glue thickness");

    fMessenger_GlueL = new G4GenericMessenger(this, "/detector/","Detector Construction");
    fMessenger_GlueL->DeclareProperty("RESIN_L", RESIN_L, "Resin thickness");

    fMessenger_XPos = new G4GenericMessenger(this, "/detector/","Detector Construction");
    fMessenger_XPos->DeclareProperty("XposTol", XposTol, "Glue average thickness");

    fMessenger_YPos = new G4GenericMessenger(this, "/detector/","Detector Construction");
    fMessenger_YPos->DeclareProperty("YposTol", YposTol, "Glue average thickness");

    fMessenger_XPos2 = new G4GenericMessenger(this, "/detector/","Detector Construction");
    fMessenger_XPos2->DeclareProperty("XposTol2", XposTol2, "Glue average thickness");

    fMessenger_YPos2 = new G4GenericMessenger(this, "/detector/","Detector Construction");
    fMessenger_YPos2->DeclareProperty("YposTol2", YposTol2, "Glue average thickness");

    fMessenger_GC = new G4GenericMessenger(this, "/detector/","Detector Construction");
    fMessenger_GC->DeclareProperty("GeomConfig", GeomConfig, "1 == Bar ; 2 == Tile");
    
    fMessenger_pi = new G4GenericMessenger(this, "/detector/","Detector Construction");
    fMessenger_pi->DeclareProperty("PerIncr", perincr, "LYSO section geometry parameter");

    fMessenger_ESR = new G4GenericMessenger(this, "/detector/","Detector Construction");
    fMessenger_ESR->DeclareProperty("ESR", ESRtrue, "LYSO covered by ESR?");

/* The material properties are created in the constructor and not in the Construct() function. They are not reinitialized with the /run/reinitialize command
    
    fMessenger_SR1 = new G4GenericMessenger(this, "/detector/","Detector Construction");
    fMessenger_SR1->DeclareProperty("LYSO_SC1", LYSO_SC1, "LYSO decay time [ns]");

    fMessenger_RT1 = new G4GenericMessenger(this, "/detector/","Detector Construction");
    fMessenger_RT1->DeclareProperty("LYSO_RT1", LYSO_RT1, "LYSO rise time [ps]");

    fMessenger_YIELD = new G4GenericMessenger(this, "/detector/","Detector Construction");
    fMessenger_YIELD->DeclareProperty("LYSO_YIELD", LYSO_YIELD, "Ammount of photons created per MeV deposited in the crystal");

    fMessenger_SR = new G4GenericMessenger(this, "/detector/","Detector Construction");
    fMessenger_SR->DeclareProperty("LYSO_SCALERESOLUTION", LYSO_SCALERESOLUTION, " ");

    fMessenger_vov = new G4GenericMessenger(this, "/detector/","Detector Construction");
    fMessenger_vov->DeclareProperty("Vov", Vovcon, "SiPM detection overvoltage");
*/
}






