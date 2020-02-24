#Contenido de calificacion.sh
#Crea 2 discos

#CREANDO DISCOS ----------------------------------------------------------------------------------------
mkdisk -size=75 -unit=M -path=/home/archivos/fase2/Disco1.disk -fit=FF
mkdisk -unit=K -size=51200 -path=/home/archivos/fase2/Disco2.disk -fit=FF

#CREANDO PARTICIONES ----------------------------------------------------------------------------------
#Crear particiones Disco1.disk
fdisk -type=P -unit=M -name=Part1 -size=20 -path=/home/archivos/fase2/Disco1.disk -fit=FF
fdisk -type=P -unit=M -name=Part2 -size=15 -path=/home/archivos/fase2/Disco1.disk -fit=FF
fdisk -type=P -unit=M -name=Part3 -size=20 -path=/home/archivos/fase2/Disco1.disk -fit=FF

#Crear particiones Disco2.disk
fdisk -type=P -unit=M -name=Part1_D2 -size=20 -path=/home/archivos/fase2/Disco2.disk -fit=FF
fdisk -type=P -unit=M -name=Part2_D2 -size=10 -path=/home/archivos/fase2/Disco2.disk -fit=FF
fdisk -type=P -unit=M -name=Part3_D2 -size=10 -path=/home/archivos/fase2/Disco2.disk -fit=FF

#Monta las particiones 

mount -path=/home/archivos/fase2/Disco1.disk -name=Part1 #vda1
mount -path=/home/archivos/fase2/Disco1.disk -name=Part2 #vda2
mount -path=/home/archivos/fase2/Disco1.disk -name=Part3 #vda3

mount -path=/home/archivos/fase2/Disco2.disk -name=Part1_D2 #vdb1
mount -path=/home/archivos/fase2/Disco2.disk -name=Part2_D2 #vdb2
mount -path=/home/archivos/fase2/Disco2.disk -name=Part3_D2 #vdb3

#Crea el sistema de archivos en todas las particiones primarias
#fast reescribe el superbloque 
#full poner /0 y luego reescribir el superbloque

mkfs -type=fast -id=vda1 -fs=3fs
mkfs -type=full -id=vda2 -fs=3fs
mkfs -type=fast -id=vda3 -fs=3fs

#Reporte1 ::::(  ) 
rep -id=vda1 -Path="/home/archivos/reports2/reporte1_vda1_sb.pdf" -name=sb
rep -id=vda1 -Path="/home/archivos/reports2/reporte1_vda1_tree.jpg" -name=tree

pause

#Creando Usuarios
#Login como root en el vda1 - Part1 de tipo EXT2
#Debe dar error porque no existe el usuario roca
Login -pwd=567 -usr=roca -id=vda1
#debe dar error porque no existe nada activo
logout
#Debe dar error contraseña incorrecta
Login -pwd=12345 -usr=root -id=vda1
Login -pwd=123 -usr=root -id=vda1
#Debe dar error porque ya hay un login activo
lOgin -pwd=error -usr=error -id=vda1
pause 

#Creando grupos en vda1
mkgrp -naMe=Archivos
mkgrp -NamE=Arqui
mkgrp -name="Compi 2"
mkgrp -NamE=Grupo1
#Error en el mkgrp grupo ya existente
mkgrp -name="Compi 2"

#Borrando Grupos
#Error no existe este grupo
rmgrp -name="DB2"
rmgrp -name=Arqui

#Crearemos 3 usuarios llamados user1, user2, user3
Mkusr  -usr="user1" -grp=root -pwd=user1
Mkusr  -usr="user2" -grp="Compi 2" -pwd=user2
Mkusr  -usr="user3" -grp="Compi 2" -pwd=user3
#Da error porque ya existe ese usuario
Mkusr  -usr="user2" -grp="Compi 2" -pwd=user2
#Da error porque no existe ese grupo
Mkusr  -usr="user2" -grp=Arqui -pwd=user2

