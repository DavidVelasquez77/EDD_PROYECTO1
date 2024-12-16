# Manual Técnico - Sistema de Renta de Activos

## 1. Entorno de Desarrollo

### 1.1 Requisitos del Sistema
- **Sistema Operativo**: Windows 10/11 (64 bits)
- **Lenguaje de Programación**: C++
- **IDE**: Visual Studio Code
- **Herramientas**: MSYS2

### 1.2 Dependencias y Herramientas
- **Compilador**: MinGW-w64 (a través de MSYS2)
- **Graphviz**: Herramienta para generación de reportes gráficos
- **Visual Studio Code**: Entorno de desarrollo
- **Extensiones VS Code**: 
  - C/C++ Extension Pack
  - Code Runner

### 1.3 Instalación de Dependencias

#### 1.3.1 Instalación de MSYS2
1. Descargar MSYS2:
   - Visitar: https://www.msys2.org/
   - Descargar instalador para Windows (64 bits)

2. Pasos de instalación:
   ```bash
   # Abrir MSYS2 MinGW 64-bit después de la instalación
   # Actualizar sistema base
   pacman -Syu

   # Cerrar y volver a abrir la terminal después de la actualización

   # Instalar herramientas de desarrollo de C++
   pacman -S mingw-w64-x86_64-toolchain
   pacman -S mingw-w64-x86_64-gcc
   pacman -S mingw-w64-x86_64-make
   ```

3. Configurar Variables de Entorno en Windows:
   - Agregar rutas al PATH:
     - `C:\msys64\mingw64\bin`
     - `C:\msys64\usr\bin`

#### 1.3.2 Visual Studio Code
1. Descargar VS Code:
   - Visitar: https://code.visualstudio.com/
   - Descargar e instalar versión para Windows

2. Instalar Extensiones:
   - Abrir VS Code
   - Ir a Extensiones (Ctrl+Shift+X)
   - Buscar e instalar:
     - "C/C++ Extension Pack"
     - "Code Runner"

#### 1.3.3 Instalación de Graphviz
1. Descargar Graphviz:
   - Visitar: https://graphviz.org/download/
   - Descargar versión para Windows (64 bits)

2. Instalación con MSYS2:
   ```bash
   # En terminal MSYS2
   pacman -S mingw-w64-x86_64-graphviz
   ```

### 1.4 Configuración de VS Code para C++

#### settings.json
```json
{
    "files.associations": {
        "*.cpp": "cpp"
    },
    "C_Cpp.default.compilerPath": "C:\\msys64\\mingw64\\bin\\g++.exe",
    "C_Cpp.default.intelliSenseMode": "gcc-x64"
}
```


### 1.5 Compilación del Proyecto
```bash
# En terminal MSYS2 o terminal integrada de VS Code
g++ -o app app.cpp
```

### 1.6 Ejecución
- Dentro de VS Code: Usar extensión Code Runner
- En terminal MSYS2: `./app.exe`

## 2. Descripción de código

## **Matriz Dispersa**: 
La Matriz Dispersa es una representación eficiente de matrices con muchos valores nulos o repetitivos (es decir, matrices donde solo un pequeño porcentaje de las posiciones tiene datos significativos). En lugar de almacenar todos los valores, la matriz dispersa únicamente guarda las posiciones y valores relevantes, ahorrando espacio y mejorando la eficiencia en ciertas operaciones.
## **Estructura `NodoMatriz`**
Esta estructura representa un nodo de la matriz dispersa. Cada nodo puede ser un usuario, un departamento o una empresa dependiendo de su ubicación en la matriz. Los campos principales son:

- **`nombreUsuario` y `contrasena`**: Identifican al usuario si el nodo es un nodo de usuario.
- **`departamento` y `empresa`**: Identifican el departamento y la empresa asociados al nodo.
- **`arbolAVL`**: Apunta a un árbol AVL para gestionar activos relacionados con el usuario.
- **`derecha` y `abajo`**: Punteros para navegar en la matriz dispersa, permitiendo la estructura de filas y columnas.

---

## **Clase `MatrizDispersa`**
La clase `MatrizDispersa` gestiona la construcción y operación de la matriz dispersa. Incluye las siguientes funcionalidades:

### **Constructor**
- Inicializa la matriz dispersa con un puntero a la `cabeza` en `nullptr`.

---

### **Métodos de la Clase**

#### **`insertarUsuario`**
- Inserta un usuario en la matriz dispersa:
  1. Busca o crea encabezados para el departamento (horizontal) y empresa (vertical).
  2. Verifica que el usuario no exista en la combinación de departamento y empresa.
  3. Inserta un nuevo nodo usuario bajo el nodo de la empresa.

#### **`buscarUsuario`**
- Busca un nodo usuario en la matriz usando:
  - Nombre del departamento.
  - Nombre de la empresa.
  - Nombre del usuario.

#### **`obtenerEncabezadosHorizontales`**
- Imprime los nombres de los departamentos registrados como encabezados horizontales.

#### **`obtenerGrupoDepartamento`**
- Calcula la posición de un departamento dentro de la matriz para fines de generación de visualización.

#### **`obtenerFilaEmpresa`**
- Calcula la posición de una empresa dentro de la matriz para fines de generación de visualización.

#### **`generarReporteGraphviz`**
- Genera un archivo `.dot` para representar gráficamente la matriz dispersa utilizando Graphviz:
  1. Crea nodos para el encabezado principal ("ADMIN").
  2. Genera encabezados para departamentos (horizontal) y empresas (vertical).
  3. Crea conexiones entre usuarios, empresas y departamentos.
  4. Agrupa nodos por filas y columnas según sus relaciones.

#### **`obtenerOInsertarEncabezado`**
- Busca o inserta un encabezado (horizontal o vertical):
  - Si el encabezado no existe, crea uno nuevo.
  - Inserta el nuevo encabezado en la posición correcta según orden alfabético.

#### **`buscarEncabezado`**
- Busca un encabezado (horizontal o vertical) en la matriz.


## **Árbol AVL**: 
Un Árbol AVL  es un árbol binario de búsqueda autobalanceado. Se asegura de que la diferencia de altura entre los subárboles izquierdo y derecho de cualquier nodo no sea mayor a 1, lo que garantiza un rendimiento eficiente para operaciones de búsqueda, inserción y eliminación.

## **Estructura `NodoAVL`**
Esta estructura representa un nodo en el árbol AVL para gestionar activos. Cada nodo contiene información sobre un activo y su estado en el árbol:

