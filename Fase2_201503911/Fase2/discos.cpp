#include "discos.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <dirent.h>
#include "estructuras.h"
#include <iostream>

using namespace std;

int NummeroPaticion = 0;
//variables para montar particiones
/*int posicionabecedario = 0;
char Letras[26] = {'a','b','c','d','e','f','g','h','i','j','k','l','m',
                   'n','o','p','q','r','s','t','u','v','w','x','y','z'};*/

void Mkdisk(int size, char fit, char unit, char path[250]){
    FILE *fp;                                                                                       //Creamos el disco con los parametros indicados
    fp = fopen (path, "wb+");

    if(fp != NULL){

        if(unit == 'k'||unit == 'K'){
            size = size*1024;
        }
        else if(unit == 'm' || unit == 'M'){
            size = size*1024*1024;
        }

        for(int r = 0; r < size; r++){                                                               //llenamos el archivo con nulos-> se debe cambiar a binario
            fprintf(fp, "%c", 0);
        }

        //CREAMOS EL MBR
        fseek(fp, 0, SEEK_SET);

        MBR mbr;
        memset(&mbr,0,sizeof(MBR));                          //inicializamos en cero el struct
        mbr.mbr_tamao = size;
        mbr.mbr_disk_signature = rand() % 100;
        mbr.disk_fit = fit;
        mbr.mbr_fecha_creacion = time(0);
        mbr.mbr_partition_1.part_status = 'N';
        mbr.mbr_partition_2.part_status = 'N';
        mbr.mbr_partition_3.part_status = 'N';
        mbr.mbr_partition_4.part_status = 'N';

        //printf("Mbr Creado:\nTamano: %d\nDiskAsignature: %d\nFit: %c\nParticion1: %c\nParticion2: %c\nParticion3: %c\nParticion4: %c\n",mbr.mbr_tamao,mbr.mbr_disk_signature,mbr.disk_fit,mbr.mbr_partition_1.part_status,mbr.mbr_partition_2.part_status,mbr.mbr_partition_3.part_status,mbr.mbr_partition_4.part_status);
        fwrite(&mbr,sizeof(MBR),1,fp);

        fclose (fp);
        fflush(fp);
        cout << "\nSe ha creado el disco satisfactoriamente.\n" << endl;
    }else{
        cout <<"Error al tratar de crear el disco indicado.\n"<<endl;
    }

}

void Rmdisk(char path[250])
{
    FILE *disco;
    disco = fopen(path,"rb");
    if (disco != NULL){
        fclose(disco);
        fflush(disco);

        printf("Esta seguro que desea eliminar el disco.(y/n)? ");
        char Decision[3];
        memset(&Decision,'\0',sizeof(Decision));
        fgets(Decision,3,stdin);

        if(tolower(Decision[0]) == 'y'){
            //verificacion si se puede eliminar el disco
            if(remove(path)==0){                                                                                    // Eliminamos el archivo
                cout << "El Disco fue eliminado satisfactoriamente." << endl;
            }else{
                cout << "Ha ocurrido un error al tratar de eliminar el disco.\n" <<endl;
            }
        }else{
            cout << "Eliminacion de disco abortada.\n" <<endl;
        }
    }else{
        cout << "No se ha encontrado el disco en la ruta establecida.(Disco no existe)\n" << endl;
        return;
    }
    fflush(stdin);
}

/*Metodos utilizados en fdisk*/
void CrearParticionFDISK(int size, char unit, char path[], char type, char fit, char name[])
{
    //printf("size: %d\nUnit: %c\nPath: %s\nType: %c\nFit: %c\nName: %s\n",Size,Unit,Path,Type,Fit,Name);

    if(unit == 'B'){
        size = size;
    }
    else if(unit == 'K'){
        size = size*1024;
    }
    else if (unit == 'm' || unit == 'M'){
        size = size*1024*1024;
    }
    else{                   //unit es opcional se tomara KB si no esta especificado en el comando
        if(unit == '\0'){
            unit = 'K';
            size = size*1024;
        }else{
            cout << "Particion no creada debido a que la unidad de medida no es reconocida.(Bytes,Kb,Mb)\n" << endl;
            return;
        }
    }

    if(fit == '\0'){        //fit es opcional se tomara el peor ajuste si no esta especificado en el comando
        fit = 'W';
    }else{
        if(fit != 'B' && fit != 'F' && fit != 'W')  {
            cout << "Particion no creada debido a que el ajuste especificado no existe.\n" <<endl;
            return;
        }
    }

    if(type == '\0'){        //fit es opcional se tomara el peor ajuste si no esta especificado en el comando
        type = 'P';
    }else{
        if(type != 'P' && type != 'E' && type != 'L')  {
            cout << "Particion no creada debido a que el tipo de particion especificada no existe.\n" <<endl;
            return;
        }
    }

    /*--captura del mbr--*/
    FILE *apun;
    apun = fopen(path,"rb");

    if(apun != NULL){
        fclose(apun);           //si existe lo cierro.
        fflush(apun);

        MBR mbr;
        mbr = getMBR(path);
        //if(mbr.mbr_disk_signature == 0){
        //verificacion que no exista una particion con el mismo nombre
        if(strcasecmp(mbr.mbr_partition_1.part_name,name)!=0
                && strcasecmp(mbr.mbr_partition_2.part_name,name)!=0
                && strcasecmp(mbr.mbr_partition_3.part_name,name)!=0
                && strcasecmp(mbr.mbr_partition_4.part_name,name)!=0)
        {
            //evaluamos las dimasiones de la particion con el disco

            if(size <= (mbr.mbr_tamao - sizeof(MBR))){

                //si la particion es primaria
                if(type == 'p' || type == 'P'){

                    int start = VerificacionAjuste(path,size);                  //start es el bit donde dara inicio l particion
                    CrearParticion(type,fit,size,name,path,NummeroPaticion,start);
                }
                else if(type == 'e' || type == 'E'){

                    //Verificamos que no haya alguna otra particion extendida creada de ser asi notificara un error
                    if( (mbr.mbr_partition_1.part_type != 'e' && mbr.mbr_partition_1.part_type != 'E')
                            && (mbr.mbr_partition_2.part_type != 'e' && mbr.mbr_partition_2.part_type != 'E')
                            && (mbr.mbr_partition_3.part_type != 'e' && mbr.mbr_partition_3.part_type != 'E')
                            && (mbr.mbr_partition_4.part_type != 'e' && mbr.mbr_partition_4.part_type != 'E') ){

                        int start = VerificacionAjuste(path,size);
                        CrearParticion(type,fit,size,name,path,NummeroPaticion,start);
                    }else{
                        printf("\nError ya existe una particion extendida creada en el disco.\n");
                    }
                }
                //******ACCIONES PARA PARTICIONES LOGICAS*****
                else{
                    if(mbr.mbr_partition_1.part_type == 'e' || mbr.mbr_partition_1.part_type == 'E'){
                        verificacionAjusteLogicas(size,path,fit,name,mbr.mbr_partition_1,mbr.mbr_partition_1.part_fit);
                    }
                    else if(mbr.mbr_partition_2.part_type == 'e' || mbr.mbr_partition_2.part_type == 'E'){
                        verificacionAjusteLogicas(size,path,fit,name,mbr.mbr_partition_2,mbr.mbr_partition_2.part_fit);
                    }

                    else if(mbr.mbr_partition_3.part_type == 'e' || mbr.mbr_partition_3.part_type == 'E'){
                        verificacionAjusteLogicas(size,path,fit,name,mbr.mbr_partition_3,mbr.mbr_partition_3.part_fit);
                    }

                    else if(mbr.mbr_partition_4.part_type == 'e' || mbr.mbr_partition_4.part_type == 'E'){
                        verificacionAjusteLogicas(size,path,fit,name,mbr.mbr_partition_4,mbr.mbr_partition_4.part_fit);
                    }else{
                        printf("No existe ninguna particion extendida.\n");
                    }

                    /*cout << "Particion Logica.";
                    //verificamos que exista alguna particion extendida
                    if((mbr.mbr_partition_1.part_type == 'e' || mbr.mbr_partition_1.part_type == 'E')
                            || (mbr.mbr_partition_2.part_type == 'e' || mbr.mbr_partition_2.part_type == 'E')
                            || (mbr.mbr_partition_3.part_type == 'e' || mbr.mbr_partition_3.part_type == 'E')
                            || (mbr.mbr_partition_4.part_type == 'e' || mbr.mbr_partition_4.part_type == 'E') ){

                        EBR ebr = getEBR(path);
                        if(ebr.part_next == -1 && strcasecmp(ebr.part_name,"EBR_INICIAL") == 0){
                            //NO HAY PARTICIONES LOGICAS ACTUALMENTE
                            //solo actualizo los datos
                            ebr.part_fit = fit;
                            memset(&ebr.part_name,'\0',sizeof(ebr.part_name));
                            strcpy(ebr.part_name,name);
                            ebr.part_next = -1;
                            ebr.part_size = size;
                            //ebr.part_start = inicio de la particion
                            ebr.part_status = 'S';

                            setEBR(ebr,path,ebr.part_start);    //seteamos el ebr actualizado

                        }else{
                            //YA EXISTE POR LO MENOS UNA PARTICION LOGICA
                            EBR ebr = getEBR(path);     //captura el ebr inicial
                            do{

                                if(ebr.part_next == -1){
                                    //actualizamos el apuntador del ebr anterior
                                    ebr.part_next = ebr.part_start + ebr.part_size;
                                    setEBR(ebr,path,ebr.part_start);

                                    //creamos el nuevo ebr
                                    EBR new_ebr;
                                    new_ebr.part_fit = fit;
                                    memset(&new_ebr.part_name,'\0',sizeof(new_ebr.part_name));
                                    strcpy(new_ebr.part_name,name);
                                    new_ebr.part_next = -1;
                                    new_ebr.part_size = size;
                                    new_ebr.part_start = ebr.part_next;      //inicia la particion donde marca el apuntador next
                                    new_ebr.part_status = 'S';

                                    setEBR(new_ebr,path,ebr.part_next);      //escribimos el nuevo ebr
                                    break;
                                }else{
                                    ebr = getEBR(path,ebr.part_next);
                                }
                            }while(true);
                        }
                    }else{
                        cout << "No se puede crear particion logica porque no existe alguna particion extendida."<<endl;
                    }*/
                }
            }else{
                cout << "Particion sobrepasa la capacidad del disco. (CrearParticionFdisk.)" << endl;
            }
        }else{
            cout << "Error al crear particion debido a que ya existe otra particion con el mismo nombre." <<endl;
        }
        //}else{
        //  cout << "No existe Master Boot Record en el disco indicado." << endl;
        // }
    }else{
        cout << "Disco especificado no existe." <<endl;
    }
}

