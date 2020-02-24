#include "estructuras.h"
#include <iostream>

using namespace std;

MBR getMBR(char Path[250]){

    //Lectura del mbr
    FILE *apun;
    MBR estru;
    apun = fopen(Path,"rb");
    if (apun != NULL){
        fseek(apun,0,SEEK_SET);
        fread(&estru,sizeof(MBR),1,apun);
        fclose(apun);
        fflush(apun);

    }else{
        memset(&estru,0,sizeof(MBR));
    }
    return estru;
}

void setMBR(MBR mbr,char Path[200]){
    FILE *Archivo;
    Archivo = fopen(Path,"rb+");

    if(Archivo!=NULL)
    {
        fseek(Archivo,0,SEEK_SET);
        fwrite(&mbr,sizeof(MBR),1,Archivo);
        fclose(Archivo);
        fflush(Archivo);
    }
    else{
        cout << "El disco Especificado No existe.(setMBR)" <<endl;
    }
}

EBR getEBR(char Path[250]){

    //Lectura del ebr devolvera el ebr que encuentre primero
    FILE *arch;
    EBR ebr;

    int inicio = 0;
    MBR mbr;
    mbr = getMBR(Path);
    if(mbr.mbr_partition_1.part_type == 'e' || mbr.mbr_partition_1.part_type == 'E'){
        inicio= mbr.mbr_partition_1.part_start;
    }
    else if(mbr.mbr_partition_2.part_type == 'e' || mbr.mbr_partition_2.part_type == 'E'){
        inicio= mbr.mbr_partition_2.part_start;
    }
    else if(mbr.mbr_partition_3.part_type == 'e' || mbr.mbr_partition_3.part_type == 'E'){
        inicio= mbr.mbr_partition_3.part_start;
    }
    else if(mbr.mbr_partition_4.part_type == 'e' || mbr.mbr_partition_4.part_type == 'E'){
        inicio= mbr.mbr_partition_4.part_start;
    }


    arch = fopen(Path,"rb");
    if (arch != NULL){
        fseek(arch,inicio,SEEK_SET);
        fread(&ebr,sizeof(EBR),1,arch);
        fclose(arch);
        fflush(arch);

    }else{
        printf("El Archivo Especificado No existe.");
    }
    return ebr;
}

void setEBR(EBR ebr, char Path[250],int start)
{
    FILE *Archivo;
    Archivo = fopen(Path,"rb+");

    if(Archivo!=NULL)
    {
        fseek(Archivo,start,SEEK_SET);
        fwrite(&ebr,sizeof(EBR),1,Archivo);
        fclose(Archivo);
        fflush(Archivo);
    }
    else{
        cout << "El disco Especificado No existe.(setEBR)" <<endl;
    }
}

EBR getEBR(char Path[250], int start)
{
    FILE *arch;
    EBR ebr;

    arch = fopen(Path,"rb");
    if (arch != NULL){
        fseek(arch,start,SEEK_SET);
        fread(&ebr,sizeof(EBR),1,arch);
        fclose(arch);
        fflush(arch);

    }else{
        printf("El Archivo Especificado No existe.");
    }
    return ebr;
}
