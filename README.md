# Timer-Resolution
Aquí les dejo con cariño este código que tiene un propósito muy especial: Maximizar el temporizador de Windows para una mejor experiencia.

Este programa, es creado por **Patsima** su servidor, en su versión **1.0.0**, está diseñado para monitorear un proceso en tu computadora y ajustar automáticamente la resolución del temporizador del sistema (Timer Resolution) cuando detecta que dicho proceso está corriendo.

### Funcionalidad:

1. **Ajuste de Timer Resolution**: Cuando un proceso específico está en ejecución, ejemplo: FortniteClient-Win64-Shipping.exe
   
2. **Desactivación del Timer Resolution**: Cuando el proceso termina, se restaura la configuración original del temporizador, evitando un consumo innecesario de recursos.

3. **Monitoreo de procesos**: Este código monitorea en tiempo real el estado del proceso que especificas. Si el proceso se está ejecutando, la resolución del temporizador se ajusta automáticamente y, cuando finaliza, el sistema vuelve a su estado normal.

### ¿Cómo funciona?

El corazón de este programa es la clase `TimerResolutionManager`, que utiliza funciones nativas del sistema operativo para interactuar con la librería `ntdll.dll`. A través de esta librería, puede consultar y ajustar el temporizador del sistema con las funciones `NtSetTimerResolution` y `NtQueryTimerResolution`. Todo está preparado para que el sistema sea eficiente y tu aplicación corra como mantequilla.

### Instrucciones antes de su uso:

1. Ejecutar en CMD como administrador los siguientes comandos: `bcdedit /set useplatformtick yes & bcdedit /set disabledynamictick yes`

2. Deshabilitar la compatibilidad con ACPI del temporizador de eventos de alta precisión (HPET). Abre Device Manager, ve hacia el apartado System Devices y da click derecho sobre High precision event timer y deshabilitalo.

### Instrucciones de uso:

1. Cuando ejecutas el programa, te pedirá el nombre del proceso que deseas monitorear (incluyendo la extensión `.exe`). Aquí es donde puedes especificar ese juego o programa que siempre quieres que tenga el temporizador al maximo.
Ejemplo: `FortniteClient-Win64-Shipping.exe`
   
2. El programa monitorea constantemente si el proceso está corriendo, ajusta la resolución cuando es necesario, y una vez que el proceso termina, todo vuelve a la normalidad.

Este código es un tributo a esos momentos en los que necesitas más de tu PC, a esos instantes en los que un milisegundo puede hacer toda la diferencia. ¡Espero lo disfruten y les sirva tanto como a mí me emocionó crearlo!

Con cariño,
**Patsima**
