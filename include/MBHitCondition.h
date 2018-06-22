#ifndef MBHITCONDITION_H
#define MBHITCONDITION_H

#include <TCutG.h>
#include <TFile.h>
#include <fstream>
#include <sstream>
#include <string>

#include "shared.h"

class MBHitCondition
{
public :
  MBHitCondition();
  ~MBHitCondition();

  int LoadFastSlowCuts(const char *);

  bool IsHit(int num_ring, int num_det, double fast, double slow, double time) const;

private :
  TCutG *** fFiducialCuts;
  TFile * fFileCuts;
  bool fCutsLoaded;

};

#endif