int VerificacionAjuste(char Path[250], int TamaoParticionaCrear){

    ESPVACIOS* pri = NULL;
    ESPVACIOS* ulti = NULL;

    MBR mbr = getMBR(Path);
    char Fit = '\0';

    if(mbr.disk_fit == 'b' ||  mbr.disk_fit == 'B'){
        Fit = 'B';
    }
    else if(mbr.disk_fit == 'f' ||  mbr.disk_fit == 'F'){
        Fit = 'F';
    }
    else if(mbr.disk_fit == 'w' ||  mbr.disk_fit == 'W'){
        Fit = 'W';
    }

    int ultimaposicionOCUPADA = sizeof(MBR);
    NummeroPaticion = 0;

    /*---captura de espacios vacios---*/

    //vacia primera
    if(mbr.mbr_partition_1.part_status == 'N' && mbr.mbr_partition_2.part_status == 'S' && mbr.mbr_partition_3.part_status == 'S' && mbr.mbr_partition_4.part_status == 'S'){

        ESPVACIOS *Nuevonodo = new ESPVACIOS();
        Nuevonodo->numeroparticion = 1;
        Nuevonodo->tamadisponible = mbr.mbr_partition_2.part_start - sizeof(MBR);
        Nuevonodo->start = sizeof(MBR);

        if(pri == NULL){
            pri = Nuevonodo;
            ulti = Nuevonodo;
            ulti->siguiente = NULL;
            ulti->siguiente = NULL;
        }else{
            ulti->siguiente = Nuevonodo;
            ulti = Nuevonodo;
            ulti->siguiente = NULL;
        }
    }
    //vacias primera-segunda
    else if (mbr.mbr_partition_1.part_status == 'N' && mbr.mbr_partition_2.part_status == 'N' && mbr.mbr_partition_3.part_status == 'S' && mbr.mbr_partition_4.part_status == 'S'){
        ESPVACIOS *Nuevonodo = (ESPVACIOS*)malloc(sizeof(ESPVACIOS));
        Nuevonodo->numeroparticion = 1;
        Nuevonodo->tamadisponible = mbr.mbr_partition_3.part_start - sizeof(MBR);
        Nuevonodo->start = sizeof(MBR);

        if(pri == NULL){
            pri = Nuevonodo;
            ulti = Nuevonodo;
            ulti->siguiente = NULL;
            ulti->siguiente = NULL;
        }else{
            ulti->siguiente = Nuevonodo;
            ulti = Nuevonodo;
            ulti->siguiente = NULL;
        }
    }
    //vacias primera-segunda-tercera
    else if (mbr.mbr_partition_1.part_status == 'N' && mbr.mbr_partition_2.part_status == 'N' && mbr.mbr_partition_3.part_status == 'N' && mbr.mbr_partition_4.part_status == 'S'){
        ESPVACIOS *Nuevonodo = (ESPVACIOS*)malloc(sizeof(ESPVACIOS));
        Nuevonodo->numeroparticion = 1;
        Nuevonodo->tamadisponible = mbr.mbr_partition_4.part_start - sizeof(MBR);
        Nuevonodo->start = sizeof(MBR);

        if(pri == NULL){
            pri = Nuevonodo;
            ulti = Nuevonodo;
            ulti->siguiente = NULL;
            ulti->siguiente = NULL;
        }else{
            ulti->siguiente = Nuevonodo;
            ulti = Nuevonodo;
            ulti->siguiente = NULL;
        }
    }
    //vacios primera-segunda-tercera-cuarta
    else if (mbr.mbr_partition_1.part_status == 'N' && mbr.mbr_partition_2.part_status == 'N' && mbr.mbr_partition_3.part_status == 'N' && mbr.mbr_partition_4.part_status == 'N'){
        ESPVACIOS *Nuevonodo = (ESPVACIOS*)malloc(sizeof(ESPVACIOS));
        Nuevonodo->numeroparticion = 1;
        Nuevonodo->tamadisponible = mbr.mbr_tamao - sizeof(MBR);
        Nuevonodo->start = sizeof(MBR);

        if(pri == NULL){
            pri = Nuevonodo;
            ulti = Nuevonodo;
            ulti->siguiente = NULL;
            ulti->siguiente = NULL;
        }else{
            ulti->siguiente = Nuevonodo;
            ulti = Nuevonodo;
            ulti->siguiente = NULL;
        }
    }
    //vacias primera-tercera
    else if (mbr.mbr_partition_1.part_status == 'N' && mbr.mbr_partition_2.part_status == 'S' && mbr.mbr_partition_3.part_status == 'N' && mbr.mbr_partition_4.part_status == 'S'){

        ESPVACIOS *Nuevonodo = (ESPVACIOS*)malloc(sizeof(ESPVACIOS));
        Nuevonodo->numeroparticion = 1;
        Nuevonodo->tamadisponible = mbr.mbr_partition_2.part_start - sizeof(MBR);
        Nuevonodo->start = sizeof(MBR);

        if(pri == NULL){
            pri = Nuevonodo;
            ulti = Nuevonodo;
            ulti->siguiente = NULL;
            ulti->siguiente = NULL;
        }else{
            ulti->siguiente = Nuevonodo;
            ulti = Nuevonodo;
            ulti->siguiente = NULL;
        }

        ESPVACIOS *Nuevonodo1 = (ESPVACIOS*)malloc(sizeof(ESPVACIOS));
        Nuevonodo1->numeroparticion = 3;
        Nuevonodo1->tamadisponible = mbr.mbr_partition_4.part_start - (mbr.mbr_partition_2.part_start + mbr.mbr_partition_2.part_size);
        Nuevonodo1->start = mbr.mbr_partition_2.part_start + mbr.mbr_partition_2.part_size;

        if(pri == NULL){
            pri = Nuevonodo1;
            ulti = Nuevonodo1;
            ulti->siguiente = NULL;
            ulti->siguiente = NULL;
        }else{
            ulti->siguiente = Nuevonodo1;
            ulti = Nuevonodo1;
            ulti->siguiente = NULL;
        }

    }
    //vacias primera-tercer-cuarta
    else if (mbr.mbr_partition_1.part_status == 'N' && mbr.mbr_partition_2.part_status == 'S' && mbr.mbr_partition_3.part_status == 'N' && mbr.mbr_partition_4.part_status == 'N'){

        ESPVACIOS *Nuevonodo = (ESPVACIOS*)malloc(sizeof(ESPVACIOS));
        Nuevonodo->numeroparticion = 1;
        Nuevonodo->tamadisponible = mbr.mbr_partition_2.part_start - sizeof(MBR);
        Nuevonodo->start = sizeof(MBR);

        if(pri == NULL){
            pri = Nuevonodo;
            ulti = Nuevonodo;
            ulti->siguiente = NULL;
            ulti->siguiente = NULL;
        }else{
            ulti->siguiente = Nuevonodo;
            ulti = Nuevonodo;
            ulti->siguiente = NULL;
        }

        ESPVACIOS *Nuevonodo1 = (ESPVACIOS*)malloc(sizeof(ESPVACIOS));
        Nuevonodo1->numeroparticion = 3;
        Nuevonodo1->tamadisponible = mbr.mbr_tamao  - (mbr.mbr_partition_2.part_start - mbr.mbr_partition_2.part_size);
        Nuevonodo1->start = mbr.mbr_partition_2.part_start + mbr.mbr_partition_2.part_size;

        if(pri == NULL){
            pri = Nuevonodo1;
            ulti = Nuevonodo1;
            ulti->siguiente = NULL;
            ulti->siguiente = NULL;
        }else{
            ulti->siguiente = Nuevonodo1;
            ulti = Nuevonodo1;
            ulti->siguiente = NULL;
        }
    }
    //vacias primera-cuarta
    else if (mbr.mbr_partition_1.part_status == 'N' && mbr.mbr_partition_2.part_status == 'S' && mbr.mbr_partition_3.part_status == 'S' && mbr.mbr_partition_4.part_status == 'N'){
        ESPVACIOS *Nuevonodo = (ESPVACIOS*)malloc(sizeof(ESPVACIOS));
        Nuevonodo->numeroparticion = 1;
        Nuevonodo->tamadisponible = mbr.mbr_partition_2.part_start - sizeof(MBR);
        Nuevonodo->start = sizeof(MBR);

        if(pri == NULL){
            pri = Nuevonodo;
            ulti = Nuevonodo;
            ulti->siguiente = NULL;
            ulti->siguiente = NULL;
        }else{
            ulti->siguiente = Nuevonodo;
            ulti = Nuevonodo;
            ulti->siguiente = NULL;
        }

        ESPVACIOS *Nuevonodo1 = (ESPVACIOS*)malloc(sizeof(ESPVACIOS));
        Nuevonodo1->numeroparticion = 4;
        Nuevonodo1->tamadisponible = mbr.mbr_tamao  - (mbr.mbr_partition_3.part_start - mbr.mbr_partition_3.part_size);
        Nuevonodo1->start = mbr.mbr_partition_3.part_start + mbr.mbr_partition_3.part_size;

        if(pri == NULL){
            pri = Nuevonodo1;
            ulti = Nuevonodo1;
            ulti->siguiente = NULL;
            ulti->siguiente = NULL;
        }else{
            ulti->siguiente = Nuevonodo1;
            ulti = Nuevonodo1;
            ulti->siguiente = NULL;
        }
    }
    //vacias primera-segunda-cuarta
    else if (mbr.mbr_partition_1.part_status == 'N' && mbr.mbr_partition_2.part_status == 'N' && mbr.mbr_partition_3.part_status == 'S' && mbr.mbr_partition_4.part_status == 'N'){
        ESPVACIOS *Nuevonodo = (ESPVACIOS*)malloc(sizeof(ESPVACIOS));
        Nuevonodo->numeroparticion = 1;
        Nuevonodo->tamadisponible = mbr.mbr_partition_3.part_start - sizeof(MBR);
        Nuevonodo->start = sizeof(MBR);

        if(pri == NULL){
            pri = Nuevonodo;
            ulti = Nuevonodo;
            ulti->siguiente = NULL;
            ulti->siguiente = NULL;
        }else{
            ulti->siguiente = Nuevonodo;
            ulti = Nuevonodo;
            ulti->siguiente = NULL;
        }

        ESPVACIOS *Nuevonodo1 = (ESPVACIOS*)malloc(sizeof(ESPVACIOS));
        Nuevonodo1->numeroparticion = 4;
        Nuevonodo1->tamadisponible = mbr.mbr_tamao  - (mbr.mbr_partition_3.part_start - mbr.mbr_partition_3.part_size);
        Nuevonodo1->start = mbr.mbr_partition_3.part_start + mbr.mbr_partition_3.part_size;

        if(pri == NULL){
            pri = Nuevonodo1;
            ulti = Nuevonodo1;
            ulti->siguiente = NULL;
            ulti->siguiente = NULL;
        }else{
            ulti->siguiente = Nuevonodo1;
            ulti = Nuevonodo1;
            ulti->siguiente = NULL;
        }
    }
    //vacias segunda
    else if (mbr.mbr_partition_1.part_status == 'S' && mbr.mbr_partition_2.part_status == 'N' && mbr.mbr_partition_3.part_status == 'S' && mbr.mbr_partition_4.part_status == 'S'){
        ESPVACIOS *Nuevonodo = (ESPVACIOS*)malloc(sizeof(ESPVACIOS));
        Nuevonodo->numeroparticion = 2;
        Nuevonodo->tamadisponible = mbr.mbr_partition_3.part_start - (mbr.mbr_partition_1.part_start + mbr.mbr_partition_1.part_size);
        Nuevonodo->start = mbr.mbr_partition_1.part_start + mbr.mbr_partition_1.part_size;

        if(pri == NULL){
            pri = Nuevonodo;
            ulti = Nuevonodo;
            ulti->siguiente = NULL;
            ulti->siguiente = NULL;
        }else{
            ulti->siguiente = Nuevonodo;
            ulti = Nuevonodo;
            ulti->siguiente = NULL;
        }
    }
    //vacias segunda-tercera
    else if (mbr.mbr_partition_1.part_status == 'S' && mbr.mbr_partition_2.part_status == 'N' && mbr.mbr_partition_3.part_status == 'N' && mbr.mbr_partition_4.part_status == 'S'){
        ESPVACIOS *Nuevonodo = (ESPVACIOS*)malloc(sizeof(ESPVACIOS));
        Nuevonodo->numeroparticion = 2;
        Nuevonodo->tamadisponible = mbr.mbr_partition_4.part_start - (mbr.mbr_partition_1.part_start + mbr.mbr_partition_1.part_size);
        Nuevonodo->start = (mbr.mbr_partition_1.part_start + mbr.mbr_partition_1.part_size);

        if(pri == NULL){
            pri = Nuevonodo;
            ulti = Nuevonodo;
            ulti->siguiente = NULL;
            ulti->siguiente = NULL;
        }else{
            ulti->siguiente = Nuevonodo;
            ulti = Nuevonodo;
            ulti->siguiente = NULL;
        }
    }
    //vacias segunda-tercera-cuarta
    else if (mbr.mbr_partition_1.part_status == 'S' && mbr.mbr_partition_2.part_status == 'N' && mbr.mbr_partition_3.part_status == 'N' && mbr.mbr_partition_4.part_status == 'N'){
        ESPVACIOS *Nuevonodo = (ESPVACIOS*)malloc(sizeof(ESPVACIOS));
        Nuevonodo->numeroparticion = 2;
        Nuevonodo->tamadisponible = mbr.mbr_tamao - (mbr.mbr_partition_1.part_start + mbr.mbr_partition_1.part_size);
        Nuevonodo->start = mbr.mbr_partition_1.part_start + mbr.mbr_partition_1.part_size;

        if(pri == NULL){
            pri = Nuevonodo;
            ulti = Nuevonodo;
            ulti->siguiente = NULL;
            ulti->siguiente = NULL;
        }else{
            ulti->siguiente = Nuevonodo;
            ulti = Nuevonodo;
            ulti->siguiente = NULL;
        }
    }
    //vacias segunda-cuarta
    else if (mbr.mbr_partition_1.part_status == 'S' && mbr.mbr_partition_2.part_status == 'N' && mbr.mbr_partition_3.part_status == 'S' && mbr.mbr_partition_4.part_status == 'N'){
        ESPVACIOS *Nuevonodo = (ESPVACIOS*)malloc(sizeof(ESPVACIOS));
        Nuevonodo->numeroparticion = 2;
        Nuevonodo->tamadisponible = mbr.mbr_partition_3.part_start - (mbr.mbr_partition_1.part_start + mbr.mbr_partition_1.part_size);
        Nuevonodo->start = mbr.mbr_partition_1.part_start + mbr.mbr_partition_1.part_size;

        if(pri == NULL){
            pri = Nuevonodo;
            ulti = Nuevonodo;
            ulti->siguiente = NULL;
            ulti->siguiente = NULL;
        }else{
            ulti->siguiente = Nuevonodo;
            ulti = Nuevonodo;
            ulti->siguiente = NULL;
        }

        ESPVACIOS *Nuevonodo1 = (ESPVACIOS*)malloc(sizeof(ESPVACIOS));
        Nuevonodo1->numeroparticion = 4;
        Nuevonodo1->tamadisponible = mbr.mbr_tamao  - (mbr.mbr_partition_3.part_start - mbr.mbr_partition_3.part_size);
        Nuevonodo1->start = mbr.mbr_partition_3.part_start + mbr.mbr_partition_3.part_size;

        if(pri == NULL){
            pri = Nuevonodo1;
            ulti = Nuevonodo1;
            ulti->siguiente = NULL;
            ulti->siguiente = NULL;
        }else{
            ulti->siguiente = Nuevonodo1;
            ulti = Nuevonodo1;
            ulti->siguiente = NULL;
        }
    }
    //vacias tercera
    else if (mbr.mbr_partition_1.part_status == 'S' && mbr.mbr_partition_2.part_status == 'S' && mbr.mbr_partition_3.part_status == 'N' && mbr.mbr_partition_4.part_status == 'S'){
        ESPVACIOS *Nuevonodo = (ESPVACIOS*)malloc(sizeof(ESPVACIOS));
        Nuevonodo->numeroparticion = 3;
        Nuevonodo->tamadisponible = mbr.mbr_partition_4.part_start - (mbr.mbr_partition_2.part_start + mbr.mbr_partition_2.part_size);
        Nuevonodo->start = (mbr.mbr_partition_2.part_start + mbr.mbr_partition_2.part_size);

        if(pri == NULL){
            pri = Nuevonodo;
            ulti = Nuevonodo;
            ulti->siguiente = NULL;
            ulti->siguiente = NULL;
        }else{
            ulti->siguiente = Nuevonodo;
            ulti = Nuevonodo;
            ulti->siguiente = NULL;
        }

    }
    //vacias tercera-cuarta
    else if (mbr.mbr_partition_1.part_status == 'S' && mbr.mbr_partition_2.part_status == 'S' && mbr.mbr_partition_3.part_status == 'N' && mbr.mbr_partition_4.part_status == 'N'){
        ESPVACIOS *Nuevonodo = (ESPVACIOS*)malloc(sizeof(ESPVACIOS));
        Nuevonodo->numeroparticion = 3;
        Nuevonodo->tamadisponible = mbr.mbr_tamao - (mbr.mbr_partition_2.part_start + mbr.mbr_partition_2.part_size);
        Nuevonodo->start = mbr.mbr_partition_2.part_start + mbr.mbr_partition_2.part_size;

        if(pri == NULL){
            pri = Nuevonodo;
            ulti = Nuevonodo;
            ulti->siguiente = NULL;
            ulti->siguiente = NULL;
        }else{
            ulti->siguiente = Nuevonodo;
            ulti = Nuevonodo;
            ulti->siguiente = NULL;
        }

    }
    //vacias cuarta
    else if (mbr.mbr_partition_1.part_status == 'S' && mbr.mbr_partition_2.part_status == 'S' && mbr.mbr_partition_3.part_status == 'S' && mbr.mbr_partition_4.part_status == 'N'){
        ESPVACIOS *Nuevonodo = (ESPVACIOS*)malloc(sizeof(ESPVACIOS));
        Nuevonodo->numeroparticion = 4;
        Nuevonodo->tamadisponible = mbr.mbr_tamao - (mbr.mbr_partition_3.part_start + mbr.mbr_partition_3.part_size);
        Nuevonodo->start = (mbr.mbr_partition_3.part_start + mbr.mbr_partition_3.part_size);

        if(pri == NULL){
            pri = Nuevonodo;
            ulti = Nuevonodo;
            ulti->siguiente = NULL;
            ulti->siguiente = NULL;
        }else{
            ulti->siguiente = Nuevonodo;
            ulti = Nuevonodo;
            ulti->siguiente = NULL;
        }

    }


    /*--verificacion de cual espacio se adecua segun el ajuste--*/
    ESPVACIOS *aux = pri;
    bool primero = true;
    int restante =0;
    int res  = 0;
    bool sicabeenalgunespacio = false;

    if(aux != NULL){
        while (aux != NULL){
            if(aux->tamadisponible > TamaoParticionaCrear){
                sicabeenalgunespacio = true;
                break;
            }
            aux = aux->siguiente;
        }
        if(sicabeenalgunespacio){           /*validacion para que entre en cualquier espacio disponible por ejemplo si tengo dos
                                             espacios debera caber como minimo en laguno de los dos de lo contrario lanzara excepcion*/

            while (aux != NULL){
                if(aux->tamadisponible > TamaoParticionaCrear){

                    res  = aux->tamadisponible - TamaoParticionaCrear;
                    if(primero){
                        if(Fit == 'f' || Fit == 'F'){
                            if(res > TamaoParticionaCrear){
                                restante = res;
                                NummeroPaticion = aux->numeroparticion;
                                ultimaposicionOCUPADA = aux->start;
                                break;
                            }
                        }
                        restante = res;
                        NummeroPaticion = aux->numeroparticion;
                        ultimaposicionOCUPADA = aux->start;
                        primero = false;

                    }else{
                        if(Fit == 'b' || Fit == 'B'){
                            if(res < restante){
                                restante = res;
                                NummeroPaticion = aux->numeroparticion;
                                ultimaposicionOCUPADA = aux->start;
                            }
                        }
                        else if(Fit == 'w' || Fit == 'W'){
                            if(res > restante){
                                restante = res;
                                NummeroPaticion = aux->numeroparticion;
                                ultimaposicionOCUPADA = aux->start;
                            }
                        } else if(Fit == 'f' || Fit == 'F'){
                            if(res > TamaoParticionaCrear){
                                restante = res;
                                NummeroPaticion = aux->numeroparticion;
                                ultimaposicionOCUPADA = aux->start;

                                break;
                            }
                        }
                    }
                }
                aux = aux->siguiente;
            }
            return ultimaposicionOCUPADA;

        }else{
            printf("La particion que desea crear supera en tama~o\na los espacios disponibles en el disco.\n");
        }
    }else{
        printf("No se encontro ningun espacio disponible para crear la particion.\n");
    }
}

