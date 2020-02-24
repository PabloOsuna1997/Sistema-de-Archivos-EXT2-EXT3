#include "analizador.h"
#include <cstring>
#include <string>
#include <iostream>
#include "verificarruta.h"
#include "discos.h"
#include <ctype.h>
#include "estructuras.h"
#include "reporte.h"


using namespace std;


int indc;       //indice actual en donde vamos en el comando.
MONTADURAS* primero = NULL;
MONTADURAS* ultimo = NULL;
int posicionabecedario = 0;
char Letras[26] = {'a','b','c','d','e','f','g','h','i','j','k','l','m',
                   'n','o','p','q','r','s','t','u','v','w','x','y','z'};

void Analisis(char comando[500])
{
    //elimino los espacios en blanco al inicio del comando.
    int i = 0;
    while (comando[i] == ' ') {
        i++;
    }

    if(comando[i] != '#'){      //SI ES DIFERENTE AL CARACTER QUE INDICA QUE ES UN COMENTARIO.
        char palabra[9];        //capturamos la primera palabra que aparezca
        memset(&palabra,'\0',sizeof(palabra));

        int j = 0;
        for(i; i < 500; i++){
            if(comando[i] != ' ' && comando[i] != '\0' && comando[i] != '\n'){
                palabra[j] = comando[i];
                j++;
            }else{
                break;
            }
        }//al finalizar de capturar la palabra analizaremos

        if(strcasecmp(palabra,"mkdisk") == 0){      //primera palabra que recibe es mkdisk

            cout << "comando ingresado mkdisk." << endl;
            ObtenerAtributo(comando,i-j+6);
            VerificacionAtributosMkdisk();

        }else if(strcasecmp(palabra,"rmdisk") == 0){

            cout << "comando ingresado rmdisk." << endl;
            ObtenerAtributo(comando,i-j+6);
            VerificacionAtributosRmdisk();

        }else if(strcasecmp(palabra,"fdisk") == 0){

            cout << "comando ingresado fdisk." << endl;
            ObtenerAtributo(comando,i-j+5);
            VerificacionAtributosFdisk();

        }else if(strcasecmp(palabra,"mount") == 0){

            cout << "comando ingresado mount." << endl;
            ObtenerAtributo(comando,i-j+5);
            VerificacionAtributosMount();

        }else if(strcasecmp(palabra,"unmount") == 0){

            cout << "comando ingresado unmount." << endl;
            ObtenerAtributo(comando,i-j+7);
            VerificacionAtributosUnmount();

        }else if(strcasecmp(palabra,"rep") == 0){

            cout << "comando ingresado rep." << endl;
            ObtenerAtributo(comando,i-j+3);
            VerificacionAtributosRep();

        }else if(strcasecmp(palabra,"exec") == 0){

            cout << "comando ingresado exec." << endl;
            ObtenerAtributo(comando,i-j+4);
            Exec();

        }else{

            cout << "Comando Incorrecto." << endl;
        }

    }else{
        for (int var = 0; var < 500; ++var) {
            cout << comando[var];
        }
    }
}

//variables globales
int size = 0;
char unit = '\0';
char path[250];
char type = '\0';
char fit = '\0';
char delete_[5];
char name[15];
int add= 0;
char id[15];

