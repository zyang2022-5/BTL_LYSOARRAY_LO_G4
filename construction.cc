#include "construction.hh"

MyDetectorConstruction::MyDetectorConstruction(MyG4Args* MainArgs)
{// constructor
    ArgsPass=MainArgs;
    DefaultValues();
    DefineMessengers();
    DefineMaterial(); // Run material function in the constructor

}

MyDetectorConstruction::~MyDetectorConstruction()
{}

void MyDetectorConstruction::DefineMaterial() // function to define a single time the materials when parametrizing, materials need to be defined in the class header! Definition of the function for the class::
{
/////////////
//Materials//
/////////////
    // import nist material data
    G4NistManager *nist = G4NistManager::Instance();

    // Find material in G4 database: Air
    worldMat = nist -> FindOrBuildMaterial("G4_AIR");
        // Material requirements for Cherenkov light
    G4double energyWorld[2] = {1.239841939*eV/0.9,1.239841939*eV/0.2};   // Calculate momentum from wavelength [0.2,0.9]nm to energy
    G4double rindexWorld[2] ={1.0, 1.0};                          // Define Refractive index for aerogel (constant, without dispersion in this case)
        //Refer material properties to material 
    G4MaterialPropertiesTable *mptWorld = new G4MaterialPropertiesTable();
    mptWorld->AddProperty("RINDEX", energyWorld, rindexWorld, 2);    // AddProperty("Mat Prop name", energy, values, num points)
    worldMat->SetMaterialPropertiesTable(mptWorld);

    // Define material class: SiO2 -> G4Material (name, density, number of components)
    SiO2 = new G4Material("SiO2", 2.201*g/cm3, 2);
    SiO2 -> AddElement(nist->FindOrBuildElement("Si"),1);
    SiO2 -> AddElement(nist->FindOrBuildElement("O"),2);
    G4MaterialPropertiesTable *mptSiO2 = new G4MaterialPropertiesTable();
    G4double energySiO2[2] = {1.239841939*eV/0.9,1.239841939*eV/0.2};
    G4double rindexSiO2[2] ={1.4585,1.4585};  
    G4double ABSSiO2[2] ={0.01*mm,0.01*mm};  
    mptSiO2->AddProperty("RINDEX", energySiO2, rindexSiO2, 2);    // AddProperty("Mat Prop name", energy, values, num points)
    mptSiO2->AddProperty("ABSLENGTH", energySiO2, ABSSiO2,2);
    SiO2->SetMaterialPropertiesTable(mptSiO2);


    // Define material class: Water -> G4Material (name, density, number of components)
    H2O = new G4Material("H2O", 1.000*g/cm3, 2);
    H2O -> AddElement(nist->FindOrBuildElement("H"),2);
    H2O -> AddElement(nist->FindOrBuildElement("O"),1);

    // Define Element class: Carbon -> element
    C = nist->FindOrBuildElement("C");
   
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // LYSO  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // LYSO definition:  https://github.com/jgpavez/lysoDetector/blob/master/src/lysoDetectorConstruction.cc

    G4double prelude_density = 7.15*g/cm3;
    prelude = new G4Material("prelude", prelude_density, 4);
    prelude->AddElement(nist->FindOrBuildElement("Lu"),66.0891838*perCent);
    prelude->AddElement(nist->FindOrBuildElement("Si"),6.6303495*perCent);
    prelude->AddElement(nist->FindOrBuildElement("O"), 18.8850049*perCent);
    prelude->AddElement(nist->FindOrBuildElement("Y"), 8.3954618*perCent);
    
    G4double Cecont =0.0019;
    scintillator = new G4Material("scintillator", prelude_density ,2);
    scintillator->AddMaterial(prelude,(100-Cecont*100)*perCent);
    scintillator->AddElement(nist->FindOrBuildElement("Ce"), (Cecont*100)*perCent);

    const G4int num = 32;
    G4double LYSO_ene[num]   =  {1.5*eV, 2.*eV ,  2.05*eV ,2.1*eV , 2.15*eV ,2.2*eV , 2.25*eV, 2.3*eV,  2.35*eV ,2.4*eV , 2.45*eV,2.5*eV , 2.55*eV ,2.6*eV , 2.65*eV, 2.7*eV , 2.75*eV, 2.8*eV , 2.85*eV ,2.9*eV ,2.95*eV ,3.*eV ,  3.05*eV, 3.1*eV , 3.15*eV ,3.2*eV , 3.25*eV, 3.3*eV , 3.35*eV ,3.4*eV , 3.45*eV, 5*eV};

    G4double LYSO_fast[num]  =  {0.0005, 0.00547041 , 0.00742609 , 0.00928865 , 0.01118718,  0.01389001,  0.01719669,  0.0213541,   0.02886363,  0.04151549,  0.06495384,  0.10380228,  0.16436757,  0.24529401,  0.34573305,  0.45515023,  0.56162549,  0.66179016,  0.77098452,  0.88530138,  0.97313946,  0.99724079,  0.97220698,  0.85723693,  0.65925813,  0.41169552,  0.13144243,  0.02564552,  0.01029099,  0.0040628,  0.00198485 ,0.0002};
    
    G4double LYSO_absv2[num]   =  {3100.12030366*mm,3043.12030366*mm, 2910.82651214*mm, 2784.92853358*mm, 2673.18826225*mm,
       2558.74171563*mm, 2434.67018908*mm, 2309.29098197*mm, 2174.98328362*mm,
       2040.58925841*mm, 1905.69079561*mm, 1770.63093617*mm, 1634.75136156*mm,
       1500.22714282*mm, 1368.97382295*mm, 1237.431274  *mm, 1105.49166167*mm,
        974.3012427 *mm,  843.12436693*mm,  707.75698288*mm,  574.16698216*mm,
        445.00515572*mm,  317.14663428*mm,  158.48142788*mm,   49.44703917*mm,
         13.05239972*mm,    5.72851208*mm,    3.75039458*mm,    3.30181138*mm,
          3.13218799*mm,    3.04577151*mm,3.*mm};

    G4double LYSO_r[num] =  {1.82,1.8200522,  1.82135278, 1.82265335, 1.82395392, 1.8252545,  1.82655507, 1.82785564, 1.82915621, 1.83045679, 1.83175736, 1.83305793, 1.8343585, 1.83565908, 1.83695965, 1.83826022, 1.8395608,  1.84086137, 1.84216194, 1.84346251, 1.84476309, 1.84606366, 1.84736423, 1.84866481, 1.84996538, 1.85126595, 1.85256652, 1.8538671,  1.85516767, 1.85646824, 1.85776882,1.86};

    G4double LYSO_scat[num] = {234.45212959*mm,234.45212959*mm, 227.68020394*mm, 221.23075095*mm, 215.08127251*mm, 209.21131583*mm, 203.6022461*mm,  198.23704898*mm, 193.08232197*mm, 188.33697299*mm, 183.67040027*mm, 179.27354488*mm, 174.97463851*mm, 170.85134353*mm, 166.91194186*mm, 163.09853917*mm, 159.42071546*mm, 155.88459404*mm, 152.46136681*mm, 149.16145631*mm, 146.04064832*mm, 142.88372645*mm, 139.96618158*mm, 137.03682031*mm, 134.27498273*mm, 131.74895681*mm,  129.02110085*mm, 126.41169996*mm, 123.94680927*mm, 121.54959211*mm, 119.27852085*mm,119.27852085*mm};
G4MaterialPropertiesTable *mptScint= new G4MaterialPropertiesTable();

  
  mptScint->AddProperty("RINDEX", LYSO_ene, LYSO_r,num);
  mptScint->AddProperty("SCINTILLATIONCOMPONENT1", LYSO_ene, LYSO_fast,num);
  mptScint->AddProperty("ABSLENGTH", LYSO_ene, LYSO_absv2,num);
  mptScint->AddConstProperty("SCINTILLATIONYIELD", LYSO_YIELD / MeV);/*Word data check*/
  mptScint->AddProperty("RAYLEIGH", LYSO_ene, LYSO_scat,num);
  mptScint->AddConstProperty("RESOLUTIONSCALE", LYSO_SCALERESOLUTION);/*10%*/
  mptScint->AddConstProperty("SCINTILLATIONTIMECONSTANT1", 39.1 * ns);
  mptScint->AddConstProperty("SCINTILLATIONYIELD1", LYSO_SC1);/*the fraction of photons in each component must be specified, all to component 1*/
  mptScint->AddConstProperty("SCINTILLATIONRISETIME1", LYSO_RT1 * ps);
  scintillator-> SetMaterialPropertiesTable(mptScint);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // RTV3145  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    H = nist -> FindOrBuildElement("H");
    Si = nist -> FindOrBuildElement("Si");
    O = nist -> FindOrBuildElement("O");
    C = nist -> FindOrBuildElement("C");
    RTV3145 = new G4Material("RTV3145", 1.27*g/cm3,4);
    RTV3145->AddElement(H, 5);
    RTV3145->AddElement(Si,3);
    RTV3145->AddElement(C,5);
    RTV3145->AddElement(O,5);

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
    G4MaterialPropertiesTable *mptRTV = new G4MaterialPropertiesTable();
    mptRTV->AddProperty("RINDEX",    RTV_ene,    RTV_RINDEX,     numRTV); // fraction of the light reflected (all=1)
    mptRTV->AddProperty("ABSLENGTH", RTV_ene,    RTV_ABSLEN,     numRTV); // fraction of the light reflected (all=1)
    RTV3145-> SetMaterialPropertiesTable(mptRTV);

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
 FR4->AddMaterial(EPOXY, 0.472*perCent);
 FR4->AddMaterial(SiO2, 0.528*perCent);
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
    G4double RESIN_W =6.5*mm;   RESIN_W=RESIN_W/2.;
    G4double RESIN_T =3.1*mm;RESIN_T=RESIN_T/2.;
    G4double FR4_L =0.8/2.*mm;     //FR4_L=FR4_L/2.;
    G4double DET_T =3.;DET_T=DET_T/2.;

    solidWorld = new G4Box("solidWorld", xWorld, yWorld, zWorld);
    if(GeomConfig==1){
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
    solidGlue = new G4Box("solidGlue", DET_T*mm, DET_T*mm+0.2*mm*G4UniformRand(), GLUE_L*mm);
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
    solidGlue = new G4Box("solidGlue", RESIN_T*mm, RESIN_W*mm+0.2*mm*G4UniformRand(), GLUE_L*mm);
    }


    solidResin = new G4Box("solidResin", RESIN_T*mm, RESIN_W*mm, RESIN_L*mm);

    solidDetector = new G4Box("solidDetector", DET_T*mm, DET_T*mm, DET_L);

    G4ThreeVector position1(0, +0.5*mm+DET_T-RESIN_W, RESIN_L*mm);
    G4RotationMatrix rotm  = G4RotationMatrix();
    G4Transform3D transform(rotm, position1);
    //G4Transform3D tr1 = G4Transform3D(rotm,position1);
    G4Box* box = new G4Box("Box_1",RESIN_T*mm, RESIN_W*mm, RESIN_L*mm+DET_L);
    G4SubtractionSolid* Resin_Sub =new G4SubtractionSolid("Resin_Sub", box, solidDetector, transform);

    solidFR4 = new G4Box("solidFR4", RESIN_T*mm, RESIN_W*mm, FR4_L);


////////////////////
// LOGIC VOLUMES  // G4LogicalVolume(solid_, material, "name");
////////////////////
    logicDetector = new G4LogicalVolume(solidDetector, worldMat, "locigDetector"); // Defined outside in class

    logicLYSO = new G4LogicalVolume(solidLYSO, scintillator, "logicLYSO");

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

    physLYSO = new G4PVPlacement(0,G4ThreeVector(0.,0.,-LYSO_L/2.*mm),logicLYSO,"physLYSO",logicWorld,false,0,true);       

if(GeomConfig == 1){
    physGlue1 = new G4PVPlacement(0,G4ThreeVector(0.,0.,+1*(+LYSO_L*mm+GLUE_L*mm)),logicGlue,"physGlue1",logicWorld,false,0,true); 
    physGlue2 = new G4PVPlacement(0,G4ThreeVector(0.,0.,-1*(+LYSO_L*mm+GLUE_L*mm)),logicGlue,"physGlue2",logicWorld,false,0,true); 
G4RotationMatrix* rM = new G4RotationMatrix();
  rM->rotateY(180.*deg);
physResin1 = new G4PVPlacement(0     ,G4ThreeVector(XposTol*mm,YposTol*mm+(RESIN_W-0.5*mm-LYSO_thick),+1*(+LYSO_L*mm+GLUE_L*mm*2+RESIN_L*mm+DET_L)),logicResin_Sub,"physResin1",logicWorld,false,0,true); 
physResin2 = new G4PVPlacement(rM,G4ThreeVector(XposTol*mm,YposTol*mm+(RESIN_W-0.5*mm-LYSO_thick),-1*(+LYSO_L*mm+GLUE_L*mm*2+RESIN_L*mm+DET_L)),logicResin_Sub,"physResin2",logicWorld,false,0,true); 
    physDetector = new G4PVPlacement(0,G4ThreeVector(XposTol*mm,YposTol*mm,+1*(+LYSO_L*mm+RESIN_L*mm*2+2*GLUE_L*mm+DET_L)),logicDetector,"physDetector",logicWorld,false,0,true); 
    physDetector = new G4PVPlacement(0,G4ThreeVector(XposTol*mm,YposTol*mm,-1*(+LYSO_L*mm+RESIN_L*mm*2+2*GLUE_L*mm+DET_L)),logicDetector,"physDetector",logicWorld,false,1,true); 

physFR41 = new G4PVPlacement(0     ,G4ThreeVector(XposTol*mm,YposTol*mm+(RESIN_W-0.5*mm-LYSO_thick),+1*(+LYSO_L*mm+GLUE_L*mm*2+2*(RESIN_L*mm+DET_L)+FR4_L)),logicFR4,"physResin1",logicWorld,false,0,true); 
physFR42 = new G4PVPlacement(rM,G4ThreeVector(XposTol*mm,YposTol*mm+(RESIN_W-0.5*mm-LYSO_thick),-1*(+LYSO_L*mm+GLUE_L*mm*2+2*(RESIN_L*mm+DET_L)+FR4_L)),logicFR4,"physResin2",logicWorld,false,0,true);
}
else if (GeomConfig == 2)
{
    physGlue1 = new G4PVPlacement   (0,G4ThreeVector(XposTol*mm,YposTol*mm+(RESIN_W-0.5*mm-DET_T),+1*(+LYSO_L*mm+GLUE_L*mm)),logicGlue,"physGlue1",logicWorld,false,0,true); 
    physResin1 = new G4PVPlacement  (0,G4ThreeVector(XposTol*mm,YposTol*mm+(RESIN_W-0.5*mm-DET_T),+1*(+LYSO_L*mm+GLUE_L*mm*2+RESIN_L*mm+DET_L)),logicResin_Sub,"physResin1",logicWorld,false,0,true); 
    physDetector = new G4PVPlacement(0,G4ThreeVector(XposTol*mm,YposTol*mm,+1*(+LYSO_L*mm+RESIN_L*mm*2+2*GLUE_L*mm+DET_L)),logicDetector,"physDetector",logicWorld,false,0,true); 
    physFR41 = new G4PVPlacement    (0,G4ThreeVector(XposTol*mm,YposTol*mm+(RESIN_W-0.5*mm-DET_T),+1*(+LYSO_L*mm+GLUE_L*mm*2+2*(RESIN_L*mm+DET_L)+FR4_L)),logicFR4,"physResin1",logicWorld,false,0,true); 
}
//////////////////////
// BORDER SURFACE //
//////////////////////
if(GeomConfig == 1){
    if(ESRtrue==1){
    G4LogicalBorderSurface *LYSO_Air_Border = new G4LogicalBorderSurface("LYSO_Glue_Border",physLYSO,physWorld,mirrorSurface);   
    }
    G4LogicalBorderSurface *Glue_Air_Border1 = new G4LogicalBorderSurface("Glue_Air_Border1",physGlue1,physWorld,groundSurface);   
    G4LogicalBorderSurface *Glue_Air_Border2 = new G4LogicalBorderSurface("Glue_Air_Border2",physGlue2,physWorld,groundSurface);  
    G4LogicalBorderSurface *Resin_Air_Border1 = new G4LogicalBorderSurface("Glue_Air_Border1",physResin1,physWorld,groundSurface);   
    G4LogicalBorderSurface *Resin_Air_Border2 = new G4LogicalBorderSurface("Glue_Air_Border1",physResin2,physWorld,groundSurface);   
    G4LogicalBorderSurface *Glue_LYSO_Border1 = new G4LogicalBorderSurface("Glue_LYSO_Border1",physGlue1,physLYSO,groundSurface);   
    G4LogicalBorderSurface *Glue_LYSO_Border2 = new G4LogicalBorderSurface("Glue_LYSO_Border2",physGlue2,physLYSO,groundSurface);   
    G4LogicalBorderSurface *Glue_Resin_Border1 = new G4LogicalBorderSurface("Glue_LYSO_Border1",physGlue1,physResin1,groundSurface);   
    G4LogicalBorderSurface *Glue_Resin_Border2 = new G4LogicalBorderSurface("Glue_LYSO_Border2",physGlue2,physResin2,groundSurface); 
}
else if (GeomConfig == 2)
{
    if(ESRtrue==1){
    G4LogicalBorderSurface *LYSO_Air_Border = new G4LogicalBorderSurface("LYSO_Glue_Border",physLYSO,physWorld,mirrorSurface);   
    }
    G4LogicalBorderSurface *Glue_Air_Border1 = new G4LogicalBorderSurface("Glue_Air_Border1",physGlue1,physWorld,groundSurface);   
    G4LogicalBorderSurface *Resin_Air_Border1 = new G4LogicalBorderSurface("Glue_Air_Border1",physResin1,physWorld,groundSurface);   
    G4LogicalBorderSurface *Glue_LYSO_Border1 = new G4LogicalBorderSurface("Glue_LYSO_Border1",physGlue1,physLYSO,groundSurface);   
    G4LogicalBorderSurface *Glue_Resin_Border1 = new G4LogicalBorderSurface("Glue_LYSO_Border1",physGlue1,physResin1,groundSurface);   

}  

//////////////////////
//  //
//////////////////////
    ConstructSDandField();

  G4double maxStep = 0.1*LYSO_thick*mm;
  fStepLimit = new G4UserLimits(maxStep);
  logicWorld->SetUserLimits(fStepLimit);
  logicLYSO->SetUserLimits(fStepLimit);

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
    LYSO_L = 57./2.;
    LYSO_thick=3./2.;

    LYSO_YIELD=ArgsPass->GetLYSO_Yield();
    LYSO_SCALERESOLUTION=ArgsPass->GetLYSO_ScaleResolution();
    if(ArgsPass->GetRnd_Geom()==1)
        {
        GLUE_L = 0.15+0.1*G4UniformRand();   GLUE_L=GLUE_L/2.;
        RESIN_L =0.3+0.4*G4UniformRand();   RESIN_L=RESIN_L/2.;
        XposTol = -0.05+G4UniformRand()*0.1;
        YposTol = -0.05+G4UniformRand()*0.1;
        }
    else{
        GLUE_L = 0.2;   GLUE_L=GLUE_L/2.;
        RESIN_L =0.5;   RESIN_L=RESIN_L/2.;
        XposTol = 0.;
        YposTol = 0.;
        }
    GeomConfig=1;
    perincr=0.;
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






