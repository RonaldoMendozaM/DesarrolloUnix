# DesarrolloUnix
Proyecto Final de Unix

Para inicializar el proyecto debes tener descargado C 
y tambien net-tools si no tienes nc

Comando:  sudo apt install net-tools      --si no tienes nc


Para correr el proyecto tienes que ejercutar por terminal de linux este comando

    gcc -o socketNet socketNet.c -pthread -- correr
Acontinuacion

    ./socketNet 8080  -- inicializar 

En otra terminal debes inciar el comando para ingresar al servidor

    nc localhost 8080 -- inicializar

Este comando es para matar el puerto 8080 cuando lo utilices 

    fuser -k 8080/tcp --matar