void ObtenerAtributo(char comando[500],int indice){

    //variables globales
    size = 0;
    unit = '\0';
    memset(&path,'\0',sizeof(path));
    type = '\0';
    fit = '\0';
    memset(&delete_,'\0',sizeof(delete_));
    memset(&name,'\0',sizeof(name));
    add= 0;
    memset(&id,'\0',sizeof(id));

    for (indice; indice < 500; indice++) {

        if(tolower(comando[indice-1]) == '-' &&
                tolower(comando[indice]) == 's' &&
                tolower(comando[indice+1]) == 'i' &&
                tolower(comando[indice+2]) == 'z' &&
                tolower(comando[indice+3]) == 'e' &&
                tolower(comando[indice+4]) == '='){

            //parametro size, adelantamos size=
            size = getSize(comando, indice+5);
            indice = indc;
            printf("Tamaño: %d\n",size);

        }else if(tolower(comando[indice-1]) == '-' &&
                 tolower(comando[indice]) == 'f' &&
                 tolower(comando[indice+1]) == 'i' &&
                 tolower(comando[indice+2]) == 't' &&
                 tolower(comando[indice+3]) == '='){
            //parametro fit, adelantamos fit=
            fit = getFit(comando,indice+4);
            indice = indc;
            printf("Ajuste: %cF\n",fit);

        }else if(tolower(comando[indice-1]) == '-' &&
                 tolower(comando[indice]) == 'u' &&
                 tolower(comando[indice+1]) == 'n' &&
                 tolower(comando[indice+2]) == 'i' &&
                 tolower(comando[indice+3]) == 't' &&
                 tolower(comando[indice+4]) == '='){

            //parametro unit, adelantamos unit=
            unit = getUnit(comando,indice+5);
            indice = indc;
            printf("Unidad: %c\n",unit);

        }else if(tolower(comando[indice-1]) == '-' &&
                 tolower(comando[indice]) == 'p' &&
                 tolower(comando[indice+1]) == 'a' &&
                 tolower(comando[indice+2]) == 't' &&
                 tolower(comando[indice+3]) == 'h' &&
                 tolower(comando[indice+4]) == '='){

            //parametro path, adelantamos path=
            char *ruta = (char*)malloc(sizeof(ruta));
            ruta = getPath(comando,indice+5);

            int indice2 = 0;
            while(*ruta != '\0'){
                char letra = *ruta;
                path[indice2] = letra;
                ruta++;
                indice2++;
            }
            cout << "Path: ";
            printf(path);
            cout << "\n";
            indice = indc;

        }else if(tolower(comando[indice-1])== '-'
                 && tolower(comando[indice]) == 't'
                 && tolower(comando[indice+1]) == 'y'
                 && tolower(comando[indice+2]) == 'p'
                 && tolower(comando[indice+3]) == 'e'
                 && tolower(comando[indice+4]) == '='){

            type = getType(comando,indice+5);
            indice = indc;
            printf("Type: %c \n",type);

        }else if(tolower(comando[indice-1]) == '-'
                 && tolower(comando[indice]) == 'n'
                 && tolower(comando[indice+1]) == 'a'
                 && tolower(comando[indice+2]) == 'm'
                 && tolower(comando[indice+3]) == 'e'
                 && tolower(comando[indice+4]) == '='){

            char *nombre = (char*)malloc(sizeof(nombre));
            nombre = getName(comando,indice+5);

            int indice2 = 0;
            memset(&name,'\0',sizeof(name));
            while(*nombre != '\0'){
                char letra = *nombre;
                name[indice2] = letra;
                nombre++;
                indice2++;
            }
            printf("Name: %s\n",name);
            indice = indc;
        }
        else if(tolower(comando[indice-1]) == '-'
                && tolower(comando[indice]) == 'd'
                && tolower(comando[indice+1]) == 'e'
                && tolower(comando[indice+2]) == 'l'
                && tolower(comando[indice+3]) == 'e'
                && tolower(comando[indice+4]) == 't'
                && tolower(comando[indice+5]) == 'e'
                && tolower(comando[indice+6]) == '='){

            char *deleteA = (char*)malloc(sizeof(delete_));
            deleteA = getDelete(comando,indice+7);
            int indice4 = 0;
            memset(&delete_,'\0',sizeof(delete_));
            while(*deleteA != '\0'){
                char letra = *deleteA;
                delete_[indice4] = letra;
                deleteA++;
                indice4++;
            }
            printf("Delete: %s\n",delete_);
            indice = indc;
        }else if(tolower(comando[indice-1]) == '-'
                 && tolower(comando[indice]) == 'a'
                 && tolower(comando[indice+1]) == 'd'
                 && tolower(comando[indice+2]) == 'd'
                 && tolower(comando[indice+3]) == '='){

            add = getAdd(comando,indice+4);
            printf("Add: %d\n",add);
            indice = indc;
            indc = 0;
        }else if (tolower(comando[indice-1]) == '-'
                  && tolower(comando[indice]) == 'i'
                  && tolower(comando[indice+1]) == 'd'
                  && tolower(comando[indice+2]) == '='){

            char *Id = (char*)malloc(sizeof(Id));
            Id = getId(comando,indice+3);
            int indice5 = 0;
            memset(&id,'\0',sizeof(id));

            while(*Id != '\0'){
                char letra = *Id;
                id[indice5] = letra;
                Id++;
                indice5++;
            }
            printf("Id: %s\n",id);
            indice = indc;

        }else if(comando[indice] == '#' ){
            //al momento de ser un numeral detectara comentario y terminara el ciclo
            break;
        }else{
            if(comando[indice] == '\0'){
                break;

            }else if(comando[indice] != '-' && comando[indice] != ' ' && comando[indice] != '\n' && comando[indice] != '\r'){
                printf("Error en los parametros del comando.\n");
                return;         //se sale del metodo ya que da error de parametros.
            }
        }
    }
}

