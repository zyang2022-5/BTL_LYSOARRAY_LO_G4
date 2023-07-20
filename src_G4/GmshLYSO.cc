#include "GmshLYSO.hh"

GmshLYSO :: GmshLYSO(MyG4Args *MainArgs)
{
	G4cout <<"Gmsh Initialize Default values" << G4endl;
	LocalArgs=MainArgs;
	Ztot = MainArgs->GetGeom_LYSO_L();
    Xtot=MainArgs->GetGeom_LYSO_thick()*2;
	ptsY=MainArgs->GetYincr();
	Znode=MainArgs->GetZnode();
	modelname=MainArgs->GetOutName();
	YSymm=MainArgs->GetYSymtrue();

	gmsh::initialize();
	gmsh::model::add(modelname);
  if(MainArgs->GetTile()==1){MakeTile();}else{MakeBar(MainArgs);}
gmsh::model::geo::synchronize();


// Export mesh to STL if MainArgs->GetMesh() is 1
if (MainArgs->GetGmshView() == 1) {
    gmsh::model::geo::synchronize();
	gmsh::fltk::run();
	gmsh::finalize();
}
gmsh::model::mesh::generate(3);
// Export mesh to STL if MainArgs->GetMesh() is 1
if (MainArgs->SaveMesh() == 1) {
    std::string stlFilename = "mesh.stl";
    gmsh::write(stlFilename);
}


// Getting nodal information

  // Print the model name and dimension:
  std::string name;
  gmsh::model::getCurrent(name);
  std::cout << "Model " << name << " (" << gmsh::model::getDimension()
            << "D)\n";

  std::vector<std::pair<int, int> > entities;
  gmsh::model::getEntities(entities);

  for(auto e : entities) {
    // Dimension and tag of the entity:
    int dim = e.first, tag = e.second;

    // Mesh data is made of `elements' (points, lines, triangles, ...), defined
    // by an ordered list of their `nodes'. Elements and nodes are identified by
    // `tags' as well (strictly positive identification numbers), and are stored
    // ("classified") in the model entity they discretize. Tags for elements and
    // nodes are globally unique (and not only per dimension, like entities).

    // A model entity of dimension 0 (a geometrical point) will contain a mesh
    // element of type point, as well as a mesh node. A model curve will contain
    // line elements as well as its interior nodes, while its boundary nodes
    // will be stored in the bounding model points. A model surface will contain
    // triangular and/or quadrangular elements and all the nodes not classified
    // on its boundary or on its embedded entities. A model volume will contain
    // tetrahedra, hexahedra, etc. and all the nodes not classified on its
    // boundary or on its embedded entities.

    // Get the mesh nodes for the entity (dim, tag):
    std::vector<std::size_t> nodeTags;
    std::vector<double> nodeCoords, nodeParams;
    gmsh::model::mesh::getNodes(nodeTags, nodeCoords, nodeParams, dim, tag);

    // Get the mesh elements for the entity (dim, tag):
    std::vector<int> elemTypes;
    gmsh::model::mesh::getElements(elemTypes, elemTags, elemNodeTags, dim, tag);

    std::string type;
    gmsh::model::getType(dim, tag, type);
    std::string name;
    gmsh::model::getEntityName(dim, tag, name);
    if(name.size()) name += " ";
    if(type == "Volume"){
		std::cout << "Entity " << name << "(" << dim << "," << tag << ") of type "
				  << type << "\n";

		// * Number of mesh nodes and elements:
		int numElem = 0;
		for(auto &tags : elemTags) numElem += tags.size();
		std::cout << " - Mesh has " << nodeTags.size() << " nodes and " << numElem
				  << " elements\n";
		Nelem=numElem;
		// * List all types of elements making up the mesh of the entity:
		for(auto elemType : elemTypes) {
		  std::string name;
		  int d, order, numv, numpv;
		  std::vector<double> param;
		  gmsh::model::mesh::getElementProperties(elemType, name, d, order, numv,
												  param, numpv);
		  std::cout << " - Element type: " << name << ", order " << order << "\n";
		  std::cout << "   with " << numv << " nodes in param coord: (";
		  for(auto p : param) std::cout << p << " ";
		  std::cout << ")\n";
		}
		//gmsh::model::mesh::getElements(elemTypes, elemTags, elemNodeTags, dim, tag);
		std::cout<<" - Nodal Tags of size "<< elemNodeTags.size() << ":: ";
		for(int i = 0; i < elemNodeTags[0].size(); i += 1){
			std::cout<< elemNodeTags[0][i]<< " ";
			}
		std::cout<<std::endl;

		//gmsh::model::mesh::getNodes(nodeTags, nodeCoords, nodeParams, dim, tag);
		gmsh::model::mesh::getNodes(tags, coord, param);
		std::cout<<tags.size()<<std::endl;
				std::cout<<coord.size()<<std::endl;
    for(std::size_t i = 0; i < coord.size(); i += 3) {
	std::cout<<"Coords::"<<coord[i]<<" "<<coord[i+1]<<" "<<coord[i+2]<<std::endl;
    }
    for(std::size_t i = 0; i < tags.size(); i += 1) {
	std::cout<<"Tags:: "<<tags[i]<<" ";
    }
	std::cout<<std::endl;
		
	}
  }
	
	}
	
