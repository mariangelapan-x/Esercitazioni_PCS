#pragma once

#include <iostream>
#include <vector>
#include <algorithm> /*To use the heap structure of the STL*/

using namespace std;

namespace SortLibrary {

template<typename T>
concept Sortable = requires(T& t) {
    {t < t} -> std::convertible_to<bool>;
};

template<Sortable T>
void BubbleSort(vector<T>& v)
{
    const unsigned int n = v.size();
    for(size_t i = 0; i <= n-2; i++)
    {
        for(size_t j = i+1; j <= n-1; j++)
        {
            if(v[j] < v[i])
            {
                //swap:
                const T tmp = v[i];
                v[i]=v[j];
                v[j]=tmp;
            } 
        }
    }
}



template<Sortable T>
void HeapSort(vector<T>& v)
{
    const unsigned int n = v.size();

    make_heap(v.begin(), v.end()); /*To transform the vector v in a heap*/

    for(size_t j=0; j<n; j++)
        //Dequeue:
        pop_heap(v.begin(), v.end() - j);               
}



template<Sortable T>
void HeapSortCustom(vector<T>& v)
{   
    unsigned int n = v.size();

    while(n)
    {
        //make_heap of dimension n
        for(size_t i=1; i<n; i++)
        {
            unsigned int p = (i-1)/2; //I compute the index of the father node related to i (i is the sx or dx son of p)

            while(v[p] < v[i])//To compare each node up to the root
            {
                //swap:
                const T tmp = v[i];
                v[i] = v[p];
                v[p] = tmp;

                if(p!=0)
                {
                    i=p;
                    p=(i-1)/2; 
                }                
            }
        }
        
        //Dequeue: swap the root and the last leaf
        T tmp = v[n-1];
        v[n-1] = v[0];
        v[0] = tmp;

        n--;
    }
}

}