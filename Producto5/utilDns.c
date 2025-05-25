#define _CRT_SECURE_NO_WARNINGS

#include "utilDns.h"



void dnsFlow() {
    char ruta_dns[MAX_LINEA];
    preguntar_y_mostrar_dns(ruta_dns);
	
    char dns_actual[DNS_IP_LEN];
	char nombre_adaptador[MAX_LINEA];
    preguntar_adaptador_y_mostrar_dns_actuales(nombre_adaptador, dns_actual);
    
    comprobar_y_guardar_dns_validos(ruta_dns, "Dnss_Accesibles.txt");
    
    char mejor_dns[MAX_LINEA];
    encontrar_dns_mas_rapido("Dnss_Accesibles.txt", mejor_dns);

	verificar_y_cambiar_dns_si_necesario(nombre_adaptador, dns_actual, mejor_dns);
}

// Preguntar el nombre y la ruta del archivo dnsips.txt y mostrar  el contenido de este archivo por pantalla.
void preguntar_y_mostrar_dns(char* ruta_dns) {
    char dns[MAX_DNS][DNS_IP_LEN];

    printf("Introduce la ruta del archivo con servidores DNS (por ejemplo: dnsips.txt): ");
    fgets(ruta_dns, MAX_LINEA, stdin);
    ruta_dns[strcspn(ruta_dns, "\r\n")] = 0; // eliminar salto de línea

    printf("\n=== Servidores DNS encontrados ===\n");
    int total = leer_dns_desde_archivo(ruta_dns, dns);
    if (total == 0) {
        printf("No se pudieron leer direcciones DNS.\n");
    }
    else {
        printf("\nTotal de DNS leídos: %d\n", total);
    }
}

// Pregunta al usuario el adaptador y muestra los servidores DNS configurados
void preguntar_adaptador_y_mostrar_dns_actuales(char* nombre_adaptador, char* dns_actual) {
    // seleccionar_adaptador(nombre_adaptador);
    seleccionar_adaptador_netsh(nombre_adaptador);

    mostrar_dns_actuales(nombre_adaptador, "info_dns_adaptador.txt", dns_actual);
}

void mostrar_dns_actuales(const char* nombre_netsh, const char* ruta_salida, char* dns_principal) {
    system("ipconfig /all > temp_dns.txt");

    FILE* temp = fopen("temp_dns.txt", "r");
    FILE* salida = fopen(ruta_salida, "w");
    char linea[MAX_LINEA];
    int mostrar = 0, capturado = 0;

    if (!temp || !salida) {
        perror("Error al abrir archivos");
        strcpy(dns_principal, "");
        return;
    }

    // Construimos el encabezado esperado (por ejemplo: "Adaptador de LAN inalámbrica Wi-Fi:")
    char sufijo_esperado[MAX_LINEA];
    snprintf(sufijo_esperado, sizeof(sufijo_esperado), " %s:", nombre_netsh);

    print_fichero_consola(salida, 1, "===================================================\n");
    print_fichero_consola(salida, 3, "DNS DEL ADAPTADOR: ", nombre_netsh, "\n");
    print_fichero_consola(salida, 1, "===================================================\n");

    while (fgets(linea, sizeof(linea), temp)) {
        // Limpiar salto de línea
        linea[strcspn(linea, "\r\n")] = 0;

        // Buscar si termina en " Wi-Fi:"
        size_t len_linea = strlen(linea);
        size_t len_sufijo = strlen(sufijo_esperado);
        if (len_linea >= len_sufijo && strcmp(linea + len_linea - len_sufijo, sufijo_esperado) == 0) 
        {
            mostrar = 1;
        }

        if (mostrar) {
            if (strstr(linea, "Servidores DNS") || strstr(linea, "Servidor DNS") || strstr(linea, "DNS Servers")) {
                print_fichero_consola(salida, 1, linea);

                if (!capturado) {
                    char* ptr = linea;
                    while (*ptr) {
                        if (*ptr >= '0' && *ptr <= '9') {
                            strncpy(dns_principal, ptr, DNS_IP_LEN);
                            dns_principal[strcspn(dns_principal, "\r\n ")] = 0;
                            capturado = 1;
                            break;
                        }
                        ptr++;
                    }
                }
            }
            else if ((strstr(linea, "Adaptador") || strstr(linea, "adapter")) && !strstr(linea, nombre_netsh)) {
                break; // hemos llegado a otro adaptador
            }
        }
    }

    print_fichero_consola(salida, 1, "\n===================================================\n");

    fclose(temp);
    fclose(salida);
    remove("temp_dns.txt");
}