GmshLYSO :: ~GmshLYSO()
{
	}

// Given a material assignment creates the logical and solid volumes 
// from the gmsh mesh
void GmshLYSO ::CreateG4LYSO(G4Material *material, G4LogicalVolume *logicWorld){
//	LocalArgs->SetVolume(0);

	int etag[4], gidx;
	double x[4],y[4],z[4];
	gidx = 1000;
	G4String tetname="G4Tet_";
	G4bool *degeneracyFlag;
	degeneracyFlag=0;
	LocalArgs->InitVolume();
	for(int i = 0; i < elemNodeTags[0].size(); i += 4) {
	//for(int i = 0; i < 1; i += 4) {
		//G4cout <<"Nodetagloop for element "<< i/4 << G4endl;
		etag[0] = elemNodeTags[0][i]-1;
		etag[1] = elemNodeTags[0][i+1]-1;
		etag[2] = elemNodeTags[0][i+2]-1;
		etag[3] = elemNodeTags[0][i+3]-1;
		G4cout <<etag[0]<<" "<<etag[1]<<" "<<etag[2]<<" "<<etag[3]<<" "<< G4endl;
        for(int j=0;j<4;j++){
            x[j]=coord[etag[j]*3];
            y[j]=coord[etag[j]*3+1];
            z[j]=coord[etag[j]*3+2];
			}
			/*
			G4cout <<"Coords x "<< x[0]<< " "<< x[1]<< " "<< x[2]<< " "<< x[3]<< " "<< G4endl;
			G4cout <<"Coords y "<< y[0]<< " "<< y[1]<< " "<< y[2]<< " "<< y[3]<< " "<< G4endl;
			G4cout <<"Coords z "<< z[0]<< " "<< z[1]<< " "<< z[2]<< " "<< z[3]<< " "<< G4endl;
			G4cout <<"Solid Tet "<< i/4 << G4endl;
			*/
			LYSOTet_Solid = new G4Tet(tetname+ G4String("solid_")+G4UIcommand::ConvertToString(gidx),
								G4ThreeVector(x[0]*mm,y[0]*mm, z[0]*mm),
								G4ThreeVector(x[1]*mm,y[1]*mm, z[1]*mm),
								G4ThreeVector(x[2]*mm,y[2]*mm, z[2]*mm),
								G4ThreeVector(x[3]*mm,y[3]*mm, z[3]*mm),
								degeneracyFlag);
			VolTet = TetraVolume(x,y,z);
			LocalArgs->AddVolume(VolTet);
			/*
			G4cout <<"Tet Volume "<< VolTet << G4endl;
			G4cout <<"Total Volume "<< LocalArgs->GetVolume() << G4endl;

			G4cout <<"Logic Tet "<< i/4 << G4endl;
			*/
			LYSOTet_Logic = new G4LogicalVolume(LYSOTet_Solid, material, tetname+ G4String("logical_")+G4UIcommand::ConvertToString(gidx));
			fScoringVolumeVec.push_back(LYSOTet_Logic);
			//G4cout <<"Phys Tet "<< i/4 << G4endl;
			LYSOTet_Phys = new G4PVPlacement(0,G4ThreeVector(0.,0.,0.),LYSOTet_Logic,tetname+ G4String("phys_")+G4UIcommand::ConvertToString(gidx),logicWorld,false,0,true);       
			lstPhysTet.push_back(LYSOTet_Phys);
			//G4cout <<"Tet Done "<< i/4 << G4endl;
			gidx+=1;
			//G4cout <<"gidx "<< gidx << G4endl;

		}
			G4cout <<"    ### "<< G4endl;
			G4cout <<"    ### Finished Mesh, Volume = "<< LocalArgs->GetVolume()<< G4endl;
			G4cout <<"    ### "<< G4endl;
	}