- **`idActivo` y `nombreActivo`**: Identifican el activo y su nombre.
- **`descripcion`**: Breve descripción del activo.
- **`tiempoMaximoRenta`**: Tiempo máximo permitido para rentar el activo.
- **`disponible`**: Indica si el activo está disponible para rentar.
- **`izquierda` y `derecha`**: Punteros a los nodos hijo izquierdo y derecho.
- **`altura`**: Altura del nodo, utilizada para calcular el balance del árbol.

---

## **Funciones y Métodos Relacionados**

### **`crearNodoAVL`**
- Crea y retorna un nuevo nodo AVL con los valores inicializados:
  - ID, nombre, descripción y tiempo máximo de renta del activo.
  - Disponibilidad inicial configurada como `true`.
  - Altura del nodo inicializada en 1.

---

### **Funciones Auxiliares**

#### **`obtenerAltura`**
- Retorna la altura de un nodo. Si el nodo es `nullptr`, devuelve 0.

#### **`obtenerBalance`**
- Calcula y retorna el factor de balance de un nodo:
  - Diferencia entre la altura del subárbol izquierdo y derecho.

#### **`rotarDerecha`**
- Realiza una rotación hacia la derecha para balancear el árbol:
  - Utilizada cuando el subárbol izquierdo tiene mayor altura.

#### **`rotarIzquierda`**
- Realiza una rotación hacia la izquierda para balancear el árbol:
  - Utilizada cuando el subárbol derecho tiene mayor altura.

---

### **Operaciones en el Árbol AVL**

#### **`insertarEnAVL`**
- Inserta un nuevo nodo en el árbol AVL:
  1. Compara el ID del activo para determinar la posición en el árbol.
  2. Ajusta la altura de los nodos afectados tras la inserción.
  3. Verifica el balance del árbol y aplica rotaciones si es necesario:
     - Rotación simple (izquierda o derecha).
     - Rotación doble (izquierda-derecha o derecha-izquierda).

#### **`encontrarMinimo`**
- Busca y retorna el nodo con el menor ID en el subárbol izquierdo:
  - Utilizado para encontrar sucesores al eliminar nodos.

#### **`eliminarNodoAVL`**
- Elimina un nodo del árbol AVL:
  1. Encuentra el nodo que coincide con el ID.
  2. Si el nodo tiene:
     - **Un hijo o ninguno**: Reemplaza el nodo con su hijo o `nullptr`.
     - **Dos hijos**: Reemplaza el nodo con su sucesor en inorden.
  3. Ajusta la altura y balance del árbol tras la eliminación.

---

### **Visualización y Búsqueda**

#### **`mostrarActivosDisponiblesAVL`**
- Recorre el árbol en inorden y muestra solo los activos disponibles:
  - Imprime información como ID, nombre, descripción y tiempo máximo de renta.

#### **`buscarActivoEnAVL`**
- Busca un activo en el árbol AVL por su ID:
  - Retorna el nodo si lo encuentra, o `nullptr` si no existe.

## **Lista Doblemente Circular Enlazada**  
Una lista doblemente circular enlazada es una estructura de datos donde cada nodo contiene un enlace al siguiente y al nodo anterior. En esta lista, el último nodo se conecta de vuelta al primero, formando un ciclo cerrado. Se utiliza para representar datos que pueden requerir recorridos circulares o en ambas direcciones.

## **Estructura `NodoTransaccion`**  
Esta estructura representa un nodo en la lista de transacciones. Cada nodo almacena información sobre una transacción específica:

- **`idTransaccion`**: Identificador único de la transacción.  
- **`idActivo`**: Identificador único del activo asociado.  
- **`activo`**: Nombre o descripción del activo.  
- **`usuario`**: Usuario que realizó la transacción.  
- **`departamento`**: Departamento al que pertenece el usuario.  
- **`empresa`**: Empresa asociada al usuario.  
- **`diasRenta`**: Número de días que el activo será rentado.  
- **`fechaRenta`**: Fecha en que se creó la transacción.  
- **`estadoTransaccion`**: Estado de la transacción (ejemplo: "Activa").  

---  

## **Clase `ListaTransacciones`**  
La clase implementa una lista doblemente circular enlazada para gestionar transacciones. Incluye métodos para insertar y mostrar transacciones:  

### **`insertarTransaccion`**  
- Inserta un nuevo nodo en la lista de forma ordenada según el `idTransaccion`.  
- Si la lista está vacía, el nuevo nodo se convierte en la cabeza y se enlaza consigo mismo.  
- Si ya hay nodos, encuentra la posición adecuada y ajusta los punteros del nodo actual, su anterior y el nuevo nodo.  
- Si el nodo se inserta antes de la cabeza, la cabeza se actualiza al nuevo nodo.  

### **`mostrarTransacciones`**  
- Recorre la lista desde la cabeza y muestra los datos de cada transacción:  
  - ID de transacción, ID del activo, usuario, departamento, empresa, días de renta y fecha de renta.  
- Si la lista está vacía, indica que no hay transacciones registradas.  

## **FUNCIONES y METODOS**: 
## **Función `generarSubArbolGraphvizConRaiz`**  
Esta función genera un subárbol en formato Graphviz a partir de un nodo de un árbol AVL, y escribe el resultado en un archivo. Es útil para visualizar la estructura del árbol y las relaciones entre nodos.  

### **Parámetros**  
- **`ofstream& archivo`**: Referencia al archivo de salida donde se escribe el código Graphviz.  
- **`string nombreRaiz`**: Nombre de la raíz del subárbol (generalmente el usuario o un identificador global).  
- **`NodoAVL* nodo`**: Puntero al nodo actual del subárbol que se está procesando.  
- **`string padre`** *(opcional)*: Nombre del nodo padre en la estructura Graphviz (por defecto, vacío para el nodo raíz).  

---

### **Funcionamiento**  

1. **Caso base**:  
   Si el nodo actual es `nullptr`, la función retorna sin realizar ninguna operación, lo que asegura que no se intente procesar nodos inexistentes.  

2. **Construcción del nodo**:  
   - Se define un nombre único para el nodo combinando el `nombreRaiz` con el `idActivo`.  
   - El color del nodo se decide según su disponibilidad:  
     - **Verde (`green`)**: Si el activo está disponible.  
     - **Rojo (`red`)**: Si el activo no está disponible.  
   - Se escribe el nodo en el archivo Graphviz con su etiqueta (`label`) que incluye:  
     - ID del activo.  
     - Nombre del activo.  
     - Descripción del activo.  
     - Tiempo máximo permitido para renta.  

3. **Conexión con el nodo padre**:  
   - Si no hay un nodo padre (es la raíz del subárbol), se conecta con el nodo raíz global (`nombreRaiz`).  
   - Si hay un nodo padre, se genera la conexión desde este al nodo actual.  

