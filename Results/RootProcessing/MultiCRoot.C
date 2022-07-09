void MultiCRoot(){
#include <string>
// Add arguments:
// TH1F or TH2F or ...
// which Branch to use -> known branches create the known labels
// Directory where the output files are

    int nh=50;
    TH1F *hist[nh];
    TH1F *histAvg;
    TString nameAvg = "geomAvg";
    histAvg = new TH1F (nameAvg, "GeomAvg",100,1000,2000);
    histAvg->SetFillStyle(3001);
    histAvg->SetFillColorAlpha(1+1, 0.01);

    TH1F *histSig;
    TString histSigname = "geomSig";
    histSig = new TH1F (histSigname, "GeomSig",100,0,200);
    histSig->SetFillStyle(3001);
    histSig->SetFillColorAlpha(1+2, 0.01);


    THStack *hstack = new THStack ("stack", "Light Output");
    //THStack *hstackAvg = new THStack ("stack Avg", "Light Output Avg");
    TLegend *leg = new TLegend(0.7,0.6,0.9,0.9);

    //Tlegend *leg = new TLegend()
    
    for (Int_t i = 0; i < nh; i++){
        TString name = Form("geom%i",i);

        hist[i] = new TH1F (name, "Geom",100,1000,2000);
        //hist[i] ->SetMarkerColor(i+1);
        //hist[i] ->SetMarkerStyle(i+1);
        hist[i]->SetFillColorAlpha(i+1, 0.01);
        hist[i]->SetFillStyle(3001);
        //histAvg[i]->SetFillColorAlpha(i+1, 0.01);
        //histAvg[i]->SetFillStyle(3001);
        hstack -> Add(hist[i]);
        //hstack -> Add(histAvg[i]);
    }
    Int_t entries;
    Double_t LO;
    Int_t per=0;
    for (Int_t i = 0; i < nh; i++){
        cout<< "***"<< i << "***";
        per =1;        
        TString filename = Form ("../output%d.root",i);
        TFile *input = new TFile(filename, "read"); 
        TTree *Scoring = (TTree*)input->Get("Scoring");
        Scoring->SetBranchAddress("fLOapprox",&LO);
        entries = Scoring->GetEntries();
        for(int j =0; j<entries; j++){
            Scoring->GetEntry(j);
            hist[i]->Fill(LO);
            if(j==entries*0.1*per){
                per=per+1;
                cout << ".";                      
            }
        }

        int meanval=hist[i]->GetMean(1);
        int sigval=hist[i]->GetStdDev(1);
        histAvg->Fill(meanval);
        histSig->Fill(sigval);
        cout<< "MeanVal: "<< meanval;
        TString namehist = Form("geomAvg%d",meanval);
        leg -> AddEntry(hist[i], namehist);
        cout<< " "<<endl;
        input -> Close();
    }

    TCanvas *c1 = new TCanvas();
    c1->SetGrid();
    c1->SetTickx();
    c1->SetTicky();
    //gStyle->SetPalette(57);
    //hstack -> Draw("PFC nostack");
    hstack -> Draw();
    //leg->Draw();


    TCanvas *c2 = new TCanvas();
    c2->SetGrid();
    c2->SetTickx();
    c2->SetTicky();
    //gStyle->SetPalette(57);
    histAvg -> Draw();
    //hstack -> Draw();

    TCanvas *c3 = new TCanvas();
    c3->SetGrid();
    c3->SetTickx();
    c3->SetTicky();
    //gStyle->SetPalette(57);
    histSig -> Draw();
    //hstack -> Draw();

}
