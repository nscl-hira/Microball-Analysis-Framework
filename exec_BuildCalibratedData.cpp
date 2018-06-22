#include <MBReader.h>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TFile.h>
#include <TTree.h>
#include <TString.h>
#include <TChain.h>
#include <TH2.h>
#include <iostream>

int main (int argc, char ** argv)
{
  if(argc<=1) return -1;
  std::string data_path("/mnt/analysis/e15190/HiRAEVTMapper_rootfiles/");
  TChain *dataChain = (TChain*) new TChain("E15190");
  std::string first_run_name(argv[1]);
  int first_run = atoi(first_run_name.c_str());
  int last_run=first_run;
  Long64_t evt_amount=0;
  if(argc==4) {
    evt_amount=atoi(argv[3]);
  }
  if(argc>2) {
    std::string last_run_name(argv[2]);
    last_run = atoi(last_run_name.c_str());
  }
  // Adding all the files to the TChain ================================
  for(int i=first_run; i<=last_run; i++)
  {
    char file_evt[100];
    sprintf (file_evt,"run-%04d*.root",i);
    std::string file_evt_string(file_evt);
    std::string path_to_evt_file(data_path+file_evt_string);
    int n_files = dataChain->Add((data_path+file_evt).c_str());
    printf("%d Root files added to chain for run %d\n", n_files, i);
  }

  MBReader MBAnalyzer(dataChain);
  MBAnalyzer.LoadGeometry("calibrations/MB_Geometry.dat");
  MBAnalyzer.LoadDetectorStatus("calibrations/MB_BadOrMissingDetectors.dat");
  MBAnalyzer.LoadMBFastSlowHitCondition("calibrations/MB_FastSlowCuts_2626.root");
  MBAnalyzer.LoadMBCentrality("calibrations/MB_ImpactParameter_48Ca_64Ni_140AMeV.dat");

  std::string FileOutName(Form("output/CalibratedData_%04d_%04d.root", first_run, last_run));

  MBAnalyzer.BuildCalibratedTree(FileOutName.c_str(), evt_amount);

  return 0;
}
