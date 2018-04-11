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

int intUnique(vector<int>& inVec, vector<int>& outVec){
  outVec.push_back(inVec[0]);
  for(vector<int>::iterator i = inVec.begin(); i != inVec.end(); ++i) {
    bool in = false;
    for (vector<int>::iterator j = outVec.begin(); j != outVec.end(); ++j){
      if (*i == *j){
        in = true;
        break;
      }
    }
    if (!in) outVec.push_back(*i);
  }
  return 0;
}

int doubleUnique(vector<double>& inVec, vector<double>& outVec){
  outVec.push_back(inVec[0]);
  for(vector<double>::iterator i = inVec.begin(); i != inVec.end(); ++i) {
    bool in = false;
    for (vector<double>::iterator j = outVec.begin(); j != outVec.end(); ++j){
      if (*i == *j){
        in = true;
        break;
      }
    }
    if (!in) outVec.push_back(*i);
  }
  return 0;
}

int readSpatFile(string& indir, string& studyCase, vector<int>& reach_ind, vector<int>& cell_ind, vector<int>& row, vector<int>& col, vector<double>& lat, vector<double>& lon, vector<double>& delta_ind) {
  /* code */


  vector<int> reach_indD, cell_indD, rowD, colD;
  vector<double> latD, lonD, delta_indD;
  //string spatFile = indir + "Salmon_0.5.Spat";
  string spatFile = indir + studyCase + ".Spat";

  //cout << spatFile << '\n';

  ifstream myfile (spatFile.c_str());
  if (myfile.is_open())
  {
    string line;
    while ( getline (myfile,line) )
    {
      stringstream sep(line);
      string field;

      int i = 0;
      while (getline(sep, field, ' ')) {
        if (!field.empty()){
          //cout << field << '\n';
          //std::cout << field << '\n';
          if (i == 0) reach_indD.push_back(atoi(field.c_str()));
          if (i == 1) cell_indD.push_back(atoi(field.c_str()));
          if (i == 2) rowD.push_back(atoi(field.c_str()));
          if (i == 3) colD.push_back(atoi(field.c_str()));
          if (i == 4) latD.push_back(atof(field.c_str()));
          if (i == 5) lonD.push_back(atof(field.c_str()));
          if (i == 6) delta_indD.push_back(atof(field.c_str()));
          i++;
        }
      }
      //cout << line << '\n';
    }
    myfile.close();

    // for(vector<int>::size_type i = 0; i != reach_ind.size(); i++) {
    //   cout << col[i] <<"\n";
    // }
    //vector<double> delta_ind;
    doubleUnique(delta_indD, delta_ind);
    //cout << delta_ind.size() << '\n';


    //vector<int> reach_ind, cell_ind, row, col;
    //vector<double> lat, lon;
    for(vector<int>::size_type i = 0; i != reach_indD.size(); i = i+delta_ind.size()) {
      //std::cout << i << '\n';
      reach_ind.push_back(reach_indD[i]);
      cell_ind.push_back(cell_indD[i]);
      row.push_back(rowD[i]);
      col.push_back(colD[i]);
      lat.push_back(latD[i]);
      lon.push_back(lonD[i]);
    }


    //vector<double> last = unique(delta_ind.begin(), delta_ind.end());
    // std::cout << reach_ind.size()<<' '<< lon.size()<< '\n';
    // for(vector<int>::iterator it = row.begin(); it != row.end(); ++it) {
    //   cout << *it<<"\n";
    // }

  }else{
      cout << "Unable to open file";
      return 1;
  }

  return 0;
}

/*
Return the number of days of a year
*/
int ndaysYear(int year){
  if (((year % 4 == 0) && !(year % 100 == 0)) || (year % 400 == 0)) {
    return 366;
  }else{
    return 365;
  }
}


/*
Convert Julian day to month and day in gregorian calendar given the julian day and the year
*/
int Julian2MonthDay(yearMonthDay& calDate){
  const int daysnl[] = {31,59,90,120,151,181,212,243,273,304,334,365};
  const int daysyl[] = {31,60,91,121,152,182,213,244,274,305,335,366};
  int int_i,int_f;

  int ndaysY = ndaysYear(calDate.year);
  for (size_t i = 1; i < 12; i++) {
    if (ndaysY == 365) {
      int_i = daysnl[i-1];
      int_f = daysnl[i];
    } else if (ndaysY == 366){
      int_i = daysyl[i-1];
      int_f = daysyl[i];
    }
    if (calDate.jday <= 31) {
      calDate.month = 1;
      calDate.day = calDate.jday;
      return 0;
    } else {
      if ((calDate.jday > int_i) && (calDate.jday <= int_f)) {
        calDate.month = i+1;
        calDate.day = calDate.jday - int_i;
        return 0;
      } else if (calDate.jday == int_i) {
        calDate.month = i;
        if (ndaysY == 365) {
          calDate.day = calDate.jday - daysnl[i-1];
          return 0;
        } else if (ndaysY == 366) {
          calDate.day = calDate.jday - daysyl[i-1];
          return 0;
        }
      }
    }

  }
  return 0;
}