4. **Recursividad sobre los hijos**:  
   - Se realiza una llamada recursiva para procesar el hijo izquierdo del nodo.  
   - Luego, se procesa el hijo derecho, asegurando un recorrido **inorden** para construir el subárbol.  

---

## **Función `generarReporteGraphvizActivosDepartamento`**  
Esta función genera un archivo Graphviz que representa los activos asociados a los usuarios de un departamento específico en formato de árbol AVL. También convierte el archivo `.dot` en una imagen `.png` para visualización directa.

---

### **Parámetros**  
- **`string* usuarios`**: Arreglo de nombres de usuarios que se desea incluir en el reporte.  
- **`int cantidadUsuarios`**: Número de usuarios en el arreglo.  
- **`const string& departamento`**: Nombre del departamento que se está analizando.  

---

### **Funcionamiento**  

1. **Inicialización del archivo Graphviz**:  
   - Se crea y abre el archivo `reporte_activos_departamento.dot`.  
   - Si ocurre un error al abrirlo, se muestra un mensaje y se detiene la ejecución.  
   - Se configuran las propiedades del grafo:  
     - **Etiqueta**: Muestra el nombre del departamento en el encabezado.  
     - **Forma de nodos**: Rectángulos.  
     - **Dirección del grafo**: De arriba hacia abajo (*top-to-bottom*).  

2. **Búsqueda de usuarios en la matriz**:  
   - Se realiza un recorrido en la estructura de matriz dispersa para buscar los nodos correspondientes a cada usuario en el arreglo.  
   - Para cada usuario:
     - Se recorre la matriz por niveles:  
       1. Departamentos.  
       2. Empresas asociadas al departamento.  
       3. Usuarios dentro de las empresas.  
     - Si el usuario coincide con alguno de los nombres en el arreglo, se obtiene su nodo correspondiente.  

3. **Generación del subárbol**:  
   - Si el usuario tiene un árbol AVL asociado, se invoca la función `generarSubArbolGraphvizConRaiz` para incluir su representación en el archivo Graphviz.  
   - Se agrega un nodo principal para representar al usuario en el grafo.  

4. **Cierre del archivo y creación de imagen**:  
   - Una vez procesados todos los usuarios, se cierra el archivo `.dot`.  
   - Se utiliza el comando del sistema `dot` para convertir el archivo `.dot` en una imagen `.png`.  

5. **Mensaje de confirmación**:  
   - Se notifica al usuario que el reporte se ha generado exitosamente.  

---

### **Visualización del Reporte**  

El archivo `.dot` generado puede ser renderizado con Graphviz y produce un grafo con:  
- Un nodo por usuario, etiquetado con su nombre.  
- Subárboles que representan los activos asociados a cada usuario.  
- El color de los nodos de los activos depende de su disponibilidad (definido en la función `generarSubArbolGraphvizConRaiz`).

---
## **Función `reporteActivosDisponiblesDepartamento`**  
Esta función solicita al usuario ingresar un departamento y genera un reporte de los activos disponibles pertenecientes a los usuarios de ese departamento. Si se confirma, genera un archivo visualizable con Graphviz.

---

### **Funcionamiento General**

1. **Solicitar el nombre del departamento**:
   - Se pide al usuario que ingrese el nombre del departamento a consultar.  
   - Utiliza `getline(cin, departamento)` para permitir el ingreso de nombres que contengan espacios.  

2. **Buscar usuarios en la matriz dispersa**:
   - Recorre la estructura de matriz dispersa buscando nodos de usuarios asociados al departamento ingresado.  
   - Se sigue este recorrido jerárquico:  
     - Nodos de departamento.  
     - Nodos de empresas dentro de los departamentos.  
     - Nodos de usuarios dentro de las empresas.  
   - Para cada usuario encontrado:
     - Verifica si su atributo `departamento` coincide con el departamento solicitado.  
     - Imprime el nombre del usuario y su empresa asociada.  
     - Guarda su nombre en el arreglo `usuarios` si el contador es menor a 100.

3. **Verificar si se encontraron usuarios**:
   - Si no se encuentra ningún usuario del departamento solicitado, muestra un mensaje y finaliza la función.

4. **Preguntar si se desea generar un reporte**:
   - Si se confirma (respuesta 's' o 'S'), llama a la función `generarReporteGraphvizActivosDepartamento` para crear el reporte visual del departamento.  

---

### **Detalles Técnicos**

#### **Declaraciones importantes**
- **`string usuarios[100]`**: Arreglo para almacenar los nombres de hasta 100 usuarios encontrados.  
- **`int contadorUsuarios`**: Contador que lleva registro del número de usuarios encontrados.  
- **`NodoMatriz*`**: Punteros para navegar en la estructura de la matriz dispersa.  

#### **Recorrido de la matriz**  
- El recorrido utiliza varios niveles de bucles `while`:
  1. **Nivel 1**: Itera sobre los nodos de departamentos (usando el puntero `nodoDepartamentoActual`).
  2. **Nivel 2**: Itera sobre las empresas dentro de cada departamento (usando `nodoEmpresa`).
  3. **Nivel 3**: Itera sobre los usuarios dentro de cada empresa (usando `usuario`).

---

### **Interacción con el Usuario**

1. **Entrada del nombre del departamento**:
   - El usuario ingresa un departamento, como "Recursos Humanos" o "TI".

2. **Resultados de la búsqueda**:
   - Se imprime una lista de usuarios pertenecientes al departamento con su empresa asociada:
     ```
     Usuarios en el departamento TI:
     - Usuario: Juan Pérez (Empresa: TechCorp)
     - Usuario: Ana López (Empresa: Innovatech)
     ```

3. **Confirmación para generar el reporte**:
   - Si el usuario responde con 's' o 'S', se genera el archivo Graphviz.  

---


## **Función `generarReporteGraphvizActivosEmpresa`**  

### **Descripción General**
Esta función genera un archivo de reporte en formato Graphviz que representa gráficamente los activos de usuarios asociados a una empresa específica. Los activos de cada usuario se extraen de un árbol AVL y se visualizan en el grafo. El reporte final se guarda como una imagen en formato PNG.

---

### **Pasos Principales**

1. **Inicialización del archivo de salida**:
   - **Archivo `reporte_activos_empresa.dot`**:
     - Crea un archivo de texto para almacenar la definición del grafo en formato Graphviz.
     - Si el archivo no se abre correctamente, muestra un mensaje de error y termina la ejecución.

