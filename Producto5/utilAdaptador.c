#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utilAdaptador.h"
#include <stdarg.h>
#include "utilSalida.h"



//Busca la info del adaptador que recibe por parámetro y la escribe en el fichero indicado
void mostrar_info_adaptador(const char* nombre_adaptador, const char* ruta_salida) {
    
    //ejecuta el comando ipConfig y guarda la salida en un fichero temporal
    system("ipconfig > temp_adaptador.txt");

    FILE* temp = fopen("temp_adaptador.txt", "r");
    FILE* salida = fopen(ruta_salida, "w");
    char linea[MAX_LINEA];
    int mostrar = 0;

    if (!temp || !salida) {
        perror("Error al abrir archivos");
        return;
    }
    // Empieza a escribir 
    print_fichero_consola(salida, 1, "========================================\n");
    print_fichero_consola(salida, 3,  "CONFIGURACION DEL ADAPTADOR: ", nombre_adaptador,"\n");
    print_fichero_consola(salida, 1, "========================================\n");

    while (fgets(linea, sizeof(linea), temp)) {
        if (strstr(linea, nombre_adaptador)) {
            mostrar = 1;
        }
        if (mostrar) {
            if (strstr(linea, "IPv4") ||
                strstr(linea, "scara de subred") ||
                strstr(linea, "Puerta de enlace")) 
            {
                print_fichero_consola(salida,1, linea);
            }
            // si aparece otro adaptador, salimos
            else if ((strstr(linea, "Adaptador") || strstr(linea, "adapter")) && !strstr(linea, nombre_adaptador)) {
                break;
            }
        }
    }

    print_fichero_consola(salida, 1, "========================================\n");

    fclose(temp);
    fclose(salida);
    remove("temp_adaptador.txt"); // eliminamos el fichero temporal
}

// Listamos los adaptadores desde la salida de ipconfig, los listamos por consola y esperamos que el usuario seleccione uno.
void seleccionar_adaptador(char* destino)
{
    system("ipconfig > temp_adaptador.txt"); // guardamos la salida de ipconfig en un fichero temporal

    FILE* temp = fopen("temp_adaptador.txt", "r");
    char linea[MAX_LINEA];
    char adaptadores[MAX_ADAPTADORES][MAX_LINEA];
    int total = 0;

    if (!temp) {
        perror("No se pudo leer los adaptadores");
        return;
    }

    printf("\nAdaptadores de red disponibles:\n");
    printf("========================================\n");

    while (fgets(linea, sizeof(linea), temp)) {
        if (strstr(linea, "Adaptador") || strstr(linea, "adapter")) {
            linea[strcspn(linea, "\r\n")] = 0;  // quitar salto de línea
            strncpy(adaptadores[total], linea, MAX_LINEA);
            printf("  [%2d] %s\n", total + 1, adaptadores[total]);
            total++;
            if (total >= MAX_ADAPTADORES) break;
        }
    }

    fclose(temp); // Cerramos el fichero temporal

    if (total == 0) {
        printf("No se encontraron adaptadores de red.\n");
        return;
    }

    printf("========================================\n");

    int opcion = 0;
    do {
        printf("Selecciona el numero del adaptador (1-%d): ", total);
        if (scanf("%d", &opcion) != 1) {
            while (getchar() != '\n'); // limpiar entrada
            opcion = 0;
        }
    } while (opcion < 1 || opcion > total);

    getchar(); // limpiar salto de línea después de scanf
    strcpy(destino, adaptadores[opcion - 1]);
}

// Mostramos los nombres de los adaptadores de red por consola y esperamos que se seleccione uno.
void seleccionar_y_mostrar_adaptador(const char* ruta_salida) {
    char adpatadorSeleccionado[MAX_LINEA];
    seleccionar_adaptador(adpatadorSeleccionado);

    // Mostrar configuracion del adaptador seleccionado
    printf("\nMostrando informacion del adaptador seleccionado:\n");
    mostrar_info_adaptador(adpatadorSeleccionado, ruta_salida);
}