// Given a material assignment creates the logical and solid volumes 
// from the gmsh mesh
void GmshLYSO ::CreateG4LYSO_GC3(G4Material *material, G4LogicalVolume *logicWorld, G4double Det_X){
//	LocalArgs->SetVolume(0);
	G4cout <<"### DET_X used in CreateG4LYSO_GC3"<< Det_X << G4endl;
	int etag[4], gidx;
	double x[4],y[4],z[4];
	gidx = 1000;
	G4String tetname="G4Tet_";
	G4bool *degeneracyFlag;
	degeneracyFlag=0;
	LocalArgs->InitVolume();
	G4double* XposGC3 = new G4double[16];
	G4double Xtrans=-8;
	
	for (int i = 0; i < 16; i += 1) {
		XposGC3[i]=-Det_X*2*7-0.2*7-0.1-Det_X+(Det_X*2+0.2)*i;
		}
	for (int j = 0; j < 16; j += 1) {
		G4cout <<"### Translation: "<< XposGC3[j] << G4endl;
		Xtrans=XposGC3[j];
		for(int i = 0; i < elemNodeTags[0].size(); i += 4) {
		//for(int i = 0; i < 1; i += 4) {
			//G4cout <<"Nodetagloop for element "<< i/4 << G4endl;
			etag[0] = elemNodeTags[0][i]-1;
			etag[1] = elemNodeTags[0][i+1]-1;
			etag[2] = elemNodeTags[0][i+2]-1;
			etag[3] = elemNodeTags[0][i+3]-1;
			G4cout <<etag[0]<<" "<<etag[1]<<" "<<etag[2]<<" "<<etag[3]<<" "<< G4endl;
			for(int j=0;j<4;j++){
				x[j]=coord[etag[j]*3];
				y[j]=coord[etag[j]*3+1];
				z[j]=coord[etag[j]*3+2];
				}
				/*
				G4cout <<"Coords x "<< x[0]<< " "<< x[1]<< " "<< x[2]<< " "<< x[3]<< " "<< G4endl;
				G4cout <<"Coords y "<< y[0]<< " "<< y[1]<< " "<< y[2]<< " "<< y[3]<< " "<< G4endl;
				G4cout <<"Coords z "<< z[0]<< " "<< z[1]<< " "<< z[2]<< " "<< z[3]<< " "<< G4endl;
				G4cout <<"Solid Tet "<< i/4 << G4endl;
				*/
				LYSOTet_Solid = new G4Tet(tetname+ G4String("solid_")+G4UIcommand::ConvertToString(gidx),
									G4ThreeVector(x[0]*mm+Xtrans*mm,y[0]*mm, z[0]*mm),
									G4ThreeVector(x[1]*mm+Xtrans*mm,y[1]*mm, z[1]*mm),
									G4ThreeVector(x[2]*mm+Xtrans*mm,y[2]*mm, z[2]*mm),
									G4ThreeVector(x[3]*mm+Xtrans*mm,y[3]*mm, z[3]*mm),
									degeneracyFlag);
				VolTet = TetraVolume(x,y,z);
				LocalArgs->AddVolume(VolTet);
				/*
				G4cout <<"Tet Volume "<< VolTet << G4endl;
				G4cout <<"Total Volume "<< LocalArgs->GetVolume() << G4endl;

				G4cout <<"Logic Tet "<< i/4 << G4endl;
				*/
				LYSOTet_Logic = new G4LogicalVolume(LYSOTet_Solid, material, tetname+ G4String("logical_")+G4UIcommand::ConvertToString(gidx));
				fScoringVolumeVec.push_back(LYSOTet_Logic);
				//G4cout <<"Phys Tet "<< i/4 << G4endl;
				LYSOTet_Phys = new G4PVPlacement(0,G4ThreeVector(0.,0.,0.),LYSOTet_Logic,tetname+ G4String("phys_")+G4UIcommand::ConvertToString(gidx),logicWorld,false,0,true);       
				lstPhysTet.push_back(LYSOTet_Phys);
				//G4cout <<"Tet Done "<< i/4 << G4endl;
				gidx+=1;
				//G4cout <<"gidx "<< gidx << G4endl;

			}
		}
			G4cout <<"    ### "<< G4endl;
			G4cout <<"    ### Finished Mesh, Volume = "<< LocalArgs->GetVolume()<< G4endl;
			G4cout <<"    ### "<< G4endl;
	}