void CrearParticion(char Type, char Fit, int Size, char Name[100], char Path[250],int NumeroParticion,int inicio){

    MBR mbr;
    mbr = getMBR(Path);

    //verificacionde que no exeda el tamaño del disco
    if(Size <= (mbr.mbr_tamao - sizeof(MBR) - mbr.mbr_partition_1.part_size - mbr.mbr_partition_2.part_size - mbr.mbr_partition_3.part_size - mbr.mbr_partition_4.part_size)){
        if(NumeroParticion == 1){//primera particion

            mbr.mbr_partition_1.part_status = 'S';
            mbr.mbr_partition_1.part_fit = Fit;
            strcpy(mbr.mbr_partition_1.part_name,Name);
            mbr.mbr_partition_1.part_size = Size;
            mbr.mbr_partition_1.part_start = inicio;
            mbr.mbr_partition_1.part_type = Type;

            printf("\nPARTICION CREADA:\npart_Status: %c\nPart_Fit: %c\nPart_Name: %s\nPart_size: %d\npart_start: %d\nPart_Type: %c",mbr.mbr_partition_1.part_status, mbr.mbr_partition_1.part_fit,mbr.mbr_partition_1.part_name,mbr.mbr_partition_1.part_size, mbr.mbr_partition_1.part_start, mbr.mbr_partition_1.part_type);
            setMBR(mbr,Path);
        }
        else if(NumeroParticion == 2){//segunda particion

            mbr.mbr_partition_2.part_status = 'S';
            mbr.mbr_partition_2.part_fit = Fit;
            strcpy(mbr.mbr_partition_2.part_name,Name);
            mbr.mbr_partition_2.part_size = Size;
            mbr.mbr_partition_2.part_start = inicio;
            mbr.mbr_partition_2.part_type = Type;

            printf("\nPARTICION CREADA:\npart_Status: %c\nPart_Fit: %c\nPart_Name: %s\nPart_size: %d\npart_start: %d\nPart_Type: %c",mbr.mbr_partition_2.part_status, mbr.mbr_partition_2.part_fit,mbr.mbr_partition_2.part_name,mbr.mbr_partition_2.part_size, mbr.mbr_partition_2.part_start, mbr.mbr_partition_2.part_type);
            setMBR(mbr,Path);
        }
        else if(NumeroParticion == 3){//tercera particion

            mbr.mbr_partition_3.part_status = 'S';
            mbr.mbr_partition_3.part_fit = Fit;
            strcpy(mbr.mbr_partition_3.part_name,Name);
            mbr.mbr_partition_3.part_size = Size;
            mbr.mbr_partition_3.part_start = inicio;
            mbr.mbr_partition_3.part_type = Type;

            printf("\nPARTICION CREADA:\npart_Status: %c\nPart_Fit: %c\nPart_Name: %s\nPart_size: %d\npart_start: %d\nPart_Type: %c",mbr.mbr_partition_3.part_status, mbr.mbr_partition_3.part_fit,mbr.mbr_partition_3.part_name,mbr.mbr_partition_3.part_size, mbr.mbr_partition_3.part_start, mbr.mbr_partition_3.part_type);
            setMBR(mbr,Path);
        }
        else  if(NumeroParticion == 4){//cuarta particion

            mbr.mbr_partition_4.part_status = 'S';
            mbr.mbr_partition_4.part_fit = Fit;
            strcpy(mbr.mbr_partition_4.part_name,Name);
            mbr.mbr_partition_4.part_size = Size;
            mbr.mbr_partition_4.part_start = inicio;
            mbr.mbr_partition_4.part_type = Type;

            printf("\nPARTICION CREADA:\npart_Status: %c\nPart_Fit: %c\nPart_Name: %s\nPart_size: %d\npart_start: %d\nPart_Type: %c",mbr.mbr_partition_4.part_status, mbr.mbr_partition_4.part_fit,mbr.mbr_partition_4.part_name,mbr.mbr_partition_4.part_size, mbr.mbr_partition_4.part_start, mbr.mbr_partition_4.part_type);
            setMBR(mbr,Path);
        }

        //ademas si el tipo de la particion es extendida se crea el ebr inicial
        if(Type == 'e' || Type == 'E'){

            //abrimos el disco
            FILE *arch;
            arch = fopen(Path,"rb+");

            switch (NumeroParticion) {
            case 1:
                fseek(arch,mbr.mbr_partition_1.part_start,SEEK_SET);            //posicionamos el cursor donde inicia la particion extendida
                //creamos el ebr inicial siempre debe permanecer
                EBR ebr;
                ebr.part_fit = Fit;
                strcpy(ebr.part_name,"EBR_INICIAL");
                ebr.part_next = -1;                                             //como no hay siguiente ebr se coloca -1 si ubiera se coloca el byte donde inicia el siguiente ebr
                ebr.part_size = sizeof(EBR);
                ebr.part_start = mbr.mbr_partition_1.part_start;
                ebr.part_status = 'N';

                fwrite(&ebr,sizeof(EBR),1,arch);
                fclose(arch);
                fflush(arch);
                break;
            case 2:
                fseek(arch,mbr.mbr_partition_2.part_start,SEEK_SET);            //posicionamos el cursor donde inicia la particion extendida
                //creamos el ebr inicial siempre debe permanecer
                EBR ebr1;
                ebr1.part_fit = Fit;
                strcpy(ebr1.part_name,"EBR_INICIAL");
                ebr1.part_next = -1;                                             //como no hay siguiente ebr se coloca -1 si ubiera se coloca el byte donde inicia el siguiente ebr
                ebr1.part_size = sizeof(EBR);
                ebr1.part_start = mbr.mbr_partition_2.part_start;
                ebr1.part_status = 'N';

                fwrite(&ebr1,sizeof(EBR),1,arch);
                fclose(arch);
                fflush(arch);
                break;
            case 3:
                fseek(arch,mbr.mbr_partition_3.part_start,SEEK_SET);            //posicionamos el cursor donde inicia la particion extendida
                EBR ebr2;
                ebr2.part_fit = Fit;
                strcpy(ebr2.part_name,"EBR_INICIAL");
                ebr2.part_next = -1;                                             //como no hay siguiente ebr se coloca -1 si ubiera se coloca el byte donde inicia el siguiente ebr
                ebr2.part_size = sizeof(EBR);
                ebr2.part_start = mbr.mbr_partition_3.part_start;
                ebr2.part_status = 'N';

                fwrite(&ebr2,sizeof(EBR),1,arch);
                fclose(arch);
                fflush(arch);
                break;
            case 4:
                fseek(arch,mbr.mbr_partition_4.part_start,SEEK_SET);            //posicionamos el cursor donde inicia la particion extendida
                EBR ebr3;
                ebr3.part_fit = Fit;
                strcpy(ebr3.part_name,"EBR_INICIAL");
                ebr3.part_next = -1;                                             //como no hay siguiente ebr se coloca -1 si ubiera se coloca el byte donde inicia el siguiente ebr
                ebr3.part_size = sizeof(EBR);
                ebr3.part_start = mbr.mbr_partition_4.part_start;
                ebr3.part_status = 'N';

                fwrite(&ebr3,sizeof(EBR),1,arch);
                fclose(arch);
                fflush(arch);
                break;
            default:
                break;
            }

            EBR ebr1;
            ebr1 = getEBR(Path);
            //printf("\n\nRECUPERACION DEL EBR:\npart_status: %c\npart_fit: %c\npart_start: %d\npart_size: %d\npart_next: %d\npart_name: %s\n",ebr1.part_status,ebr1.part_fit,ebr1.part_start,ebr1.part_size,ebr1.part_next,ebr1.part_name);

        }
        printf("\nParticion %s de %d bytes creada exitosamente\n",Name,Size);

    }else{
        printf("\nEspacio Insuficiente.\n");
    }
}