// Seleccionamos el adaptador de red usando netsh, lo mostramos por consolay esperamos que el usuario seleccione uno
void seleccionar_adaptador_netsh(char* destino) {
    system("netsh interface show interface > interfaces.txt");

    FILE* file = fopen("interfaces.txt", "r");
    if (!file) {
        perror("No se pudo ejecutar netsh o leer interfaces.txt");
        strcpy(destino, "");
        return;
    }

    char linea[MAX_LINEA];
    char adaptadores[MAX_ADAPTADORES][MAX_LINEA];
    int total = 0;
    int lineas_saltadas = 0;

    printf("\nAdaptadores de red disponibles (según netsh):\n");
    printf("==============================================\n");

    while (fgets(linea, sizeof(linea), file)) {
        // Saltar cabecera (primeras 3 líneas)
        if (lineas_saltadas < 3) {
            lineas_saltadas++;
            continue;
        }

        // Línea tipo: Enabled        Connected      Dedicated        Wi-Fi
        char* nombre = strrchr(linea, ' '); // busca última palabra
        if (nombre) {
            while (*nombre == ' ') nombre++; // saltar espacios
            // quitar salto de línea
            nombre[strcspn(nombre, "\r\n")] = 0;
            strncpy(adaptadores[total], nombre, MAX_LINEA);
            printf(" [%d] %s\n", total + 1, adaptadores[total]);
            total++;
            if (total >= MAX_ADAPTADORES) break;
        }
    }

    fclose(file);
    remove("interfaces.txt");

    if (total == 0) {
        printf("No se encontraron adaptadores de red.\n");
        strcpy(destino, "");
        return;
    }

    printf("==============================================\n");

    int opcion = 0;
    do {
        printf("Selecciona el número del adaptador (1-%d): ", total);
        if (scanf("%d", &opcion) != 1) {
            while (getchar() != '\n'); // limpiar
            opcion = 0;
        }
    } while (opcion < 1 || opcion > total);

    getchar(); // limpiar salto de línea

    strcpy(destino, adaptadores[opcion - 1]);
}



// Elimina espacios iniciales de cada campo
void limpiar_espacios(char* texto) {
    char* inicio = texto;
    while (*inicio == ' ') inicio++;

    if (inicio != texto) {
        memmove(texto, inicio, strlen(inicio) + 1);
    }
}

void init_configRed(ConfigRed* config, char* nombre)
{
    strcpy(config->ip, "");
    strcpy(config->mascara, "");
    strcpy(config->gateway, "");
    strcpy(config->dns, "");
    strcpy(config->nombre, nombre);
}

// Extrae la información de la red solicitada y la almacena en la estructura ConfigRed
void extraer_info_adaptador(const char* nombre_netsh, ConfigRed* config) {
    system("ipconfig /all > temp_adaptador.txt");
    
    char linea[MAX_LINEA];
    int dentro_bloque = 0;

    FILE* file = fopen("temp_adaptador.txt", "r");
    if (!file) {
        perror("No se pudo abrir temp_adaptador.txt");
        return;
    }

    // Construimos el encabezado esperado (por ejemplo: "Adaptador de LAN inalámbrica Wi-Fi:")
    char sufijo_esperado[MAX_LINEA];
    snprintf(sufijo_esperado, sizeof(sufijo_esperado), " %s:", config->nombre);
    
    // Recorremos las lineas del fichero
    while (fgets(linea, sizeof(linea), file)) {
        // Limpiar salto
        linea[strcspn(linea, "\r\n")] = 0;

        // Buscar si termina en " Wi-Fi:"
        size_t len_linea = strlen(linea);
        size_t len_sufijo = strlen(sufijo_esperado);
        if (len_linea >= len_sufijo && strcmp(linea + len_linea - len_sufijo, sufijo_esperado) == 0)
        {
            dentro_bloque = 1;
        }

        if (dentro_bloque) {
            if (strstr(linea, "Direcci") && strstr(linea, "IPv4")) {
                char* ptr = strchr(linea, ':');
                if (ptr) strncpy(config->ip, ptr + 1, MAX_LINEA);
            }
            else if (strstr(linea, "scara de subred") || strstr(linea, "Máscara")) {
                char* ptr = strchr(linea, ':');
                if (ptr) strncpy(config->mascara, ptr + 1, MAX_LINEA);
            }
            else if (strstr(linea, "Puerta de enlace")) {
                char* ptr = strchr(linea, ':');
                if (ptr) strncpy(config->gateway, ptr + 1, MAX_LINEA);
            }
            else if (strstr(linea, "Servidores DNS")) {
                char* ptr = strchr(linea, ':');
                if (ptr) strncpy(config->dns, ptr + 1, MAX_LINEA);
                
            }
        }
    }

    fclose(file);
    remove("temp_adaptador.txt");

    limpiar_espacios(config->ip);
    limpiar_espacios(config->mascara);
    limpiar_espacios(config->gateway);
    limpiar_espacios(config->dns);


}