void GmshLYSO ::SurfaceCoating(G4VPhysicalVolume *physWorld, G4OpticalSurface *coating){
	int ic;
	for(int i = 0; i < Nelem; i ++){
		//    G4LogicalBorderSurface *LYSO_Air_Border = new G4LogicalBorderSurface("LYSO_Glue_Border",physLYSO,physWorld,mirrorSurface);   
		LYSO_Air_Border = new G4LogicalBorderSurface(G4String("ScintillationCoating_")+G4UIcommand::ConvertToString(i+def),lstPhysTet[i],physWorld,coating);   
		ic=i;
		}
		def=ic;
	
	}
	
void GmshLYSO ::SurfaceCoating_GC3(G4VPhysicalVolume *physWorld, G4OpticalSurface *coating){
	int ic;
	int def=0;
	for (int j = 0; j < 16; j += 1) {
		for(int i = 0; i < Nelem; i ++){
			//    G4LogicalBorderSurface *LYSO_Air_Border = new G4LogicalBorderSurface("LYSO_Glue_Border",physLYSO,physWorld,mirrorSurface);   
			LYSO_Air_Border = new G4LogicalBorderSurface(G4String("ScintillationCoating_")+G4UIcommand::ConvertToString(i+def),lstPhysTet[i+def],physWorld,coating);   
			ic=i;
			}
			def=def+ic;
		}
	}
	
