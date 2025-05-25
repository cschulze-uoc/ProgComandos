#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include "utilHora.h"
#include "utilAdaptador.h"
#include "utilIpTest.h"
#include "utilDns.h"
#include "utilXml.h"
#include "utilXsl.h"


// Función principal que se ejecuta automáticamente al iniciar el programa. Maneja el flujo y guia al usuario
int main() {
    int opcion;
    char ruta_ips[256], adaptador[256];

    do {
        //Imprimimos el menú en consola
        printf("\n--- MENU PRINCIPAL ---\n");
        printf("1. Insertar fecha y hora en producto3.txt\n");
        printf("2. Comprobar IPs y guardar respuestas\n");
        printf("3. Mostrar info de adaptador de red\n");
        printf("4. Optimizar DNS\n");
        printf("5. Crear XML con datos adaptador\n");
		printf("6. Crear hoja de estilos XSL\n");
        printf("0. Salir\n");
        printf("Selecciona una: ");
        scanf("%d", &opcion); //Leemos la opción que el usuario selecciona por consola
        getchar(); // consumir salto de línea

        switch (opcion) {
        case 1:
            insertar_fecha_hora("producto3.txt");
            break;
        case 2:
            testIps("producto3.txt");
            break;
        case 3:
            seleccionar_y_mostrar_adaptador("adaptador.txt");
            break;
        case 4: {
            dnsFlow();
            break;
        }
        case 5: {
            flujo_crear_xml();
            break;
        } 
        case 6: {
            flujo_crear_xsl("configuracion.xml", "transformacion.xsl");
            break;
        }
        case 0:
            printf("Saliendo...\n");
            break;
        default:
            printf("Opcion no válida.\n"); //Si se introduce cualquier otra cosa se ignora y se repite el bucle
        }
    } while (opcion != 0);

    return 0;
}
