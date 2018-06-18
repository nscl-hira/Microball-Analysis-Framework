#include "include/MBReader.h"

//____________________________________________________
MBReader::MBReader(TChain *Chain, bool IsDataCalibrated, const char * DataType) :
fChain(Chain),
fDegToRad(TMath::Pi()/180.),
fRadToDeg(180./TMath::Pi()),
fSpeedOfLight(29.9792),
fIsMB(false),
fIsDataCalibrated(IsDataCalibrated),
fGeometryLoaded(false),
fHitConditionLoaded(false)
{
  //Parsing DataType string to allocate specific detectors
  std::string DetectorsIncluded(DataType);
  std::istringstream StreamDetectorsIncluded(DetectorsIncluded);
  std::string DetectorToAdd;
  while (StreamDetectorsIncluded>>DetectorToAdd) {
    if(DetectorToAdd.compare("MB")==0) fIsMB=true;
  }

  if(fChain!=0) {
    fMBReader = new TTreeReader(fChain);
    if(!fIsDataCalibrated) {
      if(fIsMB) fMicroball = new TTreeReaderValue<HTMicroballData>(*fMBReader, "uBall.");
    } else {
      if(fIsMB) fMicroballCal = new TTreeReaderValue<MicroballCalibratedData>(*fMBReader, "uBall.");
    }
  }
}

//____________________________________________________
MBReader::~MBReader()
{
  if(fChain!=0) {
    delete fMBReader;
    if(!fIsDataCalibrated) {
      if (fIsMB) delete fMicroball;
    } else {
      if (fIsMB) delete fMicroballCal;
    }
  }
}

//____________________________________________________
double MBReader::GetTheta(int num_ring, int num_det) const
{
  return fGeometryLoaded ? -9999 : -9999;
}

//____________________________________________________
double MBReader::GetPhi(int num_ring, int num_det) const
{
  return fGeometryLoaded ? -9999 : -9999;
}

//____________________________________________________
double MBReader::GetThetaRandom(int num_ring, int num_det) const
{
  return fGeometryLoaded ? -9999 : -9999;
}

//____________________________________________________
double MBReader::GetPhiRandom(int num_ring, int num_det) const
{
  return fGeometryLoaded ? -9999 : -9999;
}

//____________________________________________________
bool MBReader::IsHit (double fast, double tail, int num_ring, int num_det) const
{
  return fHitConditionLoaded ? false : false;
}

//____________________________________________________
void MBReader::Loop(const char * file_name, Long64_t evt_amount)
{
  // this method loops on the first "evt_amount" data entries
  // and creates a ROOT otput file named "file_name"
  // where the user can write some ROOT output
  TFile * FileOut = new TFile(file_name, "RECREATE");

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

    // Insert here code

    jentry++;
  }

  // closing output file
  FileOut->Close();
}

//____________________________________________________
void MBReader::LoopOnCalibratedData(const char * file_name, Long64_t evt_amount)
{
  // this method loops on the first "evt_amount" data entries
  // and creates a ROOT otput file named "file_name"
  // where the user can write some ROOT output
  TFile * FileOut = new TFile(file_name, "RECREATE");

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

    MicroballCalibratedData * Microball = fMicroballCal->Get();

    // Insert here code

    jentry++;
  }

  // closing output file
  FileOut->Close();
}