bool ParametrosCorrectos(char  Metodo[10]){

    printf("size: %d ",size);
    printf("unit: %c ",unit);
    printf("path: %s ",path);
    printf("type: %c ",type);
    printf("fit: %c ",fit);
    printf("delete_: %s ",delete_);
    printf("name: %s ",name);
    printf("add: %d ",add);
    printf("id: %s ",id);
    cout << "\n"<<endl;

    if(strcasecmp(Metodo,"mkdisk") == 0){

        if(type != '\0' || strcasecmp(name,"\0") != 0 || strcasecmp(delete_,"\0") != 0
                || add != 0){
            return false;
        }

    }else if (strcasecmp(Metodo,"rmdisk") == 0){

        if(type != '\0' || strcasecmp(name,"\0") != 0 || strcasecmp(delete_,"\0") != 0
                || add != 0 || unit != '\0' || size != 0 || fit != '\0'){
            return false;
        }

    }else if (strcasecmp(Metodo,"fdisk") == 0){

    }else if (strcasecmp(Metodo,"mount") == 0){
        if(size != 0 || unit != '\0' || type != '\0' || fit != '\0'
                || strcasecmp(delete_,"\0") != 0 || add != 0){
            return false;
        }
    }

    return true;
}

void VerificacionAtributosMkdisk(){

    //validacion de parametros.
    if(ParametrosCorrectos("mkdisk")){
        if(size != 0 && strcasecmp(path,"\0") != 0){
            if(fit != '\0' && fit != 'B' && fit != 'F' && fit != 'W'){
                printf("Error en el paremtro fit.\n");
                return;
            }else{
                if(fit == '\0')fit = 'F';  //primer ajuste por defecto

                if(unit != '\0' && unit != 'K' && unit != 'M'){
                    printf("Error en el paremtro unit.\n");
                    return;
                }else{
                    if(unit == '\0')unit = 'M';  //Megabytes por defecto
                }
            }
            //MANDAMOS A CREAR EL DISCO -.-
            bool pathcorrecta = SepararRuta(path,false);    //mando false porque indico que esto en mkdisk
            if(pathcorrecta){
                Mkdisk(size,fit,unit,path);

                //ACCIONES RAID1
                char pathraid[250];
                memset(&pathraid,'\0',sizeof(pathraid));

                char *ruta = (char*)malloc(sizeof(ruta));
                ruta = rutaRaid(path);

                int indice2 = 0;
                while(*ruta != '\0'){
                    char letra = *ruta;
                    pathraid[indice2] = letra;
                    ruta++;
                    indice2++;
                }
                cout << "Path de RAID1: ";
                printf(pathraid);
                cout << "\n";

                Mkdisk(size,fit,unit,pathraid);         //mandamos a crear el raid1

            }else{
                printf("Ocurrieron Problemas al tratar de crear la path para crear el disco.\n");
            }
        }else{
            printf("Error en parametros obligatorios.\n");
        }
    }else{
        cout << "Parametros no reconocidos en comando mkdisk."<<endl;
    }
}

void VerificacionAtributosRmdisk(){

    if(ParametrosCorrectos("rmdisk")){
        if(strcasecmp(path,"\0") != 0){
            Rmdisk(path);

            //ACCIONES RAID1
            char pathraid[250];
            memset(&pathraid,'\0',sizeof(pathraid));

            char *ruta = (char*)malloc(sizeof(ruta));
            ruta = rutaRaid(path);

            int indice2 = 0;
            while(*ruta != '\0'){
                char letra = *ruta;
                pathraid[indice2] = letra;
                ruta++;
                indice2++;
            }
            cout << "Path de RAID1: ";
            printf(pathraid);
            cout << "\n";

            Rmdisk(pathraid);         //mandamos a eliminar el raid1
        }else{
            cout << "Parametro Path no ha sido indicado."<<endl;
        }
    }else{
        cout << "Parametros no reconocidos en comando rmdisk."<<endl;
    }
}

