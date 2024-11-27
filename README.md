a# HexaProyecto

# Descripcion General
Es  ajedrez de 3x3 casillas, donde se enfrentan 3 peones de ambos bandos para inmovilizar al rival o llegar al otro lado del tablero.

# Software
Programado para ser subido en un ESP32, este proyecto cuenta con un algoritmo que actua como el rival de este ajedrez. Este software se encuentra en el archivo proyectolol.ino y fue programado a mano usando Arduino IDE.

En cuanto al servidor, se encuentra alojado en un archivo .js. Para hacerlo funcionar, necesitamos descargar los programas Node.js y Mosquitto, ambos son descargables de manera gratuita en su respectiva página oficial. 

Para hacerlo funcionar correctamente, hay que descargar ciertas dependencias en la carpeta donde se encuentre el servidor. Estas dependencias, sin embargo, ya estan alojadas junto al servidor, pero no funcionaran sin Nodejs y/o Mosquitto

# Hardware (PCB)
El hardware se encuentra en los archivos III.pcb, III.sch y III.pro

El archivo III.pcb contiene el diseño de la PCB. La matriz de botones conlleva un diodo para cada botón para evitar 
