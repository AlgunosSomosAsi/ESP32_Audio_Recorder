/*  V 3.0 Graba correctamente. 48 kH @ 16 bit.

* / Es necesario que todas las inicializaciones que necesite la función 'grabación (record) sean llamadas desde esta
    y manejar los errores, permitiendo repetir la operacion (abrir puerto, cerrar puerto, crear archivo, grabar, etc).
       



      Hardware:
  * - Solo 44.1 kHz a 16 bits.
  * - Posibilidad de tener una fuente de alimentación que corte la alimentación cuando hay problemas de grabación.

  * - Verificar conexion. La alimentación del módulo SD va a depender del elegido. La alimmentación del PMOD es 3.3V.
  * - Agregar pull up 10 kohm en SS a cada modulo SD. Chequear si a 3.3V o 5V.
  * - Agregar RTC externo.
  * - Pruebas con SD nuevas, de mejor calidad y velocidad.
  * - Redundancia necesaria : 4 SD para garantizar que se se realicen las grabaciones.

      Software:
  ___ Chequear por que los datos generados son el doble de lo que deberían ser teoricamente.
  ___ Manipulacion de información. Generar un audio que solo transmita información en 1 canal para verificar que se esta guardando
        la información que corresponde.
  ___ El sistema debe poder chequear que se inicio la tarjeta SD grabando archivos de prueba / flush.
  ___ Try/catch o similar. Reintentar cada paso del programa. Reintentar a una velocidad de SPI CLK menor.
  ___ Manejar errores.
  * - Si la inicializacion de la sd falla, reintentar. Limitar el numero de intentos, sino reiniciar.
  * - Implementar logger.
  * - Contar muestras perdidas / no grabadas.
  * - Si hay errores de grabación reiterados descartar la grabación y reiniciar.
  * - Generar nombre del archivo y time stamp antes de grabar.
  * - filename : agregar función.
  * - Timestamp: agregar RTC y función.
  * - Chequear tamaño restante en la SD antes de grabar. Pasar a la siguiente SD si es necesario.
  * - Cuando el sistema falla constantemente, reset y  sleep hasta el próximo ciclo de grabación.
*/