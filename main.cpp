#include <iostream>
#include <vector>
#include "reader.hpp"
using namespace std;

// Imprimir vectores
void print_vector(vector<Registro> arr){
    for (int i = 0; i < arr.size(); i++) arr[i].print();
    cout << endl;
};

// Búsqueda secuencial
int busquedaSecuencial( vector<Registro> d, bool (*condicion)(Registro r) ){
    for(int i = 0; i<d.size(); i++){
        if( condicion(d[i]) ) return i;
    }
    return -1;
}
// Busqueda secuencial (sobrecarga)
int busquedaSecuencial( vector<Registro> d, bool (*condicion)(Registro a, Registro b), Registro r ){
    for(int i = 0; i<d.size(); i++){
        if( condicion(d[i], r) ) return i;
    }
    return -1;
}

// BúsquedaBinaria
int busquedaBinaria(vector<Registro>d, bool (*condicion)(Registro r), int inicio, int final){
    // med
    // declarar variables simultáneas para izquiera y derecha, como en el ejercicio del rascacielos. WIP

    // int med = ...
    if(final<inicio) return -1;
    // if( condicion(datos[med]) ) return med;
    // izq = busquedaBinaria(...), der = busquedaBinaria(...)
    // return izq || derecha

    return 0;
}

// ============================================================================
bool esPosterior(Registro a, Registro b){
    return (
        a.fecha.tm_mday > b.fecha.tm_mday &&
        a.fecha.tm_mon >= b.fecha.tm_mon &&
        a.fecha.tm_year >= b.fecha.tm_year
    );
}

bool perteneceA(Registro r){
    return(
        r.fuente_hostname == "jeffrey.reto.com" ||
        r.fuente_hostname == "betty.reto.com" ||
        r.fuente_hostname == "katherine.reto.com" ||
        r.fuente_hostname == "scott.reto.com" ||
        r.fuente_hostname == "benjamin.reto.com" ||
        r.fuente_hostname == "samuel.reto.com" ||
        r.fuente_hostname == "raymond.reto.com"
    );
}

string obtenerIPBase(vector<Registro> d){
    int i = busquedaSecuencial(d, [](Registro r){ return r.fuente_ip != "-" });
    string ip = d[i+1].fuente_ip;
    for(int i = 0; i<3; i++) ip.pop_back();
    ip.append(".0");
    return ip;
}

int main(void){
    Reader r; 
    vector <Registro> datos = r.readFile(); 
    //1. ¿Cuántos registros tiene tu archivo?
    cout<<"1\t¿Cuántos registros tiene tu archivo?"<<endl
        <<"El archivo tiene "<<datos.size()<<" registros"<<endl<<endl;
    
    //2.¿Cuántos récords hay del segundo día registrado? ¿Qué día es este?
    cout<<"2\t ¿Cuántos récords hay del segundo día registrado? ¿Qué día es?"<<endl;
    int primerDiaCount = busquedaSecuencial(datos, *esPosterior, datos[0]);
    int segundoDiaCount = busquedaSecuencial(datos, *esPosterior, datos[primerDiaCount]) - primerDiaCount;
    
    cout<<"En el primer día (";
    datos[0].printDate();
    cout<<") hubieron "<<primerDiaCount<<" datos"<<endl;
    cout<<"En el segundo día (";
    datos[segundoDiaCount].printDate();
    cout<<") hubieron "<<segundoDiaCount<<" datos"<<endl<<endl;

    //3. ¿Alguna de las computadoras pertenece a Jeffrey, Betty, Katherine, Scott, Benjamin, Samuel o Raymond?
    cout<<"3\t¿Alguna de las computadoras pertenece a Jeffrey, Betty, Katherine, Scott, Benjamin, Samuel o Raymond? (IMPLEMENTAR BINARY SEARCH)"<<endl;
    int perteneceCount = busquedaBinaria(datos, *perteneceA, 0, datos.size()-1);
    cout<<( perteneceCount==0 ? "No." : "Si." )<<endl<<endl;

    //4. ¿Cuál es la dirección de la red interna de la compañía?
    // Sustituir para usar comparadores y búsqueda secuencial. DONE
    cout<<"4\t¿Cuál es la dirección de la red interna de la compañía?"<<endl;
    cout<<"La dirección de la red interna es: "<<obtenerIPBase(datos)<<endl;
    

    // Para las que pregunta qué mails usa y qué puertos son menores a 1000, en la función de comparación podemos hacer que los añada a un Set y luego imprimir sus valores.


    return 0;
}
