# DesarrolloUnix
Proyecto Final de Unix

Para inicializar el proyecto debes tener descargado C 
y tambien net-tools si no tienes nc

Comando:  

    sudo apt install net-tools

--si no tienes nc

Para correr el proyecto tienes que ejercutar por terminal de linux este comando
Correr

    gcc -o socketNet socketNet.c -pthread 

Acontinuacion
Inicializar

    ./socketNet 8080  

En otra terminal debes inciar el comando para ingresar al servidor
Inicializar

    nc localhost 8080 

Puedes correr los comandos que hay en la carpeta de comandos.txt para verificar el puedes mandar informacion


Este comando es para matar el puerto 8080 cuando lo utilices 

    fuser -k 8080/tcp 


Write a service running a NET socket, max connections should be 30
•	it should receive the port as program argument
•	every client connection should be handled by a thread or fork
•	it should handle SIGINT signal (Ctrl+C)
•	the service will receive the following messages 
    •	getInfo that should return a service name and the version
        •	i.e. “Mike’s service, v0.1”
    •	getNumberOfPartitions, should return number of partitions of the system
        •	it can be implemented using bash scripting or calling directly to utilities, i.e. lsblk -l | grep part
    •	getCurrentKernelVersion, should return the kernel version that the system is running
        •	just the numerical par, you can use uname -r, it also can be though bash scripting or calling directly to uname utility
    •	sshdRunning, should return true or false
        •	it should be implemented using popen()
        •	you can use one of these methods, or another one that could detect that sshd is running
            •	ps ax|grep sshd
            •	netstat -l |grep ssh
            •	verify that sshd.pid file exists (/var/run/sshd.pid)
    •	for getInfo, getNumberOfPartitions, and getCurrentKernelVersion, two of them can be implemented with bash scripting, the third one needs to be with popen or execv (or variations) if applicable.