void Verparticiones(char path[250]){
    MBR mbr;
    mbr = getMBR(path);

    printf("Particion 1: %c \n",mbr.mbr_partition_1.part_status);
    printf("Particion 2: %c \n",mbr.mbr_partition_2.part_status);
    printf("Particion 3: %c \n",mbr.mbr_partition_3.part_status);
    printf("Particion 4: %c \n",mbr.mbr_partition_4.part_status);
}

void DeleteParticion(char Path[250], char Name[15]){
    MBR mbr = getMBR(Path);
    int inicioParticion = 0;
    int size = 0;

    if(strcasecmp(mbr.mbr_partition_1.part_name,Name) == 0){
        memset(&mbr.mbr_partition_1,0,sizeof(PARTICION));
        mbr.mbr_partition_1.part_status = 'N';
        inicioParticion = mbr.mbr_partition_1.part_start;
        size = mbr.mbr_partition_1.part_size;
    }
    else  if(strcasecmp(mbr.mbr_partition_2.part_name,Name) == 0){
        memset(&mbr.mbr_partition_2,0,sizeof(PARTICION));
        mbr.mbr_partition_2.part_status = 'N';
        inicioParticion = mbr.mbr_partition_2.part_start;
        size = mbr.mbr_partition_2.part_size;
    }
    else  if(strcasecmp(mbr.mbr_partition_3.part_name,Name) == 0){
        memset(&mbr.mbr_partition_3,0,sizeof(PARTICION));
        mbr.mbr_partition_3.part_status = 'N';
        inicioParticion = mbr.mbr_partition_3.part_start;
        size = mbr.mbr_partition_3.part_size;
    }
    else  if(strcasecmp(mbr.mbr_partition_4.part_name,Name) == 0){
        memset(&mbr.mbr_partition_4,0,sizeof(PARTICION));
        mbr.mbr_partition_4.part_status = 'N';
        inicioParticion = mbr.mbr_partition_4.part_start;
        size = mbr.mbr_partition_4.part_size;
    }else{


        EBR primero = getEBR(Path);
        EBR ultimo;
        EBR aux = primero;
        EBR tmp;
        //captura del primero y ultimo ebr
        do{
            if(aux.part_next != -1){
                aux = getEBR(Path,aux.part_next);
            }else{
                ultimo = aux;
                break;
            }
        }while(true);


        aux = primero;
        do{
            if( strcasecmp(aux.part_name,Name) == 0 && strcasecmp(aux.part_name,primero.part_name) == 0 && aux.part_next == -1){
                //primer y unico ebr
                EBR ebr;
                ebr.part_fit = 'W';
                strcpy(ebr.part_name,"EBR_INICIAL");
                ebr.part_next = -1;                                             //como no hay siguiente ebr se coloca -1 si ubiera se coloca el byte donde inicia el siguiente ebr
                ebr.part_size = sizeof(EBR);
                ebr.part_start = aux.part_start;
                ebr.part_status = 'N';
                setEBR(ebr,Path,aux.part_start);
                return;
            }
            else if( strcasecmp(aux.part_name,Name) == 0 && aux.part_next != -1  && strcasecmp(aux.part_name,primero.part_name) == 0 ){
                //primer y unico ebr

                EBR ebr;
                ebr.part_fit = 'W';
                strcpy(ebr.part_name,"EBR_INICIAL");
                ebr.part_next = aux.part_next;                                             //como no hay siguiente ebr se coloca -1 si ubiera se coloca el byte donde inicia el siguiente ebr
                ebr.part_size = sizeof(EBR);
                ebr.part_start = aux.part_start;
                ebr.part_status = 'N';
                setEBR(ebr,Path,aux.part_start);
                return;
            }
            else if(strcasecmp(aux.part_name,Name) == 0 && aux.part_next != -1){
                //primer y unico ebr
                tmp.part_next = aux.part_next;
                setEBR(tmp,Path,tmp.part_start);

                FILE *file;
                file = fopen(Path,"rb+");
                if(file!=NULL)
                {
                    fseek(file, aux.part_start, SEEK_SET);
                    for(int r = 0; r < aux.part_size; r++){            //llenamos el archivo con nulos-> se debe cambiar a binario
                        fprintf(file, "%c", '\0');
                    }
                    fclose(file);
                    fflush(file);
                    printf("Particion %s Eliminada con exito.\n",Name);
                }
                return;
            }
            else if( strcasecmp(aux.part_name,Name) == 0 && aux.part_next == -1  && strcasecmp(aux.part_name,ultimo.part_name) == 0 ){
                //ultimo y unico ebr

                tmp.part_next = -1;
                setEBR(tmp,Path,tmp.part_start);
                return;

            }else if( strcasecmp(aux.part_name,Name) != 0 && aux.part_next == -1){
                //ultimo ebr y aun no encuentro la particion que deseo.
                break;

            }else{
                tmp = aux;
                aux = getEBR(Path,aux.part_next);
            }

        }while(true);

        printf("La particion especificada no existe en el disco.\n");
        return;
    }

    FILE *file;
    file = fopen(Path,"rb+");
    if(file!=NULL)
    {
        fseek(file,0,SEEK_SET);
        fwrite(&mbr,sizeof(MBR),1,file);

        //en el caso que sea un delete full
        if(strcasecmp(Name,"full") == 0){
            //nos posicionamos donde inicia la particion
            fseek(file, inicioParticion, SEEK_SET);
            for(int r = 0; r < size; r++){            //llenamos el archivo con nulos-> se debe cambiar a binario
                fprintf(file, "%c", '\0');
            }
        }
        fclose(file);
        fflush(file);
        printf("Particion %s Eliminada con exito.\n",Name);
    }
    else{
        printf("Error el archivo no existe\n");
        return;
    }
}

