//Este programa es la quinta parte del reto de la materia, para el cual implementamos grafos
//Trabajo colaborativo por:
//Lourdes Badillo, A01024232
//Eduardo Villalpando, A01023646
//22/11/2020

#include <iostream>
#include <vector>
#include <map>
#include "reader.hpp"
#include "ConexionesComputadora.hpp"
#include <set>
#include <cwctype> 
#include <ctime>
#include <string>
#include "BinarySearchTree.h"
#include "Graph.h"
using namespace std;

class Date{
    public:
        tm date;
        Date( tm date ){
            this->date = date;
            // this->date.tm_mday = date.tm_mday;
            // this->date.tm_mon = date.tm_mon; 
            // this->date.tm_year = date.tm_year; 
        }

        friend bool operator == (Date d, Date dd){
            return (
                d.date.tm_mday == dd.date.tm_mday &&
                d.date.tm_mon == dd.date.tm_mon &&
                d.date.tm_year == dd.date.tm_year
            );
        }

        friend bool operator != (Date d, Date dd){
            return (
                d.date.tm_mday != dd.date.tm_mday ||
                d.date.tm_mon != dd.date.tm_mon ||
                d.date.tm_year != dd.date.tm_year
            );
        }

        friend bool operator < (Date d, Date dd){
            return (
                d.date.tm_mday < dd.date.tm_mday ||
                d.date.tm_mon < dd.date.tm_mon ||
                d.date.tm_year < dd.date.tm_year
            );
        }
        

        string toString(){
            return to_string(this->date.tm_mday) + "/" + to_string(this->date.tm_mon+1) + "/" + to_string(this->date.tm_year+1900);
        }

};

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