rmusr -usr=user3
rmusr -usr=user3 #Debe dar error porque ya no existe
rep -id=vda1 -Path="/home/archivos/reports2/Manejo_Usuarios.txt" -ruta="/users.txt" -name=file
pause

Mkdir -P  -path=/home/archivos/mia/fase2
Mkdir -P  -path=/home/archivos/mia/carpeta2
Mkdir -P  -path=/home/archivos/mia/z
Mkdir -P  -path=/home/archivos/mia/carpeta3/carpeta7/carpeta8/carpeta9/carpeta10/carpeta11
Mkdir -P  -path=/home/archivos/mia/carpeta4/carpeta7/carpeta8/carpeta9/carpeta10/carpeta11/carpeta7/carpeta8/carpeta9/carpeta10/carpeta11
Mkdir  -path=/home/archivos/mia/carpeta2/a1
Mkdir  -path=/home/archivos/mia/carpeta2/a2
Mkdir  -path=/home/archivos/mia/carpeta2/a3
Mkdir  -path=/home/archivos/mia/carpeta2/a4
Mkdir  -path=/home/archivos/mia/carpeta2/a5
Mkdir  -path=/home/archivos/mia/carpeta2/a6
Mkdir  -path=/home/archivos/mia/carpeta2/a7
Mkdir  -path=/home/archivos/mia/carpeta2/a8
Mkdir  -path=/home/archivos/mia/carpeta2/a9
Mkdir  -path=/home/archivos/mia/carpeta2/a10
#Error ya existente
Mkdir  -path=/home/archivos/mia/carpeta2/a10
Pause

logout
#PERMISOS EN ARCHIVOS Y CARPETAS
login -usr=user2 -pwd=user2 -id=vda1
mkfile -path="/home/NOCREAR.txtS" -size=75
Pause

logout
loGin -usr=root -pwd=123 -id=vda1
#Archivo 
mkfile -path="/home/b1.txt" -size=75
#Debe dar error ruta no existe
mkfile -path="/home/Noexiste/b1.txt" -size=75
#Debe dar error size negativo
mkfile -path="/home/b1.txt" -size=-45
#archivo con parametro -p 
mkfile -p -path="/home/ahora/ya existe/b1.txt" -size=640
#File 1er Indirecto
mkfile -p -path="/home/ind.txt" -size=1200
#reporte3 ::::(  ) 
rep -id=vda1 -Path="/home/archivos/reports2/reporte3_vda1_sb.pdf" -name=sb
rep -id=vda1 -Path="/home/archivos/reports2/reporte3_vda1_tree.jpg" -name=tree

pause

#REMOVER
#Archivo Directos
rem -path=/home/ind.txt

#Reporte4 ::::(  ) 
rep -id=vda1 -Path="/home/archivos/reports2/reporte4_vda1_sb.pdf" -name=sb
rep -id=vda1 -Path="/home/archivos/reports2/reporte4_vda1_tree.jpg" -name=tree

pause

#MOVER
#debe dar error porque no existe
mv -path="/home/archivos/no existe" -dest="/home"
mv -path=/home/archivos/mia/carpeta2/a10 -dest="/home"

