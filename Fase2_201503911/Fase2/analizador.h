#ifndef ANALIZADOR_H
#define ANALIZADOR_H
#include <cstring>
#include <string>
#include <iostream>

using namespace std;

void Analisis(char comando[500]);
void VerificacionAtributosMkdisk();
void VerificacionAtributosRmdisk();
void VerificacionAtributosFdisk();
void VerificacionAtributosMount();
void VerificacionAtributosUnmount();
void VerificacionAtributosRep();
void ObtenerAtributo(char comando[500],int indice);
bool ParametrosCorrectos(char Metodo[]);
void Exec();
void MontarParticion(char Path[250], char Name[100],int Starparticion);
void DesmontarParticion(char Id[7]);
char *rutaRaid(char path[250]);


//Get atributos
int getSize(char comando[500],int i);
char getFit(char comando[500], int i);
int quitarEspacios(char comando[500], int inicio);
char getUnit(char comando[500], int i);
char *getPath(char comando[500],int i);
char getType(char coma[500],int i);
int getAdd(char coma[500],int i);
char *getDelete(char coma[500],int i);
char *getName(char coma[500],int i);
char *getId(char coma[500],int i);

#endif // ANALIZADOR_H
