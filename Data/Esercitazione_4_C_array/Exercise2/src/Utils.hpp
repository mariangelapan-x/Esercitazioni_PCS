#pragma once
#include <iostream>

using namespace std;


/* import_data: read the input data for financial assets from file
ifile_path: path of the input file
S: total investment
n: number of assets
w: vector that contains the fractions of S that we invest in the i-th assets
r: vector that contains the rates of return related to the i-th asset */
bool import_data(const string& ifile_path, double& S, size_t& n, double*& w, double*& r);


/* total_rate: compute the total final rate of return
S: total investment
V: final value of the wallet */
double total_rate(const double& S, const double& V);


/* final_value: compute the final value of our wallet
S: total investment
n: number of assets
w: vector that contains the fractions of S that we invest in the i-th assets
r: vector that contains the rates of return related to the i-th asset */
double final_value(const double& S, const size_t n, const double* const& w, const double* const& r);


/* export_data: write the results of the whole process in a file
ofile_path: path of the output file
out_str: the whole output string make with the function summarize_result */
bool export_data(const string& ofile_path, const string& out_str);


/*summarize_result: make a string that contains the whole output, in the required export format
S: total investment
n: number of assets
w: vector that contains the fractions of S that we invest in the i-th assets
r: vector that contains the rates of return related to the i-th asset
R: total rate of return 
V: final value of wallet */
string summarize_result(const double& S, const size_t& n, const double* const& w, const double* const& r, const double& R, const double& V);