2. **Encabezado del Grafo**:
   - Inicia la estructura `digraph` para Graphviz.
   - Se define el título con el nombre de la empresa:  
     - **`label`**: Muestra el título del reporte.  
     - **`labelloc="t"`**: Coloca el título en la parte superior del grafo.  
     - **`fontsize=20`**: Ajusta el tamaño del texto del título.
   - Configura la apariencia de los nodos:
     - **`shape=rectangle`**: Define los nodos como rectángulos.
   - Establece la dirección de las conexiones:
     - **`rankdir=TB`**: Configura las conexiones de arriba hacia abajo.

3. **Búsqueda de Usuarios en la Estructura**:
   - **Recorrido de la matriz de usuarios**:
     - Se inicia en la cabeza de la matriz y se recorren los nodos jerárquicamente:
       1. **Departamentos**: Avanza horizontalmente por los nodos de departamentos.
       2. **Empresas**: Dentro de cada departamento, recorre las empresas asociadas.
       3. **Usuarios**: Recorre los usuarios de cada empresa en el nivel vertical.
     - Para cada usuario, verifica:
       - Si su nombre coincide con un usuario en la lista de `usuarios`.
       - Si su empresa coincide con el nombre de la empresa proporcionada.
     - Si se encuentra un usuario que cumple ambos criterios, se guarda una referencia al nodo correspondiente.

4. **Generación del Subárbol de Activos**:
   - Si se encuentra un usuario con un árbol AVL de activos:
     - Crea un nodo en el grafo con el nombre del usuario.
     - Llama a la función auxiliar `generarSubArbolGraphvizConRaiz` para representar gráficamente los activos almacenados en el árbol AVL.

5. **Cierre del Grafo**:
   - Finaliza el archivo `dot` con la estructura adecuada de Graphviz.

6. **Generación de la Imagen**:
   - Ejecuta el comando del sistema `dot` para convertir el archivo `dot` en una imagen PNG.
   - Informa al usuario que el reporte ha sido generado exitosamente como `reporte_activos_empresa.png`.

---

### **Puntos Clave**

- **Recorrido jerárquico**: La función recorre los niveles de la matriz para identificar usuarios específicos basándose en su empresa.
- **Uso de Graphviz**: La estructura del archivo `dot` permite visualizar gráficamente la relación entre usuarios y activos.
- **Modularidad**:
  - El uso de `generarSubArbolGraphvizConRaiz` para construir el subárbol facilita la generación de nodos y conexiones de manera eficiente.
- **Salida visual**: La conversión a PNG garantiza una presentación visual clara y legible.

---

## **Función `reporteActivosDisponiblesEmpresa`**

### **Descripción General**
Esta función permite generar un reporte gráfico utilizando Graphviz para visualizar los activos de los usuarios de una empresa específica. Se solicita al usuario ingresar el nombre de la empresa, luego se busca a través de una matriz de usuarios para encontrar a los usuarios que pertenecen a esa empresa. Finalmente, se genera un reporte visual en formato Graphviz.

---

### **Pasos Principales**

1. **Entrada del Nombre de la Empresa**:
   - Se solicita al usuario que ingrese el nombre de la empresa para generar el reporte.

2. **Búsqueda de Usuarios**:
   - **Recorrido de la matriz de usuarios**:
     - Se comienza en el nodo de `matrizUsuarios.cabeza` y se avanza a través de los nodos hijos, primero por departamentos, luego por empresas y finalmente por usuarios.
     - Para cada usuario, se verifica si pertenece a la empresa indicada.

3. **Almacenamiento de Usuarios**:
   - Si un usuario pertenece a la empresa especificada, su nombre es almacenado en un arreglo llamado `usuarios`.

4. **Validación de Usuarios**:
   - Si no se encuentran usuarios en la empresa, se muestra un mensaje indicando que no se encontraron coincidencias.

5. **Generación del Reporte**:
   - Si se encontraron usuarios, se pregunta al usuario si desea generar un reporte gráfico usando Graphviz.
   - Si el usuario responde afirmativamente, se llama a la función `generarReporteGraphvizActivosEmpresa` para generar el reporte.

6. **Generación de Graphviz**:
   - La función `generarReporteGraphvizActivosEmpresa` procesa los usuarios y los activos asociados a ellos en un formato visualizable mediante Graphviz.
   - Finalmente, se ejecuta el comando del sistema `dot` para generar una imagen PNG del reporte.

---

### **Puntos Clave**

- **Recorrido jerárquico**: La matriz es recorrida jerárquicamente para filtrar usuarios según su empresa.
- **Generación de reporte**: La función genera un subárbol visual para cada usuario mediante Graphviz, mostrando sus activos.
- **Modularidad**: La función principal llama a `generarReporteGraphvizActivosEmpresa` para generar el reporte visual, manteniendo la lógica organizada.

---

## **Función `generarReporteGraphvizActivosUsuario`**

### **Descripción General**
Esta función permite generar un reporte gráfico utilizando Graphviz para visualizar los activos asociados a un usuario específico. Se solicita el nombre del usuario, se busca en la matriz de usuarios, y si se encuentra, se genera un reporte visual representando sus activos mediante un árbol AVL.

---

### **Pasos Principales**

1. **Entrada del Nombre del Usuario**:
   - Se solicita al usuario ingresar el nombre del usuario para generar el reporte.

2. **Búsqueda del Usuario**:
   - **Recorrido de la matriz de usuarios**:
     - Se inicia desde `matrizUsuarios.cabeza` y se avanza a través de los nodos hijos (departamentos → empresas → usuarios) buscando el usuario especificado.

3. **Generación del Reporte**:
   - Si el usuario es encontrado y tiene un árbol AVL asociado (`arbolAVL` no es `nullptr`), se genera el reporte en formato Graphviz.

4. **Creación del Archivo Dot**:
   - Se crea un archivo `.dot` que define el grafo, estableciendo los nodos y conexiones entre ellos según los activos del usuario.

5. **Generación de Graphviz**:
   - Una vez creado el archivo `.dot`, se utiliza el comando `dot` para generar una imagen PNG del reporte.

---

### **Estructura del Grafo**

- **Nodo Raíz**:
  - Se representa el nombre del usuario como nodo raíz.
  
- **Subárbol**:
  - Utilizando la función `generarSubArbolGraphvizConRaiz`, se generan nodos para cada activo asociado al usuario, mostrando detalles como ID, nombre, descripción y disponibilidad.

---

## **Función `reporteActivosDisponiblesUsuario`**

### **Descripción General**
Esta función permite al usuario seleccionar un usuario específico de una lista para generar un reporte gráfico utilizando Graphviz. A través de la matriz de usuarios, se muestran los detalles de cada usuario, y si se selecciona uno, se genera un reporte que visualiza sus activos asociados mediante un árbol AVL.

