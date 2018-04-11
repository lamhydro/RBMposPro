# RBMposPro
RBMposPro is a C++ software to extract time series at individual cells, time series of grid domain, time series of individual reaches and to estimate multi-annual averages and monthly averages from [RBM] (http://www.hydro.washington.edu/Lettenmaier/Models/RBM/) .Temp and and .Spat output files.  The model extract information for each of the variables recorded in .Temp file. RBMposPro output files can be easily plotted for posterior analysis.

## Project structure:
- /lib: Contains RBMtools.hpp and RBMtools.cpp.
- /src: Contains main.cpp. This file calls files in /lib
- /plot: Cotains Python scripts to plot output files.
- /test: Contains an exaple of RBMposPro.in
- makefile: Make file to compile/clean code.

## To compile:
- `make`

## To run in Linux systems:
- e.g. `./RBMposPro test/RBMposPro.in`

## INPUT data
RBM read the file RBMposPro.in, which contain:
- Line 1: Comment
- Line 2: RBM study case directory. It must contain RBM output files.
- Line 3: Comment
- Line 4: Study case name
- Line 5: Comment
- Line 6: Directory where RBMposPro output is saved.
- Line 7: Comment
- Line 8: Variable name to be extracted. It can be:[T_0, T_head, dbt, Q_in]
- Line 9: Comment
- Line 10: Extract time series at cells? if =1 extract them if not it does not.
- Line 11: Comment
- Line 12: Extract time series of fields? if =1 extract them if not it does not.
- Line 13: Comment
- Line 14: Extract time series of reaches? if =1 extract them if not it does not.
- Line 15: Comment
- Line 16: Starting date (YYYY MM DD) to begin the extraction.
- Line 17: Comment
- Line 18: End date (YYYY MM DD) to terminate the extraction.
- Line 19: Comment
- Line 20: Number of grid cells to be extracted
- Line   : Latitude longitude
- Line   : .
- Line   : .
- Line   : .
- Line n : Latitude longitude
- Line   : Comment
- Line   : Number of reaches where to extract information
- Line   : Comment
- Line   : Index of reach (see column 'reach_ind' in .Spat file)
- Line   : .
- Line   : .
- Line   : .
- Line   : Index of reach (see column 'reach_ind' in .Spat file)


## OUTPUT DATA
The model produce the following type of files:
- lat.csv: Matrix of latitudes of the grid cells
- lon.csv: Matrix of longitudes of the grid cells
- T_0_MONTH.csv: Matrix with monthly averages for month MONTH. Averages are estimated for completed months.
- T_0_pointN.csv: Daily time series (DATE VALUE) of the chosen variable at point N. N correspond to the row of LAT LON pair, so for example T_0_point1.csv contains the time series of the first LAT LON pair, T_0_point2.csv of the second pair and so on.
- T_0_fields.csv: Daily matrices of the chosen variable.
- T_0_YEAR.csv: Annual averages of the chosen variable for the year YEAR. This is only estimated for completed years.