Mkdir  -path=/home/archivos/mia/fase2/a1
Mkdir  -path=/home/archivos/mia/fase2/a2
Mkdir  -path=/home/archivos/mia/fase2/a3
Mkdir  -path=/home/archivos/mia/fase2/a4
Mkdir  -path=/home/archivos/mia/fase2/a5
Mkdir  -path=/home/archivos/mia/fase2/a6
Mkdir  -path=/home/archivos/mia/fase2/a7
Mkdir  -path=/home/archivos/mia/fase2/a8
Mkdir  -path=/home/archivos/mia/fase2/a9
Mkdir  -path=/home/archivos/mia/fase2/a10
Mkdir  -path=/home/archivos/mia/fase2/a11
Mkdir  -path=/home/archivos/mia/fase2/a12
Mkdir  -path=/home/archivos/mia/fase2/a13
Mkdir  -path=/home/archivos/mia/fase2/a14
Mkdir  -path=/home/archivos/mia/fase2/a15
Mkdir  -path=/home/archivos/mia/fase2/a16
Mkdir  -path=/home/archivos/mia/fase2/a17
Mkdir  -path=/home/archivos/mia/fase2/a18
Mkdir  -path=/home/archivos/mia/fase2/a19
Mkdir  -path=/home/archivos/mia/fase2/a20
Mkdir  -path=/home/archivos/mia/fase2/a21
Mkdir  -path=/home/archivos/mia/fase2/a22
Mkdir  -path=/home/archivos/mia/fase2/a23
Mkdir  -path=/home/archivos/mia/fase2/a24
Mkdir  -path=/home/archivos/mia/fase2/a25
Mkdir  -path=/home/archivos/mia/fase2/a26
Mkdir  -path=/home/archivos/mia/fase2/a27
Mkdir  -path=/home/archivos/mia/fase2/a28
Mkdir  -path=/home/archivos/mia/fase2/a29
Mkdir  -path=/home/archivos/mia/fase2/a30
Mkdir  -path=/home/archivos/mia/fase2/a31
Mkdir  -path=/home/archivos/mia/fase2/a32
Mkdir  -path=/home/archivos/mia/fase2/a33
Mkdir  -path=/home/archivos/mia/fase2/a34
Mkdir  -path=/home/archivos/mia/fase2/a35
Mkdir  -path=/home/archivos/mia/fase2/a36
Mkdir  -path=/home/archivos/mia/fase2/a37
Mkdir  -path=/home/archivos/mia/fase2/a38
Mkdir  -path=/home/archivos/mia/fase2/a39
Mkdir  -path=/home/archivos/mia/fase2/a40
Mkdir  -path=/home/archivos/mia/fase2/a41
Mkdir  -path=/home/archivos/mia/fase2/a42
Mkdir  -path=/home/archivos/mia/fase2/a43
Mkdir  -path=/home/archivos/mia/fase2/a44
Mkdir  -path=/home/archivos/mia/fase2/a45
Mkdir  -path=/home/archivos/mia/fase2/a46

#Primer indirecto
mkdir  -path="/home/archivos/mia/fase2/b1"
mkdir  -path="/home/archivos/mia/fase2/b2"
mkdir  -path="/home/archivos/mia/fase2/b3"
mkdir  -path="/home/archivos/mia/fase2/b4"
mkdir  -path="/home/archivos/mia/fase2/b5"
mkdir  -path="/home/archivos/mia/fase2/b6"
mkdir  -path="/home/archivos/mia/fase2/b7"
mkdir  -path="/home/archivos/mia/fase2/b8"
mkdir  -path="/home/archivos/mia/fase2/b9"
mkdir  -path="/home/archivos/mia/fase2/b10"
mkdir  -path="/home/archivos/mia/fase2/b11"
mkdir  -path="/home/archivos/mia/fase2/b12"
mkdir  -path="/home/archivos/mia/fase2/b13"
mkdir  -path="/home/archivos/mia/fase2/b14"
mkdir  -path="/home/archivos/mia/fase2/b15"
mkdir  -path="/home/archivos/mia/fase2/b16"
mkdir  -path="/home/archivos/mia/fase2/b17"
mkdir  -path="/home/archivos/mia/fase2/b18"
mkdir  -path="/home/archivos/mia/fase2/b19"
mkdir  -path="/home/archivos/mia/fase2/b20"
mkdir  -path="/home/archivos/mia/fase2/b21"
mkdir  -path="/home/archivos/mia/fase2/b22"
mkdir  -path="/home/archivos/mia/fase2/b23"
mkdir  -path="/home/archivos/mia/fase2/b24"
mkdir  -path="/home/archivos/mia/fase2/b25"
mkdir  -path="/home/archivos/mia/fase2/b26"
mkdir  -path="/home/archivos/mia/fase2/b27"
mkdir  -path="/home/archivos/mia/fase2/b28"
mkdir  -path="/home/archivos/mia/fase2/b29"
mkdir  -path="/home/archivos/mia/fase2/b30"
mkdir  -path="/home/archivos/mia/fase2/b31"
mkdir  -path="/home/archivos/mia/fase2/b32"
mkdir  -path="/home/archivos/mia/fase2/b33"
mkdir  -path="/home/archivos/mia/fase2/b34"
mkdir  -path="/home/archivos/mia/fase2/b35"
mkdir  -path="/home/archivos/mia/fase2/b36"
mkdir  -path="/home/archivos/mia/fase2/b37"
mkdir  -path="/home/archivos/mia/fase2/b38"
mkdir  -path="/home/archivos/mia/fase2/b39"
mkdir  -path="/home/archivos/mia/fase2/b40"
mkdir  -path="/home/archivos/mia/fase2/b41"
mkdir  -path="/home/archivos/mia/fase2/b42"
mkdir  -path="/home/archivos/mia/fase2/b43"
mkdir  -path="/home/archivos/mia/fase2/b44"
mkdir  -path="/home/archivos/mia/fase2/b45"
mkdir  -path="/home/archivos/mia/fase2/b46"
mkdir  -path="/home/archivos/mia/fase2/b47"
mkdir  -path="/home/archivos/mia/fase2/b48"
mkdir  -path="/home/archivos/mia/fase2/b49"
mkdir  -path="/home/archivos/mia/fase2/b50"

