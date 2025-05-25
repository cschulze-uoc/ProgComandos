#ifndef UTIL_XML_H
#define UTIL_XML_H

#define _CRT_SECURE_NO_WARNINGS
#define FICHERO_CONFIGURACION "configuracion.xml"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "utilAdaptador.h"
#include "utilDns.h"
#include "utilSalida.h"



void flujo_crear_xml();
void crear_xml(ConfigRed* config);
#endif