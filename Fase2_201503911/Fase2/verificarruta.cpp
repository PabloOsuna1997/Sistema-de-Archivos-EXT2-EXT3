#include "verificarruta.h"
#include <cstring>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <dirent.h>     //directorios
#include <sys/stat.h>

bool VerficacionPath(char Path[250],bool indicador_eliminacio_ndisco){
    char *ruta = Path;
    int indice=0;
    while(*ruta != '\0'){
        ruta++;
        indice++;
    }
    DIR *PathDestino;

    if((PathDestino=opendir(Path))==NULL ){      //si devuelve null la carpeta  no existe y no hay necesidad de crearla

        if(!indicador_eliminacio_ndisco){
            /*este indicador me servira para saber si se esta consultado la ruta para mkdisk la cual debe creala si no existe
             * o para el rmdisk que solo es consulta y devuelde un false de no existir*/

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

bool SepararRuta(char RUTA[250],bool indicador_eliminacio_ndisco){
    bool para = false;
    bool pathcorrcta = false;
    char Pataux[250]; memset(&Pataux,'\0', sizeof(Pataux));

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
            pathcorrcta = VerficacionPath(Pataux,indicador_eliminacio_ndisco);
            para = false;
        }else{
            break;
        }
    }
    return pathcorrcta;
}
