#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "Utils.hpp"

using namespace std;

int main()
{
    string input_file = "data.txt";
    double S;
    size_t n;
    double *ptr_w = nullptr;
    double *ptr_r = nullptr;

    if(!import_data(input_file, S, n, ptr_w, ptr_r))
    {
        cerr<<"Error: failed to open the file "<<input_file<<endl;
        return 1;
    }

    double V = final_value(S, n, ptr_w, ptr_r); // Calcola il valore finale del portafoglio
    double R = total_rate(S, V); // Calcola il tasso totale finale dopo l'investimento

    /* Crea una stringa di riepilogo con l'intero output */
    string output_str = summarize_result(S, n, ptr_w, ptr_r, R, V);
    cout<<output_str<<endl; 

    /* Dealloca gli array dinamici utilizzati nel programma */
    delete [] ptr_w;
    delete [] ptr_r;

    /* Esporta i risultati in un file */
    string output_file = "result.txt";
    if(!export_data(output_file, output_str))
    {
        cerr<<"Error: falied to export data to the file "<<output_file<<endl;
        return 2;
    }

    return 0;
}
