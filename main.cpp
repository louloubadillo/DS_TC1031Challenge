#include <iostream>
#include <vector>
#include "reader.hpp"
using namespace std;

void print_vector(vector<Registro> arr){
    for (int i = 0; i < arr.size(); i++){
        arr[i].print();
    }
    cout << endl;
};

int searchDataDate(vector<Registro> datos, tm f){
    int counter = 0; 
    for(int i = 0; i<datos.size(); i++){
        if(
            datos[i].fecha.tm_mday == f.tm_mday &&
            datos[i].fecha.tm_mon == f.tm_mon &&
            datos[i].fecha.tm_year == f.tm_year
        ){
            counter++;  
        }
    }
    return counter; 
};

string obtenerIPBase(vector<Registro> d){
    //XXX.XXX.X.XX
    int i = 0;
    while (d[0].fuente_ip != "-")
    {
        i++;
    }
    string ip = d[i+1].fuente_ip;
    for(int i = 0; i<3; i++){
        ip.pop_back();
    }
    ip.append(".0");
    return ip;
}

int main(void){
    Reader r; 
    vector <Registro> datos = r.readFile(); 
    //1. ¿Cuántos registros tiene tu archivo?
    cout<<"El archivo tiene "<<datos.size()<<" registros"<<endl;

    //2.¿Cuántos récords hay del segundo día registrado? ¿Qué día es este?
    int primerDiaCount = searchDataDate(datos, datos[0].fecha);
    int segundoDiaCount = searchDataDate(datos, datos[segundoDiaCount].fecha);
    
    cout<<"El primer día...."<<primerDiaCount<<endl;
    cout<<"En el segundo día (";
    datos[segundoDiaCount].printDate();
    cout<<") hubieron "<<segundoDiaCount<<" datos"<<endl;

    //3. ¿Alguna de las computadoras pertenece a Jeffrey, Betty, Katherine, Scott, Benjamin, Samuel o Raymond?


    //5. ¿Cuál es la dirección de la red interna de la compañía?
    cout<<"La dirección de la red interna es: "<<obtenerIPBase(datos)<<endl;
    
    return 0;
}