void GmshLYSO ::MakeBar(MyG4Args *MainArgs){
// Geometry
G4cout <<"### Geometry:" << G4endl;
  double dZ=Ztot/Znode;
  std::vector<int> pp;
  std::vector<int> pm;
///////////////////////////
	if(YSymm == 0){
		
				G4cout <<"Y Symm:" << G4endl;

			ptsYF = new G4double[Znode*2+1];
			for(int i = 0; i < Znode+1; i++) { ptsYF[i]=ptsY[i];	}
			for(int i = 0; i < Znode; i++) { ptsYF[Znode+i+1]=ptsYF[Znode-i-1];}
			for(int i = 0; i < Znode*2+1; i++) {
				gmsh::model::geo::addPoint(-Xtot/2, +1*ptsYF[i]*1.5,-1*Ztot+dZ*i ,0,
										   1000 + i);
				std::cout<<-Xtot/2<< " " <<+1*ptsYF[i]*1.5<< " " <<+-1*Ztot+dZ*i<< std::endl;
				pp.push_back(1000 + i);
				if (MainArgs->GetforceBottomLine()==1) {
				gmsh::model::geo::addPoint(-Xtot/2, -0.1,-1*Ztot+dZ*i ,0,
										   2000 + i);
					pm.push_back(2000 + i);
				}else{
				gmsh::model::geo::addPoint(-Xtot/2, -1*ptsYF[i]*1.5,-1*Ztot+dZ*i ,0,  2000 + i);
					pm.push_back(2000 + i);
					}
		}
	}else{
		G4cout <<"No Y Symm:" << G4endl;
			ptsYF = new G4double[Znode*2+1];
			ptsYFb = new G4double[Znode*2+1];

			for(int i = 0; i < Znode+1; i++) { ptsYF[i]=ptsY[i];	}
			for(int i = 0; i < Znode; i++) { ptsYF[Znode+i+1]=ptsYF[Znode-i-1];}
			
			for(int i = 0; i < Znode*2+1; i++) {
				gmsh::model::geo::addPoint(-Xtot/2, +1*ptsYF[i]*1.5,-1*Ztot+dZ*i ,0,
										   1000 + i);
				std::cout<<-Xtot/2<< " " <<+1*ptsYF[i]*1.5<< " " <<+-1*Ztot+dZ*i<< std::endl;
				pp.push_back(1000 + i);
			}
			////////////////////////////////
			ptsYFb = new G4double[Znode*2+1];
			for(int i = 0; i < Znode+1; i++) { 
				ptsYFb[i]=ptsY[Znode+i+1];	
				G4cout <<"Yb: "<< i<<" "<<ptsYFb[i]<<" "<< Znode+i+1 <<" "<<ptsY[i] << G4endl;
				}
			for(int i = 0; i < Znode; i++) { 
				ptsYFb[Znode+i+1]=ptsYFb[Znode-i-1];
				G4cout << "ZSymm: "<< i << Znode+i+1 << " "<<ptsYFb[Znode+i+1] <<" "<<Znode-i-1<<" "<<ptsYFb[Znode-i-1] << G4endl;
				}
			
			for(int i = 0; i < Znode*2+1; i++) {
				double yCoord = -1 * ptsYFb[i] * 1.5;
				if (MainArgs->GetforceBottomLine()==1) {
					yCoord = -0.1; // Force the bottom line at a constant height of -0.1
				}

				gmsh::model::geo::addPoint(-Xtot/2, yCoord, -1 * Ztot + dZ * i, 0, 2000 + i);
				std::cout << -Xtot/2 << " " << yCoord << " " << -1 * Ztot + dZ * i << std::endl;
				pm.push_back(2000 + i);
			}

		}

G4cout <<"making geom:" << G4endl;
	std::vector<int> Cloop ;
	int splp, splm, l0, lm, cl, sf, ext,li,lii;
	if (LocalArgs->GetSpline()==1){
		splp=gmsh::model::geo::addSpline(pp);
		splm=gmsh::model::geo::addSpline(pm);
		l0 = gmsh::model::geo::addLine(pp[Znode*2], pm[Znode*2]);
		lm = gmsh::model::geo::addLine(pp[0], pm[0]);
		Cloop = {-lm, splp, l0, -splm};
//G4cout <<Cloop << G4endl;
	}else{
		G4cout <<"### Straight:" << G4endl;

		l0 = gmsh::model::geo::addLine(pp[Znode*2], pm[Znode*2]);
		lm = gmsh::model::geo::addLine(pp[0], pm[0]);
		Cloop.push_back(-lm);
		
		for(int i = 0; i < Znode*2; i++) { 
			std::cout <<  " lp index: "<< pp[i]<<" "<< pp[i+1]<<std::endl;
			li=gmsh::model::geo::addLine(pp[i],pp[i+1]);
			Cloop.push_back(li);
		}
		//li=gmsh::model::geo::addLine(pp[0],pp[Znode*2]);
		//Cloop.push_back(li);
		Cloop.push_back(l0);
		//lii=gmsh::model::geo::addLine(pm[0],pm[Znode*2]);
		//Cloop.push_back(-lii);
		for(int i = 0; i < Znode*2; i++) { 
			std::cout <<  " lm index: "<< pm[Znode*2-i]<<" "<< pm[Znode*2-i-1]<<std::endl;

			li=gmsh::model::geo::addLine(pm[Znode*2-i],pm[Znode*2-i-1]);
			Cloop.push_back(li);
		}

std::cout <<  " Cloop: ";	}
for (int i = 0; i < Cloop.size(); i++) {
  std::cout << Cloop[i] << " ";
}
std::cout << std::endl;

cl = gmsh::model::geo::addCurveLoop(Cloop);
sf = gmsh::model::geo::addPlaneSurface({cl});
std::vector<std::pair<int, int> > ov2;
gmsh::model::geo::extrude({{2, sf}}, Xtot, 0, 0,ov2,{1},{});
gmsh::model::geo::mesh::setTransfiniteCurve(l0,2);
gmsh::model::geo::mesh::setTransfiniteCurve(lm,2);
gmsh::model::mesh::setSize(ov2, 100);


G4cout <<"### Meshing:" << G4endl;
// Meshing
int nsecmesh=MainArgs->GetZelem();
if (LocalArgs->GetSpline()==1){
gmsh::model::geo::mesh::setTransfiniteCurve(splp,nsecmesh*2+1);
gmsh::model::geo::mesh::setTransfiniteCurve(splm,nsecmesh*2+1);
}
	
	}