/*
Convert decimal year into gregorian calendar date. It also includes the julian day within the year.
*/
void decDate2CalDate(double decDate, yearMonthDay& calDate){
  calDate.year = (int)decDate;
  double deci =  decDate - calDate.year;
  int ndays = ndaysYear(calDate.year);
  calDate.jday = ndays * deci;
  if (calDate.jday==0){
    calDate.jday = ndays;
    calDate.year = calDate.year-1;
  }
  //std::cout << calDate.jday << '\n';

  Julian2MonthDay(calDate);
}

int compMonthlyAve(string& indir, string& outdir, string& var,  string& studyCase, vector<int>& reach_ind, vector<int>& cell_ind, vector<int>& row, vector<int>& col, vector<int>& months){

  double timed = 0., T_0 = 0., T_head = 0., dbt = 0., Q_in = 0.;
  int nr = 0, ncell = 0; // nd, ns

  string tempFile = indir + studyCase + ".Temp";

  //fstream myfile(HYDATdir+"STATIONS.csv", ios::in);
  fstream myfile(tempFile, ios::in);
  if(!myfile.is_open()){
    cout << "File not found!\n";
    return 1;
  }

  //ifstream myfile (tempFile.c_str());

  //if (myfile.is_open())
  //{
  unsigned int ncol = *max_element(col.begin(), col.end());
  unsigned int nrow = *max_element(row.begin(), row.end());
  //std::cout << ncol <<' '<<nrow << '\n';

  double varma[nrow][ncol];
  for (size_t i = 0; i < nrow; i++) {
    for (size_t j = 0; j < ncol; j++) {
      varma[i][j] = -999999.9;
    }
  }

  string month[] = {"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};

  for (vector<int>::iterator mo = months.begin(); mo != months.end(); ++mo) {

    cout << "-- Saving for month: "<< month[*mo-1] << '\n';

    //vector<double> T_0av;
    //int mo = 1;
    vector<int>::iterator re, ce;
    int rc = 0;
    for(re = reach_ind.begin(), ce = cell_ind.begin(); re != reach_ind.end() && ce != cell_ind.end(); ++re, ++ce) {

      string line;
      int k = 0;
      double vara = 0.;
      while ( getline (myfile,line) ){
        stringstream sep(line);
        string field;

        int i = 0;
        while (getline(sep, field, ' ')) {
          if (!field.empty()){

            if (i == 0) timed = atof(field.c_str());
            //if (i == 1) nd = atoi(field.c_str());
            if (i == 2) nr = atoi(field.c_str());
            if (i == 3) ncell = atoi(field.c_str());
            //if (i == 4) ns = atoi(field.c_str());
            if (i == 5) T_0 = atof(field.c_str());
            if (i == 6) T_head = atof(field.c_str());
            if (i == 7) dbt = atof(field.c_str());
            if (i == 8) Q_in = atof(field.c_str());

            i++;
          }
        }

        //cout <<k<<' ' <<timed<< ' ' << nd << '\n';
        yearMonthDay calDate;
        decDate2CalDate(timed, calDate);
        //std::cout <<calDate.year << ' '<< calDate.jday << ' '<< calDate.month<<' '<< calDate.day <<'\n';
        if (calDate.month == *mo) {
          if (*re == nr && *ce == ncell) {
            if (var == "T_0") {
              vara += T_0;
            } else if (var == "T_head") {
              vara += T_head;
            } else if (var == "dbt") {
              vara += dbt;
            } else if (var == "Q_in") {
              vara += Q_in;
            } else {
              cout << "Error, bad input, quitting\n";
              return 1;
            }

            k++;
          }
        }

        //cout << line << '\n';
      }

      //std::cout << mo <<' '<<*re <<' ' << *ce << ' ' <<T_0a/k <<'\n';
      //T_0av.push_back(T_0a/k);
      varma[nrow-row[rc]][col[rc]-1] = vara/k;
      //T_0ma[row[rc]-1][col[rc]-1] = T_0a/k;


      rc++;

      // Rewind the STATIONS.csv file
      myfile.clear();
      myfile.seekg(0);

    }
    //std::cout << *mo << '\n';



    // Write out 2D array
    fstream fileout(outdir+var+'_'+month[*mo-1]+".csv",ios::out);
    if(!fileout.is_open()){
      cout << "File not found!\n";
      return 1;
    }
    for (size_t i = 0; i < nrow; i++) {
      for (size_t j = 0; j < ncol; j++) {
        fileout << varma[i][j];
        if (j< ncol-1){
            fileout << ", ";
        }

      }
      fileout << '\n';
    }

  }

  return 0;
}


int compYearlyAve(string& indir, string& outdir, string& var,  string& studyCase, vector<int>& reach_ind, vector<int>& cell_ind, vector<int>& row, vector<int>& col, vector<int>& years){

  double timed = 0., T_0 = 0., T_head = 0., dbt = 0., Q_in = 0.;
  int nr = 0, ncell=0; // nd, ns

  string tempFile = indir + studyCase + ".Temp";

  //fstream myfile(HYDATdir+"STATIONS.csv", ios::in);
  fstream myfile(tempFile, ios::in);
  if(!myfile.is_open()){
    cout << "File not found!\n";
    return 1;
  }

  //ifstream myfile (tempFile.c_str());

  //if (myfile.is_open())
  //{
  unsigned int ncol = *max_element(col.begin(), col.end());
  unsigned int nrow = *max_element(row.begin(), row.end());
  //std::cout << ncol <<' '<<nrow << '\n';

  double varma[nrow][ncol];
  for (size_t i = 0; i < nrow; i++) {
    for (size_t j = 0; j < ncol; j++) {
      varma[i][j] = -999999.9;
    }
  }

  //vector<int> years = {2000, 2001};

  for (vector<int>::iterator yr = years.begin(); yr != years.end(); yr++) {

    cout << "-- Saving for year: "<< *yr << '\n';

    //vector<double> T_0av;
    //int mo = 1;
    vector<int>::iterator re, ce;
    int rc = 0;
    for(re = reach_ind.begin(), ce = cell_ind.begin(); re != reach_ind.end() && ce != cell_ind.end(); ++re, ++ce) {

      string line;
      int k = 0;
      double vara = 0.;
      while ( getline (myfile,line) ){
        stringstream sep(line);
        string field;

        int i = 0;
        while (getline(sep, field, ' ')) {
          if (!field.empty()){

            if (i == 0) timed = atof(field.c_str());
            //if (i == 1) nd = atoi(field.c_str());
            if (i == 2) nr = atoi(field.c_str());
            if (i == 3) ncell = atoi(field.c_str());
            //if (i == 4) ns = atoi(field.c_str());
            if (i == 5) T_0 = atof(field.c_str());
            if (i == 6) T_head = atof(field.c_str());
            if (i == 7) dbt = atof(field.c_str());
            if (i == 8) Q_in = atof(field.c_str());

            i++;
          }
        }

        //cout <<k<<' ' <<timed<< ' ' << nd << '\n';
        yearMonthDay calDate;
        decDate2CalDate(timed, calDate);
        //std::cout <<calDate.year << ' '<< calDate.jday << ' '<< calDate.month<<' '<< calDate.day <<'\n';
        if (calDate.year == *yr) {
          if (*re == nr && *ce == ncell) {
            if (var == "T_0") {
              vara += T_0;
            } else if (var == "T_head") {
              vara += T_head;
            } else if (var == "dbt") {
              vara += dbt;
            } else if (var == "Q_in") {
              vara += Q_in;
            } else {
              cout << "Error, bad input, quitting\n";
              return 1;
            }

            k++;
          }
        }

        //cout << line << '\n';
      }

      //std::cout << mo <<' '<<*re <<' ' << *ce << ' ' <<T_0a/k <<'\n';
      //T_0av.push_back(T_0a/k);
      varma[nrow-row[rc]][col[rc]-1] = vara/k;
      //T_0ma[row[rc]-1][col[rc]-1] = T_0a/k;


      rc++;

      // Rewind the STATIONS.csv file
      myfile.clear();
      myfile.seekg(0);

    }
    //std::cout << *yr << '\n';



    // Write out 2D array
    fstream fileout(outdir+var+'_'+to_string(*yr)+".csv",ios::out);
    if(!fileout.is_open()){
      cout << "File not found!\n";
      return 1;
    }
    for (size_t i = 0; i < nrow; i++) {
      for (size_t j = 0; j < ncol; j++) {
        fileout << varma[i][j];
        if (j< ncol-1){
            fileout << ", ";
        }

      }
      fileout << '\n';
    }

  }

  return 0;
}

int getMonthsYears(string& indir, string& studyCase, vector<int>& months, vector<int>& years){

  double timed = 0.;

  string tempFile = indir + studyCase + ".Temp";

  //fstream myfile(HYDATdir+"STATIONS.csv", ios::in);
  fstream myfile(tempFile, ios::in);
  if(!myfile.is_open()){
    cout << "File not found!\n";
    return 1;
  }

  vector<int> yearsAll, yearsInc, monthsAll;
  string line;
  while ( getline (myfile,line) ){
    stringstream sep(line);
    string field;

    int i = 0;
    while (getline(sep, field, ' ')) {
      if (!field.empty()){

        if (i == 0) timed = atof(field.c_str());
        //if (i == 1) nd = atoi(field.c_str());
        //if (i == 2) nr = atoi(field.c_str());
        //if (i == 3) ncell = atoi(field.c_str());
        //if (i == 4) ns = atoi(field.c_str());
        //if (i == 5) T_0 = atof(field.c_str());
        //if (i == 6) T_head = atof(field.c_str());
        //if (i == 7) dbt = atof(field.c_str());
        //if (i == 8) Q_in = atof(field.c_str());

        i++;
      }
    }

    //cout <<k<<' ' <<timed<< ' ' << nd << '\n';
    yearMonthDay calDate;
    decDate2CalDate(timed, calDate);
    yearsAll.push_back(calDate.year);
    monthsAll.push_back(calDate.month);
  }

  intUnique(monthsAll, months);
  intUnique(yearsAll, yearsInc);

  vector<int>::iterator i;
  for (i = yearsInc.begin(); i != yearsInc.end(); ++i) {
    int t1 = 0, t2=0;
    vector<int>::iterator j,k;
    for (j = yearsAll.begin(), k = monthsAll.begin(); j != yearsAll.end() && k != yearsAll.end(); ++j, ++k) {
      if (*i == *j && *k==1) {
        t1=1;
      } else if (*i == *j && *k==12) {
        t2=1;
      }
    }
    if (t1+t2 == 2){
      years.push_back(*i);
    }
  }

  //  for (vector<int>::iterator i = years.begin(); i != years.end(); ++i) {
  //    cout << *i << '\n';
  //  }
  //  for (vector<int>::iterator i = months.begin(); i != months.end(); ++i) {
  //    cout << *i << '\n';
  //  }

  return 0;

}

int writeOutReal2Darray(string& outdir, string& filename, double *mat, unsigned int nrow, unsigned int ncol){

  // Write out 2D array
  fstream fileout(outdir+filename,ios::out);
  if(!fileout.is_open()){
    cout << "File not found!\n";
    return 1;
  }
  for (size_t i = 0; i < nrow; ++i) {
    for (size_t j = 0; j < ncol; ++j) {
      fileout << *((mat+i*ncol) + j);
      if (j< ncol-1){
          fileout << ", ";
      }

    }
    fileout << '\n';
  }
  return 0;
}

int saveLatLonMat(string& outdir, vector<int>& row, vector<int>& col, vector<double>& lat, vector<double>& lon){

  unsigned int ncol = *max_element(col.begin(), col.end());
  unsigned int nrow = *max_element(row.begin(), row.end());
  //std::cout << ncol <<' '<<nrow << '\n';

  double matLat[nrow][ncol], matLon[nrow][ncol];

  // Initializing the matrices
  for (size_t i = 0; i < nrow; i++) {
    for (size_t j = 0; j < ncol; j++) {
      matLat[i][j] = -999999.9;
      matLon[i][j] = -999999.9;
    }
  }

  vector<int>::iterator i,j;
  unsigned int k = 0;
  for (i = row.begin(), j = col.begin(); i != row.end() && j != col.end(); ++i, ++j){
    matLat[nrow - *i][*j-1] = lat[k];
    matLon[nrow - *i][*j-1] = lon[k];
    k++;
  }

  // Get the delta lat
  bool fL = false;
  double dLat = 0.;
  for (size_t i = 0; i < nrow-1; i++) {
    for (size_t j = 0; j < ncol; j++) {
      if (matLat[i][j]!=-999999.9 && matLat[i+1][j]!=-999999.9){
        dLat = matLat[i][j]-matLat[i+1][j];
        fL = true;
        break;
      }
    }
    if (fL) break;
  }
  //cout << "dLat = " << dLat << '\n';

  double refLat = 0.;
  unsigned int idRefLat = 0;
  fL = false;
  for (size_t i = 0; i < nrow; i++) {
    for (size_t j = 0; j < ncol; j++) {
      if(matLat[i][j]!=-999999.9){
        //vLat[i] = matLat[i][j];
        refLat = matLat[i][j];
        idRefLat = i;
        fL = true;
        break;
      }
    }
    if (fL) break;
  }
  //std::cout << refLat << "lat " << idRefLat << '\n';

  double vLat[nrow];
  for (size_t i = 0; i < nrow; i++) {
    vLat[i] = refLat+((double)idRefLat - (double)i)*dLat;
    //cout << vLat[i] << '\n';
  }

  // Get the delta lon
  fL = false;
  double dLon = 0.;
  for (size_t i = 0; i < nrow; i++) {
    for (size_t j = 0; j < ncol-1; j++) {
      if (matLon[i][j]!= -999999.9 && matLon[i][j+1]!=-999999.9){
        dLon = matLon[i][j+1]-matLon[i][j];
        fL = true;
        break;
      }
    }
    if (fL) break;
  }
  //cout << "dLon = " << dLon << '\n';

  double refLon = 0.;
  unsigned int idRefLon = 0;
  fL = false;
  for (size_t j = 0; j < ncol; j++) {
    for (size_t i = 0; i < nrow; i++) {
      if(matLon[i][j]!=-999999.9){
        //vLon[j] = matLat[i][j];
        refLon = matLon[i][j];
        idRefLon = j;
        fL = true;
        break;
      }
    }
    if (fL) break;
  }
  //std::cout << refLon << "lon " << idRefLon << '\n';

  double vLon[ncol];
  for (size_t i = 0; i < ncol; i++) {
    vLon[i] = refLon-((double)idRefLon-(double)i)*dLon;
    //cout << i <<  "  "  <<refLon<<" "<< vLon[i] << '\n';
  }


  // Complete the lat and lon matrices
  for (size_t i = 0; i < nrow; i++) {
    for (size_t j = 0; j < ncol; j++) {
      if (matLat[i][j] == -999999.9) {
        matLat[i][j] = vLat[i];
      }
    }
  }
  for (size_t j = 0; j < ncol; j++) {
    for (size_t i = 0; i < nrow; i++) {
      if (matLon[i][j]==-999999.9) {
        matLon[i][j] = vLon[j];
      }
    }
  }


  // Save matrix of latitutes
  string filename = "lat.csv";
  writeOutReal2Darray(outdir, filename, (double *)matLat, nrow, ncol);

  // Save matrix of longitudes
  filename = "lon.csv";
  writeOutReal2Darray(outdir, filename, (double *)matLon, nrow, ncol);

  return 0;

}

int getTheClosestPoint(double plat, double plon, vector<double>& lat, vector<double>& lon){

  vector<double> dist;

  vector<double>::iterator i, j;
  for(j = lat.begin(), i = lon.begin(); j != lat.end() && i != lon.end(); ++j, ++i) {
    dist.push_back( sqrt(pow(*j-plat,2) + pow(*i-plon,2)) );
  }

  int posi=0;
  double distPosi = dist[posi];
  for (size_t i = 1; i < dist.size(); i++) {
    //cout << lat[i-1] << ' ' << lon[i-1] << ' ' << plat << ' ' << plon << ' ' << ' '<<dist[i-1]<<' '<< posi << ' '<<distPosi<<'\n';
    if (dist[i] < distPosi) {
      posi = i;
      distPosi = dist[posi];
    }
  }

  //cout << *min_element(dist.begin(), dist.end()) << '\n';

  return posi;
}


//####################################
int extractTS(string& indir, string& outdir, string& var,  string& studyCase, vector<int>& reach_ind, vector<int>& cell_ind, vector<double>& lat, vector<double>& lon, vector<double>& delta_ind, yearMonthDay& startD, yearMonthDay& endD, double plat, double plon, int pointID){

  double timed = 0., T_0 = 0., T_head = 0., dbt = 0., Q_in = 0.;
  int nr = 0, ncell = 0; // nd, ns


  int idd = getTheClosestPoint(plat, plon, lat, lon);
  //std::cout << lon.size() << " " << reach_ind.size() <<" "<< cell_ind.size() <<" "<< delta_ind.size()<< '\n';
  //cout << plat<<" "<<plon<<" IDD: " << idd << '\n';

  string tempFile = indir + studyCase + ".Temp";

  //fstream myfile(HYDATdir+"STATIONS.csv", ios::in);
  fstream myfile(tempFile, ios::in);
  assert(myfile.is_open());

  string line;
  //int k = 0;
  bool cond(false);
  vector<double> vara;
  vector<string> stringDates;
  while ( getline (myfile,line) ){
    stringstream sep(line);
    string field;

    int i = 0;
    while (getline(sep, field, ' ')) {
      if (!field.empty()){

        if (i == 0) timed = atof(field.c_str());
        //if (i == 1) nd = atoi(field.c_str());
        if (i == 2) nr = atoi(field.c_str());
        if (i == 3) ncell = atoi(field.c_str());
        //if (i == 4) ns = atoi(field.c_str());
        if (i == 5) T_0 = atof(field.c_str());
        if (i == 6) T_head = atof(field.c_str());
        if (i == 7) dbt = atof(field.c_str());
        if (i == 8) Q_in = atof(field.c_str());

        i++;
      }
    }

    if (reach_ind[idd]==nr && cell_ind[idd]==ncell) {
      //std::cout << reach_ind[idd]<< " "<< nr << '\n';
      //std::cout << cell_ind[idd]<< " "<< ncell<< '\n';
      //return -1;
      //cout <<k<<' ' <<timed<< ' ' << nd << '\n';
      //cout << nr<<' '<< ncell<< '\n';
      yearMonthDay calDate;
      decDate2CalDate(timed, calDate);
      //std::cout << calDate.jday << ' '<< calDate.day << '\n';

      if((calDate.year == startD.year) && (calDate.month == startD.month) && (calDate.day == startD.day)){
        cond = true;
      }


      //if (condS && condE) {
      if (cond) {

        //std::cout << calDate.jday << ' '<< calDate.day << '\n';
        string Date = to_string(calDate.year)+'/'+to_string(calDate.month)+'/'+to_string(calDate.day);
        //std::cout << Date << '\n';

        stringDates.push_back(Date);

        if (var == "T_0") {
          vara.push_back(T_0);
        } else if (var == "T_head") {
          vara.push_back(T_head);
        } else if (var == "dbt") {
          vara.push_back(dbt);
        } else if (var == "Q_in") {
          vara.push_back(Q_in);
        } else {
          cout << "Error, bad input, quitting\n";
          return 1;
        }
        if(calDate.year == endD.year && calDate.month == endD.month && calDate.day == endD.day){
          break;
        }
      }
    }
  }

  // Write out TS
  string TSfilename = var+"_point"+to_string(pointID)+".csv";
  fstream fileout(outdir+TSfilename,ios::out);
  assert(fileout.is_open());

  // for (size_t i = 0; i < 4; i++) {
  //   std::cout << stringDates[i] <<" " << vara[i]<< '\n';
  // }

  unsigned int k = 1;
  double acc = 0.;
  int j = 0;
  for(vector<double>::iterator i = vara.begin(); i != vara.end(); ++i) {
    acc += (*i);
    //std::cout << acc <<' ' <<k<< '\n';
    if (k==delta_ind.size()) {
        fileout << stringDates[j] << ", "<< acc/delta_ind.size() << "\n"; //
        acc = 0.;
        k = 0;
        j += 2;
    }
    k++;
  }

  return 0;
}





void readRBMposPro(string& filename, RBMposPro& RBMo){

  // Check existence of RBMmonYearAv.in file
  fstream file(filename);
  assert(file.is_open());

  // Reading the file
  string line;
  getline(file, line); // Comment
  getline(file, RBMo.indir);
  //cout << RBMo.indir << '\n';
  getline(file, line); // Comment
  getline(file, RBMo.studyCase);
  getline(file, line); // Comment
  getline(file, RBMo.outdir);
  getline(file, line); // Comment
  getline(file, RBMo.var);
  getline(file,line); // Comment
  getline(file,line);
  RBMo.ts = atoi(line.c_str());
  //std::cout << RBMo.ts << '\n';
  getline(file,line); // Comment
  getline(file,line);
  RBMo.tsfield = atoi(line.c_str());
  //file >> RBMo.tsfield;
  //std::cout << RBMo.tsfield << '\n';
  getline(file, line); // Comment
  getline(file, line);
  RBMo.tsreach = atoi(line.c_str());
  getline(file, line); // Comment
  getline(file, line);
  RBMo.monthAv = atoi(line.c_str());
  getline(file, line); // Comment
  getline(file, line);
  RBMo.yearAv = atoi(line.c_str());
  getline(file, line); // Comment
  getline(file, line);
  stringstream sep1(line);
  string field;
  int i = 0;
  while (getline(sep1, field, ' ')) {
    if (!field.empty()){
      if (i == 0) RBMo.startD.year = atoi(field.c_str());
      if (i == 1) RBMo.startD.month = atoi(field.c_str());
      if (i == 2) RBMo.startD.day = atoi(field.c_str());
      i++;
    }
  }
  //cout << RBMo.startD.year <<' '<< RBMo.startD.month <<' '<< RBMo.startD.day <<'\n';
  getline(file, line); // Comment
  getline(file, line);
  stringstream sep2(line);
  i = 0;
  while (getline(sep2, field, ' ')) {
    if (!field.empty()){
      if (i == 0) RBMo.endD.year = atoi(field.c_str());
      if (i == 1) RBMo.endD.month = atoi(field.c_str());
      if (i == 2) RBMo.endD.day = atoi(field.c_str());
      i++;
    }
  }
  //file >> RBMo.endD.year >> RBMo.endD.month >> RBMo.endD.day;
  //cout << RBMo.endD.year <<' '<< RBMo.endD.month <<' '<< RBMo.endD.day <<'\n';
  getline(file,line); // Comment
  getline(file,line);
  RBMo.npoints = atoi(line.c_str());
  //std::cout << RBMo.npoints << '\n';
  getline(file,line); // Comment
  for (size_t k = 0; k < RBMo.npoints; k++) {
    getline(file, line);
    stringstream sep3(line);
    i = 0;
    while (getline(sep3, field, ' ')) {
      if (!field.empty()){
        //if (i == 0) plats[i] = atof(field.c_str());
        if (i == 0) RBMo.plats.push_back(atof(field.c_str()));
        //if (i == 1) plons[i] = atof(field.c_str());
        if (i == 1) RBMo.plons.push_back(atof(field.c_str()));
        i++;
      }
    }
  }
  // for(vector<double>::iterator i = RBMo.plats.begin(); i != RBMo.plats.end(); ++i) {
  //   cout << *i<<"\n";
  // }
  getline(file, line); // Comment
  getline(file,line);
  RBMo.nreach = atoi(line.c_str());
  getline(file, line); // Comment
  for (size_t k = 0; k < RBMo.nreach; k++) {
    getline(file, line);
    RBMo.reaches.push_back(atoi(line.c_str()));
  }

}

//####################################
int extractField(string& indir, string& outdir, string& var,  string& studyCase, vector<int>& cell_ind, vector<int>& row, vector<int>& col, vector<double>& delta_ind, yearMonthDay& startD, yearMonthDay& endD){

  double timed = 0., T_0 = 0., T_head = 0., dbt = 0., Q_in = 0.;
  //int nr = 0, ncell = 0; // nd, ns

  // Reading input file
  string tempFile = indir + studyCase + ".Temp";
  fstream myfile(tempFile, ios::in);
  assert(myfile.is_open());

  // Filling-up base 2D array
  unsigned int ncol = *max_element(col.begin(), col.end());
  unsigned int nrow = *max_element(row.begin(), row.end());
  double varma[nrow][ncol];
  for (size_t i = 0; i < nrow; i++) {
    for (size_t j = 0; j < ncol; j++) {
      varma[i][j] = -999999.9;
    }
  }

  //std::cout << cell_ind.size() <<" "<<ncol<<" "<<nrow<< '\n';
  //for (size_t i = 0; i < cell_ind.size(); i++) {
  //  std::cout << row[i] <<" " << col[i] << '\n';
  //}

  unsigned int cell_indMAX = *max_element(cell_ind.begin(), cell_ind.end());

  // Write out 2D array
  fstream fileout(outdir + var + "_field" + ".csv", ios::out);
  assert(fileout.is_open());

  string line;
  unsigned int k = 0;
  double vara = 0.;
  int rc = 0;
  unsigned int counter = 0;
  bool cond(false);
  while ( getline (myfile,line) ){
    stringstream sep(line);
    string field;

    int i = 0;
    while (getline(sep, field, ' ')) {
      if (!field.empty()){

        if (i == 0) timed = atof(field.c_str());
        //if (i == 1) nd = atoi(field.c_str());
        //if (i == 2) nr = atoi(field.c_str());
        //if (i == 3) ncell = atoi(field.c_str());
        //if (i == 4) ns = atoi(field.c_str());
        if (i == 5) T_0 = atof(field.c_str());
        if (i == 6) T_head = atof(field.c_str());
        if (i == 7) dbt = atof(field.c_str());
        if (i == 8) Q_in = atof(field.c_str());

        i++;
      }
    }

    //cout <<k<<' ' <<timed<< ' ' << nd << '\n';
    yearMonthDay calDate;
    decDate2CalDate(timed, calDate);

    if((calDate.year == startD.year) && (calDate.month == startD.month) && (calDate.day == startD.day)){
      cond = true;
    }


    //std::cout <<calDate.year << ' '<< calDate.jday << ' '<< calDate.month<<' '<< calDate.day <<'\n';
    if (cond) {
      if (var == "T_0") {
        vara += T_0;
      } else if (var == "T_head") {
        vara += T_head;
      } else if (var == "dbt") {
        vara += dbt;
      } else if (var == "Q_in") {
        vara += Q_in;
      } else {
        cout << "Error, bad input, quitting\n";
        return 1;
      }
      k++;
    }
    if (k == delta_ind.size()) {
      //std::cout << row[rc]<<"  "<<col[rc] << '\n';
      varma[nrow-row[rc]][col[rc]-1] = vara/delta_ind.size();
      vara = 0.0;
      k = 0;
      rc++;
      counter++;
    }

    if (counter == cell_indMAX){
      fileout << to_string(calDate.year)+'/'+to_string(calDate.month)+'/'+to_string(calDate.day);
      fileout << '\n';
      for (size_t i = 0; i < nrow; i++) {
        for (size_t j = 0; j < ncol; j++) {
          fileout << varma[i][j];
          if (j< ncol-1){
            fileout << ", ";
          }
        }
        fileout << '\n';
      }
      counter = 0;
      rc = 0;

      if((calDate.year == endD.year) && (calDate.month == endD.month) && (calDate.day == endD.day)){
        break;
      }
    }
  }

  return 0;
}

// Count the number of occurrences of 'occ' in 'array'.
int nOccurencesInArray(vector<int>& array, int occ){
  unsigned int j = 0;
  for (vector<int>::iterator i = array.begin(); i != array.end(); ++i) {
    if (*i == occ){
      j++;
    }
  }
  return j;
}


//####################################
int extractReach(string& indir, string& outdir, string& var,  string& studyCase, vector<int>& reach_ind, vector<double>& lat, vector<double>& lon, vector<double>& delta_ind, yearMonthDay& startD, yearMonthDay& endD, int reach){

  double timed = 0., T_0 = 0., T_head = 0., dbt = 0., Q_in = 0.;
  int nr = 0; //, ncell = 0; // nd, ns

  // Get the number of cells in reach
  unsigned int ncells = nOccurencesInArray(reach_ind, reach);

  // Reading input file
  string tempFile = indir + studyCase + ".Temp";
  fstream myfile(tempFile, ios::in);
  assert(myfile.is_open());

  //
  double varma[ncells];
  for (size_t i = 0; i < ncells; i++) {
    varma[i] = -999999.9;
  }

  double vlat[ncells], vlon[ncells];
  unsigned int j = 0, ii = 0;
  for (vector<int>::iterator i = reach_ind.begin(); i != reach_ind.end(); ++i) {
    if (*i == reach){
      vlat[ii] = lat[j];
      vlon[ii] = lon[j];
      ii++;
    }
    j++;
  }

  // Write out
  fstream fileout(outdir + var + "_reach" + to_string(reach) + ".csv", ios::out);
  assert(fileout.is_open());

  for (size_t j = 0; j < ncells; j++) {
    fileout << vlat[j];
    if (j < ncells-1){
      fileout << ", ";
    }
  }
  fileout << '\n';

  for (size_t j = 0; j < ncells; j++) {
    fileout << vlon[j];
    if (j < ncells-1){
      fileout << ", ";
    }
  }
  fileout << '\n';

  string line;
  unsigned int k = 0;
  double vara = 0.;
  unsigned int counter = 0;
  bool cond(false);
  while ( getline (myfile,line) ){
    stringstream sep(line);
    string field;

    int i = 0;
    while (getline(sep, field, ' ')) {
      if (!field.empty()){

        if (i == 0) timed = atof(field.c_str());
        //if (i == 1) nd = atoi(field.c_str());
        if (i == 2) nr = atoi(field.c_str());
        //if (i == 3) ncell = atoi(field.c_str());
        //if (i == 4) ns = atoi(field.c_str());
        if (i == 5) T_0 = atof(field.c_str());
        if (i == 6) T_head = atof(field.c_str());
        if (i == 7) dbt = atof(field.c_str());
        if (i == 8) Q_in = atof(field.c_str());

        i++;
      }
    }

    //cout <<k<<' ' <<timed<< ' ' << nd << '\n';
    yearMonthDay calDate;
    decDate2CalDate(timed, calDate);

    if((calDate.year == startD.year) && (calDate.month == startD.month) && (calDate.day == startD.day)){
      cond = true;
    }


    //std::cout <<calDate.year << ' '<< calDate.jday << ' '<< calDate.month<<' '<< calDate.day <<'\n';
    if (cond) {
      if (reach == nr){
        if (var == "T_0") {
          vara += T_0;
        } else if (var == "T_head") {
          vara += T_head;
        } else if (var == "dbt") {
          vara += dbt;
        } else if (var == "Q_in") {
          vara += Q_in;
        } else {
          cout << "Error, bad input, quitting\n";
          return 1;
        }
        k++;
      }
    }
    if (k == delta_ind.size()) {
      //std::cout << row[rc]<<"  "<<col[rc] << '\n';
      varma[counter] = vara/delta_ind.size();
      vara = 0.0;
      k = 0;
      counter++;
    }

    if (counter == ncells){
      fileout << to_string(calDate.year)+'/'+to_string(calDate.month)+'/'+to_string(calDate.day);
      fileout << '\n';
      for (size_t j = 0; j < ncells; j++) {
        fileout << varma[j];
        if (j < ncells-1){
          fileout << ", ";
        }
      }
      fileout << '\n';
      counter = 0;

      if((calDate.year == endD.year) && (calDate.month == endD.month) && (calDate.day == endD.day)){
        break;
      }
    }
  }

  return 0;
}