// Lee las IPs del archivo dnsips.txt
int leer_dns_desde_archivo(const char* ruta_archivo, char dns[MAX_DNS][DNS_IP_LEN]) {
    FILE* archivo = fopen(ruta_archivo, "r");
    if (!archivo) {
        perror("No se pudo abrir el archivo de DNS");
        return 0;
    }

    int count = 0;
    while (count < MAX_DNS && fgets(dns[count], DNS_IP_LEN, archivo)) {
        dns[count][strcspn(dns[count], "\r\n")] = 0; // quitar saltos
        printf(" - %s\n", dns[count]);
        count++;
    }

    fclose(archivo);
    return count;
}

// Guarda en un nuevo archivo las DNS accesibles
int guardar_dns_accesibles(const char* ruta_entrada, const char* ruta_salida) {
    FILE* entrada = fopen(ruta_entrada, "r");
    FILE* salida = fopen(ruta_salida, "w");
    char ip[DNS_IP_LEN];
    int count = 0;

    if (!entrada || !salida) {
        perror("Error al abrir archivos DNS");
        return 0;
    }

    while (fgets(ip, sizeof(ip), entrada)) {
        ip[strcspn(ip, "\r\n")] = 0;
        char comando[128];
        snprintf(comando, sizeof(comando), "ping -n 1 %s > nul", ip);
        int ok = system(comando);
        if (ok == 0) {
            fprintf(salida, "%s\n", ip);
            printf("%s responde\n", ip);
            count++;
        }
        else {
            printf("%s no responde\n", ip);
        }
    }

    fclose(entrada);
    fclose(salida);
    return count;
}

// Lee DNS desde un archivo, hace ping a cada uno y guarda los accesibles en un archivo temporal
void comprobar_y_guardar_dns_validos(const char* ruta_entrada, const char* ruta_salida) {
    FILE* entrada = fopen(ruta_entrada, "r");
    FILE* salida = fopen(ruta_salida, "w");
    char ip[DNS_IP_LEN];
    int total = 0, validos = 0;

    if (!entrada || !salida) {
        perror("Error al abrir archivos de DNS");
        return;
    }

    printf("\nComprobando accesibilidad de servidores DNS...\n");

    while (fgets(ip, sizeof(ip), entrada)) {
        ip[strcspn(ip, "\r\n")] = 0; // Quitar salto de línea

        if (strlen(ip) == 0) continue; // Saltar líneas vacías

        char comando[128];
        snprintf(comando, sizeof(comando), "ping -n 1 %s > nul", ip);

        int resultado = system(comando);
        total++;

        if (resultado == 0) {
            fprintf(salida, "%s\n", ip);
            printf("%s responde\n", ip);
            validos++;
        }
        else {
            printf("%s no responde\n", ip);
        }
    }

    fclose(entrada);
    fclose(salida);

    printf("\nTotal de DNS probados: %d\n", total);
    printf("DNS válidos encontrados: %d (guardados en %s)\n", validos, ruta_salida);
}


