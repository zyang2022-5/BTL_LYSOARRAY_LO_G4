void Multi3DCRoot(){

// Add arguments:
// TH1F or TH2F or ...
// which Branch to use -> known branches create the known labels
// Directory where the output files are

    int nh=50;
    TH3F *hist;

    //THStack *hstack = new THStack ("stack", "Light Output (50 geom)");

    //Tlegend *leg = new TLegend()
    Int_t entries;
    Double_t PX,PZ,LO,LOmin;
    LOmin=1500;
    Int_t per=0;
    hist = new TH3F (Form("geom%d",nh), "Geom",10,-30,30,10,-1.5,1.5,4,LOmin,2000);

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
            if(LO>LOmin){
                hist->Fill(PZ,PX,LO);
            }
            if(j==entries*0.1*per){
                per=per+1;
                cout << ".";                      
            }
        }
        cout<< " "<<endl;
        input -> Close();
    }

    TCanvas *c1 = new TCanvas();
    hist->SetMarkerStyle(kFullCircle);
    hist->Draw("Box2");

}
