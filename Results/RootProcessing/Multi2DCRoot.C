void Multi2DCRoot(){

// Add arguments:
// TH1F or TH2F or ...
// which Branch to use -> known branches create the known labels
// Directory where the output files are

    int nh=50;
    TH2F *hist[nh];

    THStack *hstack = new THStack ("stack", "Light Output (50 geom)");

    //Tlegend *leg = new TLegend()
    
    for (Int_t i = 0; i < nh; i++){
        TString name = Form("geom%d",i);
        hist[i] = new TH2F (name, "Geom",100,1000,2000,100,-30,30);
        
        hstack -> Add(hist[i]);
    }
    Int_t entries;
    Double_t PX,PZ,LO;
    Int_t per=0;
    for (Int_t i = 0; i < nh; i++){
        cout<< "***"<< i << "***";
        per =1;        
        TString filename = Form ("../output%d.root",i);
        TFile *input = new TFile(filename, "read"); 
        TTree *Scoring = (TTree*)input->Get("Scoring");
        Scoring->SetBranchAddress("fPrimaryX",&PX);
        Scoring->SetBranchAddress("fPrimaryZ",&PZ);
        Scoring->SetBranchAddress("fLOapprox",&LO);
        entries = Scoring->GetEntries();
        for(int j =0; j<entries; j++){
            Scoring->GetEntry(j);
            hist[i]->Fill(LO,PZ);
            if(j==entries*0.1*per){
                per=per+1;
                cout << ".";                      
            }
        }
        cout<< " "<<endl;
        input -> Close();
    }

    TCanvas *c1 = new TCanvas();
    c1->SetGrid();
    c1->SetTickx();
    c1->SetTicky();
    hstack -> Draw("COLZ");

}
