{
  gROOT->ProcessLine(".L lib/HTMicroballDict.so");
  gROOT->ProcessLine(".L lib/MBCalibratedDict.so");
  gROOT->ProcessLine(".L shared.cpp");
  gROOT->ProcessLine(".L MBReader.cpp");
  gROOT->ProcessLine(".L MBReaderData.cpp");
  gROOT->ProcessLine(".L MBReaderCustomized.cpp");
}