void AddParticion(char Path[250],char Nombre[50],int Add){

    MBR mbr;
    mbr = getMBR(Path);

    if(Add < 0){                                                                                //si add es negativo solo quitara espacio de las particiones.
        //hacer validadciones para que que no quede espacio negativo.
        if(strcasecmp(mbr.mbr_partition_1.part_name,Nombre) == 0){
            if(mbr.mbr_partition_1.part_size > -(Add)){
                mbr.mbr_partition_1.part_size = mbr.mbr_partition_1.part_size + Add;                //siempre sera mas Add porque si es Neg (+)(-) = (-) y si es (+)(+) = (+)
            }else{
                printf("No se puede quitar mas espacio a %s ya que no hay espacio suficiente.(-Add)\n",Nombre);
                return;
            }
        }else if(strcasecmp(mbr.mbr_partition_2.part_name,Nombre) == 0){
            if(mbr.mbr_partition_2.part_size > -(Add)){
                mbr.mbr_partition_2.part_size = mbr.mbr_partition_2.part_size + Add;                //siempre sera mas Add porque si es Neg (+)(-) = (-) y si es (+)(+) = (+)
            }else{
                printf("No se puede quitar mas espacio a %s ya que no hay espacio suficiente.(-Add)\n",Nombre);
                return;
            }
        }else if(strcasecmp(mbr.mbr_partition_3.part_name,Nombre) == 0){
            if(mbr.mbr_partition_3.part_size > -(Add)){
                mbr.mbr_partition_3.part_size = mbr.mbr_partition_3.part_size + Add;                //siempre sera mas Add porque si es Neg (+)(-) = (-) y si es (+)(+) = (+)
            }else{
                printf("No se puede quitar mas espacio a %s ya que no hay espacio suficiente.(-Add)\n",Nombre);
                return;
            }
        }else if(strcasecmp(mbr.mbr_partition_4.part_name,Nombre) == 0){
            if(mbr.mbr_partition_4.part_size > -(Add)){
                mbr.mbr_partition_4.part_size = mbr.mbr_partition_4.part_size + Add;                //siempre sera mas Add porque si es Neg (+)(-) = (-) y si es (+)(+) = (+)
            }else{
                printf("No se puede quitar mas espacio a %s ya que no hay espacio suficiente.(-Add)\n",Nombre);
                return;
            }
        }else{


            bool sepudo = false;
            if(mbr.mbr_partition_1.part_type == 'e' || mbr.mbr_partition_1.part_type == 'E'){
                sepudo = AddLogicas(Path,Nombre,Add,mbr.mbr_partition_1);
            }
            else if(mbr.mbr_partition_2.part_type == 'e' || mbr.mbr_partition_2.part_type == 'E'){
                sepudo = AddLogicas(Path,Nombre,Add,mbr.mbr_partition_2);
            }

            else if(mbr.mbr_partition_3.part_type == 'e' || mbr.mbr_partition_3.part_type == 'E'){
                sepudo =AddLogicas(Path,Nombre,Add,mbr.mbr_partition_3);
            }

            else if(mbr.mbr_partition_4.part_type == 'e' || mbr.mbr_partition_4.part_type == 'E'){
                sepudo =AddLogicas(Path,Nombre,Add,mbr.mbr_partition_4);
            }

            if(!sepudo){
                printf("Particion indicada no existe en el disco actual.\n");
                return;
            }
        }

        setMBR(mbr,Path);           //seteamos denuevo el mbr
        printf("Se a disminuido %d Bytes de %s.\n",(-Add),Nombre);
    }else{
        if(strcasecmp(mbr.mbr_partition_1.part_name,Nombre) == 0){

            if(mbr.mbr_partition_2.part_status == 'S'){
                /*--las dos particiones estan pegadas por lo tanto no agrega espacio*/
                if((mbr.mbr_partition_1.part_start + mbr.mbr_partition_1.part_size) == mbr.mbr_partition_2.part_start){
                    printf("No hay espacio disponible para agregar a la particion %s.\n",Nombre);
                    return;
                }else{          //existe espacio donjde termina la 1 y empieza la 2
                    int espaciodispo = mbr.mbr_partition_2.part_start - (mbr.mbr_partition_1.part_start + mbr.mbr_partition_1.part_size);
                    if(Add <= espaciodispo){
                        mbr.mbr_partition_1.part_size = mbr.mbr_partition_1.part_size + Add;
                    }else{
                        printf("No hay suficiente espacio disponible para agregar a la particion %s.\n",Nombre);
                        return;
                    }
                }
            }else{
                if(mbr.mbr_partition_3.part_status == 'S'){
                    /*--las dos particiones estan pegadas por lo tanto no agrega espacio*/
                    if((mbr.mbr_partition_1.part_start + mbr.mbr_partition_1.part_size) == mbr.mbr_partition_3.part_start){
                        printf("No hay espacio disponible para agregar a la particion %s.\n",Nombre);
                        return;
                    }else{          //existe espacio donjde termina la 1 y empieza la 2
                        int espaciodispo = mbr.mbr_partition_3.part_start - (mbr.mbr_partition_1.part_start + mbr.mbr_partition_1.part_size);
                        if(Add <= espaciodispo){
                            mbr.mbr_partition_1.part_size = mbr.mbr_partition_1.part_size + Add;
                        }else{
                            printf("No hay suficiente espacio disponible para agregar a la particion %s.\n",Nombre);
                            return;
                        }
                    }
                }else{
                    if(mbr.mbr_partition_4.part_status == 'S'){
                        /*--las dos particiones estan pegadas por lo tanto no agrega espacio*/
                        if((mbr.mbr_partition_1.part_start + mbr.mbr_partition_1.part_size) == mbr.mbr_partition_4.part_start){
                            printf("No hay espacio disponible para agregar a la particion %s.\n",Nombre);
                            return;
                        }else{          //existe espacio donjde termina la 1 y empieza la 2
                            int espaciodispo = mbr.mbr_partition_4.part_start - (mbr.mbr_partition_1.part_start + mbr.mbr_partition_1.part_size);
                            if(Add <= espaciodispo){
                                mbr.mbr_partition_1.part_size = mbr.mbr_partition_1.part_size + Add;
                            }else{
                                printf("No hay suficiente espacio disponible para agregar a la particion %s.\n",Nombre);
                                return;
                            }
                        }
                    }else{
                        /*--las dos particiones estan pegadas por lo tanto no agrega espacio*/
                        if((mbr.mbr_partition_1.part_start + mbr.mbr_partition_1.part_size) == mbr.mbr_tamao){
                            printf("No hay espacio disponible para agregar a la particion %s.\n",Nombre);
                            return;
                        }else{          //existe espacio donjde termina la 1 y empieza la 2
                            int espaciodispo = mbr.mbr_tamao - (mbr.mbr_partition_1.part_start + mbr.mbr_partition_1.part_size);
                            if(Add <= espaciodispo){
                                mbr.mbr_partition_1.part_size = mbr.mbr_partition_1.part_size + Add;
                            }else{
                                printf("No hay suficiente espacio disponible para agregar a la particion %s.\n",Nombre);
                                return;
                            }
                        }
                    }
                }
            }
        }else if(strcasecmp(mbr.mbr_partition_2.part_name,Nombre) == 0){

            if(mbr.mbr_partition_3.part_status == 'S'){
                /*--las dos particiones estan pegadas por lo tanto no agrega espacio*/
                if((mbr.mbr_partition_2.part_start + mbr.mbr_partition_2.part_size) == mbr.mbr_partition_3.part_start){
                    printf("No hay espacio disponible para agregar a la particion %s.\n",Nombre);
                    return;
                }else{          //existe espacio donjde termina la 1 y empieza la 2
                    int espaciodispo = mbr.mbr_partition_3.part_start - (mbr.mbr_partition_2.part_start + mbr.mbr_partition_2.part_size);
                    if(Add <= espaciodispo){
                        mbr.mbr_partition_2.part_size = mbr.mbr_partition_2.part_size + Add;
                    }else{
                        printf("No hay suficiente espacio disponible para agregar a la particion %s.\n",Nombre);
                        return;
                    }
                }
            }else{
                if(mbr.mbr_partition_4.part_status == 'S'){
                    /*--las dos particiones estan pegadas por lo tanto no agrega espacio*/
                    if((mbr.mbr_partition_2.part_start + mbr.mbr_partition_2.part_size) == mbr.mbr_partition_4.part_start){
                        printf("No hay espacio disponible para agregar a la particion %s.\n",Nombre);
                        return;
                    }else{          //existe espacio donjde termina la 1 y empieza la 2
                        int espaciodispo = mbr.mbr_partition_4.part_start - (mbr.mbr_partition_2.part_start + mbr.mbr_partition_2.part_size);
                        if(Add <= espaciodispo){
                            mbr.mbr_partition_2.part_size = mbr.mbr_partition_2.part_size + Add;
                        }else{
                            printf("No hay suficiente espacio disponible para agregar a la particion %s.\n",Nombre);
                            return;
                        }
                    }
                }else{
                    /*--las dos particiones estan pegadas por lo tanto no agrega espacio*/
                    if((mbr.mbr_partition_2.part_start + mbr.mbr_partition_2.part_size) == mbr.mbr_tamao){
                        printf("No hay espacio disponible para agregar a la particion %s.\n",Nombre);
                        return;
                    }else{          //existe espacio donjde termina la 1 y empieza la 2
                        int espaciodispo = mbr.mbr_tamao - (mbr.mbr_partition_2.part_start + mbr.mbr_partition_2.part_size);
                        if(Add <= espaciodispo){
                            mbr.mbr_partition_2.part_size = mbr.mbr_partition_2.part_size + Add;
                        }else{
                            printf("No hay suficiente espacio disponible para agregar a la particion %s.\n",Nombre);
                            return;
                        }
                    }
                }
            }
        }else if(strcasecmp(mbr.mbr_partition_3.part_name,Nombre) == 0){

            if(mbr.mbr_partition_4.part_status == 'S'){
                /*--las dos particiones estan pegadas por lo tanto no agrega espacio*/
                if((mbr.mbr_partition_3.part_start + mbr.mbr_partition_3.part_size) == mbr.mbr_partition_4.part_start){
                    printf("No hay espacio disponible para agregar a la particion %s.\n",Nombre);
                    return;
                }else{          //existe espacio donjde termina la 1 y empieza la 2
                    int espaciodispo = mbr.mbr_partition_4.part_start - (mbr.mbr_partition_3.part_start + mbr.mbr_partition_3.part_size);
                    if(Add <= espaciodispo){
                        mbr.mbr_partition_3.part_size = mbr.mbr_partition_3.part_size + Add;
                    }else{
                        printf("No hay suficiente espacio disponible para agregar a la particion %s.\n",Nombre);
                        return;
                    }
                }
            }else{
                /*--las dos particiones estan pegadas por lo tanto no agrega espacio*/
                if((mbr.mbr_partition_3.part_start + mbr.mbr_partition_3.part_size) == mbr.mbr_tamao){
                    printf("No hay espacio disponible para agregar a la particion %s.\n",Nombre);
                    return;
                }else{          //existe espacio donjde termina la 1 y empieza la 2
                    int espaciodispo = mbr.mbr_tamao - (mbr.mbr_partition_3.part_start + mbr.mbr_partition_3.part_size);
                    if(Add <= espaciodispo){
                        mbr.mbr_partition_3.part_size = mbr.mbr_partition_3.part_size + Add;
                    }else{
                        printf("No hay suficiente espacio disponible para agregar a la particion %s.\n",Nombre);
                        return;
                    }
                }
            }
        }else if(strcasecmp(mbr.mbr_partition_4.part_name,Nombre) == 0){

            if((mbr.mbr_partition_4.part_start + mbr.mbr_partition_4.part_size) == mbr.mbr_tamao){
                printf("No hay espacio disponible para agregar a la particion %s.\n",Nombre);
                return;
            }else{          //existe espacio donjde termina la 1 y empieza la 2
                int espaciodispo = mbr.mbr_tamao - (mbr.mbr_partition_4.part_start + mbr.mbr_partition_4.part_size);
                if(Add <= espaciodispo){
                    mbr.mbr_partition_4.part_size = mbr.mbr_partition_4.part_size + Add;
                }else{
                    printf("No hay suficiente espacio disponible para agregar a la particion %s.\n",Nombre);
                    return;
                }
            }
        }
        else{
            bool sepudo = false;
            if(mbr.mbr_partition_1.part_type == 'e' || mbr.mbr_partition_1.part_type == 'E'){
                sepudo = AddLogicas(Path,Nombre,Add,mbr.mbr_partition_1);
            }
            else if(mbr.mbr_partition_2.part_type == 'e' || mbr.mbr_partition_2.part_type == 'E'){
                sepudo = AddLogicas(Path,Nombre,Add,mbr.mbr_partition_2);
            }

            else if(mbr.mbr_partition_3.part_type == 'e' || mbr.mbr_partition_3.part_type == 'E'){
                sepudo =AddLogicas(Path,Nombre,Add,mbr.mbr_partition_3);
            }

            else if(mbr.mbr_partition_4.part_type == 'e' || mbr.mbr_partition_4.part_type == 'E'){
                sepudo =AddLogicas(Path,Nombre,Add,mbr.mbr_partition_4);
            }

            if(!sepudo){
                printf("Particion indicada no existe en el disco actual.\n");
                return;
            }

        }
        setMBR(mbr,Path);
        printf("Se agregaran %d bytes a la Particion: %s",Add,Nombre);
    }
}

