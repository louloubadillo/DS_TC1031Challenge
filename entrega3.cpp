
//Este programa es la tercera parte del reto de la materia, para el cual implementamos conjuntos y diccionarios.
//Trabajo colaborativo por:
//Lourdes Badillo, A01024232
//Eduardo Villalpando, A01023646
//13/10/2020

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
    for(int i=0; i<datos.size(); i++){
        if(datos[i].fuente_hostname.find(".reto.com")){
            s.insert(datos[i].fuente_hostname); 
        }
    }
}

void llenarComputadoras(map<string, ConexionesComputadora> computadoras, vector<Registro> datos){
    for(int i=0; i<500; i++){ //Cambiar a datos.size()
        ConexionesComputadora c(datos[i].fuente_ip, datos[i].fuente_hostname);
        c.llenar(datos);
        computadoras.insert(pair<string,ConexionesComputadora>(datos[i].fuente_ip,c));
    }
}

bool esAnomalo(string nombre){
    // largo
    // caracteres alfanumericos
    if (nombre.size() >= 20) return true;
    for(int i=0; i<nombre.size(); i++){
        if(isalnum(nombre[i]) == false ){
            return true;  
        }
    }
    return false; 
}

string encontrarAnomalos(vector <Registro> datos){
    for(int i=0; i<datos.size(); i++){
        if( esAnomalo(datos[i].destino_hostname) ) return datos[i].destino_hostname;
    }
    return "";
}

int computadorasConConexionesEntrantes(map<string, ConexionesComputadora> computadoras){
    int n;
    for (const auto & [key, conexionComputadora] : computadoras){ // iterar el mapa <strng, ConexionesComputadora>
        if(conexionComputadora.IP.find("reto.com") && conexionComputadora.conexionesEntrantes.size() >= 1) n++;
    }
    return n;
}

int main(void){
    Reader r; 
    vector <Registro> datos = r.readFile(); 
    // Cree un conjunto de strings. En este debes agregar los nombres de los dominios/computadoras que no pertenezcan al dominio reto.com.
    set<string> notReto; 
    agregarNoRetoASet(notReto, datos);
    // Crea una diccionario de pares <string,ConexionesComputadora> usando la clase creada en el reto pasado. 
    // Este diccionario debe ser llenado con todas las ips en los datos. 
    // Por cada una, es necesario registrar todas las conexiones que hayan salido desde esta ip o hayan sido recibidas por la misma.
    map<string, ConexionesComputadora> computadoras; 
    llenarComputadoras(computadoras, datos);
    
    //Hay algún nombre de dominio que sea anómalo (Esto puede ser con inspección visual).
    cout << "1. ¿Hay algún nombre de dominio que sea anómalo?" << endl; //ds19smmrn47jp3osf6x4.com
    cout << (encontrarAnomalos(datos)!="" ? "Sí." : "No.") << endl; 
    
    // De los nombres de dominio encontrados en el paso anterior, ¿Cuál es su ip? 
    // ¿Cómo determinarías esta información de la manera más eficiente en complejidad temporal?
    cout << "2. ¿Cuál es su IP? ¿Cómo determinarías esta información de la manera más eficiente en complejidad temporal?" << endl;
    cout << "Con una inspección visual identificamos: ds19smmrn47jp3osf6x4.com" << endl; 
    cout << "Hicimos una función prototipo de cómo se podrían encontrar dominios anómalos: encontrarAnomalos(datos)" << endl; 
    cout << "Para esto usamos como parámetros el largo del dominio y si contiene caracteres no alfanuméricos" << endl; 
    cout << "Una implementación eficiente podría ser tener un algoritmo de machine learning, que vaya aprendiendo qué dominios son anómalos y los detecte en una complejidad O(1)" << endl;
    
    //De las computadoras pertenecientes al dominio reto.com determina la cantidad de ips que tienen al menos una conexión entrante. 
    //(Recuerda que ya tienes la dirección de la red y el último octeto puede tener computadoras del .1 al .254). 
    //Imprime la cantidad de computadoras.
    cout << "3. De las computadoras pertenecientes al dominio reto.com determina la cantidad de ips que tienen al menos una conexión entrante." << endl; 
    cout << computadorasConConexionesEntrantes(computadoras) << " computadoras de la red interna con al menos una conexion entrante" << endl;

    //Toma algunas computadoras que no sean server.reto.com o el servidor dhcp. Pueden ser entre 5 y 150. Obtén las ip únicas de las conexiones entrantes.
    cout << "Toma algunas computadoras que no sean server.reto.com o el servidor dhcp. Pueden ser entre 5 y 150. Obtén las ip únicas de las conexiones entrantes." << endl;
    
    
    //Considerando el resultado de las preguntas 3 y 4, ¿Qué crees que esté ocurriendo en esta red? (Pregunta sin código)
    //Para las ips encontradas en el paso anterior, determina si se han comunicado con los datos encontrados en la pregunta 1.
    //(Extra):  En caso de que hayas encontrado que las computadoras del paso 1 y 4 se comunican, determina en qué fecha ocurre la primera comunicación entre estas 2 y qué protocolo se usó.
    
    return 0;
}
