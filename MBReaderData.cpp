#include "include/MBReader.h"

//____________________________________________________
void MBReader::BuildCalibratedEvent()
{
  HTMicroballData * Microball = fMicroball->Get();

  //Filling calibrated data structures
  fMBCalibratedData.fmulti=0;
  for(int i=0; i<Microball->fmulti; i++)
  {
    if(!IsBad(Microball->fnumring[i], Microball->fnumdet[i]) && IsHit(Microball->fnumring[i], Microball->fnumdet[i],Microball->fFast[i],Microball->fTail[i],Microball->fTime[i])) {
      fMBCalibratedData.fnumring[fMBCalibratedData.fmulti] =Microball->fnumring[i];
      fMBCalibratedData.fnumdet[fMBCalibratedData.fmulti]  =Microball->fnumdet[i];
      fMBCalibratedData.fTail[fMBCalibratedData.fmulti]    =Microball->fTail[i];
      fMBCalibratedData.fFast[fMBCalibratedData.fmulti]    =Microball->fFast[i];
      fMBCalibratedData.fTime[fMBCalibratedData.fmulti]    =Microball->fTime[i];

      fMBCalibratedData.fTheta[fMBCalibratedData.fmulti]   =GetTheta(Microball->fnumring[i], Microball->fnumdet[i]);
      fMBCalibratedData.fPhi[fMBCalibratedData.fmulti]     =GetPhi(Microball->fnumring[i], Microball->fnumdet[i]);
      fMBCalibratedData.fThetaRan[fMBCalibratedData.fmulti]=GetThetaRandom(Microball->fnumring[i], Microball->fnumdet[i]);
      fMBCalibratedData.fPhiRan[fMBCalibratedData.fmulti]  =GetPhiRandom(Microball->fnumring[i], Microball->fnumdet[i]);

      fMBCalibratedData.fmulti++;
    }
    //Microball Event Varaibles
    fMBCalibratedData.fb     =GetImpactParameter(fMBCalibratedData.fmulti);
    fMBCalibratedData.fbhat  =Getbhat(fMBCalibratedData.fmulti);
  }
}

//____________________________________________________
void MBReader::BuildCalibratedTree(const char * file_name, Long64_t evt_amount)
{
  // this method constructs a calibrated data tree

  TFile * FileOut = new TFile(file_name, "RECREATE");
  if(FileOut->IsZombie()) {
    printf("Error: Cannot create file %s\n", file_name);
    return;
  }
  // Initializing output TTree
  TTree * TreeOut = new TTree ("E15190", "Calibrated Data");
  TreeOut->Branch("uBall.","MicroballCalibratedData",&fMBCalibratedData,32000,2);

  TreeOut->SetAutoSave(5000000);

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

    BuildCalibratedEvent();

    TreeOut->Fill();
  }

  //Save Output Tree to file
  TreeOut->AutoSave();

  // closing output file
  FileOut->Close();
}