bool esInterna(string IPInput){
    string IPBase = "172.26.89.0";
    // 172.26.89.0 == 172.26.89.142
    for(int i=0; i<10; i++){
        if( IPBase[i] != IPInput[i] ) return false;
    }
    return true; 
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
     *          iterar por numeroDeOcurrencias entrantes
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
Conteniendo la cantidad de numeroDeOcurrencias entrantes a cada sitio/página que no sea "-" 
y que no pertenezca al dominio "reto.com" del día especificado por la fecha de entrada. 
*/
map<string, int> conexionesPorDia(tm date, vector<Registro> datos){
    map<string, int> numeroDeOcurrencias; 
    for (int i = 0; i < datos.size(); i++){
        if(datos[i].fecha.tm_mday == date.tm_mday && datos[i].fecha.tm_mon == date.tm_mon && datos[i].fecha.tm_year == date.tm_year){ 
            if(datos[i].destino_hostname.find(".reto.com") == string::npos && datos[i].destino_hostname.find("-")== string::npos){
                numeroDeOcurrencias[datos[i].destino_hostname]++; 
            }
        }
    }
    return numeroDeOcurrencias; 
}
// Imprime un mapa
void printMap(map<string, int> numeroDeOcurrencias){
    map<string, int>::iterator it;
    for(it = numeroDeOcurrencias.begin(); it != numeroDeOcurrencias.end(); it++ ){
        cout<< it->first <<":\t" << it->second <<endl;
    }
}

/*
Contenga una función llamada top, la cual recibe un parámetro n de tipo int 
y una fecha. Esta función debe imprimir los n sitios con más accesos en esa fecha. 
Para ello, puedes usar la función conexionesPorDia y debes agregar los sitios a un 
BST utilizando como parámetro de ordenamiento la cantidad de numeroDeOcurrencias entrantes.
*/
void top(
    BinarySearchTree &tree, int n, tm date, map<string, int> &numeroDeOcurrencias, map <string, int> &promedioDiario, vector<Registro> datos){
    map<string, int> conexionesDia = conexionesPorDia(date, datos); 
    map<string, int>::iterator it;
    for(it = conexionesDia.begin(); it != conexionesDia.end(); it++ ){
        tree.insertNode(it->first, it->second);
    }
    cout << endl << "El top " << n << " del día " << date.tm_mday << "/" << date.tm_mon+1 << "/" << date.tm_year+1900 << " es:" << endl;
    tree.printKth(n); 

    map<string, int> conexionesTemp;
    tree.saveKth(n, conexionesTemp);

    for(map<string, int>::iterator i = conexionesTemp.begin(); i!=conexionesTemp.end(); ++i){
        if(i->second > 1){ i->second = 1; }
        i->second += numeroDeOcurrencias[i->first];
    }
    tree.saveKth(n, promedioDiario);
    for(map<string, int>::iterator i = promedioDiario.begin(); i!=promedioDiario.end(); ++i){
        i->second += conexionesTemp[i->first];        
    }

    numeroDeOcurrencias = conexionesTemp;
}

set<Date> obtenerFechas(vector<Registro> datos){
    set<Date> todasLasFechas;
    for(int i = 0; i<datos.size(); i++){
        Date d(datos[i].fecha);
            todasLasFechas.insert( d );
    }
    return todasLasFechas;
}


void llenarComputadoras(map<string, ConexionesComputadora> &computadoras, vector<Registro> datos){
    for(size_t i=0; i<datos.size(); i++){
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

//Función 
void conexionesDiariasEnGrafos( map<Date, Graph<string>> &grafosPorDia, set<Date> todasLasFechas, string IP_A, map<Date, int> &conexionesSalientesPorDia, map<string, ConexionesComputadora> todasLasComputadoras){
     // Iterar por cada día y hacer grafo
    for (set<Date>::iterator it = todasLasFechas.begin(); it != todasLasFechas.end(); ++it){
        Graph<string> grafo_i;
        // Iterar todas las computadoras para añadir nodos
        for(map<string, ConexionesComputadora>::iterator itt = todasLasComputadoras.begin(); itt != todasLasComputadoras.end(); ++itt){
            // Si es interna, añadir al grafo
            if(esInterna(itt->first)){
                grafo_i.add_node(itt->first);
            }
        }
        // Iterar todas las computadoras para añadir edges
        for(map<string, ConexionesComputadora>::iterator itt = todasLasComputadoras.begin(); itt != todasLasComputadoras.end(); ++itt){
            string i_ip = itt->first;
            list<Conexion> i_conexionesSalientes = itt->second.conexionesSalientes;
            // Iterar todas las conexiones salientes
            for(list<Conexion>::iterator ittt = i_conexionesSalientes.begin(); ittt != i_conexionesSalientes.end(); ++ittt){
                // Si es interna, agregar conexion
                if( esInterna(ittt->IP) ){
                    grafo_i.add_edge_element(i_ip, ittt->IP);
                    // Si destino  == IP_A, entonces guardar en map<Date, int>
                    if(ittt->IP == IP_A){
                        conexionesSalientesPorDia[*it]++;
                    }
                    
                }
            }
        }
        grafosPorDia[*it] = grafo_i;
    }
 }

 //Obtener IP con más conexiones por día
 map<Date, string> maxConexionesPorDia(map<Date, Graph<string>> grafosPorDia){
    map<Date, string> maxConexionesPorDia; 
    for(map<Date, Graph<string>>::iterator it = grafosPorDia.begin(); it != grafosPorDia.end(); ++it){
        Graph<string> grafo_hoy = it->second;
        
        map<string, int> numeroDeConexiones = grafo_hoy.saveNeigbhors();
        string ipConMasConexiones = "";
        int maxConexiones = 0;
        for(map<string, int>::iterator itt = numeroDeConexiones.begin(); itt != numeroDeConexiones.end(); ++itt){
            if(itt->second >= maxConexiones){
                ipConMasConexiones = itt->first;
                maxConexiones = itt->second;
            }
        }
        maxConexionesPorDia[it->first] = ipConMasConexiones;
    }
    return maxConexionesPorDia; 
}

bool esVerticeConMasConexionesSalientes(map<Date, string> maxConexionesPorDia, string ipA){
    bool mayorConexionesSalientes; 
    for(map<Date, string>::iterator it = maxConexionesPorDia.begin(); it != maxConexionesPorDia.end(); ++it){
        if(it->second != ipA){
            return false;
        }
    }
    cout << "El vértice con la IP: " << ipA << " tiene la mayor cantidad de conexiones salientes hacia la red interna."<<endl;
    return true;  
}



int main(void){
    Reader r; 
    vector <Registro> datos = r.readFile(); 
    map<string, ConexionesComputadora> todasLasComputadoras;
    llenarComputadoras(todasLasComputadoras, datos);

    // Una ip interna, que llamaremos A, la cual se comunica con algunas otras computadoras internas.
    string IP_INTERNA_A = "172.26.89.118"; //Jennifer
    ConexionesComputadora A = todasLasComputadoras[IP_INTERNA_A];

    // Sitio con nombre anómalo
    string IP_ANOMALA_B = encontrarAnomalos(todasLasComputadoras); 
    ConexionesComputadora B = todasLasComputadoras[IP_ANOMALA_B];

    // Sitio con mucho tráfico en un día
    string IP_TRAFICO_C = "64.158.210.175"; //Groupon que tuvo más de 400 visitas en un día
    ConexionesComputadora C = todasLasComputadoras[IP_TRAFICO_C];

    /* 1. Utilizando un grafo con las conexiones entre las ip de la red interna, 
    determina la cantidad de computadoras con las que se ha conectado A por día. 
    ¿Es A el vértice que más conexiones salientes hacia la red interna tiene? */
    map<Date, Graph<string>> grafosPorDia;
    set<Date> todasLasFechas = obtenerFechas(datos);
    map<Date, int> conexionesSalientesPorDia;

    conexionesDiariasEnGrafos(grafosPorDia, todasLasFechas, IP_INTERNA_A, conexionesSalientesPorDia, todasLasComputadoras); 
    
    cout<<"1. Utilizando un grafo con las conexiones entre las ip de la red interna, determina la cantidad de computadoras con las que se ha conectado A por día. ¿Es A el vértice que más conexiones salientes hacia la red interna tiene?"<<endl;
    map<Date, string> maxConexionesDia = maxConexionesPorDia(grafosPorDia); 
    esVerticeConMasConexionesSalientes(maxConexionesDia, IP_INTERNA_A);
    
    /* 2. Utilizando el grafo del punto anterior, ubica la cantidad de computadoras que se han conectado hacia A por día.
    ¿Existen conexiones de las demás computadoras hacia A? */

    
    return 0;
}
