#include "Utils.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

using namespace std;


bool import_data(const string& file_path, double& S, size_t& n, double*& w, double*& r)
{
    ifstream ifile(file_path);
    if(ifile.fail()) return false;

    /*I read the 1st and 2nd lines to extract S and n*/
    char c;
    ifile>>c;
    ifile>>c;
    ifile>>S;
    
    ifile>>c;
    ifile>>c;
    ifile>>n;

    ifile>>c; /*I read also the endline char*/

    /*I discard the line with the header of fractions and rates in a temporary string*/
    string tmp;
    getline(ifile, tmp);
    

    /*I read the data and I save them in the dynamic arrays w, r*/
    w = new double[n];
    r = new double[n];

    string line;
    unsigned int i=0; //counter for vectors w,r
    while(getline(ifile, line))
    {
        stringstream ss_line(line);
        string data;
        double val;
        unsigned int j=0; //counter to understand which is the 1st and 2nd value
        
        while(getline(ss_line, data, ';'))
        {   
            val = stod(data);
            if(j==0) w[i]=val;
            else r[i]=val;
            j++;
        }

        i++;
    }

    return true;
}


double final_value(const double& S, const size_t n, const double* const& w, const double* const& r)
{
    double V=0;
    for(unsigned int i=0; i<n; i++)
    {
        V+=(1+r[i])*(S*w[i]);
    }

    return V;
}


double total_rate(const double& S, const double& V)
{
    double R=(V/S)-1;
    return R;
}


string summarize_result(const double& S, const size_t& n, const double* const& w, const double* const& r, const double& R, const double& V)
{   
    stringstream ss_output; /*type stringstream that will contain the whole output required*/

    stringstream ss_S, ss_V;
    ss_S<<fixed<<setprecision(2)<<S;
    ss_V<<fixed<<setprecision(2)<<V;
    
    ss_output<<"S = "<<ss_S.str()<<", "<<"n = "<<n<<endl;

    ss_output<<"w = [ ";
    for(unsigned int i=0; i<n; i++){
        ss_output<<w[i]<<' ';
    }
    ss_output<<']'<<endl;

    ss_output<<"r = [ ";
    for(unsigned int i=0; i<n; i++){
        ss_output<<r[i]<<' ';
    }
    ss_output<<']'<<endl;

    ss_output<<"Rate of return of the portfolio: "<<R<<endl;

    ss_output<<"V: "<<ss_V.str()<<endl;

    return ss_output.str();
}


bool export_data(const string& ofile_path, const string& out_str)
{
    ofstream ofile(ofile_path);
    ofile<<out_str;
    ofile.close();

    if(ofile.fail()) return false;

    return true;
}