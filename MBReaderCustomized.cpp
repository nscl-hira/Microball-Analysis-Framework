#include "include/MBReader.h"

//____________________________________________________
void MBReader::CreateFastSlowPlots(const char * file_name, Long64_t evt_amount)
{
  // this method creates fast/slow plots
  TFile * FileOut = new TFile(file_name, "RECREATE");



  //Histograms
  TH2D ** FastSlowHist[NUM_DETS_MICROBALL];
  for(int ring=0; ring<N_MICROBALL_RINGS; ring++) {
    FastSlowHist[ring]= new TH2D*[N_MICROBALL_DETS_PER_RING[ring]];
    for(int det=0; det<N_MICROBALL_DETS_PER_RING[ring]; det++) {
      FastSlowHist[ring][det]=new TH2D(Form("MBEvsTailR%02ddet%02d",ring+1,det+1),"",4096,0,4095,4096,0,4095);
    }
  }

  Long64_t nentries=fChain->GetEntries();
  if(evt_amount!=0) {
    nentries=evt_amount;
  }
  Long64_t jentry=0;
  std::cout << "found " << nentries << " entries\n";
  for(;fMBReader->Next() && jentry<nentries; jentry++)
  {
    if(jentry%100000==0) {
      printf("Percentage = %.2f %%\n", 100*double(jentry)/nentries);
    }

    HTMicroballData * Microball = fMicroball->Get();

    for(int i=0; i<Microball->fmulti; i++) {
      FastSlowHist[Microball->fnumring[i]-1][Microball->fnumdet[i]-1]->Fill(Microball->fTail[i],Microball->fFast[i]);
    }

    jentry++;
  }

  //Saving histograms to file
  for(int ring=0; ring<N_MICROBALL_RINGS; ring++) {
    for(int det=0; det<N_MICROBALL_DETS_PER_RING[ring]; det++) {
      FileOut->WriteTObject(FastSlowHist[ring][det], FastSlowHist[ring][det]->GetName());
    }
  }

  // closing output file
  FileOut->Close();
}
