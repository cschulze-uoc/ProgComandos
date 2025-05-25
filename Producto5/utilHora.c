#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "utilHora.h"

// Inserta la fecha y la hora actuales en el fichero cuya ruta recibe por parámetro
void insertar_fecha_hora(const char* ruta_archivo) {
    FILE* archivo = fopen(ruta_archivo, "a");
    if (archivo == NULL) {
        perror("No se pudo abrir el archivo");
        return;
    }

    time_t t = time(NULL);
    struct tm* tm_info = localtime(&t);
    char buffer[64];
    strftime(buffer, sizeof(buffer), "\nFecha y hora: %Y-%m-%d %H:%M:%S\n", tm_info);
    fprintf(archivo, "%s", buffer);
    fclose(archivo);

    printf("Fecha y hora insertadas en %s\n", ruta_archivo);
}




