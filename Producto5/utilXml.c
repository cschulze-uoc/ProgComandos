#include "utilXml.h"


#define MAX_LINEA 256

void flujo_crear_xml() {
	char nombre_adaptador[MAX_LINEA];
	seleccionar_adaptador_netsh(nombre_adaptador);
	
	ConfigRed config;
	init_configRed(&config, nombre_adaptador);

	printf("Extrayendo datos de ipconfig...");
	extraer_info_adaptador(nombre_adaptador, &config);

	printf("Ok\nMidiendo tiempo de latencia del dns...");
	config.latencia_dns = medir_ping_media_ms(config.dns);

	printf("Ok\nMidiendo saltos hasta el dns...");
	config.num_saltos = listar_saltos(config.dns, config.saltos);

	printf("Ok\nCreando documento XML...");
	crear_xml(&config);

	printf("Ok\n\n-----------------Finalizado!-----------------\n");
	imprimir_archivo_en_pantalla(FICHERO_CONFIGURACION);
}

// Crea un archivo XML con el contenido de la estructura ConfigRed
void crear_xml(ConfigRed *config) {
	FILE* xml = fopen(FICHERO_CONFIGURACION, "w");
	if (!xml) {
		perror("Error al abrir el archivo XML");
		return;
	}
	fprintf(xml, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
	fprintf(xml, "<Configuracion>\n");
	fprintf(xml, "  <Adaptador>%s</Adaptador>\n", config->nombre);
	fprintf(xml, "  <IP>%s</IP>\n", config->ip);
	fprintf(xml, "  <Mascara>%s</Mascara>\n", config->mascara);
	fprintf(xml, "  <PuertaDeEnlace>%s</PuertaDeEnlace>\n", config->gateway);
	fprintf(xml, "  <DNS>%s</DNS>\n", config->dns);
	fprintf(xml, "  <LatenciaDNS>%d</LatenciaDNS>\n", config->latencia_dns);

	fprintf(xml, "  <SaltosDNS cantidad=\"%d\">\n", config->num_saltos);
	for (int i = 0; i < config->num_saltos; i++) {
		fprintf(xml, "    <Salto ip=\"%s\" />\n", config->saltos[i]);
	}
	fprintf(xml, "  </SaltosDNS>\n");
	fprintf(xml, "</Configuracion>\n");
	fclose(xml);
}