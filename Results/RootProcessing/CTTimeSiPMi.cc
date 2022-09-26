void CTTimeSiPMi(){

    TFile *input = new TFile("DefaultOutputName_Run3.root", "read");  
    TTree *Detected = (TTree*)input->Get("Detected");
    //TTree *Scoring = (TTree*)input->Get("Scoring");    

    double maxX, minX, maxt, mint;
    maxX=Detected->GetMaximum("fX");
    minX=Detected->GetMinimum("fX");
    maxt=Detected->GetMaximum("ftimeG");
    mint=Detected->GetMinimum("ftimeG");


    double fX, HitTime, fZ;
    Detected->SetBranchAddress("fX",&fX);
    Detected->SetBranchAddress("fZ",&fZ);
    Detected->SetBranchAddress("ftimeG",&HitTime);
    int entries = Detected->GetEntries();

    TH1F *hist = new TH1F("hist","Histogram",100,mint,maxt);
    for(int i =0; i<entries; i++){
        Detected->GetEntry(i);
        if(fX>-6.3 && fX<-3.2 && fZ>0){hist->Fill(HitTime);}
    }
    //hist->GetXaxis()->SetRangeUser(0, 400);
    TCanvas *c1 = new TCanvas();
    hist->Draw();
    //cout << entries<<" "<< HitTime<< endl;

}