bool AddLogicas(char Path[250],char Nombre[50],int add, PARTICION particion){

    EBR ebr = getEBR(Path);

    if(add > 0){
        do{
            if(strcasecmp(ebr.part_name,"EBR_INICIAL") == 0 && ebr.part_next == -1)
            {
                return false;
            }
            else if(strcasecmp(ebr.part_name,"EBR_INICIAL") == 0 && ebr.part_next != -1)
            {
                ebr = getEBR(Path,ebr.part_next);   //nos pasamos al ebr siguiente
            }else{
                if(ebr.part_next == -1)
                {//es el ultimo
                    if(strcasecmp(ebr.part_name,Nombre) == 0){
                        int espacio = (particion.part_start + particion.part_size) - (ebr.part_start+ebr.part_size);
                        if(espacio >= add){
                            ebr.part_size = ebr.part_size+add;
                        }else{
                            printf("No hay espacio suficiente para agregar. (logicas)\n");
                            return false;
                        }
                    }
                }else{
                    if(strcasecmp(ebr.part_name,Nombre) == 0){
                        int espacio = (ebr.part_next) - (ebr.part_start+ebr.part_size);
                        if(espacio >= add){
                            ebr.part_size = ebr.part_size+add;
                        }else{
                            printf("No hay espacio suficiente para agregar. (logicas)\n");
                        }
                    }else{
                        ebr = getEBR(Path,ebr.part_next);
                    }
                }
            }
        }while(true);

    }else{
        do{
            if(strcasecmp(ebr.part_name,"EBR_INICIAL") == 0 && ebr.part_next == -1)
            {
                return false;
            }
            else if(strcasecmp(ebr.part_name,"EBR_INICIAL") == 0 && ebr.part_next != -1)
            {
                ebr = getEBR(Path,ebr.part_next);   //nos pasamos al ebr siguiente
            }else{
                if(strcasecmp(ebr.part_name,Nombre) == 0){
                    if(ebr.part_size > add){
                        ebr.part_size = ebr.part_size+add;
                        setEBR(ebr,Path,ebr.part_start);
                        return true;
                    }else{
                        printf("No hay espacio suficiente para quitar. (logicas)\n");
                        return false;
                    }
                }else{
                    ebr = getEBR(Path,ebr.part_next);   //nos pasamos al ebr siguiente
                }
            }
        }while(true);
    }

    return true;
}

