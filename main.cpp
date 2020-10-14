//Trabajo colaborativo 
//Lourdes Badillo, A01024232
//Eduardo Villalpando, A01023646

#include <iostream>
#include <vector>
#include <map>
#include "reader.hpp"
#include "ConexionesComputadora.hpp"
using namespace std;

map<string, int> mails;
map<int, int> puertos;

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
    int med = inicio + (final - inicio)/2;
    if(final<=inicio) return -1;
    if( condicion(d[med]) ) return med;
    int izq = busquedaBinaria(d, *condicion, inicio, med-1);
    if(izq!=-1) return izq; 
    int der = busquedaBinaria(d, *condicion, med+1, final);
    if(der!=-1) return der;
    return -1;
}

// ============================================================================
// bool esPosterior(Registro a, Registro b){
//     return (
//         a.fecha.tm_mday > b.fecha.tm_mday &&
//         a.fecha.tm_mon >= b.fecha.tm_mon &&
//         a.fecha.tm_year >= b.fecha.tm_year
//     );
// }

// bool perteneceA(Registro r){
//     return(
//         r.fuente_hostname == "jeffrey.reto.com" ||
//         r.fuente_hostname == "betty.reto.com" ||
//         r.fuente_hostname == "katherine.reto.com" ||
//         r.fuente_hostname == "scott.reto.com" ||
//         r.fuente_hostname == "benjamin.reto.com" ||
//         r.fuente_hostname == "samuel.reto.com" ||
//         r.fuente_hostname == "raymond.reto.com"
//     );
// }

// bool seLlamaServer(Registro r){
//     return r.fuente_hostname == "server.reto.com" || r.destino_hostname == "server.com";
// }

// bool esCorreo(Registro r){
//     //El puerto 993 se usa para mail
//     if(r.destino_puerto == 993){
//         mails[r.destino_hostname]++; 
//     }
//     return false; 
// }

// bool esPuerto(Registro r){
//     if(r.destino_puerto<1000 && r.destino_puerto>0){
//         // Añadir al diccionario
//         puertos[r.destino_puerto]++;
//     }
//     return false; 
// }
string IPUsuario;

bool esIP(Registro r){
    return r.fuente_ip == IPUsuario;
}

string obtenerIPBase(vector<Registro> d){
    int i = busquedaSecuencial(d, [](Registro r){ return r.fuente_ip != "-";});
    string ip = d[i+1].fuente_ip;
    for(int i = 0; i<3; i++) ip.pop_back();
    ip.append(".0");
    return ip;
}
string obtenerIPUsuario(string base, int usuario){
    string ipUsuario = base;
    ipUsuario.pop_back();
    ipUsuario.append( std::to_string(usuario) );
    return ipUsuario;
}

int askNumber(vector <Registro> datos){
    string IPBase = obtenerIPBase(datos);
    int direccionUsuario;
    while(direccionUsuario < 1 || direccionUsuario > 150){
        cout<<"Ingresa un numero > \t";
        cin>>direccionUsuario;
    }
    IPUsuario = obtenerIPUsuario(IPBase, direccionUsuario);
    int usuarioIndex = busquedaSecuencial(datos, *esIP);
    if (usuarioIndex == -1) return askNumber(datos);
    return usuarioIndex; 
}

void llenar(vector <Registro> datos, ConexionesComputadora conexionUsuario){
    for(int i=0; i<datos.size(); i++){
        if( datos[i].destino_ip == conexionUsuario.IP ){
            conexionUsuario.insertarEnConexionesEntrantes(
                datos[i].destino_ip, 
                datos[i].destino_puerto,
                datos[i].destino_hostname
            );
        } 

        if( datos[i].fuente_ip == conexionUsuario.IP ){
            conexionUsuario.insertarEnConexionesSalientes(
                datos[i].fuente_ip, 
                datos[i].fuente_puerto,
                datos[i].fuente_hostname
            );
        } 
    }
}

bool esInterna(string IPBase, string IPInput){
    // 172.26.89.0 == 172.26.89.142
    for(int i=0; i<10; i++){
        if( IPBase[i] != IPInput[i] ) return false;
    }
    return true; 
}

int main(void){
    Reader r; 
    vector <Registro> datos = r.readFile(); 

    string IPBase = obtenerIPBase(datos); 

    int usuarioIndex = askNumber(datos);
    string nombreUsuario = datos[usuarioIndex].fuente_hostname;
    ConexionesComputadora conexionUsuario = ConexionesComputadora(IPUsuario, nombreUsuario); 
    llenar(datos, conexionUsuario);

    string IPConocida = "172.26.89.142"; 
    ConexionesComputadora computadoraConocida = ConexionesComputadora(IPConocida, "janet.reto.com");
    llenar(datos, computadoraConocida);
    //¿Qué dirección ip estas usando?
    cout<<"¿Qué dirección IP estás usando?"<<endl;
    cout<<"\t Estoy usando la dirección ip "<<IPConocida<<endl;
    //¿Cuál fue la ip de la última conexión que recibió esta computadora? ¿Es interna o externa?
    cout << "¿Cuál fue la ip de la última conexión que recibió esta computadora?" << endl; 
    string sth = computadoraConocida.ultimaConexionEntrante(); 
    cout<<"\t La ip de la última conexión que recibió esta computadora es: "<< sth <<endl;
    cout << "¿Es interna o externa?" << endl;
    cout << (esInterna(IPBase, sth) ? "Es interna" : "Es externa") << endl;
    //¿Cuántas conexiones entrantes tiene esta computadora?
    cout << "¿Cuántas conexiones entrantes tiene esta computadora?" << endl; 
    cout << (esInterna(IPBase, sth) ? "Es interna" : "Es externa") << endl;
    //¿Cuántas conexiones entrantes tiene esta computadora?
    cout << "¿Cuántas conexiones entrantes tiene esta computadora?" << endl; 
    cout << "\t Esta computadora tiene " << computadoraConocida.conexionesEntrantes.size() << "conexiones entrantes" << endl;
    //¿Cuántas conexiones salientes tiene esta computadora?
    cout << "¿Cuántas conexiones salientes tiene esta computadora?" << endl; 
    cout << "\t Esta computadora tiene "<<computadoraConocida.conexionesSalientes.size() << "conexiones salientes" << endl;
    //Extra: ¿Tiene esta computadora 3 conexiones seguidas a un mismo sitio web?
    cout << "¿Tiene esta computadora 3 conexiones seguidas a un mismo sitio web?" << endl; 
    return 0;
}