---

### **Pasos Principales**

1. **Lista de Usuarios**:
   - Se recorre la matriz de usuarios para extraer y mostrar todos los usuarios disponibles, junto con su información asociada (departamento y empresa).

2. **Selección del Usuario**:
   - Se solicita al usuario seleccionar un número correspondiente a un usuario de la lista mostrada.

3. **Generación del Reporte**:
   - Una vez seleccionado un usuario, se valida y pregunta si desea generar un reporte de Graphviz para ese usuario específico.

4. **Generación del Reporte**:
   - Si se confirma, se invoca la función `generarReporteGraphvizActivosUsuario` para generar un archivo Graphviz específico para el usuario seleccionado.

---

### **Estructura del Grafo**

- **Nodo Raíz**:
  - El nodo raíz es el nombre del usuario seleccionado.

- **Subárbol**:
  - Utilizando la función `generarSubArbolGraphvizConRaiz`, se visualizan los activos asociados al usuario en un árbol con detalles como ID, nombre, descripción y disponibilidad.

---

## **Función `rentarActivo`**

### **Descripción General**
La función `rentarActivo` permite a un usuario rentar un activo disponible en el sistema. Busca y valida el activo solicitado dentro de la estructura del árbol AVL y gestiona la transacción para marcar el activo como no disponible.

---

### **Funcionalidad**

1. **Validación del Usuario**:
   - Verifica si el usuario logueado existe en la matriz `matrizUsuarios`.

2. **Visualización de Activos Disponibles**:
   - Muestra activos disponibles de otros usuarios menos los activos propios del usuario logueado.

3. **Selección del Activo**:
   - Solicita al usuario ingresar el ID del activo deseado para rentar.
   - Luego solicita el número de días para la renta del activo.

4. **Validación del Activo**:
   - Busca el activo en los árboles AVL de otros usuarios, verificando si está disponible y si el tiempo solicitado para la renta está dentro del límite permitido.

5. **Registro de la Transacción**:
   - Si el activo está disponible y la renta es válida, se registra la transacción en una lista, actualizando la disponibilidad del activo.

---
## **Función `generarReporteGraphvizActivosRentados`**

### **Descripción General**

La función `generarReporteGraphvizActivosRentados` genera un reporte visual en formato Graphviz que muestra los activos rentados por un usuario específico. Utiliza las transacciones registradas en la lista de transacciones para crear un diagrama visual de los activos junto con sus detalles relacionados.

---

### **Funcionalidad**

1. **Obtención de Usuarios**:
   - Crea una lista de usuarios disponibles, extrayendo sus nombres desde la matriz `matrizUsuarios`.

2. **Selección de Usuario**:
   - Solicita al usuario seleccionar un número que corresponde a un usuario específico para generar el reporte.

3. **Recolección de Información**:
   - Recorre las transacciones para el usuario seleccionado, extrayendo información como el ID del activo, su nombre, departamento, empresa y fechas asociadas a la renta.

4. **Generación del Reporte**:
   - Crea un archivo `.dot` utilizando la biblioteca `ofstream` para escribir el diagrama.
   - Utiliza Graphviz para generar una imagen en formato `.png` a partir del archivo `.dot`.

5. **Validación de Reporte**:
   - Si el usuario no tiene activos rentados, elimina automáticamente el archivo `.dot` generado y muestra un mensaje correspondiente.

---
## **Función `devolverActivo`**

### **Descripción General**

La función `devolverActivo` busca y cambia el estado de un activo específico a "disponible". Recorre todos los AVL (Árboles AVL) en la estructura de `matrizUsuarios` para encontrar y actualizar el estado del activo correspondiente.

---

### **Funcionalidad**

1. **Recorrido de Matriz de Usuarios**:
   - Recorre las matrices correspondientes a departamentos, empresas y usuarios.

2. **Busqueda del Activo**:
   - Dentro de cada usuario, navega a través de su árbol AVL en busca del activo utilizando su `idActivo`.

3. **Cambio de Estado**:
   - Si se encuentra el activo con el `idActivo` especificado, su estado se cambia a "disponible".

4. **Validación del Activo**:
   - Si no se encuentra el activo en ninguna de las estructuras, muestra un mensaje indicando que el activo no fue encontrado.

---

## **Función `eliminarTransaccion`**

### **Descripción General**

La función `eliminarTransaccion` permite eliminar una transacción específica de la lista circular. Busca la transacción basada en su `idTransaccion` y ajusta los punteros para eliminar el nodo correspondiente.

---

### **Funcionalidad**

1. **Validación de la Lista**:
   - Verifica si la lista circular está vacía.

2. **Recorrido de la Lista Circular**:
   - Recorre la lista para encontrar la transacción con el `idTransaccion` especificado.

3. **Eliminación de la Transacción**:
   - Si la transacción es la única en la lista, elimina directamente el nodo.
   - Si es la cabeza de la lista, ajusta el puntero de la cabeza a la siguiente transacción.
   - Si es un nodo intermedio, ajusta los punteros `anterior` y `siguiente` para "saltar" el nodo a eliminar.

4. **Liberación de Memoria**:
   - Libera la memoria del nodo eliminado para evitar fugas de memoria.

---

## **Función `activosRentados`**

### **Descripción General**

La función `activosRentados` muestra los activos rentados por el usuario actual y proporciona la opción de devolver un activo específico.

---

### **Funcionalidad**

1. **Verificación de Transacciones**:
   - Verifica si hay transacciones registradas en la lista circular de transacciones.

2. **Visualización de Activos Rentados**:
   - Muestra los detalles de los activos rentados por el usuario actual, incluyendo información como el ID de transacción, ID del activo, nombre del activo, departamento, empresa, días de renta y fecha de renta.

3. **Opción de Devolución**:
   - Ofrece la posibilidad de devolver un activo, solicitando el ID del activo a devolver.
   - Si se elige devolver un activo, busca la transacción correspondiente y realiza la devolución del activo, marcándolo como disponible.

4. **Proceso de Devolución**:
   - La función `devolverActivo` es llamada para cambiar el estado del activo a disponible.
   - La transacción es marcada como "DEVUELTO" sin eliminarla de la lista.

---

## **Función `verificarPropiedadActivo`**

### **Descripción General**

La función `verificarPropiedadActivo` verifica si un activo específico pertenece al usuario actual.

---

### **Funcionalidad**

1. **Búsqueda del Usuario**:
   - Utiliza la función `matrizUsuarios.buscarUsuario` para encontrar al usuario correspondiente en la matriz de usuarios, especificando el departamento y la empresa.

