#include "Utils.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include "Eigen/Eigen"
#include <string>
#include <list>
#include <map>
#include <vector>
#include <set>


namespace PolygonalLibrary{

bool import_mesh(Polygonal_Mesh& mesh)
{
    if(!import_cell0Ds(mesh)) return false;

    if(!import_cell1Ds(mesh)) return false;

    if(!import_cell2Ds(mesh)) return false;

    return true;
}

bool import_cell0Ds(Polygonal_Mesh& mesh)
{
    ifstream ifile("./Cell0Ds.csv");

    if(ifile.fail()) return false;

    list<string> list_lines;

    string line;
    while(getline(ifile, line)) list_lines.push_back(line);

    ifile.close();

    /*I remove the header of the file
    Id;Marker;X;Y */
    list_lines.pop_front();

    mesh.num_cell0Ds = list_lines.size();

    if (mesh.num_cell0Ds == 0)
    {
        cerr<<"There is no cell 0D"<<endl;
        return false;
    }

    mesh.cell0Ds_id.reserve(mesh.num_cell0Ds);
    mesh.cell0Ds_coordinates = Eigen::MatrixXd::Zero(3,mesh.num_cell0Ds);

    for(const string& line : list_lines)
    {
        istringstream is_line(line);

        unsigned int id;
        unsigned int marker;
        char delimiter; //to storage the ';' of the csv

        is_line>>id>>delimiter>>marker>>delimiter>>mesh.cell0Ds_coordinates(0, id)>>delimiter>>mesh.cell0Ds_coordinates(1, id);

        mesh.cell0Ds_id.push_back(id);

        map<unsigned int, list<unsigned int>>& m = mesh.cell0Ds_markers;
        if(marker != 0)
        {
            auto [itor, bool_val] = m.try_emplace(marker);
            itor -> second.push_back(id);   
        }
        
    }
    
    return true;
}


bool import_cell1Ds(Polygonal_Mesh& mesh)
{
    ifstream ifile("./Cell1Ds.csv");

    if(ifile.fail()) return false;

    list<string> list_lines;

    string line;
    while(getline(ifile, line)) list_lines.push_back(line);

    ifile.close();

    mesh.num_cell1Ds = list_lines.size();

    /*I delete the header of the file
    Id;Marker;Origin;End */
    list_lines.pop_front();

    if (mesh.num_cell1Ds == 0)
    {
        cerr<<"There is no cell 1D"<<endl;
        return false;
    }

    mesh.cell1Ds_id.reserve(mesh.num_cell1Ds);
    mesh.cell1Ds_extrema = Eigen::MatrixXi::Zero(2, mesh.num_cell1Ds);

    for(const string& line : list_lines)
    {
        istringstream is_line(line);

        unsigned int id;
        unsigned int marker;
        char delimiter;

        is_line>>id>>delimiter>>marker>>delimiter>>mesh.cell1Ds_extrema(0, id)>>delimiter>>mesh.cell1Ds_extrema(1, id);
        mesh.cell1Ds_id.push_back(id);

        map<unsigned int, list<unsigned int>>& m = mesh.cell1Ds_markers;
        if(marker !=0)
        {
            auto [itor, bool_val] = m.try_emplace(marker);
            itor -> second.push_back(id);   
        }

        /*TEST2 to verify if a edge has a zero-length*/
        int& origin = mesh.cell1Ds_extrema(0, id);
        int& end = mesh.cell1Ds_extrema(1, id);
        if(origin == end)
        {
            cerr<<"TEST NOT PASSED: the edge "<<id<<" has length equal to zero";
            return false;
        }
        
    }

    return true;
}

bool import_cell2Ds(Polygonal_Mesh& mesh)
{
    ifstream ifile("./Cell2Ds.csv");

    if(ifile.fail()) return false;

    list<string> list_lines;

    string line;
    while(getline(ifile, line)) list_lines.push_back(line);

    /*I discard the header
    Id;Marker;NumVertices;Vertices;NumEdges;Edges*/
    list_lines.pop_front();

    mesh.num_cell2Ds = list_lines.size();

    if(mesh.num_cell2Ds == 0)
    {
        cerr<<"There is no cell 2D"<<endl;
        return false;
    }

    mesh.cell2Ds_id.reserve(mesh.num_cell2Ds);
    mesh.cell2Ds_vertices.reserve(mesh.num_cell2Ds);
    mesh.cell2Ds_edges.reserve(mesh.num_cell2Ds);

    for(const string& line : list_lines)
    {
        istringstream is_line(line);

        unsigned int id;
        unsigned int marker;
        unsigned int num_vert;
        unsigned int num_edges;
        char delimiter;

        is_line>>id>>delimiter>>marker>>delimiter>>num_vert;

        vector<unsigned int> vec_v;
        vec_v.reserve(num_vert);
        for(unsigned int i=0; i<num_vert; i++)
        {  
            unsigned int vert;
            is_line>>delimiter>>vert;
            vec_v.push_back(vert);
        }
        mesh.cell2Ds_vertices.push_back(vec_v);


        is_line>>delimiter>>num_edges;

        vector<unsigned int> vec_e;
        vec_e.reserve(num_edges);
        for(unsigned int j=0; j<num_edges; j++)
        {
            unsigned int edge;
            is_line>>delimiter>>edge;
            vec_e.push_back(edge);
        }
        mesh.cell2Ds_edges.push_back(vec_e);

        mesh.cell2Ds_id.push_back(id);

        map<unsigned int, list<unsigned int>>& m = mesh.cell2Ds_markers;
        if(marker != 0)
        {
            auto [itor, bool_val] = m.try_emplace(marker);
            itor -> second.push_back(id);   
        }

        /*TEST3 to verify that every polygon has a non zero-area*/
        vector<unsigned int>& vec_vert = mesh.cell2Ds_vertices[id];
        const unsigned int n = vec_vert.size();

        double area = 0.0;
        for(size_t i=0; i<n; i++)
        {   
            const unsigned int vi_id = vec_vert[i];
            const unsigned int vj_id = vec_vert[(i+1)%n]; //To close the polygon and connect the first vertice and the last vertice I use j=(i+1)%n

            const MatrixXd coord = mesh.cell0Ds_coordinates;
            const double X_vi = coord(0, vi_id);
            const double Y_vi = coord(1, vi_id);
            const double X_vj = coord(0, vj_id);
            const double Y_vj = coord(1, vj_id);

            area += (X_vi * Y_vj) - (X_vj * Y_vi);
        }
        area = abs(area / 2.0);

        if(area <= 1e-16)
        {
            cerr<<"TEST NOT PASSED: the polygon "<<id<<" has area equal to zero";
            return false;
        }

    }

    return true;
}

}