/*MONTADURAS *MontarParticion(char Path[250], char Name[100],int Ultimobitusadoicion,MONTADURAS *primero,MONTADURAS *ultimo){

    //primero1 = NULL;
    int contador = 1;
    bool mismodisco = false;
    char letramismodisco = '\0';

    MONTADURAS *aux = primero;
    while (aux != NULL) {
        if(strcasecmp(aux->Path,Path) == 0){
            contador++;
            mismodisco = true;
            letramismodisco = aux->Id[2];
        }
        aux = aux->siguiente;
    }

    MONTADURAS *Nuevonodo = new MONTADURAS();
    strcpy(Nuevonodo->Path,Path);
    strcpy(Nuevonodo->Name,Name);
    Nuevonodo->StartParticion = Ultimobitusadoicion;
    Nuevonodo->tipoF = 0;

    memset(&Nuevonodo->Id,'\0',sizeof(Nuevonodo->Id));

    for (int i = 0; i < 5; ++i) {
        if(i == 0){
            Nuevonodo->Id[i] = 'v';
        }
        else if(i == 1){
            Nuevonodo->Id[i] = 'd';
        }
        else if(i == 2){
            if(mismodisco){
                Nuevonodo->Id[i] = letramismodisco;
            }else{
                Nuevonodo->Id[i] = Letras[posicionabecedario];
                posicionabecedario++;
            }
        }
        else if(i == 3){
            char Numero[6];
            memset(&Numero,'\0',sizeof(Numero));
            sprintf(Numero,"%d",contador);
            strcat(Nuevonodo->Id,Numero);
        }
    }
    if(primero == NULL){
        primero = Nuevonodo;
        ultimo = Nuevonodo;
        primero->siguiente = NULL;
        ultimo->siguiente = NULL;
    }else{
        ultimo->siguiente = Nuevonodo;
        ultimo = Nuevonodo;
        ultimo->siguiente = NULL;
    }

    //mpresion de particiones montadas

    MONTADURAS *aux1 = primero;
    printf("\nParticiones Montadas.\n\n");
    while (aux1 != NULL) {
        printf("Id: %s\nPath: %s\nName: %s\n\n",aux1->Id,aux1->Path,aux1->Name);
        aux1 = aux1->siguiente;
    }

    printf("\nParticion Montada con nombre: %s\n",Nuevonodo->Id);
    return primero;
}
*/
/*MONTADURAS *DesmontarParticion(MONTADURAS *primero, char Id[7]){

    MONTADURAS *auxborrar ;
    MONTADURAS *anterior = NULL;

    auxborrar = primero;
    while ((auxborrar != NULL) && (strcasecmp(auxborrar->Id,Id) != 0)) {
        anterior = auxborrar;
        auxborrar = auxborrar->siguiente;
    }

    if(auxborrar == NULL){
        printf("Particion %s no ha sido montada.\n", Id);
    }else if(anterior == NULL){
        primero = primero->siguiente;
        free(auxborrar);
        printf("Particion %s desmontada.\n",Id);
    }else{
        anterior->siguiente = auxborrar->siguiente;
        free(auxborrar);
        printf("Particion %s desmontada.\n",Id);
    }
}
*/

bool ExisteLogica(char path[250],char name[15])
{
    EBR aux = getEBR(path);
    do{
        if(strcasecmp(aux.part_name,name) == 0){
            return true;
        }else{

            if(aux.part_next != -1){
                aux = getEBR(path,aux.part_next);
            }else{
                break;
            }
        }
    }while(true);

    return false;
}

