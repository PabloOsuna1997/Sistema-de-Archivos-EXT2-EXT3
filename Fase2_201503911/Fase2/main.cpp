#include <QCoreApplication>
#include <iostream>
#include "string"
#include "analizador.h"

using namespace std;

int main()
{

    cout << "\t\t\t\t\tSistema de gestion de discos.\n\t\t\t\t   Juan Pablo Osuna de Leon -- 201503911\n" << endl;
    char comando[500];     //variable donde se almacenara el comando de entrada
    memset(&comando, '\0', sizeof(comando));      //limpieza de arreglo comando

    while(true){
        memset(&comando,'\0',sizeof(comando));
        cout << "\n@201503911->";
        fflush(stdin);
        fgets(comando,500,stdin);

        if(strcasecmp(comando,"exit\n") == 0){
            break;
        }
        else if(strcasecmp(comando, "\n") == 0){
            cout << "Ingrese un comando." << endl;
        }else{
            //no es Exit y no es vacio manda a analizar.
            //mando el comando a analizar
            Analisis(comando);
        }
    }
    return 0;
}