void GmshLYSO ::MakeTile(){

	/*
	 * This function creates the Tile Detector configuration with a parametrized top surface and a flat bottom where the SiPMs are glued
	 * The commands to run it are: -TileV0 -Forcebottomline -Znode 11 -Ypos{1-1-1-1-1-1-1-1-1-1-1-1}
	 * Where the top surface has 2 symmetries trough Z=0 and X=0.
	 * The points parametrized by the Y height value in Ypos determine the height of 4 points in each of 3 planes across a given X value by the width of the crystal starting from the middle of the crystal 
	 * */
	 
	 // Currently we use a fixed number of nodes of 3x4 = 12. For this reason we need the argument -Znode <n> with n>=11 to read at least 12 values from Ypos.
     // TODO: make the number of sections and nodes an input parameter
	int nX=3; 		// Fixed sections through X
	int nodesec=4; 	// Fixed number of nodes along Z
	

    std::vector<double> Y_all; // reading the -Ypos value into a vector rather than an array 
    for (int i = 0; i < (nodesec)*nX; i++) {
		Y_all.push_back(ptsY[i]);
		std::cout<<Y_all[i]<<" "<<ptsY[i]<< " ";
	}
	std::cout<<std::endl;
	
	// Setting parameters
	// Yzero is the bottom constant value for the flat crystal surface [mm].
	// Xmin sets the starting point for the first section or first 4 points, the rest are determined through a number of divisions unitl X=0 (symmetry plane).
	// Zmin sets the starting point each point in each section until Zmax.
	double Xmin=-Xtot/2, Zmin=0., Yzero=-0.1;	 
	double Zmax=Ztot;
	double Xmax=0.;

	std::cout<< Zmin<< " "<< Zmax<< " "<< Xmin<< " "<<Xmax<< " "<<Yzero<< " "<<nX<< " "<<nodesec<< std::endl;
	
	// The Z value of each Ypos value is given equispaced segments from Xmin to Xmax 
    std::vector<double> Xins = generateEquispacedSegments(Xmin, Xmax, nX);
    // The symmetric values of X, by X=0, are given by XSym. We need one less point given the middle point was already defined (nX-1), for this same reason the initial point is one segment further (Xins[1]-Xins[0]). 
    std::vector<double> XSym = generateEquispacedSegments(Xmax+(Xins[1]-Xins[0]), Xmax+(Xmax-Xmin), nX-1);
	std::reverse(XSym.begin(), XSym.end()); // we reverse so that we can add both, Xins and Xsym, and go from the largest X to the smallest X values.
	
	printVectordouble(Xins);
	printVectordouble(XSym);
	
	// initialization of tags and vectors of tags
	std::vector<int> tr1,tr2,ptsv; 
    int surfaceTagi,idx; 
	std::vector<std::vector<int>> lineTagSecs,lineTag4Surf,PointTagSecs,combinedpts;
	std::vector<int> SurfTagSecs, lineTags1,lineTags2, pointTags, line4;
	// initialization of tags to a large value
	int ptc=1000,ltc=2000,stc=3000;
	
	
	// Point creation
	for (int sec = 0; sec < Xins.size(); sec++) {// loop over each section = X=cte values of 4 pts
		std::vector<double> Y = extractRange(Y_all, sec, nodesec);
		double Xin = Xins[sec];

		std::vector<std::vector<double>> points = calculatePoints(Y, Xin, Zmin, Zmax, Yzero); // returns a vector of vectors of the location of the points of each section (given by Y) including the bottom surface poitns as the last 2 vectors. This includes the Z=0 symmetry of each section.
		pointTags = createGmshPoints(points, ptc); 	// Create Gmsh point starting from tag ptc
		PointTagSecs.push_back(pointTags);			// vector of vectors storing point tags of each section. This includes for each section a new list of nX*2-1+2 equispaced pts from highest Z=LYSO_L to min Z=-LYSO_L and 2 extra points for the bottom surface
		ptc=pointTags.back();						// store the latest tag used to start from the following section tags
	}
	
	// Symmetrical X=0 point creation
	for (int sec = Xins.size()-2; sec > -1 ; sec--) {
		std::vector<double> Y = extractRange(Y_all, sec, nodesec);
		double Xin = XSym[sec];

		std::vector<std::vector<double>> points = calculatePoints(Y, Xin, Zmin, Zmax, Yzero);
		pointTags = createGmshPoints(points, ptc);
		PointTagSecs.push_back(pointTags);
		ptc=pointTags.back();
	}    
	
	//////////////////
    // Creating Lines
    //////////////////
	std::cout<<"nodesec iter: "<<nodesec<<std::endl;
    int nodesec1=nodesec*2-1; // modif of nodesec at flag for the total number of nodes using Z=0 symmetry
	std::cout<<"nodesec iter: "<<nodesec1<<std::endl;

	for (int sec = 0; sec < Xins.size()*2-2; sec++) { // loop over the total number of sections including X=0 symmetry
		combinedpts =combineAllIndicesint(PointTagSecs[sec], PointTagSecs[sec+1]); // returns a vector of vectors with the points of 2 consecutive sections making up all quadrilaterals between 2 consecutive sections
		for (int surf = 0; surf < nodesec1+2; surf++) { // loop over quadrilateral surface and division into triangles
			if (sec==0 && surf==0){ 	// condition for the first section and first quadrilateral
				ptsv=combinedpts[surf]; 	// points of the quadrilateral
				tr1 = getValuesAtIndices(ptsv,{0,1,2}); // returns the points at the given locations of the quadrilateral making the first triangle of the quadrilateral
				tr2 = getValuesAtIndices(ptsv,{0,2,3}); // returns the points at the given locations of the quadrilateral making the second triangle of the quadrilateral

				lineTags1 = createGmshLines(tr1, ltc,{}); 	// the function takes the points of the first triangle and makes all lines with the consequtive points : [0-1],[1-2],[2-0] lines
				line4=lineTags1;							// intermediate storage of the tags of the lines created (better code visualization)
 				lineTag4Surf.push_back(line4);				// store the tags of the lines in vector of vectors used to create each triangle surface, the lines need to be oriented with the orientation, right hand rule, pointing outside of the final volume
				ltc=lineTags1.back();						// store latest tag to start following line tags

				lineTags2 = createGmshLines(tr2, ltc,{0});	// the function takes the points of the second triangle and makes all lines, except the first one {0} which was already created with the previous triangle, with the consequtive points : [1-2],[2-0] lines
				line4={-lineTags1[2],lineTags2[0],lineTags2[1]}; // The second triangle uses a line in opposite direction [- sign] of the first triangle and both new lines in lineTags2 from the second triangle
				lineTag4Surf.push_back(line4);				// store the tags of the lines in vector of vectors used to create each triangle surface, the lines need to be oriented with the orientation, right hand rule, pointing outside of the final volume
				ltc=lineTags2.back();
			}else if (sec==0 && surf<nodesec1+1){// condition for the first section and follow up quadrilaterals with exception of the last one
				ptsv=combinedpts[surf];
				tr1 = getValuesAtIndices(ptsv,{0,1,2});
				tr2 = getValuesAtIndices(ptsv,{0,2,3});

				lineTags1 = createGmshLines(tr1, ltc,{});
				line4=lineTags1;
				lineTag4Surf.push_back(line4);
				ltc=lineTags1.back();

				lineTags2 = createGmshLines(tr2, ltc,{0,2});
				line4={-lineTags1[2],lineTags2[0],-lineTag4Surf[(surf-1)*2+0][1]};// The second triangle uses a line in opposite direction [- sign] of the first trianglethe new line in lineTags2 from the second triangle and a line from the previous quadrilateral whose tag is stored in the vector of vector to make all triangle surfaces
				lineTag4Surf.push_back(line4);
				ltc=lineTags2.back();
			
			}else if(sec==0 && surf==nodesec1+1){// condition for the first section last quadrilateral closing the section
				ptsv=combinedpts[surf];
				tr1 = getValuesAtIndices(ptsv,{0,1,2});
				tr2 = getValuesAtIndices(ptsv,{0,2,3});

				lineTags1 = createGmshLines(tr1, ltc,{1});
				line4={lineTags1[0],-lineTag4Surf[1][2],lineTags1[1]};
				lineTag4Surf.push_back(line4);
				ltc=lineTags1.back();

				lineTags2 = createGmshLines(tr2, ltc,{0,2});
				line4={-lineTags1[1],lineTags2[0],-lineTag4Surf[(surf-1)*2+0][1]};
				lineTag4Surf.push_back(line4);
				ltc=lineTags2.back();
				
			}else if(sec > 0  && surf==0){// condition for the first quadrilateral of sections different from the first one
				ptsv=combinedpts[surf];
				tr1 = getValuesAtIndices(ptsv,{0,1,2});
				tr2 = getValuesAtIndices(ptsv,{0,2,3});

				lineTags1 = createGmshLines(tr1, ltc,{0});
				line4={-lineTag4Surf[(sec-1)*(nodesec1+2)*2+surf*2+1][1],lineTags1[0],lineTags1[1]};;
				lineTag4Surf.push_back(line4);
				ltc=lineTags1.back();

				lineTags2 = createGmshLines(tr2, ltc,{0});
				line4={-lineTags1[1],lineTags2[0],lineTags2[1]};
				lineTag4Surf.push_back(line4);
				ltc=lineTags2.back();
				 
			}else if(sec > 0 && surf<nodesec1+1){// condition for follow up quadrilaterals with exception of the last one for sections different from the first one
				
				ptsv=combinedpts[surf];
				tr1 = getValuesAtIndices(ptsv,{0,1,2});
				tr2 = getValuesAtIndices(ptsv,{0,2,3});

				lineTags1 = createGmshLines(tr1, ltc,{0});
				line4={-lineTag4Surf[(sec-1)*(nodesec1+2)*2+surf*2+1][1],lineTags1[0],lineTags1[1]};;
				lineTag4Surf.push_back(line4);
				ltc=lineTags1.back();

				lineTags2 = createGmshLines(tr2, ltc,{0,2});
				line4={-lineTags1[1],lineTags2[0],-lineTag4Surf[(sec)*(nodesec1+2)*2+(surf-1)*2+0][1]};
				lineTag4Surf.push_back(line4);
				ltc=lineTags2.back();
				
			}else if(sec > 0  && surf==nodesec1+1){// condition for the last quadrilateral closing the section for sections different than the first one
				ptsv=combinedpts[surf];
				tr1 = getValuesAtIndices(ptsv,{0,1,2});
				tr2 = getValuesAtIndices(ptsv,{0,2,3});

				lineTags1 = createGmshLines(tr1, ltc,{0,1});
				line4={-lineTag4Surf[(sec-1)*(nodesec1+2)*2+surf*2+1][1],-lineTag4Surf[(sec)*(nodesec1+2)*2+surf*0+1][2],lineTags1[0]};;
				lineTag4Surf.push_back(line4);
				ltc=lineTags1.back();

				lineTags2 = createGmshLines(tr2, ltc,{0,2});
				line4={-lineTags1[0],lineTags2[0],-lineTag4Surf[(sec)*(nodesec1+2)*2+(surf-1)*2+0][1]};
				lineTag4Surf.push_back(line4);
				ltc=lineTags2.back();
			}
 		}
	}
	std::cout<<ltc<<std::endl;
	std::cout<<"### Surface Tags:"<<std::endl;
	printVectorOfVectors(lineTag4Surf);
	
    //////////////////
    // Creating Surfaces 
    //////////////////
	
	for (const auto& group : lineTag4Surf)
	{
		// Create surface using the four point tags in the group
		createGmshSurface(group,stc);
		SurfTagSecs.push_back(stc); // storage of surface tags
		stc++;
	}

	// add lateral surfaces to X=Xmin and X=Xmax to close the volume
	std::vector<int> s1 = getEvenElementsAtIndex(lineTag4Surf,0, 0, nodesec1*2+2); // return lines created for the first section corresponding to X=Xmin
	std::cout<<"s1: "<<std::endl;
	printVector(s1);
    std::reverse(s1.begin(), s1.end()); // Reverse the copy
	createGmshSurface(s1,stc);SurfTagSecs.push_back(stc);stc++;

	
	std::vector<int> s2 = getOddElementsAtIndex(lineTag4Surf,1, lineTag4Surf.size()-(nodesec1*2+3), lineTag4Surf.size()+1); // return lines created for the last section corresponding to X=Xmax
	std::cout<<"s2: "<<std::endl;
	printVector(s2);	
	createGmshSurface(s2,stc);SurfTagSecs.push_back(stc);stc++;
	
    gmsh::model::geo::synchronize(); // upload to gmsh the geometry defined previously, allows visualization and further operations
    //////////////////
    // Creating Solid
    //////////////////
    
    // Create a surface loop
    int surfaceLoop = gmsh::model::geo::addSurfaceLoop(SurfTagSecs);

    // Create a volume from the surface loop
    int volume = gmsh::model::geo::addVolume({surfaceLoop});
    
   // after this the class meshes automatically the volume and the created tetrahedrons are converted to G4 structures. 

}
