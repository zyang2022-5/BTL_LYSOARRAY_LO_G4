#include "materials.hh"
#include "util.hh"
#include "G4NistManager.hh"
#include "G4Material.hh"
#include "G4SystemOfUnits.hh"

/* Returns a G4Material representing the LYSO scintillator.
 * Right now we correctly specify the molecular
 * composition, the refractive index, absorption length, rayleigh scattering
 * length, and the scintillation spectrum and decay constant.
 *
 * Based on the LYSO definition here: https://github.com/jgpavez/lysoDetector/blob/master/src/lysoDetectorConstruction.cc
 * and using properties measured by the CMS BTL project.
 *
 * Note: You are responsible for freeing the object returned, i.e.
 *
 *     G4Material *lyso = get_lyso();
 *     // do stuff
 *     delete lyso;
 *
 * `light_yield` should be in photons/MeV, and `rise_time` in ps.
 *
 * FIXME: What is scale resolution? */
G4Material *get_lyso(double light_yield, double rise_time, double scale_resolution)
{
    int n;

    G4NistManager *nist = G4NistManager::Instance();

    G4double prelude_density = 7.125*g/cm3;
    G4Material *prelude = new G4Material("prelude", prelude_density, 4);
    prelude->AddElement(nist->FindOrBuildElement("Lu"),71.96*perCent);
    prelude->AddElement(nist->FindOrBuildElement("Si"),6.35*perCent);
    prelude->AddElement(nist->FindOrBuildElement("O"),18.08*perCent);
    prelude->AddElement(nist->FindOrBuildElement("Y"),3.62*perCent);
    
    G4double Cecont = 0.0019;
    G4Material *scintillator = new G4Material("scintillator", prelude_density ,2);
    scintillator->AddMaterial(prelude,(100-Cecont*100)*perCent);
    scintillator->AddElement(nist->FindOrBuildElement("Ce"), (Cecont*100)*perCent);

    G4MaterialPropertiesTable *mptScint = new G4MaterialPropertiesTable();

    G4double lyso_rindex_ene[1000], lyso_rindex_values[1000];

    n = read_tsv_file("data/lyso_rindex.dat", lyso_rindex_ene, lyso_rindex_values, eV, 1);

    if (n == -1) {
        fprintf(stderr, "error reading lyso_rindex.dat!\n");
        exit(1);
    }

    mptScint->AddProperty("RINDEX", lyso_rindex_ene, lyso_rindex_values, n);

    G4double lyso_spectrum_ene[1000], lyso_spectrum_values[1000];

    n = read_tsv_file("data/lyso_scintillation_spectrum.dat", lyso_spectrum_ene, lyso_spectrum_values, eV, 1);

    if (n == -1) {
        fprintf(stderr, "error reading lyso_scintillation_spectrum.dat!\n");
        exit(1);
    }

    mptScint->AddProperty("SCINTILLATIONCOMPONENT1", lyso_spectrum_ene, lyso_spectrum_values, n);

    G4double lyso_absorption_length_ene[1000], lyso_absorption_length_values[1000];

    n = read_tsv_file("data/lyso_absorption_length.dat", lyso_absorption_length_ene, lyso_absorption_length_values, eV, mm);

    if (n == -1) {
        fprintf(stderr, "error reading lyso_absorption_length.dat!\n");
        exit(1);
    }

    mptScint->AddProperty("ABSLENGTH", lyso_absorption_length_ene, lyso_absorption_length_values, n);
    mptScint->AddConstProperty("SCINTILLATIONYIELD", light_yield/MeV);

    G4double lyso_scattering_length_ene[1000], lyso_scattering_length_values[1000];

    n = read_tsv_file("data/lyso_scattering_length.dat", lyso_scattering_length_ene, lyso_scattering_length_values, eV, mm);

    if (n == -1) {
        fprintf(stderr, "error reading lyso_scattering_length.dat!\n");
        exit(1);
    }

    mptScint->AddProperty("RAYLEIGH", lyso_scattering_length_ene, lyso_scattering_length_values, n);
    mptScint->AddConstProperty("RESOLUTIONSCALE", scale_resolution);
    mptScint->AddConstProperty("SCINTILLATIONTIMECONSTANT1", 39.1 * ns);
    mptScint->AddConstProperty("SCINTILLATIONYIELD1", 1.0);
    mptScint->AddConstProperty("SCINTILLATIONRISETIME1", rise_time*ps);
    scintillator->SetMaterialPropertiesTable(mptScint);

    return scintillator;
}

/* Returns a G4Material representing the Silicone RTV used to glue the LYSO
 * array to the SiPMs. Right now we correctly specify the molecular
 * composition, the refractive index, and the absorption length.
 *
 * FIXME: What are the sources for this information? */
G4Material *get_rtv(void)
{
    G4Element *H, *Si, *O, *C;

    G4NistManager *nist = G4NistManager::Instance();

    H = nist->FindOrBuildElement("H");
    Si = nist->FindOrBuildElement("Si");
    O = nist->FindOrBuildElement("O");
    C = nist->FindOrBuildElement("C");
    G4Material *RTV3145 = new G4Material("RTV3145", 1.27*g/cm3,4);
    RTV3145->AddElement(H,5);
    RTV3145->AddElement(Si,3);
    RTV3145->AddElement(C,5);
    RTV3145->AddElement(O,5);

    G4MaterialPropertiesTable *mptRTV = new G4MaterialPropertiesTable();

    G4double rtv_rindex_ene[1000], rtv_rindex_values[1000];

    int n = read_tsv_file("data/rtv_rindex.dat", rtv_rindex_ene, rtv_rindex_values, eV, 1);

    if (n == -1) {
        fprintf(stderr, "error reading rtv_rindex.dat!\n");
        exit(1);
    }

    mptRTV->AddProperty("RINDEX", rtv_rindex_ene, rtv_rindex_values, n);

    G4double rtv_absorption_length_ene[1000], rtv_absorption_length_values[1000];

    n = read_tsv_file("data/rtv_absorption_length.dat", rtv_absorption_length_ene, rtv_absorption_length_values, eV, cm);

    if (n == -1) {
        fprintf(stderr, "error reading rtv_absorption_length.dat!\n");
        exit(1);
    }

    mptRTV->AddProperty("ABSLENGTH", rtv_absorption_length_ene, rtv_absorption_length_values, n);
    RTV3145->SetMaterialPropertiesTable(mptRTV);

    return RTV3145;
}