void VerificacionAtributosFdisk(){

    if(ParametrosCorrectos("fdisk")){

        if(strcasecmp(name,"\0") != 0 && strcasecmp(path,"\0") != 0){

            if(strcasecmp(delete_, "\0") != 0 || add != 0){      //verifico que si estos parametros son nulos de serlo se tratara
                // de creacion de una particion
                //cuando venga Add ignora los demas atributos si viene size o delete.(el primero que encuentre)
                if( add != 0){
                    if(unit == 'K'){
                        add = add * 1024;
                    }else if(unit == 'B'){
                        add = add;
                    }else if(unit == 'M'){
                        add = add * 1024 *1024;
                    }else{
                        printf("Error en el tipo de la dimension de Add.(Add Fdisk)\n");
                        return;
                    }
                    //printf("El valor de Add no es nulo.\n");
                    AddParticion(path,name,add);
                }
                else if(strcasecmp(delete_, "\0") != 0){

                    MBR mbr = getMBR(path);
                    if((strcasecmp(mbr.mbr_partition_1.part_name,name) == 0
                        ||strcasecmp(mbr.mbr_partition_2.part_name,name) == 0
                        ||strcasecmp(mbr.mbr_partition_3.part_name,name) == 0
                        ||strcasecmp(mbr.mbr_partition_4.part_name,name) == 0) && mbr.mbr_tamao != 0){

                        if(strcasecmp(delete_,"fast") == 0 || strcasecmp(delete_,"full") == 0){

                            printf("Esta seguro que desea eliminar %s del disco.(y/n)?",name);
                            char respuesta[3];
                            memset(&respuesta,'\0',sizeof(respuesta));

                            fgets(respuesta,3,stdin);
                            if(respuesta[0] == 'y' || respuesta[0] == 'Y'){
                                DeleteParticion(path,name);

                                //ACCIONES RAID1
                                char pathraid[250];
                                memset(&pathraid,'\0',sizeof(pathraid));

                                char *ruta = (char*)malloc(sizeof(ruta));
                                ruta = rutaRaid(path);

                                int indice2 = 0;
                                while(*ruta != '\0'){
                                    char letra = *ruta;
                                    pathraid[indice2] = letra;
                                    ruta++;
                                    indice2++;
                                }
                                cout << "Path de RAID1: ";
                                printf(pathraid);
                                cout << "\n";

                                DeleteParticion(pathraid,name);         //mandamos a ELIMINARN PARTICION el raid1
                            }else{
                                printf("Eliminacion de Particion abortada.\n");
                            }
                        }else{
                            printf("Error en el parametro Delete, atributo no valido.\n");
                        }
                    }else{
                        if(ExisteLogica(path,name)){
                            DeleteParticion(path,name);

                            //ACCIONES RAID1
                            char pathraid[250];
                            memset(&pathraid,'\0',sizeof(pathraid));

                            char *ruta = (char*)malloc(sizeof(ruta));
                            ruta = rutaRaid(path);

                            int indice2 = 0;
                            while(*ruta != '\0'){
                                char letra = *ruta;
                                pathraid[indice2] = letra;
                                ruta++;
                                indice2++;
                            }
                            cout << "Path de RAID1: ";
                            printf(pathraid);
                            cout << "\n";

                            DeleteParticion(pathraid,name);         //mandamos a ELIMINAR PARTICION el raid1

                        }else{
                            printf("El nombre de particion especificado no existe en el disco.\n");
                        }
                    }
                }
            }else{
                CrearParticionFDISK(size,unit,path,type,fit,name);           //mandamos a crear la particion.

                //ACCIONES RADI1
                char pathraid[250];
                memset(&pathraid,'\0',sizeof(pathraid));

                char *ruta = (char*)malloc(sizeof(ruta));
                ruta = rutaRaid(path);

                int indice2 = 0;
                while(*ruta != '\0'){
                    char letra = *ruta;
                    pathraid[indice2] = letra;
                    ruta++;
                    indice2++;
                }
                cout << "Path de RAID1: ";
                printf(pathraid);
                cout << "\n";

                CrearParticionFDISK(size,unit,pathraid,type,fit,name);          //mandamos a CREAR PARTICION el raid1

                printf("\n\nImprimiendo Particiones\n");
                Verparticiones(path);       //muestra el status de las particiones en el disco.
            }
        }else{
            printf("Error parametro fdisk Name y Path son obligatorios.\n");
        }
    }else{
        cout << "Parametros no reconocidos en comando fdisk."<<endl;
    }
}

