<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<xsl:output method="html" encoding="UTF-8" indent="yes"/>
<xsl:template match="/">
<html>
<head>
  <title>Configuración de Red</title>
  <style>
    body {
      font-family: 'Segoe UI', sans-serif;
      margin: 40px;
      background-color: #f4f6f8;
      color: #333;
    }
    h2 {
      color: #004080;
      margin-bottom: 10px;
    }
    table {
      border-collapse: collapse;
      width: 600px;
      box-shadow: 0 2px 10px rgba(0,0,0,0.1);
      background-color: #fff;
    }
    th, td {
      border: 1px solid #ccc;
      padding: 12px;
      text-align: left;
    }
    th {
      background-color: #004080;
      color: white;
    }
    tr:nth-child(even) {
      background-color: #f2f2f2;
    }
    ol {
      margin-top: 20px;
      background-color: #fff;
      padding: 30px;
      border: 1px solid #ccc;
      width: 540px;
    }
  </style>
</head>
<body>
<h2>Configuración de Red</h2>
<table>
<tr><th>Campo</th><th>Valor</th></tr>
<tr><td>Adaptador</td><td><xsl:value-of select="Configuracion/Adaptador"/></td></tr>
<tr><td>IP</td><td><xsl:value-of select="Configuracion/IP"/></td></tr>
<tr><td>Máscara</td><td><xsl:value-of select="Configuracion/Mascara"/></td></tr>
<tr><td>Puerta de Enlace</td><td><xsl:value-of select="Configuracion/PuertaDeEnlace"/></td></tr>
<tr><td>DNS</td><td><xsl:value-of select="Configuracion/DNS"/></td></tr>
<tr><td>Latencia DNS</td><td><xsl:value-of select="Configuracion/LatenciaDNS"/> ms</td></tr>
</table>
<h2>Saltos hasta el DNS</h2>
<ol>
<xsl:for-each select="Configuracion/SaltosDNS/Salto">
<li><xsl:value-of select="@ip"/></li>
</xsl:for-each>
</ol>
</body>
</html>
</xsl:template>
</xsl:stylesheet>
