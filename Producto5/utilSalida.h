#ifndef UTIL_SALIDA_H
#define UTIL_SALIDA_H
#include <stdarg.h>
#include <stdio.h>


void print_fichero_consola(FILE* salida, int n, ...);
void imprimir_archivo_en_pantalla(const char* ruta);

#endif