#reporte2 ::::(  ) 
rep -id=vda1 -Path="/home/archivos/reports2/reporte2_vda1_sb.pdf" -name=sb
rep -id=vda1 -Path="/home/archivos/reports2/reporte2_vda1_tree.jpg" -name=tree

pause

#Reporte5 ::::(  ) 
#Ultimo reporte antes de la perdida
rep -id=vda1 -Path="/home/archivos/reports2/users.txt" -ruta="/users.txt" -name=file
rep -id=vda1 -Path="/home/archivos/reports2/ls.pdf" -ruta="/" -name=ls
rep -id=vda1 -Path="/home/archivos/reports2/reporte5_vda1_sb.pdf" -name=sb
rep -id=vda1 -Path="/home/archivos/reports2/reporte5_vda1_tree.jpg" -name=tree
rep -id=vda1 -Path="/home/archivos/reports2/reporte5_vda1_inode.pdf" -name=inode
rep -id=vda1 -Path="/home/archivos/reports2/reporte5_vda1_block.pdf" -name=block
rep -id=vda1 -Path="/home/archivos/reports2/reporte5_vda1_bm_inode.txt" -name=bm_inode
rep -id=vda1 -Path="/home/archivos/reports2/reporte5_vda1_bm_block.txt" -name=bm_block
rep -id=vda1 -Path="/home/archivos/reports2/reporte5_vda1_journaling.pdf" -name=journaling
pause

#PERDIDA
Loss -id=vda1

#reporte6 ::::(  ) 
#Reporte despues de la perdida es el 6
rep -id=vda1 -Path="/home/archivos/reports2/reporte6_vda1_sb.pdf" -name=sb
rep -id=vda1 -Path="/home/archivos/reports2/reporte6_vda1_tree.jpg" -name=tree
rep -id=vda1 -Path="/home/archivos/reports2/reporte6_vda1_inode.pdf" -name=inode
rep -id=vda1 -Path="/home/archivos/reports2/reporte6_vda1_block.pdf" -name=block
rep -id=vda1 -Path="/home/archivos/reports2/reporte6_vda1_bm_inode.txt" -name=bm_inode
rep -id=vda1 -Path="/home/archivos/reports2/reporte6_vda1_bm_block.txt" -name=bm_block
rep -id=vda1 -Path="/home/archivos/reports2/reporte6_vda1_journaling.pdf" -name=journaling
pause

