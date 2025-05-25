#define _CRT_SECURE_NO_WARNINGS
#include "utilSalida.h"

// Imprime en consola y guarda en fichero simultaneamente.
// Puede recibir n parametros con texto que concatenará automáticamente, pero hay que indicarle cuantos son en "n"
void print_fichero_consola(FILE* salida, int n, ...) {
    va_list args;
    va_start(args, n);

    char buffer[256] = "";
    for (int i = 0; i < n; i++) {
        const char* fragmento = va_arg(args, const char*);
        strncat(buffer, fragmento, sizeof(buffer) - strlen(buffer) - 1);
    }

    va_end(args);

    printf("%s", buffer);
    if (salida) {
        fprintf(salida, "%s", buffer);
    }
}

// Imprime el contenido de un archivo en la consola
void imprimir_archivo_en_pantalla(const char* ruta) {
    FILE* archivo = fopen(ruta, "r");
    if (!archivo) {
        fprintf(stderr, "No se pudo abrir el archivo '%s'\n", ruta);
        return;
    }

    char linea[512];
    printf("\n=== CONTENIDO DE '%s' ===\n\n", ruta);

    while (fgets(linea, sizeof(linea), archivo)) {
        printf("%s", linea);
    }

    fclose(archivo);
    printf("\n=== FIN DE '%s' ===\n", ruta);
}