void VerificacionAtributosMount(){

    if(ParametrosCorrectos("mount")){

        if(strcasecmp(name,"\0") != 0 && strcasecmp(path,"\0") != 0){
            //verificamos de que la particion no este montada si ya esta se mostrara mensaje que ya se encuentra montada con tal id
            bool yaexiste = false;
            char Id[8];
            memset(&Id,'\0',sizeof(Id));

            MONTADURAS *aux = primero;
            while (aux != NULL) {
                if(strcasecmp(aux->Path,path) == 0 && strcasecmp(aux->Name,name) == 0){
                    yaexiste = true;
                    strcat(Id,aux->Id);
                    break;
                }
                aux = aux->siguiente;
            }
            if(!yaexiste){
                MBR mbr;
                mbr = getMBR(path);
                if (mbr.mbr_tamao != 0){
                    if(strcasecmp(mbr.mbr_partition_1.part_name,name) == 0){
                        MontarParticion(path,name,mbr.mbr_partition_1.part_start);
                    }
                    else if(strcasecmp(mbr.mbr_partition_2.part_name,name) == 0){
                        MontarParticion(path,name,mbr.mbr_partition_2.part_start);
                    }
                    else if(strcasecmp(mbr.mbr_partition_3.part_name,name) == 0){

                        MontarParticion(path,name,mbr.mbr_partition_3.part_start);
                    }
                    else if(strcasecmp(mbr.mbr_partition_4.part_name,name) == 0){

                        MontarParticion(path,name,mbr.mbr_partition_4.part_start);
                    }else{
                        printf("Particion %s NO existe en el disco.\n",name);
                    }
                }else{
                    printf("El Disco Especificado en la path %s No existe.(getMBR)\n",path);
                }
            }else{
                printf("La Particion ya se encuentra montada con id %s\n",Id);
            }
        }else{
            cout << "Error Mount Nombre y Path son obligatorios."<<endl;
        }
    }else{
        cout << "Parametros no reconocidos en comando mount."<<endl;
    }
}

void VerificacionAtributosUnmount(){

    if(ParametrosCorrectos("unmount")){

        if(strcasecmp(id,"\0") != 0){

            if(primero != NULL){
                DesmontarParticion(id);
            }else {
                printf("No existen particiones montadas.\n");
                return;
            }
        }else{
            printf("Error Unmount Id obligatorio.\n");
            return;
        }
    }else{
        cout << "Parametros no reconocidos en comando Unmount."<<endl;
        return;
    }
}

void VerificacionAtributosRep(){

    char iDD[9];
    char N[10];
    memset(&N,'\0', sizeof(N));
    memset(&iDD,'\0', sizeof(iDD));
    strcpy(iDD,id);
    strcpy(N,name);

    if(strcasecmp(path,"\0") != 0 && strcasecmp(name,"\0") != 0 && strcasecmp(id,"\0") != 0){

        //printf("Id: %s\nPath: %s\nName: %s\n",iDD,Path,N,RUTA);
        //cout << "que mierda pasa " << endl;
        ReporteMBR(iDD,path,N,primero);

    }else{
        printf("Error Rep. Path, Nombre y Id son obligatorios.\n");
    }
}

void Exec()
{
    if(ParametrosCorrectos("exec")){
        if(strcasecmp(path,"\0") != 0){

            //ENVIAMOS A VERIFICAR LA RUTA ESPECIFICADA
            FILE *apun;

            apun = fopen(path,"rb");
            if (apun != NULL){
                fclose(apun);
                fflush(apun);

            }else{
                printf("No se encontro Script en la path indicada.\n");
                return;
            }

            bool pathcorrcta = false;
            pathcorrcta = SepararRuta(path,false);

            if(pathcorrcta){
                char linea[500];
                memset(&linea,'\0',sizeof(linea));
                FILE *fich;

                fich = fopen(path, "r");
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
                //return true;
            }else {
                printf("Path Incorrecta.\n");
            }
        }else{
            printf("Error exec, Path es obligatorio.\0");
        }
    }else{
        cout << "Parametros no reconocidos en comando exec."<<endl;
    }
}

/*------------METODOS GET----------------*/
int getSize(char comando[500],int i){
    int size;
    char numero[10];                 //almacenaremos el numero leido max 10 digitos
    memset(&numero,'\0', sizeof(numero));

    while(comando[i] == ' '){        //si viene spacios los omitimos.
        i++;
    }

    int indice = 0;
    while(comando[i] != ' ' && comando[i] != '\n'){         //capturaremos hasta encontrar espacio o salto de liea
        numero[indice] = comando[i];
        indice++;
        if(i < strlen(comando)){
            i++;
        }else{
            break;
        }
    }
    size = atoi(numero);
    indc = i;
    return size;
}

