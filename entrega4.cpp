//Este programa es la cuarta parte del reto de la materia, para el cual implementamos maps y BST
//Trabajo colaborativo por:
//Lourdes Badillo, A01024232
//Eduardo Villalpando, A01023646
//9/11/2020

#include <iostream>
#include <vector>
#include <map>
#include "reader.hpp"
#include "ConexionesComputadora.hpp"
#include <set>
#include <cwctype> 
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


void agregarNoRetoASet(set<string> s, vector<Registro> datos){
    for(int i=0; i<5000; i++){
        if(datos[i].fuente_hostname.find(".reto.com") || datos[i].fuente_hostname.find("-") ){
            s.insert(datos[i].fuente_hostname); 
        }
    }
}

void llenarComputadoras(map<string, ConexionesComputadora> &computadoras, vector<Registro> datos){
    // throwback a nuestro debugging de una hora, porque no estábamos editando el map original :)
    for(int i=0; i<6000; i++){ //Cambiar a datos.size()
        ConexionesComputadora c(datos[i].fuente_ip, datos[i].fuente_hostname);
        c.llenar(datos);
        computadoras.insert(pair<string,ConexionesComputadora>(datos[i].fuente_ip,c));
    }
}

void agregarNoRetoASetyLlenarComputadoras(set<string> s, map<string, ConexionesComputadora> &computadoras, vector<Registro> datos){
    for(size_t i=0; i<datos.size(); i++){
        if(datos[i].fuente_hostname.find(".reto.com") || datos[i].fuente_hostname.find("-") ){
            s.insert(datos[i].fuente_hostname); 
        }
        // Correr si la IP origen no está en el map
        if( datos[i].fuente_ip != "-" && computadoras.find(datos[i].fuente_ip) == computadoras.end() ){
            ConexionesComputadora c(datos[i].fuente_ip, datos[i].fuente_hostname);
            c.llenar(datos);
            computadoras.insert(pair<string,ConexionesComputadora>(datos[i].fuente_ip,c));
        }
        // Correr si la IP destino no está en el map
        if( datos[i].destino_ip != "-" && computadoras.find(datos[i].destino_ip) == computadoras.end()){
            ConexionesComputadora c(datos[i].destino_ip, datos[i].destino_hostname);
            c.llenar(datos);
            computadoras.insert(pair<string,ConexionesComputadora>(datos[i].destino_ip,c));
        }
    }
}

bool esAnomalo(string nombre){
    // largo
    // caracteres alfanumericos
    if (nombre.size() >= 20) return true;
    for(int i=0; i<nombre.size(); i++){
        if(isalpha(nombre[i]) == false && nombre[i] != '.' && nombre[i] != '-'&&  nombre[i] != '/' ){
            return true;  
        }
    }
    return false; 
}

string encontrarAnomalos(map<string, ConexionesComputadora> computadoras){
    map<string, ConexionesComputadora>::iterator it;
    for(it = computadoras.begin(); it != computadoras.end(); it++){
        if( esAnomalo( it->second.nombre ) ) return it->first;
    }
    return "";
}


int computadorasConConexionesEntrantes(map<string, ConexionesComputadora> computadoras){ 
    int n = 0;
    map<string, ConexionesComputadora>::iterator it;
    for(it = computadoras.begin(); it != computadoras.end(); it++ ){
        if(it->second.nombre.find("reto.com") != string::npos && it->second.conexionesEntrantes.size() >= 1) n++;
    }
    return n;
} 

set<string> obtenerIPsEntrantes(map<string, ConexionesComputadora> computadoras) {
    /**
     * - for computadora in computadoras
     * -    si computadora.IP != server.reto.com
     *          iterar por conexiones entrantes
     *              si conexionEntrante.puerto != 67
     *                  agregar IP al set (para que no se repita :))
     * regresar set
     * Los mensajes DHCP utilizan el puerto 67 (UDP) como puerto del servidor
    */
    set<string> ipsUnicas;

    map<string, ConexionesComputadora>::iterator it;
    int n = 0;
    for(it = computadoras.begin(); it != computadoras.end(); it++ ){
        if( it->second.nombre.find(".reto.com") == string::npos /*no lo encontró*/ ){
            n++;
            // Convertir a vector para poder acceder los índices de manera más fácil ;D
            vector<Conexion> conexionesV{begin(it->second.conexionesEntrantes), end(it->second.conexionesEntrantes) };
            set<string> nombresPorComputadoraUnicos;
            for(int i=0; i< conexionesV.size(); i++ ){
                if ( conexionesV[i].puerto != 67 ){
                    nombresPorComputadoraUnicos.insert(conexionesV[i].host);
                    ipsUnicas.insert( conexionesV[i].IP );
                }
            }
        }
    }

   return ipsUnicas; 
}

/*
conexionesPorDia
Recibe una fecha 
Regresa un map<string, int>
Conteniendo la cantidad de conexiones entrantes a cada sitio/página que no sea "-" 
y que no pertenezca al dominio "reto.com" del día especificado por la fecha de entrada. 
*/
map<string, int> conexionesPorDia(tm date, vector<Registro> datos){
    map<string, int> conexiones; 
    for (int i = 0; i < datos.size(); i++ ){
        if(datos[i].fecha.tm_mday == date.tm_mday && datos[i].fecha.tm_mon == date.tm_mon && datos[i].fecha.tm_year == date.tm_year ){
            if(!datos[i].fuente_hostname.find(".reto.com") && !datos[i].fuente_hostname.find("-") ){
                conexiones[datos[i].fuente_hostname]++; 
            }
        }
    }
}

/*
Contenga una función llamada top, la cual recibe un parámetro n de tipo int 
y una fecha. Esta función debe imprimir los n sitios con más accesos en esa fecha. 
Para ello, puedes usar la función conexionesPorDia y debes agregar los sitios a un 
BST utilizando como parámetro de ordenamiento la cantidad de conexiones entrantes.
*/
void top(int n, tm date){

}


int main(void){
    Reader r; 
    vector <Registro> datos = r.readFile(); 
    map<string, ConexionesComputadora> computadoras;
    
    cout << "1. ¿Existe algún sitio que se mantenga en el top 5 en todos los días?" << endl;
    cout << "2. ¿Existe algún sitio que entre al top 5 a partir de un día y de ahí aparezca en todos los días subsecuentes?" << endl;
    cout << "3. ¿Existe algún sitio que aparezca en el top 5 con una cantidad más alta de tráfico que lo normal?" << endl;

    return 0;
}
