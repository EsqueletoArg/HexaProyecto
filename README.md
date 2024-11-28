# HexaProyecto

# Descripcion General
Es  ajedrez de 3x3 casillas, donde se enfrentan 3 peones de ambos bandos para inmovilizar al rival o llegar al otro lado del tablero.

# Software
* Programado para ser subido en un ESP32, este proyecto cuenta con un algoritmo que actua como el rival de este ajedrez. Este software se encuentra en la carpeta **/Firmware**, en el archivo nombrado "*HexaProyecto.ino*" y fue programado a mano usando Arduino IDE.

* En cuanto al servidor, se encuentra alojado en la carpeta **/Firmware/Server** en el archivo "*Servidor.js*". Para hacerlo funcionar, necesitamos descargar los programas Node.js y Mosquitto, ambos son descargables de manera gratuita en su respectiva página oficial. 

* Para hacerlo funcionar correctamente, hay que descargar ciertas dependencias en la carpeta donde se encuentre el servidor. Estas dependencias no funcionaran sin Nodejs y/o Mosquitto. Para descargarlas hay que dirigirse al directorio donde se encuentra el archivo "*Servidor.js*" via cmd e introducir los comandos "*npm init -y*" y "*npm install mqtt express body-parser*". Descargar cada dependencia podría tardar un tiempo, pero no de manera significativa.

# Hardware (PCB)
El hardware de la PCB se encuentra en la carpeta 

El archivo III.pcb contiene el diseño de la PCB. La matriz de botones conlleva un diodo para cada botón para evitar 