#Recovery
Recovery -id=vda1
#reporte7 ::::(  ) 
#Reporte despues del recovery es el 7
rep -id=vda1 -Path="/home/archivos/reports2/reporte7_vda1_sb.pdf" -name=sb
rep -id=vda1 -Path="/home/archivos/reports2/reporte7_vda1_tree.jpg" -name=tree
rep -id=vda1 -Path="/home/archivos/reports2/reporte7_vda1_inode.pdf" -name=inode
rep -id=vda1 -Path="/home/archivos/reports2/reporte7_vda1_block.pdf" -name=block
rep -id=vda1 -Path="/home/archivos/reports2/reporte7_vda1_bm_inode.txt" -name=bm_inode
rep -id=vda1 -Path="/home/archivos/reports2/reporte7_vda1_bm_block.txt" -name=bm_block
rep -id=vda1 -Path="/home/archivos/reports2/reporte7_vda1_journaling.pdf" -name=journaling
pause


#*************************************
#A PARTIR DE AQUI YA NO SON MINIMOS
#*************************************
#Renombrar
logout
Login -pwd=123 -usr=root -id=vda1
ren -path=/home/a10 -name=ren10
ren -path="/home/archivos/mia/z" -name=rename
logout
loGin -usr=user2 -pwd=user2 -id=vda1
#Este debe fallar por permisos de escritura
ren -path="/home/b1.txt" -name=brenombre.txt
logoUt
#Pausa para el renombrar
pause

Login -pwd=123 -usr=root -id=vda1

#Indirectos dobles
mkfile -path="/home/b2ble.txt" -size=18000
#Indirectos triples
mkfile -path="/home/b3ple.txt" -size=280000
#Reporte 8
rep -id=vda1 -Path="/home/archivos/reports2/reporte8_vda1_tree.jpg" -name=tree
pause 

#Copiar
#Debe dar error porque destino no existe
cp -path=/home/archivos/mia/carpeta3 -dest=/home/archivos/mia/yo/no/existo
cp -path=/home/archivos/mia/carpeta3 -dest=/home/archivos/mia/carpeta2
#Copiar indirectos
cp -path=/home/archivos/mia/fase2 -dest=/home/archivos/mia/carpeta2
#Reporte 9
rep -id=vda1 -Path="/home/archivos/reports2/reporte9_vda1_tree.jpg" -name=tree
pause 

#FIND
find -Path="/" -name=b1.txt
pause
find -Path="/" -name=rename
#Debe dar error porque no exxiste path
find -Path="/no/existo" -name=?
pause

#EDIT
edit -path="/home/b1.txt" -cont=hola_mundo
edit -path="/home/ahora/ya existe/b1.txt" -cont=Como_estas
#Reporte 10
rep -id=vda1 -Path="/home/archivos/reports2/reporte10_vda1_tree.jpg" -name=tree

#CAT
logout
Login -pwd=user2 -usr=user2 -id=vda1
#Escritura
cat -file=/home/b1.txt
logout
Login -pwd=123 -usr=root -id=vda1
cat -file=/users.txt
Pause 


#CHGRP
chgrp -usr=user1 -grp="Compi 2"
rep -id=vda1 -Path="/home/archivos/reports2/usersCHGRP.txt" -ruta="/users.txt" -name=file
logout
loGin -usr=user1 -pwd=user1 -id=vda1
#Debe marcar error porque no es root
chgrp -usr=user1 -grp=Archivos
logout
pause

#CHOWN
Login -pwd=123 -usr=root -id=vda1
chown -path="/home/archivos/mia/fase2" -usr=user2 -R
rep -id=vda1 -Path="/home/archivos/reports2/lsCHOWN.pdf" -ruta="/home/archivos/mia/fase2" -name=ls
pause

chmod -path=/ -ugo=000 -R
rep -id=vda1 -Path="/home/archivos/reports2/lsULTIMO.pdf" -ruta="/" -name=ls
pause


#FIN DE LA CALIFICACION :)(