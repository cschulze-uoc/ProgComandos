#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utilIpTest.h"
#include "utilHora.h"
#include "utilSalida.h"

// Recibe la ruta del fichero del usuario y llama a las funciones
void testIps(const char* rutaSalida) {
    char ruta_ips[256];

    printf("Introduce la ruta del archivo con IPs: ");
    fgets(ruta_ips, sizeof(ruta_ips), stdin);
    ruta_ips[strcspn(ruta_ips, "\n")] = 0;

    imprimir_ips(ruta_ips);

    insertar_fecha_hora(rutaSalida);
    comprobar_ips(ruta_ips, rutaSalida);
}

// Imprime en consola las IP's que se tendrán que comprobar
void imprimir_ips(const char* ruta_ips) {
    FILE* archivo = fopen(ruta_ips, "r");
    char ip[100];

    if (!archivo) {
        perror("No se pudo abrir el archivo de IPs");
        return;
    }

    printf("\nListado de IPs a comprobar:\n");
    printf("--------------------------------------\n");

    while (fgets(ip, sizeof(ip), archivo)) {
        ip[strcspn(ip, "\r\n")] = 0;
        printf(" - %s\n", ip);
    }

    printf("--------------------------------------\n");

    fclose(archivo);
}

// Ejecuta el ping a la IP que recibe como parametro y devuelve 0 o 1 en función del resultado
int probar_ip(const char* ip) {
    char comando[128];
    snprintf(comando, sizeof(comando), "ping -n 1 %s > nul", ip);
    return system(comando) == 0 ? 1 : 0;
}

// Función abre accede al listado de IP's del fichero, las prueba una a una, imprime y guarda los resultados
void comprobar_ips(const char* ruta_archivo_ips, const char* ruta_salida) {
    FILE* archivo_ips = fopen(ruta_archivo_ips, "r");
    FILE* salida = fopen(ruta_salida, "a");
    char ip[100];
    int total = 0, exitosas = 0;

    if (!archivo_ips || !salida) {
        perror("Error al abrir archivos");
        return;
    }

    print_fichero_consola(salida, 1, "\n========================================\n");
    print_fichero_consola(salida, 1, "RESULTADO DEL TEST DE IPs\n");
    print_fichero_consola(salida, 1, "========================================\n");

    while (fgets(ip, sizeof(ip), archivo_ips)) {
        ip[strcspn(ip, "\r\n")] = 0;
        total++;

       
        int ok = probar_ip(ip);
        char buffer[256];
        if (ok) {
            snprintf(buffer, sizeof(buffer), "  %-20s -> OK\n", ip);
            print_fichero_consola(salida, 1, buffer);
            exitosas++;
        }
        else {
            snprintf(buffer, sizeof(buffer), "  %-20s -> FAIL\n", ip);
            print_fichero_consola(salida, 1, buffer);
        }
    }

    print_fichero_consola(salida, 1, "----------------------------------------\n");
    char bufferS[256];
    snprintf(bufferS, sizeof(bufferS), "Total: %d IPs comprobadas | %d OK | %d FAIL\n",
        total, exitosas, total - exitosas);
    print_fichero_consola(salida, 1, bufferS);
    print_fichero_consola(salida, 1, "========================================\n\n");

    printf("Test finalizado. Resultados guardados en '%s'\n", ruta_salida);

    fclose(archivo_ips);
    fclose(salida);
}
