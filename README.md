# Proyecto FP.047 - Comandos personalizados para sistema operativo

Este repositorio contiene el código fuente desarrollado como parte del proyecto final de la asignatura **"Programa comandos personalizados para sistema operativo"** (FP.047), impartida en la UOC.

## Descripción del proyecto

A través de cinco productos progresivos, se han desarrollado herramientas de consola en lenguaje **C** orientadas a automatizar tareas del sistema operativo Windows, especialmente en relación con la configuración de red y la generación de informes.

El proyecto combina el uso de comandos del sistema (`ipconfig`, `ping`, `netsh`, `tracert`) con técnicas de programación modular y la generación de documentos estructurados (XML/XSLT).

## Funcionalidades principales

- Inserción automática de fecha y hora en ficheros.
- Escaneo de direcciones IP y verificación mediante ping.
- Extracción y visualización de configuraciones de adaptadores de red.
- Detección del servidor DNS más rápido (latencia y saltos).
- Reconfiguración automática del DNS con `netsh`.
- Generación de informe XML con IP, máscara, puerta de enlace y DNS.
- Transformación del XML a HTML mediante una hoja de estilo **XSLT**.

## Estructura del repositorio

- `main.c` — Punto de entrada del programa, con menú principal.
- `utilHora.c/h` — Funciones para insertar fecha/hora.
- `utilIpTest.c/h` — Funciones para testeo de IPs.
- `utilAdaptador.c/h` — Funciones para mostrar/configurar adaptadores.
- `utilDns.c/h` — Funciones para comparar y cambiar DNS.
- `xml_utils.c/h` — Funciones para generar el archivo XML y la hoja XSLT.
- `configuracion.xml` — Archivo generado con la configuración de red.
- `transformacion.xsl` — Hoja de estilo para visualizar el XML en navegador.

## Requisitos

- Windows con soporte para los comandos `ipconfig`, `ping`, `tracert` y `netsh`.
- Visual Studio Community (recomendado por la asignatura).
- Navegador con soporte para transformación XSLT (o servidor como IIS).

## Cómo ejecutar

1. Abre el proyecto en Visual Studio.
2. Compila y ejecuta el programa.
3. Usa el menú para seleccionar funcionalidades (insertar hora, analizar IPs, generar XML...).
4. Para visualizar el XML transformado, abre `configuracion.xml` desde un servidor web (por ejemplo, IIS) con `transformacion.xsl` en la misma carpeta.

## Autor

**Carlos Schulze Ramos**  
Proyecto realizado como parte del módulo FP.047 — UOC

---
