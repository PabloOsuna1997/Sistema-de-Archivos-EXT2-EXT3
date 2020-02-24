#ifndef ESTRUCTURASF1_H
#define ESTRUCTURASF1_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <dirent.h>


/*--EXTENDED BOOT RECORD--*/
typedef struct {
    char part_status;
    int part_next;
    char part_fit;
    int part_start;
    int part_size;
    char part_name[16];

}EBR;


/*--PARTICION--*/
typedef struct {
    char part_status;
    char part_type;
    char part_fit;
    int part_start;
    int part_size;
    char part_name[16];

}PARTICION;


/*--MASTER BOOT RECORD--*/
typedef struct {
    int mbr_tamao;
    time_t mbr_fecha_creacion;
    int mbr_disk_signature;
    char disk_fit;
    PARTICION mbr_partition_1;
    PARTICION mbr_partition_2;
    PARTICION mbr_partition_3;
    PARTICION mbr_partition_4;
    //PARTICION mbr_partition[4];
}MBR;


typedef struct MONTADURAS{
    char Id[8];             //vda_____
    char Path[250];
    char Name[100];
    struct MONTADURAS* siguiente;
    int StartParticion;
    int tipoF;
}MONTADURAS;

//MONTADURAS* primero1;
//MONTADURAS* ultimo = NULL;

/*------ESPACIOS DICPONIBLES --------*/ /*Capturare los espacios disponibles en la particion para aplicar ajustes B,F,W.*/
typedef struct ESPVACIOS{
    int tamadisponible;
    int numeroparticion;
    int start;
    struct ESPVACIOS* siguiente;
}ESPVACIOS;

typedef struct ESPACIOSLOGICAS{
    int tamadisponible;
    EBR ebraactual;
    int start;
    struct ESPACIOSLOGICAS* siguiente;
}ESPACIOSLOGICAS;

//ESPVACIOS* pri = NULL;
//ESPVACIOS* ulti = NULL;


MBR getMBR(char path[250]);
void setMBR(MBR mbr,char Path[200]);
EBR getEBR(char Path[250]);
void setEBR(EBR ebr, char Path[250], int start);
EBR getEBR(char Path[250],int start);

#endif // ESTRUCTURASF1_H