// Compara latencia de los DNS válidos y devuelve el mejor
void encontrar_dns_mas_rapido(const char* ruta_validos, char* mejor_dns) {
    FILE* archivo = fopen(ruta_validos, "r");
    if (!archivo) {
        perror("No se pudo abrir el archivo de DNS válidos");
        strcpy(mejor_dns, "");
        return;
    }

    char ip[DNS_IP_LEN];
    int mejor_tiempo = 9999;
    char candidatos[MAX_DNS][DNS_IP_LEN];
    int total_candidatos = 0;

    printf("\nMedición de velocidad de DNS:\n");

    while (fgets(ip, sizeof(ip), archivo)) {
        ip[strcspn(ip, "\r\n")] = 0;
        if (strlen(ip) == 0) continue;

        int tiempo = medir_ping_ms(ip);
        printf(" - %s: %d ms\n", ip, tiempo);

        if (tiempo < mejor_tiempo) {
            // Nueva mejor latencia → reiniciar candidatos
            mejor_tiempo = tiempo;
            strcpy(candidatos[0], ip);
            total_candidatos = 1;
        }
        else if (tiempo == mejor_tiempo) {
            // Mismo tiempo que el mejor → añadir a candidatos
            if (total_candidatos < MAX_DNS) {
                strcpy(candidatos[total_candidatos], ip);
                total_candidatos++;
            }
        }
    }

    fclose(archivo);

    if (total_candidatos == 0) {
        printf("No se encontró un DNS válido.\n");
        strcpy(mejor_dns, "");
        return;
    }

    // Si solo hay uno, es el mejor
    if (total_candidatos == 1) {
        strcpy(mejor_dns, candidatos[0]);
        printf("\nEl DNS más rápido es: %s con %d ms\n", mejor_dns, mejor_tiempo);
        return;
    }

    // Si hay mas de un candidato, comparar por número de saltos
    printf("\nHay %d candidatos con %d ms\n", total_candidatos, mejor_tiempo);
    int min_saltos = 99;
    for (int i = 0; i < total_candidatos; i++) {
        int saltos = contar_saltos(candidatos[i]);
        printf("   - %s tiene %d saltos\n", candidatos[i], saltos);
        if (saltos < min_saltos) {
            min_saltos = saltos;
            strcpy(mejor_dns, candidatos[i]);
        }
    }

    printf("\nEl DNS más rápido (y cercano) es: %s con %d ms y %d saltos\n", mejor_dns, mejor_tiempo, min_saltos);
}


// Hace ping a una IP y mide el tiempo de respuesta en milisegundos
int medir_ping_ms(const char* ip) {
    char comando[128];
    snprintf(comando, sizeof(comando), "ping -n 1 -w 1000 %s > ping_temp.txt", ip);

    system(comando);

    FILE* ping_output = fopen("ping_temp.txt", "r");
    if (!ping_output) {
        perror("No se pudo abrir ping_temp.txt");
        return 9999; // Si hay un error asignamos un valor muy alto para que no gane
    }

    char linea[MAX_LINEA];
    int tiempo_ms = 9999;

    while (fgets(linea, sizeof(linea), ping_output)) {
        char* pos = strstr(linea, "tiempo=");
        int offset = 7;
        if (!pos) {
            pos = strstr(linea, "time=");
            offset = 5;
        }

        if (pos) {
            pos += offset;
            tiempo_ms = atoi(pos);
            break;
        }
    }

    fclose(ping_output);
    remove("ping_temp.txt");
    return tiempo_ms;
}

int medir_ping_media_ms(const char* ip) {
    char comando[128];
    snprintf(comando, sizeof(comando), "ping -n 4 -w 1000 %s > ping_temp.txt", ip);

    system(comando);

    FILE* file = fopen("ping_temp.txt", "r");
    if (!file) {
        perror("No se pudo abrir ping_temp.txt");
        return 9999;
    }

    char linea[256];
    int media = 9999;

    while (fgets(linea, sizeof(linea), file)) {
        if (strstr(linea, "Media =") || strstr(linea, "Average =")) {
            char* ptr = strstr(linea, "Media =");
            if (!ptr) ptr = strstr(linea, "Average =");
            if (ptr) {
                ptr += 8; // saltar "Media ="
                while (*ptr && (*ptr < '0' || *ptr > '9')) ptr++;
                media = atoi(ptr);
                break;
            }
        }
    }

    fclose(file);
    remove("ping_temp.txt");
    return media;
}

