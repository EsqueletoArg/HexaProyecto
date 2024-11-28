# HexaProyecto

# Descripcion General:
Es  ajedrez de 3x3 casillas, donde se enfrentan 3 peones de ambos bandos para inmovilizar al rival o llegar al otro lado del tablero.

# Software:
* Programado para ser subido en un ESP32, este proyecto cuenta con un algoritmo que actua como el rival de este ajedrez. Este software se encuentra en la carpeta **/Firmware**, en el archivo nombrado "*HexaProyecto.ino*" y fue programado a mano usando Arduino IDE. Si desean construir uno en sus hogares, en las primeras lineas de esta programacion deberían cambiar ciertos valores para que el proyecto pueda conectarse a internet correctamente.

* En cuanto al servidor, se encuentra alojado en la carpeta **/Firmware/Server** en el archivo "*Servidor.js*". Para hacerlo funcionar, necesitamos descargar los programas Node.js y Mosquitto, ambos son descargables de manera gratuita en su respectiva página oficial. 

* Para hacerlo funcionar correctamente, hay que descargar ciertas dependencias en la carpeta donde se encuentre el servidor. Estas dependencias no funcionaran sin Nodejs y/o Mosquitto. Para descargarlas hay que dirigirse al directorio donde se encuentra el archivo "*Servidor.js*" via cmd e introducir los comandos "*npm init -y*" y "*npm install mqtt express body-parser*". Descargar cada dependencia podría tardar un tiempo, pero no de manera significativa.

# Hardware (PCB):
* El hardware de la PCB se encuentra en la carpeta **/Hardware_(PCB)**. Cada archivo dentro de esta carpeta es parte del diseño del PCB, desde el PCB final, hasta el esquema usado como referencia.

* Como el ESP32 está conectado al internet, esto reducía enormemente la cantidad de pines (Entradas y salidas), que podiamos usar. Por eso decidimos hacer una matriz de botones, lo que nos permitio reducir la cantidad de pines usados de 9 a 6.

* Para la placa usamos unos LEDs inteligentes llamados "WS2812", que con cierto nivel de programación son capaces de formar cualquier color. Estos LEDs solo necesitan una fuente de alimentacion de 5V, GND y una entrada de datos. Estos LEDs tambien tienen un cable de salida de datos que nos permitió poner 9 LEDs en serie y, por tanto, usar una sola salida del ESP32.

# Hardware (3D):
* Para el 3D hicimos un marco para encajar la PCB. Si bien puede sonar a que es por motivos de poco esfuerzo, la verdad es otra: Es algo simple, barato y que le permite al PCB relucir todo el esfuerzo que le dedicamos para diseñarlo.