2. **Verificación del Árbol AVL**:
   - Verifica si el usuario tiene un árbol AVL asociado.
   
3. **Búsqueda del Activo**:
   - Si el usuario y su árbol AVL existen, se utiliza la función `buscarActivoEnAVL` para buscar el activo con el `idActivo` proporcionado.

4. **Resultado**:
   - Retorna `true` si el activo fue encontrado en el árbol AVL del usuario.
   - Retorna `false` si el activo no fue encontrado o si el usuario no tiene un árbol AVL asociado.

---
## **Función `mostrarMisActivosRentados`**

### **Descripción General**

La función `mostrarMisActivosRentados` permite al usuario visualizar los activos que ha rentado de otros usuarios, verificando si esos activos son propiedad del usuario actual.

---

### **Funcionalidad**

1. **Verificación de Transacciones**:
   - Verifica si hay transacciones registradas en la lista `listaTransacciones`. Si no hay transacciones, se muestra un mensaje correspondiente.

2. **Iteración sobre las Transacciones**:
   - Recorre cada transacción en la lista circular `listaTransacciones`.

3. **Filtrado de Activos Rentados por Otros Usuarios**:
   - Compara cada transacción con el usuario actual.
   - Utiliza `verificarPropiedadActivo` para verificar si el activo pertenece al usuario actual.

4. **Impresión de Detalles**:
   - Si el activo es propiedad del usuario actual, se imprimen los detalles de la transacción.

5. **Resultado**:
   - Si no se encuentran activos rentados por otros usuarios que pertenezcan al usuario actual, muestra un mensaje correspondiente.

---

## **Función `mostrarActivosRentados`**

### **Descripción General**

La función `mostrarActivosRentados` permite al usuario visualizar los detalles de los activos que ha rentado.

---

### **Funcionalidad**

1. **Validación de Transacciones**:
   - Verifica si existen transacciones registradas en la lista circular `listaTransacciones`. Si no hay transacciones, se muestra un mensaje correspondiente.

2. **Iteración sobre las Transacciones**:
   - Recorre cada transacción en la lista circular `listaTransacciones`.

3. **Filtrado de Activos del Usuario Actual**:
   - Muestra las transacciones cuyo usuario coincida con el nombre de usuario actual (`nombreUsuario`).

4. **Impresión de Detalles**:
   - Para cada transacción, se imprimen los detalles del activo rentado.

---
## **Función `generarReporteGraphvizListaCircularDoble`**

### **Descripción General**

La función `generarReporteGraphvizListaCircularDoble` genera un reporte visual en formato Graphviz de las transacciones registradas en una lista circular doble.

---

### **Funcionalidad**

1. **Validación de Transacciones**:
   - Verifica si la lista `lista` contiene transacciones. Si está vacía, muestra un mensaje correspondiente.

2. **Creación del Archivo DOT**:
   - Abre un archivo `reporte_transacciones.dot` para escribir la representación gráfica.

3. **Iteración sobre las Transacciones**:
   - Recorre cada transacción en la lista circular `lista.cabeza`.

4. **Creación de Nodos**:
   - Para cada transacción, crea un nodo en el archivo DOT que incluye los detalles del activo y del usuario.

5. **Conexiones entre Nodos**:
   - Crea enlaces entre nodos para mostrar la circularidad de la lista.

6. **Generación del Archivo de Imagen**:
   - Utiliza `dot` para convertir el archivo DOT a una imagen PNG (`reporte_transacciones.png`).

---
## **Función `OrdenarTransacciones`**

### **Descripción General**

La función `OrdenarTransacciones` permite ordenar una lista circular doble de transacciones según el ID de transacción en orden ascendente o descendente.

---

### **Funcionalidad**

1. **Validación Inicial**:
   - Verifica si la lista está vacía o contiene solo un elemento, en cuyo caso no se puede ordenar.

2. **Opciones de Ordenamiento**:
   - Presenta al usuario la opción de elegir entre ordenar ascendentemente o descendentemente.

3. **Comparación Personalizada**:
   - Utiliza una función de comparación alfanumérica para manejar correctamente IDs que contienen números mezclados con letras.

4. **Ordenamiento Burbujita**:
   - Implementa un algoritmo de ordenamiento burbuja para manejar listas circulares dobles. Se intercambian los datos (no solo los punteros) de los nodos según la comparación definida.

5. **Salida**:
   - Notifica al usuario si el ordenamiento fue realizado correctamente y muestra si fue ascendente o descendente.

---

## **Función `mostrarActivosAVL`**

### **Descripción General**

La función `mostrarActivosAVL` recorre y muestra los activos almacenados en un árbol AVL en orden.

---

### **Funcionalidad**

1. **Recorrido en InOrden**:
   - Utiliza el recorrido en inorden para mostrar los nodos del árbol AVL.
   
2. **Impresión de Nodos**:
   - Cada nodo se imprime mostrando sus atributos: `idActivo`, `nombreActivo`, `descripcion` y `tiempoMaximoRenta`.

3. **Llamadas Recursivas**:
   - Se realiza una llamada recursiva a la izquierda y derecha del árbol para recorrer todos los nodos.

---
## **Función `eliminarActivo`**

### **Descripción General**

La función `eliminarActivo` permite eliminar un activo específico del árbol AVL asociado a un usuario.

---

### **Funcionalidad**

1. **Busqueda del Usuario**:
   - Se busca al usuario utilizando `matrizUsuarios.buscarUsuario(departamento, empresa, nombreUsuario)`.

2. **Mostrar Activos**:
   - Muestra los activos disponibles en el árbol AVL del usuario mediante la función `mostrarActivosAVL`.

3. **Eliminación del Activo**:
   - Se solicita al usuario ingresar el ID del activo que desea eliminar.
   - Se busca el activo correspondiente en el árbol AVL.
   - Si se encuentra, se guarda la información del activo antes de eliminarlo (ID, nombre, descripción, tiempo máximo de renta).
   - Luego, se elimina el nodo correspondiente utilizando la función `eliminarNodoAVL`.

4. **Actualización del Árbol AVL**:
   - Una vez eliminado el activo, el árbol AVL se actualiza.

---
## **Función `generarCaracterAleatorio`**

### **Descripción General**

La función `generarCaracterAleatorio` genera un carácter aleatorio utilizando dos semillas pseudoaleatorias combinadas.

---

### **Funcionamiento**

1. **Definición de Semillas**:
   - `seed1` y `seed2` son dos semillas pseudoaleatorias inicializadas con valores predeterminados.