char getFit(char comando[500], int i)
{
    char fit;
    i = quitarEspacios(comando,i);
    fit = toupper(comando[i]);
    i++;
    indc = i;
    return fit;

}

char getUnit(char comando[500], int i){
    char unit;
    i = quitarEspacios(comando,i);
    unit = toupper(comando[i]);
    i++;
    indc = i;
    return unit;
}

char *getPath(char comando[500],int i){
    char *ruta = (char*)malloc(sizeof(ruta));
    char Path[250];
    memset(&Path,'\0', sizeof(Path));

    i = quitarEspacios(comando,i);
    int indiceP = 0;
    bool primeracomilla = false;            //primera comilla entrara a un estado donde acepte caracteres especiales
    //para las ruitas.
    while((comando[i] != ' ' && comando[i] != '\n') || primeracomilla == true){

        if(comando[i] == '\"'){//entro a estado de ruta
            if(primeracomilla == true){     //si es true significa que es la ultima comilla para cerrar
                primeracomilla = false;
                //Path[indiceP] = comando[i];
                indiceP++;
                break;
            }else{
                primeracomilla = true;
            }
        }else{
            Path[indiceP] = comando[i];
            indiceP++;
        }


        if(i < strlen(comando)){
            i++;
        }else{
            break;
        }
    }
    if(primeracomilla)memset(&Path,'\0',sizeof(Path));    //validacion por si no viene comilla de cierre

    ruta = Path;
    indc = i;
    return ruta;
}

char getType(char coma[500],int i){
    char Type = '\0';
    quitarEspacios(coma,i);
    Type = toupper(coma[i]);
    indc = i;
    return Type;
}

char *getName(char coma[500],int i){
    char *Nombre = (char*)malloc(sizeof(Nombre));
    char Name[50];  memset(&Name,'\0', sizeof(Name));
    i = quitarEspacios(coma,i);
    int indice2 = 0;
    while(coma[i] != ' ' && coma[i] != '\n'){
        Name[indice2] = coma[i];
        indice2++;
        if(i < strlen(coma)){
            i++;
        }else{
            break;
        }
    }
    Nombre = Name;
    indc = i;

    return Nombre;
}

char *getDelete(char coma[500],int i){
    char *DELETE = (char*)malloc(sizeof(DELETE));
    char Delete[5]; memset(&Delete,'\0', sizeof(Delete));

    i = quitarEspacios(coma,i);
    if(coma[i+1] == 'a' || coma[i+1] == 'A'){
        strcpy(Delete, "fast");
        i=i+4;
    }else if(coma[i+1] == 'u' || coma[i+1] == 'U' ) {
        strcpy(Delete, "full");
        i=i+4;
    }
    DELETE = Delete;
    indc = i;
    return DELETE;
}

int getAdd(char coma[500],int i){
    int Add = 0;
    char agregarunidades[10]; memset(&agregarunidades,'\0', sizeof(agregarunidades));

    i = quitarEspacios(coma,i);
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
    indc = i;
    return Add;
}

char *getId(char coma[500],int i){
    char *Nombre = (char*)malloc(sizeof(Nombre));
    char Id[15];
    memset(Id,'\0',sizeof(Id));
    i = quitarEspacios(coma,i);

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
    Nombre = Id;
    indc = i;
    return Nombre;
}

int quitarEspacios(char comando[500], int inicio)
{
    while(comando[inicio] == ' '){        //si viene spacios los omitimos.
        inicio++;
    }
    indc = inicio;
    return inicio;
}

char *rutaRaid(char path[250]){
    for(int i = 0; i < 250; i++)
    {
        if(path[i] == '.'){
            path[i] = '_';
            path[i+1] = 'r';
            path[i+2] = 'a';
            path[i+3] = '1';
            path[i+4] = '.';
            path[i+5] = 'd';
            path[i+6] = 'i';
            path[i+7] = 's';
            path[i+8] = 'k';
            break;
        }
    }

    char *PATH = (char*)malloc(sizeof(PATH));
    PATH = path;
    return PATH;
}

//montar y desmontaar particiones
void MontarParticion(char Path[250], char Name[100],int Starparticion){

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

void DesmontarParticion(char Id[7]){

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

