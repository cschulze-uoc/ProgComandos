#ifndef UTIL_DNS_H
#define UTIL_DNS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "utilAdaptador.h"

#define MAX_DNS 20
#define DNS_IP_LEN 32
#define DNS_IP_LEN 32

// Función principal que maneja el flujo de optimización de DNS
void dnsFlow();

// Preguntar el nombre y la ruta del archivo dnsips.txt y mostrar  el contenido de este archivo por pantalla
void preguntar_y_mostrar_dns();

//Preguntar al usuario el adaptador de red que quiere modificar y mostrar los servidores DNS configurados en este.
void preguntar_adaptador_y_mostrar_dns_actuales();

// mostrar los servidores DNS configurados en este adaptador
void mostrar_dns_actuales(const char* nombre_adaptador, const char* ruta_salida, char* dns_principal);

// Lee las IPs del archivo de texto dnsips.txt
// Devuelve el número de IPs leídas y las guarda en el array dns
int leer_dns_desde_archivo(const char* ruta_archivo, char dns[MAX_DNS][DNS_IP_LEN]);

// Lee DNS desde un archivo, hace ping a cada uno y guarda los accesibles en un archivo temporal
void comprobar_y_guardar_dns_validos(const char* ruta_entrada, const char* ruta_salida);

// Guarda las DNS que responden al ping en un archivo temporal
// Devuelve el número de IPs accesibles
int guardar_dns_accesibles(const char* ruta_entrada, const char* ruta_salida);

// Compara latencia de los DNS válidos y devuelve el mejor
void encontrar_dns_mas_rapido(const char* ruta_validos, char* mejor_dns);

// Cuenta cuántos saltos hay hasta una IP usando tracert
int contar_saltos(const char* ip);

// Guarda los saltos que devuelve tracert hasta una ip
int listar_saltos(const char* ip_destino, char saltos[MAX_SALTOS][SALTO_IP_LEN]);

// Cambia el DNS de un adaptador usando netsh si es mas rápido que el actual
void verificar_y_cambiar_dns_si_necesario(const char* nombre_adaptador, const char* dns_actual, const char* dns_rapido);

int medir_ping_media_ms(const char* ip);

#endif
