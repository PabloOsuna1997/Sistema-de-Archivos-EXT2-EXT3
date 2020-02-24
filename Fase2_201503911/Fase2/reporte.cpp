#include "reporte.h"
#include "estructuras.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <dirent.h>
#include "verificarruta.h"

void ReporteMBR(char Id[7],char Path[250],char Name[7],MONTADURAS *primero){

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
                do {

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

                    if(ebr.part_next != -1)
                    {
                        ebr = getEBR(ruta,ebr.part_next);
                    }else{
                        break;
                    }

                }while(true);
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
                    fprintf(dot,"<TD COLSPAN=\"100\"><FONT COLOR=\"red\">%s  EXTENDIDA </FONT><BR/>%0.2f %c  \n %0.2f M.</TD>\n",mbr.mbr_partition_1.part_name,porcentaje,'%',KB);
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
                    fprintf(dot,"<TD COLSPAN=\"100\"><FONT COLOR=\"red\">%s  EXTENDIDA </FONT><BR/>%0.2f %c  \n %0.2f KB.</TD>\n",mbr.mbr_partition_2.part_name,porcentaje,'%',KB);
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
                    fprintf(dot,"<TD COLSPAN=\"100\"><FONT COLOR=\"red\">%s  EXTENDIDA </FONT><BR/>%0.2f %c  \n %0.2f M.</TD>\n",mbr.mbr_partition_3.part_name,porcentaje,'%',KB);
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
                    fprintf(dot,"<TD COLSPAN=\"100\"><FONT COLOR=\"red\">%s  EXTENDIDA </FONT><BR/>%0.2f %c  \n %0.2f M.</TD>\n",mbr.mbr_partition_4.part_name,porcentaje,'%',KB);
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

                    do{
                        porcentaje = ebr.part_size;
                        float KB = porcentaje*1000;
                        porcentaje = ((float)porcentaje/(float)mbr.mbr_partition_1.part_size);
                        porcentaje = porcentaje*100;

                        if(strcasecmp(ebr.part_name,"EBR_INICIAL") == 0 && ebr.part_next == -1)
                        {
                            fprintf(dot,"<TD><FONT COLOR=\"red\">EBR</FONT><BR/>%0.2f %c  \n %0.2f M.</TD>\n",porcentaje,'%',KB);
                            break;
                        }else if(strcasecmp(ebr.part_name,"EBR_INICIAL") == 0 && ebr.part_next != -1)
                        {
                            porcentaje = ebr.part_size;
                            float KB = porcentaje*1000;
                            porcentaje = ((float)porcentaje/(float)mbr.mbr_partition_1.part_size);
                            porcentaje = porcentaje*100;
                            fprintf(dot,"<TD><FONT COLOR=\"red\">EBR_INICIAL</FONT><BR/>%0.2f %c  \n %0.2f M.</TD>\n",porcentaje,'%',KB);

                            porcentaje = ebr.part_next - (ebr.part_start + ebr.part_size);
                            KB = porcentaje*1000;
                            porcentaje = ((float)porcentaje/(float)mbr.mbr_partition_1.part_size);
                            porcentaje = porcentaje*100;

                            fprintf(dot,"<TD><FONT COLOR=\"red\">LIBRE</FONT><BR/>%0.2f %c  \n %0.2f M.</TD>\n",porcentaje,'%',KB);

                            ebr = getEBR(ruta,ebr.part_next);   //nos pasamos al ebr siguiente
                        }else{
                            if(ebr.part_next == -1)
                            {
                                fprintf(dot,"<TD><FONT COLOR=\"red\">EBR</FONT><BR/>%.2f %c  \n %0.2f M.</TD>\n",porcentaje,'%',KB);
                                fprintf(dot,"<TD><FONT COLOR=\"red\">%s</FONT><BR/>%.2f %c  \n %0.2f M.</TD>\n",ebr.part_name,porcentaje,'%',KB);
                                break;
                            }else{
                                fprintf(dot,"<TD><FONT COLOR=\"red\">EBR</FONT><BR/>%0.2f %c  \n %0.2f M.</TD>\n",porcentaje,'%',KB);
                                fprintf(dot,"<TD><FONT COLOR=\"red\">%s</FONT><BR/>%0.2f %c  \n %0.2f M.</TD>\n",ebr.part_name,porcentaje,'%',KB);
                                EBR tmp = ebr;
                                ebr = getEBR(ruta,ebr.part_next);   //nos pasamos al ebr siguiente

                                if((tmp.part_start + tmp.part_size) < ebr.part_start){

                                    porcentaje = ebr.part_start - (tmp.part_start + tmp.part_size);
                                    KB = porcentaje*1000;
                                    porcentaje = ((float)porcentaje/(float)mbr.mbr_partition_1.part_size);
                                    porcentaje = porcentaje*100;

                                    fprintf(dot,"<TD><FONT COLOR=\"red\">LIBRE</FONT><BR/>%0.2f %c  \n %0.2f M.</TD>\n",porcentaje,'%',KB);
                                }
                            }
                        }
                    }while(true);


                    if((ebr.part_start+ ebr.part_size) < (mbr.mbr_partition_1.part_start + mbr.mbr_partition_1.part_size)){

                        porcentaje = (mbr.mbr_partition_1.part_start + mbr.mbr_partition_1.part_size) -(ebr.part_start+ ebr.part_size);
                        float KB = porcentaje*1000;
                        porcentaje = ((float)porcentaje/(float)mbr.mbr_partition_1.part_size);
                        porcentaje = porcentaje*100;
                        fprintf(dot,"<TD ROWSPAN=\"3\"><FONT COLOR=\"red\">LIBRE</FONT><BR/>%0.2f %c  \n %0.2f M.</TD>\n",porcentaje,'%',KB);
                    }
                }else if((mbr.mbr_partition_2.part_type == 'e' || mbr.mbr_partition_2.part_type == 'E')){


                    do{
                        porcentaje = ebr.part_size;
                        float KB = porcentaje*1000;
                        porcentaje = ((float)porcentaje/(float)mbr.mbr_partition_2.part_size);
                        porcentaje = porcentaje*100;
                        if(strcasecmp(ebr.part_name,"EBR_INICIAL") == 0 && ebr.part_next == -1)
                        {
                            fprintf(dot,"<TD><FONT COLOR=\"red\">EBR</FONT><BR/>%0.2f %c  \n %0.2f M.</TD>\n",porcentaje,'%',KB);
                            break;
                        }
                        else if(strcasecmp(ebr.part_name,"EBR_INICIAL") == 0 && ebr.part_next != -1)
                        {
                            porcentaje = ebr.part_size;
                            float KB = porcentaje*1000;
                            porcentaje = ((float)porcentaje/(float)mbr.mbr_partition_2.part_size);
                            porcentaje = porcentaje*100;
                            fprintf(dot,"<TD><FONT COLOR=\"red\">EBR_INICIAL</FONT><BR/>%0.2f %c  \n %0.2f M.</TD>\n",porcentaje,'%',KB);

                            porcentaje = ebr.part_next - (ebr.part_start + ebr.part_size);
                            KB = porcentaje*1000;
                            porcentaje = ((float)porcentaje/(float)mbr.mbr_partition_2.part_size);
                            porcentaje = porcentaje*100;

                            fprintf(dot,"<TD><FONT COLOR=\"red\">LIBRE</FONT><BR/>%0.2f %c  \n %0.2f M.</TD>\n",porcentaje,'%',KB);

                            ebr = getEBR(ruta,ebr.part_next);   //nos pasamos al ebr siguiente
                        }else{
                            if(ebr.part_next == -1)
                            {
                                fprintf(dot,"<TD><FONT COLOR=\"red\">EBR</FONT><BR/>%.2f %c  \n %0.2f M.</TD>\n",porcentaje,'%',KB);
                                fprintf(dot,"<TD><FONT COLOR=\"red\">%s</FONT><BR/>%.2f %c  \n %0.2f M.</TD>\n",ebr.part_name,porcentaje,'%',KB);
                                break;
                            }else{
                                fprintf(dot,"<TD><FONT COLOR=\"red\">EBR</FONT><BR/>%0.2f %c  \n %0.2f M.</TD>\n",porcentaje,'%',KB);
                                fprintf(dot,"<TD><FONT COLOR=\"red\">%s</FONT><BR/>%0.2f %c  \n %0.2f M.</TD>\n",ebr.part_name,porcentaje,'%',KB);
                                EBR tmp = ebr;
                                ebr = getEBR(ruta,ebr.part_next);   //nos pasamos al ebr siguiente

                                if((tmp.part_start + tmp.part_size) < ebr.part_start){

                                    porcentaje = ebr.part_start - (tmp.part_start + tmp.part_size);
                                    KB = porcentaje*1000;
                                    porcentaje = ((float)porcentaje/(float)mbr.mbr_partition_2.part_size);
                                    porcentaje = porcentaje*100;

                                    fprintf(dot,"<TD><FONT COLOR=\"red\">LIBRE</FONT><BR/>%0.2f %c  \n %0.2f M.</TD>\n",porcentaje,'%',KB);
                                }
                            }
                        }
                    }while(true);


                    //vaalidacion de solo el taamaño de la particion******
                    //el ebr tiene la informacion de la ultima particion logica
                    if((ebr.part_start+ ebr.part_size) < (mbr.mbr_partition_2.part_start + mbr.mbr_partition_2.part_size)){

                        porcentaje = (mbr.mbr_partition_2.part_start + mbr.mbr_partition_2.part_size) -(ebr.part_start+ ebr.part_size);
                        float KB = porcentaje*1000;
                        porcentaje = ((float)porcentaje/(float)mbr.mbr_partition_2.part_size);
                        porcentaje = porcentaje*100;

                        fprintf(dot,"<TD><FONT COLOR=\"red\">LIBRE</FONT><BR/>%0.2f %c  \n %0.2f M.</TD>\n",porcentaje,'%',KB);
                    }
                }
                else if((mbr.mbr_partition_3.part_type == 'e' || mbr.mbr_partition_3.part_type == 'E')){

                    do{
                        float KB = porcentaje*1000;
                        porcentaje = ((float)porcentaje/(float)mbr.mbr_partition_3.part_size);
                        porcentaje = porcentaje*100;

                        if(strcasecmp(ebr.part_name,"EBR_INICIAL") == 0 && ebr.part_next == -1)
                        {
                            fprintf(dot,"<TD><FONT COLOR=\"red\">EBR</FONT><BR/>%0.2f %c  \n %0.2f M.</TD>\n",porcentaje,'%',KB);
                            break;
                        }else{
                            if(ebr.part_next == -1)
                            {
                                fprintf(dot,"<TD><FONT COLOR=\"red\">EBR</FONT><BR/>%.2f %c  \n %0.2f M.</TD>\n",porcentaje,'%',KB);
                                fprintf(dot,"<TD><FONT COLOR=\"red\">%s</FONT><BR/>%.2f %c  \n %0.2f M.</TD>\n",ebr.part_name,porcentaje,'%',KB);
                                break;
                            }else if(strcasecmp(ebr.part_name,"EBR_INICIAL") == 0 && ebr.part_next != -1)
                            {
                                porcentaje = ebr.part_size;
                                float KB = porcentaje*1000;
                                porcentaje = ((float)porcentaje/(float)mbr.mbr_partition_3.part_size);
                                porcentaje = porcentaje*100;
                                fprintf(dot,"<TD><FONT COLOR=\"red\">EBR_INICIAL</FONT><BR/>%0.2f %c  \n %0.2f M.</TD>\n",porcentaje,'%',KB);

                                porcentaje = ebr.part_next - (ebr.part_start + ebr.part_size);
                                KB = porcentaje*1000;
                                porcentaje = ((float)porcentaje/(float)mbr.mbr_partition_3.part_size);
                                porcentaje = porcentaje*100;

                                fprintf(dot,"<TD><FONT COLOR=\"red\">LIBRE</FONT><BR/>%0.2f %c  \n %0.2f M.</TD>\n",porcentaje,'%',KB);

                                ebr = getEBR(ruta,ebr.part_next);   //nos pasamos al ebr siguiente
                            }else{
                                fprintf(dot,"<TD><FONT COLOR=\"red\">EBR</FONT><BR/>%0.2f %c  \n %0.2f M.</TD>\n",porcentaje,'%',KB);
                                fprintf(dot,"<TD><FONT COLOR=\"red\">%s</FONT><BR/>%0.2f %c  \n %0.2f M.</TD>\n",ebr.part_name,porcentaje,'%',KB);
                                EBR tmp = ebr;
                                ebr = getEBR(ruta,ebr.part_next);   //nos pasamos al ebr siguiente

                                if((tmp.part_start + tmp.part_size) < ebr.part_start){

                                    porcentaje = (mbr.mbr_partition_3.part_start + mbr.mbr_partition_3.part_size) -(ebr.part_start+ ebr.part_size);
                                    KB = porcentaje*1000;
                                    porcentaje = ((float)porcentaje/(float)mbr.mbr_partition_3.part_size);
                                    porcentaje = porcentaje*100;

                                    fprintf(dot,"<TD><FONT COLOR=\"red\">LIBRE</FONT><BR/>%0.2f %c  \n %0.2f M.</TD>\n",porcentaje,'%',KB);
                                }
                            }
                        }
                    }while(true);


                    //vaalidacion de solo el taamaño de la particion******
                    if((ebr.part_start+ ebr.part_size) < (mbr.mbr_partition_3.part_start + mbr.mbr_partition_3.part_size)){

                        porcentaje = mbr.mbr_partition_3.part_size - ebr.part_size;
                        float KB = porcentaje*1000;
                        porcentaje = ((float)porcentaje/(float)mbr.mbr_partition_3.part_size);
                        porcentaje = porcentaje*100;

                        fprintf(dot,"<TD ROWSPAN=\"3\"><FONT COLOR=\"red\">LIBRE</FONT><BR/>%0.2f %c  \n %0.2f M.</TD>\n",porcentaje,'%',KB);
                    }
                }
                else if((mbr.mbr_partition_4.part_type == 'e' || mbr.mbr_partition_4.part_type == 'E')){

                    do{
                        float KB = porcentaje*1000;
                        porcentaje = ((float)porcentaje/(float)mbr.mbr_partition_4.part_size);
                        porcentaje = porcentaje*100;

                        if(strcasecmp(ebr.part_name,"EBR_INICIAL") == 0 && ebr.part_next == -1)
                        {
                            fprintf(dot,"<TD><FONT COLOR=\"red\">EBR</FONT><BR/>%0.2f %c  \n %0.2f M.</TD>\n",porcentaje,'%',KB);
                            break;
                        }else{
                            if(ebr.part_next == -1)
                            {
                                fprintf(dot,"<TD><FONT COLOR=\"red\">EBR</FONT><BR/>%.2f %c  \n %0.2f M.</TD>\n",porcentaje,'%',KB);
                                fprintf(dot,"<TD><FONT COLOR=\"red\">%s</FONT><BR/>%.2f %c  \n %0.2f M.</TD>\n",ebr.part_name,porcentaje,'%',KB);
                                break;
                            }else if(strcasecmp(ebr.part_name,"EBR_INICIAL") == 0 && ebr.part_next != -1)
                            {
                                porcentaje = ebr.part_size;
                                float KB = porcentaje*1000;
                                porcentaje = ((float)porcentaje/(float)mbr.mbr_partition_4.part_size);
                                porcentaje = porcentaje*100;
                                fprintf(dot,"<TD><FONT COLOR=\"red\">EBR_INICIAL</FONT><BR/>%0.2f %c  \n %0.2f M.</TD>\n",porcentaje,'%',KB);

                                porcentaje = ebr.part_next - (ebr.part_start + ebr.part_size);
                                KB = porcentaje*1000;
                                porcentaje = ((float)porcentaje/(float)mbr.mbr_partition_4.part_size);
                                porcentaje = porcentaje*100;

                                fprintf(dot,"<TD><FONT COLOR=\"red\">LIBRE</FONT><BR/>%0.2f %c  \n %0.2f M.</TD>\n",porcentaje,'%',KB);

                                ebr = getEBR(ruta,ebr.part_next);   //nos pasamos al ebr siguiente
                            }else{
                                fprintf(dot,"<TD><FONT COLOR=\"red\">EBR</FONT><BR/>%0.2f %c  \n %0.2f M.</TD>\n",porcentaje,'%',KB);
                                fprintf(dot,"<TD><FONT COLOR=\"red\">%s</FONT><BR/>%0.2f %c  \n %0.2f M.</TD>\n",ebr.part_name,porcentaje,'%',KB);
                                EBR tmp = ebr;
                                ebr = getEBR(ruta,ebr.part_next);   //nos pasamos al ebr siguiente

                                if((tmp.part_start + tmp.part_size) < ebr.part_start){

                                    porcentaje = (mbr.mbr_partition_4.part_start + mbr.mbr_partition_4.part_size) -(ebr.part_start+ ebr.part_size);
                                    KB = porcentaje*1000;
                                    porcentaje = ((float)porcentaje/(float)mbr.mbr_partition_4.part_size);
                                    porcentaje = porcentaje*100;

                                    fprintf(dot,"<TD><FONT COLOR=\"red\">LIBRE</FONT><BR/>%0.2f %c  \n %0.2f M.</TD>\n",porcentaje,'%',KB);
                                }
                            }
                        }
                    }while(true);


                    //vaalidacion de solo el taamaño de la particion******
                    if((ebr.part_start+ ebr.part_size) < (mbr.mbr_partition_4.part_start + mbr.mbr_partition_4.part_size)){

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
        else{
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
                VerificacionPath = VerficacionPath(Pataux,false);        //mando true porque solo voy a consultar la ruta
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
