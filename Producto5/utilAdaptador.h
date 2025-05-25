#ifndef UTIL_ADAPTADOR_H
#define UTIL_ADAPTADOR_H

#define MAX_ADAPTADORES 20 //Número máximo de adaptadores de red que se buscarán en el sistema
#define MAX_LINEA 256 //Número máximo de carácteres en cada línea

#define MAX_SALTOS 30
#define SALTO_IP_LEN 64


// Estructura para almacenar la configuración de red
typedef struct {
    char nombre[MAX_LINEA];
    char ip[MAX_LINEA];
    char mascara[MAX_LINEA];
    char gateway[MAX_LINEA];
    char dns[MAX_LINEA];
    int latencia_dns;
    char saltos[MAX_SALTOS][SALTO_IP_LEN];
    int num_saltos;
} ConfigRed;

void init_configRed(ConfigRed* config, char* nombre);

void mostrar_info_adaptador(const char* nombre_adaptador, const char* ruta_salida);
void seleccionar_adaptador(char* destino);
void seleccionar_y_mostrar_adaptador(const char* ruta_salida);
void seleccionar_adaptador_netsh(char* destino);
void extraer_info_adaptador(const char* nombre_netsh, ConfigRed* config);
#endif