#include "include/MBReader.h"

//____________________________________________________
MBReader::MBReader(TChain *Chain, bool IsDataCalibrated, const char * DataType) :
fChain(Chain),
fDegToRad(TMath::Pi()/180.),
fRadToDeg(180./TMath::Pi()),
fSpeedOfLight(29.9792),
fIsMB(false),
fIsDataCalibrated(IsDataCalibrated),
fMBGeometryLoaded(false),
fMBStatusLoaded(false),
fMBHitConditionLoaded(false),
fMBCentralityLoaded(false),
fMicroballStatus(new MBDetectorStatus()),
fMicroballGeometry(new MBGeometry()),
fMicroballHitCondition(new MBHitCondition()),
fMicroballCentrality(new MBImpactParameter())
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
int MBReader::LoadMBGeometry(const char * file_name)
{
  int NLines=fMicroballGeometry->LoadGeometry(file_name);
  if(NLines>0) {
    fMBGeometryLoaded=true;
    printf("Loaded Microball geometry from file %s\n", file_name);
    return NLines;
  } else {
    fMBGeometryLoaded=false;
    printf("Error: Error while loading Microball geometry %s\n", file_name);
    return -1;
  }
}

//____________________________________________________
int MBReader::LoadMBDetectorStatus(const char * file_name)
{
  int NLines=fMicroballStatus->LoadBadDetectors(file_name);
  if(NLines>0) {
    fMBStatusLoaded=true;
    printf("Loaded Microball status from file %s\n", file_name);
    return NLines;
  } else {
    fMBStatusLoaded=false;
    printf("Error: Error while loading Microball status %s\n", file_name);
    return -1;
  }
}

//____________________________________________________
int MBReader::LoadMBFastSlowHitCondition(const char * file_name)
{
  int NLines=fMicroballHitCondition->LoadFastSlowCuts(file_name);
  if(NLines>0) {
    fMBHitConditionLoaded=true;
    printf("Loaded Microball Fast Slow Hit conditions from file %s\n", file_name);
    return NLines;
  } else {
    fMBHitConditionLoaded=false;
    printf("Error: Error while loading Microball Fast Slow Hit conditions %s\n", file_name);
    return -1;
  }
}

//____________________________________________________
int MBReader::LoadMBCentrality(const char * file_name)
{
  int NLines=fMicroballCentrality->LoadImpactParameter(file_name);
  if(NLines>0) {
    fMBCentralityLoaded=true;
    printf("Loaded Microball centrality conditions from file %s\n", file_name);
    return NLines;
  } else {
    fMBCentralityLoaded=false;
    printf("Error: Error while loading Microball centrality conditions %s\n", file_name);
    return -1;
  }
}

//____________________________________________________
double MBReader::GetTheta(int num_ring, int num_det) const
{
  return fMBGeometryLoaded ? fMicroballGeometry->GetTheta(num_ring, num_det) : -9999;
}

//____________________________________________________
double MBReader::GetPhi(int num_ring, int num_det) const
{
  return fMBGeometryLoaded ? fMicroballGeometry->GetPhi(num_ring, num_det) : -9999;
}

//____________________________________________________
double MBReader::GetThetaRandom(int num_ring, int num_det) const
{
  return fMBGeometryLoaded ? fMicroballGeometry->GetThetaRandom(num_ring, num_det) : -9999;
}

//____________________________________________________
double MBReader::GetPhiRandom(int num_ring, int num_det) const
{
  return fMBGeometryLoaded ? fMicroballGeometry->GetPhiRandom(num_ring, num_det) : -9999;
}

//____________________________________________________
double MBReader::GetImpactParameter(int multiplicity) const
{
  return fMBCentralityLoaded ? fMicroballCentrality->GetImpactParameter(multiplicity) : -9999;
}

//____________________________________________________
double MBReader::Getbhat(int multiplicity) const
{
  return fMBCentralityLoaded ? fMicroballCentrality->Getbhat(multiplicity) : -9999;
}

//____________________________________________________
bool MBReader::IsBad (int num_ring, int num_det) const
{
  return fMBStatusLoaded ? fMicroballStatus->IsBad(num_ring,num_det) : false;
}

//____________________________________________________
bool MBReader::IsHit (int num_ring, int num_det, double fast, double tail, double time) const
{
  return fMBHitConditionLoaded ? fMicroballHitCondition->IsHit(num_ring,num_det,fast,tail,time) : false;
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
