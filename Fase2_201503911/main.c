#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <dirent.h>
//#include <conio.c>

/*----------------------------------------------------------------ESTRUCTURAS FASE 1------------------------------------------------------------------*/

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

/*--EXTENDED BOOT RECORD--*/
typedef struct {
    char part_status;
    int part_next;
    char part_fit;
    int part_start;
    int part_size;
    char part_name[16];

}EBR;

/*------MONTAR PARTICIONES--------*/  /*Lista en la cual tendre las particiones montadas.*/
typedef struct{
    char Id[8];             //vda1__
    char Path[250];
    char Name[100];
    struct MONTADURAS* siguiente;
    int StartParticion;
    int tipoF;
}MONTADURAS;

MONTADURAS* primero = NULL;
MONTADURAS* ultimo = NULL;

/*------ESPACIOS DICPONIBLES --------*/ /*Capturare los espacios disponibles en la particion para aplicar ajustes B,F,W.*/
typedef struct{
    int tamadisponible;
    int numeroparticion;
    int start;
    struct MONTADURAS* siguiente;
}ESPVACIOS  ;

ESPVACIOS* pri = NULL;
ESPVACIOS* ulti = NULL;

/*----------------------------------------------------------------FIN ESTRUCTURAS FASE 1------------------------------------------------------------------*/

/*----------------------------------------------------------------ESTRUCTURAS FASE 2------------------------------------------------------------------*/

typedef struct{
    int s_filesystem_type;
    int s_inodes_count;
    int s_blocks_count;
    int s_free_blocks_count;
    int s_free_inodes_count;
    timer_t s_mtime;
    timer_t s_umtime;
    int s_mnt_count;
    int s_magic;
    int s_inode_size;
    int s_block_size;
    int s_first_ino;
    int s_first_blo;
    int s_bm_inode_start;
    int s_bm_block_start;
    int s_inode_start;
    int s_block_start;

}SUPERBLOQUE;

typedef struct{
    int i_uid;
    int i_gid;
    int i_size;
    timer_t i_atime;
    timer_t i_ctime;
    timer_t i_mtime;
    int i_block[15];
    char i_type;
    int i_perm;

}TABLAINODOS;

typedef struct{
    char b_name[12];
    int b_inodo;
}CONTENTCARPETA;

typedef struct{
    CONTENTCARPETA Content[4];
}BLOQUECARPETA;

typedef struct{
    char b_content[64];
}BLOQUEARCHIVO;

typedef struct{
    int b_pointers[16];
}BLOQUEAPUNTADORES;

typedef struct{
    char Usuario[15];
    char pwd[15];
    char id[8];         //id de la particion montada que esta utilizando.
    char Path[250];     //path del disco
    char Name[100];     //NOMBRE DE LA PARTICION
    int StartParticion;
    int Tipo;           //tipo de formateo que tiene esta particion a la que esta logueada
    int IDUsuario;
}USUARIOLOGUEADO;

USUARIOLOGUEADO UsuarioLog;

typedef struct{
    int numero;             //vda1__
    char contenido[500];
    struct ARCHIVOS* siguiente;
}ARCHIVOS;

ARCHIVOS* primeroA = NULL;
ARCHIVOS* ultimoA = NULL;

typedef struct{
    int Journal_Tipo_Operacion;     //si es 0 es porque es nulo
    int Jorunal_Tipo;
    char Journal_Nombre[20];
    char Journal_Contenido[150];
    timer_t Jorunal_Fecha;
    int Journal_Propietario;
    int Journal_Permisos;
}JOURNALING;


/*----------------------------------------------------------------FIN ESTRUCTURAS FASE 2------------------------------------------------------------------*/

/*----------------------------------------------------------------DECLARACION DE METODOS-----------------------------------------------------------*/
char* getUnit(char comando[500],int i);
char* getFit(char comando[500], int i);
char* getPath(char comando[500], int i);
char *getFile(char comando[500],int i);
char* getUnit(char comando[500], int i);
char *getType(char coma[500],int i);
char *getName(char coma[500],int i);
char *getDelete(char coma[500],int i);
int getAdd(char coma[500],int i);
bool Exec(char coma[500]);

MBR getMBR(char Path[250]);
void setMBR(MBR mbr,char Path[200]);
EBR getEBR(char Path[250]);
bool VerficacionPath(char Path[250]);
bool SepararRuta(char RUTA[250]);
void Analisis(char comando[500]);
void mkdisk(char comando[500]);
void rmdisk(char comando[500]);
void fdisk (char comando[500]);
void CrearParticionFDISK(int Size, char Unit, char Path[250],char Type, char Fit, char Name[100]);
int VerificacionAjuste(char Path[250], int TamaoParticionaCrear);
void CrearParticion(char Type, char Fit, int Size, char Name[100], char Path[250],int NumeroParticion,int inicio);
void EliminarParticion(char Path[250], char Name[15]);
void Unmount(char coma[500]);
void Moun(char comando[500]);
void MontarParticion(char Path[250], char Name[100],int StartParticion);
void Rep(char coma[500]);
void ReporteMBR(char Id[9],char Path[250],char Name[10],char Ruta[250]);
void Verparticiones(char path[250]);
void Agregar(char Path[250],char Nombre[50],int Add);
char *BuscarArchivo(char RUTA[250]);

/*FASE2*/
int DespejandoN(int TamaoParticion);
int getPrimerBloqueLibre(char PathDisco[250],int StartBitmap);
int getPrimerInodoLibre(char PathDisco[250], int StartBitmap);
void setBINODO(char PathDisco[250], int StarBM, char Bitmap[]);
void setBBLOQUE(char PathDisco[250], int StarBM, char Bitmap[]);
char *getBIBLOQUE(char PathDisco[250],int Start);
char *getBIINODO(char PathDisco[250],int Start);
char *getArchivo(char PathDisco[250],int Posicion);
TABLAINODOS getInodo(char PathDisco[250],int starInodo);
TABLAINODOS VerificacionExisteCarpeta(TABLAINODOS inodoPadre,char Nombre[25]);
BLOQUECARPETA getBloqueCarpeta(int Posicion);
void SeparaRutaMkfile(char RUTA[250],char contenido[500]);
SUPERBLOQUE getSB(char Path[250],int start);
TABLAINODOS CrearCarpeta(TABLAINODOS inodoPadre, char NombreCarpeta[25]);
TABLAINODOS CrearArchivo(TABLAINODOS inodoPadre, char NombreArchivo[25], char contenido[500]);
void Generardot(TABLAINODOS padre, FILE *dot, SUPERBLOQUE sb);
TABLAINODOS SeparaRutaMOVER(char RUTA[250]);
void CrearCarpetaMover(TABLAINODOS inodoPadre, int NumeroInodoHijo, char NombreInodoHijo[20]);
TABLAINODOS SeparaRutaDestinoMOVER(char RUTA[250]);
TABLAINODOS SeparaRutaREMOVER(char RUTA[250]);
BLOQUEARCHIVO getBloqueArchivo(int Posicion,char Path[250]);
JOURNALING getJOURNALING(char Path[200],int start);
int getUgo(char comando[500],int i);
TABLAINODOS SeparaRutaCHMOD(char RUTA[250]);
/*-------------------------------------------------------------FIN DECLARACION DE METODOS-------------------------------------------------------------------*/

/*-----------------------------------------------------------------VARIABLES GLOBALES--------------------------------------------------------------*/
bool Bandera_Esto_En_Recovery = false;
int ID_TREE = 0;
int ID_TREE_PADRE  = 0;
int indc = 0;
int PrimerJournalingLibre = 0;          //variable que me servira para moverme entre los jorunaling.
bool indicador_eliminacio_ndisco = false;
int NummeroPaticion = 0;

//variables para montar particiones
int posicionabecedario = 0;
char Letras[26] = {'a','b','c','d','e','f','g','h','i','j','k','l','m',
                   'n','o','p','q','r','s','t','u','v','w','x','y','z'};

int NumeroStructuras = 0;           //numero de estructuras inodos que se podran almacenar.
int LimiteBloques = 4380;
int CapacidadCarpetas = 17518;
int Bytesmaximosdearchivo = 280320; //bytes
bool ParametroP = false;        //parametro usado en mkdir
bool ParametroR = false;        //parametro usado en chmod


/*------------------------------------------------------------------METODOS FASE 1-----------------------------------------------------------------*/
bool VerficacionPath(char Path[250]){
    char *ruta = Path;
    int indice=0;
    while(*ruta != '\0'){
        ruta++;
        indice++;
    }
    DIR *PathDestino;

    if((PathDestino=opendir(Path))==NULL ){      //si devuelve null la carpeta si existe y no hay necesidad de crearla

        if(!indicador_eliminacio_ndisco){   //este indicador me servira para saber si se esta consultado la ruta para mkdisk la cual debe creala si no existe
            //o para el rmdisk que solo es consulta y devuelde un false de no existir

            if(mkdir(Path,0777) == 0){      //creamo la carpeta con permisos de totales
                //printf("Se ha creado la carpeta %s\n",Path);
                return true;
            }
            else {
                printf("Error al crear carpeta.\n");
                return false;
            }

        }else{
            return false;
        }
    }else{
        return true;
    }
}

bool SepararRuta(char RUTA[250]){
    bool para = false;
    bool pathcorrcta = false;
    char Pataux[250];
    memset(&Pataux,'\0', sizeof(Pataux));

    int indice = 1;
    for (int i = 0; i < 250; i++) {
        if(RUTA[0] == '/' && i == 0){
            Pataux[0]='/';
            i++;
        }else{
            if(RUTA[0] != '/' && i == 0){
                Pataux[0] = '/';
            }
        }

        while (RUTA[i] != '/' || i == 0) {
            Pataux[indice] = RUTA[i];
            i++;
            indice++;
            if(RUTA[i] == '.'){
                memset(&Pataux,'\0',sizeof(Pataux));
                para = true;
                break;
            }
        }
        Pataux[indice++] = '/';
        if(!para){
            pathcorrcta = VerficacionPath(Pataux);
            para = false;
        }else{
            break;
        }
    }
    return pathcorrcta;
}

void Analisis(char comando[500]){


    //quito los \r que pudiera tener
    for (int i = 0; i < 500; i++) {
        if(comando[i] == '\r'){
            comando[i] = '\n';
            comando[i+1] = '\0';
        }
    }
    //captura del comando de entrada

    /*--quitar los espacios en blanco al inicio--*/
    int i = 0;
    while (comando[i] == ' ') {
        i++;
    }
    /*-------------------------------------------*/

    if(comando[i] != '#'){
        char palabra[9];
        memset(&palabra,'\0',sizeof(palabra));
        int j = 0;
        for(i = i; i < 500; i++){
            if(comando[i] != ' ' && comando[i] != '\0' && comando[i] != '\n'){
                palabra[j] = comando[i];
                j++;
            }else{
                break;
            }
        }
        if(strcasecmp(palabra,"mkdisk") == 0){
            //printf("comando ingresado mkdisk.\n");
            mkdisk(comando);
        }
        else if(strcasecmp(palabra,"mkfs") == 0){
            //printf("comando ingresado mkfs.\n");
            Mkfs(comando);
        }
        else if(strcasecmp(palabra,"mount") == 0){
            //printf("comando ingresado mount.\n");
            Moun(comando);
        }
        else if(strcasecmp(palabra,"unmount") == 0){
            //printf("comando ingresado unmount.\n");
            Unmount(comando);
        }
        else if(strcasecmp(palabra,"fdisk") == 0){
            //printf("comando ingresado fdisk.\n");
            fdisk(comando);
        }
        else if(strcasecmp(palabra,"rep") == 0){
            //printf("comando ingresado rep.\n");

            if((strcasecmp(UsuarioLog.Usuario,"\0") != 0) && (strcasecmp(UsuarioLog.pwd,"\0") != 0) &&(strcasecmp(UsuarioLog.id,"\0") != 0)){

                if(UsuarioLog.Tipo == 3 && Bandera_Esto_En_Recovery == false){
                    JOURNALING nuevo;

                    nuevo.Journal_Tipo_Operacion = 6;
                    nuevo.Jorunal_Tipo = 0;

                    memset(&nuevo.Journal_Nombre,'\0',sizeof(nuevo.Journal_Nombre));
                    strcpy(nuevo.Journal_Nombre,"Rep");

                    memset(&nuevo.Journal_Contenido,'\0',sizeof(nuevo.Journal_Contenido));
                    strcpy(nuevo.Journal_Contenido,comando);

                    nuevo.Jorunal_Fecha = time(0);
                    nuevo.Journal_Propietario = 1;
                    nuevo.Journal_Permisos = 664;

                    SUPERBLOQUE sb;
                    sb = getSB(UsuarioLog.Path,UsuarioLog.StartParticion);
                    //seguido del super bloque empezaran los journaling
                    //siempre y cuando sea paraticion en ext3

                    setJOURNALING(nuevo,UsuarioLog.Path,UsuarioLog.StartParticion +sizeof(SUPERBLOQUE) + (PrimerJournalingLibre * sizeof(JOURNALING)));
                    PrimerJournalingLibre++;

                }
                Rep(comando);
            }else{
                printf("Error no hay sesion activa (Root).\n");
            }

        }
        else if(strcasecmp(palabra,"rmdisk") == 0){
            //printf("comando ingresado rmdisk.\n");
            rmdisk(comando);
        }
        else if(strcasecmp(palabra,"exec") == 0){
            //printf("comando ingresado exec.\n");
            Exec(comando);
        }else if(strcasecmp(palabra,"login") == 0){
            //printf("comando ingresado login.\n");
            Login(comando);
        }else if(strcasecmp(palabra,"logout") == 0){
            //printf("comando ingresado logout.\n");
            if((strcasecmp(UsuarioLog.Usuario,"\0") != 0) && (strcasecmp(UsuarioLog.pwd,"\0") != 0) &&(strcasecmp(UsuarioLog.id,"\0") != 0)){
                if(UsuarioLog.Tipo == 3 && Bandera_Esto_En_Recovery == false){
                    JOURNALING nuevo;

                    nuevo.Journal_Tipo_Operacion = 7;
                    nuevo.Jorunal_Tipo = 0;

                    memset(&nuevo.Journal_Nombre,'\0',sizeof(nuevo.Journal_Nombre));
                    strcpy(nuevo.Journal_Nombre,"Logout");

                    memset(&nuevo.Journal_Contenido,'\0',sizeof(nuevo.Journal_Contenido));
                    strcpy(nuevo.Journal_Contenido,comando);

                    nuevo.Jorunal_Fecha = time(0);
                    nuevo.Journal_Propietario = 1;
                    nuevo.Journal_Permisos = 664;

                    SUPERBLOQUE sb;
                    sb = getSB(UsuarioLog.Path,UsuarioLog.StartParticion);
                    //seguido del super bloque empezaran los journaling
                    //siempre y cuando sea paraticion en ext3

                    setJOURNALING(nuevo,UsuarioLog.Path,UsuarioLog.StartParticion +sizeof(SUPERBLOQUE) + (PrimerJournalingLibre * sizeof(JOURNALING)));
                    PrimerJournalingLibre++;

                }

                Logout();
            }else{
                printf("Error no hay sesion activa (Root).\n");
            }
        }else if(strcasecmp(palabra,"mkgrp") == 0){
            //printf("comando ingresado logout.\n");
            if((strcasecmp(UsuarioLog.Usuario,"\0") != 0) && (strcasecmp(UsuarioLog.pwd,"\0") != 0) &&(strcasecmp(UsuarioLog.id,"\0") != 0)){
                Mkgrp(comando);
            }else{
                printf("Error no hay sesion activa (Root).\n");
            }
        }
        else if(strcasecmp(palabra,"rmgrp") == 0){
            //printf("comando ingresado logout.\n");
            if((strcasecmp(UsuarioLog.Usuario,"\0") != 0) && (strcasecmp(UsuarioLog.pwd,"\0") != 0) &&(strcasecmp(UsuarioLog.id,"\0") != 0)){
                Rmgrp(comando);
            }else{
                printf("Error no hay sesion activa (Root).\n");
            }
        }else if(strcasecmp(palabra,"mkusr") == 0){
            //printf("comando ingresado logout.\n");
            if((strcasecmp(UsuarioLog.Usuario,"\0") != 0) && (strcasecmp(UsuarioLog.pwd,"\0") != 0) &&(strcasecmp(UsuarioLog.id,"\0") != 0)){
                Mkusr(comando);
            }else{
                printf("Error no hay sesion activa (Root).\n");
            }
        }
        else if(strcasecmp(palabra,"rmusr") == 0){
            //printf("comando ingresado logout.\n");
            if((strcasecmp(UsuarioLog.Usuario,"\0") != 0) && (strcasecmp(UsuarioLog.pwd,"\0") != 0) &&(strcasecmp(UsuarioLog.id,"\0") != 0)){
                Rmusr(comando);
            }else{
                printf("Error no hay sesion activa (Root).\n");
            }
        }else if(strcasecmp(palabra,"mkdir") == 0){
            //printexec -path="/home/osuna/Escritorio/Archivos/Primer Semestre 2019/Fase 2/EntradaF2.sh"f("comando ingresado logout.\n");
            if((strcasecmp(UsuarioLog.Usuario,"\0") != 0) && (strcasecmp(UsuarioLog.pwd,"\0") != 0) &&(strcasecmp(UsuarioLog.id,"\0") != 0)){
                Mkdir(comando);
                if(UsuarioLog.Tipo == 3 && Bandera_Esto_En_Recovery == false){
                    JOURNALING nuevo;

                    nuevo.Journal_Tipo_Operacion = 8;
                    nuevo.Jorunal_Tipo = 0;

                    memset(&nuevo.Journal_Nombre,'\0',sizeof(nuevo.Journal_Nombre));
                    strcpy(nuevo.Journal_Nombre,"Mkdir");

                    memset(&nuevo.Journal_Contenido,'\0',sizeof(nuevo.Journal_Contenido));
                    strcpy(nuevo.Journal_Contenido,comando);

                    nuevo.Jorunal_Fecha = time(0);
                    nuevo.Journal_Propietario = 1;
                    nuevo.Journal_Permisos = 664;

                    SUPERBLOQUE sb;
                    sb = getSB(UsuarioLog.Path,UsuarioLog.StartParticion);
                    //seguido del super bloque empezaran los journaling
                    //siempre y cuando sea paraticion en ext3

                    setJOURNALING(nuevo,UsuarioLog.Path,UsuarioLog.StartParticion +sizeof(SUPERBLOQUE) + (PrimerJournalingLibre * sizeof(JOURNALING)));
                    PrimerJournalingLibre++;

                }
            }else{
                printf("Error no hay sesion activa.\n");
            }
        }
        else if(strcasecmp(palabra,"mkfile") == 0){
            //printf("comando ingresado mkfile.\n");
            if((strcasecmp(UsuarioLog.Usuario,"\0") != 0) && (strcasecmp(UsuarioLog.pwd,"\0") != 0) &&(strcasecmp(UsuarioLog.id,"\0") != 0)){
                Mkfile(comando);
                if(UsuarioLog.Tipo == 3 && Bandera_Esto_En_Recovery == false){
                    JOURNALING nuevo;

                    nuevo.Journal_Tipo_Operacion = 9;
                    nuevo.Jorunal_Tipo = 0;

                    memset(&nuevo.Journal_Nombre,'\0',sizeof(nuevo.Journal_Nombre));
                    strcpy(nuevo.Journal_Nombre,"Mkfile");

                    memset(&nuevo.Journal_Contenido,'\0',sizeof(nuevo.Journal_Contenido));
                    strcpy(nuevo.Journal_Contenido,comando);

                    nuevo.Jorunal_Fecha = time(0);
                    nuevo.Journal_Propietario = 1;
                    nuevo.Journal_Permisos = 664;

                    SUPERBLOQUE sb;
                    sb = getSB(UsuarioLog.Path,UsuarioLog.StartParticion);
                    //seguido del super bloque empezaran los journaling
                    //siempre y cuando sea paraticion en ext3

                    setJOURNALING(nuevo,UsuarioLog.Path,UsuarioLog.StartParticion +sizeof(SUPERBLOQUE) + (PrimerJournalingLibre * sizeof(JOURNALING)));
                    PrimerJournalingLibre++;

                }
            }else{
                printf("Error no hay sesion activa.\n");
            }
        }
        else if(strcasecmp(palabra,"find") == 0){
            //printf("comando ingresado find.\n");
            if((strcasecmp(UsuarioLog.Usuario,"\0") != 0) && (strcasecmp(UsuarioLog.pwd,"\0") != 0) &&(strcasecmp(UsuarioLog.id,"\0") != 0)){
                Find(comando);
                if(UsuarioLog.Tipo == 3 && Bandera_Esto_En_Recovery == false){
                    JOURNALING nuevo;

                    nuevo.Journal_Tipo_Operacion = 10;
                    nuevo.Jorunal_Tipo = 0;

                    memset(&nuevo.Journal_Nombre,'\0',sizeof(nuevo.Journal_Nombre));
                    strcpy(nuevo.Journal_Nombre,"Find");

                    memset(&nuevo.Journal_Contenido,'\0',sizeof(nuevo.Journal_Contenido));
                    strcpy(nuevo.Journal_Contenido,comando);

                    nuevo.Jorunal_Fecha = time(0);
                    nuevo.Journal_Propietario = 1;
                    nuevo.Journal_Permisos = 664;

                    SUPERBLOQUE sb;
                    sb = getSB(UsuarioLog.Path,UsuarioLog.StartParticion);
                    //seguido del super bloque empezaran los journaling
                    //siempre y cuando sea paraticion en ext3

                    setJOURNALING(nuevo,UsuarioLog.Path,UsuarioLog.StartParticion +sizeof(SUPERBLOQUE) + (PrimerJournalingLibre * sizeof(JOURNALING)));
                    PrimerJournalingLibre++;

                }
            }else{
                printf("Error no hay sesion activa.\n");
            }
        }
        else if(strcasecmp(palabra,"lsusers") == 0){
            if((strcasecmp(UsuarioLog.Usuario,"\0") != 0) && (strcasecmp(UsuarioLog.pwd,"\0") != 0) &&(strcasecmp(UsuarioLog.id,"\0") != 0)){
                LsUsers();
            }else{
                printf("Error no hay sesion activa (Root).\n");
            }
        }else if(strcasecmp(palabra,"pause") == 0){
            char letra = '\0';

            printf("Sistema Pausado,Presiona culaquier tecla para continuar.\n");
            scanf("%c",&letra);

            if(UsuarioLog.Tipo == 3 && Bandera_Esto_En_Recovery == false){
                JOURNALING nuevo;

                nuevo.Journal_Tipo_Operacion = 11;
                nuevo.Jorunal_Tipo = 0;

                memset(&nuevo.Journal_Nombre,'\0',sizeof(nuevo.Journal_Nombre));
                strcpy(nuevo.Journal_Nombre,"Pause");

                memset(&nuevo.Journal_Contenido,'\0',sizeof(nuevo.Journal_Contenido));
                strcpy(nuevo.Journal_Contenido,comando);

                nuevo.Jorunal_Fecha = time(0);
                nuevo.Journal_Propietario = 1;
                nuevo.Journal_Permisos = 664;

                SUPERBLOQUE sb;
                sb = getSB(UsuarioLog.Path,UsuarioLog.StartParticion);
                //seguido del super bloque empezaran los journaling
                //siempre y cuando sea paraticion en ext3

                setJOURNALING(nuevo,UsuarioLog.Path,UsuarioLog.StartParticion +sizeof(SUPERBLOQUE) + (PrimerJournalingLibre * sizeof(JOURNALING)));
                PrimerJournalingLibre++;

            }
            return;

        }else if(strcasecmp(palabra,"loss") == 0){

            Loss(comando);
            //loss no lo agrego al journaling porque cuando haga un recovery se enciclaria porque dentro del mismo
            //archvio vendria otro loss y otro recovery y asi se quedaria enciclado..

            /*if(UsuarioLog.Tipo == 3){
                JOURNALING nuevo;

                nuevo.Journal_Tipo_Operacion = 12;
                nuevo.Jorunal_Tipo = 0;

                memset(&nuevo.Journal_Nombre,'\0',sizeof(nuevo.Journal_Nombre));
                strcpy(nuevo.Journal_Nombre,"Loss");

                memset(&nuevo.Journal_Contenido,'\0',sizeof(nuevo.Journal_Contenido));
                strcpy(nuevo.Journal_Contenido,comando);

                nuevo.Jorunal_Fecha = time(0);
                nuevo.Journal_Propietario = 1;
                nuevo.Journal_Permisos = 664;

                SUPERBLOQUE sb;
                sb = getSB(UsuarioLog.Path,UsuarioLog.StartParticion);
                //seguido del super bloque empezaran los journaling
                //siempre y cuando sea paraticion en ext3

                setJOURNALING(nuevo,UsuarioLog.Path,UsuarioLog.StartParticion +sizeof(SUPERBLOQUE) + (PrimerJournalingLibre * sizeof(JOURNALING)));
                PrimerJournalingLibre++;

            }
            */
            return;

        }else if(strcasecmp(palabra,"cat") == 0){
            if((strcasecmp(UsuarioLog.Usuario,"\0") != 0) && (strcasecmp(UsuarioLog.pwd,"\0") != 0) &&(strcasecmp(UsuarioLog.id,"\0") != 0)){
                Cat(comando);
                if(UsuarioLog.Tipo == 3 && Bandera_Esto_En_Recovery == false){
                    JOURNALING nuevo;

                    nuevo.Journal_Tipo_Operacion = 13;
                    nuevo.Jorunal_Tipo = 0;

                    memset(&nuevo.Journal_Nombre,'\0',sizeof(nuevo.Journal_Nombre));
                    strcpy(nuevo.Journal_Nombre,"Cat");

                    memset(&nuevo.Journal_Contenido,'\0',sizeof(nuevo.Journal_Contenido));
                    strcpy(nuevo.Journal_Contenido,comando);

                    nuevo.Jorunal_Fecha = time(0);
                    nuevo.Journal_Propietario = 1;
                    nuevo.Journal_Permisos = 664;

                    SUPERBLOQUE sb;
                    sb = getSB(UsuarioLog.Path,UsuarioLog.StartParticion);
                    //seguido del super bloque empezaran los journaling
                    //siempre y cuando sea paraticion en ext3

                    setJOURNALING(nuevo,UsuarioLog.Path,UsuarioLog.StartParticion +sizeof(SUPERBLOQUE) + (PrimerJournalingLibre * sizeof(JOURNALING)));
                    PrimerJournalingLibre++;

                }
            }else{
                printf("Error no hay sesion activa para usar cat.\n");
            }
        }
        else if(strcasecmp(palabra,"mv") == 0){
            if((strcasecmp(UsuarioLog.Usuario,"\0") != 0) && (strcasecmp(UsuarioLog.pwd,"\0") != 0) &&(strcasecmp(UsuarioLog.id,"\0") != 0)){
                Mover(comando);
                if(UsuarioLog.Tipo == 3 && Bandera_Esto_En_Recovery == false){
                    JOURNALING nuevo;

                    nuevo.Journal_Tipo_Operacion = 14;
                    nuevo.Jorunal_Tipo = 0;

                    memset(&nuevo.Journal_Nombre,'\0',sizeof(nuevo.Journal_Nombre));
                    strcpy(nuevo.Journal_Nombre,"MV");

                    memset(&nuevo.Journal_Contenido,'\0',sizeof(nuevo.Journal_Contenido));
                    strcpy(nuevo.Journal_Contenido,comando);

                    nuevo.Jorunal_Fecha = time(0);
                    nuevo.Journal_Propietario = 1;
                    nuevo.Journal_Permisos = 664;

                    SUPERBLOQUE sb;
                    sb = getSB(UsuarioLog.Path,UsuarioLog.StartParticion);
                    //seguido del super bloque empezaran los journaling
                    //siempre y cuando sea paraticion en ext3

                    setJOURNALING(nuevo,UsuarioLog.Path,UsuarioLog.StartParticion +sizeof(SUPERBLOQUE) + (PrimerJournalingLibre * sizeof(JOURNALING)));
                    PrimerJournalingLibre++;

                }
            }else{
                printf("Error no hay sesion activa para usar Mover.\n");
            }
        }
        else if(strcasecmp(palabra,"rem") == 0){
            if((strcasecmp(UsuarioLog.Usuario,"\0") != 0) && (strcasecmp(UsuarioLog.pwd,"\0") != 0) &&(strcasecmp(UsuarioLog.id,"\0") != 0)){
                Remover(comando);
                if(UsuarioLog.Tipo == 3 && Bandera_Esto_En_Recovery == false){
                    JOURNALING nuevo;

                    nuevo.Journal_Tipo_Operacion = 15;
                    nuevo.Jorunal_Tipo = 0;

                    memset(&nuevo.Journal_Nombre,'\0',sizeof(nuevo.Journal_Nombre));
                    strcpy(nuevo.Journal_Nombre,"REM");

                    memset(&nuevo.Journal_Contenido,'\0',sizeof(nuevo.Journal_Contenido));
                    strcpy(nuevo.Journal_Contenido,comando);

                    nuevo.Jorunal_Fecha = time(0);
                    nuevo.Journal_Propietario = 1;
                    nuevo.Journal_Permisos = 664;

                    SUPERBLOQUE sb;
                    sb = getSB(UsuarioLog.Path,UsuarioLog.StartParticion);
                    //seguido del super bloque empezaran los journaling
                    //siempre y cuando sea paraticion en ext3

                    setJOURNALING(nuevo,UsuarioLog.Path,UsuarioLog.StartParticion +sizeof(SUPERBLOQUE) + (PrimerJournalingLibre * sizeof(JOURNALING)));
                    PrimerJournalingLibre++;

                }
            }else{
                printf("Error no hay sesion activa para usar Remover.\n");
            }
        }
        else if(strcasecmp(palabra,"chmod") == 0){
            if((strcasecmp(UsuarioLog.Usuario,"\0") != 0) && (strcasecmp(UsuarioLog.pwd,"\0") != 0) &&(strcasecmp(UsuarioLog.id,"\0") != 0)){

                if(UsuarioLog.Tipo == 3 && Bandera_Esto_En_Recovery == false){
                    JOURNALING nuevo;

                    nuevo.Journal_Tipo_Operacion = 16;
                    nuevo.Jorunal_Tipo = 0;

                    memset(&nuevo.Journal_Nombre,'\0',sizeof(nuevo.Journal_Nombre));
                    strcpy(nuevo.Journal_Nombre,"CHMOD");

                    memset(&nuevo.Journal_Contenido,'\0',sizeof(nuevo.Journal_Contenido));
                    strcpy(nuevo.Journal_Contenido,comando);

                    nuevo.Jorunal_Fecha = time(0);
                    nuevo.Journal_Propietario = 1;
                    nuevo.Journal_Permisos = 664;

                    SUPERBLOQUE sb;
                    sb = getSB(UsuarioLog.Path,UsuarioLog.StartParticion);
                    //seguido del super bloque empezaran los journaling
                    //siempre y cuando sea paraticion en ext3

                    setJOURNALING(nuevo,UsuarioLog.Path,UsuarioLog.StartParticion +sizeof(SUPERBLOQUE) + (PrimerJournalingLibre * sizeof(JOURNALING)));
                    PrimerJournalingLibre++;

                }
                Chmod(comando);
            }else{
                printf("Error no hay sesion activa para usar Chmod.\n");
            }
        }

        else if(strcasecmp(palabra,"convert") == 0){
            if((strcasecmp(UsuarioLog.Usuario,"\0") != 0) && (strcasecmp(UsuarioLog.pwd,"\0") != 0) &&(strcasecmp(UsuarioLog.id,"\0") != 0)){
                Convert(comando);
            }else{
                printf("Debe inicar sesion como Root.\n");
            }
        }else if(strcasecmp(palabra,"recovery") == 0){
            //recovery tampoco lo meto al journaling como a loss porque si no cuando haaga un recovery y hay loss y recovery
            //dentro del mismo archivo se quedaria enciclado

            if((strcasecmp(UsuarioLog.Usuario,"\0") != 0) && (strcasecmp(UsuarioLog.pwd,"\0") != 0) &&(strcasecmp(UsuarioLog.id,"\0") != 0)){
                Recovery(comando);
            }else{
                printf("Debe inicar sesion como Root.\n");
            }
        }
        else{
            printf("Comando no reconocido.\n");
        }
        memset(&palabra,'\0',sizeof(palabra));

    }else if(strcasecmp(comando,"\0") == 0){

    }else{
        printf(comando);            //es un comentario solo se imprime el comando.
    }
}

bool Exec(char coma[500]){

    char Path[250];
    memset(&Path,'\0', sizeof(Path));

    for (int i = 0; i < 500; i++) {
        if(coma[i] == 'p' || coma[i] == 'P'){       //colocar error si viene mal escrito la palabra path
            char *ruta = (char*)malloc(sizeof(ruta));
            ruta = getPath(coma,i);

            int indice = 0;
            memset(&Path,'\0',sizeof(Path));

            while(*ruta != '\0'){
                char letra = *ruta;
                Path[indice] = letra;
                ruta++;
                indice++;
            }

            i = indc;
            indc = 0;
        }else if(coma[i] == '#' ){
            //al momento de ser un numeral detectara comentario y terminara el ciclo
            break;
        }
    }
    if(strcasecmp(Path,"\0") != 0){
        printf("Ruta->");
        printf(Path);
        printf("\n");
        //ENVIAMOS A VERIFICAR LA RUTA ESPECIFICADA
        FILE *apun;
        apun = fopen(Path,"rb");
        if (apun != NULL){
            fclose(apun);
            fflush(apun);

        }else{
            printf("No se encontro Script en la path indicada.\n");
            return;
        }

        bool pathcorrcta = false;
        pathcorrcta = SepararRuta(Path);

        if(pathcorrcta){
            char linea[500];
            memset(&linea,'\0',sizeof(linea));
            FILE *fich;

            fich = fopen(Path, "r");
            //Lee línea a línea y escribe en pantalla hasta el fin de fichero
            int indice1 = 0;
            while(fgets(linea, 500, (FILE*) fich)) {
                if(linea[0] != '#' && linea[0] != '\n'){
                    printf("\n@201503911-> %s\n", linea);
                    Analisis(linea);
                }else if(linea[0] == '\n'){
                }else{
                    printf("\n@201503911-> %s\n", linea);
                }
                indice1++;
                memset(&linea,'\0',sizeof(linea));
            }
            fclose(fich);
            return true;
        }else {
            printf("Path Incorrecta.\n");
        }
    }else{
        printf("Error exec. Path es obligatorio.\0");
    }
}

void mkdisk(char comando[500]){
    int i = 0;
    while (comando[i] == ' ') {
        i++;
    }
    i = i+6;
    while(comando[i] == ' '){
        i++;
    }
    //size=
    //path=
    //unit=
    //fit=

    int TamaoSize = 0;
    char FIT[3];
    char Unit = '\0';
    char RUTA[250];

    memset(&FIT,'\0',sizeof(FIT));
    memset(&RUTA,'\0',sizeof(RUTA));

    for (i = i; i < 500; i++) {

        if((comando[i] == 's' || comando[i] == 'S')
                &&(comando[i+1] == 'i' || comando[i+1] == 'I')
                &&(comando[i+2] == 'z' || comando[i+2] == 'Z')
                &&(comando[i+3] == 'e' || comando[i+3] == 'E')){

            TamaoSize = getSize(comando,i);
            printf("Size->%d\n",TamaoSize);
            i = indc;
            indc = 0;
        }
        else if((comando[i] == 'f' || comando[i] == 'F')
                &&(comando[i+1] == 'i' || comando[i+1] == 'I')
                &&(comando[i+2] == 't' || comando[i+2] == 'T')){
            char *fit = (char*)malloc(sizeof(fit));
            fit = getFit(comando,i);
            int indice = 0;

            memset(&FIT,'\0',sizeof(FIT));

            while(*fit != '\0'){
                char letra = *fit;
                FIT[indice] = letra;
                fit++;
                indice++;
            }
            printf("Fit->%s\n",FIT);
            /*printf(FIT);
            printf("\n");*/
            i = indc;
            indc = 0;
        }
        else if((comando[i] == 'u' || comando[i] == 'U')
                &&(comando[i+1] == 'n' || comando[i+1] == 'N')
                &&(comando[i+2] == 'i' || comando[i+2] == 'I')
                &&(comando[i+3] == 't' || comando[i+3] == 'T')){
            char *unit = (char*)malloc(sizeof(unit));
            unit = getUnit(comando,i);

            memset(&Unit,'\0',sizeof(Unit));

            Unit = unit;
            printf("Unit->%c \n",Unit);
            i = indc;
            indc = 0;
        }
        else if((comando[i] == 'p' || comando[i] == 'P')
                &&(comando[i+1] == 'a' || comando[i+1] == 'A')
                &&(comando[i+2] == 't' || comando[i+2] == 'T')
                &&(comando[i+3] == 'h' || comando[i+3] == 'H')){

            char *ruta = (char*)malloc(sizeof(ruta));
            ruta = getPath(comando,i);

            int indice = 0;
            memset(&RUTA,'\0',sizeof(RUTA));

            while(*ruta != '\0'){
                char letra = *ruta;
                RUTA[indice] = letra;
                ruta++;
                indice++;
            }

            printf("Ruta->");
            printf(RUTA);
            printf("\n");
            i = indc;
            indc = 0;
        }else if(comando[i] == '#' ){
            //al momento de ser un numeral detectara comentario y terminara el ciclo
            break;
        }
        else{
            if(comando[i-1] == '\n'){
                break;
            }
            else if(comando[i] != '-' && comando[i] != ' ' && comando[i] != '\n' && comando[i] != '\r'){
                printf("Error en los parametros del comando mkdisk.");
                return;         //se sale del metodo ya que da error de parametros.
            }
        }
    }

    //verificacion de atributos obligatorios
    if(TamaoSize != 0 && strcasecmp(RUTA,"\0") != 0){

        //ENVIAMOS A VERIFICAR LA RUTA ESPECIFICADA
        bool pathcorrcta = SepararRuta(RUTA);
        if(TamaoSize > 0){
            if(pathcorrcta){            //si la path es correcta se crea el disco

                //DESPUES DE VERIFICAR LA RUTA CREAMOS EL DISCO
                //printf("\n#crea un disco de %d %c en la carperta: %s \n",TamaoSize,Unit,RUTA);
                FILE *fp;                                                                                       //Creamos el disco con los parametros indicados
                fp = fopen (RUTA, "wb+");

                if(Unit == 'k'||Unit == 'K'){
                    TamaoSize = TamaoSize*1024;
                }
                else if(Unit == 'm' || (Unit == 'M') || Unit == '\0'){
                    TamaoSize = TamaoSize*1024*1024;
                }else{
                    if(Unit == '\0'){
                        Unit = 'm';
                        TamaoSize = TamaoSize*1024*1024;
                    }else{
                        printf("Error al crear disco en unidad de medida mkdisk.\n");
                    }
                }

                for(int r = 0; r < TamaoSize; r++){                                                               //llenamos el archivo con nulos-> se debe cambiar a binario
                    fprintf(fp, "%c", 0);
                }

                //CREAMOS EL MBR
                fseek(fp, 0, SEEK_SET);

                MBR mbr;
                memset(&mbr,0,sizeof(MBR));                          //inicializamos en cero el struct
                mbr.mbr_tamao = TamaoSize;
                mbr.mbr_disk_signature = rand() % 100;
                mbr.disk_fit = FIT[0];
                mbr.mbr_fecha_creacion = time(0);
                mbr.mbr_partition_1.part_status = 'N';
                mbr.mbr_partition_2.part_status = 'N';
                mbr.mbr_partition_3.part_status = 'N';
                mbr.mbr_partition_4.part_status = 'N';

                //printf("Mbr Creado:\nTamano: %d\nDiskAsignature: %d\nFit: %c\nParticion1: %c\nParticion2: %c\nParticion3: %c\nParticion4: %c\n",mbr.mbr_tamao,mbr.mbr_disk_signature,mbr.disk_fit,mbr.mbr_partition_1.part_status,mbr.mbr_partition_2.part_status,mbr.mbr_partition_3.part_status,mbr.mbr_partition_4.part_status);
                fwrite(&mbr,sizeof(MBR),1,fp);

                fclose (fp);
                fflush(fp);
                printf("\nSe ha creado el disco satisfactoriamente.\n");
            }
        }
    }else{
        printf("Error en comando mkdisk parametros Path y Size son obligatorios.\n");
        return;
    }
}

void rmdisk(char comando[500]){
    char RUTA[250];
    memset(&RUTA,'\0',sizeof(RUTA));
    int i = 0;
    while (comando[i] == ' ') { //saltamos espacios antes de palabra rmdisk
        i++;
    }
    i = i+6;                    //saltamos palabra "rmdisk"
    while(comando[i] == ' '){   //saltamos espacios despues de la palabra rmdisk
        i++;
    }

    for (i = i; i < 500; i++) {

        if((comando[i] == 'p' || comando[i] == 'P')
                &&(comando[i+1] == 'a' || comando[i+1] == 'A')
                &&(comando[i+2] == 't' || comando[i+2] == 'T')
                &&(comando[i+3] == 'h' || comando[i+3] == 'H')){
            char *ruta = (char*)malloc(sizeof(ruta));
            ruta = getPath(comando,i);

            int indice = 0;
            memset(&RUTA,'\0',sizeof(RUTA));

            while(*ruta != '\0'){
                char letra = *ruta;
                RUTA[indice] = letra;
                ruta++;
                indice++;
            }

            printf("Ruta->");
            printf(RUTA);
            printf("\n");
            i = indc;
            indc = 0;
            break;
        }else if(comando[i] == '#' ){
            //al momento de ser un numeral detectara comentario y terminara el ciclo
            break;
        }else{
            if(comando[i-1] == '\n'){
                break;
            }
            else if(comando[i] != '-' && comando[i] != ' ' && comando[i] != '\n' && comando[i] != '\r'){
                printf("Error en los parametros del comando rmdisk.\n");
                return;
            }
        }
    }
    if(strcasecmp(RUTA,"\0") != 0){
        FILE *disco;
        disco = fopen(RUTA,"rb");
        if (disco != NULL){
            fclose(disco);
            fflush(disco);

            printf("Esta seguro que desea eliminar el disco. (y/n)\n");
            char Decision[3];
            memset(&Decision,'\0',sizeof(Decision));
            fgets(Decision,3,stdin);

            if(Decision[0] == 'y' || Decision[0] == 'Y'){
                //verificacion si se puede eliminar el disco
                if(remove(RUTA)==0){                                                                                    // Eliminamos el archivo
                    printf("El Disco fue eliminado satisfactoriamente.");
                }else{
                    printf("No se pudo eliminar el archivo.\n");
                }
            }else{
                printf("Eliminacion de disco abortada.\n");
            }
        }else{
            printf("No se ha encontrado el disco en la ruta establecida.(Disco no existe)\n");
            return;
        }
        indicador_eliminacio_ndisco = false;
        fflush(stdin);
    }else{
        printf("Error en comando Rmdisk el parametro Path es obligatorio.\n");
    }
}

void fdisk (char comando[500]){

    int i = 0;
    while (comando[i] == ' ') {
        i++;
    }
    i = i+5;
    while(comando[i] == ' '){
        i++;
    }
    char Type = '\0';
    int TamaoSize = 0;
    char FIT[3];
    char Unit = '\0';
    char RUTA[250];
    char Delete[5];
    int Add = 0;
    char Nombre[250];

    memset(&Delete,'\0',sizeof(Delete));
    memset(&FIT,'\0',sizeof(FIT));
    memset(&RUTA,'\0',sizeof(RUTA));
    memset(&Nombre,'\0',sizeof(Nombre));

    for(int i = 6; i < 500; i++){
        if((comando[i] == 's' || comando[i] == 'S')
                &&(comando[i+1] == 'i' || comando[i+1] == 'I')
                &&(comando[i+2] == 'z' || comando[i+2] == 'Z')
                &&(comando[i+3] == 'e' || comando[i+3] == 'E')){

            TamaoSize = getSize(comando,i);
            printf("Size->%d\n",TamaoSize);
            i = indc;
            indc = 0;
        }
        else if((comando[i] == 'f' || comando[i] == 'F')
                &&(comando[i+1] == 'i' || comando[i+1] == 'I')
                &&(comando[i+2] == 't' || comando[i+2] == 'T')){
            char *fit = (char*)malloc(sizeof(fit));
            fit = getFit(comando,i);
            int indice = 0;

            memset(&FIT,'\0',sizeof(FIT));

            while(*fit != '\0'){
                char letra = *fit;
                FIT[indice] = letra;
                fit++;
                indice++;
            }
            printf("Fit->%s\n",FIT);
            /*printf(FIT);
            printf("\n");*/
            i = indc;
            indc = 0;
        }
        else if((comando[i] == 'u' || comando[i] == 'U')
                &&(comando[i+1] == 'n' || comando[i+1] == 'N')
                &&(comando[i+2] == 'i' || comando[i+2] == 'I')
                &&(comando[i+3] == 't' || comando[i+3] == 'T')){
            char *unit = (char*)malloc(sizeof(unit));
            unit = getUnit(comando,i);

            memset(&Unit,'\0',sizeof(Unit));
            Unit = unit;
            printf("Unit->%c \n",Unit);
            i = indc;
            indc = 0;
        }
        else if((comando[i] == 'p' || comando[i] == 'P')
                &&(comando[i+1] == 'a' || comando[i+1] == 'A')
                &&(comando[i+2] == 't' || comando[i+2] == 'T')
                &&(comando[i+3] == 'h' || comando[i+3] == 'H')){

            char *ruta = (char*)malloc(sizeof(ruta));
            ruta = getPath(comando,i);
            int indice = 0;
            memset(&RUTA,'\0',sizeof(RUTA));

            while(*ruta != '\0'){
                char letra = *ruta;
                RUTA[indice] = letra;
                ruta++;
                indice++;
            }
            printf("Ruta->");
            printf(RUTA);
            printf("\n");
            i = indc;
            indc = 0;
        }
        else if((comando[i] == 't' || comando[i] == 'T')
                &&(comando[i+1] == 'y' || comando[i+1] == 'Y')
                &&(comando[i+2] == 'p' || comando[i+2] == 'P')
                &&(comando[i+3] == 'e' || comando[i+3] == 'E')){
            char *type = (char*)malloc(sizeof(type));
            type = getType(comando,i);
            i = indc;
            indc = 0;
            memset(&Type,'\0',sizeof(Type));
            Type = type;
            printf("Type->%c \n",Type);

        }
        else if((comando[i] == 'n' || comando[i] == 'N')
                &&(comando[i+1] == 'a' || comando[i+1] == 'A')
                &&(comando[i+2] == 'm' || comando[i+2] == 'M')
                &&(comando[i+3] == 'e' || comando[i+3] == 'E')){

            char *nombre = (char*)malloc(sizeof(nombre));
            nombre = getName(comando,i);
            int indice = 0;
            memset(&Nombre,'\0',sizeof(Nombre));
            while(*nombre != '\0'){
                char letra = *nombre;
                Nombre[indice] = letra;
                nombre++;
                indice++;
            }
            printf("Name: %s\n",Nombre);
            i = indc;
            indc = 0;
        }
        else if((comando[i] == 'd' || comando[i] == 'D')
                && (comando[i+1] == 'e' || comando[i+1] == 'E')
                && (comando[i+2] == 'l' || comando[i+2] == 'L')
                && (comando[i+3] == 'e' || comando[i+3] == 'E')
                && (comando[i+4] == 't' || comando[i+4] == 'T')
                && (comando[i+5] == 'e' || comando[i+5] == 'E')){

            char *delete = (char*)malloc(sizeof(delete));
            delete = getDelete(comando,i);
            int indice = 0;
            memset(&Delete,'\0',sizeof(Delete));
            while(*delete != '\0'){
                char letra = *delete;
                Delete[indice] = letra;
                delete++;
                indice++;
            }
            printf("Delete: %s\n",Delete);
            i = indc;
            indc = 0;
        }
        else if((comando[i] == 'a' || comando[i] == 'A')
                && (comando[i+1] == 'd' || comando[i+1] == 'D')
                && (comando[i+2] == 'd' || comando[i+2] == 'D')){

            Add = getAdd(comando,i);
            printf("Add: %d\n",Add);
            i = indc;
            indc = 0;
        }else if(comando[i] == '#' ){
            //al momento de ser un numeral detectara comentario y terminara el ciclo
            break;
        }else{
            if(comando[i-1] == '\n'){
                break;
            }
            else if(comando[i] != '-' && comando[i] != ' ' && comando[i] != '\n' && comando[i] != '\r'){
                printf("Error en los parametros del comando fdisk.\n");
                return;
            }
        }
    }

    if(strcasecmp(Nombre,"\0") != 0 && strcasecmp(RUTA,"\0") != 0){
        //Una vez capturados los valores se manda a la accion que amerite.
        if(strcasecmp(Delete, "\0") != 0 || Add != 0){      //verifico que si estos parametros son nulos de serlo se tratara
            // de creacion de una particion
            //cuando venga Add ignora los demas atributos si viene size o delete.(el primero que encuentre)
            if( Add != 0){
                if(Unit == 'k' || Unit == 'K'){
                    Add = Add * 1024;
                }else if(Unit == 'b' || Unit == 'B'){
                    Add = Add;
                }else if(Unit == 'M' || Unit == 'm'){
                    Add = Add * 1024 *1024;
                }else{
                    printf("Error en la dimension de Add.(Add Fdisk)\n");
                    return;
                }
                //printf("El valor de Add no es nulo.\n");
                Agregar(RUTA,Nombre,Add);
            }
            else if(strcasecmp(Delete, "\0") != 0){

                MBR mbr = getMBR(RUTA);
                if((strcasecmp(mbr.mbr_partition_1.part_name,Nombre) == 0
                    ||strcasecmp(mbr.mbr_partition_2.part_name,Nombre) == 0
                    ||strcasecmp(mbr.mbr_partition_3.part_name,Nombre) == 0
                    ||strcasecmp(mbr.mbr_partition_4.part_name,Nombre) == 0) && mbr.mbr_tamao != 0){

                    if(strcasecmp(Delete,"fast") == 0 || strcasecmp(Delete,"full") == 0){
                        //printf("El valo de delete no es nulo.\n");
                        printf("Esta seguro que desea eliminar la particion.(y/n)");
                        char respuesta[3];
                        memset(&respuesta,'\0',sizeof(respuesta));

                        fgets(respuesta,3,stdin);
                        if(respuesta[0] == 'y' || respuesta[0] == 'Y'){
                            EliminarParticion(RUTA,Nombre);
                        }else{
                            printf("Eliminacion de Particion abortada.\n");
                        }
                    }else{
                        printf("Error en el parametro Delete, atributo no valido.\n");
                    }
                }else{
                    printf("El nombre de particion especificado no esxiste en el disco.\n");
                }
            }
        }else{
            CrearParticionFDISK(TamaoSize,Unit,RUTA,Type,FIT[0],Nombre);           //mandamos a crear la particion.
            printf("\n\nImprimiendo Particiones\n");
            Verparticiones(RUTA);       //muestra el status de las particiones en el disco.
        }
    }else{
        printf("Error parametro fdisk Name y Path son obligatorios.\n");
    }
}

void Agregar(char Path[250],char Nombre[50],int Add){
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
            printf("Particion indicada no existe en el disco actual.\n");
            return;
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
            printf("Particion indicada no existe en el disco actual.\n");
            return;
        }
        setMBR(mbr,Path);
        printf("Se agregaran %d bytes a la Particion: %s",Add,Nombre);
    }
}

void Unmount(char coma[500]){
    int i = 0;
    while (coma[i] == ' ') {
        i++;
    }
    i = i+7;
    while(coma[i] == ' '){
        i++;
    }

    char Id[7];
    bool Correcto = true;
    memset(Id,'\0',sizeof(Id));


    for (i = i; i < 500; i++) {

        if ((coma[i] == 'i' || coma[i] == 'I')
                &&(coma[i+1] == 'd' || coma[i+1] == 'D')){

            char id[15];
            memset(id,'\0',sizeof(id));
            id[0] = coma[i];
            id[1] = coma[i+1];
            id[2] = coma[i+2];
            if(strcasecmp(id,"id=") == 0){
                i = i + 3;
                while(coma[i] == ' '){
                    i++;
                }
                //inicializamos el arreglo en nulo
                memset(Id,'\0', sizeof(Id));
                int indice2 = 0;
                while(coma[i] != ' ' && coma[i] != '\n'){
                    Id[indice2] = coma[i];
                    indice2++;
                    if(i < strlen(coma)){
                        i++;
                    }else{
                        break;
                    }
                }
            }else{
                Correcto = false;
            }
        } else if(coma[i] == '#' ){
            //al momento de ser un numeral detectara comentario y terminara el ciclo
            break;
        }else{
            if(coma[i-1] == '\n'){
                break;
            }
            else if(coma[i] != '-' && coma[i] != ' ' && coma[i] != '\n'){
                printf("Error en los parametros del comando Unmount.");
                return;         //se sale del metodo ya que da error de parametros.
            }
        }
    }
    if(strcasecmp(Id,"\0") != 0){
        if(Correcto ){
            if(primero != NULL){
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
            }else {
                printf("No existen particiones montadas.\n");
            }
        }else{
            printf("Comando Incorrecto.Se esperaba Unmount.\n");
        }
    }else{
        printf("Error Unmount Id obligatorio.\n");
    }
}

void Moun(char comando[500]){
    int i = 0;
    while (comando[i] == ' ') {
        i++;
    }
    i = i+5;
    while(comando[i] == ' '){
        i++;
    }
    char Path[250];
    memset(&Path,'\0',sizeof(Path));

    char Nombre[250];
    memset(&Nombre,'\0',sizeof(Nombre));

    for (i =i; i < 500; i++) {

        if((comando[i] == 'p' || comando[i] == 'P')
                &&(comando[i+1] == 'a' || comando[i+1] == 'A')
                &&(comando[i+2] == 't' || comando[i+2] == 'T')
                &&(comando[i+3] == 'h' || comando[i+3] == 'H')){

            memset(&Path,'\0',sizeof(Path));

            char *ruta = (char*)malloc(sizeof(ruta));
            ruta = getPath(comando,i);

            int indice = 0;
            while(*ruta != '\0'){
                char letra = *ruta;
                Path[indice] = letra;
                ruta++;
                indice++;
            }

            printf("Paht: ");
            printf(Path);
            printf("\n");
            i = indc;
            indc = 0;
        }
        else if((comando[i] == 'n' || comando[i] == 'N')
                &&(comando[i+1] == 'a' || comando[i+1] == 'A')
                &&(comando[i+2] == 'm' || comando[i+2] == 'M')
                &&(comando[i+3] == 'e' || comando[i+3] == 'E')){

            memset(&Nombre,'\0',sizeof(Nombre));

            char *nom = (char*)malloc(sizeof(nom));
            nom = getName(comando,i);

            int indice = 0;
            while(*nom != '\0'){
                char letra = *nom;
                Nombre[indice] = letra;
                nom++;
                indice++;
            }

            printf("Nombre: ");
            printf(Nombre);
            printf("\n");
            i = indc;
            indc = 0;
        } else if(comando[i] == '#' ){
            //al momento de ser un numeral detectara comentario y terminara el ciclo
            break;
        }else{
            if(comando[i-1] == '\n'){
                break;
            }
            else if(comando[i] != '-' && comando[i] != ' ' && comando[i] != '\n' && comando[i] != '\r' && comando[i] != '\r'){
                printf("Error en los parametros del comando Mount.");
                return;         //se sale del metodo ya que da error de parametros.
            }
        }
    }

    if(strcasecmp(Nombre,"\0") != 0 && strcasecmp(Path,"\0") != 0){
        //verificamos de que la particion no este montada si ya esta se mostrara mensaje que ya se encuentra montada con tal id
        bool yaexiste = false;
        char Id[8];
        memset(&Id,'\0',sizeof(Id));

        MONTADURAS *aux = primero;
        while (aux != NULL) {
            if(strcasecmp(aux->Path,Path) == 0 && strcasecmp(aux->Name,Nombre) == 0){
                yaexiste = true;
                strcat(Id,aux->Id);
                break;
            }
            aux = aux->siguiente;
        }
        if(!yaexiste){
            MBR mbr;
            mbr = getMBR(Path);
            if (mbr.mbr_tamao != 0){
                if(strcasecmp(mbr.mbr_partition_1.part_name,Nombre) == 0){
                    MontarParticion(Path,Nombre,mbr.mbr_partition_1.part_start);
                }
                else if(strcasecmp(mbr.mbr_partition_2.part_name,Nombre) == 0){
                    MontarParticion(Path,Nombre,mbr.mbr_partition_2.part_start);
                }
                else if(strcasecmp(mbr.mbr_partition_3.part_name,Nombre) == 0){

                    MontarParticion(Path,Nombre,mbr.mbr_partition_3.part_start);
                }
                else if(strcasecmp(mbr.mbr_partition_4.part_name,Nombre) == 0){

                    MontarParticion(Path,Nombre,mbr.mbr_partition_4.part_start);
                }else{
                    printf("Particion %s NO existe en el disco.\n",Nombre);
                }

            }else{
                printf("El Disco Especificado en la path %s No existe.(getMBR)\n",Path);
            }
        }else{
            printf("La Particion ya se encuentra montada con id %s\n",Id);
        }
    }else{
        printf("Error Mount Nombre y Path son obligatorios.\n");
    }
}

void MontarParticion(char Path[250], char Name[100],int Starparticion){

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

    MONTADURAS *Nuevonodo = (MONTADURAS*)malloc(sizeof(MONTADURAS));
    strcpy(Nuevonodo->Path,Path);
    strcpy(Nuevonodo->Name,Name);
    Nuevonodo->StartParticion = Starparticion;
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
}

void CrearParticionFDISK(int Size, char Unit, char Path[250],char Type, char Fit, char Name[100]){

    //printf("size: %d\nUnit: %c\nPath: %s\nType: %c\nFit: %c\nName: %s\n",Size,Unit,Path,Type,Fit,Name);
    if(Unit == 'b' || Unit == 'B'){
        Size = Size;
    }
    else if(Unit == 'k' || Unit == 'K'){
        Size = Size*1024;
    }
    else if (Unit == 'm' || Unit == 'M'){
        Size = Size*1024*1024;
    }
    else{                   //unit es opcional se tomara KB si no esta especificado en el comando
        if(Unit == '\0'){
            Unit = 'K';
            Size = Size*1024;
        }else{
            printf("Particion no creada debido a que la unidad de medida no es reconocida.(Bytes,Kb,Mb)\n");
            return;
        }
    }

    if(Fit == '\0'){        //fit es opcional se tomara el peor ajuste si no esta especificado en el comando
        Fit = 'W';
    }else{
        if((Fit != 'b' && Fit != 'B')
                && (Fit != 'f' && Fit != 'F')
                && (Fit != 'w' && Fit != 'W'))  {
            printf("Particion no creada debido a que el ajuste especificado no existe.\n");
            return;
        }
    }

    /*--captura del mbr--*/
    FILE *apun;
    apun = fopen(Path,"rb");

    if(apun != NULL){
        fclose(apun);           //si existe lo cierro.
        fflush(apun);

        MBR mbr;
        mbr = getMBR(Path);

        //verificacion que no exista una particion con el mismo nombre
        if(strcasecmp(mbr.mbr_partition_1.part_name,Name)!=0
                && strcasecmp(mbr.mbr_partition_2.part_name,Name)!=0
                && strcasecmp(mbr.mbr_partition_3.part_name,Name)!=0
                && strcasecmp(mbr.mbr_partition_4.part_name,Name)!=0)
        {
            // printf("\nMBR RECUPERADO:\nTamano: %d\nDiskAsignature: %d\nFit: %c\nParticion1: %c\nParticion2: %c\nParticion3: %c\nParticion4: %c\n",mbr.mbr_tamao,mbr.mbr_disk_signature,mbr.disk_fit,mbr.mbr_partition_1.part_status,mbr.mbr_partition_2.part_status,mbr.mbr_partition_3.part_status,mbr.mbr_partition_4.part_status);

            //evaluamos las dimasiones de la particion con el disco

            if(Size <= (mbr.mbr_tamao - sizeof(MBR))){

                //si la particion es primaria
                if(Type == 'p' || Type == 'P'){

                    int start = VerificacionAjuste(Path,Size);                  //start es el bit donde dara inicio l particion
                    CrearParticion(Type,Fit,Size,Name,Path,NummeroPaticion,start);
                }
                else if(Type == 'e' || Type == 'E'){

                    //Verificamos que no haya alguna otra particion extendida creada de ser asi notificara un error
                    if( (mbr.mbr_partition_1.part_type != 'e' && mbr.mbr_partition_1.part_type != 'E')
                            && (mbr.mbr_partition_2.part_type != 'e' && mbr.mbr_partition_2.part_type != 'E')
                            && (mbr.mbr_partition_3.part_type != 'e' && mbr.mbr_partition_3.part_type != 'E')
                            && (mbr.mbr_partition_4.part_type != 'e' && mbr.mbr_partition_4.part_type != 'E') ){

                        int start = VerificacionAjuste(Path,Size);
                        CrearParticion(Type,Fit,Size,Name,Path,NummeroPaticion,start);
                    }else{
                        printf("\nError ya existe una particion extendida creada en el disco.\n");
                    }
                }
            }else{
                printf("\nParticion sobrepasa la capacidad del disco. (CrearParticionFdisk.)\n");
            }
        }else{
            printf("Error al crear particion debido a que ya existe otra particion con el mismo nombre.\n");
        }
    }else{
        printf("Disco especificado no existe.\n");
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

        //si el tipo de la [particion es extendida se crea el mbr inicial
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
                ebr.part_start = 0;
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
                ebr1.part_start = 0;
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
                ebr2.part_start = 0;
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
                ebr3.part_start = 0;
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

void EliminarParticion(char Path[250], char Name[15]){
    MBR mbr = getMBR(Path);

    if(strcasecmp(mbr.mbr_partition_1.part_name,Name) == 0){
        memset(&mbr.mbr_partition_1,0,sizeof(PARTICION));
        mbr.mbr_partition_1.part_status = 'N';
    }
    else  if(strcasecmp(mbr.mbr_partition_2.part_name,Name) == 0){
        memset(&mbr.mbr_partition_2,0,sizeof(PARTICION));
        mbr.mbr_partition_2.part_status = 'N';
    }
    else  if(strcasecmp(mbr.mbr_partition_3.part_name,Name) == 0){
        memset(&mbr.mbr_partition_3,0,sizeof(PARTICION));
        mbr.mbr_partition_3.part_status = 'N';
    }
    else  if(strcasecmp(mbr.mbr_partition_4.part_name,Name) == 0){
        memset(&mbr.mbr_partition_4,0,sizeof(PARTICION));
        mbr.mbr_partition_4.part_status = 'N';
    }else{
        printf("El nombre de particion especificado no esxiste en el disco.\n");
    }

    FILE *file;
    file = fopen(Path,"rb+");

    if(file!=NULL)
    {
        fseek(file,0,SEEK_SET);
        fwrite(&mbr,sizeof(MBR),1,file);
        fclose(file);
        fflush(file);

        printf("Particion %s Eliminada con exit.\n",Name);
    }
    else{
        printf("Error el archivo no existe\n");
        return;
    }
}

void Rep(char coma[500]){
    char Path[250];
    char RUTA[250];
    char Name[10];
    char Id[9];
    bool Correcto = true;

    memset(&Path,'\0',sizeof(Path));
    memset(&RUTA,'\0',sizeof(RUTA));
    memset(&Name,'\0',sizeof(Name));
    memset(&Id,'\0',sizeof(Id));

    int i = 0;
    while (coma[i] == ' ') {
        i++;
    }
    i = i+4;
    while(coma[i] == ' '){
        i++;
    }

    for ( i = i; i < 500; i++) {
        if(coma[i] == '\0'){
            break;
        }

        if((coma[i] == 'p' || coma[i] == 'P')
                &&(coma[i+1] == 'a' || coma[i+1] == 'A')
                &&(coma[i+2] == 't' || coma[i+2] == 'T')
                &&(coma[i+3] == 'h' || coma[i+3] == 'H')){
            char *ruta = (char*)malloc(sizeof(ruta));
            ruta = getPath(coma,i);

            int indice = 0;
            memset(&Path,'\0',sizeof(Path));

            while(*ruta != '\0'){
                char letra = *ruta;
                Path[indice] = letra;
                ruta++;
                indice++;
            }

            //printf("Ruta->");
            //printf(Path);
            //printf("\n");
            i = indc;
            indc = 0;
        }
        if((coma[i] == 'r' || coma[i] == 'R')
                &&(coma[i+1] == 'u' || coma[i+1] == 'U')
                &&(coma[i+2] == 't' || coma[i+2] == 'T')
                &&(coma[i+3] == 'a' || coma[i+3] == 'A')){

            char *ruta = (char*)malloc(sizeof(ruta));
            ruta = getPath(coma,i);

            int indice = 0;
            memset(&RUTA,'\0',sizeof(RUTA));

            while(*ruta != '\0'){
                char letra = *ruta;
                RUTA[indice] = letra;
                ruta++;
                indice++;
            }

            //printf("Ruta->");
            //printf(Path);
            //printf("\n");
            i = indc;
            indc = 0;
        }else if ((coma[i] == 'n' || coma[i] == 'N')
                  &&(coma[i+1] == 'a' || coma[i+1] == 'A')
                  &&(coma[i+2] == 'm' || coma[i+2] == 'M')
                  &&(coma[i+3] == 'e' || coma[i+3] == 'E')){
            char *nombre = (char*)malloc(sizeof(nombre));
            nombre = getName(coma ,i);
            int indice = 0;
            memset(&Name,'\0',sizeof(Name));
            while(*nombre != '\0'){
                char letra = *nombre;
                Name[indice] = letra;
                nombre++;
                indice++;
            }
            //printf("Name: %s\n",Name);
            i = indc;
            indc = 0;
        }else if ((coma[i] == 'i' || coma[i] == 'I')
                  &&(coma[i+1] == 'd' || coma[i+1] == 'D')){
            char id[15];
            memset(id,'\0',sizeof(id));
            id[0] = coma[i];
            id[1] = coma[i+1];
            id[2] = coma[i+2];
            if(strcasecmp(id,"id=") == 0){
                i = i + 3;
                while(coma[i] == ' '){
                    i++;
                }
                //inicializamos el arreglo en nulo
                memset(Id,'\0', sizeof(Id));
                int indice2 = 0;
                while(coma[i] != ' ' && coma[i] != '\n'){
                    Id[indice2] = coma[i];
                    indice2++;
                    if(i < strlen(coma)){
                        i++;
                    }else{
                        break;
                    }
                }
            }else{
                printf("Error en la sintaxis de Id=.(Rep)\n");
                return;
            }
        } else if(coma[i] == '#' ){
            //al momento de ser un numeral detectara comentario y terminara el ciclo
            break;
        }else{
            if(coma[i-1] == '\n'){
                break;
            }
            else if(coma[i] != '-' && coma[i] != ' ' && coma[i] != '\n'){
                printf("Error en los parametros del comando mkdisk.");
                return;         //se sale del metodo ya que da error de parametros.
            }
        }
    }
    char iDD[9];
    char N[10];
    memset(&N,'\0', sizeof(N));
    memset(&iDD,'\0', sizeof(iDD));
    strcpy(iDD,Id);
    strcpy(N,Name);

    if(strcasecmp(Path,"\0") != 0 && strcasecmp(Name,"\0") != 0 && strcasecmp(Id,"\0") != 0){
        if(Correcto){
            printf("Id: %s\nPath: %s\nName: %s\n",iDD,Path,N,RUTA);
            ReporteMBR(iDD,Path,N,RUTA);

        }else{
            printf("Comando Incorrecto.\n");
        }
    }else{
        printf("Error Rep. Path, Nombre y Id son obligatorios.\n");
    }
}

void ReporteMBR(char Id[9],char Path[250],char Name[10],char Ruta[250]){

    bool VerificacionPath = false;
    MONTADURAS *aux = primero;
    char ruta[250];
    bool loencontre = false;                //buscamos en nustas particiones montadas para ver si ya esta montada de lo contrario dara error
    memset(&ruta,'\0',sizeof(ruta));
    while (aux != NULL) {
        if(strcasecmp(aux->Id,Id)==0){
            strcpy(ruta,aux->Path);
            loencontre = true;
            break;
        }
        aux = aux->siguiente;
    }
    if(loencontre){
        MBR mbr;
        mbr = getMBR(ruta);

        if(strcasecmp(Name,"mbr")==0)
        {
            FILE *dot;
            dot = fopen("/home/osuna/Escritorio/Reporte.dot","w+");

            fprintf(dot,"digraph test {\n");
            fprintf(dot,"graph [ratio=fill];\n");
            fprintf(dot,"node [label=\"\N\", fontsize=15, shape=plaintext];\n");
            fprintf(dot,"graph [bb=\"0,0,352,154\"];\n");
            fprintf(dot,"arset [label=<\n");
            fprintf(dot,"<TABLE ALIGN=\"LEFT\">\n");

            fprintf(dot,"<TR>\n");
            fprintf(dot,"<TD>NOMBRE</TD>");
            fprintf(dot,"<TD>VALOR</TD>");
            fprintf(dot,"</TR>\n");

            fprintf(dot,"<TR>\n");
            fprintf(dot,"<TD>mbr_tamaño</TD>");
            fprintf(dot,"<TD>%d </TD>",mbr.mbr_tamao);
            fprintf(dot,"</TR>\n");

            fprintf(dot,"<TR>\n");
            fprintf(dot,"<TD>mbr_fecha_creacion</TD>");
            fprintf(dot,"<TD>%s</TD>",ctime(&mbr.mbr_fecha_creacion));
            fprintf(dot,"</TR>\n");

            fprintf(dot,"<TR>\n");
            fprintf(dot,"<TD>mbr_disk_signature</TD>");
            fprintf(dot,"<TD>%d </TD>",mbr.mbr_disk_signature);
            fprintf(dot,"</TR>\n");

            if(mbr.mbr_partition_1.part_status == 'S'){
                fprintf(dot,"<TR>\n");
                fprintf(dot,"<TD>part_status_1</TD>");
                fprintf(dot,"<TD>%c</TD>",mbr.mbr_partition_1.part_status);
                fprintf(dot,"</TR>\n");

                fprintf(dot,"<TR>\n");
                fprintf(dot,"<TD>part_type_1</TD>");
                fprintf(dot,"<TD>%c </TD>",mbr.mbr_partition_1.part_type);
                fprintf(dot,"</TR>\n");

                fprintf(dot,"<TR>\n");
                fprintf(dot,"<TD>part_fit_1</TD>");
                fprintf(dot,"<TD>%c </TD>",mbr.mbr_partition_1.part_fit);
                fprintf(dot,"</TR>\n");

                fprintf(dot,"<TR>\n");
                fprintf(dot,"<TD>part_start_1</TD>");
                fprintf(dot,"<TD>%d </TD>",mbr.mbr_partition_1.part_start);
                fprintf(dot,"</TR>\n");

                fprintf(dot,"<TR>\n");
                fprintf(dot,"<TD>part_size_1</TD>");
                fprintf(dot,"<TD>%d </TD>",mbr.mbr_partition_1.part_size);
                fprintf(dot,"</TR>\n");

                fprintf(dot,"<TR>\n");
                fprintf(dot,"<TD>part_name_1</TD>");
                fprintf(dot,"<TD>%s </TD>",mbr.mbr_partition_1.part_name);
                fprintf(dot,"</TR>\n");
            }
            if(mbr.mbr_partition_2.part_status == 'S'){
                fprintf(dot,"<TR>\n");
                fprintf(dot,"<TD>part_status_2</TD>");
                fprintf(dot,"<TD>%c</TD>",mbr.mbr_partition_2.part_status);
                fprintf(dot,"</TR>\n");

                fprintf(dot,"<TR>\n");
                fprintf(dot,"<TD>part_type_2</TD>");
                fprintf(dot,"<TD>%c </TD>",mbr.mbr_partition_2.part_type);
                fprintf(dot,"</TR>\n");

                fprintf(dot,"<TR>\n");
                fprintf(dot,"<TD>part_fit_2</TD>");
                fprintf(dot,"<TD>%c </TD>",mbr.mbr_partition_2.part_fit);
                fprintf(dot,"</TR>\n");

                fprintf(dot,"<TR>\n");
                fprintf(dot,"<TD>part_start_2</TD>");
                fprintf(dot,"<TD>%d </TD>",mbr.mbr_partition_2.part_start);
                fprintf(dot,"</TR>\n");

                fprintf(dot,"<TR>\n");
                fprintf(dot,"<TD>part_size_2</TD>");
                fprintf(dot,"<TD>%d </TD>",mbr.mbr_partition_2.part_size);
                fprintf(dot,"</TR>\n");

                fprintf(dot,"<TR>\n");
                fprintf(dot,"<TD>part_name_2</TD>");
                fprintf(dot,"<TD>%s </TD>",mbr.mbr_partition_2.part_name);
                fprintf(dot,"</TR>\n");
            }
            if(mbr.mbr_partition_3.part_status == 'S'){
                fprintf(dot,"<TR>\n");
                fprintf(dot,"<TD>part_status_3</TD>");
                fprintf(dot,"<TD>%c</TD>",mbr.mbr_partition_3.part_status);
                fprintf(dot,"</TR>\n");

                fprintf(dot,"<TR>\n");
                fprintf(dot,"<TD>part_type_3</TD>");
                fprintf(dot,"<TD>%c </TD>",mbr.mbr_partition_3.part_type);
                fprintf(dot,"</TR>\n");

                fprintf(dot,"<TR>\n");
                fprintf(dot,"<TD>part_fit_3</TD>");
                fprintf(dot,"<TD>%c </TD>",mbr.mbr_partition_3.part_fit);
                fprintf(dot,"</TR>\n");

                fprintf(dot,"<TR>\n");
                fprintf(dot,"<TD>part_start_3</TD>");
                fprintf(dot,"<TD>%d </TD>",mbr.mbr_partition_3.part_start);
                fprintf(dot,"</TR>\n");

                fprintf(dot,"<TR>\n");
                fprintf(dot,"<TD>part_size_3</TD>");
                fprintf(dot,"<TD>%d </TD>",mbr.mbr_partition_3.part_size);
                fprintf(dot,"</TR>\n");

                fprintf(dot,"<TR>\n");
                fprintf(dot,"<TD>part_name_3</TD>");
                fprintf(dot,"<TD>%s </TD>",mbr.mbr_partition_3.part_name);
                fprintf(dot,"</TR>\n");
            }
            if(mbr.mbr_partition_4.part_status == 'S'){
                fprintf(dot,"<TR>\n");
                fprintf(dot,"<TD>part_status_4</TD>");
                fprintf(dot,"<TD>%c</TD>",mbr.mbr_partition_4.part_status);
                fprintf(dot,"</TR>\n");

                fprintf(dot,"<TR>\n");
                fprintf(dot,"<TD>part_type_4</TD>");
                fprintf(dot,"<TD>%c</TD>",mbr.mbr_partition_4.part_type);
                fprintf(dot,"</TR>\n");

                fprintf(dot,"<TR>\n");
                fprintf(dot,"<TD>part_fit_4</TD>");
                fprintf(dot,"<TD>%c</TD>",mbr.mbr_partition_4.part_fit);
                fprintf(dot,"</TR>\n");

                fprintf(dot,"<TR>\n");
                fprintf(dot,"<TD>part_start_4</TD>");
                fprintf(dot,"<TD>%d</TD>",mbr.mbr_partition_4.part_start);
                fprintf(dot,"</TR>\n");

                fprintf(dot,"<TR>\n");
                fprintf(dot,"<TD>part_size_4</TD>");
                fprintf(dot,"<TD>%d</TD>",mbr.mbr_partition_4.part_size);
                fprintf(dot,"</TR>\n");

                fprintf(dot,"<TR>\n");
                fprintf(dot,"<TD>part_name_4</TD>");
                fprintf(dot,"<TD>%s</TD>",mbr.mbr_partition_4.part_name);
                fprintf(dot,"</TR>\n");
            }


            //SI EXISTE ALGUNA PARTICION EXTENDIDA SE DEBEN MOSTRAR LOS DATOS.
            if(mbr.mbr_partition_1.part_type == 'e' || mbr.mbr_partition_1.part_type == 'E'
                    ||mbr.mbr_partition_2.part_type == 'e' || mbr.mbr_partition_2.part_type == 'E'
                    ||mbr.mbr_partition_3.part_type == 'e' || mbr.mbr_partition_3.part_type == 'E'
                    ||mbr.mbr_partition_4.part_type == 'e' || mbr.mbr_partition_4.part_type == 'E'){

                EBR ebr;
                ebr = getEBR(ruta);

                fprintf(dot,"<TR>\n");
                fprintf(dot,"<TD>NOMBRE</TD>");
                fprintf(dot,"<TD>VALOR</TD>");
                fprintf(dot,"</TR>\n");

                fprintf(dot,"<TR>\n");
                fprintf(dot,"<TD>part_status_1</TD>");
                fprintf(dot,"<TD>%c</TD>",ebr.part_status);
                fprintf(dot,"</TR>\n");

                fprintf(dot,"<TR>\n");
                fprintf(dot,"<TD>part_fit_1</TD>");
                fprintf(dot,"<TD>%c </TD>",ebr.part_fit);
                fprintf(dot,"</TR>\n");

                fprintf(dot,"<TR>\n");
                fprintf(dot,"<TD>part_start_1</TD>");
                fprintf(dot,"<TD>%d </TD>",ebr.part_start);
                fprintf(dot,"</TR>\n");

                fprintf(dot,"<TR>\n");
                fprintf(dot,"<TD>part_size_1</TD>");
                fprintf(dot,"<TD>%d </TD>",ebr.part_size);
                fprintf(dot,"</TR>\n");

                fprintf(dot,"<TR>\n");
                fprintf(dot,"<TD>part_next_1</TD>");
                fprintf(dot,"<TD>%d </TD>",ebr.part_next);
                fprintf(dot,"</TR>\n");

                fprintf(dot,"<TR>\n");
                fprintf(dot,"<TD>part_name_1</TD>");
                fprintf(dot,"<TD>%s </TD>",ebr.part_name);
                fprintf(dot,"</TR>\n");
            }

            fprintf(dot,"</TABLE>\n");
            fprintf(dot,">, ];\n");
            fprintf(dot,"}");

            fclose(dot);
            fflush(dot);

            /* system("dot -Tpng /home/osuna/Escritorio/Reporte.dot -o /home/osuna/Escritorio/Reporte.png");
            system("gwenview /home/osuna/Escritorio/Reporte.png");*/


        }else if(strcasecmp(Name,"disk")==0){

            float porcentaje = 0.0;
            int ultimaposicion = 0;
            int aux = 0;
            FILE *dot;
            dot = fopen("/home/osuna/Escritorio/Reporte.dot","w+");


            fprintf(dot,"digraph html {\n");
            fprintf(dot,"abc [shape=none, margin=0, label=<\n");
            fprintf(dot," <TABLE BORDER=\"0\" CELLBORDER=\"1\" CELLSPACING=\"0\" CELLPADDING=\"4\">\n");

            fprintf(dot,"<TR>\n");
            aux = porcentaje;
            float KBytes = sizeof(MBR) *1000;
            porcentaje = ((float)sizeof(MBR)/(float)mbr.mbr_tamao);
            porcentaje = porcentaje*100;

            fprintf(dot,"<TD ROWSPAN=\"3\"><FONT COLOR=\"red\">MBR</FONT><BR/>%0.5f %c  \n %0.2f KB.</TD>\n",porcentaje,'%',KBytes);
            ultimaposicion = aux;

            if(mbr.mbr_partition_1.part_status == 'S'){

                porcentaje = mbr.mbr_partition_1.part_size;
                float KB = porcentaje*1000;
                porcentaje = ((float)porcentaje/(float)mbr.mbr_tamao);
                porcentaje = porcentaje*100;

                if(mbr.mbr_partition_1.part_type == 'p' || mbr.mbr_partition_1.part_type == 'P'){
                    fprintf(dot,"<TD ROWSPAN=\"3\"><FONT COLOR=\"red\">%s  PRIMARIA </FONT><BR/>%0.2f %c  \n %0.2f M.</TD>\n",mbr.mbr_partition_1.part_name,porcentaje,'%',KB);
                }
                else {
                    fprintf(dot,"<TD COLSPAN=\"3\"><FONT COLOR=\"red\">%s  EXTENDIDA </FONT><BR/>%0.2f %c  \n %0.2f M.</TD>\n",mbr.mbr_partition_1.part_name,porcentaje,'%',KB);
                }
                ultimaposicion = mbr.mbr_partition_1.part_start + mbr.mbr_partition_1.part_size;
            }
            if(mbr.mbr_partition_2.part_status == 'S'){

                if(ultimaposicion < mbr.mbr_partition_2.part_start){
                    porcentaje = mbr.mbr_partition_2.part_start - ultimaposicion;
                    float KB = porcentaje*1000;
                    porcentaje = ((float)porcentaje/(float)mbr.mbr_tamao);
                    porcentaje = porcentaje*100;

                    fprintf(dot,"<TD ROWSPAN=\"3\"><FONT COLOR=\"red\">LIBRE</FONT><BR/>%0.2f %c  \n %0.2f M.</TD>\n",porcentaje,'%',KB);
                    ultimaposicion = mbr.mbr_partition_2.part_start + mbr.mbr_partition_2.part_size;
                }

                porcentaje = mbr.mbr_partition_2.part_size;
                float KB = porcentaje*1000;
                porcentaje = ((float)porcentaje/(float)mbr.mbr_tamao);
                porcentaje = porcentaje*100;

                if(mbr.mbr_partition_2.part_type == 'p' || mbr.mbr_partition_2.part_type == 'P'){
                    fprintf(dot,"<TD ROWSPAN=\"3\"><FONT COLOR=\"red\">%s  PRIMARIA </FONT><BR/>%0.2f %c  \n %0.2f M.</TD>\n",mbr.mbr_partition_2.part_name,porcentaje,'%',KB);
                }
                else {
                    fprintf(dot,"<TD COLSPAN=\"3\"><FONT COLOR=\"red\">%s  EXTENDIDA </FONT><BR/>%0.2f %c  \n %0.2f KB.</TD>\n",mbr.mbr_partition_2.part_name,porcentaje,'%',KB);
                }
                ultimaposicion = mbr.mbr_partition_2.part_start + mbr.mbr_partition_2.part_size;

            }
            if(mbr.mbr_partition_3.part_status == 'S'){

                if(ultimaposicion < mbr.mbr_partition_3.part_start){
                    porcentaje = mbr.mbr_partition_3.part_start - ultimaposicion;
                    float KB = porcentaje*1000;
                    porcentaje = ((float)porcentaje/(float)mbr.mbr_tamao);
                    porcentaje = porcentaje*100;

                    fprintf(dot,"<TD ROWSPAN=\"3\"><FONT COLOR=\"red\">LIBRE</FONT><BR/>%0.2f %c  \n %0.2f M.</TD>\n",porcentaje,'%',KB);
                    ultimaposicion = mbr.mbr_partition_3.part_start + mbr.mbr_partition_3.part_size;
                }

                porcentaje = mbr.mbr_partition_3.part_size;
                float KB = porcentaje*1000;
                porcentaje = ((float)porcentaje/(float)mbr.mbr_tamao);
                porcentaje = porcentaje*100;

                if(mbr.mbr_partition_3.part_type == 'p' || mbr.mbr_partition_3.part_type == 'P'){
                    fprintf(dot,"<TD ROWSPAN=\"3\"><FONT COLOR=\"red\">%s  PRIMARIA </FONT><BR/>%0.2f %c  \n %0.2f M.</TD>\n",mbr.mbr_partition_3.part_name,porcentaje,'%',KB);
                }
                else {
                    fprintf(dot,"<TD COLSPAN=\"3\"><FONT COLOR=\"red\">%s  EXTENDIDA </FONT><BR/>%0.2f %c  \n %0.2f M.</TD>\n",mbr.mbr_partition_3.part_name,porcentaje,'%',KB);
                }
                ultimaposicion = mbr.mbr_partition_3.part_start + mbr.mbr_partition_3.part_size;
            }
            if(mbr.mbr_partition_4.part_status == 'S'){
                if(ultimaposicion < mbr.mbr_partition_4.part_start){
                    porcentaje = mbr.mbr_partition_4.part_start - ultimaposicion;
                    float KB = porcentaje*1000;
                    porcentaje = ((float)porcentaje/(float)mbr.mbr_tamao);
                    porcentaje = porcentaje*100;

                    fprintf(dot,"<TD ROWSPAN=\"3\"><FONT COLOR=\"red\">LIBRE</FONT><BR/>%0.2f %c  \n %0.2f M.</TD>\n",porcentaje,'%',KB);
                    ultimaposicion = mbr.mbr_partition_4.part_start + mbr.mbr_partition_4.part_size;
                }

                porcentaje = mbr.mbr_partition_4.part_size;
                float KB = porcentaje*1000;
                porcentaje = ((float)porcentaje/(float)mbr.mbr_tamao);
                porcentaje = porcentaje*100;

                if(mbr.mbr_partition_4.part_type == 'p' || mbr.mbr_partition_4.part_type == 'P'){
                    fprintf(dot,"<TD ROWSPAN=\"3\"><FONT COLOR=\"red\">%s  PRIMARIA </FONT><BR/>%0.2f %c  \n %0.2f M.</TD>\n",mbr.mbr_partition_4.part_name,porcentaje,'%',KB);
                }
                else {
                    fprintf(dot,"<TD COLSPAN=\"3\"><FONT COLOR=\"red\">%s  EXTENDIDA </FONT><BR/>%0.2f %c  \n %0.2f M.</TD>\n",mbr.mbr_partition_4.part_name,porcentaje,'%',KB);
                }
                ultimaposicion = mbr.mbr_partition_4.part_start + mbr.mbr_partition_4.part_size;
            }

            if((sizeof(MBR)+ mbr.mbr_partition_1.part_size+mbr.mbr_partition_2.part_size+mbr.mbr_partition_3.part_size+mbr.mbr_partition_4.part_size) < mbr.mbr_tamao){
                if(mbr.mbr_partition_4.part_status == 'S'){
                    porcentaje = (mbr.mbr_tamao -(mbr.mbr_partition_4.part_start+ mbr.mbr_partition_4.part_size));
                }else{
                    if(mbr.mbr_partition_3.part_status == 'S'){
                        porcentaje = (mbr.mbr_tamao -(mbr.mbr_partition_3.part_start+ mbr.mbr_partition_3.part_size));
                    }else{
                        if(mbr.mbr_partition_2.part_status == 'S'){
                            porcentaje = (mbr.mbr_tamao -(mbr.mbr_partition_2.part_start+ mbr.mbr_partition_2.part_size));
                        }else{
                            if(mbr.mbr_partition_1.part_status == 'S'){
                                porcentaje = (mbr.mbr_tamao -(mbr.mbr_partition_1.part_start+ mbr.mbr_partition_1.part_size));
                            }else{
                                porcentaje = (mbr.mbr_tamao -(sizeof(MBR)));
                            }
                        }
                    }
                }
                float KB = porcentaje*1000;
                porcentaje = ((float)porcentaje/(float)mbr.mbr_tamao);
                porcentaje = porcentaje*100;

                fprintf(dot,"<TD ROWSPAN=\"3\"><FONT COLOR=\"red\">LIBRE</FONT><BR/>%0.2f %c  \n %0.2f M.</TD>\n",porcentaje,'%',KB);
            }
            fprintf(dot,"</TR>");


            //SI EXISTE ALGUNA PARTICION EXTENDIDA SE DEBEN MOSTRAR LOS DATOS.
            if(mbr.mbr_partition_1.part_type == 'e' || mbr.mbr_partition_1.part_type == 'E'
                    ||mbr.mbr_partition_2.part_type == 'e' || mbr.mbr_partition_2.part_type == 'E'
                    ||mbr.mbr_partition_3.part_type == 'e' || mbr.mbr_partition_3.part_type == 'E'
                    ||mbr.mbr_partition_4.part_type == 'e' || mbr.mbr_partition_4.part_type == 'E'){

                fprintf(dot,"<TR>");
                EBR ebr = getEBR(ruta);
                porcentaje = ebr.part_size;

                if(mbr.mbr_partition_1.part_type == 'e' || mbr.mbr_partition_1.part_type == 'E'){

                    float KB = porcentaje*1000;
                    porcentaje = ((float)porcentaje/(float)mbr.mbr_partition_1.part_size);
                    porcentaje = porcentaje*100;

                    fprintf(dot,"<TD><FONT COLOR=\"red\">EBR</FONT><BR/>%0.5f %c  \n %0.2f M.</TD>\n",porcentaje,'%',KB);

                    if((ebr.part_start+ ebr.part_size) < mbr.mbr_partition_1.part_size){

                        porcentaje = mbr.mbr_partition_1.part_size - ebr.part_size;
                        float KB = porcentaje*1000;
                        porcentaje = ((float)porcentaje/(float)mbr.mbr_partition_1.part_size);
                        porcentaje = porcentaje*100;
                        fprintf(dot,"<TD ROWSPAN=\"3\"><FONT COLOR=\"red\">LIBRE</FONT><BR/>%0.2f %c  \n %0.2f M.</TD>\n",porcentaje,'%',KB);
                    }
                }else if((mbr.mbr_partition_2.part_type == 'e' || mbr.mbr_partition_2.part_type == 'E')){

                    float KB = porcentaje*1000;
                    porcentaje = ((float)porcentaje/(float)mbr.mbr_partition_2.part_size);
                    porcentaje = porcentaje*100;

                    fprintf(dot,"<TD><FONT COLOR=\"red\">EBR</FONT><BR/>%0.5f %c  \n %0.2f M.</TD>\n",porcentaje,'%',KB);

                    if((ebr.part_start+ ebr.part_size) < mbr.mbr_partition_2.part_size){

                        porcentaje = mbr.mbr_partition_2.part_size - ebr.part_size;
                        float KB = porcentaje*1000;
                        porcentaje = ((float)porcentaje/(float)mbr.mbr_partition_2.part_size);
                        porcentaje = porcentaje*100;

                        fprintf(dot,"<TD ROWSPAN=\"3\"><FONT COLOR=\"red\">LIBRE</FONT><BR/>%0.2f %c  \n %0.2f M.</TD>\n",porcentaje,'%',KB);
                    }
                }
                else if((mbr.mbr_partition_3.part_type == 'e' || mbr.mbr_partition_3.part_type == 'E')){

                    float KB = porcentaje*1000;
                    porcentaje = ((float)porcentaje/(float)mbr.mbr_partition_3.part_size);
                    porcentaje = porcentaje*100;

                    fprintf(dot,"<TD><FONT COLOR=\"red\">EBR</FONT><BR/>%0.5f %c  \n %0.2f M.</TD>\n",porcentaje,'%',KB);
                    if((ebr.part_start+ ebr.part_size) < mbr.mbr_partition_3.part_size){

                        porcentaje = mbr.mbr_partition_3.part_size - ebr.part_size;
                        float KB = porcentaje*1000;
                        porcentaje = ((float)porcentaje/(float)mbr.mbr_partition_3.part_size);
                        porcentaje = porcentaje*100;

                        fprintf(dot,"<TD ROWSPAN=\"3\"><FONT COLOR=\"red\">LIBRE</FONT><BR/>%0.2f %c  \n %0.2f M.</TD>\n",porcentaje,'%',KB);
                    }
                }
                else if((mbr.mbr_partition_4.part_type == 'e' || mbr.mbr_partition_4.part_type == 'E')){

                    float KB = porcentaje*1000;
                    porcentaje = ((float)porcentaje/(float)mbr.mbr_partition_4.part_size);
                    porcentaje = porcentaje*100;

                    fprintf(dot,"<TD><FONT COLOR=\"red\">EBR</FONT><BR/>%0.5f %c  \n %0.2f M.</TD>\n",porcentaje,'%',KB);
                    if((ebr.part_start+ ebr.part_size) < mbr.mbr_partition_4.part_size){

                        porcentaje = mbr.mbr_partition_4.part_size - ebr.part_size;
                        float KB = porcentaje*1000;
                        porcentaje = ((float)porcentaje/(float)mbr.mbr_partition_4.part_size);
                        porcentaje = porcentaje*100;

                        fprintf(dot,"<TD ROWSPAN=\"3\"><FONT COLOR=\"red\">LIBRE</FONT><BR/>%0.2f %c  \n %0.2f M.</TD>\n",porcentaje,'%',KB);
                    }
                }
                fprintf(dot,"</TR>");
            }
            fprintf(dot,"  </TABLE>>];\n");
            fprintf(dot,"}");
            fclose(dot);
            fflush(dot);
        }
        else if(strcasecmp(Name,"inode") == 0){

            SUPERBLOQUE sb;
            sb = getSB(UsuarioLog.Path,UsuarioLog.StartParticion);

            FILE *dot;
            dot = fopen("/home/osuna/Escritorio/Reporte.dot","w+");

            fprintf(dot,"digraph Inodos{\n");
            fprintf(dot,"rankdir=\"LR\";\n");
            fprintf(dot,"node [shape=record fontname=Arial];\n");

            /*capturaremos el bitmap de inode */
            char Bitmap[NumeroStructuras];
            memset(&Bitmap,'\0',sizeof(Bitmap));

            char *bit = getBIINODO(UsuarioLog.Path,sb.s_bm_inode_start);
            int indice =0;
            while (*bit != NULL) {
                char letra = *bit;
                Bitmap[indice] = letra;
                bit++;
                indice++;
            }

            int apuntador = 0;
            for (int var = 0; var < NumeroStructuras; ++var) {

                if(Bitmap[var] == '1'){     //POR CADA INODO SIO ESTE EXISTE EN ESA POSICION
                    TABLAINODOS inodo = getInodo(UsuarioLog.Path,sb.s_inode_start + (var * sizeof(TABLAINODOS)));

                    int i_uid = inodo.i_uid;
                    int i_gid = inodo.i_gid;
                    int i_size = inodo.i_size;
                    //ctime
                    //a_time
                    //mtime
                    int apuntadores[15];
                    memset(&apuntadores,-1,sizeof(apuntadores));

                    for (int var2 = 0; var2 < 15; ++var2) {
                        apuntadores[var2] = inodo.i_block[var2];
                    }

                    char type = inodo.i_type;
                    int perm = inodo.i_perm;

                    /*
                        //EJEMPLO POR CADA INODO
                        NUMERO  [label="one\nltwo three\nlfour five six seven\n"]
                    */
                    fprintf(dot,"%d  ",var);
                    fprintf(dot,"[label = \"  INODO:%d",var);
                    fprintf(dot,"%c",92);           //salto de linea
                    fprintf(dot,"n");               //salto de linea
                    fprintf(dot,"i_uid:    %d",i_uid);
                    fprintf(dot,"%c",92);           //salto de linea
                    fprintf(dot,"n");               //salto de linea
                    fprintf(dot,"i_gid:    %d  ",i_gid);
                    fprintf(dot,"%c",92);           //salto de linea
                    fprintf(dot,"n");               //salto de linea
                    fprintf(dot,"i_size:    %d  ",i_size);
                    fprintf(dot,"%c",92);           //salto de linea
                    fprintf(dot,"n");               //salto de linea
                    fprintf(dot,"i_atime:    %s  ",ctime(&inodo.i_atime));
                    fprintf(dot,"%c",92);           //salto de linea
                    fprintf(dot,"n");               //salto de linea
                    fprintf(dot,"i_ctime:    %s  ",ctime(&inodo.i_ctime));
                    fprintf(dot,"%c",92);           //salto de linea
                    fprintf(dot,"n");               //salto de linea
                    fprintf(dot,"i_mtime:    %s  ",ctime(&inodo.i_mtime));
                    fprintf(dot,"%c",92);           //salto de linea
                    fprintf(dot,"n");               //salto de linea

                    //ctime,atime,mtime
                    for (int var1 = 0; var1 < 15; var1++) {

                        fprintf(dot,"i_block%d:      ",var1);
                        fprintf(dot,"%d  ",inodo.i_block[var1]);
                        fprintf(dot,"%c",92);           //salto de linea
                        fprintf(dot,"n");               //salto de linea
                    }
                    fprintf(dot,"i_type:    %c  ",type);
                    fprintf(dot,"%c",92);           //salto de linea
                    fprintf(dot,"n");               //salto de linea
                    fprintf(dot,"i_perm:    %d  \"]\n",perm);

                    if(var != 0){
                        fprintf(dot,"%d",apuntador);
                        fprintf(dot,"->");
                        fprintf(dot,"%d\n",var);

                        apuntador = var;
                    }else{
                        apuntador = var;
                    }
                }
            }
            fprintf(dot,"}");

            fclose(dot);
            fflush(dot);


        }
        else if(strcasecmp(Name,"block") == 0){

            SUPERBLOQUE sb;
            sb = getSB(UsuarioLog.Path,UsuarioLog.StartParticion);

            FILE *dot;
            dot = fopen("/home/osuna/Escritorio/Reporte.dot","w+");

            fprintf(dot,"digraph Inodos{\n");
            fprintf(dot,"rankdir=\"LR\";\n");
            fprintf(dot,"node [shape=record fontname=Arial];\n");

            /*capturaremos el bitmap de inode */
            char Bitmap[NumeroStructuras];
            memset(&Bitmap,'\0',sizeof(Bitmap));

            char *bit = getBIINODO(UsuarioLog.Path,sb.s_bm_inode_start);
            int indice =0;
            while (*bit != NULL) {
                char letra = *bit;
                Bitmap[indice] = letra;
                bit++;
                indice++;
            }

            int apuntador = 1;
            int id = 0;
            for (int var = 0; var < NumeroStructuras; ++var) {

                if(Bitmap[var] == '1'){     //POR CADA INODO SIO ESTE EXISTE EN ESA POSICION
                    TABLAINODOS inodo = getInodo(UsuarioLog.Path,sb.s_inode_start + (var * sizeof(TABLAINODOS)));

                    if(inodo.i_type == '0'){
                        //capturaremos un bloque tipo carpeta

                        for (int bloque = 0; bloque < 15; ++bloque){

                            if(inodo.i_block[bloque] != -1){
                                id++;
                                BLOQUECARPETA carpeta = getBloqueCarpeta(sb.s_block_start + (inodo.i_block[bloque] * sizeof(BLOQUEARCHIVO)));
                                //quiza deberia hacer un salto de linea cada 25 caracteres para que el cuadro no quede kuy grande

                                fprintf(dot,"%d  ",id);
                                fprintf(dot,"[label = \"  BLOQUE:%d",inodo.i_block[bloque]);
                                fprintf(dot,"%c",92);           //salto de linea
                                fprintf(dot,"n");               //salto de linea
                                fprintf(dot,"b_name   b_inodo");

                                fprintf(dot,"%c",92);           //salto de linea
                                fprintf(dot,"n");               //salto de linea

                                for (int pos = 0; pos < 4; ++pos) {
                                    fprintf(dot,"%s   ",carpeta.Content[pos].b_name);
                                    fprintf(dot,"%d",carpeta.Content[pos].b_inodo);
                                    fprintf(dot,"%c",92);           //salto de linea
                                    fprintf(dot,"n");               //salto de linea
                                }
                                fprintf(dot,"\"]\n");

                            }

                            if(id != 1 && (id != apuntador)){
                                fprintf(dot,"%d",apuntador);
                                fprintf(dot,"->");
                                fprintf(dot,"%d\n",id);

                                apuntador = id;
                            }else{
                                apuntador = id;
                            }
                        }


                    }else if(inodo.i_type == '1'){
                        //capturaremos un inoddo tipo archivo

                        for (int bloque = 0; bloque < 15; ++bloque){

                            if(inodo.i_block[bloque] != -1){
                                id++;
                                char *contenido = getArchivo(UsuarioLog.Path,sb.s_block_start + (inodo.i_block[bloque] * sizeof(BLOQUEARCHIVO)));
                                //quiza deberia hacer un salto de linea cada 25 caracteres para que el cuadro no quede kuy grande

                                char cont[64];
                                memset(&cont,'\0',sizeof(cont));
                                int indice = 0;
                                while (*contenido != NULL) {
                                    char letra = *contenido;
                                    cont[indice] = letra;
                                    contenido++;
                                    indice++;
                                }

                                fprintf(dot,"%d  ",id);
                                fprintf(dot,"[label = \"  BLOQUE:%d",inodo.i_block[bloque]);
                                fprintf(dot,"%c",92);           //salto de linea
                                fprintf(dot,"n");               //salto de linea
                                for (int letra = 0; letra < 64; ++letra) {
                                    if(cont[letra] != '\0'){
                                        if(cont[letra] == '\n'){
                                            fprintf(dot,"%c",92);           //salto de linea
                                            fprintf(dot,"n");               //salto de linea
                                        }else{
                                            fprintf(dot,"%c",cont[letra]);           //salto de linea
                                        }
                                    }
                                }
                                fprintf(dot,"\"]\n");

                            }

                            if(id != 1 && (id != apuntador)){
                                fprintf(dot,"%d",apuntador);
                                fprintf(dot,"->");
                                fprintf(dot,"%d\n",id);

                                apuntador = id;
                            }else{
                                apuntador = id;
                            }
                        }
                    }
                }
            }

            fprintf(dot,"}");

            fclose(dot);
            fflush(dot);
        }else if(strcasecmp(Name,"bm_inode") == 0){

            SUPERBLOQUE sb;
            sb = getSB(UsuarioLog.Path,UsuarioLog.StartParticion);

            FILE *dot;
            dot = fopen("/home/osuna/Escritorio/BM_INODE.txt","w+");

            char Bitmap[NumeroStructuras];
            memset(&Bitmap,'\0',sizeof(Bitmap));

            char *bit = getBIINODO(UsuarioLog.Path,sb.s_bm_inode_start);
            int indice =0;
            while (*bit != NULL) {
                char letra = *bit;
                Bitmap[indice] = letra;
                bit++;
                indice++;
            }
            int columna = 0;
            for (int var = 0; var < NumeroStructuras; ++var) {

                if(columna < 20){
                    if(Bitmap[var] == '1'){
                        fprintf(dot,"1");
                    }else{
                        fprintf(dot,"0");
                    }
                    columna++;
                }else{
                    columna =0;
                    fprintf(dot,"\n");
                    if(Bitmap[var] == '1'){
                        fprintf(dot,"1");
                    }else{
                        fprintf(dot,"0");
                    }
                    columna++;
                }
            }
            fclose(dot);
            fflush(dot);

            return;
        }else if(strcasecmp(Name,"bm_block") == 0){
            SUPERBLOQUE sb;
            sb = getSB(UsuarioLog.Path,UsuarioLog.StartParticion);

            FILE *dot;
            dot = fopen("/home/osuna/Escritorio/BM_BLOCK.txt","w+");

            char Bitmap[3*NumeroStructuras];
            memset(&Bitmap,'\0',sizeof(Bitmap));

            char *bit = getBIBLOQUE(UsuarioLog.Path,sb.s_bm_block_start);
            int indice =0;
            while (*bit != NULL) {
                char letra = *bit;
                Bitmap[indice] = letra;
                bit++;
                indice++;
            }
            int columna = 0;
            for (int var = 0; var < 3*NumeroStructuras; ++var) {

                if(columna < 20){
                    if(Bitmap[var] == '1'){
                        fprintf(dot,"1");
                    }else{
                        fprintf(dot,"0");
                    }
                    columna++;
                }else{
                    columna =0;
                    fprintf(dot,"\n");
                    if(Bitmap[var] == '1'){
                        fprintf(dot,"1");
                    }else{
                        fprintf(dot,"0");
                    }
                    columna++;
                }
            }
            fclose(dot);
            fflush(dot);
            return;

        }else if(strcasecmp(Name,"sb") == 0){
            SUPERBLOQUE sb = getSB(UsuarioLog.Path,UsuarioLog.StartParticion);
            FILE *dot;
            dot = fopen("/home/osuna/Escritorio/Reporte.dot","w+");

            fprintf(dot,"digraph test {\n");
            fprintf(dot,"graph [ratio=fill];\n");
            fprintf(dot,"node [label=\"\N\", fontsize=15, shape=plaintext];\n");
            fprintf(dot,"graph [bb=\"0,0,352,154\"];\n");
            fprintf(dot,"arset [label=<\n");
            fprintf(dot,"<TABLE ALIGN=\"LEFT\">\n");

            fprintf(dot,"<TR>\n");
            fprintf(dot,"<TD>NOMBRE</TD>");
            fprintf(dot,"<TD>VALOR</TD>");
            fprintf(dot,"</TR>\n");

            fprintf(dot,"<TR>\n");
            fprintf(dot,"<TD>s_filesystem_type</TD>");
            fprintf(dot,"<TD>%d </TD>",sb.s_filesystem_type);
            fprintf(dot,"</TR>\n");

            fprintf(dot,"<TR>\n");
            fprintf(dot,"<TD>s_inodes_count</TD>");
            fprintf(dot,"<TD>%d </TD>",sb.s_inodes_count);
            fprintf(dot,"</TR>\n");

            fprintf(dot,"<TR>\n");
            fprintf(dot,"<TD>s_blocks_count</TD>");
            fprintf(dot,"<TD>%d </TD>",sb.s_blocks_count);
            fprintf(dot,"</TR>\n");

            fprintf(dot,"<TR>\n");
            fprintf(dot,"<TD>s_free_blocks_count</TD>");
            fprintf(dot,"<TD>%d </TD>",sb.s_free_blocks_count);
            fprintf(dot,"</TR>\n");

            fprintf(dot,"<TR>\n");
            fprintf(dot,"<TD>s_free_inodes_count</TD>");
            fprintf(dot,"<TD>%d </TD>",sb.s_free_inodes_count);
            fprintf(dot,"</TR>\n");

            fprintf(dot,"<TR>\n");
            fprintf(dot,"<TD>s_mtime</TD>");
            fprintf(dot,"<TD>%s</TD>",ctime(&sb.s_mtime));
            fprintf(dot,"</TR>\n");

            fprintf(dot,"<TR>\n");
            fprintf(dot,"<TD>s_umtime</TD>");
            fprintf(dot,"<TD>%s</TD>",ctime(&sb.s_umtime));
            fprintf(dot,"</TR>\n");

            fprintf(dot,"<TR>\n");
            fprintf(dot,"<TD>s_mnt_count</TD>");
            fprintf(dot,"<TD>%d</TD>",sb.s_mnt_count);
            fprintf(dot,"</TR>\n");

            fprintf(dot,"<TR>\n");
            fprintf(dot,"<TD>s_magic</TD>");
            fprintf(dot,"<TD>%d</TD>",sb.s_magic);
            fprintf(dot,"</TR>\n");

            fprintf(dot,"<TR>\n");
            fprintf(dot,"<TD>s_inodes_size</TD>");
            fprintf(dot,"<TD>%d</TD>",sb.s_inode_size);
            fprintf(dot,"</TR>\n");

            fprintf(dot,"<TR>\n");
            fprintf(dot,"<TD>s_block_size</TD>");
            fprintf(dot,"<TD>%d</TD>",sb.s_block_size);
            fprintf(dot,"</TR>\n");

            fprintf(dot,"<TR>\n");
            fprintf(dot,"<TD>s_first_ino</TD>");
            fprintf(dot,"<TD>%d</TD>",sb.s_first_ino);
            fprintf(dot,"</TR>\n");

            fprintf(dot,"<TR>\n");
            fprintf(dot,"<TD>s_first_blo</TD>");
            fprintf(dot,"<TD>%d</TD>",sb.s_first_blo);
            fprintf(dot,"</TR>\n");

            fprintf(dot,"<TR>\n");
            fprintf(dot,"<TD>s_bm_inode_start</TD>");
            fprintf(dot,"<TD>%d</TD>",sb.s_bm_inode_start);
            fprintf(dot,"</TR>\n");

            fprintf(dot,"<TR>\n");
            fprintf(dot,"<TD>s_bm_block_start</TD>");
            fprintf(dot,"<TD>%d</TD>",sb.s_bm_block_start);
            fprintf(dot,"</TR>\n");

            fprintf(dot,"<TR>\n");
            fprintf(dot,"<TD>s_inode_start</TD>");
            fprintf(dot,"<TD>%d</TD>",sb.s_inode_start);
            fprintf(dot,"</TR>\n");

            fprintf(dot,"<TR>\n");
            fprintf(dot,"<TD>s_block_start</TD>");
            fprintf(dot,"<TD>%d</TD>",sb.s_block_start);
            fprintf(dot,"</TR>\n");



            fprintf(dot,"</TABLE>\n");
            fprintf(dot,">, ];\n");
            fprintf(dot,"}");

            fclose(dot);
            fflush(dot);


        }else if(strcasecmp(Name,"tree") == 0){

            ID_TREE = 0;
            ID_TREE_PADRE = 0;
            FILE *dot;
            dot = fopen("/home/osuna/Escritorio/Reporte.dot","w+");

            fprintf(dot,"digraph Inodos{\n");
            fprintf(dot,"rankdir=\"LR\";\n");
            fprintf(dot,"node [shape=record fontname=Arial];\n");

            //mandar a capturar todo el contenido
            SUPERBLOQUE sb = getSB(UsuarioLog.Path,UsuarioLog.StartParticion);

            TABLAINODOS raiz = getInodo(UsuarioLog.Path,sb.s_inode_start);
            if(raiz.i_type != '\0' && raiz.i_perm != 0){
                //si la raiz es nula porque se aplico algun loss no debe hacer nada /*validar ese error*/
                Generardot(raiz,dot,sb);
            }

            fprintf(dot,"}");

            fclose(dot);
            fflush(dot);
        }else if(strcasecmp(Name,"file") == 0){

            char *contenido = (char*)malloc(sizeof(contenido));
            contenido = BuscarArchivo(Ruta);

            if(contenido != NULL){
                FILE *dot;
                dot = fopen("/home/osuna/Escritorio/Reporte.dot","w+");

                fprintf(dot,"digraph Inodos{\n");
                fprintf(dot,"rankdir=\"LR\";\n");
                fprintf(dot,"node [shape=record fontname=Arial];\n");

                fprintf(dot,"1 [label = \"%s\"]\n",contenido);

                fprintf(dot,"}");
                fclose(dot);
                fflush(dot);
            }else{
                printf("Error el archivo en la ruta %s NO existe.\n");
                return;
            }


        } else if(strcasecmp(Name,"journaling") == 0){

            if(UsuarioLog.Tipo == 3){

                SUPERBLOQUE sb;
                sb = getSB(UsuarioLog.Path,UsuarioLog.StartParticion);

                FILE *dot;
                dot = fopen("/home/osuna/Escritorio/Reporte.dot","w+");

                fprintf(dot,"digraph Journaling{\n");
                fprintf(dot,"rankdir=\"LR\";\n");
                fprintf(dot,"node [shape=record fontname=Arial];\n");

                int apuntador = 0;
                for (int var = 0; var < NumeroStructuras; ++var) {          //numerostructuras es la cantidad de journaling que hay

                    if(var >= PrimerJournalingLibre){
                        break;
                    }

                    JOURNALING jour = getJOURNALING(UsuarioLog.Path, UsuarioLog.StartParticion + sizeof(SUPERBLOQUE) + (var * sizeof(JOURNALING)));

                    if(jour.Journal_Tipo_Operacion != 0){           //si es 0 signifca que es nulo

                        int tipoOP = jour.Journal_Tipo_Operacion;
                        int tipo = jour.Jorunal_Tipo;
                        //nombre
                        //contenido
                        //fecha
                        int propietario = jour.Journal_Propietario;
                        int permisos = jour.Journal_Permisos;

                        char Contenido[500];
                        memset(&Contenido,'\0',sizeof(Contenido));
                        int i = 0;

                        for (int var = 0; var < strlen(jour.Journal_Contenido); ++var) {

                            char le = 92;
                            char com = 34;
                            if(jour.Journal_Contenido[var] == '\"'){
                                Contenido[i] = le;          //salto de linea
                                i++;
                                Contenido[i] = com;
                                i++;

                            }else{
                                Contenido[i] = jour.Journal_Contenido[var];
                                i++;
                            }
                        }
                        fprintf(dot,"%d  ",var);
                        fprintf(dot,"[label = \"  JOURNALING:%d",var);
                        fprintf(dot,"%c",92);           //salto de linea
                        fprintf(dot,"n");               //salto de linea
                        fprintf(dot,"Tipo_Operacion:    %d",tipoOP);
                        fprintf(dot,"%c",92);           //salto de linea
                        fprintf(dot,"n");               //salto de linea
                        fprintf(dot,"Tipo:    %d  ",tipo);
                        fprintf(dot,"%c",92);           //salto de linea
                        fprintf(dot,"n");               //salto de linea
                        fprintf(dot,"Nombre:    %s  ",jour.Journal_Nombre);
                        fprintf(dot,"%c",92);           //salto de linea
                        fprintf(dot,"n");               //salto de linea
                        fprintf(dot,"Contenido:    %s  ",Contenido);
                        fprintf(dot,"%c",92);           //salto de linea
                        fprintf(dot,"n");               //salto de linea
                        fprintf(dot,"Fecha:    %s  ",ctime(&jour.Jorunal_Fecha));
                        fprintf(dot,"%c",92);           //salto de linea
                        fprintf(dot,"n");               //salto de linea
                        fprintf(dot,"Propietario:    %d  ",propietario);
                        fprintf(dot,"%c",92);           //salto de linea
                        fprintf(dot,"n");               //salto de linea
                        fprintf(dot,"Permisos:    %d  \"]\n",permisos);

                        if(var != 0){
                            fprintf(dot,"%d",apuntador);
                            fprintf(dot,"->");
                            fprintf(dot,"%d\n",var);
                            apuntador = var;

                        }else{
                            apuntador = var;
                        }

                    }//fin if de jour nulo

                }//fin for

                fprintf(dot,"}");

                fclose(dot);
                fflush(dot);

            }else{
                printf("No se puede reportar Journaling debido que no esta formateado en EXT3\n");
                return;
            }
        }else{
            printf("El reporte solicitado no exite.\n");
            return;
        }


        char Destino[250];
        memset(&Destino,'\0',sizeof(Destino));
        //ENVIAMOS A VERIFICAR LA RUTA ESPECIFICADA
        bool para = false;
        char Pataux[50];
        memset(&Pataux,'\0', sizeof(Pataux));
        int indice = 1;
        for (int i = 0; i < 250; i++) {
            if(Path[0] == '/' && i == 0){
                Pataux[0]='/';
                i++;
            }else{
                if(Path[0] != '/' && i == 0){
                    Pataux[0] = '/';
                }
            }
            while (Path[i] != '/' || i == 0) {
                Pataux[indice] = Path[i];
                i++;
                indice++;
                if(Path[i] == '.'){
                    memset(&Pataux,'\0',sizeof(Pataux));
                    para = true;
                    break;
                }
            }
            Pataux[indice++] = '/';
            if(!para){
                VerificacionPath = VerficacionPath(Pataux);
                para = false;
            }else{
                break;
            }
        }
        if(VerificacionPath){
            strcpy(Destino,"dot -Tpng /home/osuna/Escritorio/Reporte.dot -o ");
            strcat(Destino,Path);
            system(Destino);
            char Destino2[250];
            memset(&Destino2,'\0',sizeof(Destino2));
            strcpy(Destino2,"gwenview ");
            strcat(Destino2,Path);
            //system(Destino2);
            printf("Reporte creado con exito.\n");
        }else{
            printf("Path no existe y no se pudo crear.\n");
        }
    }else{
        printf("Error la particion no esta monatada o no existe.\n");
    }
}

void Generardot(TABLAINODOS inodo, FILE *dot,SUPERBLOQUE sb){

    int padre = ID_TREE;
    int nuevop = ID_TREE_PADRE;
    //ID_TREE_PADRE = ID_TREE;

    int i_uid = inodo.i_uid;
    int i_gid = inodo.i_gid;
    int i_size = inodo.i_size;
    int apuntadores[15];
    memset(&apuntadores,-1,sizeof(apuntadores));
    for (int var2 = 0; var2 < 15; ++var2) {
        apuntadores[var2] = inodo.i_block[var2];
    }
    char type = inodo.i_type;
    int perm = inodo.i_perm;

    /*          CREAMOS EL INODO            */

    fprintf(dot,"%d  ",ID_TREE);
    fprintf(dot,"[label = \"i_uid:    %d",i_uid);
    fprintf(dot,"%c",92);           //salto de linea
    fprintf(dot,"n");               //salto de linea
    fprintf(dot,"i_gid:    %d  ",i_gid);
    fprintf(dot,"%c",92);           //salto de linea
    fprintf(dot,"n");               //salto de linea
    fprintf(dot,"i_size:    %d  ",i_size);
    fprintf(dot,"%c",92);           //salto de linea
    fprintf(dot,"n");               //salto de linea
    fprintf(dot,"i_atime:    %s  ",ctime(&inodo.i_atime));
    fprintf(dot,"%c",92);           //salto de linea
    fprintf(dot,"n");               //salto de linea
    fprintf(dot,"i_ctime:    %s  ",ctime(&inodo.i_ctime));
    fprintf(dot,"%c",92);           //salto de linea
    fprintf(dot,"n");               //salto de linea
    fprintf(dot,"i_mtime:    %s  ",ctime(&inodo.i_mtime));
    fprintf(dot,"%c",92);           //salto de linea
    fprintf(dot,"n");               //salto de linea
    for (int var1 = 0; var1 < 15; var1++) {

        fprintf(dot,"i_block%d:      ",var1);
        fprintf(dot,"%d  ",inodo.i_block[var1]);
        fprintf(dot,"%c",92);           //salto de linea
        fprintf(dot,"n");               //salto de linea
    }
    fprintf(dot,"i_type:    %c  ",type);
    fprintf(dot,"%c",92);           //salto de linea
    fprintf(dot,"n");               //salto de linea
    fprintf(dot,"i_perm:    %d  \"]\n",perm);

    fprintf(dot,"%d",ID_TREE_PADRE);
    fprintf(dot,"->");
    fprintf(dot,"%d\n",ID_TREE);

    ID_TREE_PADRE = ID_TREE;
    ID_TREE++;


    /*        SI EL INODO ES DE TIPO CARPETA          */
    if(inodo.i_type == '0'){
        int padraux1 = ID_TREE_PADRE;

        for (int bloque = 0; bloque < 15; ++bloque){
            ID_TREE_PADRE = padraux1;

            if(inodo.i_block[bloque] != -1){
                BLOQUECARPETA carpeta = getBloqueCarpeta(sb.s_block_start + (inodo.i_block[bloque] * sizeof(BLOQUEARCHIVO)));
                fprintf(dot,"%d  ",ID_TREE);
                fprintf(dot,"[label = \"b_name   b_inodo");

                fprintf(dot,"%c",92);           //salto de linea
                fprintf(dot,"n");               //salto de linea

                for (int pos = 0; pos < 4; ++pos) {
                    fprintf(dot,"%s   ",carpeta.Content[pos].b_name);
                    fprintf(dot,"%d",carpeta.Content[pos].b_inodo);
                    fprintf(dot,"%c",92);           //salto de linea
                    fprintf(dot,"n");               //salto de linea
                }
                fprintf(dot,"\"]\n");

                fprintf(dot,"%d",ID_TREE_PADRE);
                fprintf(dot,"->");
                fprintf(dot,"%d\n",ID_TREE);


                ID_TREE_PADRE = ID_TREE;
                int padraux = ID_TREE_PADRE;
                ID_TREE++;

                //int auxint = ID_TREE_PADRE;

                int pos = 0;
                if(bloque == 0 ){
                    pos = pos + 2;
                }
                for (pos = pos; pos < 4; ++pos) {
                    if(carpeta.Content[pos].b_inodo != -1){
                        TABLAINODOS ino = getInodo(UsuarioLog.Path,sb.s_inode_start + (carpeta.Content[pos].b_inodo * sizeof(TABLAINODOS)));
                        Generardot(ino,dot,sb);
                        ID_TREE_PADRE = padraux;
                    }
                }
            }
        }
        ID_TREE_PADRE = padraux1;
    }
    /*          SI EL INODO ES DE TIPO ARCHIVO      */
    else if(inodo.i_type == '1'){
        for (int bloque = 0; bloque < 15; ++bloque){
            if(inodo.i_block[bloque] != -1){
                char *contenido = getArchivo(UsuarioLog.Path,sb.s_block_start + (inodo.i_block[bloque] * sizeof(BLOQUEARCHIVO)));
                char cont[64];
                memset(&cont,'\0',sizeof(cont));
                int indice = 0;
                while (*contenido != NULL) {
                    char letra = *contenido;
                    cont[indice] = letra;
                    contenido++;
                    indice++;
                }
                fprintf(dot,"%d  ",ID_TREE);
                fprintf(dot,"[label = \"");
                for (int letra = 0; letra < 64; ++letra) {
                    if(cont[letra] != '\0'){
                        if(cont[letra] == '\n'){
                            fprintf(dot,"%c",92);           //salto de linea
                            fprintf(dot,"n");               //salto de linea
                        }else{
                            fprintf(dot,"%c",cont[letra]);           //salto de linea
                        }
                    }
                }
                fprintf(dot,"\"]\n");
                fprintf(dot,"%d",ID_TREE_PADRE);
                fprintf(dot,"->");
                fprintf(dot,"%d\n",ID_TREE);
                ID_TREE++;
            }
        }
        ID_TREE_PADRE = ID_TREE;
    }
}

/*----------METODOS GET---------*/

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
        // printf("El Disco Especificado en la path %s No existe.(getMBR)\n",Path);
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
        printf("El disco Especificado No existe.(setMBR)\n");
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

char *getUnit(char comando[500],int i){
    char* UNIT = (char*)malloc(sizeof(UNIT));
    char Unit;
    if(comando[i] == 'U' || comando[i] == 'u'){
        char unit[8];
        memset(&unit,'\0', sizeof(unit));
        unit[0] = comando[i];
        unit[1] = comando[i+1];
        unit[2] = comando[i+2];
        unit[3] = comando[i+3];
        unit[4] = comando[i+4];
        if(strcasecmp(unit, "unit=") == 0){
            i = i + 5;
            while(comando[i] == ' '){
                i++;
            }
            Unit = comando[i];
        }else{
            // printf("Error de sintaxis unit=\n");
        }
        //printf("Unidad %c \n", Unit);
        UNIT = Unit;
    }
    indc = i;
    return UNIT;
}

char *getPath(char comando[500],int i){
    char *ruta = (char*)malloc(sizeof(ruta));
    char Path[250];
    if((comando[i] == 'p' || comando[i] == 'P')                 //PATH PARA CUALQUIER OTRO
            || (comando[i] == 'c' || comando[i] == 'C')         //CONT PARA MKFILE
            || (comando[i] == 'r' || comando[i] == 'R')
            || (comando[i] == 'd' || comando[i] == 'D')){       //RUTA PARA DESTINO
        char path[8];
        memset(&path,'\0', sizeof(path));
        path[0] = comando[i];
        path[1] = comando[i+1];
        path[2] = comando[i+2];
        path[3] = comando[i+3];
        path[4] = comando[i+4];

        if((strcasecmp(path, "path=") == 0)
                || (strcasecmp(path, "cont=") == 0)
                || (strcasecmp(path, "ruta=") == 0)
                || (strcasecmp(path, "dest=") == 0)){
            i = i + 5;
            while(comando[i] == ' '){
                i++;
            }
            //inicializamos el arreglo en nulo
            memset(&Path,'\0', sizeof(Path));
            int indice1 = 0;
            bool primeracomilla = false;           //si viene una path dentro de comillas podra aceptar espacios.
            while((comando[i] != ' ' && comando[i] != '\n') || primeracomilla == true){
                if(comando[i] == '\"'){

                    /*cuando primera comilla sea false significa que no a venido ninguna antes.
                     * cuando venga la primera comilla tomara el valor de true para que siga entrando al while y acepte los espacios
                     * cuando venga otra comilla se tornara false y significa que ya no aceptara mas espacios. */

                    if(primeracomilla == true){
                        primeracomilla = false;
                    }else{
                        primeracomilla = true;
                    }
                    // i++;    //saltamos para que el path no agarre las comillas
                }else{
                    //se concatenara el path
                    Path[indice1] = comando[i];
                    indice1++;
                }
                if(i < strlen(comando)){
                    i++;
                }else{
                    break;
                }
            }
            // printf("\npath %s \n",Path);
        }else{
            printf("Error de sintaxis path=\n");
            //return;
        }
    }
    ruta = Path;
    indc = i;
    return ruta;
}

char *getFile(char comando[500],int i){
    char *ruta = (char*)malloc(sizeof(ruta));
    char Path[250];
    if((comando[i] == 'f' || comando[i] == 'F')){       //RUTA PARA REP
        char path[8];
        memset(&path,'\0', sizeof(path));
        path[0] = comando[i];
        path[1] = comando[i+1];
        path[2] = comando[i+2];
        path[3] = comando[i+3];

        if((strcasecmp(path, "file") == 0)){
            i = i + 6;      //debe adelantar filen=
            while(comando[i] == ' '){
                i++;
            }

            //inicializamos el arreglo en nulo
            memset(&Path,'\0', sizeof(Path));
            int indice1 = 0;
            bool primeracomilla = false;           //si viene una path dentro de comillas podra aceptar espacios.
            while((comando[i] != ' ' && comando[i] != '\n') || primeracomilla == true){
                if(comando[i] == '\"'){

                    /*cuando primera comilla sea false significa que no a venido ninguna antes.
                     * cuando venga la primera comilla tomara el valor de true para que siga entrando al while y acepte los espacios
                     * cuando venga otra comilla se tornara false y significa que ya no aceptara mas espacios. */

                    if(primeracomilla == true){
                        primeracomilla = false;
                    }else{
                        primeracomilla = true;
                    }
                    // i++;    //saltamos para que el path no agarre las comillas
                }else{
                    //se concatenara el path
                    Path[indice1] = comando[i];
                    indice1++;
                }
                if(i < strlen(comando)){
                    i++;
                }else{
                    break;
                }
            }
            // printf("\npath %s \n",Path);
        }else{
            printf("Error de sintaxis fileN= \n");
            //return;
        }
    }
    ruta = Path;
    indc = i;
    return ruta;
}

int getUgo(char comando[500],int i){
    int TamaoSize = 0;

    if((comando[i] == 'u' || comando[i] == 'U')){

        char Size[8];
        memset(&Size, '\0',sizeof(Size));
        Size[0] = comando[i];
        Size[1] = comando[i+1];
        Size[2] = comando[i+2];
        Size[3] = comando[i+3];

        if(strcasecmp(Size, "ugo=") == 0){

            memset(&Size, '\0',sizeof(Size));
            char numero[7];
            memset(&numero,'\0', sizeof(numero));
            //adelanto i hasta s-i-z-e-~-:-~
            i = i + 4;
            while(comando[i] == ' '){
                i++;
            }
            int indice = 0;
            while(comando[i] != ' ' && comando[i] != '\n'){
                numero[indice] = comando[i];
                indice++;
                if(i < strlen(comando)){
                    i++;
                }else{
                    break;
                }
            }
            TamaoSize = atoi(numero);
            //printf("numero %d\n",TamaoSize);

        }else{
            printf("Error en la sintaxis de \"ugo=\".\n");
        }
    }
    indc = i;
    return TamaoSize;
}

int getSize(char comando[500],int i){
    int TamaoSize = 0;

    if((comando[i] == 's' || comando[i] == 'S')){

        char Size[8];
        memset(&Size, '\0',sizeof(Size));
        Size[0] = comando[i];
        Size[1] = comando[i+1];
        Size[2] = comando[i+2];
        Size[3] = comando[i+3];
        Size[4] = comando[i+4];

        if(strcasecmp(Size, "size=") == 0){

            memset(&Size, '\0',sizeof(Size));
            char numero[7];
            memset(&numero,'\0', sizeof(numero));
            //adelanto i hasta s-i-z-e-~-:-~
            i = i + 5;
            while(comando[i] == ' '){
                i++;
            }
            int indice = 0;
            while(comando[i] != ' ' && comando[i] != '\n'){
                numero[indice] = comando[i];
                indice++;
                if(i < strlen(comando)){
                    i++;
                }else{
                    break;
                }
            }
            TamaoSize = atoi(numero);
            //printf("numero %d\n",TamaoSize);

        }else{
            printf("Error en la sintaxis de \"size=\".\n");
        }
    }
    indc = i;
    return TamaoSize;
}

char *getFit(char comando[500],int i){
    char Ajuste[2];
    char auxfit[2];
    char* fito = (char*)malloc(sizeof(fito));
    if(comando[i] == 'f' || comando[i] == 'F'){
        char fit[7];
        memset(&fit,'\0', sizeof(fit));
        fit[0] = comando[i];
        fit[1] = comando[i+1];
        fit[2] = comando[i+2];
        fit[3] = comando[i+3];
        if(strcasecmp(fit, "fit=") == 0){
            memset(&fit,'\0', sizeof(fit));
            i = i + 4;
            while(comando[i] == ' '){
                i++;
            }
            memset(&Ajuste,'\0',sizeof(Ajuste));
            Ajuste[0] = comando[i];
            Ajuste[1] = comando[i+1];
            i = i+2;

            strcpy(auxfit,Ajuste);
            fito = auxfit;
            // printf(auxfit);
        }
    }
    indc = i;
    return fito;
}

char *getType(char coma[500],int i){
    char *TYPE = (char*)malloc(sizeof(TYPE));
    char type[15];
    char Type = '\0';
    memset(&type,'\0',sizeof(type));

    type[0] = coma[i];
    type[1] = coma[i+1];
    type[2] = coma[i+2];
    type[3] = coma[i+3];
    type[4] = coma[i+4];

    if(strcasecmp(type, "type=") == 0){

        i = i + 5;
        while(coma[i] == ' '){
            i++;
        }

        Type = coma[i];

    }else{
        printf("Error en la sintaxis \"type=\".\n");
    }

    TYPE = Type;
    indc = i;
    return TYPE;
}

char *getName(char coma[500],int i){
    char *Nombre = (char*)malloc(sizeof(Nombre));
    char name[15];
    char Name[50];
    memset(&name,'\0',sizeof(name));
    name[0] = coma[i];
    name[1] = coma[i+1];
    name[2] = coma[i+2];
    name[3] = coma[i+3];
    name[4] = coma[i+4];

    if(strcasecmp(name,"name=") == 0){

        i = i + 5;
        while(coma[i] == ' '){
            i++;
        }

        //inicializamos el arreglo en nulo
        memset(&Name,'\0', sizeof(Name));

        int indice1 = 0;
        bool primeracomilla = false;
        while((coma[i] != ' ' && coma[i] != '\n') || primeracomilla == true){
            if(coma[i] == '\"'){

                /*cuando primera comilla sea false significa que no a venido ninguna antes.
                 * cuando venga la primera comilla tomara el valor de true para que siga entrando al while y acepte los espacios
                 * cuando venga otra comilla se tornara false y significa que ya no aceptara mas espacios. */

                if(primeracomilla == true){
                    primeracomilla = false;
                }else{
                    primeracomilla = true;
                }
                // i++;    //saltamos para que el path no agarre las comillas
            }else{
                //se concatenara el path
                if(coma[i] != '\r'){

                    Name[indice1] = coma[i];
                    indice1++;
                }
            }
            if(i < strlen(coma)){
                i++;
            }else{
                break;
            }
        }

        Nombre = Name;
        indc = i;

    }else{
        printf("Error de sintaxis en \"name=\".\n");
    }
    return Nombre;
}

char *getDelete(char coma[500],int i){
    char *DELETE = (char*)malloc(sizeof(DELETE));
    char dele[15];
    char Delete[5];
    memset(&Delete,'\0', sizeof(Delete));
    memset(&dele,'\0', sizeof(dele));

    dele[0] = coma[i];
    dele[1] = coma[i+1];
    dele[2] = coma[i+2];
    dele[3] = coma[i+3];
    dele[4] = coma[i+4];
    dele[5] = coma[i+5];
    dele[6] = coma[i+6];

    if(strcasecmp(dele,"delete=") == 0){
        i = i + 7;
        while(coma[i] == ' '){
            i++;
        }
        if(coma[i +1] == 'a' || coma[i +1] == 'A'){
            strcpy(Delete, "fast");
            i=i+3;
        }else if(coma[i+1] == 'u' || coma[i+1] == 'U' ) {
            strcpy(Delete, "full");
            i=i+3;
        }
        DELETE = Delete;
        indc = i;
    }else{
        printf("Error de sintaxis en \"delete=\".\n");
    }
    return DELETE;
}

int getAdd(char coma[500],int i){
    int Add = 0;
    char add[15];
    char agregarunidades[10];
    memset(&agregarunidades,'\0', sizeof(agregarunidades));
    memset(&add,'\0', sizeof(add));

    add[0] = coma[i];
    add[1] = coma[i+1];
    add[2] = coma[i+2];
    add[3] = coma[i+3];
    if(strcasecmp(add,"add=") == 0){

        i = i + 4;                      //adelanto toda la palabra "Add->"
        while(coma[i] == ' '){
            i++;
        }

        int indice3 = 0;
        while(coma[i] != ' ' && coma[i] != '\n'){
            agregarunidades[indice3] = coma[i];
            indice3++;
            if(i < strlen(coma)){
                i++;
            }else{
                break;
            }
        }
        Add = atoi(agregarunidades);     //sirve para parsera arreglo char a int

    }else{
        printf("Error de sintaxis en \"add=\".\n");
    }
    indc = i;
    return Add;
}

void Verparticiones(char path[250]){
    MBR mbr;
    mbr = getMBR(path);

    printf("Particion 1: %c \n",mbr.mbr_partition_1.part_status);
    printf("Particion 2: %c \n",mbr.mbr_partition_2.part_status);
    printf("Particion 3: %c \n",mbr.mbr_partition_3.part_status);
    printf("Particion 4: %c \n",mbr.mbr_partition_4.part_status);
}

/*----------FIN METODOS GET---------*/

/*-------------------------------------METODOS FASE 2----------------------------------*/

/*--------------------------METODOS GET FASE 2----------------------------------*/

char *getTypeMKFS(char coma[500],int i){

    char *DELETE = (char*)malloc(sizeof(DELETE));
    char dele[15];
    char Delete[5];
    memset(&Delete,'\0', sizeof(Delete));
    memset(&dele,'\0', sizeof(dele));

    dele[0] = coma[i];
    dele[1] = coma[i+1];
    dele[2] = coma[i+2];
    dele[3] = coma[i+3];
    dele[4] = coma[i+4];

    if(strcasecmp(dele,"type=") == 0){
        i = i + 5;
        while(coma[i] == ' '){
            i++;
        }
        if(coma[i +1] == 'a' || coma[i +1] == 'A'){
            strcpy(Delete, "fast");
            i=i+3;
        }else if(coma[i+1] == 'u' || coma[i+1] == 'U' ) {
            strcpy(Delete, "full");
            i=i+3;
        }
        DELETE = Delete;
        indc = i;
    }else{
        printf("Error de sintaxis en \"delete=\".\n");
    }
    return DELETE;
}

int getFS(char coma[500], int i ){
    int FS = 0;;
    char fs[15];
    char Fs[5];

    memset(&fs,'\0',sizeof(fs));
    memset(&Fs,'\0',sizeof(Fs));

    fs[0] = coma[i];
    fs[1] = coma[i+1];
    fs[2] = coma[i+2];

    if(strcasecmp(fs,"fs=") == 0){
        i = i + 3;
        while(coma[i] == ' '){
            i++;
        }
        if(coma[i] == '2' && (coma[i+1] == 'f' || coma[i+1] == 'F' ) && (coma[i+2] == 's' || coma[i+2] == 'S')){
            FS = 2;
        }
        else if(coma[i] == '3' && (coma[i+1] == 'f' || coma[i+1] == 'F' ) && (coma[i+2] == 's' || coma[i+2] == 'S')){
            FS = 3;
        }
    }else{
        printf("Error de sintaxis en Fs= .\n");
    }
    indc = i+2;  // aumneto en fs despues de 2 ó 3
    return FS;
}

void setSB(SUPERBLOQUE sb,char Path[200],int start){
    FILE *Archivo;
    Archivo = fopen(Path,"rb+");

    if(Archivo!=NULL)
    {
        fseek(Archivo,start,SEEK_SET);
        fwrite(&sb,sizeof(SUPERBLOQUE),1,Archivo);
        fclose(Archivo);
        fflush(Archivo);
    }
    else{
        printf("El disco Especificado No existe.(setMBR)\n");
    }
}

SUPERBLOQUE getSB(char Path[250],int start){

    //Lectura del SUPERBLOQUE
    FILE *apun;
    SUPERBLOQUE sb;
    apun = fopen(Path,"rb");
    if (apun != NULL){
        fseek(apun,start,SEEK_SET);
        fread(&sb,sizeof(SUPERBLOQUE),1,apun);
        fclose(apun);
        fflush(apun);

    }else{
        memset(&sb,0,sizeof(SUPERBLOQUE));
        // printf("El Disco Especificado en la path %s No existe.(getMBR)\n",Path);
    }
    return sb;
}

//capturara los valores de usr-pwd-id
char *getUSR(char comando[500],int i){
    char *usr = (char*)malloc(sizeof(usr));
    char USR[15];
    memset(&USR,'\0',sizeof(USR));

    char ID[15];
    memset(&ID,'\0',sizeof(ID));


    if(((comando[i] == 'u' || comando[i] == 'U')
        &&(comando[i+1] == 's' || comando[i+1] == 'S')
        &&(comando[i+2] == 'r' || comando[i+2] == 'R'))
            || ((comando[i] == 'p' || comando[i] == 'P')
                &&(comando[i+1] == 'w' || comando[i+1] == 'W')
                &&(comando[i+2] == 'd' || comando[i+2] == 'D'))
            || ((comando[i] == 'g' || comando[i] == 'G')
                &&(comando[i+1] == 'r' || comando[i+1] == 'R')
                &&(comando[i+2] == 'p' || comando[i+2] == 'P'))){

        char us[8];
        memset(&us,'\0',sizeof(us));

        us[0] = comando[i];
        us[1] = comando[i+1];
        us[2] = comando[i+2];
        us[3] = comando[i+3];

        if((strcasecmp(us,"usr=")==0)
                || (strcasecmp(us,"pwd=")==0)
                || (strcasecmp(us,"grp=")==0)){
            i = i + 4;
            while (comando[i] ==  ' ') {
                i++;
            }

            int indice1 = 0;
            bool primeracomilla = false;           //si viene una path dentro de comillas podra aceptar espacios.
            while((comando[i] != ' ' && comando[i] != '\n') || primeracomilla == true){
                if(comando[i] == '\"'){

                    /*cuando primera comilla sea false significa que no a venido ninguna antes.
                     * cuando venga la primera comilla tomara el valor de true para que siga entrando al while y acepte los espacios
                     * cuando venga otra comilla se tornara false y significa que ya no aceptara mas espacios. */

                    if(primeracomilla == true){
                        primeracomilla = false;
                    }else{
                        primeracomilla = true;
                    }
                    // i++;    //saltamos para que el path no agarre las comillas
                }else{
                    //se concatenara el path
                    USR[indice1] = comando[i];
                    indice1++;
                }
                if(i < strlen(comando)){            //indica que i ha superado el tamaño del comando por lo tanto lo detiene
                    i++;
                }else{
                    break;
                }
            }
            usr = USR;
        }else{
            printf("Error en la sintaxis de usr= o pwd= p grp=.(LOGIN-GETUSR)\n ");
        }

    }else if((comando[i] == 'i' || comando[i] == 'I')
             &&(comando[i+1] == 'd' || comando[i+1] == 'D')){
        char id[8];
        memset(&id,'\0',sizeof(id));

        id[0] = comando[i];
        id[1] = comando[i+1];
        id[2] = comando[i+2];

        if((strcasecmp(id,"id=")==0)){
            i = i + 3;
            while (comando[i] ==  ' ') {
                i++;
            }

            int indice1 = 0;
            bool primeracomilla = false;           //si viene una path dentro de comillas podra aceptar espacios.
            while((comando[i] != ' ' && comando[i] != '\n') || primeracomilla == true){
                if(comando[i] == '\"'){

                    /*cuando primera comilla sea false significa que no a venido ninguna antes.
                     * cuando venga la primera comilla tomara el valor de true para que siga entrando al while y acepte los espacios
                     * cuando venga otra comilla se tornara false y significa que ya no aceptara mas espacios. */

                    if(primeracomilla == true){
                        primeracomilla = false;
                    }else{
                        primeracomilla = true;
                    }
                    // i++;    //saltamos para que el path no agarre las comillas
                }else{
                    //se concatenara el path
                    ID[indice1] = comando[i];
                    indice1++;
                }
                if(i < strlen(comando)){            //indica que i ha superado el tamaño del comando por lo tanto lo detiene
                    i++;
                }else{
                    break;
                }
            }
            usr = ID;
        }else{
            printf("Error en la sintaxis de usr=.(LOGIN-GETUSR)\n ");
        }

    }else{
        //error no es usr
    }
    indc = i;
    return usr;
}

/*--------------------------FIN METODOS GET -----------------------------------*/

int DespejandoN(int TamaoParticion){

    float  N = (float)(TamaoParticion - sizeof(SUPERBLOQUE)) / (float)(1 + 3 + sizeof(TABLAINODOS) + (3*sizeof(BLOQUEARCHIVO)));

    int NumeroEstructuras1 = (int)(floor(N));

    return NumeroEstructuras1;
}

void Mkfs(char comando[500]){
    int i = 0;
    bool Error = false;
    while (comando[i] == ' ') {
        i++;
    }
    i = i+4;
    while(comando[i] == ' '){
        i++;
    }
    //id=
    //type=
    //fs=

    char Fs[4];
    char Id[8];
    char Type[5];     //fast o full

    memset(&Fs,'\0',sizeof(Fs));
    memset(&Id,'\0',sizeof(Id));
    memset(&Type, '\0',sizeof(Type));

    for (i = i; i < 500; i++) {

        if ((comando[i] == 'i' || comando[i] == 'I')
                &&(comando[i+1] == 'd' || comando[i+1] == 'D')){

            char id[15];
            memset(id,'\0',sizeof(id));
            id[0] = comando[i];
            id[1] = comando[i+1];
            id[2] = comando[i+2];
            if(strcasecmp(id,"id=") == 0){
                i = i + 3;
                while(comando[i] == ' '){
                    i++;
                }
                //inicializamos el arreglo en nulo
                memset(Id,'\0', sizeof(Id));
                int indice2 = 0;
                while(comando[i] != ' ' && comando[i] != '\n'){
                    Id[indice2] = comando[i];
                    indice2++;
                    if(i < strlen(comando)){
                        i++;
                    }else{
                        break;
                    }
                }
                printf("Id:  %s\n",Id);
            }else{
                printf("Error de Sintaxis en Id=.\n");
                return;     //paramos todo el metodo de una vez.
            }
        }
        else if((comando[i] == 't' || comando[i] == 'T')
                && (comando[i+1] == 'y' || comando[i+1] == 'Y')
                &&(comando[i+2] == 'p' || comando[i+2] == 'P')
                &&(comando[i+3] == 'e' || comando[i+3] == 'E')){
            char *type = (char*)malloc(sizeof(type));
            type = getTypeMKFS(comando,i);
            int indice = 0;
            memset(&Type,'\0',sizeof(Type));
            while(*type != '\0'){
                char letra = *type;
                Type[indice] = letra;
                type++;
                indice++;
            }
            printf("Type: %s\n",Type);
            i = indc;
            indc = 0;
        }
        else if((comando[i] == 'f' || comando[i] == 'F')
                && (comando[i+1] == 's' || comando[i+1] == 'S')){
            int fs = getFS(comando,i);
            if(fs == 2){
                strcpy(Fs,"2fs");
            }
            else if(fs == 3){
                strcpy(Fs,"3fs");
            }
            else{
                printf("Parametro establecido en Fs erroneo (debe ser 2fs ó 3fs). \n");
            }
            printf("FS: %s\n",Fs);
            i = indc;
            indc = 0;
        }else if(comando[i] == '#'){
            break;
        }else{
            if(comando[i-1] == '\n'){
                break;
            }
            else if(comando[i] == '\0'){
                break;
            }
            else if(comando[i] != '-' && comando[i] != ' ' && comando[i] != '\n' && comando[i] != '\r' && comando[i] != '\r'){
                printf("Error en los parametros del comando Mkfs.\n");
                return;
            }
        }
    }

    //empezamos acciones de mkfs.
    MONTADURAS *aux = primero;
    char NombreParticion[20];
    char PathDisco[250];
    bool Siestamontada = false;

    memset(&NombreParticion,'\0',sizeof(NombreParticion));
    memset(&PathDisco,'\0',sizeof(PathDisco));

    while(aux != NULL){
        if(strcasecmp(aux->Id, Id) == 0){
            strcpy(NombreParticion,aux->Name);
            strcpy(PathDisco,aux->Path);
            aux->tipoF = Fs[0] - '0';
            Siestamontada = true;
            break;
        }else{
            aux = aux->siguiente;
        }
    }
    if(Siestamontada){

        MBR mbr = getMBR(PathDisco);
        int TamaoParticion = 0;
        int StartParticion = 0;
        if(strcasecmp(mbr.mbr_partition_1.part_name, NombreParticion) == 0){
            TamaoParticion = mbr.mbr_partition_1.part_size;
            StartParticion = mbr.mbr_partition_1.part_start;
        }
        else if(strcasecmp(mbr.mbr_partition_2.part_name, NombreParticion) == 0){
            TamaoParticion = mbr.mbr_partition_2.part_size;
            StartParticion = mbr.mbr_partition_2.part_start;
        }
        else if(strcasecmp(mbr.mbr_partition_3.part_name, NombreParticion) == 0){
            TamaoParticion = mbr.mbr_partition_3.part_size;
            StartParticion = mbr.mbr_partition_3.part_start;
        }
        else if(strcasecmp(mbr.mbr_partition_4.part_name, NombreParticion) == 0){
            TamaoParticion = mbr.mbr_partition_4.part_size;
            StartParticion = mbr.mbr_partition_4.part_start;
        }

        int NumeroStruct = 0;
        SUPERBLOQUE sb;
        if(Fs[0] == '2'){
            NumeroStruct = DespejandoN(TamaoParticion);
            NumeroStructuras = NumeroStruct;  // hacemos global el numeor de estructuras que se estaran manejando

            printf("Particion: %s Tamaño: %d\n",NombreParticion,TamaoParticion);

            sb.s_filesystem_type = Fs[0] - '0';
            sb.s_inodes_count = NumeroStruct;
            sb.s_blocks_count = 3*NumeroStruct;
            sb.s_free_blocks_count = (3*NumeroStruct) -2;
            sb.s_free_inodes_count = NumeroStruct-2;
            sb.s_mtime = time(0);
            sb.s_umtime = time(0);
            sb.s_mnt_count = sb.s_mnt_count++;
            sb.s_magic = 201503911;  //averiguar que es esto
            sb.s_inode_size = sizeof(TABLAINODOS);
            sb.s_block_size = sizeof(BLOQUEAPUNTADORES);  //no importa cual bloque ponga ya que todos pesan 64 bits
            sb.s_first_ino = 2;
            sb.s_first_blo = 2;
            sb.s_bm_inode_start = StartParticion + sizeof(SUPERBLOQUE);
            sb.s_bm_block_start = StartParticion + sizeof(SUPERBLOQUE) + NumeroStruct;      //numerostruct = numero de inodos que tendra mi sistema de archivos.
            sb.s_inode_start = StartParticion + sizeof(SUPERBLOQUE) + NumeroStruct + (3*NumeroStruct);
            sb.s_block_start = StartParticion + sizeof(SUPERBLOQUE) + NumeroStruct + (3*NumeroStruct) +(NumeroStruct*sizeof(TABLAINODOS));

        }
        else if (Fs[0] == '3'){
            NumeroStruct = DespejandoNEXT3(TamaoParticion);
            NumeroStructuras = NumeroStruct;  // hacemos global el numeor de estructuras que se estaran manejando

            printf("Particion: %s Tamaño: %d\n",NombreParticion,TamaoParticion);

            sb.s_filesystem_type = Fs[0] - '0';
            sb.s_inodes_count = NumeroStruct;
            sb.s_blocks_count = 3*NumeroStruct;
            sb.s_free_blocks_count = (3*NumeroStruct)-2;
            sb.s_free_inodes_count = NumeroStruct-2;
            sb.s_mtime = time(0);
            sb.s_umtime = time(0);
            sb.s_mnt_count = sb.s_mnt_count++;
            sb.s_magic = 201503911;  //averiguar que es esto
            sb.s_inode_size = sizeof(TABLAINODOS);
            sb.s_block_size = sizeof(BLOQUEAPUNTADORES);  //no importa cual bloque ponga ya que todos pesan 64 bits
            sb.s_first_ino = 2;
            sb.s_first_blo = 2;
            sb.s_bm_inode_start = StartParticion +(NumeroStruct * sizeof(JOURNALING))+ sizeof(SUPERBLOQUE);
            sb.s_bm_block_start = StartParticion +(NumeroStruct * sizeof(JOURNALING))+ sizeof(SUPERBLOQUE) + NumeroStruct;      //numerostruct = numero de inodos que tendra mi sistema de archivos.
            sb.s_inode_start = StartParticion +(NumeroStruct * sizeof(JOURNALING))+ sizeof(SUPERBLOQUE) + NumeroStruct + (3*NumeroStruct);
            sb.s_block_start = StartParticion +(NumeroStruct * sizeof(JOURNALING))+ sizeof(SUPERBLOQUE) + NumeroStruct + (3*NumeroStruct) +(NumeroStruct*sizeof(TABLAINODOS));

        }

        //mandamos a setear el super bloque.
        setSB(sb,PathDisco,StartParticion);

        //------escribimos el bitmap de inodos ------------
        char inodos[NumeroStruct];
        char bloques[ 3 * NumeroStruct];

        for (int var = 0; var < NumeroStruct; ++var) {
            inodos[var] = '0';
        }
        for (int var = 0; var < 3*NumeroStruct; ++var) {
            bloques[var] = '0';
        }
        inodos[0] = '1';        //Inodo Raiz
        inodos[1] = '1';        //bloque asociado a raiz
        bloques[0] = '1';       //Inodo archivo users
        bloques[1] = '1';       //bloque asociado a archivo users

        /*----ESCRIBIMOS LOS BITMAPS-----*/
        setBINODO(PathDisco,sb.s_bm_inode_start,inodos);
        setBBLOQUE(PathDisco,sb.s_bm_block_start,bloques);

        /*----------CREACION DEL INODO RAIZ----------------*/
        TABLAINODOS Raiz;
        Raiz.i_uid = 1;
        Raiz.i_gid = 1;
        Raiz.i_size = 0;
        Raiz.i_atime = time(0);
        Raiz.i_ctime = time(0);
        Raiz.i_mtime = time(0);
        memset(&Raiz.i_block,-1,sizeof(Raiz.i_block));          //inicializamos todos sus punteros a -1
        Raiz.i_type = '0';      //tipo 0 signifca que es carpeta.
        Raiz.i_perm = 664;      //permisos de el archivo.
        //actualizacion de primer puntero para el bloque.
        Raiz.i_block[0] = 0;  // el primer auntador del primer inodo apuntara al bloque 0.

        /*---------CREACION DE BLOQUE ASOCIADO------*/
        BLOQUECARPETA bloqueasociado;

        CONTENTCARPETA actual;
        actual.b_inodo = 0;
        memset(&actual.b_name,'\0',sizeof(actual.b_name));
        strcpy(actual.b_name,"/");
        bloqueasociado.Content[0] = actual;

        CONTENTCARPETA padre;
        padre.b_inodo = 0;
        memset(&padre.b_name,'\0',sizeof(padre.b_name));
        strcpy(padre.b_name,"/");
        bloqueasociado.Content[1] = padre;

        CONTENTCARPETA user;
        user.b_inodo = 1;
        memset(&user.b_name,'\0',sizeof(user.b_name));
        strcpy(user.b_name,"User.txt");
        bloqueasociado.Content[2] = user;

        CONTENTCARPETA vacio;
        vacio.b_inodo = -1;
        memset(&vacio.b_name,'\0',sizeof(vacio.b_name));
        bloqueasociado.Content[3] = vacio;

        /*----------CREACION DEL INODO ARCHIVO USERS----------------*/
        TABLAINODOS archivouser;
        archivouser.i_uid = 1;
        archivouser.i_gid = 1;
        archivouser.i_size = 0;
        archivouser.i_atime = time(0);
        archivouser.i_ctime = time(0);
        archivouser.i_mtime = time(0);
        memset(&archivouser.i_block,-1,sizeof(archivouser.i_block));          //inicializamos todos sus punteros a -1
        archivouser.i_type = '1';      //tipo 1 signifca que es un archivo.
        archivouser.i_perm = 664;      //permisos de el archivo.
        //actualizacion de primer puntero para el bloque.
        archivouser.i_block[0] = 1;  // el primer auntador del primer inodo apuntara al bloque 1.

        //**********algo pasa aca y se pierde siempre el path del disco
        char aux[250];
        memset(&aux,'\0',sizeof(250));
        strcpy(aux,PathDisco);

        /*----------CREACION DE BLOQUE ASOCICADO-----------------*/
        BLOQUEARCHIVO Archivo;
        char contenido[64];
        memset(&contenido,'\0',sizeof(contenido));
        strcpy(contenido,"1,G,root\n1,U,root,root,123\n");
        strcpy(Archivo.b_content,contenido);

        //***************volemos a asignarle el valor que habiamos guardado en aux.
        memset(&PathDisco,'\0',sizeof(PathDisco));
        strcpy(PathDisco,aux);

        /*--------------MANDAMOS A ESCRIBIR LOS INODOS Y BLOQUES QUE UTILIZAMOS------------------*/

        InsertarInodo(PathDisco,((0*sizeof(TABLAINODOS)) + sb.s_inode_start),Raiz);
        InsertarInodo(PathDisco,((1*sizeof(TABLAINODOS)) + sb.s_inode_start),archivouser);
        InsertarBloque_Carpeta(PathDisco,((0*sizeof(BLOQUEARCHIVO)) + sb.s_block_start),bloqueasociado);
        InsertarBloque_Archivo(PathDisco,((1*sizeof(BLOQUEARCHIVO)) + sb.s_block_start),Archivo);

        //RECUPERACION DE ARCHIVO USER

        //char *archivo = (char*)malloc(sizeof(archivo));
        //archivo = getArchivo(PathDisco,sb.s_block_start + (1*sizeof(BLOQUEAPUNTADORES)));
        //printf("Archivo:\n%s\n",archivo);

        //int primerblo = getPrimerBloqueLibre(PathDisco,sb.s_bm_block_start);
        //int primerino = getPrimerInodoLibre(PathDisco,sb.s_bm_inode_start);
        //printf("Primer Bloque: %d\nPrimer Inodo: %d\n",primerblo,primerino);

        /*// para prueba vamos a actiualizar la posicion 2 de inodos
            ActualizarBINODO(PathDisco,sb.s_bm_inode_start,2);
            ActualizarBBLOQUE(PathDisco,sb.s_bm_block_start,2);

            int primerblo1 = getPrimerBloqueLibre(PathDisco,sb.s_bm_block_start);
            int primerino1 = getPrimerInodoLibre(PathDisco,sb.s_bm_inode_start);
            printf("Primer Bloque: %d\nPrimer Inodo: %d\n",primerblo1,primerino1);*/


        //UsuarioLog.Tipo = Fs[0] - '0';

    }else{
        printf("Error el id %s no se encuentra montado.\n",Id);
        return;
    }
}

void Login(char comando[500]){
    int i = 0;

    char USR[15];
    memset(&USR,'\0',sizeof(USR));

    char PWD[15];
    memset(&PWD,'\0',sizeof(PWD));

    char ID[15];
    memset(&ID,'\0',sizeof(ID));

    while (comando[i] == ' ') {
        i++;
    }
    i = i+5;
    while(comando[i] == ' '){
        i++;
    }
    for (i = i; i < strlen(comando); i++) {
        if((comando[i] == 'u' || comando[i] == 'U')
                &&(comando[i+1] == 's' || comando[i+1] == 'S')
                &&(comando[i+2] == 'r' || comando[i+2] == 'R')){

            char *usr = (char*)malloc(sizeof(usr));
            usr = getUSR(comando,i);
            int indice = 0;
            while(*usr != '\0'){
                char letra = *usr;
                USR[indice] = letra;
                usr++;
                indice++;
            }
            i = indc;
            printf("Id_Usuario: %s\n",USR);
        }else if((comando[i] == 'p' || comando[i] == 'P')
                 &&(comando[i+1] == 'w' || comando[i+1] == 'W')
                 &&(comando[i+2] == 'd' || comando[i+2] == 'D')){

            char *pwd = (char*)malloc(sizeof(pwd));
            pwd = getUSR(comando,i);
            int indice = 0;
            while(*pwd != '\0'){
                char letra = *pwd;
                PWD[indice] = letra;
                pwd++;
                indice++;
            }
            i = indc;
            printf("Password: %s\n",PWD);
        }else if((comando[i] == 'i' || comando[i] == 'I')
                 && (comando[i+1] == 'd' || comando[i+1] == 'D')){

            char *id = (char*)malloc(sizeof(id));
            id = getUSR(comando,i);
            int indice = 0;
            while(*id != '\0'){
                char letra = *id;
                ID[indice] = letra;
                id++;
                indice++;
            }
            i = indc;
            printf("Id: %s\n",ID);
        } else if(comando[i] == '#'){
            break;
        }else{
            if(comando[i-1] == '\n'){
                break;
            }
            else if(comando[i] != '-' && comando[i] != ' ' && comando[i] != '\n' && comando[i] != '\r'){
                printf("Error en los parametros del comando login.\n");
                return;
            }
        }
    }
    /*Hacemos validaciones que usuario exista.*/
    char rut[250];
    memset(&rut,'\0',sizeof(rut));
    int inicio = 0;

    MONTADURAS *aux1 = primero;
    bool siexiste11 = false;
    while (aux1 != NULL) {
        if(strcasecmp(aux1->Id,ID) == 0){
            strcpy(rut,aux1->Path);   //path del disco
            inicio = aux1->StartParticion;
            siexiste11 = true;
            break;
        }else{
            aux1 = aux1->siguiente;
        }
    }
    //si existe el mount indicado
    int IdUsuario = 0;
    if(siexiste11){
        //verificamos que exista el usuario
        /*  CAPTURAMOS TODO EL ARCHIVO DE USUARIOS COMPLETO*/

        SUPERBLOQUE sb = getSB(rut,inicio);
        TABLAINODOS inodoUsr = getInodo(rut,sb.s_inode_start + sizeof(TABLAINODOS));

        char ArchivoUser[Bytesmaximosdearchivo];
        memset(&ArchivoUser,'\0',sizeof(ArchivoUser));
        int indice = 0;
        bool sisepudo = false;          //bandera de satisfaccion
        for (int var = 0; var < 15; ++var) {
            if(inodoUsr.i_block[var] != -1){
                char *archivo = (char*)malloc(sizeof(archivo));
                archivo =getArchivo(rut,sb.s_block_start + (inodoUsr.i_block[var] * sizeof(BLOQUEARCHIVO)));
                //int indice = 0;
                while (*archivo != NULL) {
                    char letra = *archivo;
                    ArchivoUser[indice] = letra;
                    archivo++;
                    indice++;
                }

                //vemos si en ese bloque esta el grupo que deseamos eliminar
                bool estado = false;
                char NombreG[12];
                memset(&NombreG,'\0',sizeof(NombreG));
                int ind = 0;
                int posicioncorrelativo = 0;  //numero de bit donde esta el correlativo en la linea
                bool yaestaennombre = false;
                int cantcomas = 0;
                for (int var1 = 0; var1 < strlen(ArchivoUser); var1++) {

                    if(ArchivoUser[var1] == 'U'){
                        posicioncorrelativo = var1-2;
                        var1++;
                        estado = true;
                        cantcomas = 0;
                    }
                    if(estado){
                        if(ArchivoUser[var1] != ','){
                            NombreG[ind] = ArchivoUser[var1];
                            ind++;
                        }
                        else{
                            cantcomas++;
                            /*  esto lo hago para que no se confunda al agarra el nombre de
                            grupo como que fuera el nombre del usuario entonces al encontrar
                            la primera coma reseteara el nombre e indicara que lo que viene
                            ya sera el nombre. y cuando venga otra coma pero ya estaba en estado de nombre verificara el nombre*/
                            if(cantcomas == 3){
                                if(strcmp(USR,NombreG) == 0){
                                    char Posicion = ArchivoUser[posicioncorrelativo];
                                    IdUsuario = Posicion - '0';

                                    char pass[50];
                                    memset(&pass,'\0',sizeof(pass));
                                    int inds = 0;

                                    var1++;     //aumento la coma que viene
                                    while (ArchivoUser[var1] != '\n') {
                                        pass[inds] = ArchivoUser[var1];
                                        var1++;
                                        inds++;
                                    }


                                    if(IdUsuario == 0){
                                        printf("Usuario No existe.\n");
                                        memset(&UsuarioLog,0,sizeof(UsuarioLog));
                                        memset(&UsuarioLog.id,'\0',sizeof(UsuarioLog.id));
                                        memset(&UsuarioLog.Usuario,'\0',sizeof(UsuarioLog.Usuario));
                                        memset(&UsuarioLog.pwd,'\0',sizeof(UsuarioLog.pwd));

                                        return;
                                    }else if(strcmp(PWD,pass) != 0){
                                        printf("Password Incorrecta.\n");
                                        memset(&UsuarioLog,0,sizeof(UsuarioLog));
                                        memset(&UsuarioLog.id,'\0',sizeof(UsuarioLog.id));
                                        memset(&UsuarioLog.Usuario,'\0',sizeof(UsuarioLog.Usuario));
                                        memset(&UsuarioLog.pwd,'\0',sizeof(UsuarioLog.pwd));

                                        return;
                                    }

                                    estado = false;
                                    sisepudo = true;
                                    yaestaennombre = false;
                                    break;
                                }else{
                                    memset(&NombreG,'\0',sizeof(NombreG));
                                    estado = false;
                                    ind = 0;
                                }
                            }else{
                                memset(&NombreG,'\0',sizeof(NombreG));
                                ind = 0;
                            }
                        }
                    }
                }
                memset(&ArchivoUser,'\0',sizeof(ArchivoUser));
                indice = 0;
            }
        }
        if(sisepudo == false){
            printf("Usuario no existe.\n");
            return;
        }

    }else{
        printf("Id de particion especificado no se encuentra en particiones montadas.\n");
        return;
    }


    //Iniciamos Sesion Cargamos en maemoria el usiario.
    strcpy(UsuarioLog.id,ID);                   //id asignado al montar particion
    strcpy(UsuarioLog.pwd,PWD);                 //passwword del usuario
    strcpy(UsuarioLog.Usuario,USR);             //nombre del usuario
    UsuarioLog.IDUsuario = IdUsuario;
    //UsuarioLog.Tipo = 0;
    //al momento de hacer el login tambien debo guardar el path de la particion.
    MONTADURAS *aux = primero;
    bool siexiste1 = false;
    while (aux != NULL) {
        if(strcasecmp(aux->Id,ID) == 0){
            strcpy(UsuarioLog.Path,aux->Path);   //path del disco
            strcpy(UsuarioLog.Name,aux->Name);   //nombre particion
            UsuarioLog.StartParticion = aux->StartParticion;
            UsuarioLog.Tipo = aux->tipoF;       //al usuario le agregamos el tipo de formato que tiene la particion en la que se loguea
            siexiste1 = true;
            break;
        }else{
            aux = aux->siguiente;
        }
    }
    if(!siexiste1){
        printf("Id especificado no se encuentra en particiones montadas.\n");
        memset(&UsuarioLog,0,sizeof(USUARIOLOGUEADO));
    }else{
        //insertamos un journaling
        if(UsuarioLog.Tipo == 3 && Bandera_Esto_En_Recovery == false){
            JOURNALING nuevo;

            nuevo.Journal_Tipo_Operacion = 1;
            nuevo.Jorunal_Tipo = 0;
            memset(&nuevo.Journal_Nombre,'\0',sizeof(nuevo.Journal_Nombre));
            strcpy(nuevo.Journal_Nombre,"Login");
            memset(&nuevo.Journal_Contenido,'\0',sizeof(nuevo.Journal_Contenido));
            strcpy(nuevo.Journal_Contenido,comando);
            nuevo.Jorunal_Fecha = time(0);
            nuevo.Journal_Propietario = 1;
            nuevo.Journal_Permisos = 664;

            SUPERBLOQUE sb;
            sb = getSB(UsuarioLog.Path,UsuarioLog.StartParticion);
            //seguido del super bloque empezaran los journaling
            //siempre y cuando sea paraticion en ext3

            setJOURNALING(nuevo,UsuarioLog.Path,UsuarioLog.StartParticion +sizeof(SUPERBLOQUE) + (PrimerJournalingLibre * sizeof(JOURNALING)));
            PrimerJournalingLibre++;

        }

        printf("Has Iniciado sesion %s.\n",UsuarioLog.Usuario);
    }
}

void setJOURNALING(JOURNALING journaling,char Path[200],int start){
    FILE *Archivo;
    Archivo = fopen(Path,"rb+");

    if(Archivo!=NULL)
    {
        fseek(Archivo,start,SEEK_SET);
        fwrite(&journaling,sizeof(JOURNALING),1,Archivo);
        fclose(Archivo);
        fflush(Archivo);
    }
    else{
        printf("El disco Especificado No existe.(setJOURNALING)\n");
    }
}

JOURNALING getJOURNALING(char Path[200],int start){

    JOURNALING jour;
    memset(&jour,'0',sizeof(jour));

    jour.Journal_Tipo_Operacion = 0;

    FILE *Archivo;
    Archivo = fopen(Path,"rb+");

    if(Archivo!=NULL)
    {
        fseek(Archivo,start,SEEK_SET);
        fread(&jour,sizeof(JOURNALING),1,Archivo);
        fclose(Archivo);
        fflush(Archivo);


    }
    else{
        printf("El disco Especificado No existe.(setJOURNALING)\n");
    }

    return jour;
}

void Logout(){
    //USUARIOLOGUEADO aux = UsuarioLog;
    if((strcasecmp(UsuarioLog.Usuario,"\0") != 0) && (strcasecmp(UsuarioLog.pwd,"\0") != 0) &&(strcasecmp(UsuarioLog.id,"\0") != 0)){
        printf("Se ha cerrado sesion (%s) hasta luego.\n",UsuarioLog.Usuario);
        memset(&UsuarioLog,0,sizeof(USUARIOLOGUEADO));
    }else{
        printf("Error no hay sesion activa que cerrar.\n");
    }
}

void LsUsers(){
    SUPERBLOQUE sb = getSB(UsuarioLog.Path,UsuarioLog.StartParticion);
    TABLAINODOS inodoUsr = getInodo(UsuarioLog.Path,sb.s_inode_start + sizeof(TABLAINODOS));
    printf("***********Usuarios y Grupos*********\n");
    for (int var = 0; var < 15; ++var) {
        if(inodoUsr.i_block[var] != -1){
            char *archivo = (char*)malloc(sizeof(archivo));
            archivo =getArchivo(UsuarioLog.Path,sb.s_block_start + (inodoUsr.i_block[var] * sizeof(BLOQUEARCHIVO)));
            printf(archivo);
        }
    }
}

void Mkfile(char comando[500]){
    int i = 0;
    while (comando[i] == ' ') {
        i++;
    }
    i = i+6;
    while(comando[i] == ' '){
        i++;
    }

    char PATH[250];
    memset(&PATH,'\0',sizeof(PATH));

    char CONT[250];
    memset(&CONT,'\0',sizeof(CONT));

    int TamaoSize = 0;

    bool ParaP = false;

    for (i = i; i < 500; i++) {
        if((comando[i] == 'p' || comando[i] == 'P')
                &&(comando[i+1] == 'a' || comando[i+1] == 'A')
                &&(comando[i+2] == 't' || comando[i+2] == 'T')
                &&(comando[i+3] == 'h' || comando[i+3] == 'H')){

            char *path = (char*)malloc(sizeof(path));
            path = getPath(comando,i);

            int indice =0;
            while (*path != NULL) {
                char letra = *path;
                PATH[indice] = letra;
                path++;
                indice++;
            }
            i = indc;
            printf("Path: %s\n",PATH);
        }else if((comando[i] == 'c' || comando[i] == 'C')
                 &&(comando[i+1] == 'o' || comando[i+1] == 'O')
                 &&(comando[i+2] == 'n' || comando[i+2] == 'N')
                 &&(comando[i+3] == 't' || comando[i+3] == 'T')){

            char *cont = (char*)malloc(sizeof(cont));
            cont = getPath(comando,i);

            int indice =0;
            while (*cont != NULL) {
                char letra = *cont;
                CONT[indice] = letra;
                cont++;
                indice++;
            }
            i = indc;
            printf("Cont: %s\n",CONT);
        }
        else if((comando[i] == 'p' || comando[i] == 'P')
                &&(comando[i-1] == '-')
                &&(comando[i+1] == ' ')){
            //viene el parametro P lo que indica que todas las carpetas si no existen debe crearlas
            //cuando no viene las carpetas que no existen solo se notifica un error
            ParaP = true;
            i++;
        }else if((comando[i] == 's' || comando[i] == 'S')
                 &&(comando[i+1] == 'i' || comando[i+1] == 'I')
                 &&(comando[i+2] == 'z' || comando[i+2] == 'Z')
                 &&(comando[i+3] == 'e' || comando[i+3] == 'E')){

            TamaoSize = getSize(comando,i);
            printf("Size->%d\n",TamaoSize);
            i = indc;
            indc = 0;
        }else if(comando[i] == '#'){

            break;
        }else{
            if(comando[i-1] == '\n'){
                break;
            }
            else if(comando[i] == '\0'){
                break;
            }
            else if(comando[i] != '-' && comando[i] != ' ' && comando[i] != '\n' && comando[i] != '\r' && comando[i] != '/'){
                printf("Error en los parametros del comando mkfile.\n");
                return;
            }
        }
    }

    /*HACER LA VALIDACION QUE SI YA EXISTE UN ARCHIVO CON ESE NOMBRE.*/
    if(ParaP){
        //si viene el parametro p creara todas las carpetas si no existen.
        ParametroP = true;

        char Archivo[500];
        memset(&Archivo,'\0',sizeof(Archivo));

        if((strcasecmp(CONT, "\0") != 0)){
            //mandamos a capturar el texto del archivo de mi disco fisico

            char *cont = (char*)malloc(sizeof(cont));
            FILE *apun;
            apun = fopen(CONT,"rb");
            if (apun != NULL){

                fseek(apun,0,SEEK_SET);
                fread(&Archivo,sizeof(Archivo),1,apun);

                fclose(apun);
                fflush(apun);

                /* int indice = 0;
                while (*cont != NULL) {
                    char letra = *cont;
                    Archivo[indice] = letra;
                    indice++;
                    cont++;
                }*/

            }else{
                printf("Archivo Especificado en Cont no existe.\n");
                return;
            }

            printf("contenido Archivo: %s\n",Archivo);

            SeparaRutaMkfile(PATH,Archivo);

        }

        else if(TamaoSize != 0 && (strcasecmp(CONT, "\0") == 0)){

            if(TamaoSize < 0){
                printf("Size no permitido.\n");
                return;
            }

            char contenido[TamaoSize+2];
            memset(&contenido,'\0',sizeof(contenido));

            int caracter = 0;
            for (int var = 0; var < TamaoSize; ++var) {
                if(var < 10){
                    contenido[var] = caracter + '0';
                    caracter++;
                }else if(caracter == 10){
                    caracter = 0;
                    contenido[var] = caracter + '0';
                    caracter++;
                }else{
                    contenido[var] = caracter + '0';
                    caracter++;
                }
            }
            char co[500];
            memset(&co,'\0',sizeof(co));
            strcpy(co,contenido);

            SeparaRutaMkfile(PATH,co);
        }

    }else{
        // si no viene el parametro si no existe alguna carpeta debe detectar error.


        if(TamaoSize < 0){
            printf("Size no permitido.\n");
            return;
        }
        else {
            char contenido[TamaoSize+2];
            memset(&contenido,'\0',sizeof(contenido));

            int caracter = 0;
            for (int var = 0; var < TamaoSize; ++var) {
                if(var < 10){
                    contenido[var] = caracter + '0';
                    caracter++;
                }else if(caracter == 10){
                    caracter = 0;
                    contenido[var] = caracter + '0';
                    caracter++;
                }else{
                    contenido[var] = caracter + '0';
                    caracter++;
                }
            }
            char co[500];
            memset(&co,'\0',sizeof(co));

            strcpy(CONT,contenido);    //copiamos el contenido a una arreglo auxiliar porque contenido siempre perdia la info
        }

        ParametroP = false;
        SeparaRutaMkfile(PATH,CONT); //debo mandar el contenido que deberia tener el archivo con el size
    }

}

void Mkdir(char comando[500]){
    int i = 0;
    while (comando[i] == ' ') {
        i++;
    }
    i = i+5;
    while(comando[i] == ' '){
        i++;
    }

    char PATH[250];
    memset(&PATH,'\0',sizeof(PATH));

    bool ParaP = false;

    for (i = i; i < 500; i++) {
        if((comando[i] == 'p' || comando[i] == 'P')
                &&(comando[i+1] == 'a' || comando[i+1] == 'A')
                &&(comando[i+2] == 't' || comando[i+2] == 'T')
                &&(comando[i+3] == 'h' || comando[i+3] == 'H')){

            char *path = (char*)malloc(sizeof(path));
            path = getPath(comando,i);

            int indice =0;
            while (*path != NULL) {
                char letra = *path;
                PATH[indice] = letra;
                path++;
                indice++;
            }
            i = indc;
            printf("Path: %s\n",PATH);
        }
        else if((comando[i] == 'p' || comando[i] == 'P')
                &&(comando[i-1] == '-')
                &&(comando[i+1] == ' ')){
            //viene el parametro P lo que indica que todas las carpetas si no existen debe crearlas
            //cuando no viene las carpetas que no existen solo se notifica un error
            ParaP = true;
            //i = i +2;
        }else if(comando[i] == '#'){

            break;
        }else{
            if(comando[i-1] == '\n'){
                break;
            }
            else if(comando[i] == '\0'){
                break;
            }
            else if(comando[i] != '-' && comando[i] != ' ' && comando[i] != '\n' && comando[i] != '\r' && comando[i] != '/'){
                printf("Error en los parametros del comando mkdir.\n");
                return;
            }
        }
    }

    /* VERIFICAMOS SI VIENE P Y EMPEZAMOS A CREAR LAS CARPETAS*/
    if(ParaP){
        //si viene el parametro p creara todas las carpetas si no existen.
        ParametroP = true;
        SepararRutaMkdir(PATH);
        
    }else{
        // si no viene el parametro si no existe alguna carpeta debe detectar error.
        ParametroP = false;
        SepararRutaMkdir(PATH);
    }

}

void SeparaRutaMkfile(char RUTA[250],char contenido[500]){

    char co[500];
    memset(&co,'\0',sizeof(co));
    strcpy(co,contenido);

    SUPERBLOQUE sb = getSB(UsuarioLog.Path,UsuarioLog.StartParticion);

    bool para = false;
    TABLAINODOS padre;
    padre = getInodo(UsuarioLog.Path,sb.s_inode_start);

    char Pataux[250];
    memset(&Pataux,'\0', sizeof(Pataux));

    int indice = 1;
    for (int i = 0; i < strlen(RUTA); i++) {

        if(RUTA[0] == '/' && i == 0){
            Pataux[0]= '/';
            //mandamos a verificar si existe esa carpeta creada en la particion
            //si existe devolvera el nodo actual
            padre = VerificacionExisteCarpeta(padre, Pataux);
            memset(&Pataux,'\0',sizeof(Pataux));
            i++;
            indice = 0;
        }else{
            if(RUTA[0] != '/' && i == 0){
                Pataux[0] = '/';
            }
        }
        while ((RUTA[i] != '/' || i == 0) &&  i < 250){
            Pataux[indice] = RUTA[i];
            i++;
            indice++;
            if(RUTA[i] == '.'){
                //memset(&Pataux,'\0',sizeof(Pataux));
                Pataux[indice] = RUTA[i];
                i++;
                indice++;
                para = true;
                //break;
            }
        }
        if(!para){
            //mandamos a crear bloque de tipo archivo.
            para = false;
        }else{
            //strcat(Pataux,"txt");
            CrearArchivo(padre,Pataux,contenido);
            break;
            //return;
        }
        //mandamos a verificar si existe la carpeta
        //si existe devolvera el nodo actual
        //si devuelve nulo significa que esa carpeta no existe en el padre.
        TABLAINODOS aux = padre;
        padre = VerificacionExisteCarpeta(padre,Pataux);

        //el nodo no exite pero el parametro p es true en el comando mkdir
        //lo que significa que debemos crear esa carpta.
        if(padre.i_type == '\0'){
            if(ParametroP){         //si el parametro o vino en el comando se crea si no error
                aux = CrearCarpeta(aux,Pataux);
                padre = VerificacionExisteCarpeta(aux,Pataux);
            }else{
                printf("Carpeta %s no existe.\n",Pataux);
                return;
            }
        }
        indice = 0;
        memset(&Pataux,'\0',sizeof(Pataux));

    }
}

void SepararRutaMkdir(char RUTA[250]){

    SUPERBLOQUE sb = getSB(UsuarioLog.Path,UsuarioLog.StartParticion);

    bool para = false;
    TABLAINODOS padre;
    padre = getInodo(UsuarioLog.Path,sb.s_inode_start);

    char Pataux[250];
    memset(&Pataux,'\0', sizeof(Pataux));

    int indice = 1;
    for (int i = 0; i < strlen(RUTA); i++) {

        if(RUTA[0] == '/' && i == 0){
            Pataux[0]= '/';
            //mandamos a verificar si existe esa carpeta creada en la particion
            //si existe devolvera el nodo actual
            padre = VerificacionExisteCarpeta(padre, Pataux);
            memset(&Pataux,'\0',sizeof(Pataux));
            i++;
            indice = 0;
        }else{
            if(RUTA[0] != '/' && i == 0){
                Pataux[0] = '/';
            }
        }
        while ((RUTA[i] != '/' || i == 0) && i < 250){
            Pataux[indice] = RUTA[i];
            i++;
            indice++;
            if(RUTA[i] == '.'){
                memset(&Pataux,'\0',sizeof(Pataux));
                para = true;
                break;
            }
        }
        if(!para){
            //mandamos a crear bloque de tipo archivo.
            para = false;
        }else{
            //strcat(Pataux,"txt");
            //CrearArchivo(padre,Pataux);
            printf("En Mkdir no se pueden crear Archivos prueba con MKFILE.\n");
            return;
        }
        //mandamos a verificar si existe la carpeta
        //si existe devolvera el nodo actual
        //si devuelve nulo significa que esa carpeta no existe en el padre.
        TABLAINODOS aux = padre;
        padre = VerificacionExisteCarpeta(padre,Pataux);

        //el nodo no exite pero el parametro p es true en el comando mkdir
        //lo que significa que debemos crear esa carpta.
        if(padre.i_type == '\0'){
            if(ParametroP){         //si el parametro o vino en el comando se crea si no error
                aux = CrearCarpeta(aux,Pataux);
                padre = VerificacionExisteCarpeta(aux,Pataux);
                //if()
            }else{
               if(i == 250){
                    ParametroP = true;
                    aux = CrearCarpeta(aux,Pataux);
                    padre = VerificacionExisteCarpeta(aux,Pataux);
                    ParametroP = false;
                    //padre = VerificacionExisteCarpeta(aux,Pataux);
                    //return;
                }else{

                    printf("Carpeta %s no existe.\n",Pataux);
                    return;
                }
//                printf("Carpeta %s no existe.\n",Pataux);
//                return;
            }
        }else{
            // si padre me devuelve algun valor e i es 250 significa
            //la carpeta ya existe por lo tanto no puede crearla
            if(i == 250){
                 printf("Carpeta %s YA existe.\n",Pataux);
            }
        }
        indice = 0;
        memset(&Pataux,'\0',sizeof(Pataux));
    }
}

TABLAINODOS VerificacionExisteCarpeta(TABLAINODOS inodoPadre,char Nombre[25]){

    SUPERBLOQUE sb = getSB(UsuarioLog.Path,UsuarioLog.StartParticion);
    int apuntadorinodohijo = 0;
    TABLAINODOS ExisteHijo;
    memset(&ExisteHijo,0,sizeof(ExisteHijo));

    for (int var = 0; var < 15; ++var) {

        if(inodoPadre.i_block[var] != -1){          //si el apuntadore es diferente de -1
            BLOQUECARPETA carpeta = getBloqueCarpeta(sb.s_block_start + (inodoPadre.i_block[var]*sizeof(BLOQUECARPETA)));

            if(strcmp(carpeta.Content[0].b_name,Nombre) == 0){
                apuntadorinodohijo = carpeta.Content[0].b_inodo;
                ExisteHijo = getInodo(UsuarioLog.Path,sb.s_inode_start + (apuntadorinodohijo*sizeof(TABLAINODOS)));
                break;
            }
            else if(strcmp(carpeta.Content[1].b_name,Nombre) == 0){
                apuntadorinodohijo = carpeta.Content[1].b_inodo;
                ExisteHijo = getInodo(UsuarioLog.Path,sb.s_inode_start + (apuntadorinodohijo*sizeof(TABLAINODOS)));
                break;
            }
            else if(strcmp(carpeta.Content[2].b_name,Nombre) == 0){
                apuntadorinodohijo = carpeta.Content[2].b_inodo;
                ExisteHijo = getInodo(UsuarioLog.Path,sb.s_inode_start + (apuntadorinodohijo*sizeof(TABLAINODOS)));
                break;
            }
            else if(strcmp(carpeta.Content[3].b_name,Nombre) == 0){
                apuntadorinodohijo = carpeta.Content[3].b_inodo;
                ExisteHijo = getInodo(UsuarioLog.Path,sb.s_inode_start + (apuntadorinodohijo*sizeof(TABLAINODOS)));
                break;
            }
        }
    }
    return ExisteHijo;
}

TABLAINODOS CrearArchivo(TABLAINODOS inodoPadre, char NombreArchivo[25], char contenido[500]){

    SUPERBLOQUE sb = getSB(UsuarioLog.Path,UsuarioLog.StartParticion);
    int primerInodoLibre = getPrimerInodoLibre(UsuarioLog.Path,sb.s_bm_inode_start);
    int primeBlloqueLibre = getPrimerBloqueLibre(UsuarioLog.Path,sb.s_bm_block_start);
    BLOQUECARPETA father = getBloqueCarpeta(sb.s_block_start + (inodoPadre.i_block[0]*sizeof(BLOQUECARPETA)));


    for (int var = 0; var < 15; ++var) {

        //busca el apuntador que sea diferente de -1 para buscar en ese bloque para ver si
        //aun tiene espacio disponible algo de lo contrario se ira al apuntador -1 que va a ser
        //un bloque nuevecito de 4 posiciones
        if(inodoPadre.i_block[var] != -1){

            BLOQUECARPETA carpeta = getBloqueCarpeta(sb.s_block_start + (inodoPadre.i_block[var]*sizeof(BLOQUECARPETA)));

            //si el bloque aun tiene un espacio vacio en sus apuntadores ahi inserta el archivo.
            if(carpeta.Content[0].b_inodo == -1){

                carpeta.Content[0].b_inodo = primerInodoLibre;
                memset(&carpeta.Content[0].b_name,'\0',sizeof(carpeta.Content[0].b_name));
                strcpy(carpeta.Content[0].b_name,NombreArchivo);
                InsertarBloque_Carpeta(UsuarioLog.Path,sb.s_block_start + (inodoPadre.i_block[var]*sizeof(BLOQUECARPETA)),carpeta);

                //insertamos el inodo de la nueva carpeta
                TABLAINODOS Inodo;
                Inodo.i_uid = UsuarioLog.IDUsuario;
                Inodo.i_gid = 1;
                Inodo.i_size = strlen(contenido);
                Inodo.i_atime = time(0);
                Inodo.i_ctime = time(0);
                Inodo.i_mtime = time(0);
                memset(&Inodo.i_block,-1,sizeof(Inodo.i_block));          //inicializamos todos sus punteros a -1
                Inodo.i_type = '1';      //tipo 0 signifca que es carpeta.
                Inodo.i_perm = 664;      //permisos de el archivo.
                //actualizacion de primer puntero para el bloque.
                Inodo.i_block[0] = primeBlloqueLibre;  // el primer auntador del primer inodo apuntara al bloque 0.
                InsertarInodo(UsuarioLog.Path,sb.s_inode_start + (primerInodoLibre * sizeof(TABLAINODOS)),Inodo);

                /*peddddodoo*/
                if(strlen(contenido) <= 64){
                    //si la cantidad nueva sigue siendo menor a 64 bits se escribe en el bloque que leimos
                    BLOQUEARCHIVO bloqueasociado;
                    memset(&bloqueasociado.b_content,'\0',sizeof(bloqueasociado.b_content));
                    strcpy(bloqueasociado.b_content,contenido);
                    InsertarBloque_Archivo(UsuarioLog.Path,sb.s_block_start + (primeBlloqueLibre * sizeof(BLOQUECARPETA)),bloqueasociado);

                    ActualizarBINODO(UsuarioLog.Path,sb.s_bm_inode_start,primerInodoLibre);
                    ActualizarBBLOQUE(UsuarioLog.Path,sb.s_bm_block_start,primeBlloqueLibre);

                    printf("Se ha creado el archivo %s\n",NombreArchivo);

                    //RECUPERACION DE ARCHIVO USER
                    // char *archivo = (char*)malloc(sizeof(archivo));
                    //archivo = getArchivo(UsuarioLog.Path,sb.s_block_start + (bloqueenquemequede*sizeof(BLOQUEAPUNTADORES)));
                    //printf("Archivo:\n%s\n",archivo);

                    //int primerblo1 = getPrimerBloqueLibre(UsuarioLog.Path,sb.s_bm_block_start);
                    //int primerino1 = getPrimerInodoLibre(UsuarioLog.Path,sb.s_bm_inode_start);
                    //printf("Primer Bloque: %d\nPrimer Inodo: %d\n",primerblo1,primerino1);
                    return;
                }else{

                    //como el archivo excede los 64 bits haremos uso de mas bloques de contenido
                    char PrimerArchivo[64];
                    memset(&PrimerArchivo,'\0',sizeof(PrimerArchivo));

                    char *pauxiliar = (char*)malloc(sizeof(pauxiliar));
                    pauxiliar = contenido;


                    //completamos los 64 bits del bloque actual
                    int indice = 0;
                    while (indice < 64) {
                        char letra = *pauxiliar;
                        PrimerArchivo[indice] = letra;
                        pauxiliar++;
                        indice++;
                    }

                    BLOQUEARCHIVO bloqueasociado;
                    memset(&bloqueasociado.b_content,'\0',sizeof(bloqueasociado.b_content));
                    strcpy(bloqueasociado.b_content,PrimerArchivo);
                    InsertarBloque_Archivo(UsuarioLog.Path,sb.s_block_start + (primeBlloqueLibre * sizeof(BLOQUECARPETA)),bloqueasociado);

                    ActualizarBINODO(UsuarioLog.Path,sb.s_bm_inode_start,primerInodoLibre);
                    ActualizarBBLOQUE(UsuarioLog.Path,sb.s_bm_block_start,primeBlloqueLibre);

                    //bvolvemos a capturar el primer bloque libre y lo llenamos de nuevo
                    while (*pauxiliar != NULL) {
                        indice = 0;
                        char Arch[64];
                        memset(&Arch,'\0',sizeof(Arch));

                        while (*pauxiliar != NULL) {
                            if(indice < 64){
                                char letra = *pauxiliar;
                                Arch[indice] = letra;
                                pauxiliar++;
                                indice++;

                            }else{
                                break;
                            }
                        }

                        int primebloquelibre = getPrimerBloqueLibre(UsuarioLog.Path,sb.s_bm_block_start);

                        //lo insertamos y actualizamos el apuntador del inodo
                        BLOQUEARCHIVO masnuevo;
                        memset(&masnuevo.b_content,'\0',sizeof(masnuevo.b_content));
                        strcpy(masnuevo.b_content,Arch);
                        InsertarBloque_Archivo(UsuarioLog.Path,sb.s_block_start + (primebloquelibre * sizeof(BLOQUEARCHIVO)),masnuevo);

                        ActualizarBBLOQUE(UsuarioLog.Path,sb.s_bm_block_start,primebloquelibre);

                        //actualizamos el inodo
                        //el primer apuntador -1 que encuentra
                        int var1 =0;
                        for (var1 = 0; var1 < 12; ++var1) {
                            if(Inodo.i_block[var1] == -1){
                                break;
                            }
                        }
                        Inodo.i_block[var1] = primebloquelibre;
                        InsertarInodo(UsuarioLog.Path,sb.s_inode_start + (primerInodoLibre * sizeof(TABLAINODOS)),Inodo);

                        TABLAINODOS inuser = getInodo(UsuarioLog.Path,sb.s_inode_start + sizeof(TABLAINODOS));
                        //RECUPERACION DE ARCHIVO USER aun n o esta
                        //char *archivo = (char*)malloc(sizeof(archivo));
                        //archivo = getArchivo(UsuarioLog.Path,sb.s_block_start + (primebloquelibre*sizeof(BLOQUEAPUNTADORES)));
                        // printf("Archivo:\n%s\n",archivo);
                    }
                }

                /*pedisiimooomo*/


                /*BLOQUEARCHIVO bloqueasociado;
                memset(&bloqueasociado,'\0',sizeof(bloqueasociado));
                InsertarBloque_Archivo(UsuarioLog.Path,sb.s_block_start + (primeBlloqueLibre * sizeof(BLOQUECARPETA)),bloqueasociado);

                ActualizarBINODO(UsuarioLog.Path,sb.s_bm_inode_start,primerInodoLibre);
                ActualizarBBLOQUE(UsuarioLog.Path,sb.s_bm_block_start,primeBlloqueLibre);*/

                printf("Se ha creado el archivo %s\n",NombreArchivo);
                break;
            }
            else if(carpeta.Content[1].b_inodo == -1){
                carpeta.Content[1].b_inodo = primerInodoLibre;
                memset(&carpeta.Content[1].b_name,'\0',sizeof(carpeta.Content[1].b_name));
                strcpy(carpeta.Content[1].b_name,NombreArchivo);
                InsertarBloque_Carpeta(UsuarioLog.Path,sb.s_block_start + (inodoPadre.i_block[var]*sizeof(BLOQUECARPETA)),carpeta);

                //insertamos el inodo de la nueva carpeta
                TABLAINODOS Inodo;
                Inodo.i_uid = UsuarioLog.IDUsuario;
                Inodo.i_gid = 1;
                Inodo.i_size = strlen(contenido);
                Inodo.i_atime = time(0);
                Inodo.i_ctime = time(0);
                Inodo.i_mtime = time(0);
                memset(&Inodo.i_block,-1,sizeof(Inodo.i_block));          //inicializamos todos sus punteros a -1
                Inodo.i_type = '1';      //tipo 0 signifca que es carpeta.
                Inodo.i_perm = 664;      //permisos de el archivo.
                //actualizacion de primer puntero para el bloque.
                Inodo.i_block[0] = primeBlloqueLibre;  // el primer auntador del primer inodo apuntara al bloque 0.
                InsertarInodo(UsuarioLog.Path,sb.s_inode_start + (primerInodoLibre * sizeof(TABLAINODOS)),Inodo);



                /*peddddodoo*/
                if(strlen(contenido) <= 64){
                    //si la cantidad nueva sigue siendo menor a 64 bits se escribe en el bloque que leimos
                    BLOQUEARCHIVO bloqueasociado;
                    memset(&bloqueasociado.b_content,'\0',sizeof(bloqueasociado.b_content));
                    strcpy(bloqueasociado.b_content,contenido);
                    InsertarBloque_Archivo(UsuarioLog.Path,sb.s_block_start + (primeBlloqueLibre * sizeof(BLOQUECARPETA)),bloqueasociado);

                    ActualizarBINODO(UsuarioLog.Path,sb.s_bm_inode_start,primerInodoLibre);
                    ActualizarBBLOQUE(UsuarioLog.Path,sb.s_bm_block_start,primeBlloqueLibre);

                    printf("Se ha creado el archivo %s\n",NombreArchivo);

                    //RECUPERACION DE ARCHIVO USER
                    // char *archivo = (char*)malloc(sizeof(archivo));
                    //archivo = getArchivo(UsuarioLog.Path,sb.s_block_start + (bloqueenquemequede*sizeof(BLOQUEAPUNTADORES)));
                    //printf("Archivo:\n%s\n",archivo);

                    //int primerblo1 = getPrimerBloqueLibre(UsuarioLog.Path,sb.s_bm_block_start);
                    //int primerino1 = getPrimerInodoLibre(UsuarioLog.Path,sb.s_bm_inode_start);
                    //printf("Primer Bloque: %d\nPrimer Inodo: %d\n",primerblo1,primerino1);
                    return;
                }else{

                    //como el archivo excede los 64 bits haremos uso de mas bloques de contenido
                    char PrimerArchivo[64];
                    memset(&PrimerArchivo,'\0',sizeof(PrimerArchivo));

                    char *pauxiliar = (char*)malloc(sizeof(pauxiliar));
                    pauxiliar = contenido;


                    //completamos los 64 bits del bloque actual
                    int indice = 0;
                    while (indice < 64) {
                        char letra = *pauxiliar;
                        PrimerArchivo[indice] = letra;
                        pauxiliar++;
                        indice++;
                    }

                    BLOQUEARCHIVO bloqueasociado;
                    memset(&bloqueasociado.b_content,'\0',sizeof(bloqueasociado.b_content));
                    strcpy(bloqueasociado.b_content,PrimerArchivo);
                    InsertarBloque_Archivo(UsuarioLog.Path,sb.s_block_start + (primeBlloqueLibre * sizeof(BLOQUECARPETA)),bloqueasociado);

                    ActualizarBINODO(UsuarioLog.Path,sb.s_bm_inode_start,primerInodoLibre);
                    ActualizarBBLOQUE(UsuarioLog.Path,sb.s_bm_block_start,primeBlloqueLibre);

                    //bvolvemos a capturar el primer bloque libre y lo llenamos de nuevo
                    while (*pauxiliar != NULL) {
                        indice = 0;
                        char Arch[64];
                        memset(&Arch,'\0',sizeof(Arch));

                        while (*pauxiliar != NULL) {
                            if(indice < 64){
                                char letra = *pauxiliar;
                                Arch[indice] = letra;
                                pauxiliar++;
                                indice++;

                            }else{
                                break;
                            }
                        }

                        int primebloquelibre = getPrimerBloqueLibre(UsuarioLog.Path,sb.s_bm_block_start);

                        //lo insertamos y actualizamos el apuntador del inodo
                        BLOQUEARCHIVO masnuevo;
                        memset(&masnuevo.b_content,'\0',sizeof(masnuevo.b_content));
                        strcpy(masnuevo.b_content,Arch);
                        InsertarBloque_Archivo(UsuarioLog.Path,sb.s_block_start + (primebloquelibre * sizeof(BLOQUEARCHIVO)),masnuevo);

                        ActualizarBBLOQUE(UsuarioLog.Path,sb.s_bm_block_start,primebloquelibre);

                        //actualizamos el inodo
                        //el primer apuntador -1 que encuentra
                        int var1 =0;
                        for (var1 = 0; var1 < 12; ++var1) {
                            if(Inodo.i_block[var1] == -1){
                                break;
                            }
                        }
                        Inodo.i_block[var1] = primebloquelibre;
                        InsertarInodo(UsuarioLog.Path,sb.s_inode_start + (primerInodoLibre * sizeof(TABLAINODOS)),Inodo);

                        TABLAINODOS inuser = getInodo(UsuarioLog.Path,sb.s_inode_start + sizeof(TABLAINODOS));
                        //RECUPERACION DE ARCHIVO USER aun n o esta
                        //char *archivo = (char*)malloc(sizeof(archivo));
                        //archivo = getArchivo(UsuarioLog.Path,sb.s_block_start + (primebloquelibre*sizeof(BLOQUEAPUNTADORES)));
                        // printf("Archivo:\n%s\n",archivo);
                    }
                }


                /*BLOQUEARCHIVO bloqueasociado;
                memset(&bloqueasociado,'\0',sizeof(bloqueasociado));
                InsertarBloque_Archivo(UsuarioLog.Path,sb.s_block_start + (primeBlloqueLibre * sizeof(BLOQUECARPETA)),bloqueasociado);

                ActualizarBINODO(UsuarioLog.Path,sb.s_bm_inode_start,primerInodoLibre);
                ActualizarBBLOQUE(UsuarioLog.Path,sb.s_bm_block_start,primeBlloqueLibre);*/

                printf("Se ha creado el archivo %s\n",NombreArchivo);
                break;
            }
            else if(carpeta.Content[2].b_inodo == -1){
                carpeta.Content[2].b_inodo = primerInodoLibre;
                memset(&carpeta.Content[2].b_name,'\0',sizeof(carpeta.Content[2].b_name));
                strcpy(carpeta.Content[2].b_name,NombreArchivo);
                InsertarBloque_Carpeta(UsuarioLog.Path,sb.s_block_start + (inodoPadre.i_block[var]*sizeof(BLOQUECARPETA)),carpeta);

                //insertamos el inodo de la nueva carpeta
                TABLAINODOS Inodo;
                Inodo.i_uid = UsuarioLog.IDUsuario;
                Inodo.i_gid = 1;
                Inodo.i_size = strlen(contenido);
                Inodo.i_atime = time(0);
                Inodo.i_ctime = time(0);
                Inodo.i_mtime = time(0);
                memset(&Inodo.i_block,-1,sizeof(Inodo.i_block));          //inicializamos todos sus punteros a -1
                Inodo.i_type = '1';      //tipo 0 signifca que es carpeta.
                Inodo.i_perm = 664;      //permisos de el archivo.
                //actualizacion de primer puntero para el bloque.
                Inodo.i_block[0] = primeBlloqueLibre;  // el primer auntador del primer inodo apuntara al bloque 0.
                InsertarInodo(UsuarioLog.Path,sb.s_inode_start + (primerInodoLibre * sizeof(TABLAINODOS)),Inodo);

                /*peddddodoo*/
                if(strlen(contenido) <= 64){
                    //si la cantidad nueva sigue siendo menor a 64 bits se escribe en el bloque que leimos
                    BLOQUEARCHIVO bloqueasociado;
                    memset(&bloqueasociado.b_content,'\0',sizeof(bloqueasociado.b_content));
                    strcpy(bloqueasociado.b_content,contenido);
                    InsertarBloque_Archivo(UsuarioLog.Path,sb.s_block_start + (primeBlloqueLibre * sizeof(BLOQUECARPETA)),bloqueasociado);

                    ActualizarBINODO(UsuarioLog.Path,sb.s_bm_inode_start,primerInodoLibre);
                    ActualizarBBLOQUE(UsuarioLog.Path,sb.s_bm_block_start,primeBlloqueLibre);

                    printf("Se ha creado el archivo %s\n",NombreArchivo);

                    //RECUPERACION DE ARCHIVO USER
                    // char *archivo = (char*)malloc(sizeof(archivo));
                    //archivo = getArchivo(UsuarioLog.Path,sb.s_block_start + (bloqueenquemequede*sizeof(BLOQUEAPUNTADORES)));
                    //printf("Archivo:\n%s\n",archivo);

                    //int primerblo1 = getPrimerBloqueLibre(UsuarioLog.Path,sb.s_bm_block_start);
                    //int primerino1 = getPrimerInodoLibre(UsuarioLog.Path,sb.s_bm_inode_start);
                    //printf("Primer Bloque: %d\nPrimer Inodo: %d\n",primerblo1,primerino1);
                    return;
                }else{

                    //como el archivo excede los 64 bits haremos uso de mas bloques de contenido
                    char PrimerArchivo[64];
                    memset(&PrimerArchivo,'\0',sizeof(PrimerArchivo));

                    char *pauxiliar = (char*)malloc(sizeof(pauxiliar));
                    pauxiliar = contenido;


                    //completamos los 64 bits del bloque actual
                    int indice = 0;
                    while (indice < 64) {
                        char letra = *pauxiliar;
                        PrimerArchivo[indice] = letra;
                        pauxiliar++;
                        indice++;
                    }

                    BLOQUEARCHIVO bloqueasociado;
                    memset(&bloqueasociado.b_content,'\0',sizeof(bloqueasociado.b_content));
                    strcpy(bloqueasociado.b_content,PrimerArchivo);
                    InsertarBloque_Archivo(UsuarioLog.Path,sb.s_block_start + (primeBlloqueLibre * sizeof(BLOQUECARPETA)),bloqueasociado);

                    ActualizarBINODO(UsuarioLog.Path,sb.s_bm_inode_start,primerInodoLibre);
                    ActualizarBBLOQUE(UsuarioLog.Path,sb.s_bm_block_start,primeBlloqueLibre);

                    //bvolvemos a capturar el primer bloque libre y lo llenamos de nuevo
                    while (*pauxiliar != NULL) {
                        indice = 0;
                        char Arch[64];
                        memset(&Arch,'\0',sizeof(Arch));

                        while (*pauxiliar != NULL) {
                            if(indice < 64){
                                char letra = *pauxiliar;
                                Arch[indice] = letra;
                                pauxiliar++;
                                indice++;

                            }else{
                                break;
                            }
                        }

                        int primebloquelibre = getPrimerBloqueLibre(UsuarioLog.Path,sb.s_bm_block_start);

                        //lo insertamos y actualizamos el apuntador del inodo
                        BLOQUEARCHIVO masnuevo;
                        memset(&masnuevo.b_content,'\0',sizeof(masnuevo.b_content));
                        strcpy(masnuevo.b_content,Arch);
                        InsertarBloque_Archivo(UsuarioLog.Path,sb.s_block_start + (primebloquelibre * sizeof(BLOQUEARCHIVO)),masnuevo);

                        ActualizarBBLOQUE(UsuarioLog.Path,sb.s_bm_block_start,primebloquelibre);

                        //actualizamos el inodo
                        //el primer apuntador -1 que encuentra
                        int var1 =0;
                        for (var1 = 0; var1 < 12; ++var1) {
                            if(Inodo.i_block[var1] == -1){
                                break;
                            }
                        }
                        Inodo.i_block[var1] = primebloquelibre;
                        InsertarInodo(UsuarioLog.Path,sb.s_inode_start + (primerInodoLibre * sizeof(TABLAINODOS)),Inodo);

                        TABLAINODOS inuser = getInodo(UsuarioLog.Path,sb.s_inode_start + sizeof(TABLAINODOS));
                        //RECUPERACION DE ARCHIVO USER aun n o esta
                        //char *archivo = (char*)malloc(sizeof(archivo));
                        //archivo = getArchivo(UsuarioLog.Path,sb.s_block_start + (primebloquelibre*sizeof(BLOQUEAPUNTADORES)));
                        // printf("Archivo:\n%s\n",archivo);
                    }
                }


                /*BLOQUEARCHIVO bloqueasociado;
                memset(&bloqueasociado,'\0',sizeof(bloqueasociado));
                InsertarBloque_Archivo(UsuarioLog.Path,sb.s_block_start + (primeBlloqueLibre * sizeof(BLOQUECARPETA)),bloqueasociado);

                ActualizarBINODO(UsuarioLog.Path,sb.s_bm_inode_start,primerInodoLibre);
                ActualizarBBLOQUE(UsuarioLog.Path,sb.s_bm_block_start,primeBlloqueLibre);*/

                printf("Se ha creado el archivo %s\n",NombreArchivo);
                break;
            }
            else if(carpeta.Content[3].b_inodo == -1){

                carpeta.Content[3].b_inodo = primerInodoLibre;
                memset(&carpeta.Content[3].b_name,'\0',sizeof(carpeta.Content[3].b_name));
                strcpy(carpeta.Content[3].b_name,NombreArchivo);
                InsertarBloque_Carpeta(UsuarioLog.Path,sb.s_block_start + (inodoPadre.i_block[var]*sizeof(BLOQUECARPETA)),carpeta);

                //insertamos el inodo de la nueva carpeta
                TABLAINODOS Inodo;
                Inodo.i_uid = UsuarioLog.IDUsuario;
                Inodo.i_gid = 1;
                Inodo.i_size = strlen(contenido);
                Inodo.i_atime = time(0);
                Inodo.i_ctime = time(0);
                Inodo.i_mtime = time(0);
                memset(&Inodo.i_block,-1,sizeof(Inodo.i_block));          //inicializamos todos sus punteros a -1
                Inodo.i_type = '1';      //tipo 0 signifca que es carpeta.
                Inodo.i_perm = 664;      //permisos de el archivo.
                //actualizacion de primer puntero para el bloque.
                Inodo.i_block[0] = primeBlloqueLibre;  // el primer auntador del primer inodo apuntara al bloque 0.
                InsertarInodo(UsuarioLog.Path,sb.s_inode_start + (primerInodoLibre * sizeof(TABLAINODOS)),Inodo);



                /*peddddodoo*/
                if(strlen(contenido) <= 64){
                    //si la cantidad nueva sigue siendo menor a 64 bits se escribe en el bloque que leimos
                    BLOQUEARCHIVO bloqueasociado;
                    memset(&bloqueasociado.b_content,'\0',sizeof(bloqueasociado.b_content));
                    strcpy(bloqueasociado.b_content,contenido);
                    InsertarBloque_Archivo(UsuarioLog.Path,sb.s_block_start + (primeBlloqueLibre * sizeof(BLOQUECARPETA)),bloqueasociado);

                    ActualizarBINODO(UsuarioLog.Path,sb.s_bm_inode_start,primerInodoLibre);
                    ActualizarBBLOQUE(UsuarioLog.Path,sb.s_bm_block_start,primeBlloqueLibre);

                    printf("Se ha creado el archivo %s\n",NombreArchivo);

                    //RECUPERACION DE ARCHIVO USER
                    // char *archivo = (char*)malloc(sizeof(archivo));
                    //archivo = getArchivo(UsuarioLog.Path,sb.s_block_start + (bloqueenquemequede*sizeof(BLOQUEAPUNTADORES)));
                    //printf("Archivo:\n%s\n",archivo);

                    //int primerblo1 = getPrimerBloqueLibre(UsuarioLog.Path,sb.s_bm_block_start);
                    //int primerino1 = getPrimerInodoLibre(UsuarioLog.Path,sb.s_bm_inode_start);
                    //printf("Primer Bloque: %d\nPrimer Inodo: %d\n",primerblo1,primerino1);
                    return;
                }else{

                    //como el archivo excede los 64 bits haremos uso de mas bloques de contenido
                    char PrimerArchivo[64];
                    memset(&PrimerArchivo,'\0',sizeof(PrimerArchivo));

                    char *pauxiliar = (char*)malloc(sizeof(pauxiliar));
                    pauxiliar = contenido;


                    //completamos los 64 bits del bloque actual
                    int indice = 0;
                    while (indice < 64) {
                        char letra = *pauxiliar;
                        PrimerArchivo[indice] = letra;
                        pauxiliar++;
                        indice++;
                    }

                    BLOQUEARCHIVO bloqueasociado;
                    memset(&bloqueasociado.b_content,'\0',sizeof(bloqueasociado.b_content));
                    strcpy(bloqueasociado.b_content,PrimerArchivo);
                    InsertarBloque_Archivo(UsuarioLog.Path,sb.s_block_start + (primeBlloqueLibre * sizeof(BLOQUECARPETA)),bloqueasociado);

                    ActualizarBINODO(UsuarioLog.Path,sb.s_bm_inode_start,primerInodoLibre);
                    ActualizarBBLOQUE(UsuarioLog.Path,sb.s_bm_block_start,primeBlloqueLibre);

                    //bvolvemos a capturar el primer bloque libre y lo llenamos de nuevo
                    while (*pauxiliar != NULL) {
                        indice = 0;
                        char Arch[64];
                        memset(&Arch,'\0',sizeof(Arch));

                        while (*pauxiliar != NULL) {
                            if(indice < 64){
                                char letra = *pauxiliar;
                                Arch[indice] = letra;
                                pauxiliar++;
                                indice++;

                            }else{
                                break;
                            }
                        }

                        int primebloquelibre = getPrimerBloqueLibre(UsuarioLog.Path,sb.s_bm_block_start);

                        //lo insertamos y actualizamos el apuntador del inodo
                        BLOQUEARCHIVO masnuevo;
                        memset(&masnuevo.b_content,'\0',sizeof(masnuevo.b_content));
                        strcpy(masnuevo.b_content,Arch);
                        InsertarBloque_Archivo(UsuarioLog.Path,sb.s_block_start + (primebloquelibre * sizeof(BLOQUEARCHIVO)),masnuevo);

                        ActualizarBBLOQUE(UsuarioLog.Path,sb.s_bm_block_start,primebloquelibre);

                        //actualizamos el inodo
                        //el primer apuntador -1 que encuentra
                        int var1 =0;
                        for (var1 = 0; var1 < 12; ++var1) {
                            if(Inodo.i_block[var1] == -1){
                                break;
                            }
                        }
                        Inodo.i_block[var1] = primebloquelibre;
                        InsertarInodo(UsuarioLog.Path,sb.s_inode_start + (primerInodoLibre * sizeof(TABLAINODOS)),Inodo);

                        TABLAINODOS inuser = getInodo(UsuarioLog.Path,sb.s_inode_start + sizeof(TABLAINODOS));
                        //RECUPERACION DE ARCHIVO USER aun n o esta
                        //char *archivo = (char*)malloc(sizeof(archivo));
                        //archivo = getArchivo(UsuarioLog.Path,sb.s_block_start + (primebloquelibre*sizeof(BLOQUEAPUNTADORES)));
                        // printf("Archivo:\n%s\n",archivo);
                    }
                }

                /*BLOQUEARCHIVO bloqueasociado;
                memset(&bloqueasociado,'\0',sizeof(bloqueasociado));
                InsertarBloque_Archivo(UsuarioLog.Path,sb.s_block_start + (primeBlloqueLibre * sizeof(BLOQUECARPETA)),bloqueasociado);

                ActualizarBINODO(UsuarioLog.Path,sb.s_bm_inode_start,primerInodoLibre);
                ActualizarBBLOQUE(UsuarioLog.Path,sb.s_bm_block_start,primeBlloqueLibre);*/

                printf("Se ha creado el archivo %s\n",NombreArchivo);
                break;
            }
        }else{      //significa que el apuntador es -1

            //insertamos el bloque nuevo con su primer apuntador a el inodo nuevo que crearemos
            // los otros 3 estaran vacios
            BLOQUECARPETA nuevo;

            CONTENTCARPETA nuevacarp;
            nuevacarp.b_inodo = primerInodoLibre;
            memset(&nuevacarp.b_name,'\0',sizeof(nuevacarp.b_name));
            strcpy(nuevacarp.b_name,NombreArchivo);
            nuevo.Content[0] = nuevacarp;

            CONTENTCARPETA con00;
            con00.b_inodo = -1;
            memset(&con00.b_name,'\0',sizeof(con00.b_name));
            nuevo.Content[1] = con00;

            CONTENTCARPETA con11;
            con11.b_inodo = -1;
            memset(&con11.b_name,'\0',sizeof(con11.b_name));
            nuevo.Content[2] = con11;

            CONTENTCARPETA con22;
            con22.b_inodo = -1;
            memset(&con22.b_name,'\0',sizeof(con22.b_name));
            nuevo.Content[3] = con22;

            InsertarBloque_Carpeta(UsuarioLog.Path,sb.s_block_start + (primeBlloqueLibre * sizeof(BLOQUECARPETA)),nuevo);

            ActualizarBBLOQUE(UsuarioLog.Path,sb.s_bm_block_start,primeBlloqueLibre);
            //actualizamos el inodo tambien
            inodoPadre.i_block[var] = primeBlloqueLibre;
            InsertarInodo(UsuarioLog.Path,sb.s_inode_start + (father.Content[0].b_inodo * sizeof(TABLAINODOS)),inodoPadre);

            inodoPadre = getInodo(UsuarioLog.Path,sb.s_inode_start + (father.Content[0].b_inodo * sizeof(TABLAINODOS)));
            //volvemos a capturar el nuevo blque libre
            primeBlloqueLibre = getPrimerBloqueLibre(UsuarioLog.Path,sb.s_bm_block_start);

            //insertamos el inodo de la nueva carpeta
            TABLAINODOS Inodo;
            Inodo.i_uid = UsuarioLog.IDUsuario;
            Inodo.i_gid = 1;
            Inodo.i_size = strlen(contenido);
            Inodo.i_atime = time(0);
            Inodo.i_ctime = time(0);
            Inodo.i_mtime = time(0);
            memset(&Inodo.i_block,-1,sizeof(Inodo.i_block));          //inicializamos todos sus punteros a -1
            Inodo.i_type = '1';      //tipo 0 signifca que es carpeta.
            Inodo.i_perm = 664;      //permisos de el archivo.
            //actualizacion de primer puntero para el bloque.
            Inodo.i_block[0] = primeBlloqueLibre;  // el primer auntador del primer inodo apuntara al bloque 0.
            InsertarInodo(UsuarioLog.Path,sb.s_inode_start + (primerInodoLibre * sizeof(TABLAINODOS)),Inodo);

            /*peddddodoo*/
            if(strlen(contenido) <= 64){
                //si la cantidad nueva sigue siendo menor a 64 bits se escribe en el bloque que leimos
                BLOQUEARCHIVO bloqueasociado;
                memset(&bloqueasociado.b_content,'\0',sizeof(bloqueasociado.b_content));
                strcpy(bloqueasociado.b_content,contenido);
                InsertarBloque_Archivo(UsuarioLog.Path,sb.s_block_start + (primeBlloqueLibre * sizeof(BLOQUECARPETA)),bloqueasociado);

                ActualizarBINODO(UsuarioLog.Path,sb.s_bm_inode_start,primerInodoLibre);
                ActualizarBBLOQUE(UsuarioLog.Path,sb.s_bm_block_start,primeBlloqueLibre);

                printf("Se ha creado el archivo %s\n",NombreArchivo);

                //RECUPERACION DE ARCHIVO USER
                // char *archivo = (char*)malloc(sizeof(archivo));
                //archivo = getArchivo(UsuarioLog.Path,sb.s_block_start + (bloqueenquemequede*sizeof(BLOQUEAPUNTADORES)));
                //printf("Archivo:\n%s\n",archivo);

                //int primerblo1 = getPrimerBloqueLibre(UsuarioLog.Path,sb.s_bm_block_start);
                //int primerino1 = getPrimerInodoLibre(UsuarioLog.Path,sb.s_bm_inode_start);
                //printf("Primer Bloque: %d\nPrimer Inodo: %d\n",primerblo1,primerino1);
                return;
            }else{

                //como el archivo excede los 64 bits haremos uso de mas bloques de contenido
                char PrimerArchivo[64];
                memset(&PrimerArchivo,'\0',sizeof(PrimerArchivo));

                char *pauxiliar = (char*)malloc(sizeof(pauxiliar));
                pauxiliar = contenido;


                //completamos los 64 bits del bloque actual
                int indice = 0;
                while (indice < 64) {
                    char letra = *pauxiliar;
                    PrimerArchivo[indice] = letra;
                    pauxiliar++;
                    indice++;
                }

                BLOQUEARCHIVO bloqueasociado;
                memset(&bloqueasociado.b_content,'\0',sizeof(bloqueasociado.b_content));
                strcpy(bloqueasociado.b_content,PrimerArchivo);
                InsertarBloque_Archivo(UsuarioLog.Path,sb.s_block_start + (primeBlloqueLibre * sizeof(BLOQUECARPETA)),bloqueasociado);

                ActualizarBINODO(UsuarioLog.Path,sb.s_bm_inode_start,primerInodoLibre);
                ActualizarBBLOQUE(UsuarioLog.Path,sb.s_bm_block_start,primeBlloqueLibre);

                //bvolvemos a capturar el primer bloque libre y lo llenamos de nuevo
                while (*pauxiliar != NULL) {
                    indice = 0;
                    char Arch[64];
                    memset(&Arch,'\0',sizeof(Arch));

                    while (*pauxiliar != NULL) {
                        if(indice < 64){
                            char letra = *pauxiliar;
                            Arch[indice] = letra;
                            pauxiliar++;
                            indice++;

                        }else{
                            break;
                        }
                    }

                    int primebloquelibre = getPrimerBloqueLibre(UsuarioLog.Path,sb.s_bm_block_start);

                    //lo insertamos y actualizamos el apuntador del inodo
                    BLOQUEARCHIVO masnuevo;
                    memset(&masnuevo.b_content,'\0',sizeof(masnuevo.b_content));
                    strcpy(masnuevo.b_content,Arch);
                    InsertarBloque_Archivo(UsuarioLog.Path,sb.s_block_start + (primebloquelibre * sizeof(BLOQUEARCHIVO)),masnuevo);

                    ActualizarBBLOQUE(UsuarioLog.Path,sb.s_bm_block_start,primebloquelibre);

                    //actualizamos el inodo
                    //el primer apuntador -1 que encuentra
                    int var1 =0;
                    for (var1 = 0; var1 < 12; ++var1) {
                        if(Inodo.i_block[var1] == -1){
                            break;
                        }
                    }
                    Inodo.i_block[var1] = primebloquelibre;
                    InsertarInodo(UsuarioLog.Path,sb.s_inode_start + sizeof(TABLAINODOS),Inodo);

                    TABLAINODOS inuser = getInodo(UsuarioLog.Path,sb.s_inode_start + sizeof(TABLAINODOS));
                    //RECUPERACION DE ARCHIVO USER aun n o esta
                    //char *archivo = (char*)malloc(sizeof(archivo));
                    //archivo = getArchivo(UsuarioLog.Path,sb.s_block_start + (primebloquelibre*sizeof(BLOQUEAPUNTADORES)));
                    // printf("Archivo:\n%s\n",archivo);
                }
            }

            /*pedisiimooomo*/


            /*BLOQUEARCHIVO bloqueasociado;
            memset(&bloqueasociado,'\0',sizeof(bloqueasociado));
            InsertarBloque_Archivo(UsuarioLog.Path,sb.s_block_start + (primeBlloqueLibre * sizeof(BLOQUECARPETA)),bloqueasociado);

            ActualizarBINODO(UsuarioLog.Path,sb.s_bm_inode_start,primerInodoLibre);
            ActualizarBBLOQUE(UsuarioLog.Path,sb.s_bm_block_start,primeBlloqueLibre);*/

            printf("Se ha creado el archivo %s\n",NombreArchivo);
            break;
        }
    }

    primerInodoLibre = getPrimerInodoLibre(UsuarioLog.Path,sb.s_bm_inode_start);
    primeBlloqueLibre = getPrimerBloqueLibre(UsuarioLog.Path,sb.s_bm_block_start);

    sb.s_first_ino = primerInodoLibre;
    sb.s_first_blo = primeBlloqueLibre;

    setSB(sb,UsuarioLog.Path,UsuarioLog.StartParticion);        //actualizamos el sb
    return inodoPadre;
}

TABLAINODOS CrearCarpeta(TABLAINODOS inodoPadre, char NombreCarpeta[25]){

    SUPERBLOQUE sb = getSB(UsuarioLog.Path,UsuarioLog.StartParticion);

    int primerInodoLibre = getPrimerInodoLibre(UsuarioLog.Path,sb.s_bm_inode_start);
    int primeBlloqueLibre = getPrimerBloqueLibre(UsuarioLog.Path,sb.s_bm_block_start);
    BLOQUECARPETA father = getBloqueCarpeta(sb.s_block_start + (inodoPadre.i_block[0]*sizeof(BLOQUECARPETA)));


    for (int var = 0; var < 15; ++var) {

        if(inodoPadre.i_block[var] != -1){
            BLOQUECARPETA carpeta = getBloqueCarpeta(sb.s_block_start + (inodoPadre.i_block[var]*sizeof(BLOQUECARPETA)));

            if(carpeta.Content[0].b_inodo == -1){

                carpeta.Content[0].b_inodo = primerInodoLibre;
                memset(&carpeta.Content[0].b_name,'\0',sizeof(carpeta.Content[0].b_name));
                strcpy(carpeta.Content[0].b_name,NombreCarpeta);
                InsertarBloque_Carpeta(UsuarioLog.Path,sb.s_block_start + (inodoPadre.i_block[var]*sizeof(BLOQUECARPETA)),carpeta);

                //insertamos el inodo de la nueva carpeta
                TABLAINODOS Inodo;
                Inodo.i_uid = UsuarioLog.IDUsuario;
                Inodo.i_gid = 1;
                Inodo.i_size = 0;
                Inodo.i_atime = time(0);
                Inodo.i_ctime = time(0);
                Inodo.i_mtime = time(0);
                memset(&Inodo.i_block,-1,sizeof(Inodo.i_block));          //inicializamos todos sus punteros a -1
                Inodo.i_type = '0';      //tipo 0 signifca que es carpeta.
                Inodo.i_perm = 664;      //permisos de el archivo.
                //actualizacion de primer puntero para el bloque.
                Inodo.i_block[0] = primeBlloqueLibre;  // el primer auntador del primer inodo apuntara al bloque 0.

                InsertarInodo(UsuarioLog.Path,sb.s_inode_start + (primerInodoLibre * sizeof(TABLAINODOS)),Inodo);

                BLOQUECARPETA bloqueasociado;
                CONTENTCARPETA actual;
                actual.b_inodo = primerInodoLibre;
                memset(&actual.b_name,'\0',sizeof(actual.b_name));
                strcpy(actual.b_name,NombreCarpeta);
                bloqueasociado.Content[0] = actual;
                CONTENTCARPETA padre;
                padre.b_inodo = father.Content[0].b_inodo;
                memset(&padre.b_name,'\0',sizeof(padre.b_name));
                strcpy(padre.b_name,father.Content[0].b_name);
                bloqueasociado.Content[1] = padre;
                CONTENTCARPETA con1;
                con1.b_inodo = -1;
                memset(&con1.b_name,'\0',sizeof(con1.b_name));
                bloqueasociado.Content[2] = con1;
                CONTENTCARPETA con2;
                con2.b_inodo = -1;
                memset(&con2.b_name,'\0',sizeof(con2.b_name));
                bloqueasociado.Content[3] = con2;

                InsertarBloque_Carpeta(UsuarioLog.Path,sb.s_block_start + (primeBlloqueLibre * sizeof(BLOQUECARPETA)),bloqueasociado);

                ActualizarBINODO(UsuarioLog.Path,sb.s_bm_inode_start,primerInodoLibre);
                ActualizarBBLOQUE(UsuarioLog.Path,sb.s_bm_block_start,primeBlloqueLibre);

                printf("Se ha creado la carpeta %s.\n",NombreCarpeta);
                break;
            }
            else if(carpeta.Content[1].b_inodo == -1){
                carpeta.Content[1].b_inodo = primerInodoLibre;
                memset(&carpeta.Content[1].b_name,'\0',sizeof(carpeta.Content[1].b_name));
                strcpy(carpeta.Content[1].b_name,NombreCarpeta);
                InsertarBloque_Carpeta(UsuarioLog.Path,sb.s_block_start + (inodoPadre.i_block[var]*sizeof(BLOQUECARPETA)),carpeta);

                //insertamos el inodo de la nueva carpeta
                TABLAINODOS Inodo;
                Inodo.i_uid = UsuarioLog.IDUsuario;
                Inodo.i_gid = 1;
                Inodo.i_size = 0;
                Inodo.i_atime = time(0);
                Inodo.i_ctime = time(0);
                Inodo.i_mtime = time(0);
                memset(&Inodo.i_block,-1,sizeof(Inodo.i_block));          //inicializamos todos sus punteros a -1
                Inodo.i_type = '0';      //tipo 0 signifca que es carpeta.
                Inodo.i_perm = 664;      //permisos de el archivo.
                //actualizacion de primer puntero para el bloque.
                Inodo.i_block[0] = primeBlloqueLibre;  // el primer auntador del primer inodo apuntara al bloque 0.

                InsertarInodo(UsuarioLog.Path,sb.s_inode_start + (primerInodoLibre * sizeof(TABLAINODOS)),Inodo);

                BLOQUECARPETA bloqueasociado;
                CONTENTCARPETA actual;
                actual.b_inodo = primerInodoLibre;
                memset(&actual.b_name,'\0',sizeof(actual.b_name));
                strcpy(actual.b_name,NombreCarpeta);
                bloqueasociado.Content[0] = actual;
                CONTENTCARPETA padre;
                padre.b_inodo = father.Content[0].b_inodo;
                memset(&padre.b_name,'\0',sizeof(padre.b_name));
                strcpy(padre.b_name,father.Content[0].b_name);
                bloqueasociado.Content[1] = padre;
                CONTENTCARPETA con1;
                con1.b_inodo = -1;
                memset(&con1.b_name,'\0',sizeof(con1.b_name));
                bloqueasociado.Content[2] = con1;
                CONTENTCARPETA con2;
                con2.b_inodo = -1;
                memset(&con2.b_name,'\0',sizeof(con2.b_name));
                bloqueasociado.Content[3] = con2;

                InsertarBloque_Carpeta(UsuarioLog.Path,sb.s_block_start + (primeBlloqueLibre * sizeof(BLOQUECARPETA)),bloqueasociado);

                ActualizarBINODO(UsuarioLog.Path,sb.s_bm_inode_start,primerInodoLibre);
                ActualizarBBLOQUE(UsuarioLog.Path,sb.s_bm_block_start,primeBlloqueLibre);

                printf("Se ha creado la carpeta %s.\n",NombreCarpeta);
                break;
            }
            else if(carpeta.Content[2].b_inodo == -1){
                carpeta.Content[2].b_inodo = primerInodoLibre;
                memset(&carpeta.Content[2].b_name,'\0',sizeof(carpeta.Content[2].b_name));
                strcpy(carpeta.Content[2].b_name,NombreCarpeta);
                InsertarBloque_Carpeta(UsuarioLog.Path,sb.s_block_start + (inodoPadre.i_block[var]*sizeof(BLOQUECARPETA)),carpeta);

                //insertamos el inodo de la nueva carpeta
                TABLAINODOS Inodo;
                Inodo.i_uid = UsuarioLog.IDUsuario;
                Inodo.i_gid = 1;
                Inodo.i_size = 0;
                Inodo.i_atime = time(0);
                Inodo.i_ctime = time(0);
                Inodo.i_mtime = time(0);
                memset(&Inodo.i_block,-1,sizeof(Inodo.i_block));          //inicializamos todos sus punteros a -1
                Inodo.i_type = '0';      //tipo 0 signifca que es carpeta.
                Inodo.i_perm = 664;      //permisos de el archivo.
                //actualizacion de primer puntero para el bloque.
                Inodo.i_block[0] = primeBlloqueLibre;  // el primer auntador del primer inodo apuntara al bloque 0.

                InsertarInodo(UsuarioLog.Path,sb.s_inode_start + (primerInodoLibre * sizeof(TABLAINODOS)),Inodo);

                BLOQUECARPETA bloqueasociado;
                CONTENTCARPETA actual;
                actual.b_inodo = primerInodoLibre;
                memset(&actual.b_name,'\0',sizeof(actual.b_name));
                strcpy(actual.b_name,NombreCarpeta);
                bloqueasociado.Content[0] = actual;
                CONTENTCARPETA padre;
                padre.b_inodo = father.Content[0].b_inodo;
                memset(&padre.b_name,'\0',sizeof(padre.b_name));
                strcpy(padre.b_name,father.Content[0].b_name);
                bloqueasociado.Content[1] = padre;
                CONTENTCARPETA con1;
                con1.b_inodo = -1;
                memset(&con1.b_name,'\0',sizeof(con1.b_name));
                bloqueasociado.Content[2] = con1;
                CONTENTCARPETA con2;
                con2.b_inodo = -1;
                memset(&con2.b_name,'\0',sizeof(con2.b_name));
                bloqueasociado.Content[3] = con2;

                InsertarBloque_Carpeta(UsuarioLog.Path,sb.s_block_start + (primeBlloqueLibre * sizeof(BLOQUECARPETA)),bloqueasociado);

                ActualizarBINODO(UsuarioLog.Path,sb.s_bm_inode_start,primerInodoLibre);
                ActualizarBBLOQUE(UsuarioLog.Path,sb.s_bm_block_start,primeBlloqueLibre);

                printf("Se ha creado la carpeta %s.\n",NombreCarpeta);
                break;
            }
            else if(carpeta.Content[3].b_inodo == -1){

                carpeta.Content[3].b_inodo = primerInodoLibre;
                memset(&carpeta.Content[3].b_name,'\0',sizeof(carpeta.Content[3].b_name));
                strcpy(carpeta.Content[3].b_name,NombreCarpeta);
                //actualizamos
                InsertarBloque_Carpeta(UsuarioLog.Path,sb.s_block_start + (inodoPadre.i_block[var]*sizeof(BLOQUECARPETA)),carpeta);

                //insertamos el inodo de la nueva carpeta
                TABLAINODOS Inodo;
                Inodo.i_uid = UsuarioLog.IDUsuario;
                Inodo.i_gid = 1;
                Inodo.i_size = 0;
                Inodo.i_atime = time(0);
                Inodo.i_ctime = time(0);
                Inodo.i_mtime = time(0);
                memset(&Inodo.i_block,-1,sizeof(Inodo.i_block));          //inicializamos todos sus punteros a -1
                Inodo.i_type = '0';      //tipo 0 signifca que es carpeta.
                Inodo.i_perm = 664;      //permisos de el archivo.
                //actualizacion de primer puntero para el bloque.
                Inodo.i_block[0] = primeBlloqueLibre;  // el primer auntador del primer inodo apuntara al bloque 0.

                InsertarInodo(UsuarioLog.Path,sb.s_inode_start + (primerInodoLibre * sizeof(TABLAINODOS)),Inodo);

                BLOQUECARPETA bloqueasociado;
                CONTENTCARPETA actual;
                actual.b_inodo = primerInodoLibre;
                memset(&actual.b_name,'\0',sizeof(actual.b_name));
                strcpy(actual.b_name,NombreCarpeta);
                bloqueasociado.Content[0] = actual;
                CONTENTCARPETA padre;
                padre.b_inodo = father.Content[0].b_inodo;
                memset(&padre.b_name,'\0',sizeof(padre.b_name));
                strcpy(padre.b_name,father.Content[0].b_name);
                bloqueasociado.Content[1] = padre;
                CONTENTCARPETA con1;
                con1.b_inodo = -1;
                memset(&con1.b_name,'\0',sizeof(con1.b_name));
                bloqueasociado.Content[2] = con1;
                CONTENTCARPETA con2;
                con2.b_inodo = -1;
                memset(&con2.b_name,'\0',sizeof(con2.b_name));
                bloqueasociado.Content[3] = con2;

                InsertarBloque_Carpeta(UsuarioLog.Path,sb.s_block_start + (primeBlloqueLibre * sizeof(BLOQUECARPETA)),bloqueasociado);

                ActualizarBINODO(UsuarioLog.Path,sb.s_bm_inode_start,primerInodoLibre);
                ActualizarBBLOQUE(UsuarioLog.Path,sb.s_bm_block_start,primeBlloqueLibre);

                printf("Se ha creado la carpeta %s.\n",NombreCarpeta);
                break;
            }
        }else{      //significa que el apuntador es -1
            //insertamos el bloque del apuntador del inodo
            BLOQUECARPETA nuevo;

            CONTENTCARPETA nuevacarp;
            nuevacarp.b_inodo = primerInodoLibre;
            memset(&nuevacarp.b_name,'\0',sizeof(nuevacarp.b_name));
            strcpy(nuevacarp.b_name,NombreCarpeta);
            nuevo.Content[0] = nuevacarp;

            CONTENTCARPETA con00;
            con00.b_inodo = -1;
            memset(&con00.b_name,'\0',sizeof(con00.b_name));
            nuevo.Content[1] = con00;

            CONTENTCARPETA con11;
            con11.b_inodo = -1;
            memset(&con11.b_name,'\0',sizeof(con11.b_name));
            nuevo.Content[2] = con11;

            CONTENTCARPETA con22;
            con22.b_inodo = -1;
            memset(&con22.b_name,'\0',sizeof(con22.b_name));
            nuevo.Content[3] = con22;

            InsertarBloque_Carpeta(UsuarioLog.Path,sb.s_block_start + (primeBlloqueLibre * sizeof(BLOQUECARPETA)),nuevo);

            ActualizarBBLOQUE(UsuarioLog.Path,sb.s_bm_block_start,primeBlloqueLibre);
            //actualizamos el inodo tambien
            inodoPadre.i_block[var] = primeBlloqueLibre;
            InsertarInodo(UsuarioLog.Path,sb.s_inode_start + (father.Content[0].b_inodo * sizeof(TABLAINODOS)),inodoPadre);

            inodoPadre = getInodo(UsuarioLog.Path,sb.s_inode_start + (father.Content[0].b_inodo * sizeof(TABLAINODOS)));
            //volvemos a capturar el nuevo blque libre
            primeBlloqueLibre = getPrimerBloqueLibre(UsuarioLog.Path,sb.s_bm_block_start);

            //insertamos el inodo de la nueva carpeta
            TABLAINODOS Inodo;
            Inodo.i_uid = UsuarioLog.IDUsuario;
            Inodo.i_gid = 1;
            Inodo.i_size = 0;
            Inodo.i_atime = time(0);
            Inodo.i_ctime = time(0);
            Inodo.i_mtime = time(0);
            memset(&Inodo.i_block,-1,sizeof(Inodo.i_block));          //inicializamos todos sus punteros a -1
            Inodo.i_type = '0';      //tipo 0 signifca que es carpeta.
            Inodo.i_perm = 664;      //permisos de el archivo.
            //actualizacion de primer puntero para el bloque.
            Inodo.i_block[0] = primeBlloqueLibre;  // el primer auntador del primer inodo apuntara al bloque 0.

            InsertarInodo(UsuarioLog.Path,sb.s_inode_start + (primerInodoLibre * sizeof(TABLAINODOS)),Inodo);

            BLOQUECARPETA bloqueasociado;

            CONTENTCARPETA actual;
            actual.b_inodo = primerInodoLibre;
            memset(&actual.b_name,'\0',sizeof(actual.b_name));
            strcpy(actual.b_name,NombreCarpeta);
            bloqueasociado.Content[0] = actual;

            CONTENTCARPETA padre;
            padre.b_inodo = father.Content[0].b_inodo;
            memset(&padre.b_name,'\0',sizeof(padre.b_name));
            strcpy(padre.b_name,father.Content[0].b_name);
            bloqueasociado.Content[1] = padre;

            CONTENTCARPETA con1;
            con1.b_inodo = -1;
            memset(&con1.b_name,'\0',sizeof(con1.b_name));
            bloqueasociado.Content[2] = con1;

            CONTENTCARPETA con2;
            con2.b_inodo = -1;
            memset(&con2.b_name,'\0',sizeof(con2.b_name));
            bloqueasociado.Content[3] = con2;

            InsertarBloque_Carpeta(UsuarioLog.Path,sb.s_block_start + (primeBlloqueLibre * sizeof(BLOQUECARPETA)),bloqueasociado);

            ActualizarBINODO(UsuarioLog.Path,sb.s_bm_inode_start,primerInodoLibre);
            ActualizarBBLOQUE(UsuarioLog.Path,sb.s_bm_block_start,primeBlloqueLibre);

            printf("Se ha creado la carpeta %s.\n",NombreCarpeta);
            break;
        }
    }

    primerInodoLibre = getPrimerInodoLibre(UsuarioLog.Path,sb.s_bm_inode_start);
    primeBlloqueLibre = getPrimerBloqueLibre(UsuarioLog.Path,sb.s_bm_block_start);

    sb.s_first_ino = primerInodoLibre;
    sb.s_first_blo = primeBlloqueLibre;

    setSB(sb,UsuarioLog.Path,UsuarioLog.StartParticion);        //actualizamos el sb
    return inodoPadre;
}

//recupera bloque carpeta recibiendo posicion exacta dentro del disco
BLOQUECARPETA getBloqueCarpeta(int Posicion){
    BLOQUECARPETA carpeta;
    FILE *ar;
    ar = fopen(UsuarioLog.Path,"rb");
    if (ar != NULL){
        fseek(ar,Posicion,SEEK_SET);
        fread(&carpeta,sizeof(BLOQUECARPETA),1,ar);
        fclose(ar);
        fflush(ar);

    }else{
        printf("No sirvio chavo.(getbloque)\n");
    }

    return carpeta;
}

//get contenido de algun bloque de archivo recibiendo su posicion exacta en el disco
char *getArchivo(char PathDisco[250],int Posicion){

    char *archivo = (char*)malloc(sizeof(archivo));
    memset(&archivo,'\0',sizeof(archivo));

    FILE *ar;
    char cont[68];
    memset(&cont,'\0',sizeof(cont));
    ar = fopen(PathDisco,"rb");

    if (ar != NULL){
        fseek(ar,Posicion,SEEK_SET);
        fread(&cont,sizeof(BLOQUEARCHIVO),1,ar);

        fclose(ar);
        fflush(ar);

    }else{
        printf("No sirvio chavo.(getarchivo)\n");
    }

    archivo = cont;
    return archivo;
}

char *BuscarArchivo(char RUTA[250]){

    SUPERBLOQUE sb = getSB(UsuarioLog.Path,UsuarioLog.StartParticion);

    bool para = false;
    TABLAINODOS padre;
    padre = getInodo(UsuarioLog.Path,sb.s_inode_start);

    char Pataux[250];
    memset(&Pataux,'\0', sizeof(Pataux));

    int indice = 1;
    for (int i = 0; i < strlen(RUTA); i++) {

        if(RUTA[0] == '/' && i == 0){
            Pataux[0]= '/';
            padre = VerificacionExisteCarpeta(padre, Pataux);
            memset(&Pataux,'\0',sizeof(Pataux));
            i++;
            indice = 0;
        }else{
            if(RUTA[0] != '/' && i == 0){
                Pataux[0] = '/';
            }
        }
        while ((RUTA[i] != '/' || i == 0) &&  i < 250){
            Pataux[indice] = RUTA[i];
            i++;
            indice++;
            if(RUTA[i] == '.'){
                Pataux[indice] = RUTA[i];
                i++;
                indice++;
                para = true;
                //break;
            }
        }
        if(!para){
            para = false;
        }else{
            //capturar contenido.

            char *contenido = (char*)malloc(sizeof(contenido));
            memset(&contenido,'\0',sizeof(contenido));

            for (int apuntador = 0; apuntador < 15; apuntador++){
                if(padre.i_block[apuntador] != -1){
                    SUPERBLOQUE sb = getSB(UsuarioLog.Path,UsuarioLog.StartParticion);
                    BLOQUECARPETA father = getBloqueCarpeta(sb.s_block_start + (padre.i_block[0]*sizeof(BLOQUECARPETA)));

                    for (int bloque = 0; bloque < 4; bloque++) {

                        //printf("que pedo\n");
                        if(strcmp(father.Content[bloque].b_name,Pataux) ==0){
                            //significa que encontro el archivo en alguno de sus bloques.
                            //mandamoa a capturar el contenido del archivo.]

                            int posicionInodoArchivo = sb.s_inode_start + (father.Content[bloque].b_inodo * sizeof(TABLAINODOS));
                            TABLAINODOS InodoArchivo = getInodo(UsuarioLog.Path,posicionInodoArchivo);

                            char ContenidoCompletoArchivo[1000];
                            memset(&ContenidoCompletoArchivo,'\0',sizeof(ContenidoCompletoArchivo));
                            int indice = 0;

                            for (int bloqueinodo = 0; bloqueinodo < 15; bloqueinodo++) {

                                if(InodoArchivo.i_block[bloqueinodo] != -1){
                                    //existe es apuntador por ende recuperamos el contenido.

                                    int posicionbloque = sb.s_block_start + (InodoArchivo.i_block[bloqueinodo] * sizeof(BLOQUEARCHIVO));
                                    char *contenidobloque = (char*)malloc(sizeof(contenidobloque));
                                    contenidobloque = getArchivo(UsuarioLog.Path,posicionbloque);

                                    while (*contenidobloque != NULL) {
                                        char letra = *contenidobloque;
                                        ContenidoCompletoArchivo[indice] = letra;
                                        indice++;
                                        contenidobloque++;
                                    }
                                }
                            }
                            //printf("contenido del archivo es:\n%s\n",ContenidoCompletoArchivo);
                            contenido = ContenidoCompletoArchivo;
                            return contenido;
                            break;
                        }

                    }
                }
            }
            return contenido;
        }

        TABLAINODOS aux = padre;
        padre = VerificacionExisteCarpeta(padre,Pataux);

        //verificamos que existan las carpetas de no existi se reportara error
        if(padre.i_type == '\0'){
            printf("Carpeta %s no existe.\n",Pataux);
            return;
        }
        indice = 0;
        memset(&Pataux,'\0',sizeof(Pataux));

    }
}

void Mkusr(char comando[500]){
    int i = 0;
    bool Error = false;
    while (comando[i] == ' ') {
        i++;
    }
    i = i+5;
    while(comando[i] == ' '){
        i++;
    }

    char USR[15];
    memset(&USR,'\0',sizeof(USR));

    char PWD[15];
    memset(&PWD,'\0',sizeof(PWD));

    char GRP[15];
    memset(&GRP,'\0',sizeof(GRP));

    for (i = i; i < 500; i++) {
        if((comando[i] == 'u' || comando[i] == 'U')
                &&(comando[i+1] == 's' || comando[i+1] == 'S')
                &&(comando[i+2] == 'r' || comando[i+2] == 'R')){

            char *usr = (char*)malloc(sizeof(usr));
            usr = getUSR(comando,i);
            int indice = 0;
            while(*usr != '\0'){
                char letra = *usr;
                USR[indice] = letra;
                usr++;
                indice++;
            }
            i = indc;
            printf("Id_Usuario: %s\n",USR);
        }else if((comando[i] == 'p' || comando[i] == 'P')
                 &&(comando[i+1] == 'w' || comando[i+1] == 'W')
                 &&(comando[i+2] == 'd' || comando[i+2] == 'D')){

            char *pwd = (char*)malloc(sizeof(pwd));
            pwd = getUSR(comando,i);
            int indice = 0;
            while(*pwd != '\0'){
                char letra = *pwd;
                PWD[indice] = letra;
                pwd++;
                indice++;
            }
            i = indc;
            printf("Password: %s\n",PWD);
        }else if((comando[i] == 'g' || comando[i] == 'G')
                 && (comando[i+1] == 'r' || comando[i+1] == 'R')
                 && (comando[i+2] == 'p' || comando[i+2] == 'P')){

            char *grp = (char*)malloc(sizeof(grp));
            grp = getUSR(comando,i);
            int indice = 0;
            while(*grp != '\0'){
                char letra = *grp;
                GRP[indice] = letra;
                grp++;
                indice++;
            }
            i = indc;
            printf("grp: %s\n",GRP);
        } else if(comando[i] == '#'){
            break;
        }else{
            if(comando[i-1] == '\n'){
                break;
            }
            else if(comando[i] != '-' && comando[i] != ' ' && comando[i] != '\n' && comando[i] != '\r'){
                printf("Error en los parametros del comando mkusr.\n");
                return;
            }
        }
    }

    /*creacion de grupo*/
    SUPERBLOQUE sb;
    sb = getSB(UsuarioLog.Path,UsuarioLog.StartParticion);

    if(sb.s_magic != 0)
    {
        //strcat(Archivo,grupo);
        /*  Agregamos el archivo de user a una cadena a auxiliar y concatenamos el nuevoo grupo
            y se verificara que el contenido no super los 64 bits de caso contrario debera crearse
            otro bloque de contenido.*/

        char Auxiliar[12*64]; //cpaacidad para 12 bloques de contenido de 64 bits c/u /* solo apuntadores directo */
        memset(&Auxiliar,'\0',sizeof(Auxiliar));

        char Auxiliar1[12*64]; //cpaacidad para 12 bloques de contenido de 64 bits c/u /* solo apuntadores directo */
        memset(&Auxiliar1,'\0',sizeof(Auxiliar1));

        TABLAINODOS inodouser = getInodo(UsuarioLog.Path,(sb.s_inode_start + sizeof(TABLAINODOS)));
        //ahora concatenamos todo el contenido.
        int bloqueenquemequede = 0;   //NUMERO ACTUAL QUE LEI...
        int indice1 = 0;

        for (int var = 0; var < 15; ++var) {

            if(inodouser.i_block[var] != -1){

                int x = (int)inodouser.i_block[var];
                char *arch = (char*)malloc(sizeof(arch));

                arch = getArchivo(UsuarioLog.Path,(sb.s_block_start + (x*sizeof(BLOQUEAPUNTADORES))));        //aumentamos 1 porque el bloque que es de la raiz
                //cantidad de caracteres que tiene ese bloque.
                int c =0 ;
                while (*arch != NULL) {
                    char letra = *arch;
                    Auxiliar1[indice1] = letra;
                    arch++;
                    c++;
                    indice1++;
                }

                if(c < 64){     //si el bloque aun tiene capacidad para almacenar mas caracteres.
                    bloqueenquemequede = x;
                    arch = getArchivo(UsuarioLog.Path,(sb.s_block_start + (x*sizeof(BLOQUEAPUNTADORES))));        //aumentamos 1 porque el bloque que es de la raiz

                    char Archivo[64]; //aca almacenaremos el archivo que recuperemos
                    memset(&Archivo,'\0',sizeof(Archivo));

                    int indice = 0;
                    while (*arch != NULL) {
                        char letra = *arch;
                        Archivo[indice] = letra;
                        arch++;
                        indice++;
                    }
                    strcat(Auxiliar,Archivo);           //concatenamos todos losbloques de tipo archivo que sean diferente de -1

                    /*--analisis para saber el numero de grupo que toca*/
                    int numero = 0;
                    for (int var = 0; var < 12*64; ++var) {
                        if(Auxiliar1[var] == 'U' && Auxiliar1[var-1] == ',' && Auxiliar1[var+1] == ','){
                            numero++;
                        }
                    }
                    // printf("Hay %d grupos.\n",numero);
                    numero++;

                    char num[6];
                    memset(&num,'\0',sizeof(num));
                    sprintf(num,"%d",numero);

                    //nuevo grupo
                    char grupo[50];
                    memset(&grupo,'\0',sizeof(grupo));
                    strcat(grupo,num);
                    strcat(grupo,",U,");
                    strcat(grupo,GRP);
                    strcat(grupo,",");
                    strcat(grupo,USR);
                    strcat(grupo,",");
                    strcat(grupo,PWD);
                    strcat(grupo,"\n");

                    //concatenamos el nuevo grupo a todo el texto
                    strcat(Auxiliar,grupo);

                    //ahora verificamos cuantos caracteres tiene tiene todo el texto para saber cuantos bloques usaremos
                    int cantidad =0;
                    for (int var = 0; var < strlen(Auxiliar); ++var) {
                        if(Auxiliar[var] != '\0'){
                            cantidad++;
                        }
                    }
                    //printf("EL archivo tiene %d caractees.\n",cantidad);

                    if(cantidad <= 64){
                        //si la cantidad nueva sigue siendo menor a 64 bits se escribe en el bloque que leimos
                        BLOQUEARCHIVO nuevo;
                        memset(&nuevo.b_content,'\0',sizeof(nuevo.b_content));
                        strcpy(nuevo.b_content,Auxiliar);
                        InsertarBloque_Archivo(UsuarioLog.Path,sb.s_block_start + (bloqueenquemequede*sizeof(BLOQUEARCHIVO)),nuevo);

                        //RECUPERACION DE ARCHIVO USER
                        // char *archivo = (char*)malloc(sizeof(archivo));
                        //archivo = getArchivo(UsuarioLog.Path,sb.s_block_start + (bloqueenquemequede*sizeof(BLOQUEAPUNTADORES)));
                        //printf("Archivo:\n%s\n",archivo);

                        //int primerblo1 = getPrimerBloqueLibre(UsuarioLog.Path,sb.s_bm_block_start);
                        //int primerino1 = getPrimerInodoLibre(UsuarioLog.Path,sb.s_bm_inode_start);
                        //printf("Primer Bloque: %d\nPrimer Inodo: %d\n",primerblo1,primerino1);

                        if(UsuarioLog.Tipo == 3 && Bandera_Esto_En_Recovery == false){
                            JOURNALING nuevo;

                            nuevo.Journal_Tipo_Operacion = 3;
                            nuevo.Jorunal_Tipo = 0;

                            memset(&nuevo.Journal_Nombre,'\0',sizeof(nuevo.Journal_Nombre));
                            strcpy(nuevo.Journal_Nombre,"Mkusr");

                            memset(&nuevo.Journal_Contenido,'\0',sizeof(nuevo.Journal_Contenido));
                            strcpy(nuevo.Journal_Contenido,comando);

                            nuevo.Jorunal_Fecha = time(0);
                            nuevo.Journal_Propietario = 1;
                            nuevo.Journal_Permisos = 664;

                            SUPERBLOQUE sb;
                            sb = getSB(UsuarioLog.Path,UsuarioLog.StartParticion);
                            //seguido del super bloque empezaran los journaling
                            //siempre y cuando sea paraticion en ext3

                            setJOURNALING(nuevo,UsuarioLog.Path,UsuarioLog.StartParticion +sizeof(SUPERBLOQUE) + (PrimerJournalingLibre * sizeof(JOURNALING)));
                            PrimerJournalingLibre++;

                        }
                        return;
                    }else{

                        //como el archivo excede los 64 bits haremos uso de mas bloques de contenido
                        char PrimerArchivo[64];
                        memset(&PrimerArchivo,'\0',sizeof(PrimerArchivo));

                        char *pauxiliar = (char*)malloc(sizeof(pauxiliar));
                        pauxiliar = Auxiliar;


                        //completamos los 64 bits del bloque actual
                        int indice = 0;
                        while (indice < 64) {
                            char letra = *pauxiliar;
                            PrimerArchivo[indice] = letra;
                            pauxiliar++;
                            indice++;
                        }

                        BLOQUEARCHIVO nuevo;
                        memset(&nuevo.b_content,'\0',sizeof(nuevo.b_content));
                        strcpy(nuevo.b_content,PrimerArchivo);
                        InsertarBloque_Archivo(UsuarioLog.Path,sb.s_block_start + (bloqueenquemequede * sizeof(BLOQUEARCHIVO)),nuevo);

                        //bvolvemos a capturar el primer bloque libre y lo llenamos de nuevo
                        while (*pauxiliar != NULL) {
                            indice = 0;
                            char Arch[64];
                            memset(&Arch,'\0',sizeof(Arch));

                            while (*pauxiliar != NULL) {
                                if(indice < 64){
                                    char letra = *pauxiliar;
                                    Arch[indice] = letra;
                                    pauxiliar++;
                                    indice++;

                                }else{
                                    break;
                                }
                            }

                            int primebloquelibre = getPrimerBloqueLibre(UsuarioLog.Path,sb.s_bm_block_start);

                            //lo insertamos y actualizamos el apuntador del inodo
                            BLOQUEARCHIVO masnuevo;
                            memset(&masnuevo.b_content,'\0',sizeof(masnuevo.b_content));
                            strcpy(masnuevo.b_content,Arch);
                            InsertarBloque_Archivo(UsuarioLog.Path,sb.s_block_start + (primebloquelibre * sizeof(BLOQUEARCHIVO)),masnuevo);

                            ActualizarBBLOQUE(UsuarioLog.Path,sb.s_bm_block_start,primebloquelibre);

                            //actualizamos el inodo
                            //el primer apuntador -1 que encuentra
                            int var1 =0;
                            for (var1 = 0; var1 < 12; ++var1) {
                                if(inodouser.i_block[var1] == -1){
                                    break;
                                }
                            }
                            inodouser.i_block[var1] = primebloquelibre;
                            InsertarInodo(UsuarioLog.Path,sb.s_inode_start + sizeof(TABLAINODOS),inodouser);

                            TABLAINODOS inuser = getInodo(UsuarioLog.Path,sb.s_inode_start + sizeof(TABLAINODOS));
                            //RECUPERACION DE ARCHIVO USER aun n o esta
                            //char *archivo = (char*)malloc(sizeof(archivo));
                            //archivo = getArchivo(UsuarioLog.Path,sb.s_block_start + (primebloquelibre*sizeof(BLOQUEAPUNTADORES)));
                            // printf("Archivo:\n%s\n",archivo);

                            if(UsuarioLog.Tipo == 3 && Bandera_Esto_En_Recovery == false){
                                JOURNALING nuevo;

                                nuevo.Journal_Tipo_Operacion = 3;
                                nuevo.Jorunal_Tipo = 0;
                                memset(&nuevo.Journal_Nombre,'\0',sizeof(nuevo.Journal_Nombre));
                                strcpy(nuevo.Journal_Nombre,"Mkusr");
                                memset(&nuevo.Journal_Contenido,'\0',sizeof(nuevo.Journal_Contenido));
                                strcpy(nuevo.Journal_Contenido,comando);
                                nuevo.Jorunal_Fecha = time(0);
                                nuevo.Journal_Propietario = 1;
                                nuevo.Journal_Permisos = 664;

                                SUPERBLOQUE sb;
                                sb = getSB(UsuarioLog.Path,UsuarioLog.StartParticion);
                                //seguido del super bloque empezaran los journaling
                                //siempre y cuando sea paraticion en ext3

                                setJOURNALING(nuevo,UsuarioLog.Path,UsuarioLog.StartParticion +sizeof(SUPERBLOQUE) + (PrimerJournalingLibre * sizeof(JOURNALING)));
                                PrimerJournalingLibre++;

                            }
                        }
                    }
                    break;
                }else{
                    //SI YA NO TIENE CAPACIDAD PARA ALMACENAR MAS CARACTERES PARAMOS AL SIGUIENTE APUNTADOR
                }
            }else{
                //es -1 osea que esta libre

                //creamos el nuevo nodo
                /*--analisis para saber el numero de grupo que toca*/
                int numero = 0;
                for (int var = 0; var < 12*64; ++var) {
                    if(Auxiliar1[var] == 'G' && Auxiliar1[var-1] == ',' && Auxiliar1[var+1] == ','){
                        numero++;
                    }
                }
                //printf("Hay %d grupos.\n",numero);
                numero++;

                char num[6];
                memset(&num,'\0',sizeof(num));
                sprintf(num,"%d",numero);

                char grupo[50];
                memset(&grupo,'\0',sizeof(grupo));
                strcat(grupo,num);
                strcat(grupo,",U,");
                strcat(grupo,GRP);
                strcat(grupo,",");
                strcat(grupo,USR);
                strcat(grupo,",");
                strcat(grupo,PWD);
                strcat(grupo,"\n");

                //concatenamos el nuevo grupo a todo el texto
                strcat(Auxiliar,grupo);

                int primebloquelibre = getPrimerBloqueLibre(UsuarioLog.Path,sb.s_bm_block_start);
                BLOQUEARCHIVO masnuevo;
                memset(&masnuevo.b_content,'\0',sizeof(masnuevo.b_content));
                strcpy(masnuevo.b_content,Auxiliar);
                InsertarBloque_Archivo(UsuarioLog.Path,sb.s_block_start + (primebloquelibre * sizeof(BLOQUEARCHIVO)),masnuevo);

                ActualizarBBLOQUE(UsuarioLog.Path,sb.s_bm_block_start,primebloquelibre);

                //actualizamos el inodo
                inodouser.i_block[var] = primebloquelibre;
                InsertarInodo(UsuarioLog.Path,sb.s_inode_start + sizeof(TABLAINODOS),inodouser);


                TABLAINODOS inuser = getInodo(UsuarioLog.Path,sb.s_inode_start + sizeof(TABLAINODOS));
                //RECUPERACION DE ARCHIVO USER aun n o esta
                //char *archivo = (char*)malloc(sizeof(archivo));
                //archivo = getArchivo(UsuarioLog.Path,sb.s_block_start + (primebloquelibre*sizeof(BLOQUEAPUNTADORES)));
                //printf("Archivo:\n%s\n",archivo);

                //int primerblo1 = getPrimerBloqueLibre(UsuarioLog.Path,sb.s_bm_block_start);
                // int primerino1 = getPrimerInodoLibre(UsuarioLog.Path,sb.s_bm_inode_start);
                //printf("Primer Bloque: %d\nPrimer Inodo: %d\n",primerblo1,primerino1);

                if(UsuarioLog.Tipo == 3 && Bandera_Esto_En_Recovery == false){
                    JOURNALING nuevo;

                    nuevo.Journal_Tipo_Operacion = 3;
                    nuevo.Jorunal_Tipo = 0;

                    memset(&nuevo.Journal_Nombre,'\0',sizeof(nuevo.Journal_Nombre));
                    strcpy(nuevo.Journal_Nombre,"Mkusr");

                    memset(&nuevo.Journal_Contenido,'\0',sizeof(nuevo.Journal_Contenido));
                    strcpy(nuevo.Journal_Contenido,comando);

                    nuevo.Jorunal_Fecha = time(0);
                    nuevo.Journal_Propietario = 1;
                    nuevo.Journal_Permisos = 664;

                    SUPERBLOQUE sb;
                    sb = getSB(UsuarioLog.Path,UsuarioLog.StartParticion);
                    //seguido del super bloque empezaran los journaling
                    //siempre y cuando sea paraticion en ext3

                    setJOURNALING(nuevo,UsuarioLog.Path,UsuarioLog.StartParticion +sizeof(SUPERBLOQUE) + (PrimerJournalingLibre * sizeof(JOURNALING)));
                    PrimerJournalingLibre++;

                }
                return;
            }
        }

    }else{
        printf("Usuario Root debe iniciar sesion.\n");
    }

    int primerInodoLibre = getPrimerInodoLibre(UsuarioLog.Path,sb.s_bm_inode_start);
    int primeBlloqueLibre = getPrimerBloqueLibre(UsuarioLog.Path,sb.s_bm_block_start);

    sb.s_first_ino = primerInodoLibre;
    sb.s_first_blo = primeBlloqueLibre;

    setSB(sb,UsuarioLog.Path,UsuarioLog.StartParticion);        //actualizamos el sb
}

void Rmusr(char comando[500]){
    int i = 0;
    bool Error = false;
    while (comando[i] == ' ') {
        i++;
    }
    i = i+5;
    while(comando[i] == ' '){
        i++;
    }

    char USR[11];
    memset(&USR,'\0',sizeof(USR));
    for (i = i; i < 500; i++) {
        if((comando[i] == 'u' || comando[i] == 'U')
                &&(comando[i+1] == 's' || comando[i+1] == 'S')
                &&(comando[i+2] == 'r' || comando[i+2] == 'R')){

            char *usr = (char*)malloc(sizeof(usr));
            usr = getUSR(comando,i);
            int indice = 0;
            while(*usr != '\0'){
                char letra = *usr;
                USR[indice] = letra;
                usr++;
                indice++;
            }
            i = indc;
            printf("Id_Usuario: %s\n",USR);
            break;
        }else if(comando[i] == '#'){
            break;
        }else{
            if(comando[i-1] == '\n'){
                break;
            }
            else if(comando[i] == '\0'){
                break;
            }
            else if(comando[i] != '-' && comando[i] != ' ' && comando[i] != '\n' && comando[i] != '\r'){
                printf("Error en los parametros del comando rmusr.\n");
                return;
            }
        }
    }

    /*  CAPTURAMOS TODO EL ARCHIVO DE USUARIOS COMPLETO*/
    SUPERBLOQUE sb = getSB(UsuarioLog.Path,UsuarioLog.StartParticion);
    TABLAINODOS inodoUsr = getInodo(UsuarioLog.Path,sb.s_inode_start + sizeof(TABLAINODOS));

    char ArchivoUser[Bytesmaximosdearchivo];
    memset(&ArchivoUser,'\0',sizeof(ArchivoUser));
    int indice = 0;
    bool sisepudo = false;          //bandera de satisfaccion
    for (int var = 0; var < 15; ++var) {
        if(inodoUsr.i_block[var] != -1){
            char *archivo = (char*)malloc(sizeof(archivo));
            archivo =getArchivo(UsuarioLog.Path,sb.s_block_start + (inodoUsr.i_block[var] * sizeof(BLOQUEARCHIVO)));
            //int indice = 0;
            while (*archivo != NULL) {
                char letra = *archivo;
                ArchivoUser[indice] = letra;
                archivo++;
                indice++;
            }

            //vemos si en ese bloque esta el grupo que deseamos eliminar
            bool estado = false;
            char NombreG[12];
            memset(&NombreG,'\0',sizeof(NombreG));
            int ind = 0;
            int posicioncorrelativo = 0;  //numero de bit donde esta el correlativo en la linea
            bool yaestaennombre = false;
            int cantcomas = 0;
            for (int var1 = 0; var1 < strlen(ArchivoUser); ++var1) {

                if(ArchivoUser[var1] == 'U'){
                    posicioncorrelativo = var1-2;
                    var1++;
                    estado = true;
                    cantcomas = 0;
                }
                if(estado){
                    if(ArchivoUser[var1] != ','){
                        NombreG[ind] = ArchivoUser[var1];
                        ind++;
                    }
                    else{
                        cantcomas++;
                        /*  esto lo hago para que no se confunda al agarra el nombre de
                            grupo como que fuera el nombre del usuario entonces al encontrar
                            la primera coma reseteara el nombre e indicara que lo que viene
                            ya sera el nombre. y cuando venga otra coma pero ya estaba en estado de nombre verificara el nombre*/
                        if(cantcomas == 3){
                            if(strcmp(USR,NombreG) == 0){
                                //si es el grupo que deseo eliminar
                                //printf("Si es el grupo que deseo eliminar.\n");
                                //int nuevo = var1 - strlen(NombreG) - 4;
                                ArchivoUser[posicioncorrelativo] = '0';       //actualizamos el estado a '0'
                                //actualizamos
                                BLOQUEARCHIVO aux;
                                strcpy(aux.b_content,ArchivoUser);

                                InsertarBloque_Archivo(UsuarioLog.Path,sb.s_block_start + (inodoUsr.i_block[var] * sizeof(BLOQUEARCHIVO)),aux);
                                estado = false;
                                sisepudo = true;
                                yaestaennombre = false;

                                if(UsuarioLog.Tipo == 3 && Bandera_Esto_En_Recovery == false){
                                    JOURNALING nuevo;

                                    nuevo.Journal_Tipo_Operacion = 4;
                                    nuevo.Jorunal_Tipo = 0;
                                    memset(&nuevo.Journal_Nombre,'\0',sizeof(nuevo.Journal_Nombre));
                                    strcpy(nuevo.Journal_Nombre,"Rmusr");
                                    memset(&nuevo.Journal_Contenido,'\0',sizeof(nuevo.Journal_Contenido));
                                    strcpy(nuevo.Journal_Contenido,comando);
                                    nuevo.Jorunal_Fecha = time(0);
                                    nuevo.Journal_Propietario = 1;
                                    nuevo.Journal_Permisos = 664;

                                    SUPERBLOQUE sb;
                                    sb = getSB(UsuarioLog.Path,UsuarioLog.StartParticion);
                                    //seguido del super bloque empezaran los journaling
                                    //siempre y cuando sea paraticion en ext3

                                    setJOURNALING(nuevo,UsuarioLog.Path,UsuarioLog.StartParticion +sizeof(SUPERBLOQUE) + (PrimerJournalingLibre * sizeof(JOURNALING)));
                                    PrimerJournalingLibre++;

                                }

                                break;
                            }else{
                                memset(&NombreG,'\0',sizeof(NombreG));
                                //printf("NO es el grupo que deseo eliminar.\n");
                                estado = false;
                                ind = 0;
                            }
                        }else{
                            memset(&NombreG,'\0',sizeof(NombreG));
                            ind = 0;
                        }
                    }
                }
            }
            memset(&ArchivoUser,'\0',sizeof(ArchivoUser));
            indice = 0;
        }
    }
    if(sisepudo == false){
        printf("Usuario no existe.\n");
    }
}

void Rmgrp(char comando[500]){
    int i = 0;
    bool Error = false;
    while (comando[i] == ' ') {
        i++;
    }
    i = i+5;
    while(comando[i] == ' '){
        i++;
    }

    char NAME[11];
    memset(&NAME,'\0',sizeof(NAME));
    for (i = i; i < 500; i++) {
        if((comando[i] == 'n' || comando[i] == 'N')
                &&(comando[i+1] == 'a' || comando[i+1] == 'A')
                &&(comando[i+2] == 'm' || comando[i+2] == 'M')
                &&(comando[i+3] == 'e' || comando[i+3] == 'E')){

            char *name = (char*)malloc(sizeof(name));
            name = getName(comando,i);

            int indice = 0;
            while (*name != NULL) {
                char letra = *name;
                NAME[indice] = letra;
                name++;
                indice++;
            }
            printf("Rmgrp Name: %s\n",NAME);
            break;
        }else if(comando[i] == '#'){
            break;
        }else{
            if(comando[i-1] == '\n'){
                break;
            }
            else if(comando[i] == '\0'){
                break;
            }
            else if(comando[i] != '-' && comando[i] != ' ' && comando[i] != '\n' && comando[i] != '\r'){
                printf("Error en los parametros del comando rmgrp.\n");
                return;
            }
        }
    }

    /*  CAPTURAMOS TODO EL ARCHIVO DE USUARIOS COMPLETO*/
    SUPERBLOQUE sb = getSB(UsuarioLog.Path,UsuarioLog.StartParticion);
    TABLAINODOS inodoUsr = getInodo(UsuarioLog.Path,sb.s_inode_start + sizeof(TABLAINODOS));

    char ArchivoUser[Bytesmaximosdearchivo];
    memset(&ArchivoUser,'\0',sizeof(ArchivoUser));
    int indice = 0;
    bool sisepudo = false;          //bandera de satisfaccion
    for (int var = 0; var < 15; ++var) {
        if(inodoUsr.i_block[var] != -1){
            char *archivo = (char*)malloc(sizeof(archivo));
            archivo =getArchivo(UsuarioLog.Path,sb.s_block_start + (inodoUsr.i_block[var] * sizeof(BLOQUEARCHIVO)));
            //int indice = 0;
            while (*archivo != NULL) {
                char letra = *archivo;
                ArchivoUser[indice] = letra;
                archivo++;
                indice++;
            }

            //vemos si en ese bloque esta el grupo que deseamos eliminar
            bool estado = false;
            char NombreG[12];
            memset(&NombreG,'\0',sizeof(NombreG));
            int ind = 0;
            for (int var1 = 0; var1 < strlen(ArchivoUser); ++var1) {

                if(ArchivoUser[var1] == 'G'){
                    var1++;
                    estado = true;
                }
                if(ArchivoUser[var1] == '\n'){
                    if(strcmp(NAME,NombreG) == 0){
                        //si es el grupo que deseo eliminar
                        //printf("Si es el grupo que deseo eliminar.\n");
                        int nuevo = var1 - strlen(NombreG) - 4;
                        ArchivoUser[nuevo] = '0';       //actualizamos el estado a '0'
                        //actualizamos
                        BLOQUEARCHIVO aux;
                        strcpy(aux.b_content,ArchivoUser);

                        InsertarBloque_Archivo(UsuarioLog.Path,sb.s_block_start + (inodoUsr.i_block[var] * sizeof(BLOQUEARCHIVO)),aux);
                        estado = false;
                        sisepudo = true;

                        if(UsuarioLog.Tipo == 3 && Bandera_Esto_En_Recovery == false){
                            JOURNALING nuevo;

                            nuevo.Journal_Tipo_Operacion = 5;
                            nuevo.Jorunal_Tipo = 0;
                            memset(&nuevo.Journal_Nombre,'\0',sizeof(nuevo.Journal_Nombre));
                            strcpy(nuevo.Journal_Nombre,"Rmgrp");
                            memset(&nuevo.Journal_Contenido,'\0',sizeof(nuevo.Journal_Contenido));
                            strcpy(nuevo.Journal_Contenido,comando);
                            nuevo.Jorunal_Fecha = time(0);
                            nuevo.Journal_Propietario = 1;
                            nuevo.Journal_Permisos = 664;

                            SUPERBLOQUE sb;
                            sb = getSB(UsuarioLog.Path,UsuarioLog.StartParticion);
                            //seguido del super bloque empezaran los journaling
                            //siempre y cuando sea paraticion en ext3

                            setJOURNALING(nuevo,UsuarioLog.Path,UsuarioLog.StartParticion +sizeof(SUPERBLOQUE) + (PrimerJournalingLibre * sizeof(JOURNALING)));
                            PrimerJournalingLibre++;

                        }

                        break;
                    }else{
                        memset(&NombreG,'\0',sizeof(NombreG));
                        //printf("NO es el grupo que deseo eliminar.\n");
                        estado = false;
                        ind = 0;
                    }
                }
                if(estado){
                    if(ArchivoUser[var1] != ','){
                        NombreG[ind] = ArchivoUser[var1];
                        ind++;
                    }
                }
            }
            memset(&ArchivoUser,'\0',sizeof(ArchivoUser));
            indice = 0;
        }
    }
    if(sisepudo == false){
        printf("Grupo no existe.\n");
    }
}

void Mkgrp(char comando[500]){
    int i = 0;
    bool Error = false;
    while (comando[i] == ' ') {
        i++;
    }
    i = i+5;
    while(comando[i] == ' '){
        i++;
    }

    char NAME[11];
    memset(&NAME,'\0',sizeof(NAME));
    for (i = i; i < 500; i++) {
        if((comando[i] == 'n' || comando[i] == 'N')
                &&(comando[i+1] == 'a' || comando[i+1] == 'A')
                &&(comando[i+2] == 'm' || comando[i+2] == 'M')
                &&(comando[i+3] == 'e' || comando[i+3] == 'E')){

            char *name = (char*)malloc(sizeof(name));
            name = getName(comando,i);

            int indice = 0;
            while (*name != NULL) {
                char letra = *name;
                NAME[indice] = letra;
                name++;
                indice++;
            }
            printf("Mkgrp Name: %s\n",NAME);
            break;
        }else if(comando[i] == '#'){
            break;
        }else{
            if(comando[i-1] == '\n'){
                break;
            }
            else if(comando[i] == '\0'){
                break;
            }
            else if(comando[i] != '-' && comando[i] != ' ' && comando[i] != '\n' && comando[i] != '\r'){
                printf("Error en los parametros del comando mkgrp.\n");
                return;
            }
        }
    }
    /*creacion de grupo*/
    SUPERBLOQUE sb;
    sb = getSB(UsuarioLog.Path,UsuarioLog.StartParticion);

    if(sb.s_magic != 0)
    {
        //strcat(Archivo,grupo);
        /*  Agregamos el archivo de user a una cadena a auxiliar y concatenamos el nuevoo grupo
            y se verificara que el contenido no super los 64 bits de caso contrario debera crearse
            otro bloque de contenido.*/

        char Auxiliar[12*64]; //cpaacidad para 12 bloques de contenido de 64 bits c/u /* solo apuntadores directo */
        memset(&Auxiliar,'\0',sizeof(Auxiliar));

        char Auxiliar1[12*64]; //cpaacidad para 12 bloques de contenido de 64 bits c/u /* solo apuntadores directo */
        memset(&Auxiliar1,'\0',sizeof(Auxiliar1));

        TABLAINODOS inodouser = getInodo(UsuarioLog.Path,(sb.s_inode_start + sizeof(TABLAINODOS)));
        //ahora concatenamos todo el contenido.
        int bloqueenquemequede = 0;   //NUMERO ACTUAL QUE LEI...
        int indice1 = 0;

        for (int var = 0; var < 15; ++var) {

            if(inodouser.i_block[var] != -1){

                int x = (int)inodouser.i_block[var];
                char *arch = (char*)malloc(sizeof(arch));

                arch = getArchivo(UsuarioLog.Path,(sb.s_block_start + (x*sizeof(BLOQUEAPUNTADORES))));        //aumentamos 1 porque el bloque que es de la raiz
                //cantidad de caracteres que tiene ese bloque.
                int c =0 ;
                while (*arch != NULL) {
                    char letra = *arch;
                    Auxiliar1[indice1] = letra;
                    arch++;
                    c++;
                    indice1++;
                }

                if(c < 64){     //si el bloque aun tiene capacidad para almacenar mas caracteres.
                    bloqueenquemequede = x;
                    arch = getArchivo(UsuarioLog.Path,(sb.s_block_start + (x*sizeof(BLOQUEAPUNTADORES))));        //aumentamos 1 porque el bloque que es de la raiz

                    char Archivo[64]; //aca almacenaremos el archivo que recuperemos
                    memset(&Archivo,'\0',sizeof(Archivo));

                    int indice = 0;
                    while (*arch != NULL) {
                        char letra = *arch;
                        Archivo[indice] = letra;
                        arch++;
                        indice++;
                    }
                    strcat(Auxiliar,Archivo);           //concatenamos todos losbloques de tipo archivo que sean diferente de -1

                    /*--analisis para saber el numero de grupo que toca*/
                    int numero = 0;
                    for (int var = 0; var < 12*64; ++var) {
                        if(Auxiliar1[var] == 'G' && Auxiliar1[var-1] == ',' && Auxiliar1[var+1] == ','){
                            numero++;
                        }
                    }
                    // printf("Hay %d grupos.\n",numero);
                    numero++;

                    char num[6];
                    memset(&num,'\0',sizeof(num));
                    sprintf(num,"%d",numero);

                    //nuevo grupo
                    char grupo[25];
                    memset(&grupo,'\0',sizeof(grupo));
                    strcat(grupo,num);
                    strcat(grupo,",G,");
                    strcat(grupo,NAME);
                    strcat(grupo,"\n");

                    //concatenamos el nuevo grupo a todo el texto
                    strcat(Auxiliar,grupo);

                    //ahora verificamos cuantos caracteres tiene tiene todo el texto para saber cuantos bloques usaremos
                    int cantidad =0;
                    for (int var = 0; var < strlen(Auxiliar); ++var) {
                        if(Auxiliar[var] != '\0'){
                            cantidad++;
                        }
                    }
                    //printf("EL archivo tiene %d caractees.\n",cantidad);

                    if(cantidad <= 64){
                        //si la cantidad nueva sigue siendo menor a 64 bits se escribe en el bloque que leimos
                        BLOQUEARCHIVO nuevo;
                        memset(&nuevo.b_content,'\0',sizeof(nuevo.b_content));
                        strcpy(nuevo.b_content,Auxiliar);
                        InsertarBloque_Archivo(UsuarioLog.Path,sb.s_block_start + (bloqueenquemequede*sizeof(BLOQUEARCHIVO)),nuevo);

                        //RECUPERACION DE ARCHIVO USER
                        // char *archivo = (char*)malloc(sizeof(archivo));
                        //archivo = getArchivo(UsuarioLog.Path,sb.s_block_start + (bloqueenquemequede*sizeof(BLOQUEAPUNTADORES)));
                        //printf("Archivo:\n%s\n",archivo);

                        //int primerblo1 = getPrimerBloqueLibre(UsuarioLog.Path,sb.s_bm_block_start);
                        //int primerino1 = getPrimerInodoLibre(UsuarioLog.Path,sb.s_bm_inode_start);
                        //printf("Primer Bloque: %d\nPrimer Inodo: %d\n",primerblo1,primerino1);


                        if(UsuarioLog.Tipo == 3 && Bandera_Esto_En_Recovery == false){
                            JOURNALING nuevo;

                            nuevo.Journal_Tipo_Operacion = 2;
                            nuevo.Jorunal_Tipo = 0;

                            memset(&nuevo.Journal_Nombre,'\0',sizeof(nuevo.Journal_Nombre));
                            strcpy(nuevo.Journal_Nombre,"Mkgrp");

                            memset(&nuevo.Journal_Contenido,'\0',sizeof(nuevo.Journal_Contenido));
                            strcpy(nuevo.Journal_Contenido,comando);

                            nuevo.Jorunal_Fecha = time(0);
                            nuevo.Journal_Propietario = 1;
                            nuevo.Journal_Permisos = 664;

                            SUPERBLOQUE sb;
                            sb = getSB(UsuarioLog.Path,UsuarioLog.StartParticion);
                            //seguido del super bloque empezaran los journaling
                            //siempre y cuando sea paraticion en ext3

                            setJOURNALING(nuevo,UsuarioLog.Path,UsuarioLog.StartParticion +sizeof(SUPERBLOQUE) + (PrimerJournalingLibre * sizeof(JOURNALING)));
                            PrimerJournalingLibre++;

                        }

                        return;
                    }else{

                        //como el archivo excede los 64 bits haremos uso de mas bloques de contenido
                        char PrimerArchivo[64];
                        memset(&PrimerArchivo,'\0',sizeof(PrimerArchivo));

                        char *pauxiliar = (char*)malloc(sizeof(pauxiliar));
                        pauxiliar = Auxiliar;

                        int indice = 0;
                        while (indice < 64) {
                            char letra = *pauxiliar;
                            PrimerArchivo[indice] = letra;
                            pauxiliar++;
                            indice++;
                        }

                        BLOQUEARCHIVO nuevo;
                        memset(&nuevo.b_content,'\0',sizeof(nuevo.b_content));
                        strcpy(nuevo.b_content,PrimerArchivo);
                        InsertarBloque_Archivo(UsuarioLog.Path,sb.s_block_start + (bloqueenquemequede * sizeof(BLOQUEARCHIVO)),nuevo);


                        while (*pauxiliar != NULL) {
                            indice = 0;
                            char Arch[64];
                            memset(&Arch,'\0',sizeof(Arch));

                            while (*pauxiliar != NULL) {
                                if(indice < 64){
                                    char letra = *pauxiliar;
                                    Arch[indice] = letra;
                                    pauxiliar++;
                                    indice++;

                                }else{
                                    break;
                                }
                            }
                            int primebloquelibre = getPrimerBloqueLibre(UsuarioLog.Path,sb.s_bm_block_start);

                            BLOQUEARCHIVO masnuevo;
                            memset(&masnuevo.b_content,'\0',sizeof(masnuevo.b_content));
                            strcpy(masnuevo.b_content,Arch);
                            InsertarBloque_Archivo(UsuarioLog.Path,sb.s_block_start + (primebloquelibre * sizeof(BLOQUEARCHIVO)),masnuevo);

                            ActualizarBBLOQUE(UsuarioLog.Path,sb.s_bm_block_start,primebloquelibre);

                            //actualizamos el inodo
                            //el primer apuntador -1 que encuentra
                            int var1 =0;
                            for (var1 = 0; var1 < 12; ++var1) {
                                if(inodouser.i_block[var1] == -1){
                                    break;
                                }
                            }
                            inodouser.i_block[var1] = primebloquelibre;
                            InsertarInodo(UsuarioLog.Path,sb.s_inode_start + sizeof(TABLAINODOS),inodouser);

                            TABLAINODOS inuser = getInodo(UsuarioLog.Path,sb.s_inode_start + sizeof(TABLAINODOS));
                            //RECUPERACION DE ARCHIVO USER aun n o esta
                            //char *archivo = (char*)malloc(sizeof(archivo));
                            //archivo = getArchivo(UsuarioLog.Path,sb.s_block_start + (primebloquelibre*sizeof(BLOQUEAPUNTADORES)));
                            // printf("Archivo:\n%s\n",archivo);

                            if(UsuarioLog.Tipo == 3 && Bandera_Esto_En_Recovery == false){
                                JOURNALING nuevo;

                                nuevo.Journal_Tipo_Operacion = 2;
                                nuevo.Jorunal_Tipo = 0;

                                memset(&nuevo.Journal_Nombre,'\0',sizeof(nuevo.Journal_Nombre));
                                strcpy(nuevo.Journal_Nombre,"Mkgrp");

                                memset(&nuevo.Journal_Contenido,'\0',sizeof(nuevo.Journal_Contenido));
                                strcpy(nuevo.Journal_Contenido,comando);

                                nuevo.Jorunal_Fecha = time(0);
                                nuevo.Journal_Propietario = 1;
                                nuevo.Journal_Permisos = 664;

                                SUPERBLOQUE sb;
                                sb = getSB(UsuarioLog.Path,UsuarioLog.StartParticion);
                                //seguido del super bloque empezaran los journaling
                                //siempre y cuando sea paraticion en ext3

                                setJOURNALING(nuevo,UsuarioLog.Path,UsuarioLog.StartParticion +sizeof(SUPERBLOQUE) + (PrimerJournalingLibre * sizeof(JOURNALING)));
                                PrimerJournalingLibre++;

                            }
                        }
                    }
                    break;
                }else{
                    //SI YA NO TIENE CAPACIDAD PARA ALMACENAR MAS CARACTERES PARAMOS AL SIGUIENTE APUNTADOR
                }
            }else{
                //es -1 osea que esta libre

                //creamos el nuevo nodo
                /*--analisis para saber el numero de grupo que toca*/
                int numero = 0;
                for (int var = 0; var < 12*64; ++var) {
                    if(Auxiliar1[var] == 'G' && Auxiliar1[var-1] == ',' && Auxiliar1[var+1] == ','){
                        numero++;
                    }
                }
                //printf("Hay %d grupos.\n",numero);
                numero++;

                char num[6];
                memset(&num,'\0',sizeof(num));
                sprintf(num,"%d",numero);

                //nuevo grupo
                char grupo[25];
                memset(&grupo,'\0',sizeof(grupo));
                strcat(grupo,num);
                strcat(grupo,",G,");
                strcat(grupo,NAME);
                strcat(grupo,"\n");

                //concatenamos el nuevo grupo a todo el texto
                strcat(Auxiliar,grupo);

                int primebloquelibre = getPrimerBloqueLibre(UsuarioLog.Path,sb.s_bm_block_start);
                BLOQUEARCHIVO masnuevo;
                memset(&masnuevo.b_content,'\0',sizeof(masnuevo.b_content));
                strcpy(masnuevo.b_content,Auxiliar);
                InsertarBloque_Archivo(UsuarioLog.Path,sb.s_block_start + (primebloquelibre * sizeof(BLOQUEARCHIVO)),masnuevo);

                ActualizarBBLOQUE(UsuarioLog.Path,sb.s_bm_block_start,primebloquelibre);

                //actualizamos el inodo
                inodouser.i_block[var] = primebloquelibre;
                InsertarInodo(UsuarioLog.Path,sb.s_inode_start + sizeof(TABLAINODOS),inodouser);


                TABLAINODOS inuser = getInodo(UsuarioLog.Path,sb.s_inode_start + sizeof(TABLAINODOS));
                //RECUPERACION DE ARCHIVO USER aun n o esta
                //char *archivo = (char*)malloc(sizeof(archivo));
                //archivo = getArchivo(UsuarioLog.Path,sb.s_block_start + (primebloquelibre*sizeof(BLOQUEAPUNTADORES)));
                //printf("Archivo:\n%s\n",archivo);

                //int primerblo1 = getPrimerBloqueLibre(UsuarioLog.Path,sb.s_bm_block_start);
                // int primerino1 = getPrimerInodoLibre(UsuarioLog.Path,sb.s_bm_inode_start);
                //printf("Primer Bloque: %d\nPrimer Inodo: %d\n",primerblo1,primerino1);

                if(UsuarioLog.Tipo == 3 && Bandera_Esto_En_Recovery == false){
                    JOURNALING nuevo;

                    nuevo.Journal_Tipo_Operacion = 2;
                    nuevo.Jorunal_Tipo = 0;

                    memset(&nuevo.Journal_Nombre,'\0',sizeof(nuevo.Journal_Nombre));
                    strcpy(nuevo.Journal_Nombre,"Mkgrp");

                    memset(&nuevo.Journal_Contenido,'\0',sizeof(nuevo.Journal_Contenido));
                    strcpy(nuevo.Journal_Contenido,comando);

                    nuevo.Jorunal_Fecha = time(0);
                    nuevo.Journal_Propietario = 1;
                    nuevo.Journal_Permisos = 664;

                    SUPERBLOQUE sb;
                    sb = getSB(UsuarioLog.Path,UsuarioLog.StartParticion);
                    //seguido del super bloque empezaran los journaling
                    //siempre y cuando sea paraticion en ext3


                    setJOURNALING(nuevo,UsuarioLog.Path,UsuarioLog.StartParticion +sizeof(SUPERBLOQUE) + (PrimerJournalingLibre * sizeof(JOURNALING)));
                    PrimerJournalingLibre++;

                }
                return;
            }
        }

    }else{
        printf("Usuario Root debe iniciar sesion.\n");
    }


    int primerInodoLibre = getPrimerInodoLibre(UsuarioLog.Path,sb.s_bm_inode_start);
    int primeBlloqueLibre = getPrimerBloqueLibre(UsuarioLog.Path,sb.s_bm_block_start);

    sb.s_first_ino = primerInodoLibre;
    sb.s_first_blo = primeBlloqueLibre;

    setSB(sb,UsuarioLog.Path,UsuarioLog.StartParticion);        //actualizamos el sb
}

//setea el bitmpa de inodos
void setBINODO(char PathDisco[250], int StarBM, char BIT[NumeroStructuras]){

    char Bitmap[NumeroStructuras];
    memset(&Bitmap,'0',sizeof(Bitmap));
    strcpy(Bitmap,BIT);

    FILE *Archivo;
    Archivo = fopen(PathDisco,"rb+");

    if(Archivo!=NULL)
    {
        //escribimos el bitmap de inodos
        fseek(Archivo,StarBM,SEEK_SET);
        fwrite(&Bitmap,sizeof(Bitmap),1,Archivo);

        fclose(Archivo);
        fflush(Archivo);
    }
    else{
        printf("El disco Especificado No existe.(SB-MKFS)\n");
        return;
    }
}

//setea el bitmap de bloque
void setBBLOQUE(char PathDisco[250], int StarBM, char BIT[3*NumeroStructuras]){

    char Bitmap[3*NumeroStructuras];
    memset(&Bitmap,'0',sizeof(Bitmap));
    strcpy(Bitmap,BIT);

    FILE *Archivo;
    Archivo = fopen(PathDisco,"rb+");

    if(Archivo!=NULL)
    {
        //escribimos el bitmap de inodos
        fseek(Archivo,StarBM,SEEK_SET);
        fwrite(&Bitmap,sizeof(Bitmap),1,Archivo);

        fclose(Archivo);
        fflush(Archivo);
    }
    else{
        printf("El disco Especificado No existe.(SB-MKFS)\n");
        return;
    }
}

//retorna el bitmap de bloque completo
char *getBIBLOQUE(char PathDisco[250],int Start){

    //recuperacion de bitmap de inodos y bloques.
    /*---- generalizar en un solo metodo*/
    char *bitmap = (char*)malloc(sizeof(bitmap));
    FILE *apun;
    char bloq[3*NumeroStructuras];
    memset(&bloq,'0',sizeof(bloq));

    apun = fopen(PathDisco,"rb");
    if (apun != NULL){

        int iniciobloq = Start;
        fseek(apun,iniciobloq,SEEK_SET);
        fread(&bloq,sizeof(bloq),1,apun);

        fclose(apun);
        fflush(apun);

    }else{
        memset(&bloq,'0',bloq);
    }

    bitmap = bloq;
    return bitmap;
}

//retorna el bitmap de inodo completo
char *getBIINODO(char PathDisco[250],int Start){

    //recuperacion de bitmap de inodos y bloques.
    /*---- generalizar en un solo metodo*/
    char *bitmap = (char*)malloc(sizeof(bitmap));
    FILE *apun;
    char ino[NumeroStructuras];
    memset(&ino,'0',sizeof(ino));

    apun = fopen(PathDisco,"rb");
    if (apun != NULL){

        int iniciobloq = Start;
        fseek(apun,iniciobloq,SEEK_SET);
        fread(&ino,sizeof(ino),1,apun);

        fclose(apun);
        fflush(apun);

    }else{
        memset(&ino,'0',ino);
    }

    bitmap = ino;
    return bitmap;
}

//retorna la posicion del primer bloque libre
int getPrimerBloqueLibre(char PathDisco[250],int StartBitmap){

    //recuperacion de bitmap de inodos y bloques.
    /*---- generalizar en un solo metodo*/
    FILE *apun;

    char bloq[3*NumeroStructuras];
    memset(&bloq,'0',sizeof(bloq));

    apun = fopen(PathDisco,"rb");
    if (apun != NULL){

        //int iniciobloq = StartParticion + sizeof(SUPERBLOQUE) + NumeroStructuras;
        fseek(apun,StartBitmap,SEEK_SET);
        fread(&bloq,sizeof(bloq),1,apun);

        fclose(apun);
        fflush(apun);

    }else{
        memset(&bloq,'0',bloq);
    }

    for (int var = 0; var < sizeof(bloq); ++var) {

        if(bloq[var] == '0'){       //posicion disponible
            return var;
        }
    }
}

//retorna la posiicon del primer inodo libre
int getPrimerInodoLibre(char PathDisco[250],int StartBitmap){

    //recuperacion de bitmap de inodos y bloques.
    /*---- generalizar en un solo metodo*/
    FILE *apun;
    char inodes[NumeroStructuras];
    memset(&inodes,'0',sizeof(inodes));

    apun = fopen(PathDisco,"rb");
    if (apun != NULL){

        //int inicio = StartParticion + sizeof(SUPERBLOQUE);
        fseek(apun,StartBitmap,SEEK_SET);
        fread(&inodes,sizeof(inodes),1,apun);

        fclose(apun);
        fflush(apun);

    }else{
        memset(&inodes,'0',inodes);
    }

    for (int var = 0; var < sizeof(inodes); ++var) {

        if(inodes[var] == '0'){       //posicion disponible
            return var;
        }
    }
}

//get inodo recibiendo la posicion exacta del inodo dentro del disco
TABLAINODOS getInodo(char PathDisco[250],int starInodo){

    FILE *ar;
    TABLAINODOS inodo;
    ar = fopen(PathDisco,"rb");

    if (ar != NULL){
        fseek(ar,starInodo,SEEK_SET);
        fread(&inodo,sizeof(inodo),1,ar);
        fclose(ar);
        fflush(ar);
        return inodo;

    }else{
        printf("No sirvio chavo. (getinodo)\n");
    }
}

void ActualizarBINODO(char PathDisco[250],int inodeStart,int PosicionActualizar){
    //metodo encargado de actualizar el bitmap de inodos,
    //recibira path del disco, posicion donde inicia el bitmap y
    //posicion en el bitmap que se desea actualizar

    /*-----RECUPERACION DE BITMAP DE INODOS PARA SABER CUAL ES PRIMERO VACIO Y ACTUALIZARLO DE UNA VEZ------*/
    char *BitInodo = (char*)malloc(sizeof(BitInodo));
    BitInodo = getBIINODO(PathDisco,inodeStart);
    char BIINODO[NumeroStructuras];
    int indice = 0;
    memset(&BIINODO,'\0',sizeof(BIINODO));
    while(*BitInodo != '\0'){
        char letra = *BitInodo;
        BIINODO[indice] = letra;
        BitInodo++;
        indice++;
    }
    //ACTUALIZACION DE BITMAP
    for (int var = 0; var < NumeroStructuras; ++var) {
        if(PosicionActualizar == var){
            BIINODO[var] = '1';
            setBINODO(PathDisco,inodeStart,BIINODO);
            break;
        }
    }
}

void ActualizarBBLOQUE(char PathDisco[250],int bloqueStart,int PosicionActualizar){
    //metodo encargado de actualizar el bitmap de bloques,
    //recibira path del disco, posicion donde inicia el bitmap y
    //posicion en el bitmap que se desea actualizar

    /*-----RECUPERACION DE BITMAP DE INODOS PARA SABER CUAL ES PRIMERO VACIO Y ACTUALIZARLO DE UNA VEZ------*/
    char *BitBloque = (char*)malloc(sizeof(BitBloque));
    BitBloque = getBIBLOQUE(PathDisco,bloqueStart);
    char BIBLOQUE[3*NumeroStructuras];
    int indice = 0;
    memset(&BIBLOQUE,'\0',sizeof(BIBLOQUE));
    while(*BitBloque != '\0'){
        char letra = *BitBloque;
        BIBLOQUE[indice] = letra;
        BitBloque++;
        indice++;
    }
    //ACTUALIZACION DE BITMAP
    for (int var = 0; var < 3*NumeroStructuras; ++var) {
        if(PosicionActualizar == var){
            BIBLOQUE[var] = '1';
            setBBLOQUE(PathDisco,bloqueStart,BIBLOQUE);
            break;
        }
    }
}

void ActualizarBINODOREMOVE(char PathDisco[250],int inodeStart,int PosicionActualizar){
    //metodo encargado de actualizar el bitmap de inodos,
    //recibira path del disco, posicion donde inicia el bitmap y
    //posicion en el bitmap que se desea actualizar

    /*-----RECUPERACION DE BITMAP DE INODOS PARA SABER CUAL ES PRIMERO VACIO Y ACTUALIZARLO DE UNA VEZ------*/
    char *BitInodo = (char*)malloc(sizeof(BitInodo));
    BitInodo = getBIINODO(PathDisco,inodeStart);
    char BIINODO[NumeroStructuras];
    int indice = 0;
    memset(&BIINODO,'\0',sizeof(BIINODO));
    while(*BitInodo != '\0'){
        char letra = *BitInodo;
        BIINODO[indice] = letra;
        BitInodo++;
        indice++;
    }
    //ACTUALIZACION DE BITMAP
    for (int var = 0; var < NumeroStructuras; ++var) {
        if(PosicionActualizar == var){
            BIINODO[var] = '0';
            setBINODO(PathDisco,inodeStart,BIINODO);
            break;
        }
    }
}

void ActualizarBBLOQUEREMOVE(char PathDisco[250],int bloqueStart,int PosicionActualizar){
    //metodo encargado de actualizar el bitmap de bloques,
    //recibira path del disco, posicion donde inicia el bitmap y
    //posicion en el bitmap que se desea actualizar

    /*-----RECUPERACION DE BITMAP DE INODOS PARA SABER CUAL ES PRIMERO VACIO Y ACTUALIZARLO DE UNA VEZ------*/
    char *BitBloque = (char*)malloc(sizeof(BitBloque));
    BitBloque = getBIBLOQUE(PathDisco,bloqueStart);
    char BIBLOQUE[3*NumeroStructuras];
    int indice = 0;
    memset(&BIBLOQUE,'\0',sizeof(BIBLOQUE));
    while(*BitBloque != '\0'){
        char letra = *BitBloque;
        BIBLOQUE[indice] = letra;
        BitBloque++;
        indice++;
    }
    //ACTUALIZACION DE BITMAP
    for (int var = 0; var < 3*NumeroStructuras; ++var) {
        if(PosicionActualizar == var){
            BIBLOQUE[var] = '0';
            setBBLOQUE(PathDisco,bloqueStart,BIBLOQUE);
            break;
        }
    }
}

/* INSERCION DE INODOS EN DISCO */
void InsertarInodo(char PathDisco[250],int StartInodo,TABLAINODOS Inodo){

    FILE *Archivo;
    Archivo = fopen(PathDisco,"rb+");
    if(Archivo!=NULL)
    {
        //escribimos el bitmap de inodos
        fseek(Archivo,StartInodo,SEEK_SET);
        fwrite(&Inodo,sizeof(Inodo),1,Archivo);

        fclose(Archivo);
        fflush(Archivo);
    }

}

/* INSERCION DE BLOQUES EN DISCO*/
void InsertarBloque_Carpeta(char PathDisco[250],int StartBloque,BLOQUECARPETA bloque){

    FILE *Archivo;
    Archivo = fopen(PathDisco,"rb+");
    if(Archivo!=NULL)
    {
        fseek(Archivo,StartBloque,SEEK_SET);
        fwrite(&bloque,sizeof(bloque),1,Archivo);

        fclose(Archivo);
        fflush(Archivo);
    }
}
void InsertarBloque_Apuntador(char PathDisco[250],int StartBloque,BLOQUEAPUNTADORES bloque){

    FILE *Archivo;
    Archivo = fopen(PathDisco,"rb+");
    if(Archivo!=NULL)
    {
        fseek(Archivo,StartBloque,SEEK_SET);
        fwrite(&bloque,sizeof(bloque),1,Archivo);

        fclose(Archivo);
        fflush(Archivo);
    }
}
void InsertarBloque_Archivo(char PathDisco[250],int StartBloque,BLOQUEARCHIVO bloque){

    FILE *Archivo;
    Archivo = fopen(PathDisco,"rb+");
    if(Archivo!=NULL)
    {
        fseek(Archivo,StartBloque,SEEK_SET);
        fwrite(&bloque,sizeof(bloque),1,Archivo);

        fclose(Archivo);
        fflush(Archivo);
    }
}

void Find(char coma[500]){
    char Path[250];
    char Name[10];
    bool Correcto = true;

    memset(&Path,'\0',sizeof(Path));
    memset(&Name,'\0',sizeof(Name));

    int i = 0;
    while (coma[i] == ' ') {
        i++;
    }
    i = i+4;
    while(coma[i] == ' '){
        i++;
    }

    for ( i = i; i < 500; i++) {
        if(coma[i] == '\0'){
            break;
        }

        if((coma[i] == 'p' || coma[i] == 'P')
                &&(coma[i+1] == 'a' || coma[i+1] == 'A')
                &&(coma[i+2] == 't' || coma[i+2] == 'T')
                &&(coma[i+3] == 'h' || coma[i+3] == 'H')){
            char *ruta = (char*)malloc(sizeof(ruta));
            ruta = getPath(coma,i);

            int indice = 0;
            memset(&Path,'\0',sizeof(Path));

            while(*ruta != '\0'){
                char letra = *ruta;
                Path[indice] = letra;
                ruta++;
                indice++;
            }

            printf("Ruta->");
            printf(Path);
            printf("\n");
            i = indc;
            indc = 0;
        }else if ((coma[i] == 'n' || coma[i] == 'N')
                  &&(coma[i+1] == 'a' || coma[i+1] == 'A')
                  &&(coma[i+2] == 'm' || coma[i+2] == 'M')
                  &&(coma[i+3] == 'e' || coma[i+3] == 'E')){
            char *nombre = (char*)malloc(sizeof(nombre));
            nombre = getName(coma ,i);
            int indice = 0;
            memset(&Name,'\0',sizeof(Name));
            while(*nombre != '\0'){
                char letra = *nombre;
                Name[indice] = letra;
                nombre++;
                indice++;
            }
            printf("Name: %s\n",Name);
            i = indc;
            indc = 0;
        }else if(coma[i] == '#' ){
            //al momento de ser un numeral detectara comentario y terminara el ciclo
            break;
        }else{
            if(coma[i-1] == '\n'){
                break;
            }
            else if(coma[i] != '-' && coma[i] != ' ' && coma[i] != '\n'){
                printf("Error en los parametros del comando mkdisk.");
                return;         //se sale del metodo ya que da error de parametros.
            }
        }
    }

    /*MANDAMOS A BUSCAR*/
    SeparaRutaFIND(Path,Name);

}

void SeparaRutaFIND(char RUTA[250],char Name[10]){

    //ruta va a ser la carptea de inicio de la busqueda
    char co[10];
    memset(&co,'\0',sizeof(co));
    strcpy(co,Name);

    SUPERBLOQUE sb = getSB(UsuarioLog.Path,UsuarioLog.StartParticion);

    bool para = false;
    TABLAINODOS padre;
    padre = getInodo(UsuarioLog.Path,sb.s_inode_start);
    char Pataux[250];
    memset(&Pataux,'\0', sizeof(Pataux));

    int indice = 1;
    int cantidadtabs = 0;
    for (int i = 0; i < strlen(RUTA); i++) {

        if(RUTA[0] == '/' && i == 0){
            Pataux[0]= '/';
            padre = VerificacionExisteCarpeta(padre, Pataux);
            printf("#%s\n",Pataux);
            cantidadtabs++;
            cantidadtabs++;
            if(RUTA[i +1] == '\0'){
                ImpresionFind(padre,cantidadtabs,Name);
                return;
            }

            cantidadtabs++;
            memset(&Pataux,'\0',sizeof(Pataux));
            i++;
            indice = 0;
        }else{
            if(RUTA[0] != '/' && i == 0){
                Pataux[0] = '/';
            }
        }
        while ((RUTA[i] != '/' || i == 0) &&  i < 250){
            Pataux[indice] = RUTA[i];
            i++;
            indice++;

            if(RUTA[i] == '.'){
                Pataux[indice] = RUTA[i];
                i++;
                indice++;
                para = true;
            }
        }

        if(!para){
            //mandamos a crear bloque de tipo archivo.
            para = false;
        }else{
            //pataux = nombre del archivo.
            break;
        }

        TABLAINODOS aux = padre;
        padre = VerificacionExisteCarpeta(padre,Pataux);
        printf("#");
        for (int a = 0; a < cantidadtabs; a++) {
            printf(" ");
        }
        printf("|_");
        printf("%s\n",Pataux);
        cantidadtabs++;
        cantidadtabs++;

        if(padre.i_type == '\0'){
            printf("Carpeta %s no existe.\n",Pataux);
            return;
        }
        indice = 0;
        memset(&Pataux,'\0',sizeof(Pataux));

    }

    //el ultimo padre que quede sera a partir de ahi buscaremos en todo el archivo o carpeta
    //a partir de esta carpeta empezaremos a desglosar las carpetas.
    ImpresionFind(padre,cantidadtabs,Name);
}

void ImpresionFind(TABLAINODOS padre,int cantidadtabs,char Name[10]){

    SUPERBLOQUE sb = getSB(UsuarioLog.Path,UsuarioLog.StartParticion);

    if(padre.i_type == '0'){

        for (int var = 0; var < 15; ++var) {

            if(padre.i_block[var] != -1){

                BLOQUECARPETA bloque = getBloqueCarpeta(sb.s_block_start + (padre.i_block[var]* sizeof(BLOQUECARPETA)));

                int var1 = 0;
                if(var == 0){
                    var1 = var1 + 2;
                }
                for (var1 = var1; var1 < 4; var1++) {

                    if(bloque.Content[var1].b_inodo != -1){

                        TABLAINODOS inodoactual = getInodo(UsuarioLog.Path,sb.s_inode_start + (bloque.Content[var1].b_inodo * sizeof(TABLAINODOS)));

                        if(inodoactual.i_type == '1'){
                            if(strcasecmp(Name,"?.*") == 0){

                                if(bloque.Content[var1].b_name[1] == '.'){          //la unica referencia del nombre de un archivo esta en el bloque padre
                                    printf("#");
                                    for (int a = 0; a < cantidadtabs; a++) {
                                        printf(" ");
                                    }
                                    printf("|_");
                                    printf("%s\n",bloque.Content[var1].b_name);
                                    // cantidadtabs++;
                                    int aux = cantidadtabs;

                                    ImpresionFind(inodoactual,cantidadtabs,Name);
                                    cantidadtabs = aux;
                                }

                            }else{
                                printf("#");
                                for (int a = 0; a < cantidadtabs; a++) {
                                    printf(" ");
                                }
                                printf("|_");
                                printf("%s\n",bloque.Content[var1].b_name);
                                // cantidadtabs++;
                                int aux = cantidadtabs;

                                if(inodoactual.i_type == '0'){
                                    cantidadtabs++;
                                    cantidadtabs++;
                                }
                                ImpresionFind(inodoactual,cantidadtabs,Name);
                                cantidadtabs = aux;
                            }


                        }else{

                            printf("#");
                            for (int a = 0; a < cantidadtabs; a++) {
                                printf(" ");
                            }
                            printf("|_");
                            printf("%s\n",bloque.Content[var1].b_name);
                            // cantidadtabs++;
                            int aux = cantidadtabs;

                            if(inodoactual.i_type == '0'){
                                cantidadtabs++;
                                cantidadtabs++;
                            }
                            ImpresionFind(inodoactual,cantidadtabs,Name);
                            cantidadtabs = aux;
                        }
                    }
                }
            }
        }
    }else{
        //si es archivo aqui ya hacemos la validacion del parametro name de el comando find.

    }
}

void Loss(char coma[500]){

    char Id[9];
    bool Correcto = true;
    memset(&Id,'\0',sizeof(Id));

    int i = 0;
    while (coma[i] == ' ') {
        i++;
    }
    i = i+4;
    while(coma[i] == ' '){
        i++;
    }

    for ( i = i; i < 500; i++) {
        if(coma[i] == '\0'){
            break;
        }

        if ((coma[i] == 'i' || coma[i] == 'I')
                &&(coma[i+1] == 'd' || coma[i+1] == 'D')){
            char id[15];
            memset(id,'\0',sizeof(id));
            id[0] = coma[i];
            id[1] = coma[i+1];
            id[2] = coma[i+2];
            if(strcasecmp(id,"id=") == 0){
                i = i + 3;
                while(coma[i] == ' '){
                    i++;
                }
                //inicializamos el arreglo en nulo
                memset(Id,'\0', sizeof(Id));
                int indice2 = 0;
                while(coma[i] != ' ' && coma[i] != '\n'){
                    Id[indice2] = coma[i];
                    indice2++;
                    if(i < strlen(coma)){
                        i++;
                    }else{
                        break;
                    }
                }

                printf("ID Loss: %s\n",Id);
            }else{
                printf("Error en la sintaxis de Id=.(Rep)\n");
                return;
            }
        } else if(coma[i] == '#' ){
            //al momento de ser un numeral detectara comentario y terminara el ciclo
            break;
        }else{
            if(coma[i-1] == '\n'){
                break;
            }
            else if(coma[i] != '-' && coma[i] != ' ' && coma[i] != '\n'){
                printf("Error en los parametros del comando LOSS.");
                return;         //se sale del metodo ya que da error de parametros.
            }
        }
    }

    MONTADURAS *aux = primero;
    char NombreParticion[20];
    char PathDisco[250];
    int startpartiicon=0;
    bool Siestamontada = false;

    memset(&NombreParticion,'\0',sizeof(NombreParticion));
    memset(&PathDisco,'\0',sizeof(PathDisco));

    while(aux != NULL){
        if(strcasecmp(aux->Id, Id) == 0){
            strcpy(NombreParticion,aux->Name);
            strcpy(PathDisco,aux->Path);
            startpartiicon = aux->StartParticion;
            Siestamontada = true;
            break;
        }else{
            aux = aux->siguiente;
        }
    }
    if(Siestamontada){

        //si esta montada hacemos la limpieza.

        MBR mbr =getMBR(PathDisco);
        SUPERBLOQUE sb = getSB(PathDisco,startpartiicon);
        int n = 0;
        if(strcmp(mbr.mbr_partition_1.part_name,NombreParticion)==0){
            n = DespejandoN(mbr.mbr_partition_1.part_size);
        }
        else if(strcmp(mbr.mbr_partition_2.part_name,NombreParticion)==0){
            n = DespejandoN(mbr.mbr_partition_2.part_size);
        }
        else if(strcmp(mbr.mbr_partition_3.part_name,NombreParticion)==0){
            n = DespejandoN(mbr.mbr_partition_3.part_size);
        }
        else if(strcmp(mbr.mbr_partition_4.part_name,NombreParticion)==0){
            n = DespejandoN(mbr.mbr_partition_4.part_size);
        }


        /*      SETAMOS EL BITMAP DE INODOS EN NULOS        */
        char BitmapI[n];
        memset(&BitmapI,'\0',sizeof(BitmapI));
        setBINODO(PathDisco,sb.s_bm_inode_start,BitmapI);

        /*      SETEAMOS EL BITMAP DE BLOQUES EN NULOS      */
        char BitmapB[3*n];
        memset(&BitmapB,'\0',sizeof(BitmapB));
        setBINODO(PathDisco,sb.s_bm_block_start,BitmapB);

        /*      SETEAMOS UN ARREGLO DEL TAMA;O DE N*SIZEOF(INODO)       */
        char INODO[NumeroStructuras*sizeof(TABLAINODOS)];
        memset(&INODO,'\0',sizeof(INODO));
        FILE *Archivo;
        Archivo = fopen(PathDisco,"rb+");
        if(Archivo!=NULL)
        {
            fseek(Archivo,sb.s_inode_start,SEEK_SET);
            fwrite(&INODO,sizeof(INODO),1,Archivo);

            fclose(Archivo);
            fflush(Archivo);
        }
        else{
            printf("El disco Especificado No existe.(SB-MKFS)\n");
            return;
        }

        /*      SETEAMOS UNA ARREGLO DEL TAMA;O DE 3*N*SIZEOF(BLOQUE)       */
        char bloPart1[NumeroStructuras*64];
        //char BLOQUE[3*NumeroStructuras*sizeof(BLOQUEARCHIVO)];
        //memset(&BLOQUE,'\0',sizeof(3*NumeroStructuras*sizeof(BLOQUEARCHIVO)));
        memset(&bloPart1,'\0',sizeof(bloPart1));

        FILE *Archivo1;
        Archivo1 = fopen(PathDisco,"rb+");
        if(Archivo!=NULL)
        {
            fseek(Archivo1,sb.s_block_start,SEEK_SET);
            fwrite(&bloPart1,sizeof(bloPart1),1,Archivo1);

            fseek(Archivo1,sb.s_block_start + sizeof(bloPart1),SEEK_SET);
            fwrite(&bloPart1,sizeof(bloPart1),1,Archivo1);

            fseek(Archivo1,sb.s_block_start + sizeof(bloPart1),SEEK_SET);
            fwrite(&bloPart1,sizeof(bloPart1),1,Archivo1);

            fclose(Archivo1);
            fflush(Archivo1);
        }
        else{
            printf("El disco Especificado No existe.(SB-MKFS)\n");
            return;
        }

        printf("La particion: %s con id: %s ha sufrido un fallo irreparable, perdida total de la informacion\n",NombreParticion,Id);

    }else{
        printf("%s no se encuentra montada.\n",Id);
    }

}

void Cat(char coma[500]){
    primeroA = NULL;
    ultimoA = NULL;
    char Id[9];
    bool Correcto = true;
    memset(&Id,'\0',sizeof(Id));

    int i = 0;
    while (coma[i] == ' ') {
        i++;
    }
    i = i+4;
    while(coma[i] == ' '){
        i++;
    }

    for ( i = i; i < 500; i++) {
        if(coma[i] == '\0'){
            break;
        }

        if ((coma[i] == 'f' || coma[i] == 'F')
                &&(coma[i+1] == 'i' || coma[i+1] == 'I')
                &&(coma[i+2] == 'l' || coma[i+2] == 'L')
                &&(coma[i+3] == 'e' || coma[i+3] == 'E')){

            char *file = (char*)malloc(sizeof(file));
            file  = getFile(coma,i);

            char RUTA[250];
            memset(&RUTA,'\0',sizeof(RUTA));

            int indice = 0;
            while (*file != NULL) {
                char letra = *file;
                RUTA[indice] = letra;
                indice++;
                file++;
            }

            printf("Ruta: %s\n",RUTA);
            int numero = coma[i+4] - 48;
            i = indc;

            char Contenido[500];
            memset(&Contenido,'\0',sizeof(Contenido));

            char *contenido = BuscarArchivo(RUTA);
            int indice1 = 0;
            while (*contenido != NULL) {
                char letra = *contenido;
                Contenido[indice1] = letra;
                indice1++;
                contenido++;
            }


            ARCHIVOS *Nuevonodo = (ARCHIVOS*)malloc(sizeof(ARCHIVOS));
            strcpy(Nuevonodo->contenido,Contenido);
            Nuevonodo->numero = numero;

            //insertamos de una ves de forma ordenada.
            //cuando termine de capturar todos olos textos y almacenarlos los imprimiremos en orden establecido.
            ARCHIVOS *aux1 = primeroA;
            ARCHIVOS *aux2;

            while((aux1 != NULL) && aux1->numero < numero){
                aux2 = aux1;
                aux1 = aux1->siguiente;
            }


            if(primeroA == aux1){           //principio de la lista
                primeroA = Nuevonodo;
            }else{                          //dato mayo se agreaga despues
                aux2->siguiente = Nuevonodo;
            }

            Nuevonodo->siguiente = aux1;

        }  else if(coma[i] == '#' ){
            //al momento de ser un numeral detectara comentario y terminara el ciclo
            break;
        }else{
            if(coma[i-1] == '\n'){
                break;
            }
            else if(coma[i] != '-' && coma[i] != ' ' && coma[i] != '\n'){
                printf("Error en los parametros del comando file.");
                return;         //se sale del metodo ya que da error de parametros.
            }
        }
    }

    //imprimimos

    ARCHIVOS *aux = primeroA;
    printf("\n");
    while(aux != NULL){

        printf("# %s\n",aux->contenido);
        aux =  aux->siguiente;
    }

}

char NOMBRE_carptea_oarchivo[200];
void Mover(char coma[500]){
    memset(&NOMBRE_carptea_oarchivo,'\0',sizeof(NOMBRE_carptea_oarchivo));

    char ORIGEN[250];
    memset(&ORIGEN,'\0',sizeof(ORIGEN));

    char DESTINO[250];
    memset(&DESTINO,'\0',sizeof(DESTINO));

    int i = 0;
    while (coma[i] == ' ') {
        i++;
    }
    i = i+2;
    while(coma[i] == ' '){
        i++;
    }

    for ( i = i; i < 500; i++) {
        if(coma[i] == '\0'){
            break;
        }

        if ((coma[i] == 'p' || coma[i] == 'P')
                &&(coma[i+1] == 'a' || coma[i+1] == 'A')
                &&(coma[i+2] == 't' || coma[i+2] == 'T')
                &&(coma[i+3] == 'h' || coma[i+3] == 'H')){

            char *file = (char*)malloc(sizeof(file));
            file  = getPath(coma,i);


            int indice = 0;
            while (*file != NULL) {
                char letra = *file;
                ORIGEN[indice] = letra;
                indice++;
                file++;
            }

            printf("Origen: %s\n",ORIGEN);
            i = indc;

        }
        if ((coma[i] == 'd' || coma[i] == 'D')
                &&(coma[i+1] == 'e' || coma[i+1] == 'E')
                &&(coma[i+2] == 's' || coma[i+2] == 'S')
                &&(coma[i+3] == 't' || coma[i+3] == 'T')){

            char *file = (char*)malloc(sizeof(file));
            file  = getPath(coma,i);

            int indice = 0;
            while (*file != NULL) {
                char letra = *file;
                DESTINO[indice] = letra;
                indice++;
                file++;
            }

            printf("Destino: %s\n",DESTINO);
            i = indc;

        }else if(coma[i] == '#' ){
            //al momento de ser un numeral detectara comentario y terminara el ciclo
            break;
        }else{
            if(coma[i-1] == '\n'){
                break;
            }
            else if(coma[i] != '-' && coma[i] != ' ' && coma[i] != '\n'){
                printf("Error en los parametros del comando mover.");
                return;         //se sale del metodo ya que da error de parametros.
            }
        }
    }
    //mandamos a capturar y verificar las rutas

    SUPERBLOQUE sb;
    sb = getSB(UsuarioLog.Path,UsuarioLog.StartParticion);

    TABLAINODOS InodoaMover;
    int NumeroInodoMover = 0;

    TABLAINODOS padre;
    padre =  SeparaRutaMOVER(ORIGEN);


    TABLAINODOS destino;
    destino =  SeparaRutaDestinoMOVER(DESTINO);

    if(padre.i_type != '\0' && destino.i_type != '\0' && padre.i_type == '0'){
        BLOQUECARPETA bloque = getBloqueCarpeta(sb.s_block_start + (padre.i_block[0] * sizeof(BLOQUECARPETA)));

        bool ya = false;
        for (int var = 0; var < 15; var++) {

            BLOQUECARPETA blo = getBloqueCarpeta(sb.s_block_start + (padre.i_block[var] * sizeof(BLOQUECARPETA)));
            for (int var1 = 0; var1 < 4; var1++) {

                char Nombre[20];
                memset(&Nombre,'\0',sizeof(Nombre));
                strcpy(Nombre,NOMBRE_carptea_oarchivo);

                if(strcmp(blo.Content[var1].b_name,NOMBRE_carptea_oarchivo) == 0){
                    //actualizamos la referencia del padre del inodo que se esta moviendo
                    //actualizamos este apuntador a -1
                    NumeroInodoMover = blo.Content[var1].b_inodo;
                    memset(&blo.Content[var1].b_name,'\0',sizeof(blo.Content[var1].b_name));
                    blo.Content[var1].b_inodo = -1;

                    InodoaMover = getInodo(UsuarioLog.Path,sb.s_inode_start + (NumeroInodoMover * sizeof(TABLAINODOS)));

                    InsertarBloque_Carpeta(UsuarioLog.Path,sb.s_block_start + (padre.i_block[var] * sizeof(BLOQUECARPETA)), blo);
                    ya = true;
                    break;
                }
            }
            if(ya) break;
        }

        //lo insertamos en el nuevo inodo destino
        char Nombre1[20];
        memset(&Nombre1,'\0',sizeof(Nombre1));
        strcpy(Nombre1,NOMBRE_carptea_oarchivo);

        BLOQUECARPETA NuevoPadre = getBloqueCarpeta(sb.s_block_start + (destino.i_block[0]*sizeof(BLOQUECARPETA)));
        int NumeroInodoNUevoPadre = NuevoPadre.Content[0].b_inodo;          //en la posicion 0 del bloque 0 siempre apunta al inodo actual

        CrearCarpetaMover(destino,NumeroInodoMover,NOMBRE_carptea_oarchivo);

        //actualizamos la referencia del hijo que se esta moviendo
        if(InodoaMover.i_type == '0'){
            BLOQUECARPETA car = getBloqueCarpeta(sb.s_block_start + (InodoaMover.i_block[0] * sizeof(BLOQUECARPETA)));
            car.Content[1].b_inodo = NumeroInodoNUevoPadre;
            strcpy(car.Content[1].b_name,NuevoPadre.Content[0].b_name);

            InsertarBloque_Carpeta(UsuarioLog.Path,sb.s_block_start +(InodoaMover.i_block[0] * sizeof(BLOQUECARPETA)),car);
        }else{
            printf("Se esta moviendo un txt\n");
        }

    }else{
        printf("Rutas Especificadas no Existen.\n");
    }
}

TABLAINODOS SeparaRutaMOVER(char RUTA[250]){

    TABLAINODOS aux;

    SUPERBLOQUE sb = getSB(UsuarioLog.Path,UsuarioLog.StartParticion);
    bool para = false;

    TABLAINODOS padre;      //empezaremos con la raiz. SIEMPRE
    padre = getInodo(UsuarioLog.Path,sb.s_inode_start);

    char Pataux[250];
    memset(&Pataux,'\0', sizeof(Pataux));

    char Pataux1[250];
    memset(&Pataux1,'\0', sizeof(Pataux1));

    int indice = 1;
    for (int i = 0; i < strlen(RUTA); i++) {

        if(RUTA[0] == '/' && i == 0){
            Pataux[0]= '/';
            padre = VerificacionExisteCarpeta(padre, Pataux);       //mandamos a verificar si existe la carpeta.
            memset(&Pataux,'\0',sizeof(Pataux));
            i++;
            indice = 0;
        }else{
            if(RUTA[0] != '/' && i == 0){
                Pataux[0] = '/';
            }
        }
        while ((RUTA[i] != '/' || i == 0) &&  i < 250){
            Pataux[indice] = RUTA[i];
            i++;
            indice++;

            if(RUTA[i] == '.'){
                Pataux[indice] = RUTA[i];
                i++;
                indice++;
                para = true;
            }
        }

        if(!para){
            //mandamos a crear bloque de tipo archivo.
            para = false;
        }else{
            //cuando venga un archivo ya capturamos su padre
            strcpy(NOMBRE_carptea_oarchivo,Pataux);
            return padre;
        }

        aux = padre;
        padre = VerificacionExisteCarpeta(padre,Pataux);

        if(padre.i_type == '\0'){
            printf("Carpeta %s no existe.\n",Pataux);
            return;
        }

        indice = 0;
        strcpy(Pataux1,Pataux);
        memset(&Pataux,'\0',sizeof(Pataux));
    }

    //si viene a este return significa que se desea mover una carpeta
    strcpy(NOMBRE_carptea_oarchivo,Pataux1);
    return aux;  //<-  sera el padre de la carpeta que deseo mover.
}

TABLAINODOS SeparaRutaDestinoMOVER(char RUTA[250]){

    TABLAINODOS aux;

    SUPERBLOQUE sb = getSB(UsuarioLog.Path,UsuarioLog.StartParticion);
    bool para = false;

    TABLAINODOS padre;      //empezaremos con la raiz. SIEMPRE
    padre = getInodo(UsuarioLog.Path,sb.s_inode_start);

    char Pataux[250];
    memset(&Pataux,'\0', sizeof(Pataux));

    char Pataux1[250];
    memset(&Pataux1,'\0', sizeof(Pataux1));

    int indice = 1;
    for (int i = 0; i < strlen(RUTA); i++) {

        if(RUTA[0] == '/' && i == 0){
            Pataux[0]= '/';
            padre = VerificacionExisteCarpeta(padre, Pataux);       //mandamos a verificar si existe la carpeta.
            memset(&Pataux,'\0',sizeof(Pataux));
            i++;
            indice = 0;
        }else{
            if(RUTA[0] != '/' && i == 0){
                Pataux[0] = '/';
            }
        }
        while ((RUTA[i] != '/' || i == 0) &&  i < 250){
            Pataux[indice] = RUTA[i];
            i++;
            indice++;

            if(RUTA[i] == '.'){
                Pataux[indice] = RUTA[i];
                i++;
                indice++;
                para = true;
            }
        }

        if(!para){
            //mandamos a crear bloque de tipo archivo.
            para = false;
        }else{
            //cuando venga un archivo ya capturamos su padre
            printf("Error en ruta de destino.\n");
            return padre;
        }

        aux = padre;
        padre = VerificacionExisteCarpeta(padre,Pataux);

        if(padre.i_type == '\0'){
            printf("Carpeta %s no existe.\n",Pataux);
            return;
        }

        indice = 0;
        strcpy(Pataux1,Pataux);
        memset(&Pataux,'\0',sizeof(Pataux));
    }

    //si viene a este return significa que se desea mover una carpeta
    return padre;  //<-  sera la carpeta destino
}

void CrearCarpetaMover(TABLAINODOS inodoPadre, int NumeroInodoHijo,char NombreInodoHijo[20]){

    SUPERBLOQUE sb = getSB(UsuarioLog.Path,UsuarioLog.StartParticion);
    int primeBlloqueLibre = getPrimerBloqueLibre(UsuarioLog.Path,sb.s_bm_block_start);
    BLOQUECARPETA father = getBloqueCarpeta(sb.s_block_start + (inodoPadre.i_block[0]*sizeof(BLOQUECARPETA)));


    for (int var = 0; var < 15; ++var) {

        if(inodoPadre.i_block[var] != -1){
            BLOQUECARPETA carpeta = getBloqueCarpeta(sb.s_block_start + (inodoPadre.i_block[var]*sizeof(BLOQUECARPETA)));

            if(carpeta.Content[0].b_inodo == -1){

                carpeta.Content[0].b_inodo = NumeroInodoHijo;
                memset(&carpeta.Content[0].b_name,'\0',sizeof(carpeta.Content[0].b_name));
                strcpy(carpeta.Content[0].b_name,NombreInodoHijo);
                InsertarBloque_Carpeta(UsuarioLog.Path,sb.s_block_start + (inodoPadre.i_block[var]*sizeof(BLOQUECARPETA)),carpeta);

                printf("Se ha movido la carpeta %s.\n",NombreInodoHijo);
                break;
            }
            else if(carpeta.Content[1].b_inodo == -1){

                carpeta.Content[1].b_inodo = NumeroInodoHijo;
                memset(&carpeta.Content[1].b_name,'\0',sizeof(carpeta.Content[1].b_name));
                strcpy(carpeta.Content[1].b_name,NombreInodoHijo);
                InsertarBloque_Carpeta(UsuarioLog.Path,sb.s_block_start + (inodoPadre.i_block[var]*sizeof(BLOQUECARPETA)),carpeta);

                printf("Se ha movido la carpeta %s.\n",NombreInodoHijo);
                break;
            }
            else if(carpeta.Content[2].b_inodo == -1){

                carpeta.Content[2].b_inodo = NumeroInodoHijo;
                memset(&carpeta.Content[2].b_name,'\0',sizeof(carpeta.Content[2].b_name));
                strcpy(carpeta.Content[2].b_name,NombreInodoHijo);
                InsertarBloque_Carpeta(UsuarioLog.Path,sb.s_block_start + (inodoPadre.i_block[var]*sizeof(BLOQUECARPETA)),carpeta);

                printf("Se ha movido la carpeta %s.\n",NombreInodoHijo);
                break;
            }
            else if(carpeta.Content[3].b_inodo == -1){


                carpeta.Content[3].b_inodo = NumeroInodoHijo;
                memset(&carpeta.Content[3].b_name,'\0',sizeof(carpeta.Content[3].b_name));
                strcpy(carpeta.Content[3].b_name,NombreInodoHijo);
                InsertarBloque_Carpeta(UsuarioLog.Path,sb.s_block_start + (inodoPadre.i_block[var]*sizeof(BLOQUECARPETA)),carpeta);

                printf("Se ha movido la carpeta %s.\n",NombreInodoHijo);
                break;
            }
        }else{      //significa que el apuntador es -1
            //insertamos el bloque del apuntador del inodo
            BLOQUECARPETA nuevo;

            CONTENTCARPETA nuevacarp;
            nuevacarp.b_inodo = NumeroInodoHijo;
            memset(&nuevacarp.b_name,'\0',sizeof(nuevacarp.b_name));
            strcpy(nuevacarp.b_name,NOMBRE_carptea_oarchivo);
            nuevo.Content[0] = nuevacarp;

            CONTENTCARPETA con00;
            con00.b_inodo = -1;
            memset(&con00.b_name,'\0',sizeof(con00.b_name));
            nuevo.Content[1] = con00;

            CONTENTCARPETA con11;
            con11.b_inodo = -1;
            memset(&con11.b_name,'\0',sizeof(con11.b_name));
            nuevo.Content[2] = con11;

            CONTENTCARPETA con22;
            con22.b_inodo = -1;
            memset(&con22.b_name,'\0',sizeof(con22.b_name));
            nuevo.Content[3] = con22;

            InsertarBloque_Carpeta(UsuarioLog.Path,sb.s_block_start + (primeBlloqueLibre * sizeof(BLOQUECARPETA)),nuevo);

            ActualizarBBLOQUE(UsuarioLog.Path,sb.s_bm_block_start,primeBlloqueLibre);
            //actualizamos el inodo tambien
            inodoPadre.i_block[var] = primeBlloqueLibre;
            InsertarInodo(UsuarioLog.Path,sb.s_inode_start + (father.Content[0].b_inodo * sizeof(TABLAINODOS)),inodoPadre);

            printf("Se ha movido la carpeta %s.\n",NombreInodoHijo);
            break;
        }
    }
}

char NOMBRE_REMOVER_carpeta_o_archivo[200];
void Remover(char coma[500]){

    memset(&NOMBRE_REMOVER_carpeta_o_archivo,'\0',sizeof(NOMBRE_REMOVER_carpeta_o_archivo));

    char RUTA[250];
    memset(&RUTA,'\0',sizeof(RUTA));

    int i = 0;
    while (coma[i] == ' ') {
        i++;
    }
    i = i+3;
    while(coma[i] == ' '){
        i++;
    }

    for ( i = i; i < 500; i++) {
        if(coma[i] == '\0'){
            break;
        }

        if ((coma[i] == 'p' || coma[i] == 'P')
                &&(coma[i+1] == 'a' || coma[i+1] == 'A')
                &&(coma[i+2] == 't' || coma[i+2] == 'T')
                &&(coma[i+3] == 'h' || coma[i+3] == 'H')){

            char *file = (char*)malloc(sizeof(file));
            file  = getPath(coma,i);


            int indice = 0;
            while (*file != NULL) {
                char letra = *file;
                RUTA[indice] = letra;
                indice++;
                file++;
            }

            printf("Ruta Remover: %s\n",RUTA);
            i = indc;

        }
    }
    //capturamos el padre del objetivo a elimnar.
    SUPERBLOQUE sb;
    sb = getSB(UsuarioLog.Path,UsuarioLog.StartParticion);

    TABLAINODOS padre_objetivo_eliminar;
    padre_objetivo_eliminar = SeparaRutaREMOVER(RUTA);

    //ahora mando a verificar si tiene mas hijos

    /* inodos*/
    if(padre_objetivo_eliminar.i_perm != 0){

        for (int var = 0; var < 15; var++) {

            if(padre_objetivo_eliminar.i_block[var] != -1){

                BLOQUECARPETA blo = getBloqueCarpeta(sb.s_block_start + (padre_objetivo_eliminar.i_block[var] * sizeof(BLOQUECARPETA)));

                for (int var1 = 0; var1 < 4; var1++) {

                    char Nombre[20];
                    memset(&Nombre,'\0',sizeof(Nombre));
                    strcpy(Nombre,NOMBRE_REMOVER_carpeta_o_archivo);

                    if(strcmp(blo.Content[var1].b_name,NOMBRE_REMOVER_carpeta_o_archivo) == 0){
                        TABLAINODOS InodoAEliminar;
                        InodoAEliminar = getInodo(UsuarioLog.Path,sb.s_inode_start + (blo.Content[var1].b_inodo * sizeof(TABLAINODOS)));
                        EliminarTodosHijos(InodoAEliminar);


                        //eliminamos inodo.
                        char nulosInodo[sizeof(TABLAINODOS)];
                        memset(&nulosInodo,'\0',sizeof(nulosInodo));
                        InsertarNUlosI(UsuarioLog.Path,nulosInodo,sb.s_inode_start + (blo.Content[var1].b_inodo * sizeof(TABLAINODOS)));
                        ActualizarBINODOREMOVE(UsuarioLog.Path,sb.s_bm_inode_start,blo.Content[var1].b_inodo);


                        blo.Content[var1].b_inodo = -1;
                        memset(&blo.Content[var1].b_name,'\0',sizeof(blo.Content[var1].b_name));
                        InsertarBloque_Carpeta(UsuarioLog.Path,sb.s_block_start + (padre_objetivo_eliminar.i_block[var] * sizeof(BLOQUEARCHIVO)),blo);
                        return;
                    }
                }
            }
        }
    }else{
        //printf("");
    }
}

void EliminarTodosHijos(TABLAINODOS InodoActual){

    SUPERBLOQUE sb;
    sb = getSB(UsuarioLog.Path,UsuarioLog.StartParticion);

    if(InodoActual.i_type == '0'){
        for (int var = 0; var < 15; var++) {

            if(InodoActual.i_block[var] != -1){

                if(InodoActual.i_type == '0'){

                    BLOQUECARPETA bloque;
                    bloque = getBloqueCarpeta(sb.s_block_start + (InodoActual.i_block[var] * sizeof(BLOQUECARPETA)));

                    int var1 = 0;
                    if(var == 0){
                        var1 = var1+2;
                    }

                    for (var1 = var1; var1 < 4; var1++) {

                        if(bloque.Content[var1].b_inodo != -1){

                            TABLAINODOS inodo;
                            inodo = getInodo(UsuarioLog.Path,sb.s_inode_start + (bloque.Content[var1].b_inodo * sizeof(TABLAINODOS)));
                            EliminarTodosHijos(inodo);

                            //eliminamos inodo.
                            char nulosInodo[sizeof(TABLAINODOS)];
                            memset(&nulosInodo,'\0',sizeof(nulosInodo));

                            InsertarNUlosI(UsuarioLog.Path,nulosInodo,sb.s_inode_start + (bloque.Content[var1].b_inodo * sizeof(TABLAINODOS)));

                            ActualizarBINODOREMOVE(UsuarioLog.Path,sb.s_bm_inode_start,bloque.Content[var1].b_inodo);
                        }
                    }
                    //eliminamos bloque
                    char nulosBloque[sizeof(BLOQUEARCHIVO)];
                    memset(&nulosBloque,'\0',sizeof(nulosBloque));

                    InsertarNUlosB(UsuarioLog.Path,nulosBloque,sb.s_block_start + (InodoActual.i_block[var] * sizeof(BLOQUECARPETA)));

                    ActualizarBBLOQUEREMOVE(UsuarioLog.Path,sb.s_bm_block_start,InodoActual.i_block[var]);
                }
            }
        }
    }else{
        //inodo tipo archivo.
        for (int var2 = 0; var2 < 15; var2++) {
            if(InodoActual.i_block[var2] != -1){

                char nulosBloque[sizeof(BLOQUEARCHIVO)];
                memset(&nulosBloque,'\0',sizeof(nulosBloque));

                InsertarNUlosB(UsuarioLog.Path,nulosBloque,sb.s_block_start + (InodoActual.i_block[var2] * sizeof(BLOQUECARPETA)));
                ActualizarBBLOQUEREMOVE(UsuarioLog.Path,sb.s_bm_block_start,InodoActual.i_block[var2]);
            }
        }
    }

}

void InsertarNUlosI(char PathDisco[250],char nulosInodos[sizeof(TABLAINODOS)],int Inicio){

    //INSERTARA ARREGLOS CON NULOS DE TAMAÑO DE TABLAINODOS PARA LIMPIAR INODO POR INODO
    FILE *Archivo;
    Archivo = fopen(PathDisco,"rb+");
    if(Archivo!=NULL)
    {
        //escribimos el bitmap de inodos
        fseek(Archivo,Inicio,SEEK_SET);
        fwrite(&nulosInodos,sizeof(nulosInodos),1,Archivo);

        fclose(Archivo);
        fflush(Archivo);
    }

}

void InsertarNUlosB(char PathDisco[250],char nulosBloques[sizeof(BLOQUEARCHIVO)],int Inicio){

    FILE *Archivo;
    Archivo = fopen(PathDisco,"rb+");
    if(Archivo!=NULL)
    {
        //escribimos el bitmap de inodos
        fseek(Archivo,Inicio,SEEK_SET);
        fwrite(&nulosBloques,sizeof(nulosBloques),1,Archivo);

        fclose(Archivo);
        fflush(Archivo);
    }

}

TABLAINODOS SeparaRutaREMOVER(char RUTA[250]){

    TABLAINODOS aux;

    SUPERBLOQUE sb = getSB(UsuarioLog.Path,UsuarioLog.StartParticion);
    bool para = false;

    TABLAINODOS padre;      //empezaremos con la raiz. SIEMPRE
    padre = getInodo(UsuarioLog.Path,sb.s_inode_start);

    char Pataux[250];
    memset(&Pataux,'\0', sizeof(Pataux));

    char Pataux1[250];
    memset(&Pataux1,'\0', sizeof(Pataux1));

    int indice = 1;
    for (int i = 0; i < strlen(RUTA); i++) {

        if(RUTA[0] == '/' && i == 0){
            Pataux[0]= '/';
            padre = VerificacionExisteCarpeta(padre, Pataux);       //mandamos a verificar si existe la carpeta.
            memset(&Pataux,'\0',sizeof(Pataux));
            i++;
            indice = 0;
        }else{
            if(RUTA[0] != '/' && i == 0){
                Pataux[0] = '/';
            }
        }
        while ((RUTA[i] != '/' || i == 0) &&  i < 250){
            Pataux[indice] = RUTA[i];
            i++;
            indice++;

            if(RUTA[i] == '.'){
                Pataux[indice] = RUTA[i];
                i++;
                indice++;
                para = true;
            }
        }

        if(!para){
            //mandamos a crear bloque de tipo archivo.
            para = false;
        }else{
            //cuando venga un archivo ya capturamos su padre
            strcpy(NOMBRE_REMOVER_carpeta_o_archivo,Pataux);
            return padre;
        }

        aux = padre;
        padre = VerificacionExisteCarpeta(padre,Pataux);

        if(padre.i_type == '\0'){
            printf("Carpeta %s no existe.\n",Pataux);
            TABLAINODOS aux22;
            memset(&aux22,0,sizeof(aux22));
            return aux22;
        }

        indice = 0;
        strcpy(Pataux1,Pataux);
        memset(&Pataux,'\0',sizeof(Pataux));
    }

    //si viene a este return significa que se desea mover una carpeta
    strcpy(NOMBRE_REMOVER_carpeta_o_archivo,Pataux1);
    return aux;  //<-  sera el padre de la carpeta que deseo mover.
}

int DespejandoNEXT3(int TamaoParticion){
    float  N = (float)(TamaoParticion-sizeof(SUPERBLOQUE)) / (float)(1 + 3 + sizeof(TABLAINODOS) + (3*sizeof(BLOQUEARCHIVO)) + sizeof(JOURNALING));

    int NumeroEstructuras = (int)(floor(N));

    return NumeroEstructuras;
}

void Convert(char comando[500]){

    if(UsuarioLog.Tipo == 2){

        int i = 0;
        bool Error = false;
        while (comando[i] == ' ') {
            i++;
        }
        i = i+7;        //convert
        while(comando[i] == ' '){
            i++;
        }

        char Id[8];
        memset(&Id,'\0',sizeof(Id));

        for (i = i; i < 500; i++) {

            if ((comando[i] == 'i' || comando[i] == 'I')
                    &&(comando[i+1] == 'd' || comando[i+1] == 'D')){

                char id[15];
                memset(id,'\0',sizeof(id));
                id[0] = comando[i];
                id[1] = comando[i+1];
                id[2] = comando[i+2];
                if(strcasecmp(id,"id=") == 0){
                    i = i + 3;
                    while(comando[i] == ' '){
                        i++;
                    }
                    //inicializamos el arreglo en nulo
                    memset(Id,'\0', sizeof(Id));

                    int indice2 = 0;
                    while(comando[i] != ' ' && comando[i] != '\n'){
                        Id[indice2] = comando[i];
                        indice2++;
                        if(i < strlen(comando)){
                            i++;
                        }else{
                            break;
                        }
                    }
                    printf("Id Convert:  %s\n",Id);
                }else{
                    printf("Error de Sintaxis en Id=.\n");
                    return;     //paramos todo el metodo de una vez.
                }
            }
            else if(comando[i] == '#'){
                break;
            }else{
                if(comando[i-1] == '\n'){
                    break;
                }
                else if(comando[i] == '\0'){
                    break;
                }
                else if(comando[i] != '-' && comando[i] != ' ' && comando[i] != '\n' && comando[i] != '\r'){
                    printf("Error en los parametros del comando Mkfs.\n");
                    return;
                }
            }
        }

        //empezamos acciones de mkfs.
        MONTADURAS *aux = primero;
        char NombreParticion[20];
        char PathDisco[250];
        bool Siestamontada = false;

        memset(&NombreParticion,'\0',sizeof(NombreParticion));
        memset(&PathDisco,'\0',sizeof(PathDisco));

        while(aux != NULL){
            if(strcasecmp(aux->Id, Id) == 0){
                strcpy(NombreParticion,aux->Name);
                strcpy(PathDisco,aux->Path);
                aux->tipoF = 3;
                UsuarioLog.Tipo = 3;
                Siestamontada = true;
                break;
            }else{
                aux = aux->siguiente;
            }
        }
        if(Siestamontada){

            MBR mbr = getMBR(PathDisco);
            int TamaoParticion = 0;
            int StartParticion = 0;
            if(strcasecmp(mbr.mbr_partition_1.part_name, NombreParticion) == 0){
                TamaoParticion = mbr.mbr_partition_1.part_size;
                StartParticion = mbr.mbr_partition_1.part_start;
            }
            else if(strcasecmp(mbr.mbr_partition_2.part_name, NombreParticion) == 0){
                TamaoParticion = mbr.mbr_partition_2.part_size;
                StartParticion = mbr.mbr_partition_2.part_start;
            }
            else if(strcasecmp(mbr.mbr_partition_3.part_name, NombreParticion) == 0){
                TamaoParticion = mbr.mbr_partition_3.part_size;
                StartParticion = mbr.mbr_partition_3.part_start;
            }
            else if(strcasecmp(mbr.mbr_partition_4.part_name, NombreParticion) == 0){
                TamaoParticion = mbr.mbr_partition_4.part_size;
                StartParticion = mbr.mbr_partition_4.part_start;
            }

            //el super bloque de el sistema ext2 a convertir
            SUPERBLOQUE sb2;
            sb2 = getSB(UsuarioLog.Path,UsuarioLog.StartParticion);

            int NumeroStruct = DespejandoNEXT3(TamaoParticion);
            NumeroStructuras = NumeroStruct;    //hacemos global el numeor de estructuras que se estaran manejando

            printf("Particion: %s Tamaño: %d\n",NombreParticion,TamaoParticion);

            SUPERBLOQUE sb3;
            sb3.s_filesystem_type = 3;
            sb3.s_inodes_count = NumeroStruct;
            sb3.s_blocks_count = 3*NumeroStruct;
            sb3.s_free_blocks_count = 3*NumeroStruct;
            sb3.s_free_inodes_count = NumeroStruct;
            sb3.s_mtime = sb2.s_mtime;
            sb3.s_umtime = sb2.s_umtime;
            sb3.s_mnt_count = sb3.s_mnt_count++;
            sb3.s_magic = 201503911;  //averiguar que es esto
            sb3.s_inode_size = sizeof(TABLAINODOS);
            sb3.s_block_size = sizeof(BLOQUEAPUNTADORES);  //no importa cual bloque ponga ya que todos pesan 64 bits
            sb3.s_first_ino = sb2.s_first_ino;
            sb3.s_first_blo = sb2.s_first_blo;
            sb3.s_bm_inode_start = StartParticion + (NumeroStruct*sizeof(JOURNALING)) + sizeof(SUPERBLOQUE);
            sb3.s_bm_block_start = StartParticion + (NumeroStruct*sizeof(JOURNALING)) + sizeof(SUPERBLOQUE) + NumeroStruct;      //numerostruct = numero de inodos que tendra mi sistema de archivos.
            sb3.s_inode_start = StartParticion + (NumeroStruct*sizeof(JOURNALING)) + sizeof(SUPERBLOQUE) + NumeroStruct + (3*NumeroStruct);
            sb3.s_block_start = StartParticion + (NumeroStruct*sizeof(JOURNALING)) + sizeof(SUPERBLOQUE) + NumeroStruct + (3*NumeroStruct) +(NumeroStruct*sizeof(TABLAINODOS));

            /*          HACEMOS UNA COPIA DEL DISCO         */
            FILE *copia;                                                                                       //Creamos el disco con los parametros indicados
            copia = fopen ("/home/osuna/Escritorio/CopiaEXT2.disk", "wb+");

            for(int r = 0; r < mbr.mbr_tamao; r++){                                                               //llenamos el archivo con nulos-> se debe cambiar a binario
                fprintf(copia, "%c", 0);
            }
            fflush(copia);
            fclose(copia);

            FILE *original,*copia1;
            int  data1 = 0;
            original = fopen(PathDisco,"rb");
            copia1 = fopen("/home/osuna/Escritorio/CopiaEXT2.disk","wb+");

            while( !feof(original) ){
                data1 = fgetc(original);
                fputc(data1, copia1);
            }
            close(original);
            close(copia1);

            /*---- CAPTURAMOS Y ESCRIBIMOS LOS BITMAP DE INODO DEL SISTEMA EXT2 -----*/
            char inodes[NumeroStructuras];
            memset(&inodes,'0',sizeof(inodes));

            char bloques[3*NumeroStructuras];
            memset(&bloques,'0',sizeof(bloques));

            int n = DespejandoN(mbr.mbr_tamao);         //NUMERO DE ESTURUCTURAS DEL SISTEMA EXT2
            /////////////////////////////////////
            FILE *apun;
            char ino[n];
            memset(&ino,'0',sizeof(ino));
            apun = fopen("/home/osuna/Escritorio/CopiaEXT2.disk","rb");
            if (apun != NULL){
                fseek(apun,sb2.s_bm_inode_start,SEEK_SET);
                fread(&ino,sizeof(ino),1,apun);

                fclose(apun);
                fflush(apun);

            }else{
                memset(&ino,'0',ino);
            }
            ////////////////////////////////////

            /////////////////////////////////////
            FILE *apun1;
            char blo[3*n];
            memset(&blo,'0',sizeof(blo));
            apun1 = fopen("/home/osuna/Escritorio/CopiaEXT2.disk","rb");
            if (apun1 != NULL){

                fseek(apun1,sb2.s_bm_block_start,SEEK_SET);
                fread(&blo,sizeof(blo),1,apun1);

                fclose(apun1);
                fflush(apun1);

            }else{
                memset(&blo,'0',blo);
            }
            ////////////////////////////////////

            strcpy(inodes,ino);
            strcpy(bloques,blo);

            //mandamos a setear el super bloque.
            setSB(sb3,PathDisco,StartParticion);

            //FORMATEAMOS EL DISCO
            FormateoEXT3(NombreParticion);

            setBINODO(UsuarioLog.Path,sb3.s_bm_inode_start,inodes);
            setBBLOQUE(UsuarioLog.Path,sb3.s_bm_block_start,bloques);

            /*---- CAPTURAMOS Y ESCRIBIMOS LOS INODOS DEL SISTEMA EXT2 -----*/

            TABLAINODOS RaizExt2;
            RaizExt2 = getInodo("/home/osuna/Escritorio/CopiaEXT2.disk",sb2.s_inode_start);

            //para saber que numero de inodo es.
            BLOQUECARPETA Icarpeta;
            FILE *ar2;
            ar2 = fopen("/home/osuna/Escritorio/CopiaEXT2.disk","rb");
            if (ar2 != NULL){
                int Posicion = sb2.s_block_start + (RaizExt2.i_block[0] * sizeof(BLOQUEARCHIVO));
                fseek(ar2,Posicion,SEEK_SET);
                fread(&Icarpeta,sizeof(BLOQUECARPETA),1,ar2);
                fclose(ar2);
                fflush(ar2);

            }else{
                printf("No se pudo capturar bloque en convert.\n");
                return;
            }
            printf("\n");
            CorrerSistemaEXT2(RaizExt2,sb2,sb3,Icarpeta.Content[0].b_inodo);
            /*---- CAPTURAMOS Y ESCRIBIMOS LOS BLOQUES DEL SISTEMA EXT2 -----*/
        }else{
            printf("Erro al tratar de convertir a EXT3 la particion porque no se encuentra montada.\n");
        }
    }else{
        printf("Error Esta particion no se puede convertir a EXT3\n");
    }
}

void FormateoEXT3(char NombreParticion[20]){

    MBR mbr =getMBR(UsuarioLog.Path);
    SUPERBLOQUE sb = getSB("/home/osuna/Escritorio/CopiaEXT2.disk",UsuarioLog.StartParticion);

    int n = 0;
    if(strcmp(mbr.mbr_partition_1.part_name,NombreParticion)==0){
        n = DespejandoN(mbr.mbr_partition_1.part_size);
    }
    else if(strcmp(mbr.mbr_partition_2.part_name,NombreParticion)==0){
        n = DespejandoN(mbr.mbr_partition_2.part_size);
    }
    else if(strcmp(mbr.mbr_partition_3.part_name,NombreParticion)==0){
        n = DespejandoN(mbr.mbr_partition_3.part_size);
    }
    else if(strcmp(mbr.mbr_partition_4.part_name,NombreParticion)==0){
        n = DespejandoN(mbr.mbr_partition_4.part_size);
    }


    /*      SETAMOS EL BITMAP DE INODOS EN NULOS        */
    char BitmapI[n];
    memset(&BitmapI,'\0',sizeof(BitmapI));
    setBINODO(UsuarioLog.Path,sb.s_bm_inode_start,BitmapI);

    /*      SETEAMOS EL BITMAP DE BLOQUES EN NULOS      */
    char BitmapB[3*n];
    memset(&BitmapB,'\0',sizeof(BitmapB));
    setBINODO(UsuarioLog.Path,sb.s_bm_block_start,BitmapB);

    /*      SETEAMOS UN ARREGLO DEL TAMA;O DE N*SIZEOF(INODO)       */
    char INODO[n*sizeof(TABLAINODOS)];
    memset(&INODO,'\0',sizeof(INODO));

    FILE *Archivo;
    Archivo = fopen(UsuarioLog.Path,"rb+");
    if(Archivo!=NULL)
    {
        fseek(Archivo,sb.s_inode_start,SEEK_SET);
        fwrite(&INODO,sizeof(INODO),1,Archivo);

        fclose(Archivo);
        fflush(Archivo);
    }
    else{
        printf("El disco Especificado No existe.(SB-MKFS)\n");
        return;
    }

    /*      SETEAMOS UNA ARREGLO DEL TAMA;O DE 3*N*SIZEOF(BLOQUE)       */

    int inicio = sb.s_block_start;          //el bucle se repite 3 veces porque mno se porque si ponia el arreglo del tama;o de 3 veces no lo aguantaba
    for (int var = 0; var < 3; ++var) {

        int numero  = n*sizeof(BLOQUEARCHIVO);
        char BLOQUE[numero];
        memset(&BLOQUE,'\0',sizeof(BLOQUE));

        FILE *Archivo1;
        Archivo1 = fopen(UsuarioLog.Path,"rb+");
        if(Archivo!=NULL)
        {
            fseek(Archivo1,inicio,SEEK_SET);
            fwrite(&BLOQUE,sizeof(BLOQUE),1,Archivo1);

            fclose(Archivo1);
            fflush(Archivo1);
            inicio = inicio + numero;
        }
        else{
            printf("El disco Especificado No existe.(FormateoEXT3)\n");
            return;
        }
    }
}

BLOQUEARCHIVO getBloqueArchivo(int Posicion,char Path[250]){
    BLOQUEARCHIVO carpeta;
    FILE *ar;
    ar = fopen(Path,"rb");
    if (ar != NULL){
        fseek(ar,Posicion,SEEK_SET);
        fread(&carpeta,sizeof(BLOQUEARCHIVO),1,ar);
        fclose(ar);
        fflush(ar);

    }else{
        printf("No sirvio chavo.(getbloque)\n");
    }

    return carpeta;
}

void CorrerSistemaEXT2(TABLAINODOS inodo,SUPERBLOQUE sb,SUPERBLOQUE sbnuevo,int numeroinodo){

    //insertamos el inodo
    InsertarInodo(UsuarioLog.Path,sbnuevo.s_inode_start + (numeroinodo * sizeof(TABLAINODOS)),inodo);

    /*        SI EL INODO ES DE TIPO CARPETA          */
    if(inodo.i_type == '0'){
        for (int bloque = 0; bloque < 15; ++bloque){
            if(inodo.i_block[bloque] != -1){

                BLOQUECARPETA carpeta;
                FILE *ar;
                ar = fopen("/home/osuna/Escritorio/CopiaEXT2.disk","rb");
                if (ar != NULL){
                    int Posicion = sb.s_block_start + (inodo.i_block[bloque] * sizeof(BLOQUEARCHIVO));
                    fseek(ar,Posicion,SEEK_SET);
                    fread(&carpeta,sizeof(BLOQUECARPETA),1,ar);
                    fclose(ar);
                    fflush(ar);

                }else{
                    printf("No se pudo capturar bloque en convert.\n");
                    return;
                }

                //insertamos bloque
                InsertarBloque_Carpeta(UsuarioLog.Path,sbnuevo.s_block_start + (inodo.i_block[bloque] * sizeof(BLOQUEARCHIVO)),carpeta);

                int pos = 0;
                if(bloque == 0 ){
                    pos = pos + 2;
                }
                for (pos = pos; pos < 4; ++pos) {
                    if(carpeta.Content[pos].b_inodo != -1){

                        FILE *ar1;
                        TABLAINODOS inodo;
                        ar1 = fopen("/home/osuna/Escritorio/CopiaEXT2.disk","rb");

                        if (ar != NULL){
                            int starInodo = sb.s_inode_start + (carpeta.Content[pos].b_inodo * sizeof(TABLAINODOS));
                            fseek(ar1,starInodo,SEEK_SET);
                            fread(&inodo,sizeof(inodo),1,ar1);
                            fclose(ar1);
                            fflush(ar1);

                        }else{
                            printf("No se pudo capturar inodo en convert.\n");
                            return;
                        }
                        CorrerSistemaEXT2(inodo,sb,sbnuevo,carpeta.Content[pos].b_inodo);
                    }
                }
            }
        }
    }
    /*          SI EL INODO ES DE TIPO ARCHIVO      */
    else if(inodo.i_type == '1'){

        for (int bloque = 0; bloque < 15; ++bloque){

            if(inodo.i_block[bloque] != -1){
                BLOQUEARCHIVO arch;
                arch = getBloqueArchivo(sb.s_block_start +(inodo.i_block[bloque] * sizeof(BLOQUEARCHIVO)),"/home/osuna/Escritorio/CopiaEXT2.disk");

                //insertamos bloque
                InsertarBloque_Archivo(UsuarioLog.Path,sbnuevo.s_block_start + (inodo.i_block[bloque] * sizeof(BLOQUEARCHIVO)),arch);
            }
        }
    }
}

void Recovery(char comando[500]){

    int i = 0;
    bool Error = false;
    while (comando[i] == ' ') {
        i++;
    }
    i = i+8;        //recovery
    while(comando[i] == ' '){
        i++;
    }

    char Id[8];
    memset(&Id,'\0',sizeof(Id));

    for (i = i; i < 500; i++) {

        if ((comando[i] == 'i' || comando[i] == 'I')
                &&(comando[i+1] == 'd' || comando[i+1] == 'D')){

            char id[15];
            memset(id,'\0',sizeof(id));
            id[0] = comando[i];
            id[1] = comando[i+1];
            id[2] = comando[i+2];
            if(strcasecmp(id,"id=") == 0){
                i = i + 3;
                while(comando[i] == ' '){
                    i++;
                }
                //inicializamos el arreglo en nulo
                memset(Id,'\0', sizeof(Id));

                int indice2 = 0;
                while(comando[i] != ' ' && comando[i] != '\n'){
                    Id[indice2] = comando[i];
                    indice2++;
                    if(i < strlen(comando)){
                        i++;
                    }else{
                        break;
                    }
                }
                printf("Id Recovery:  %s\n",Id);
            }else{
                printf("Error de Sintaxis en Id=.\n");
                return;     //paramos todo el metodo de una vez.
            }
        }
        else if(comando[i] == '#'){
            break;
        }else{
            if(comando[i-1] == '\n'){
                break;
            }
            else if(comando[i] == '\0'){
                break;
            }
            else if(comando[i] != '-' && comando[i] != ' ' && comando[i] != '\n' && comando[i] != '\r'){
                printf("Error en los parametros del comando Mkfs.\n");
                return;
            }
        }
    }

    //recovery
    //como se borro todo debemos crear denuevo el archivo de user. y la raiz

    MBR mbr = getMBR(UsuarioLog.Path);
    int TamaoParticion = 0;
    int StartParticion = 0;
    if(strcasecmp(mbr.mbr_partition_1.part_name, UsuarioLog.Name) == 0){
        TamaoParticion = mbr.mbr_partition_1.part_size;
        StartParticion = mbr.mbr_partition_1.part_start;
    }
    else if(strcasecmp(mbr.mbr_partition_2.part_name, UsuarioLog.Name) == 0){
        TamaoParticion = mbr.mbr_partition_2.part_size;
        StartParticion = mbr.mbr_partition_2.part_start;
    }
    else if(strcasecmp(mbr.mbr_partition_3.part_name, UsuarioLog.Name) == 0){
        TamaoParticion = mbr.mbr_partition_3.part_size;
        StartParticion = mbr.mbr_partition_3.part_start;
    }
    else if(strcasecmp(mbr.mbr_partition_4.part_name, UsuarioLog.Name) == 0){
        TamaoParticion = mbr.mbr_partition_4.part_size;
        StartParticion = mbr.mbr_partition_4.part_start;
    }

    int NumeroStruct = 0;
    NumeroStruct = DespejandoNEXT3(TamaoParticion);
    NumeroStructuras = NumeroStruct;

    SUPERBLOQUE sb;
    sb = getSB(UsuarioLog.Path,UsuarioLog.StartParticion);

    //------escribimos el bitmap de inodos ------------
    char inodos[NumeroStruct];
    char bloques[ 3 * NumeroStruct];

    for (int var = 0; var < NumeroStruct; ++var) {
        inodos[var] = '0';
    }
    for (int var = 0; var < 3*NumeroStruct; ++var) {
        bloques[var] = '0';
    }
    inodos[0] = '1';        //Inodo Raiz
    inodos[1] = '1';        //bloque asociado a raiz
    bloques[0] = '1';       //Inodo archivo users
    bloques[1] = '1';       //bloque asociado a archivo users

    /*----ESCRIBIMOS LOS BITMAPS-----*/
    setBINODO(UsuarioLog.Path,sb.s_bm_inode_start,inodos);
    setBBLOQUE(UsuarioLog.Path,sb.s_bm_block_start,bloques);

    /*----------CREACION DEL INODO RAIZ----------------*/
    TABLAINODOS Raiz;
    Raiz.i_uid = 1;
    Raiz.i_gid = 1;
    Raiz.i_size = 0;
    Raiz.i_atime = time(0);
    Raiz.i_ctime = time(0);
    Raiz.i_mtime = time(0);
    memset(&Raiz.i_block,-1,sizeof(Raiz.i_block));          //inicializamos todos sus punteros a -1
    Raiz.i_type = '0';      //tipo 0 signifca que es carpeta.
    Raiz.i_perm = 664;      //permisos de el archivo.
    //actualizacion de primer puntero para el bloque.
    Raiz.i_block[0] = 0;  // el primer auntador del primer inodo apuntara al bloque 0.

    /*---------CREACION DE BLOQUE ASOCIADO------*/
    BLOQUECARPETA bloqueasociado;

    CONTENTCARPETA actual;
    actual.b_inodo = 0;
    memset(&actual.b_name,'\0',sizeof(actual.b_name));
    strcpy(actual.b_name,"/");
    bloqueasociado.Content[0] = actual;

    CONTENTCARPETA padre;
    padre.b_inodo = 0;
    memset(&padre.b_name,'\0',sizeof(padre.b_name));
    strcpy(padre.b_name,"/");
    bloqueasociado.Content[1] = padre;

    CONTENTCARPETA user;
    user.b_inodo = 1;
    memset(&user.b_name,'\0',sizeof(user.b_name));
    strcpy(user.b_name,"User.txt");
    bloqueasociado.Content[2] = user;

    CONTENTCARPETA vacio;
    vacio.b_inodo = -1;
    memset(&vacio.b_name,'\0',sizeof(vacio.b_name));
    bloqueasociado.Content[3] = vacio;

    /*----------CREACION DEL INODO ARCHIVO USERS----------------*/
    TABLAINODOS archivouser;
    archivouser.i_uid = 1;
    archivouser.i_gid = 1;
    archivouser.i_size = 0;
    archivouser.i_atime = time(0);
    archivouser.i_ctime = time(0);
    archivouser.i_mtime = time(0);
    memset(&archivouser.i_block,-1,sizeof(archivouser.i_block));          //inicializamos todos sus punteros a -1
    archivouser.i_type = '1';      //tipo 1 signifca que es un archivo.
    archivouser.i_perm = 664;      //permisos de el archivo.
    //actualizacion de primer puntero para el bloque.
    archivouser.i_block[0] = 1;  // el primer auntador del primer inodo apuntara al bloque 1.

    //**********algo pasa aca y se pierde siempre el path del disco
    char aux[250];
    memset(&aux,'\0',sizeof(250));
    strcpy(aux,UsuarioLog.Path);

    /*----------CREACION DE BLOQUE ASOCICADO-----------------*/
    BLOQUEARCHIVO Archivo;
    char contenido[64];
    memset(&contenido,'\0',sizeof(contenido));
    strcpy(contenido,"1,G,root\n1,U,root,root,123\n");
    strcpy(Archivo.b_content,contenido);

    //***************volemos a asignarle el valor que habiamos guardado en aux.
    memset(&UsuarioLog.Path,'\0',sizeof(UsuarioLog.Path));
    strcpy(UsuarioLog.Path,aux);

    /*--------------MANDAMOS A ESCRIBIR LOS INODOS Y BLOQUES QUE UTILIZAMOS------------------*/

    InsertarInodo(UsuarioLog.Path,((0*sizeof(TABLAINODOS)) + sb.s_inode_start),Raiz);
    InsertarInodo(UsuarioLog.Path,((1*sizeof(TABLAINODOS)) + sb.s_inode_start),archivouser);
    InsertarBloque_Carpeta(UsuarioLog.Path,((0*sizeof(BLOQUEARCHIVO)) + sb.s_block_start),bloqueasociado);
    InsertarBloque_Archivo(UsuarioLog.Path,((1*sizeof(BLOQUEARCHIVO)) + sb.s_block_start),Archivo);

    //ahora recorremos todos nuestros journaling y mandamos a ejecutar todos sus comandos.

    //hago esto porque cuando en el journaling venia un logaut se perdia todo como el path y el start
    char pathAuxiliar[250];
    memset(&pathAuxiliar,'\0',sizeof(pathAuxiliar));
    int startParticionAuxiliar = 0;

    strcpy(pathAuxiliar,UsuarioLog.Path);
    startParticionAuxiliar = UsuarioLog.StartParticion;


    if(UsuarioLog.Tipo == 3){
        for (int var = 0; var < NumeroStructuras; ++var) {          //numerostructuras es la cantidad de journaling que hay
            strcpy(UsuarioLog.Path,aux);
            if(var >= PrimerJournalingLibre){
                break;
            }

            //ya entendi porque se enciclaba y es porque al mandar a aejecutar los journaling se seguia escribiendo mas journalings
            // y era una de nuca acabar....
            JOURNALING jour = getJOURNALING(pathAuxiliar, startParticionAuxiliar + sizeof(SUPERBLOQUE) + (var * sizeof(JOURNALING)));
            Bandera_Esto_En_Recovery = true;

            if(jour.Journal_Tipo_Operacion != 0){           //si es 0 signifca que es nulo
                char Contenido[500];
                memset(&Contenido,'\0',sizeof(Contenido));

                strcpy(Contenido,jour.Journal_Contenido);
                printf("Comando: %s\n",Contenido);

                Analisis(Contenido);

            }//fin if de jour nulo

        }//fin for
        Bandera_Esto_En_Recovery = false;
    }

}

char Nombre_carpeta_chmod[200];
TABLAINODOS SeparaRutaCHMOD(char RUTA[250]){
    memset(&Nombre_carpeta_chmod,'\0',sizeof(Nombre_carpeta_chmod));

    TABLAINODOS aux;
    SUPERBLOQUE sb = getSB(UsuarioLog.Path,UsuarioLog.StartParticion);
    bool para = false;

    TABLAINODOS padre;      //empezaremos con la raiz. SIEMPRE
    padre = getInodo(UsuarioLog.Path,sb.s_inode_start);

    char Pataux[250];
    memset(&Pataux,'\0', sizeof(Pataux));

    char Pataux1[250];
    memset(&Pataux1,'\0', sizeof(Pataux1));

    int indice = 1;
    for (int i = 0; i < strlen(RUTA); i++) {

        if(RUTA[0] == '/' && i == 0){
            Pataux[0]= '/';
            padre = VerificacionExisteCarpeta(padre, Pataux);       //mandamos a verificar si existe la carpeta.
            memset(&Pataux,'\0',sizeof(Pataux));
            i++;
            indice = 0;
        }else{
            if(RUTA[0] != '/' && i == 0){
                Pataux[0] = '/';
            }
        }
        while ((RUTA[i] != '/' || i == 0) &&  i < 250){
            Pataux[indice] = RUTA[i];
            i++;
            indice++;

            if(RUTA[i] == '.'){
                Pataux[indice] = RUTA[i];
                i++;
                indice++;
                para = true;
            }
        }
        if(!para){
            //mandamos a crear bloque de tipo archivo.
            para = false;
        }else{
            //cuando venga un archivo ya capturamos su padre
            strcpy(Nombre_carpeta_chmod,Pataux);
            return padre;
        }

        aux = padre;
        padre = VerificacionExisteCarpeta(padre,Pataux);

        if(padre.i_type == '\0'){
            printf("Carpeta %s no existe.\n",Pataux);
            return;
        }

        indice = 0;
        strcpy(Pataux1,Pataux);
        memset(&Pataux,'\0',sizeof(Pataux));
    }

    //si viene a este return significa que se desea mover una carpeta
    strcpy(Nombre_carpeta_chmod,Pataux1);
    return aux; // sera el padre de la carpeta que deseo mover.
}

void Chmod(char coma[500]){

    char RUTA[250];
    memset(&RUTA,'\0',sizeof(RUTA));
    bool ParaR = false;
    int ugo = 0;

    int i = 0;
    while (coma[i] == ' ') {
        i++;
    }
    i = i+5;        //chmod
    while(coma[i] == ' '){
        i++;
    }

    for ( i = i; i < 500; i++) {
        if(coma[i] == '\0'){
            break;
        }

        if ((coma[i] == 'p' || coma[i] == 'P')
                &&(coma[i+1] == 'a' || coma[i+1] == 'A')
                &&(coma[i+2] == 't' || coma[i+2] == 'T')
                &&(coma[i+3] == 'h' || coma[i+3] == 'H')){

            char *file = (char*)malloc(sizeof(file));
            file  = getPath(coma,i);


            int indice = 0;
            while (*file != NULL) {
                char letra = *file;
                RUTA[indice] = letra;
                indice++;
                file++;
            }

            printf("Path Chmod: %s\n",RUTA);
            i = indc;

        }
        else if ((coma[i] == 'u' || coma[i] == 'U')
                 &&(coma[i+1] == 'g' || coma[i+1] == 'G')
                 &&(coma[i+2] == 'o' || coma[i+2] == 'O')){

            ugo = getUgo(coma,i);

            printf("UGO Chmod: %d\n",ugo);
            i = indc;

        }else if((coma[i] == 'r' || coma[i] == 'R')
                 &&(coma[i-1] == '-')
                 &&((coma[i+1] == ' ') ||(coma[i+1] == '\n'))){
            //viene el parametro P lo que indica que todas las carpetas si no existen debe crearlas
            //cuando no viene las carpetas que no existen solo se notifica un error
            ParaR = true;

            //i = i +2;
        }else if(coma[i] == '#'){

            break;
        }else{
            if(coma[i-1] == '\n'){
                break;
            }
            else if(coma[i] == '\0'){
                break;
            }
            else if(coma[i] != '-' && coma[i] != ' ' && coma[i] != '\n' && coma[i] != '/'){
                printf("Error en los parametros del comando chmod.\n");
                return;
            }
        }
    }

    /* VERIFICAMOS SI VIENE P Y EMPEZAMOS A CREAR LAS CARPETAS*/
    if(ParaR){
        //si viene el parametro p creara todas las carpetas si no existen.
        ParametroR = true;
        //printf("R -> True\n");

    }else{
        // si no viene el parametro si no existe alguna carpeta debe detectar error.
        ParametroR = false;
        //printf("R -> False\n");
    }

    SUPERBLOQUE sb;
    sb = getSB(UsuarioLog.Path,UsuarioLog.StartParticion);

    TABLAINODOS inodo;
    inodo= SeparaRutaCHMOD(RUTA);               //capturara el padre del inodo a modificar

    TABLAINODOS Hijo;
    if(inodo.i_type == '\0'){
        printf("Carpeta no existe.\n");
        return;

    }
    Hijo = VerificacionExisteCarpeta(inodo,Nombre_carpeta_chmod);

    if(Hijo.i_type == '\0'){
        printf("Carpeta no existe.\n");
        return;
    }

    BLOQUECARPETA car = getBloqueCarpeta(sb.s_block_start + (Hijo.i_block[0] * sizeof(BLOQUECARPETA)));

    TienePermisos(Hijo,sb.s_inode_start + (car.Content[0].b_inodo * sizeof(TABLAINODOS)),ugo);

}

void TienePermisos(TABLAINODOS Inodo,int posisicion_Inodo,int ugo){

    USUARIOLOGUEADO usu;
    usu = UsuarioLog;
    if(UsuarioLog.IDUsuario == 1 || (Inodo.i_uid == UsuarioLog.IDUsuario)){          //es root el puede hacer lo que le plazca
        Inodo.i_perm = ugo;
        InsertarInodo(UsuarioLog.Path,posisicion_Inodo,Inodo);
    }else{
        printf("Accion Denegada No tiene suficientes permisos para modificar carpeta.\n");
    }
}

int main()
{
    printf("Size de Journaling: %d\n",sizeof(JOURNALING));

    printf("\t\t\t\t\tSistema de gestion de discos.\n\t\t\t\t   Juan Pablo Osuna de Leon -- 201503911\n");

    char comando[500];
    memset(&comando, '\0', sizeof(comando));      //limpieza de arreglo comando

    while (true) {
        memset(&comando,'\0',sizeof(comando));
        printf("\n@201503911 -> ");                            //limipieza de buffer
        fflush(stdin);
        fgets(comando,500,stdin);
        //scanf("%s",&comando);
        //strcat(comando,"\n");


        if(strcasecmp(comando, "exit\n") == 0){
            break;
        }
        else if(strcasecmp(comando, "\n") != 0){

        }
        //si quiero evitar algunos saltos de linea inexplicables meto esta llamaca en el case anterios
        //pero a la hora de solo dar enter en la consola no dira ningun mensaje de error
        //debera mostrar error que no se escribio nigun comando.
        Analisis(comando);
    }
    return 0;
}
