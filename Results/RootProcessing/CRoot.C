void CRoot(){
    TCanvas *c1 = new TCanvas();
    TH1F *hist = new TH1F("hist","Histogram",100,0,200);
    TFile *input = new TFile("output0.root", "read");

    TTree *Hits = (TTree*)input->Get("Hits");
    //TTree *Scoring = (TTree*)input->Get("Scoring");    

    int HitEvt;
    double HitTime, HitPath;
    Hits->SetBranchAddress("fEvent",&HitEvt);
    Hits->SetBranchAddress("ftime",&HitTime);
    Hits->SetBranchAddress("fMeanPath",&HitPath);
    int entries = Hits->GetEntries();

    for(int i =0; i<entries; i++){
        Hits->GetEntry(i);
        hist->Fill(HitPath);
    }
    //hist->GetXaxis()->SetRangeUser(0, 400);
    hist->Draw();
    cout << entries<<" "<< HitTime<< endl;

    /*
    TCanvas *c2 = new TCanvas();
    TH1F *hist = new TH1F("hist","XY",100,-30,30,20,-1.5,1.5);
    TTree *Hits = (TTree*)input->Get("Scoring");
    //int HitEvt;
    double PX, PZ, LO;
    Hits->SetBranchAddress("fPrimaryX",&PX);
    Hits->SetBranchAddress("fPrimaryZ",&PZ);
    Hits->SetBranchAddress("fLOapprox",&LO);
    for(int i =0; i<entries; i++){
        Hits->GetEntry(i);
        hist->Fill(PZ,PX);
    }
    hist->Draw("colz");
    */

    input -> Close();

}