int listar_saltos(const char* ip_destino, char saltos[MAX_SALTOS][SALTO_IP_LEN])
{
    char comando[128];
    snprintf(comando, sizeof(comando), "tracert -d -h %d -w 100 %s > tracert_temp.txt", MAX_SALTOS, ip_destino);
    system(comando);

    FILE* archivo = fopen("tracert_temp.txt", "r");
    if (!archivo) {
        perror("No se pudo abrir tracert_temp.txt");
        return 0;
    }

    char linea[256];
    int count = 0;

    while (fgets(linea, sizeof(linea), archivo) && count < MAX_SALTOS) {
        char* ptr = linea;
        while (*ptr == ' ' || *ptr == '\t') ptr++;

        // Comprobar si comienza con número
        if (*ptr >= '0' && *ptr <= '9') {
            int salto_registrado = 0;

            // Buscar tokens con pinta de IP
            char* token = strtok(ptr, " \t");
            while (token) {
                int dots = 0;
                int valid = 1;
                for (int i = 0; token[i]; i++) {
                    if (token[i] == '.') dots++;
                    else if (token[i] < '0' || token[i] > '9') {
                        valid = 0;
                        break;
                    }
                }

                if (dots == 3 && valid && strlen(token) < SALTO_IP_LEN) {
                    strncpy(saltos[count], token, SALTO_IP_LEN);
                    saltos[count][SALTO_IP_LEN - 1] = '\0';
                    salto_registrado = 1;
                    break;
                }

                token = strtok(NULL, " \t");
            }

            if (!salto_registrado) {
                strncpy(saltos[count], "No disponible", SALTO_IP_LEN);
                saltos[count][SALTO_IP_LEN - 1] = '\0';
            }

            count++;
        }
    }

    fclose(archivo);
    remove("tracert_temp.txt");
    return count;
}

// Cuenta cuántos saltos hay hasta una IP usando tracert
int contar_saltos(const char* ip) {
    char comando[128];
    snprintf(comando, sizeof(comando), "tracert -h 10 -w 100 %s > tracert_temp.txt", ip);
    system(comando);

    FILE* traza = fopen("tracert_temp.txt", "r");
    if (!traza) {
        perror("No se pudo abrir tracert_temp.txt");
        return 99; // Número alto si falla
    }

    char linea[MAX_LINEA];
    int saltos = 0;

    while (fgets(linea, sizeof(linea), traza)) {
        // Saltar espacios al principio
        char* ptr = linea;
        while (*ptr == ' ' || *ptr == '\t') ptr++;

        // Si comienza con un número, es un salto
        if (*ptr >= '0' && *ptr <= '9') {
            saltos++;
        }
    }

    fclose(traza);
    remove("tracert_temp.txt");
    return saltos;
}

// Cambia el DNS de un adaptador usando netsh
void cambiar_dns_con_netsh(const char* adaptador, const char* dns) {
    char comando[256];
    snprintf(comando, sizeof(comando),
        "netsh interface ip set dns name=\"%s\" static %s",
        adaptador, dns);
    printf("Ejecutando comando:\n%s\n", comando);
    int r = system(comando);
    if (r == 0) {
        printf("DNS cambiado correctamente a %s en el adaptador '%s'\n", dns, adaptador);
    }
    else {
        printf("Error al cambiar el DNS con netsh.\n");
    }
}

// Verifica si el DNS actual es diferente al óptimo y lo cambia si es necesario
void verificar_y_cambiar_dns_si_necesario(const char* nombre_adaptador, const char* dns_actual, const char* dns_rapido) {
    printf("\nVerificando si es necesario cambiar el DNS...\n");
    printf("  DNS actual:  %s\n", dns_actual);
    printf("  DNS óptimo:  %s\n", dns_rapido);

    if (strcmp(dns_actual, dns_rapido) != 0) {
        printf("El DNS más rápido no está configurado. Se procederá a cambiarlo.\n");
        cambiar_dns_con_netsh(nombre_adaptador, dns_rapido);
    }
    else {
        printf("El DNS más rápido ya está configurado. No es necesario cambiarlo.\n");
    }
}