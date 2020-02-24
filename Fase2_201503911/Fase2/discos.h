#ifndef DISCOS_H
#define DISCOS_H
#include "estructuras.h"

void Mkdisk(int size,char fit,char unit, char path[250]);
void Rmdisk(char path[250]);

//particiones
void CrearParticionFDISK(int size, char unit, char path[250], char type, char fit, char name[15]);
int VerificacionAjuste(char Path[250], int TamaoParticionaCrear);
void CrearParticion(char Type, char Fit, int Size, char Name[100], char Path[250],int NumeroParticion,int inicio);
void Verparticiones(char path[250]);
void DeleteParticion(char Path[250], char Name[15]);
bool ExisteLogica(char path[], char name[250]);
void AddParticion(char Path[250],char Nombre[50],int Add);
bool AddLogicas(char Path[250], char Nombre[50], int Add, PARTICION particion);
//MONTADURAS *MontarParticion(char Path[250], char Name[100],int Starparticion,MONTADURAS *primero,MONTADURAS *ultimo);
//MONTADURAS *DesmontarParticion(MONTADURAS *primero, char Id[7]);
void verificacionAjusteLogicas(int TamaoParticionaCrear, char path[250], char Fit, char name[15], PARTICION particion, int fitParticion);

#endif // DISCOS_H