2. **Cálculo del Carácter**:
   - Ambas semillas se combinan utilizando la operación XOR (`^`).
   - El valor resultante se usa para seleccionar un carácter aleatorio de la cadena `caracteres`, que contiene letras mayúsculas, minúsculas y números.

3. **Selección del Carácter**:
   - Se usa la fórmula `combinedSeed % caracteres.length()` para obtener un índice dentro del rango de la cadena de caracteres.
   - Finalmente, se retorna el carácter correspondiente al índice calculado.

---
## **Función `generarIDActivo`**

### **Descripción General**

La función `generarIDActivo` crea un ID único de 15 caracteres alfanuméricos utilizando la función `generarCaracterAleatorio`.

---

### **Funcionamiento**

1. **Iteración para Generar Carácteres**:
   - Se utiliza un bucle `for` para generar 15 caracteres.
   - Dentro de cada iteración, se llama a `generarCaracterAleatorio()` para obtener un carácter alfanumérico.

2. **Composición del ID**:
   - Cada carácter obtenido se concatena a la variable `id`, formando una cadena de 15 caracteres.

3. **Retorno**:
   - La función devuelve el ID único generado como una cadena de 15 caracteres alfanuméricos.

---

## **Función `iniciarSesion`**

### **Descripción General**

La función `iniciarSesion` permite al usuario iniciar sesión proporcionando su nombre de usuario y contraseña. Dependiendo de si el usuario es un **administrador** o un **usuario regular**, se realizan diferentes verificaciones y configuraciones.

---

### **Funcionamiento**

1. **Ingreso de Datos**:
   - Solicita al usuario que ingrese su **nombre de usuario** y **contraseña**.
   
2. **Verificación de Usuario Admin**:
   - Si el nombre de usuario es "admin" y la contraseña es "admin", se inicia sesión como **administrador**.
   - En este caso, no es necesario solicitar el **departamento** ni la **empresa**. 

3. **Inicio de Sesión como Usuario Regular**:
   - Si el usuario no es administrador:
     - Solicita **departamento** y **empresa**.
     - Busca el usuario en la estructura `matrizUsuarios` utilizando los datos ingresados.
     - Verifica si el usuario existe y si la contraseña coincide con la registrada.
   
4. **Resultados**:
   - Si el usuario es encontrado y la contraseña coincide, se inicia sesión como usuario regular.
   - Si no se encuentra el usuario o las credenciales son incorrectas, se muestra un mensaje de error.

---

### **Retorno**

- `true` si es administrador.
- `false` si es un usuario normal.

---
## **Función `registrarUsuario`**

### **Descripción General**

La función `registrarUsuario` permite registrar un nuevo usuario en la estructura de datos `matrizUsuarios`. Solicita los datos necesarios para crear un nuevo usuario, incluyendo nombre, contraseña, departamento y empresa.

---

### **Funcionamiento**

1. **Ingreso de Datos**:
   - Solicita al usuario que ingrese los siguientes datos:
     - Nombre de usuario.
     - Contraseña.
     - Departamento.
     - Empresa.

2. **Inserción en la Matriz de Usuarios**:
   - Utiliza la función `insertarUsuario` de la clase `matrizUsuarios` para agregar un nuevo usuario a la estructura correspondiente.

3. **Resultado**:
   - Se muestra un mensaje de éxito indicando que el usuario ha sido registrado correctamente.

---

## **Función `agregarActivo`**

### **Descripción General**

La función `agregarActivo` permite agregar un nuevo activo a la lista de activos de un usuario específico. Genera un ID único para el activo y lo inserta en el árbol AVL correspondiente al usuario.

---

### **Funcionamiento**

1. **Entrada de Datos**:
   - Solicita al usuario ingresar:
     - Nombre del nuevo activo.
     - Descripción del nuevo activo.
     - Tiempo máximo de renta en días.

2. **Generación de ID Único**:
   - Utiliza la función `generarIDActivo` para generar un ID único alfanumérico para cada activo.

3. **Inserción en el Árbol AVL**:
   - Busca al usuario en la `matrizUsuarios` utilizando su nombre, departamento y empresa.
   - Si el usuario es encontrado, el activo se agrega al árbol AVL correspondiente mediante la función `insertarEnAVL`.

4. **Resultado**:
   - Si el activo es agregado exitosamente, se muestra un mensaje de confirmación.
   - En caso de errores, como usuario no encontrado, se informa al usuario.

---

## **Función `modificarActivo`**

### **Descripción General**

La función `modificarActivo` permite modificar la descripción de un activo específico en el árbol AVL de un usuario. Busca el activo mediante su ID y permite actualizar su descripción.

---

### **Funcionamiento**

1. **Buscar Usuario y Activo**:
   - Busca al usuario en la `matrizUsuarios` mediante su departamento, empresa y nombre de usuario.
   - Si el usuario posee un árbol AVL, busca el activo mediante su ID.

2. **Validación del Activo**:
   - Si se encuentra el activo correspondiente, se muestra su información actual: 
     - ID del activo.
     - Nombre del activo.
     - Descripción actual.
     - Tiempo máximo de renta.

3. **Actualizar Descripción**:
   - Solicita al usuario una nueva descripción para el activo.
   - Actualiza la descripción en el nodo correspondiente del árbol AVL.

4. **Resultado**:
   - Muestra un mensaje confirmando la modificación si se realizó exitosamente.
   - Si no se encuentra el activo, se informa al usuario con un mensaje de error.

---

## **Función `menuUsuario`**

### **Descripción General**

La función `menuUsuario` representa el menú principal del usuario después de iniciar sesión. Ofrece una interfaz interactiva con opciones relacionadas a la gestión de activos, rentas y sesión.

---

### **Funcionamiento**

1. **Interfaz del Menú**:
   - Presenta opciones organizadas en categorías para gestionar activos, rentar activos y cerrar sesión.
   - Utiliza colores para resaltar diferentes secciones:
     - **Azul**: Encabezado del menú.
     - **Cyan**: Secciones principales.
     - **Amarillo**: Opciones del menú.
     - **Verde**: Mensajes de éxito.
     - **Rojo**: Mensajes de error.

2. **Validación de Entrada**:
   - Si el usuario ingresa un dato incorrecto (como letras o caracteres no permitidos en la opción), se limpia la entrada y se solicita nuevamente la opción.

3. **Acciones según Opción**:
   - Cada opción (1 a 7) ejecuta una función correspondiente relacionada a la gestión de activos o sesiones:
     - **1**: Agregar Nuevo Activo
     - **2**: Eliminar Activo
     - **3**: Modificar Activo
     - **4**: Rentar Nuevo Activo
     - **5**: Ver Todos los Activos en Renta
     - **6**: Mis Activos Rentados
     - **7**: Cerrar Sesión

