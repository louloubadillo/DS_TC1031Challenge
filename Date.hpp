# include <iostream>
using namespace std;

class Fecha{
    private:
        int day, month, year, format;
        int diasDeMes[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    public:
        Fecha(int _day, int _month, int _year, int _format = 0){
            day = _day;
            month = _month;
            year = _year;
            format = _format;
        }
        
        Fecha diaSiguiente(){
            if( day < diasDeMes[month] ){
                return Fecha(day+1, month, year, format);
            } else{
                if(month+1<12){
                    return Fecha(1, month+1, year, format);
                } else {
                    return Fecha(1, 1, year+1, format);
                }
            }
        }

        Fecha diaAnterior(){
            if( day > 1 ){
                return Fecha(day-1, month, year, format);
            } else {
                if(month>1){
                    return Fecha(diasDeMes[month-1], month-1, year, format);
                } else {
                    return Fecha(31, 12, year-1, format);
                }
            }
        }

        void imprimirEnFormato(int f){
            if(f == 0){
                cout << day << "/" << month << "/" << year%1000 << endl;
            } else if (f == 1){
                cout << day << "/" << month << "/" << year << endl;
            } else if (f == 2){
                cout << year << "/" << month << "/" << day << endl;
            }
        }
};