void verificacionAjusteLogicas(int TamaoParticionaCrear, char path[250], char Fit, char name[15],PARTICION particion,int fitParticion){
    //recibira el ebr inicial
    ESPACIOSLOGICAS* pri = NULL;
    ESPACIOSLOGICAS* ulti = NULL;
    EBR ebr = getEBR(path);
    EBR tmp = ebr;
    //EBR anterior;
    //EBR siguiente;

    do{
        if(strcasecmp(ebr.part_name,"EBR_INICIAL") == 0  && ebr.part_next == -1){
            //no hay logicas
            int espacio = particion.part_size;      //tengo todo el espacio disponible.

            ESPACIOSLOGICAS *Nuevonodo = new ESPACIOSLOGICAS();
            Nuevonodo->ebraactual = ebr;
            Nuevonodo->tamadisponible = espacio;
            Nuevonodo->start = ebr.part_start;

            if(pri == NULL){
                pri = Nuevonodo;
                ulti = Nuevonodo;
                ulti->siguiente = NULL;
                ulti->siguiente = NULL;
            }else{
                ulti->siguiente = Nuevonodo;
                ulti = Nuevonodo;
                ulti->siguiente = NULL;
            }
            break;
        }
        else if(strcasecmp(ebr.part_name,"EBR_INICIAL") == 0  && ebr.part_next != -1){
            //no hay logicas
            if((  (ebr.part_next) - (ebr.part_start + ebr.part_size)) > 0){
                //existe un espacio disponible entre los dos ebr's
                int espacio = ebr.part_next - (ebr.part_start + ebr.part_size);

                ESPACIOSLOGICAS *Nuevonodo = new ESPACIOSLOGICAS();
                Nuevonodo->ebraactual = ebr;
                Nuevonodo->tamadisponible = espacio;
                Nuevonodo->start = ebr.part_start + ebr.part_size;      /// iniciara al teminar ese ebr

                if(pri == NULL){
                    pri = Nuevonodo;
                    ulti = Nuevonodo;
                    ulti->siguiente = NULL;
                    ulti->siguiente = NULL;
                }else{
                    ulti->siguiente = Nuevonodo;
                    ulti = Nuevonodo;
                    ulti->siguiente = NULL;
                }
            }

            tmp = ebr;
            ebr = getEBR(path,ebr.part_next);
        }


        else if(ebr.part_next != -1){

            if(( (ebr.part_next) - (ebr.part_start + ebr.part_size)) > 0){
                //existe un espacio disponible entre los dos ebr's
                int espacio = ebr.part_next - (ebr.part_start + ebr.part_size);

                ESPACIOSLOGICAS *Nuevonodo = new ESPACIOSLOGICAS();
                Nuevonodo->ebraactual = ebr;
                Nuevonodo->tamadisponible = espacio;
                Nuevonodo->start = ebr.part_start + ebr.part_size;      /// iniciara al teminar ese ebr

                if(pri == NULL){
                    pri = Nuevonodo;
                    ulti = Nuevonodo;
                    ulti->siguiente = NULL;
                    ulti->siguiente = NULL;
                }else{
                    ulti->siguiente = Nuevonodo;
                    ulti = Nuevonodo;
                    ulti->siguiente = NULL;
                }
            }

            tmp = ebr;
            ebr = getEBR(path,ebr.part_next);

        }else{

            if(( (particion.part_start +particion.part_size) - (ebr.part_start + ebr.part_size) ) > 0){
                //existe un espacio disponible
                int espacio = (particion.part_start +particion.part_size) - (ebr.part_start + ebr.part_size);

                ESPACIOSLOGICAS *Nuevonodo = new ESPACIOSLOGICAS();
                Nuevonodo->ebraactual = ebr;
                Nuevonodo->tamadisponible = espacio;
                Nuevonodo->start = ebr.part_start  + ebr.part_size;

                if(pri == NULL){
                    pri = Nuevonodo;
                    ulti = Nuevonodo;
                    ulti->siguiente = NULL;
                    ulti->siguiente = NULL;
                }else{
                    ulti->siguiente = Nuevonodo;
                    ulti = Nuevonodo;
                    ulti->siguiente = NULL;
                }
            }
            break;
        }
    }while(true);


    //--verificacion de cual espacio se adecua segun el ajuste--
    ESPACIOSLOGICAS *aux = pri;
    ESPACIOSLOGICAS *ACTUAL;
    bool primero = true;
    int restante =0;
    int res  = 0;
    bool sicabeenalgunespacio = false;
    int Ultimobitusado = 0;

    if(aux != NULL){
        while (aux != NULL){
            if(aux->tamadisponible > TamaoParticionaCrear){
                sicabeenalgunespacio = true;
                break;
            }
            aux = aux->siguiente;
        }
        if(sicabeenalgunespacio){            //validacion para que entre en cualquier espacio disponible por ejemplo si tengo dos
            //espacios debera caber como minimo en laguno de los dos de lo contrario lanzara excepcion

            // aux = pri;
            while (aux != NULL){
                if(aux->tamadisponible > TamaoParticionaCrear){
                    res  = aux->tamadisponible - TamaoParticionaCrear;
                    //restante = 0;
                    if(primero){            //primeravez que viene.
                        if(fitParticion == 'f' || fitParticion == 'F'){
                            if(aux->tamadisponible > TamaoParticionaCrear){
                                ACTUAL = aux;
                                break;
                            }
                        }
                        else if(fitParticion == 'b' || fitParticion == 'B'){
                            if(aux->tamadisponible > TamaoParticionaCrear){
                                ACTUAL = aux;
                            }
                        }
                        else if(fitParticion == 'w' || fitParticion == 'W'){
                            if(aux->tamadisponible > TamaoParticionaCrear){
                                ACTUAL = aux;
                            }
                        }

                        restante = res;
                        Ultimobitusado = aux->start;
                        ACTUAL = aux;
                        primero = false;

                    }else{
                        if(fitParticion == 'b' || fitParticion == 'B'){
                            if(res < restante){
                                restante = res;
                                Ultimobitusado = aux->start;
                                ACTUAL = aux;
                            }
                        }
                        else if(fitParticion == 'w' || fitParticion == 'W'){
                            if(res > restante){
                                restante = res;
                                Ultimobitusado = aux->start;
                                ACTUAL = aux;
                            }
                        } else if(fitParticion == 'f' || fitParticion == 'F'){
                            if(aux->tamadisponible > TamaoParticionaCrear){
                                restante = res;
                                Ultimobitusado = aux->start;
                                ACTUAL = aux;
                                break;
                            }
                        }
                    }
                }
                aux = aux->siguiente;
            }
            //posicion donde deberia insertarse la nueva particion
            ////////////////////////////////////////////////////////////////////////////
            ///////////////////////////////////////////////////////////////////////////
            /// /*cout << "Particion Logica.";
            //verificamos que exista alguna particion extendida

            ebr = ACTUAL->ebraactual;  //SETEAMOS EL EBR QUE VAMOS A INGRESAR

            if(ebr.part_next == -1 && strcasecmp(ebr.part_name,"EBR_INICIAL") == 0){
                //NO HAY PARTICIONES LOGICAS ACTUALMENTE
                //solo actualizo los datos
                ebr.part_fit = Fit;
                memset(&ebr.part_name,'\0',sizeof(ebr.part_name));
                strcpy(ebr.part_name,name);
                ebr.part_next = -1;
                ebr.part_size = TamaoParticionaCrear;
                //ebr.part_start = inicio de la particion
                ebr.part_status = 'S';

                setEBR(ebr,path,ebr.part_start);    //seteamos el ebr actualizado

            }
            else if(ebr.part_next != -1 && strcasecmp(ebr.part_name,"EBR_INICIAL") == 0){
                //NO HAY PARTICIONES LOGICAS ACTUALMENTE
                //solo actualizo los datos
                ebr.part_fit = Fit;
                memset(&ebr.part_name,'\0',sizeof(ebr.part_name));
                strcpy(ebr.part_name,name);
                //ebr.part_next = -1;
                ebr.part_size = TamaoParticionaCrear;
                //ebr.part_start = inicio de la particion
                ebr.part_status = 'S';

                setEBR(ebr,path,ebr.part_start);    //seteamos el ebr actualizado

            }else{
                //YA EXISTE POR LO MENOS UNA PARTICION LOGICA


                if(ebr.part_next == -1){
                    //actualizamos el apuntador del ebr anterior
                    ebr.part_next = ebr.part_start + ebr.part_size;
                    setEBR(ebr,path,ebr.part_start);

                    //creamos el nuevo ebr
                    EBR new_ebr;
                    new_ebr.part_fit = Fit;
                    memset(&new_ebr.part_name,'\0',sizeof(new_ebr.part_name));
                    strcpy(new_ebr.part_name,name);
                    new_ebr.part_next = -1;
                    new_ebr.part_size = TamaoParticionaCrear;
                    new_ebr.part_start = ebr.part_next;      //inicia la particion donde marca el apuntador next
                    new_ebr.part_status = 'S';

                    setEBR(new_ebr,path,ebr.part_next);      //escribimos el nuevo ebr
                    //break;
                }else{

                    //creamos el nuevo ebr
                    EBR new_ebr;
                    new_ebr.part_fit = Fit;
                    memset(&new_ebr.part_name,'\0',sizeof(new_ebr.part_name));
                    strcpy(new_ebr.part_name,name);
                    new_ebr.part_next = ebr.part_next;                         //hacemos que apunte a lo que apuntaba el actual
                    new_ebr.part_size = TamaoParticionaCrear;
                    new_ebr.part_start = ebr.part_start + ebr.part_size;      //inicia la particion donde marca el apuntador next
                    new_ebr.part_status = 'S';

                    setEBR(new_ebr,path,new_ebr.part_start);      //escribimos el nuevo ebr

                    ebr.part_next = new_ebr.part_start;
                    setEBR(ebr,path,ebr.part_start);

                }
            }

            /////////////////////////////////////////////////////////////////////////////////
            /////////////////////////////////////////////////////////////////////////////////

        }else{
            printf("La particion que desea crear supera en tama~o\na los espacios disponibles en el disco.\n");
        }
    }else{
        printf("No se encontro ningun espacio disponible para crear la particion.\n");
    }
}