4. **Control de Excepciones**:
   - Maneja excepciones genéricas y específicas al ejecutar cada opción para asegurar la estabilidad del programa.

---
## **Función `menuAdministrador`**

### **Descripción General**

La función `menuAdministrador` proporciona un menú interactivo para el administrador, con diversas opciones relacionadas a la gestión de usuarios, reportes, y herramientas administrativas.

---

### **Funcionamiento**

1. **Interfaz del Menú**:
   - Presenta una serie de opciones organizadas en categorías.
   - Utiliza colores para diferenciar cada sección:
     - **Azul**: Encabezado del menú.
     - **Cyan**: Secciones principales.
     - **Amarillo**: Opciones del menú.
     - **Verde**: Mensajes de éxito.
     - **Rojo**: Mensajes de error.

2. **Validación de Entrada**:
   - Si el usuario introduce un dato inválido (como letras o caracteres no permitidos en la opción), la función limpia la entrada y solicita nuevamente la opción mediante el uso de `limits`.

3. **Acciones según Opción**:
   - Cada opción (1 a 9) ejecuta funciones específicas relacionadas a la gestión o generación de reportes:
     - **1**: Registrar Nuevo Usuario
     - **2**: Visualizar Matriz Dispersa
     - **3**: Reporte de Activos por Departamento
     - **4**: Reporte de Activos por Empresa
     - **5**: Reporte de Transacciones
     - **6**: Reporte de Activos por Usuario
     - **7**: Generar Activos Rentados por Usuario
     - **8**: Ordenar Registro de Transacciones
     - **9**: Regresar al Menú Principal

4. **Control de Excepciones**:
   - Maneja excepciones en caso de errores al ejecutar cada opción para asegurar una buena experiencia al administrador.

---

## Función obtenerOpcionValida

### Descripción General

La función `obtenerOpcionValida` se encarga de solicitar una opción al usuario, validando que sea un número entero válido. En caso de una entrada no válida, limpia el flujo de entrada y solicita nuevamente hasta recibir una opción correcta.

---

### Funcionamiento

#### Validación de Entrada
1. **Solicitud Inicial**: La función espera que el usuario ingrese un valor entero.
2. **Verificación de Entrada**: 
   - Se utiliza `cin >> opcion` para intentar leer un número.
   - Si la entrada es válida, se limpia el búfer de entrada usando `cin.ignore` y se devuelve el valor ingresado.
3. **Manejo de Errores**:
   - Si la entrada no es válida (por ejemplo, letras o caracteres especiales), el flujo de entrada se limpia con:
     - `cin.clear()` para restablecer el estado de error del flujo.
     - `cin.ignore(numeric_limits<streamsize>::max(), '\n')` para descartar el resto de la entrada.
   - Se muestra un mensaje de error en la consola utilizando colores ANSI para resaltar el texto:
     - **Rojo** para el error.
     - **Azul** para solicitar nuevamente la opción.

4. **Reintento**: La función sigue solicitando una opción hasta que se recibe un número válido.

---

## Función Principal del Menú (`main`)

### Descripción General

La función principal del programa implementa un menú interactivo que permite a los usuarios:
1. Iniciar sesión como administrador o usuario.
2. Salir del sistema.

El menú utiliza colores para mejorar la experiencia del usuario y simbolismos gráficos para hacer la interfaz más atractiva.

---

### Funcionamiento

#### Declaración de Constantes
- Se declaran constantes para los códigos de color ANSI y algunos elementos gráficos, como símbolos y encabezados, para mejorar la legibilidad del menú:
  - **RESET**: Restablece el color del texto.
  - **BLUE**: Color azul para encabezados y mensajes informativos.
  - **GREEN**: Color verde para mensajes positivos.
  - **RED**: Color rojo para mensajes de error.
  - **YELLOW**: Color amarillo para resaltar opciones.
  - **HEADER**: Texto decorativo del encabezado del menú.
  - **WELCOME_SYMBOL** y **LOCK_SYMBOL**: Iconos para mejorar la interfaz gráfica.

#### Limpieza de Pantalla
- Se utiliza un preprocesador (`#ifdef _WIN32`) para limpiar la pantalla:
  - **Windows**: Usa el comando `cls`.
  - **Otros sistemas**: Usa el comando `clear`.

#### Interfaz del Menú
- Se imprime el encabezado y un mensaje de bienvenida utilizando las constantes de color y elementos gráficos.
- Se presentan dos opciones principales:
  1. Iniciar sesión.
  2. Salir del sistema.

#### Lógica del Menú
- Se utiliza un bucle `do-while` para mantener el menú activo hasta que el usuario seleccione la opción de salir (`opcion == 2`).
- Las entradas del usuario son validadas mediante la función `obtenerOpcionValida`.

#### Validación de Opciones
- La lógica del menú utiliza una estructura `switch` para gestionar las opciones:
  - **Caso 1: Iniciar Sesión**  
    - Llama a la función `iniciarSesion` para validar las credenciales del usuario.
    - Si el usuario es administrador:
      - Se ejecuta el menú administrativo mediante `menuAdministrador`.
    - Si el usuario es un cliente válido (encontrado en la `matrizUsuarios`):
      - Se ejecuta el menú de usuario mediante `menuUsuario`.
    - Si las credenciales son inválidas:
      - Muestra un mensaje de error en rojo y regresa al menú principal.
  - **Caso 2: Salir del Sistema**  
    - Imprime un mensaje de despedida en verde y finaliza la ejecución.
  - **Default (Opción Inválida)**  
    - Muestra un mensaje de error en rojo indicando que la opción ingresada es inválida.

---

## Breve Explicación de las Librerías

### 1. `#include <iostream>`
- Permite la entrada y salida estándar, como imprimir en la consola (`std::cout`) o leer datos del teclado (`std::cin`).

### 2. `#include <string>`
- Proporciona la clase `std::string` para manejar cadenas de texto de manera fácil y eficiente.

### 3. `#include <ctime>`
- Permite trabajar con fechas y horas, como obtener la fecha/hora actual o formatearlas.

### 4. `#include <fstream>`
- Facilita el manejo de archivos para lectura y escritura, como crear o leer archivos de texto.

### 5. `#include <sstream>`
- Proporciona herramientas para manejar cadenas como flujos de datos, útil para convertir entre cadenas y otros tipos de datos.

### 6. `#include <limits>`
- Ofrece información sobre los límites de tipos de datos, como el valor máximo de un flujo, usado para limpiar entradas inválidas.
