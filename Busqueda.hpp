#ifndef Busqueda_h
#define Busqueda_h
#include <vector>
using namespace std;

<template class T>
class Busqueda{
    private:
        vector<T> datos;
    public:
        Busqueda(vector<T> d);
        ~Busqueda();
        int buscSecuencial (<T> e){
        for(int i = 0; i<datos.size(); i++){
            if(datos[i] == e){
                return i; 
            }
        }
        return -1; 

    int binarySearchRec(vector<int> arr, int element, int min, int max){
            int middle = (min + max)/2; 
            if(max < min){
                return -1; 
            }
            if(element == arr[middle]){
                return middle; 
            }
            else if (element < arr[middle]){
                return binarySearchRec(arr,element, min, middle-1); 
            }
            else{
                return binarySearchRec( arr, element, middle+1, max); 
            }
        } 

        int busqBinaria(vector<int> arr, int element){
            return binarySearchRec( arr, element, 0, elements.size()); 
        }

        Busqueda(vector<Registro> d){
             this->datos = d;
        }

        Busqueda::~Busqueda(){

        }
};



#endif