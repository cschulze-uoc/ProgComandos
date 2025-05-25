#include "utilXsl.h"

void flujo_crear_xsl(const char* xml_path, const char* xsl_path) {
	// Crea el fichero xsl (si ya existe lo sobreescribe)
    printf("Creando el fichero %s...\n", xsl_path);
    FILE* xsl = fopen(xsl_path, "w");
    if (!xsl) {
        perror("No se pudo crear el archivo XSL");
        return;
    }

    fprintf(xsl,
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<xsl:stylesheet version=\"1.0\" xmlns:xsl=\"http://www.w3.org/1999/XSL/Transform\">\n"
        "<xsl:output method=\"html\" encoding=\"UTF-8\" indent=\"yes\"/>\n"
        "<xsl:template match=\"/\">\n"
        "<html>\n"
        "<head>\n"
        "  <title>Configuración de Red</title>\n"
        "  <style>\n"
        "    body {\n"
        "      font-family: 'Segoe UI', sans-serif;\n"
        "      margin: 40px;\n"
        "      background-color: #f4f6f8;\n"
        "      color: #333;\n"
        "    }\n"
        "    h2 {\n"
        "      color: #004080;\n"
        "      margin-bottom: 10px;\n"
        "    }\n"
        "    table {\n"
        "      border-collapse: collapse;\n"
        "      width: 600px;\n"
        "      box-shadow: 0 2px 10px rgba(0,0,0,0.1);\n"
        "      background-color: #fff;\n"
        "    }\n"
        "    th, td {\n"
        "      border: 1px solid #ccc;\n"
        "      padding: 12px;\n"
        "      text-align: left;\n"
        "    }\n"
        "    th {\n"
        "      background-color: #004080;\n"
        "      color: white;\n"
        "    }\n"
        "    tr:nth-child(even) {\n"
        "      background-color: #f2f2f2;\n"
        "    }\n"
        "    ol {\n"
        "      margin-top: 20px;\n"
        "      background-color: #fff;\n"
        "      padding: 30px;\n"
        "      border: 1px solid #ccc;\n"
        "      width: 540px;\n"
        "    }\n"
        "  </style>\n"
        "</head>\n"
        "<body>\n"
        "<h2>Configuración de Red</h2>\n"
        "<table>\n"
        "<tr><th>Campo</th><th>Valor</th></tr>\n"
        "<tr><td>Adaptador</td><td><xsl:value-of select=\"Configuracion/Adaptador\"/></td></tr>\n"
        "<tr><td>IP</td><td><xsl:value-of select=\"Configuracion/IP\"/></td></tr>\n"
        "<tr><td>Máscara</td><td><xsl:value-of select=\"Configuracion/Mascara\"/></td></tr>\n"
        "<tr><td>Puerta de Enlace</td><td><xsl:value-of select=\"Configuracion/PuertaDeEnlace\"/></td></tr>\n"
        "<tr><td>DNS</td><td><xsl:value-of select=\"Configuracion/DNS\"/></td></tr>\n"
        "<tr><td>Latencia DNS</td><td><xsl:value-of select=\"Configuracion/LatenciaDNS\"/> ms</td></tr>\n"
        "</table>\n"
        "<h2>Saltos hasta el DNS</h2>\n"
        "<ol>\n"
        "<xsl:for-each select=\"Configuracion/SaltosDNS/Salto\">\n"
        "<li><xsl:value-of select=\"@ip\"/></li>\n"
        "</xsl:for-each>\n"
        "</ol>\n"
        "</body>\n"
        "</html>\n"
        "</xsl:template>\n"
        "</xsl:stylesheet>\n");
    fclose(xsl);
    printf("Fichero creado\n");
    
    printf("Comprobando fichero %s...\n", xml_path);

    // Leemos el fichero XML
    FILE* original = fopen(xml_path, "r");
    if (!original) {
        perror("No se pudo abrir el XML para leer");
        return;
    }

    char temp_path[256] = "configuracion_temp.xml";
    FILE* temp = fopen(temp_path, "w");
    if (!temp) {
        perror("No se pudo abrir archivo temporal XML");
        fclose(original);
        return;
    }

    char linea[512];
    int stylesheet_insertado = 0;

    while (fgets(linea, sizeof(linea), original)) {
        fputs(linea, temp);
        if (!stylesheet_insertado && strstr(linea, "<?xml")) {
			// Insertar la referencia al XSL justo después de la declaración XML
            printf("Referenciando XSL en XML...\n");

            fprintf(temp, "<?xml-stylesheet type=\"text/xsl\" href=\"%s\"?>\n", xsl_path);
            stylesheet_insertado = 1;
        }
    }

    fclose(original);
    fclose(temp);

    // Reemplazar el XML original
    remove(xml_path);
    rename(temp_path, xml_path);
    printf("Terminado %s...\n", xml_path);
    printf("Abra el fichero %s en un navegador para ver los estilos\n", xml_path);

}