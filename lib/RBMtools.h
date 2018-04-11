#include <iostream>
#include <string>
#include <vector>


using namespace std;

#ifndef ADD_HEADER
#define ADD_HEADER

struct yearMonthDay{
  int year, month, day, jday;
};

struct RBMposPro{
  string indir, studyCase, outdir, var;
  bool ts, tsfield, tsreach, monthAv, yearAv;
  yearMonthDay startD, endD;
  unsigned int npoints, nreach;
  vector<double> plats, plons;
  vector<int> reaches;
};

// Return a int vector 'outVec' with unique values from 'inVec'
int intUnique(vector<int>& inVec, vector<int>& outVec);

// Return a double vector 'outVec' with unique values from 'inVec'
int doubleUnique(vector<double>& inVec, vector<double>& outVec);

// Read the RBM output *.Spat file
int readSpatFile(string& indir, string& studyCase, vector<int>& reach_ind, vector<int>& cell_ind, vector<int>& row, vector<int>& col, vector<double>& lat, vector<double>& lon, vector<double>& delta_ind);

// Return the number of days of a year
int ndaysYear(int year);

// Convert Julian day to month and day in gregorian calendar given the julian day and the year
int Julian2MonthDay(yearMonthDay& calDate);

// Convert decimal year into gregorian calendar date. It also includes the julian day within the year.
void decDate2CalDate(double decDate, yearMonthDay& calDate);

// Estimate averages for each month
int compMonthlyAve(string& indir, string& outdir, string& var,  string& studyCase, vector<int>& reach_ind, vector<int>& cell_ind, vector<int>& row, vector<int>& col, vector<int>& months);

// Estimate average for each year
int compYearlyAve(string& indir, string& outdir, string& var,  string& studyCase, vector<int>& reach_ind, vector<int>& cell_ind, vector<int>& row, vector<int>& col, vector<int>& years);

// Get completed months and years
int getMonthsYears(string& indir, string& studyCase, vector<int>& months, vector<int>& years);

// Write 2D array of type double
int writeOutReal2Darray(string& outdir, string& filename, double *mat, unsigned int nrow, unsigned int ncol);

// Save Lat and Lon 2D arrays
int saveLatLonMat(string& outdir, vector<int>& row, vector<int>& col, vector<double>& lat, vector<double>& lon);

// Get the closest point based on X (Lon) and Y (Lat) coord.
int getTheClosestPoint(double plat, double plon, vector<double>& lat, vector<double>& lon);

// Extract time series at a cell with Lat Lon coord.
int extractTS(string& indir, string& outdir, string& var,  string& studyCase, vector<int>& reach_ind, vector<int>& cell_ind, vector<double>& lat, vector<double>& lon, vector<double>& delta_ind, yearMonthDay& startD, yearMonthDay& endD, double plat, double plon, int pointID);

// Read RBMposPro.in file
void readRBMposPro(string& filename, RBMposPro& RBMo);

// Extract daily grid fields
int extractField(string& indir, string& outdir, string& var,  string& studyCase, vector<int>& cell_ind, vector<int>& row, vector<int>& col, vector<double>& delta_ind, yearMonthDay& startD, yearMonthDay& endD);

// Count the number of occurrences of 'occ' in 'array'.
int nOccurencesInArray(vector<int>& array, int occ);

// Extract daily values along reach
int extractReach(string& indir, string& outdir, string& var,  string& studyCase, vector<int>& reach_ind, vector<double>& lat, vector<double>& lon, vector<double>& delta_ind, yearMonthDay& startD, yearMonthDay& endD, int reach);


#endif // ADD_HEADER
