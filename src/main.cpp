#include <cassert>
#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <math.h>

#include "RBMtools.h"

using namespace std;

int main(int argc, char const *argv[]) {

  (void) argc;
  RBMposPro RBMo;
  string filename = argv[1];

  // Read input RBMposPro.in
  readRBMposPro(filename, RBMo);


  vector<int> reach_ind,cell_ind,  row, col;
  vector<double> lat, lon, delta_ind;

  // Read the *.Spat file
  readSpatFile(RBMo.indir, RBMo.studyCase, reach_ind, cell_ind, row, col, lat, lon, delta_ind);


  // Record the lan, lon matrixes
  saveLatLonMat(RBMo.outdir, row, col, lat, lon);

  // for(vector<int>::iterator it = row.begin(); it != row.end(); ++it) {
  //   cout << *it<<"\n";
  // }

  // double decDate=2001.9987;
  // yearMonthDay calDate;
  // decDate2CalDate(decDate, calDate);
  // std::cout <<calDate.year << ' '<< calDate.jday << ' '<< calDate.month<<' '<< calDate.day <<'\n';

  // Get completed months and years
  vector<int> months, years;
  getMonthsYears(RBMo.indir, RBMo.studyCase, months, years);

  // Estimate monthly averages
  if (RBMo.monthAv){
    if (!months.empty()){
      cout << "Saving monthly averages" << '\n';
      compMonthlyAve(RBMo.indir, RBMo.outdir, RBMo.var, RBMo.studyCase, reach_ind, cell_ind, row, col, months);
    }else{
      std::cout << "No saving monthly averages. No completed months." << '\n';
      //return 1;
    }
  }

  // Estimate yearly averages
  if (RBMo.yearAv){
    if (!years.empty()){
      cout << "Saving multi-annual averages" << '\n';
      compYearlyAve(RBMo.indir, RBMo.outdir, RBMo.var, RBMo.studyCase, reach_ind, cell_ind, row, col, years);
    }else{
      cout << "No saving for averaged years. No completed years." << '\n';
      //return 1;
    }
  }

  if (RBMo.ts){
    cout << "Saving time series at cells" << '\n';
    for (size_t i = 0; i < RBMo.npoints; i++) {
          cout << "-- Saving for cell: "<< i+1 << '\n';
          extractTS(RBMo.indir, RBMo.outdir, RBMo.var, RBMo.studyCase, reach_ind,  cell_ind,  lat,  lon, delta_ind, RBMo.startD, RBMo.endD, RBMo.plats[i],  RBMo.plons[i], i+1);
    }
  }

  if (RBMo.tsfield){
    cout << "Saving time series of fields" << '\n';
    extractField(RBMo.indir, RBMo.outdir, RBMo.var, RBMo.studyCase, cell_ind,  row,  col, delta_ind, RBMo.startD, RBMo.endD);
  }

  if (RBMo.tsreach){
    cout << "Saving time series of reaches" << '\n';
    for (size_t i = 0; i < RBMo.nreach; i++) {
      cout << "-- Saving for reach: "<< RBMo.reaches[i] << '\n';
      extractReach(RBMo.indir, RBMo.outdir, RBMo.var, RBMo.studyCase, reach_ind, lat,  lon, delta_ind, RBMo.startD, RBMo.endD, RBMo.reaches[i]);
    }
  }


  return 0;
}
