#include <iostream>
#include <string>
#include <ctime>
#include <fstream>
#include <sstream>
#include <limits>

using namespace std;



string nombreUsuario, contrasena, departamento, empresa;

//estructura del nodo de avl de activos 
struct NodoAVL {
    string idActivo;
    string nombreActivo;
    string descripcion;
    int tiempoMaximoRenta; 
    bool disponible;
    NodoAVL* izquierda;
    NodoAVL* derecha;
    int altura;
};


// estrtuctura del nodo de la matris dispoersa 
struct NodoMatriz {
    string nombreUsuario;   
    string contrasena;      
    string departamento;    
    string empresa;           
    NodoAVL* arbolAVL;        
    NodoMatriz* derecha;      
    NodoMatriz* abajo;        
    

    NodoMatriz(string usuario, string pass, string dept = "", string emp = "")
        : nombreUsuario(usuario), 
          contrasena(pass), 
          departamento(dept),
          empresa(emp),
          arbolAVL(nullptr), 
          derecha(nullptr), 
          abajo(nullptr) 
    {} 
};

// clase de matriz dispersita 
class MatrizDispersa {
public:
    NodoMatriz* cabeza;

public:
    MatrizDispersa() : cabeza(nullptr) {}

// Modificación del método insertarUsuario
void insertarUsuario(string departamento, string empresa, string usuario, string contrasena) {
    NodoMatriz* nodoDepartamento = obtenerOInsertarEncabezado(departamento, true);
    NodoMatriz* nodoEmpresa = obtenerOInsertarEncabezado(empresa, false, nodoDepartamento);

    // Verificar si el usuario ya existe
    NodoMatriz* actual = nodoEmpresa->abajo;
    while (actual != nullptr) {
        if (actual->nombreUsuario == usuario) {
            cout << "Error: El usuario ya existe en esta empresa y departamento.\n";
            return;
        }
        actual = actual->abajo;
    }

    // Crear nuevo usuario con su departamento
    NodoMatriz* nuevoUsuario = new NodoMatriz(usuario, contrasena, departamento, empresa );
    nuevoUsuario->abajo = nodoEmpresa->abajo;
    nodoEmpresa->abajo = nuevoUsuario;

    cout << "Usuario registrado exitosamente en la matriz dispersa.\n";
}
    // Buscar un usuario en la matriz
    NodoMatriz* buscarUsuario(string departamento, string empresa, string usuario) {
        NodoMatriz* nodoDepartamento = buscarEncabezado(departamento, true);
        if (!nodoDepartamento) return nullptr;

        NodoMatriz* nodoEmpresa = buscarEncabezado(empresa, false, nodoDepartamento);
        if (!nodoEmpresa) return nullptr;

        NodoMatriz* actual = nodoEmpresa->abajo;
        while (actual != nullptr) {
            if (actual->nombreUsuario == usuario) {
                return actual;
            }
            actual = actual->abajo;
        }

        return nullptr;
    }
    

// Obtener encabezados horizontales (departamentos)
    void obtenerEncabezadosHorizontales() {
        NodoMatriz* actual = cabeza;
        cout << "Encabezados horizontales (Departamentos):\n";
        while (actual) {
            cout << actual->nombreUsuario << " ";
            actual = actual->derecha;
        }
        cout << endl;
    }

// Obtener grupo (columna) según el departamento
int obtenerGrupoDepartamento(string departamento) {
    NodoMatriz* actual = cabeza;
    int grupo = 2; // El grupo empieza en 2 después del nodo ADMIN
    while (actual) {
        if (actual->nombreUsuario == departamento) return grupo;
        actual = actual->derecha;
        grupo++;
    }
    return grupo; // Devuelve la posición del departamento
}

// Obtener fila según la empresa
int obtenerFilaEmpresa(string empresa) {
    NodoMatriz* actual = cabeza;
    NodoMatriz* actualEmpresa = nullptr;
    int fila = 2; // La fila empieza en 2 después del nodo ADMIN
    while (actual) {
        actualEmpresa = actual->abajo;
        while (actualEmpresa) {
            if (actualEmpresa->empresa == empresa) return fila;
            actualEmpresa = actualEmpresa->abajo;
            fila++;
        }
        actual = actual->derecha;
    }
    return fila; // Devuelve la posición de la empresa
}

void generarReporteGraphviz() {
    ofstream archivo("reporte_matriz.dot");
    if (!archivo.is_open()) {
        cout << "Error al crear el archivo de reporte.\n";
        return;
    }

    archivo << "digraph MatrizDispersa {\n";
    archivo << "  node [shape=box, style=filled, fontname=Arial];\n";
    archivo << "  splines=ortho;\n";
    archivo << "  rankdir=TB;\n";

    // Encabezado principal "ADMIN"
    archivo << "  \"ADMIN\" [fillcolor=gray, group=1, pos=\"0,0!\"];\n";

// 1. Generar los encabezados de departamentos (primera fila)
NodoMatriz* actualDepartamento = cabeza;
NodoMatriz* anteriorDepartamento = nullptr;
int columnaDepartamento = 2; // Initialize columnaDepartamento
while (actualDepartamento) {
    archivo << "  \"" << actualDepartamento->nombreUsuario 
            << "\" [fillcolor=lightblue, group=" << columnaDepartamento 
            << ", pos=\"" << columnaDepartamento << ",0!\"];\n";
    
    // Conectar departamentos desde ADMIN
    if (anteriorDepartamento != nullptr) {
        archivo << "  \"" << anteriorDepartamento->nombreUsuario 
                << "\" -> \"" << actualDepartamento->nombreUsuario << "\";\n";
    } else {
        archivo << "  \"ADMIN\" -> \"" << actualDepartamento->nombreUsuario << "\";\n";
    }
    
    anteriorDepartamento = actualDepartamento;
    actualDepartamento = actualDepartamento->derecha;
    columnaDepartamento++;
}

// 2. Generar los encabezados de empresas (primera columna)
actualDepartamento = cabeza;
NodoMatriz* anteriorEmpresa = nullptr;
int filaEmpresa = 1;
while (actualDepartamento) {
    NodoMatriz* actualEmpresa = actualDepartamento->abajo;
    while (actualEmpresa) {
        archivo << "  \"" << actualEmpresa->empresa 
                << "\" [fillcolor=lightgreen, group=1, pos=\"0," << -filaEmpresa << "!\"];\n";
        
        // Conectar empresas desde ADMIN
        if (anteriorEmpresa != nullptr) {
            archivo << "  \"" << anteriorEmpresa->empresa 
                    << "\" -> \"" << actualEmpresa->empresa << "\";\n";
        } else {
            archivo << "  \"ADMIN\" -> \"" << actualEmpresa->empresa << "\";\n";
        }
        
        anteriorEmpresa = actualEmpresa;
        actualEmpresa = actualEmpresa->abajo;
        filaEmpresa++;
    }
    actualDepartamento = actualDepartamento->derecha;
}

// 3. Generar conexiones de usuarios (similar patrón)
actualDepartamento = cabeza;
while (actualDepartamento) {
    NodoMatriz* actualEmpresa = actualDepartamento->abajo;
    while (actualEmpresa) {
        NodoMatriz* anteriorUsuario = nullptr;
        NodoMatriz* actualUsuario = actualEmpresa->abajo;
        
        while (actualUsuario) {
            // Conectar usuarios a su empresa
            if (anteriorUsuario != nullptr) {
                archivo << "  \"" << anteriorUsuario->nombreUsuario 
                        << "\" -> \"" << actualUsuario->nombreUsuario << "\";\n";
            } else {
                archivo << "  \"" << actualEmpresa->empresa 
                        << "\" -> \"" << actualUsuario->nombreUsuario << "\";\n";
            }
            
            anteriorUsuario = actualUsuario;
            actualUsuario = actualUsuario->abajo;
        }
        
        actualEmpresa = actualEmpresa->abajo;
    }
    
    actualDepartamento = actualDepartamento->derecha;
}
    // Agrupar los nodos en la primera fila y columna
    archivo << "  { rank=same; \"ADMIN\" ";
    actualDepartamento = cabeza;
    while (actualDepartamento) {
        archivo << "\"" << actualDepartamento->nombreUsuario << "\" ";
        actualDepartamento = actualDepartamento->derecha;
    }
    archivo << "}\n";

    archivo << "}\n";
    archivo.close();

    // Generar la imagen usando Graphviz con layout específico
    system("dot -Tpng -Kfdp reporte_matriz.dot -o reporte_matriz.png");
    cout << "Reporte generado como 'reporte_matriz.png'.\n";
}


public:
    // Obtener o insertar un encabezado (horizontal o vertical)
    NodoMatriz* obtenerOInsertarEncabezado(string nombre, bool esHorizontal, NodoMatriz* base = nullptr) {
        NodoMatriz** referencia = esHorizontal ? &cabeza : &base->derecha;

        while (*referencia != nullptr && (*referencia)->nombreUsuario < nombre) {
            referencia = esHorizontal ? &((*referencia)->derecha) : &((*referencia)->abajo);
        }

        if (*referencia == nullptr || (*referencia)->nombreUsuario != nombre) {
            NodoMatriz* nuevoEncabezado = new NodoMatriz(nombre, "");
            nuevoEncabezado->derecha = esHorizontal ? *referencia : nullptr;
            nuevoEncabezado->abajo = esHorizontal ? nullptr : *referencia;
            *referencia = nuevoEncabezado;
        }

        return *referencia;
    }

    // Buscar un encabezado (horizontal o vertical)
    NodoMatriz* buscarEncabezado(string nombre, bool esHorizontal, NodoMatriz* base = nullptr) {
        NodoMatriz* actual = esHorizontal ? cabeza : base->derecha;
        while (actual != nullptr) {
            if (actual->nombreUsuario == nombre) {
                return actual;
            }
            actual = esHorizontal ? actual->derecha : actual->abajo;
        }
        return nullptr;
    }
};

// nodo para un nuevo activo en el arbol avl
NodoAVL* crearNodoAVL(string id, string nombre, string descripcion, int tiempoMaximo) {
    NodoAVL* nuevoNodo = new NodoAVL();
    nuevoNodo->idActivo = id;
    nuevoNodo->nombreActivo = nombre;
    nuevoNodo->descripcion = descripcion;
    nuevoNodo->tiempoMaximoRenta = tiempoMaximo; 
    nuevoNodo->disponible = true; 
    nuevoNodo->izquierda = nullptr;
    nuevoNodo->derecha = nullptr;
    nuevoNodo->altura = 1;
    return nuevoNodo;
}

// ovtener la altura de un nodo
int obtenerAltura(NodoAVL* nodo) {
    return nodo ? nodo->altura : 0;
}

// este nomas pa balancear el arbol
int obtenerBalance(NodoAVL* nodo) {
    return nodo ? obtenerAltura(nodo->izquierda) - obtenerAltura(nodo->derecha) : 0;
}

// rotar a la derecha
NodoAVL* rotarDerecha(NodoAVL* y) {
    NodoAVL* x = y->izquierda;
    NodoAVL* T2 = x->derecha;

    x->derecha = y;
    y->izquierda = T2;

    y->altura = max(obtenerAltura(y->izquierda), obtenerAltura(y->derecha)) + 1;
    x->altura = max(obtenerAltura(x->izquierda), obtenerAltura(x->derecha)) + 1;

    return x;
}

// rotar a la izquierda
NodoAVL* rotarIzquierda(NodoAVL* x) {
    NodoAVL* y = x->derecha;
    NodoAVL* T2 = y->izquierda;

    y->izquierda = x;
    x->derecha = T2;

    x->altura = max(obtenerAltura(x->izquierda), obtenerAltura(x->derecha)) + 1;
    y->altura = max(obtenerAltura(y->izquierda), obtenerAltura(y->derecha)) + 1;

    return y;
}

// este nomas pa insertar un nuevo nodo en el arbol 
NodoAVL* insertarEnAVL(NodoAVL* nodo, string id, string nombre, string descripcion, int tiempoMaximo) {
    if (!nodo)
        return crearNodoAVL(id, nombre, descripcion, tiempoMaximo);

    if (id < nodo->idActivo)
        nodo->izquierda = insertarEnAVL(nodo->izquierda, id, nombre, descripcion, tiempoMaximo);
    else if (id > nodo->idActivo)
        nodo->derecha = insertarEnAVL(nodo->derecha, id, nombre, descripcion, tiempoMaximo);
    else
        return nodo;

    nodo->altura = 1 + max(obtenerAltura(nodo->izquierda), obtenerAltura(nodo->derecha));
    int balance = obtenerBalance(nodo);
 // diferentes casos para balancear el arbol si se me fue alguno pos F 
    if (balance > 1 && id < nodo->izquierda->idActivo)
        return rotarDerecha(nodo);
    if (balance < -1 && id > nodo->derecha->idActivo)
        return rotarIzquierda(nodo);
    if (balance > 1 && id > nodo->izquierda->idActivo) {
        nodo->izquierda = rotarIzquierda(nodo->izquierda);
        return rotarDerecha(nodo);
    }
    if (balance < -1 && id < nodo->derecha->idActivo) {
        nodo->derecha = rotarDerecha(nodo->derecha);
        return rotarIzquierda(nodo);
    }

    return nodo;
}

MatrizDispersa matrizUsuarios; 

// funcion auxiliar para encontrar el nodo con el valor mino del avl
NodoAVL* encontrarMinimo(NodoAVL* nodo) {
    while (nodo->izquierda != nullptr)
        nodo = nodo->izquierda;
    return nodo;
}

// se lo mocha de arbol totalmente no parcial 
NodoAVL* eliminarNodoAVL(NodoAVL* raiz, string id) {
    if (!raiz)
        return raiz;

    if (id < raiz->idActivo)
        raiz->izquierda = eliminarNodoAVL(raiz->izquierda, id);
    else if (id > raiz->idActivo)
        raiz->derecha = eliminarNodoAVL(raiz->derecha, id);
    else {
        //  si el nodo tiene un hijo o no
        if (!raiz->izquierda || !raiz->derecha) {
            NodoAVL* temp = raiz->izquierda ? raiz->izquierda : raiz->derecha;
            delete raiz;
            return temp;
        } else {
            // si tiene dos hijos  
            NodoAVL* temp = encontrarMinimo(raiz->derecha);
            raiz->idActivo = temp->idActivo;
            raiz->nombreActivo = temp->nombreActivo;
            raiz->descripcion = temp->descripcion;
            raiz->tiempoMaximoRenta = temp->tiempoMaximoRenta;
            raiz->derecha = eliminarNodoAVL(raiz->derecha, temp->idActivo);
        }
    }

    // Actualizar altura y balancear
    raiz->altura = 1 + max(obtenerAltura(raiz->izquierda), obtenerAltura(raiz->derecha));
    int balance = obtenerBalance(raiz);

    if (balance > 1 && obtenerBalance(raiz->izquierda) >= 0)
        return rotarDerecha(raiz);
    if (balance > 1 && obtenerBalance(raiz->izquierda) < 0) {
        raiz->izquierda = rotarIzquierda(raiz->izquierda);
        return rotarDerecha(raiz);
    }
    if (balance < -1 && obtenerBalance(raiz->derecha) <= 0)
        return rotarIzquierda(raiz);
    if (balance < -1 && obtenerBalance(raiz->derecha) > 0) {
        raiz->derecha = rotarDerecha(raiz->derecha);
        return rotarIzquierda(raiz);
    }

    return raiz;
}

// Nueva función para mostrar solo activos disponibles
void mostrarActivosDisponiblesAVL(NodoAVL* nodo) {
    if (!nodo) return;

    // inorder traversal para mostrar los activos ordenados
    mostrarActivosDisponiblesAVL(nodo->izquierda);

    if (nodo->disponible) {
        cout << "ID Activo: " << nodo->idActivo << endl;
        cout << "Nombre: " << nodo->nombreActivo << endl;
        cout << "Descripcion: " << nodo->descripcion << endl;
        cout << "Tiempo Máximo de Renta: " << nodo->tiempoMaximoRenta << " dias" << endl;
        cout << "----------------------------" << endl;
    }

    mostrarActivosDisponiblesAVL(nodo->derecha);
}

// funcion para buscar un activo en el avl 
NodoAVL* buscarActivoEnAVL(NodoAVL* raiz, const string& idActivo) {
    if (!raiz) return nullptr;

    if (raiz->idActivo == idActivo) {
        return raiz;
    } else if (idActivo < raiz->idActivo) {
        return buscarActivoEnAVL(raiz->izquierda, idActivo);
    } else {
        return buscarActivoEnAVL(raiz->derecha, idActivo);
    }
}

// funcion del esqueleto para arboles en graphiz 
void generarSubArbolGraphvizConRaiz(ofstream& archivo, string nombreRaiz, NodoAVL* nodo, string padre = "") {
    if (!nodo) return;

    
    string nombreNodo = nombreRaiz + "_" + nodo->idActivo;
    
    // vamos a definir el color basado en disponibilidad verde es que si esta, rojo que no 
    string colorNodo = nodo->disponible ? "green" : "red";
    
 
    archivo << "    \"" << nombreNodo << "\" [label=\"" 
            << "ID: " << nodo->idActivo 
            << "\\nNombre: " << nodo->nombreActivo 
            << "\\nDesc: " << nodo->descripcion 
            << "\\nTiempo Max: " << nodo->tiempoMaximoRenta 
            << "\", color=\"" << colorNodo << "\", style=filled, fillcolor=\"" << colorNodo << "\"];\n";

    // conexiones con el nodo padre 
    if (padre.empty()) {
        //  nodo raiz el nombre del usuario 
        archivo << "    \"" << nombreRaiz << "\" -> \"" << nombreNodo << "\";\n";
    } else {
        archivo << "    \"" << padre << "\" -> \"" << nombreNodo << "\";\n";
    }

    // los hijos se recorreran de izquierda a derecha
    generarSubArbolGraphvizConRaiz(archivo, nombreRaiz, nodo->izquierda, nombreNodo);
    generarSubArbolGraphvizConRaiz(archivo, nombreRaiz, nodo->derecha, nombreNodo);
}

// funcion para generar el reporte de activos en graphviz dependiendo el depa 
void generarReporteGraphvizActivosDepartamento(string* usuarios, int cantidadUsuarios, const string& departamento) {
    ofstream archivo("reporte_activos_departamento.dot");
    
    if (!archivo.is_open()) {
        cout << "Error al crear el archivo Graphviz." << endl;
        return;
    }

    archivo << "digraph ArbolActivos {\n";
    archivo << "    label=\"Reporte del departamento: " << departamento << "\";\n";
    archivo << "    labelloc=\"t\";\n";
    archivo << "    fontsize=20;\n";
    archivo << "    node [shape=rectangle];\n";
    archivo << "    rankdir=TB;\n";

    // vamos a buscar el usuario en la matriz 
    for (int i = 0; i < cantidadUsuarios; i++) {
        
        NodoMatriz* usuarioNodo = nullptr;
        NodoMatriz* nodoDepartamento = matrizUsuarios.cabeza;
        while (nodoDepartamento) {
            NodoMatriz* nodoEmpresa = nodoDepartamento->derecha;
            while (nodoEmpresa) {
                NodoMatriz* usuario = nodoEmpresa->abajo;
                while (usuario) {
                    if (usuario->nombreUsuario == usuarios[i]) {
                        usuarioNodo = usuario;
                        break;
                    }
                    usuario = usuario->abajo;
                }
                if (usuarioNodo) break;
                nodoEmpresa = nodoEmpresa->derecha;
            }
            if (usuarioNodo) break;
            nodoDepartamento = nodoDepartamento->derecha;
        }

        // si se encuentra el usuario se genera el subarbol 
        if (usuarioNodo && usuarioNodo->arbolAVL) {
           
            archivo << "    \"" << usuarios[i] << "\" [label=\"" << usuarios[i] << "\"];\n";
            
           
            generarSubArbolGraphvizConRaiz(archivo, usuarios[i], usuarioNodo->arbolAVL);
        }
    }

    archivo << "}\n";
    archivo.close();

    
    string comando = "dot -Tpng reporte_activos_departamento.dot -o reporte_activos_departamento.png";
    system(comando.c_str());

    cout << "Reporte Graphviz generado exitosamente en reporte_activos_departamento.png" << endl;
}

// algran dejo otra tarea el ing, hacerla en la noche 

// funcion para pedirle el depa a consultar para el reporte 
void reporteActivosDisponiblesDepartamento() {
    string departamento;
    cout << "Ingrese el departamento: ";
    cin.ignore();
    getline(cin, departamento);

    string usuarios[100];
    int contadorUsuarios = 0;

    cout << "Usuarios en el departamento " << departamento << ":" << endl;

    // aquí vamos a pasar por   toda la matriz buscando usuarios del departamento especfico
    NodoMatriz* nodoDepartamentoActual = matrizUsuarios.cabeza;
    while (nodoDepartamentoActual != nullptr) {
        NodoMatriz* nodoEmpresa = nodoDepartamentoActual->derecha;
        while (nodoEmpresa != nullptr) {
            NodoMatriz* usuario = nodoEmpresa->abajo;
            while (usuario != nullptr) {
                // mirar si el usuario es del departamento 
                if (usuario->departamento == departamento) {
                    cout << "- Usuario: " << usuario->nombreUsuario 
                         << " (Empresa: " << nodoEmpresa->nombreUsuario << ")" << endl;
                    if (contadorUsuarios < 100) {
                        usuarios[contadorUsuarios++] = usuario->nombreUsuario;
                    }
                }
                usuario = usuario->abajo;
            }
            nodoEmpresa = nodoEmpresa->derecha;
        }
        nodoDepartamentoActual = nodoDepartamentoActual->derecha;
    }

    if (contadorUsuarios == 0) {
        cout << "No se encontraron usuarios en el departamento " << departamento << endl;
        return;
    }

    char respuesta;
    cout << "¿Desea generar un reporte de Graphviz? (s/n): ";
    cin >> respuesta;

    if (respuesta == 's' || respuesta == 'S') {
        generarReporteGraphvizActivosDepartamento(usuarios, contadorUsuarios, departamento);

    }
}

// funcion para generar el reporte de activos en graphviz dependiendo la empresa
void generarReporteGraphvizActivosEmpresa(string* usuarios, int cantidadUsuarios, const string& empresa) {
    ofstream archivo("reporte_activos_empresa.dot");
    
    if (!archivo.is_open()) {
        cout << "Error al crear el archivo Graphviz." << endl;
        return;
    }

  
    archivo << "digraph ArbolActivos {\n";
    archivo << "    label=\"Reporte de empresa: " << empresa << "\";\n";
    archivo << "    labelloc=\"t\";\n";
    archivo << "    fontsize=20;\n";
    archivo << "    node [shape=rectangle];\n";
    archivo << "    rankdir=TB;\n";

    
    for (int i = 0; i < cantidadUsuarios; i++) {
        // bscar el usuario en la matriz
        NodoMatriz* usuarioNodo = nullptr;
        NodoMatriz* nodoDepartamento = matrizUsuarios.cabeza;
        while (nodoDepartamento) {
            NodoMatriz* nodoEmpresa = nodoDepartamento->derecha;
            while (nodoEmpresa) {
                NodoMatriz* usuario = nodoEmpresa->abajo;
                while (usuario) {
                    if (usuario->nombreUsuario == usuarios[i] && usuario->empresa == empresa) {
                        usuarioNodo = usuario;
                        break;
                    }
                    usuario = usuario->abajo;
                }
                if (usuarioNodo) break;
                nodoEmpresa = nodoEmpresa->derecha;
            }
            if (usuarioNodo) break;
            nodoDepartamento = nodoDepartamento->derecha;
        }

        // si se encuentra el usuario se genera el subarbol
        if (usuarioNodo && usuarioNodo->arbolAVL) {
            // Crear nodo raíz con nombre de usuario
            archivo << "    \"" << usuarios[i] << "\" [label=\"" << usuarios[i] << "\"];\n";
            
        
            generarSubArbolGraphvizConRaiz(archivo, usuarios[i], usuarioNodo->arbolAVL);
        }
    }

    archivo << "}\n";
    archivo.close();

    
    string comando = "dot -Tpng reporte_activos_empresa.dot -o reporte_activos_empresa.png";
    system(comando.c_str());

    cout << "Reporte Graphviz generado exitosamente en reporte_activos_empresa.png" << endl;
}

// algp como que esta mal en la funcion de arriba me lleva la verga 

// funcion para pedirle la empresa a consultar para el reporte
void reporteActivosDisponiblesEmpresa() {
    string empresa;
    cout << "Ingrese el nombre de la empresa: ";
    cin.ignore();
    getline(cin, empresa);

    
    string usuarios[100];
    int contadorUsuarios = 0;

    cout << "Usuarios en la empresa " << empresa << ":" << endl;

    //  pasamos por toda la matriz buscando usuarios de la empresa especifica
    NodoMatriz* nodoDepartamento = matrizUsuarios.cabeza;
    while (nodoDepartamento != nullptr) {
        NodoMatriz* nodoEmpresa = nodoDepartamento->derecha;
        while (nodoEmpresa != nullptr) {
            // buscar usuarios de la empresa
            NodoMatriz* usuario = nodoEmpresa->abajo;
            while (usuario != nullptr) {
                // validamos si el usuario es de la empresa
                if (usuario->empresa == empresa) {
                    cout << "- Usuario: " << usuario->nombreUsuario 
                         << " (Departamento: " << usuario->departamento << ")" << endl;
                    if (contadorUsuarios < 100) {
                        usuarios[contadorUsuarios++] = usuario->nombreUsuario;
                    }
                }
                usuario = usuario->abajo;
            }
            nodoEmpresa = nodoEmpresa->derecha;
        }
        nodoDepartamento = nodoDepartamento->derecha;
    }

    if (contadorUsuarios == 0) {
        cout << "No se encontraron usuarios en la empresa " << empresa << endl;
        return;
    }

    char respuesta;
    cout << "¿Desea generar un reporte de Graphviz? (s/n): ";
    cin >> respuesta;

    if (respuesta == 's' || respuesta == 'S') {
        generarReporteGraphvizActivosEmpresa(usuarios, contadorUsuarios, empresa);
    }
}

// funcion para generar el reporte de activos en graphviz dependiendo el usuario
void generarReporteGraphvizActivosUsuario(string nombreUsuario) {
    ofstream archivo("reporte_activos_usuario.dot");
    
    if (!archivo.is_open()) {
        cout << "Error al crear el archivo Graphviz." << endl;
        return;
    }

    // buscar el usuario en la matriz
    NodoMatriz* usuarioNodo = nullptr;
    NodoMatriz* nodoDepartamento = matrizUsuarios.cabeza;
    while (nodoDepartamento) {
        NodoMatriz* nodoEmpresa = nodoDepartamento->derecha;
        while (nodoEmpresa) {
            NodoMatriz* usuario = nodoEmpresa->abajo;
            while (usuario) {
                if (usuario->nombreUsuario == nombreUsuario) {
                    usuarioNodo = usuario;
                    break;
                }
                usuario = usuario->abajo;
            }
            if (usuarioNodo) break;
            nodoEmpresa = nodoEmpresa->derecha;
        }
        if (usuarioNodo) break;
        nodoDepartamento = nodoDepartamento->derecha;
    }

    
    archivo << "digraph ArbolActivos {\n";
    archivo << "    label=\"Reporte de Activos del Usuario: " << nombreUsuario << "\";\n";
    archivo << "    labelloc=\"t\";\n";
    archivo << "    fontsize=20;\n";
    archivo << "    node [shape=rectangle];\n";
    archivo << "    rankdir=TB;\n";

    // si se encuentra el usuario se genera el subarbol
    if (usuarioNodo && usuarioNodo->arbolAVL) {
        // nodo raiz con el nombre del usuario
        archivo << "    \"" << nombreUsuario << "\" [label=\"" << nombreUsuario << "\"];\n";
        
      
        generarSubArbolGraphvizConRaiz(archivo, nombreUsuario, usuarioNodo->arbolAVL);
    } else {
        cout << "Usuario no encontrado." << endl;
        return;
    }

    archivo << "}\n";
    archivo.close();

   
    string comando = "dot -Tpng reporte_activos_usuario.dot -o reporte_activos_usuario.png";
    system(comando.c_str());

    cout << "Reporte Graphviz generado exitosamente en reporte_activos_usuario.png" << endl;
}

// funcion para pedirle el usuario a consultar para el reporte
void reporteActivosDisponiblesUsuario() {

    string usuarios[100];
    int contadorUsuarios = 0;

    cout << "Lista de Usuarios:" << endl;

    // pasamos por toda la matriz buscando usuarios
    NodoMatriz* nodoDepartamento = matrizUsuarios.cabeza;
    while (nodoDepartamento != nullptr) {
        NodoMatriz* nodoEmpresa = nodoDepartamento->derecha;
        while (nodoEmpresa != nullptr) {
            NodoMatriz* usuario = nodoEmpresa->abajo;
            while (usuario != nullptr) {
                cout << contadorUsuarios + 1 << ". Usuario: " << usuario->nombreUsuario 
                     << " (Departamento: " << usuario->departamento 
                     << ", Empresa: " << nodoEmpresa->nombreUsuario << ")" << endl;
                
                if (contadorUsuarios < 100) {
                    usuarios[contadorUsuarios++] = usuario->nombreUsuario;
                }
                usuario = usuario->abajo;
            }
            nodoEmpresa = nodoEmpresa->derecha;
        }
        nodoDepartamento = nodoDepartamento->derecha;
    }

    if (contadorUsuarios == 0) {
        cout << "No se encontraron usuarios." << endl;
        return;
    }

    int seleccion;
    cout << "Seleccione el número de usuario para generar el reporte: ";
    cin >> seleccion;

    if (seleccion > 0 && seleccion <= contadorUsuarios) {
        char respuesta;
        cout << "¿Desea generar un reporte de Graphviz para " 
             << usuarios[seleccion-1] << "? (s/n): ";
        cin >> respuesta;

        if (respuesta == 's' || respuesta == 'S') {
            generarReporteGraphvizActivosUsuario(usuarios[seleccion-1]);
        }
    } else {
        cout << "Selección invalida." << endl;
    }
}

// Estructura para el nodo de la lista circular de transacciones
struct NodoTransaccion {
    string idTransaccion;
    string idActivo;
    string activo;
    string usuario;
    string departamento;
    string empresa;
    int diasRenta;
    time_t fechaRenta;
    string estadoTransaccion; 
    NodoTransaccion* siguiente;
    NodoTransaccion* anterior;

    NodoTransaccion(string idTrans, string idAct, string act, string user, string depto, 
                    string emp, int dias) 
        : idTransaccion(idTrans), idActivo(idAct), activo(act) , usuario(user), 
          departamento(depto), empresa(emp), diasRenta(dias) {
        time(&fechaRenta);
        time(&fechaRenta);
        siguiente = anterior = nullptr;
    }
};

// clase para la lista 2 circular de transacciones
class ListaTransacciones {
public:
    NodoTransaccion* cabeza;
    
public:
    ListaTransacciones() : cabeza(nullptr) {}

    void insertarTransaccion(string idTrans, string idAct, string act, string user, 
                           string depto, string emp, int dias) {
        NodoTransaccion* nuevo = new NodoTransaccion(idTrans, idAct, act, user, 
                                                    depto, emp, dias);

        if (!cabeza) {
            cabeza = nuevo;
            nuevo->siguiente = nuevo;
            nuevo->anterior = nuevo;
            return;
        }

        // buscar la posición correcta para insertar la transacción
        NodoTransaccion* actual = cabeza;
        do {
            if (nuevo->idTransaccion < actual->idTransaccion) {
                // insertar antes del nodo actual
                nuevo->siguiente = actual;
                nuevo->anterior = actual->anterior;
                actual->anterior->siguiente = nuevo;
                actual->anterior = nuevo;
                if (actual == cabeza) {
                    cabeza = nuevo;
                }
                return;
            }
            actual = actual->siguiente;
        } while (actual != cabeza);

        // si llega aquí, se inserta al final de la lista
        nuevo->siguiente = cabeza;
        nuevo->anterior = cabeza->anterior;
        cabeza->anterior->siguiente = nuevo;
        cabeza->anterior = nuevo;
    }

// funcion para mostrar datos de transacciones
    void mostrarTransacciones() {
        if (!cabeza) {
            cout << "No hay transacciones registradas.\n";
            return;
        }

        NodoTransaccion* actual = cabeza;
        do {
            cout << "\n-------- Transaccion --------\n";
            cout << "ID Transaccion: " << actual->idTransaccion << endl;
            cout << "ID Activo: " << actual->idActivo << endl;
            cout << "Activo: " << actual->activo << endl;
            cout << "Usuario: " << actual->usuario << endl;
            cout << "Departamento: " << actual->departamento << endl;
            cout << "Empresa: " << actual->empresa << endl;
            cout << "Dias de renta: " << actual->diasRenta << endl;
            cout << "Fecha de renta: " << ctime(&actual->fechaRenta);
            actual = actual->siguiente;
        } while (actual != cabeza);
    }
};

// Variable global para manejar las transacciones , le pedi permiso al aux :v
ListaTransacciones listaTransacciones;

// funcion para mostrar los activos del avl en orden
void mostrarActivosAVL(NodoAVL* nodo);

// esta variable es para la declaración de la funcion para generar el id del activo
string generarIDActivo();

// esta varibale es para delcarion de la función para generar un ID unico de transaccion
string generarIDActivo();

// Modificar la función de rentarActivo

// funcion para rentar un activo 
void rentarActivo() {
    NodoMatriz* usuarioActual = matrizUsuarios.buscarUsuario(departamento, empresa, nombreUsuario);
    if (!usuarioActual) {
        cout << "Error: Usuario no encontrado.\n";
        return;
    }

    cout << "---------------------------------------------------" << endl;
    cout << "-------------- Catalogo de Activos ----------------" << endl;
    cout << "---------------------------------------------------" << endl;

    // mostrar activos disponibles de todos los usuarios menos los del usuario actual
    bool hayActivos = false;
    NodoMatriz* deptoActual = matrizUsuarios.cabeza;
    while (deptoActual != nullptr) {
        NodoMatriz* empresaActual = deptoActual->derecha;
        while (empresaActual != nullptr) {
            NodoMatriz* userActual = empresaActual->abajo;
            while (userActual != nullptr) {
                // evitar mostrar los activos del usuario logeado
                if (userActual->nombreUsuario != nombreUsuario && userActual->arbolAVL) {
                    mostrarActivosDisponiblesAVL(userActual->arbolAVL);
                    hayActivos = true;
                }
                userActual = userActual->abajo;
            }
            empresaActual = empresaActual->derecha;
        }
        deptoActual = deptoActual->derecha;
    }

    if (!hayActivos) {
        cout << "No hay activos disponibles para rentar.\n";
        return;
    }

    string idActivoRentar;
    cout << "\nIngrese el ID del activo que desea rentar: ";
    cin >> idActivoRentar;

    int diasRenta;
    cout << "Ingrese el numero de dias para rentar: ";
    cin >> diasRenta;

    // buscar y validar el activo en los árboles AVL de otros usuarios
    NodoAVL* activoEncontrado = nullptr;
    NodoMatriz* propietarioActivo = nullptr; // con esta guardamosreferencia al propietario del activo

    deptoActual = matrizUsuarios.cabeza;
    while (deptoActual != nullptr) {
        NodoMatriz* empresaActual = deptoActual->derecha;
        while (empresaActual != nullptr) {
            NodoMatriz* userActual = empresaActual->abajo;
            while (userActual != nullptr) {
                // ignorar los activos del usuario logeado
                if (userActual->nombreUsuario != nombreUsuario && userActual->arbolAVL) {
                    NodoAVL* actual = userActual->arbolAVL;
                    while (actual != nullptr) {
                        if (idActivoRentar == actual->idActivo) {
                            // verificar si el activo está disponible
                            if (!actual->disponible) {
                                cout << "Error: El activo ya está rentado.\n";
                                return;
                            }

                            activoEncontrado = actual;
                            propietarioActivo = userActual; // guardar el propietario del activo

                            // validar el tiempo máximo de renta
                            if (diasRenta > activoEncontrado->tiempoMaximoRenta) {
                                cout << "Error: El tiempo solicitado excede el tiempo maximo permitido para este activo.\n";
                                return;
                            }

                            break;
                        } else if (idActivoRentar < actual->idActivo) {
                            actual = actual->izquierda;
                        } else {
                            actual = actual->derecha;
                        }
                    }
                }
                if (activoEncontrado) break; 
                userActual = userActual->abajo;
            }
            if (activoEncontrado) break; 
            empresaActual = empresaActual->derecha;
        }
        if (activoEncontrado) break; 
        deptoActual = deptoActual->derecha;
    }

    if (!activoEncontrado) {
        cout << "Error: No se encontró el activo especificado o no está disponible.\n";
        return;
    }

    // cambiar el estado de disponibilidad del activo a false (rentado)
    activoEncontrado->disponible = false;

    string idTransaccion = generarIDActivo(); // generar ID único para la transacción
    listaTransacciones.insertarTransaccion(
        idTransaccion, idActivoRentar, activoEncontrado->nombreActivo, nombreUsuario,
        departamento, empresa, diasRenta
    );
}

// funcion para el reporte de actvios rentados por usuario 
void generarReporteGraphvizActivosRentados() {
    
    string usuarios[100];
    int contadorUsuarios = 0;

    cout << "Lista de Usuarios:" << endl;

    // buscar usuarios en la matriz 
    NodoMatriz* nodoDepartamento = matrizUsuarios.cabeza;
    while (nodoDepartamento != nullptr) {
        NodoMatriz* nodoEmpresa = nodoDepartamento->derecha;
        while (nodoEmpresa != nullptr) {
            NodoMatriz* usuario = nodoEmpresa->abajo;
            while (usuario != nullptr) {
                cout << contadorUsuarios + 1 << ". Usuario: " << usuario->nombreUsuario
                     << " (Departamento: " << usuario->departamento
                     << ", Empresa: " << nodoEmpresa->nombreUsuario << ")" << endl;

                if (contadorUsuarios < 100) {
                    usuarios[contadorUsuarios++] = usuario->nombreUsuario;
                }
                usuario = usuario->abajo;
            }
            nodoEmpresa = nodoEmpresa->derecha;
        }
        nodoDepartamento = nodoDepartamento->derecha;
    }

    if (contadorUsuarios == 0) {
        cout << "No se encontraron usuarios." << endl;
        return;
    }

    int seleccion;
    cout << "Seleccione el número de usuario para generar el reporte: ";
    cin >> seleccion;

    if (seleccion > 0 && seleccion <= contadorUsuarios) {
        string usuarioSeleccionado = usuarios[seleccion-1];
        
        // función para reemplazar comillas por espacios
        auto reemplazarComillas = [](string& texto) {
            for (int i = 0; texto[i] != '\0'; i++) {
                if (texto[i] == '"') {
                    texto[i] = ' ';
                }
            }
        };


        ofstream archivo("reporte_activos_" + usuarioSeleccionado + ".dot");
        if (!archivo) {
            cout << "Error al crear el archivo de reporte." << endl;
            return;
        }

   
        archivo << "digraph ActivosRentados {\n";
        archivo << "    rankdir=LR;\n";
        archivo << "    node [shape=record, style=filled, color=lightblue];\n";
        archivo << "    label=\"Activos Rentados por " << usuarioSeleccionado << "\";\n\n";

        // variables para contar y rastrear activos
        int contadorActivos = 0;
        bool tieneActivos = false;

        // recorrer las transacciones
        NodoTransaccion* actual = listaTransacciones.cabeza;
        do {
            if (actual->usuario == usuarioSeleccionado) {
                tieneActivos = true;
                
                // preparar strings para escape de comillas y formato de fecha
                string activo = actual->activo;
                string departamento = actual->departamento;
                string empresa = actual->empresa;
                
                // Reemplazar comillas
                reemplazarComillas(activo);
                reemplazarComillas(departamento);
                reemplazarComillas(empresa);

                // cnvertir fecha a string pinche bufer
                char fechaBuffer[26];
                ctime_s(fechaBuffer, sizeof(fechaBuffer), &actual->fechaRenta);

                archivo << "    " << actual->idActivo << " [label=\"{ID del Activo: " << actual->idActivo 
                        << "|Activo: " << activo
                        << "|Departamento: " << departamento 
                        << "|Empresa: " << empresa 
                        << "|Dias de renta: " << actual->diasRenta 
                        << "|Fecha de renta: " << fechaBuffer << "}\"];\n";
                contadorActivos++;
            }
            actual = actual->siguiente;
        } while (actual != listaTransacciones.cabeza);

        archivo << "}\n";
        archivo.close();

        if (!tieneActivos) {
            cout << "El usuario " << usuarioSeleccionado << " no tiene activos rentados." << endl;
            remove(("reporte_activos_" + usuarioSeleccionado + ".dot").c_str());
            return;
        }

      
        string comando = "dot -Tpng reporte_activos_" + usuarioSeleccionado + ".dot -o reporte_activos_" + usuarioSeleccionado + ".png";
        int resultado = system(comando.c_str());

        if (resultado == 0) {
            cout << "Reporte generado exitosamente para " << usuarioSeleccionado 
                 << ". Total de activos: " << contadorActivos << endl;
            cout << "Archivo: reporte_activos_" << usuarioSeleccionado << ".png" << endl;
        } else {
            cout << "Error al generar el reporte de Graphviz." << endl;
        }
    } else {
        cout << "Selección invalida." << endl;
    }
}

// funcion para devolver un activo (cambiar a disponible)
void devolverActivo(string idActivo) {
    // buscar el activo en todos los avl de los usuarios para cambiar el estado a disponible
    NodoMatriz* deptoActual = matrizUsuarios.cabeza;
    while (deptoActual != nullptr) {
        NodoMatriz* empresaActual = deptoActual->derecha;
        while (empresaActual != nullptr) {
            NodoMatriz* userActual = empresaActual->abajo;
            while (userActual != nullptr) {
                NodoAVL* actual = userActual->arbolAVL;
                while (actual != nullptr) {
                    if (idActivo == actual->idActivo) {
                        // encontrar y cambiar el estado a disponible
                        actual->disponible = true;
                        cout << "Activo " << idActivo << " devuelto correctamente\n";
                        return;
                    } else if (idActivo < actual->idActivo) {
                        actual = actual->izquierda;
                    } else {
                        actual = actual->derecha;
                    }
                }
                userActual = userActual->abajo;
            }
            empresaActual = empresaActual->derecha;
        }
        deptoActual = deptoActual->derecha;
    }

    cout << "No se encontro el activo especificado.\n";
}

// funcion para eliminar una transacción de la lista circular
void eliminarTransaccion(string idTransaccion) {
    if (!listaTransacciones.cabeza) return;

    NodoTransaccion* actual = listaTransacciones.cabeza;
    NodoTransaccion* anterior = nullptr;

    // recorremos s la lista para encontrar la transaccion
    do {
        if (actual->idTransaccion == idTransaccion) {
            // si es el único nodo
            if (actual->siguiente == actual) {
                listaTransacciones.cabeza = nullptr;
                delete actual;
                return;
            }

            // si es la cabeza
            if (actual == listaTransacciones.cabeza) {
                listaTransacciones.cabeza = actual->siguiente;
            }

            // ajustar los punteros para "saltar" el nodo a eliminar
            if (anterior) {
                anterior->siguiente = actual->siguiente;
            }

            // liberar memoria su no peta xd
            delete actual;
            return;
        }

        anterior = actual;
        actual = actual->siguiente;
    } while (actual != listaTransacciones.cabeza);
}


// funcion para mostrar los activos rentados por el usuario actual
void activosRentados() {
    // verificar si hay transacciones registradas
    bool tieneTransacciones = false;
    NodoTransaccion* actual = listaTransacciones.cabeza;
    
    if (!actual) {
        cout << "No hay activos rentados.\n";
        return;
    }

   
    cout << "---------------------------------------------------" << endl;
    cout << "-------------- Activos Rentados -------------------" << endl;
    cout << "---------------------------------------------------" << endl;

    string activosRentadosUsuario[100];  // ajustas el tamaño si es necesario no creo que nos meta 200 usuarios o si ....
    int contadorActivos = 0;

    
    do {
        if (actual->usuario == nombreUsuario) {
            tieneTransacciones = true;
            
            cout << "\n-------- Activo Rentado --------\n";
            cout << "ID Transaccion: " << actual->idTransaccion << endl;
            cout << "ID Activo: " << actual->idActivo << endl;
            cout << "Activo: " << actual->activo << endl;
            cout << "Usuario: " << actual->usuario << endl;
            cout << "Departamento: " << actual->departamento << endl;
            cout << "Empresa: " << actual->empresa << endl;
            cout << "Dias de renta: " << actual->diasRenta << endl;
            cout << "Fecha de renta: " << ctime(&actual->fechaRenta);
            
            // guardar el ID del activo para lo de la  devolucion
            activosRentadosUsuario[contadorActivos++] = actual->idActivo;
        }
        actual = actual->siguiente;
    } while (actual != listaTransacciones.cabeza);

    // si no hay transacciones para este usuario
    if (!tieneTransacciones) {
        cout << "No tiene activos rentados en este momento.\n";
        return;
    }

    // opcion de devolucion de activo
    char opcion;
    cout << "\n¿Desea devolver un activo? (S/N): ";
    cin >> opcion;

    if (opcion == 'S' || opcion == 's') { // si es S o s es que si quiere devolver si no pos no
        string idActivoDevolver;
        cout << "Ingrese el ID del activo a devolver: ";
        cin >> idActivoDevolver;

        // buscar la transacción del activo a devolver
        NodoTransaccion* transaccionADevolver = nullptr;
        actual = listaTransacciones.cabeza;
        do {
            if (actual->usuario == nombreUsuario && actual->idActivo == idActivoDevolver) {
                transaccionADevolver = actual;
                break;
            }
            actual = actual->siguiente;
        } while (actual != listaTransacciones.cabeza);

        if (transaccionADevolver) {
           
            cout << "\n--- Resumen de Devolucion ---\n";
            cout << "ID Activo: " << transaccionADevolver->idActivo << endl;
            cout << "Activo rentado: " << transaccionADevolver->activo << endl;
            cout << "Dias rentados: " << transaccionADevolver->diasRenta << endl;
            
            // devolver el activo (cambiar a disponible)
            devolverActivo(transaccionADevolver->idActivo);

            // Marcar la transacción como devuelta (sin eliminarla)
            transaccionADevolver->estadoTransaccion = "DEVUELTO";

            cout << "Activo devuelto exitosamente.\n";
        } else {
            cout << "No se encontró un activo con ese ID entre sus rentados.\n";
        }
    }
}

//  funicion de la propietorio de un activo
bool verificarPropiedadActivo(const string& idActivo, const string& nombreUsuario) {
    NodoMatriz* usuarioActual = matrizUsuarios.buscarUsuario(departamento, empresa, nombreUsuario);
    if (!usuarioActual || !usuarioActual->arbolAVL) {
        return false;
    }

    NodoAVL* activo = buscarActivoEnAVL(usuarioActual->arbolAVL, idActivo);
    return activo != nullptr; // retorna true si el activo fue encontrado
}


// funcion para mostrar los activos rentados por otros usuarios pero que son del usuario actual
void mostrarMisActivosRentados() {
    if (!listaTransacciones.cabeza) {
        cout << "No hay transacciones registradas.\n";
        return;
    }

    cout << "---------------------------------------------------" << endl;
    cout << "------------- Mis Activos Rentados ---------------" << endl;
    cout << "---------------------------------------------------" << endl;

    bool hayActivosRentados = false;
    NodoTransaccion* actual = listaTransacciones.cabeza;

    do {
        
        if (actual->usuario != nombreUsuario && verificarPropiedadActivo(actual->idActivo, nombreUsuario)) {
            hayActivosRentados = true;
            cout << "\n-------- Activo Rentado --------\n";
            cout << "ID Transaccion: " << actual->idTransaccion << endl;
            cout << "ID Activo: " << actual->idActivo << endl;
            cout << "Activo: " << actual->activo << endl;
            cout << "Usuario que rento: " << actual->usuario << endl;
            cout << "Departamento: " << actual->departamento << endl;
            cout << "Empresa: " << actual->empresa << endl;
            cout << "Dias de renta: " << actual->diasRenta << endl;
            cout << "Fecha de renta: " << ctime(&actual->fechaRenta);
        }
        actual = actual->siguiente;
    } while (actual != listaTransacciones.cabeza);

    if (!hayActivosRentados) {
        cout << "No tiene activos rentados por otros usuarios en este momento.\n";
    }
}

// funcion para mostrar los activos rentados por el usuario actual
void mostrarActivosRentados() {
    if (!listaTransacciones.cabeza) {
        cout << "No hay activos rentados.\n";
        return;
    }

    cout << "---------------------------------------------------" << endl;
    cout << "-------------- Activos Rentados ----------------" << endl;
    cout << "---------------------------------------------------" << endl;

    NodoTransaccion* actual = listaTransacciones.cabeza;
    do {
        
        if (actual->usuario == nombreUsuario) {
            cout << "\n-------- Transaccion --------\n";
            cout << "ID Transaccion: " << actual->idTransaccion << endl;
            cout << "ID Activo: " << actual->idActivo << endl;
            cout << "Activo: " << actual->activo << endl;
            cout << "Usuario: " << actual->usuario << endl;
            cout << "Departamento: " << actual->departamento << endl;
            cout << "Empresa: " << actual->empresa << endl;
            cout << "Dias de renta: " << actual->diasRenta << endl;
            cout << "Fecha de renta: " << ctime(&actual->fechaRenta);
        }
        actual = actual->siguiente;
    } while (actual != listaTransacciones.cabeza);
}

// funcion para reporte de transacciones osea la lista 2 circular en el estado actual luego colocar lo de asc o desc
void generarReporteGraphvizListaCircularDoble(ListaTransacciones& lista) {
    if (!lista.cabeza) {
        cout << "No hay transacciones para generar el reporte Graphviz.\n";
        return;
    }

    ofstream archivo("reporte_transacciones.dot");
    if (!archivo) {
        cerr << "Error al crear el archivo DOT.\n";
        return;
    }

   
    archivo << "digraph ListaTransacciones {\n";
    archivo << "    rankdir=LR;\n";  
    archivo << "    node [shape=record, style=filled, fillcolor=lightblue];\n";

    NodoTransaccion* actual = lista.cabeza;
    int contador = 0;

    do {
        
        auto escaparCadena = [](const string& str) {
            string resultado;
            for (char c : str) {
                switch (c) {
                    case '"': resultado += "\\\""; break;
                    case '\\': resultado += "\\\\"; break;
                    default: resultado += c;
                }
            }
            return resultado;
        };

       
        archivo << "    nodo" << contador << " [label=\"{ID Trans: " 
                << escaparCadena(actual->idTransaccion) << "\\n"
                << "ID Activo: " << escaparCadena(actual->idActivo) << "\\n"
                << "Activo: " << escaparCadena(actual->activo) << "\\n"
                << "Usuario: " << escaparCadena(actual->usuario) << "\\n"
                << "Departamento: " << escaparCadena(actual->departamento) << "\\n"
                << "Empresa: " << escaparCadena(actual->empresa) << "\\n"
                << "Días Renta: " << actual->diasRenta << "\\n"
                << "Fecha Renta: " << ctime(&actual->fechaRenta) << "}\"];\n";

        // creamos cada enlace entre nodos
        if (contador > 0) {
            archivo << "    nodo" << (contador-1) << " -> nodo" << contador << " [color=blue];\n";
            archivo << "    nodo" << contador << " -> nodo" << (contador-1) << " [color=red, style=dashed];\n";
        }

        actual = actual->siguiente;
        contador++;
    } while (actual != lista.cabeza);

    // enlazar el último con el primero para mostrar la circularidad
    archivo << "    nodo" << (contador-1) << " -> nodo0 [color=green, constraint=false];\n";
    archivo << "    nodo0 -> nodo" << (contador-1) << " [color=green, style=dashed, constraint=false];\n";

    archivo << "}\n";
    archivo.close();

    
    system("dot -Tpng reporte_transacciones.dot -o reporte_transacciones.png");
    cout << "Reporte Graphviz generado: reporte_transacciones.png\n";
}

// funcione para ordenar la lista de trsansaciones si asc o desc
void OrdenarTransacciones(ListaTransacciones& lista) {
    // verificar si la lista está vacia o tiene un solo elemento
    if (!lista.cabeza || lista.cabeza->siguiente == lista.cabeza) {
        cout << "La lista está vacia o tiene un solo elemento. No se puede ordenar.\n";
        return;
    }

   
    int opcion;
    cout << "\n--- Ordenar Transacciones ---\n";
    cout << "1. Ordenar Ascendentemente (A-Z, 1-9)\n";
    cout << "2. Ordenar Descendentemente (Z-A, 9-1)\n";
    cout << "Ingrese su opcion: ";
    cin >> opcion;

    // validamos la entrada del usuario
    if (opcion != 1 && opcion != 2) {
        cout << "Opcion invalida. Se cancelo el ordenamiento.\n";
        return;
    }

    // definir si es ascendente o descendente
    bool ascendente = (opcion == 1);

    // función de comparacion personalizada para IDs alfanuméricos
    auto compararIDs = [](const string& a, const string& b) -> bool {
        // Comparación alfanumérica
        int i = 0, j = 0;
        while (i < a.length() && j < b.length()) {
            // Si son dígitos, comparar como números
            if (isdigit(a[i]) && isdigit(b[j])) {
                long long numA = 0, numB = 0;
                while (i < a.length() && isdigit(a[i])) {
                    numA = numA * 10 + (a[i] - '0');
                    i++;
                }
                while (j < b.length() && isdigit(b[j])) {
                    numB = numB * 10 + (b[j] - '0');
                    j++;
                }
                if (numA != numB) return numA < numB;
            }
            
            // Si son letras, comparar como caracteres
            if (a[i] != b[j]) return a[i] < b[j];
            
            i++;
            j++;
        }
        
        // Si una cadena es más corta, se considera menor
        return a.length() < b.length();
    };

    // contar numero de nodos
    int numNodos = 0;
    NodoTransaccion* actual = lista.cabeza;
    do {
        numNodos++;
        actual = actual->siguiente;
    } while (actual != lista.cabeza);

    // Algoritmo de ordenamiento burbujita para manejar a lista circular 2 enlazada
    for (int i = 0; i < numNodos - 1; i++) {
        bool intercambiado = false;
        actual = lista.cabeza;
        
        for (int j = 0; j < numNodos - i - 1; j++) {
            NodoTransaccion* siguiente = actual->siguiente;
            
            // comparar los ids de transaccion
            bool necesitaIntercambio = ascendente ? 
                compararIDs(siguiente->idTransaccion, actual->idTransaccion) : 
                compararIDs(actual->idTransaccion, siguiente->idTransaccion);
            
            if (necesitaIntercambio) {
                // intercambiar información de los nodos (no los punteros)
                swap(actual->idTransaccion, siguiente->idTransaccion);
                swap(actual->idActivo, siguiente->idActivo);
                swap(actual->activo, siguiente->activo);
                swap(actual->usuario, siguiente->usuario);
                swap(actual->departamento, siguiente->departamento);
                swap(actual->empresa, siguiente->empresa);
                swap(actual->diasRenta, siguiente->diasRenta);
                swap(actual->fechaRenta, siguiente->fechaRenta);

                intercambiado = true;
            }
            
            actual = siguiente;
        }
        
        // si no hubo intercambios, la lista ya está ordenada
        if (!intercambiado) break;
    }

    
    cout << "Transacciones ordenadas " 
         << (ascendente ? "ascendentemente" : "descendentemente") 
         << " por ID de Transacción.\n";
}

// funcion para mostrar los activos del avl en orden
void mostrarActivosAVL(NodoAVL* nodo) {
    if (!nodo)
        return;

    mostrarActivosAVL(nodo->izquierda);
    cout << ">> ID: " << nodo->idActivo << "; Nombre = " << nodo->nombreActivo
        << "; Descripcion: " << nodo->descripcion << "; Tiempo Maximo de Renta = " << nodo->tiempoMaximoRenta << endl;
    mostrarActivosAVL(nodo->derecha);
}

// funcion para eliminar un activo del usuario
void eliminarActivo() {
    NodoMatriz* usuarioActual = matrizUsuarios.buscarUsuario(departamento, empresa, nombreUsuario);

    if (!usuarioActual || !usuarioActual->arbolAVL) {
        cout << "No tienes activos registrados para eliminar.\n";
        return;
    }

    cout << "Lista de activos disponibles:\n";
    mostrarActivosAVL(usuarioActual->arbolAVL);

    cout << "Ingrese el ID del activo que desea eliminar: ";
    string idActivoEliminar;
    cin >> idActivoEliminar;

    // guardar la información del activo antes de eliminarlo antes me lo mochaba pero ya no jajajaj
    string nombreActivoEliminado;
    string descripcionActivoEliminado;
    int tiempoMaximoRentaActivoEliminado;
    
    // buscar el activo y guardar su información
    NodoAVL* actual = usuarioActual->arbolAVL;
    bool encontrado = false;
    
    while (actual != nullptr && !encontrado) {
        if (idActivoEliminar == actual->idActivo) {
            nombreActivoEliminado = actual->nombreActivo;
            descripcionActivoEliminado = actual->descripcion;
            tiempoMaximoRentaActivoEliminado = actual->tiempoMaximoRenta;
            encontrado = true;
        } else if (idActivoEliminar < actual->idActivo) {
            actual = actual->izquierda;
        } else {
            actual = actual->derecha;
        }
    }

    NodoAVL* arbolModificado = eliminarNodoAVL(usuarioActual->arbolAVL, idActivoEliminar);

    if (arbolModificado != usuarioActual->arbolAVL || encontrado) {
        usuarioActual->arbolAVL = arbolModificado;
        
        cout << "---------------------------------------------------" << endl;
        cout << "-------------- Eliminando Activo ------------------" << endl;
        cout << "---------------------------------------------------" << endl;
        cout << ">> Activo Eliminado;\n";
        cout << ">> ID = " << idActivoEliminar << "\n";
        cout << ">> Nombre = " << nombreActivoEliminado << "\n";
        cout << ">> descripcion = " << descripcionActivoEliminado << "\n\n";
        cout << ">> Tiempo Maximo de Renta = " << tiempoMaximoRentaActivoEliminado << "\n";
    } else {
        cout << "Error: No se encontro un activo con el ID especificado.\n";
    }
}


// funcion para generar un carácter aleatorio
char generarCaracterAleatorio() {
    //generan números pseudoaleatorios
    static unsigned long seed1 = 1;
    static unsigned long seed2 = 0;
    const std::string caracteres = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    
    // combinar dos semillas
    seed1 = (seed1 * 24271 + 3522483246);
    seed2 = (seed2 * 2143618259 + 12345);
    
    unsigned long combinedSeed = seed1 ^ seed2;
    return caracteres[combinedSeed % caracteres.length()];
}

// funncion para generar un ID uunico de 15 caracteres alfanuméricos
string generarIDActivo() {
    string id;
    
    for (int i = 0; i < 15; i++) {
        id += generarCaracterAleatorio();
    }
    return id;
}

// funcion para iniciar sesion
bool iniciarSesion() {
    cout << "Ingresar Nombre de Usuario: ";
    cin >> nombreUsuario;
    cout << "Ingresar Contrasena: ";
    cin >> contrasena;

    // verificar si es un usuario admin
    if (nombreUsuario == "admin" && contrasena == "admin") {
        departamento = "N/A";
        empresa = "N/A";
        return true; // inicia sesion como admin
    } else {
        cout << "Ingresar Departamento: ";
        cin >> departamento;
        cout << "Ingresar Empresa: ";
        cin >> empresa;

        NodoMatriz* nodoUsuario = matrizUsuarios.buscarUsuario(departamento, empresa, nombreUsuario);

        if (nodoUsuario != nullptr && nodoUsuario->contrasena == contrasena) {
            cout << "Inicio de sesion exitoso. Bienvenido, " << nodoUsuario->nombreUsuario << "!\n";
            return false; // inicia sesion como usuario normal
        } else {
            cout << "Error: Usuario o contrasena incorrectos.\n";
            return false; 
        }
    }
}

// funcion para registrar usuarios
void registrarUsuario() {
    string usuario, pass, depto, emp;
    cout << "---------------------------------------------------" << endl;
    cout << "----------------Registrar Usuario-------------------" << endl;
    cout << "---------------------------------------------------" << endl;

    cout << "Ingrese el nombre de usuario: ";
    cin >> usuario;

    cout << "Ingrese la contrasena: ";
    cin >> pass;

    cout << "Ingrese el departamento: ";
    cin >> depto;

    cout << "Ingrese la empresa: ";
    cin >> emp;

    matrizUsuarios.insertarUsuario(depto, emp, usuario, pass);
}

// funcion para agregar un activo
void agregarActivo() {
    string nombreActivo, descripcionActivo, idActivo;
    int tiempoMaximoRenta;

    cout << "Ingrese el nombre del nuevo activo: ";
    cin.ignore();
    getline(cin, nombreActivo);

    cout << "Ingrese la descripcion del nuevo activo: ";
    getline(cin, descripcionActivo);

    cout << "Ingrese el tiempo maximo de renta (en dias): ";
    cin >> tiempoMaximoRenta;

    idActivo = generarIDActivo();
    cout << "El ID unico del nuevo activo es: " << idActivo << endl;

    NodoMatriz* usuarioActual = matrizUsuarios.buscarUsuario(departamento, empresa, nombreUsuario);

    if (!usuarioActual) {
        cout << "Error: Usuario no encontrado. No se puede agregar el activo.\n";
        return;
    }

    usuarioActual->arbolAVL = insertarEnAVL(
        usuarioActual->arbolAVL, idActivo, nombreActivo, descripcionActivo, tiempoMaximoRenta
    );
    cout << "Activo agregado exitosamente.\n";
}

// funcion para modificar un activo
void modificarActivo() {
    NodoMatriz* usuarioActual = matrizUsuarios.buscarUsuario(departamento, empresa, nombreUsuario);

    if (!usuarioActual || !usuarioActual->arbolAVL) {
        cout << "No tienes activos registrados para modificar.\n";
        return;
    }

    // mostrar la lista de activos
    cout << "Lista de activos disponibles:\n";
    mostrarActivosAVL(usuarioActual->arbolAVL);

    // pedir al usuario el ID del activo a modificar
    cout << "Ingrese el ID del activo que desea modificar: ";
    string idActivoModificar;
    cin >> idActivoModificar;

    // Buscar el activo en el árbol AVL
    NodoAVL* actual = usuarioActual->arbolAVL;
    NodoAVL* activoEncontrado = nullptr;

    while (actual != nullptr && activoEncontrado == nullptr) {
        if (idActivoModificar == actual->idActivo) {
            activoEncontrado = actual;
        } else if (idActivoModificar < actual->idActivo) {
            actual = actual->izquierda;
        } else {
            actual = actual->derecha;
        }
    }

    // validar si el activo fue encontrado
    if (activoEncontrado) {
        cout << "\nActivo seleccionado:\n";
        cout << ">> ID: " << activoEncontrado->idActivo << "\n";
        cout << ">> Nombre: " << activoEncontrado->nombreActivo << "\n";
        cout << ">> Descripcion actual: " << activoEncontrado->descripcion << "\n";
        cout << ">> Tiempo Maximo de Renta: " << activoEncontrado->tiempoMaximoRenta << "\n";

        // pedir la nueva descripción
        cout << "Ingrese la nueva descripcion para este activo: ";
        cin.ignore();
        string nuevaDescripcion;
        getline(cin, nuevaDescripcion);

        // actualizar la descripcion del activo
        activoEncontrado->descripcion = nuevaDescripcion;

        // Confirmar la modificación
        cout << "\n------Descripcion modificada exitosamente------\n";
        cout << ">> ID: " << activoEncontrado->idActivo << "\n";
        cout << ">> Nombre: " << activoEncontrado->nombreActivo << "\n";
        cout << ">> Nueva Descripcion: " << activoEncontrado->descripcion << "\n";
        cout << ">> Tiempo Maximo de Renta: " << activoEncontrado->tiempoMaximoRenta << "\n";
    } else {
        cout << "Error: No se encontró un activo con el ID especificado.\n";
    }
}

// menu de usuario ya ingresado 
void menuUsuario() {
    int opcion;
    const string RESET = "\033[0m";
    const string BLUE = "\033[1;34m";
    const string GREEN = "\033[1;32m";
    const string RED = "\033[1;31m";
    const string YELLOW = "\033[1;33m";
    const string CYAN = "\033[1;36m";

    do {
        // limpia la pantall
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif

        // Encabezado
        cout << BLUE << "+================================================+" << RESET << endl;
        cout << BLUE << "|                SISTEMA DE GESTION              |" << RESET << endl;
        cout << BLUE << "|                  -= Usuario =-                 |" << RESET << endl;
        cout << BLUE << "+================================================+" << RESET << endl << endl;

        
        cout << CYAN << "  [Gestion de Activos]" << RESET << endl;
        cout << YELLOW << "  [1] " << GREEN << "Agregar Nuevo Activo" << RESET << endl;
        cout << YELLOW << "  [2] " << GREEN << "Eliminar Activo" << RESET << endl;
        cout << YELLOW << "  [3] " << GREEN << "Modificar Activo" << RESET << endl << endl;

       
        cout << CYAN << "  [Sistema de Rentas]" << RESET << endl;
        cout << YELLOW << "  [4] " << GREEN << "Rentar Nuevo Activo" << RESET << endl;
        cout << YELLOW << "  [5] " << GREEN << "Ver Todos los Activos en Renta" << RESET << endl;
        cout << YELLOW << "  [6] " << GREEN << "Mis Activos Rentados" << RESET << endl << endl;

      
        cout << CYAN << "  [Sesion]" << RESET << endl;
        cout << YELLOW << "  [7] " << RED << "Cerrar Sesion" << RESET << endl << endl;

        cout << BLUE << "  -> Seleccione una opcion: " << RESET;
        
        // validacion de entrada
        if (!(cin >> opcion)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << RED << "\n  X Error: Ingrese un número valido" << RESET << endl;
            system("pause");
            continue;
        }

        cout << endl;

        try {
            switch (opcion) {
                case 1: {
                    cout << CYAN << "  >> Iniciando registro de activo..." << RESET << endl;
                    agregarActivo();
                    break;
                }
                case 2: {
                    cout << CYAN << "  >> Proceso de eliminación de activo..." << RESET << endl;
                    eliminarActivo();
                    break;
                }
                case 3: {
                    cout << CYAN << "  >> Modificando activo..." << RESET << endl;
                    modificarActivo();
                    break;
                }
                case 4: {
                    cout << CYAN << "  >> Iniciando proceso de renta..." << RESET << endl;
                    rentarActivo();
                    break;
                }
                case 5: {
                    cout << CYAN << "  >> Consultando activos en renta..." << RESET << endl;
                    activosRentados();
                    break;
                }
                case 6: {
                    cout << CYAN << "  >> Consultando mis activos rentados..." << RESET << endl;
                    mostrarMisActivosRentados();
                    break;
                }
                case 7: {
                    cout << GREEN << "  <<-- Cerrando sesión..." << RESET << endl;
                    cout << GREEN << "  ¡Hasta pronto! :3 " << RESET << endl;
                    return;
                }
                default: {
                    cout << RED << "  X Error: Opción invalida (1-7)" << RESET << endl;
                }
            }
        } catch (const exception& e) {
            cout << RED << "  X Error: " << e.what() << RESET << endl;
        }

        cout << endl;
        cout << BLUE << "  Presione cualquier tecla para continuar..." << RESET << endl;
        system("pause");
    } while (true);
}

// memu de administrador
void menuAdministrador() {
    int opcion;
    const string RESET = "\033[0m";
    const string BLUE = "\033[1;34m";
    const string GREEN = "\033[1;32m";
    const string RED = "\033[1;31m";
    const string YELLOW = "\033[1;33m";
    const string CYAN = "\033[1;36m";

    do {
        // limpia la pantalla carrea duro esto jajaja debi usarlo antes jajaja
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif

        cout << BLUE << "+================================================+" << RESET << endl;
        cout << BLUE << "|                PANEL DE CONTROL                 |" << RESET << endl;
        cout << BLUE << "|              -= Administrador =-                |" << RESET << endl;
        cout << BLUE << "+================================================+" << RESET << endl << endl;

       
        cout << CYAN << "  [Gestion de Usuarios]" << RESET << endl;
        cout << YELLOW << "  [1] " << GREEN << "Registrar Nuevo Usuario" << RESET << endl << endl;

        cout << CYAN << "  [Reportes y Visualizacion]" << RESET << endl;
        cout << YELLOW << "  [2] " << GREEN << "Visualizar Matriz Dispersa" << RESET << endl;
        cout << YELLOW << "  [3] " << GREEN << "Reporte de Activos por Departamento" << RESET << endl;
        cout << YELLOW << "  [4] " << GREEN << "Reporte de Activos por Empresa" << RESET << endl;
        cout << YELLOW << "  [5] " << GREEN << "Reporte de Transacciones" << RESET << endl;
        cout << YELLOW << "  [6] " << GREEN << "Reporte de Activos por un Usuario" << RESET << endl;
        cout << YELLOW << "  [7] " << GREEN << "Activos en Rentados por un Usuario" << RESET << endl << endl;

        
        cout << CYAN << "  [Herramientas]" << RESET << endl;
        cout << YELLOW << "  [8] " << GREEN << "Ordenar Registro de Transacciones" << RESET << endl;
        cout << YELLOW << "  [9] " << RED << "Regresar al Menu Principal O_O" << RESET << endl << endl;

        cout << BLUE << "  -> Ingrese su opcion: " << RESET;
        
        // validación de entrada que si ponemos un string no se caiga pedi usar limits :v 
        if (!(cin >> opcion)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << RED << "\n  X Error: Por favor ingrese un numero valido" << RESET << endl;
            system("pause");
            continue;
        }

        cout << endl;

        try {
            switch (opcion) {
                case 1: {
                    cout << CYAN << "  >> Iniciando registro de usuario..." << RESET << endl;
                    registrarUsuario();
                    break;
                }
                case 2: {
                    cout << CYAN << "  >> Generando visualización de matriz..." << RESET << endl;
                    matrizUsuarios.generarReporteGraphviz();
                    break;
                }
                case 3: {
                    cout << CYAN << "  >> Preparando reporte de departamento..." << RESET << endl;
                    reporteActivosDisponiblesDepartamento();
                    break;
                }
                case 4: {
                    cout << CYAN << "  >> Preparando reporte de empresa..." << RESET << endl;
                    reporteActivosDisponiblesEmpresa();
                    break;
                }
                case 5: {
                    cout << CYAN << "  >> Generando historial de transacciones..." << RESET << endl;
                    generarReporteGraphvizListaCircularDoble(listaTransacciones);
                    break;
                }
                case 6: {
                    cout << CYAN << "  >> Consultando activos por usuario..." << RESET << endl;
                    reporteActivosDisponiblesUsuario();
                    break;
                }
                case 7: {
                    cout << CYAN << "  >> Generando reporte de activos rentados..." << RESET << endl;
                    generarReporteGraphvizActivosRentados();
                    break;
                }
                case 8: {
                    cout << CYAN << "  >> Ordenando transacciones..." << RESET << endl;
                    OrdenarTransacciones(listaTransacciones);
                    break;
                }
                case 9: {
                    cout << GREEN << "  <<-- Regresando al menu principal..." << RESET << endl;
                    return;
                }
                default: {
                    cout << RED << "  X Error: Opcion invalida (1-9)" << RESET << endl;
                }
            }
        } catch (const exception& e) {
            cout << RED << "  X Error: " << e.what() << RESET << endl;
        }

        cout << endl;
        system("pause");
    } while (true);
}

// funcion para obtener una opcion valida lo que decia antes de usar numeric limits
int obtenerOpcionValida() {
    int opcion;
    while (true) {
        if (cin >> opcion) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return opcion;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "\033[1;31m[ERROR] Por favor, ingrese un numero valido.\033[0m" << endl;
        cout << "\033[1;34mIngresar Opcion: \033[0m";
    }
}

// decorar cuando ya tenga todo listo

// menu principal
int main() {
    int opcion;
    do {
        
        const string RESET = "\033[0m";
        const string BLUE = "\033[1;34m";
        const string GREEN = "\033[1;32m";
        const string RED = "\033[1;31m";
        const string YELLOW = "\033[1;33m";

        
        const string HEADER = 
            "+------------------------------------------------+\n"
            "|               Sistema de Activos                 |\n"
            "+------------------------------------------------+";
            
        const string WELCOME_SYMBOL = "  /\\_/\\  ";
        const string LOCK_SYMBOL = " [>=<] ";

      
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif

        cout << BLUE << HEADER << RESET << endl;
        cout << GREEN << WELCOME_SYMBOL << RESET << endl;
        cout << BLUE << "       Bienvenido al sistema de renta de activos" << RESET << endl;
        cout << BLUE << "+------------------------------------------------+" << RESET << endl << endl;
        
        cout << YELLOW << "[1]" << GREEN << " Iniciar sesion " << LOCK_SYMBOL << RESET << endl;
        cout << YELLOW << "[2]" << RED << " Salir del sistema  O_O" << RESET << endl << endl;
        
        cout << BLUE << "Ingresar Opcion: " << RESET;
        opcion = obtenerOpcionValida();

        switch (opcion) {
            case 1: {
                bool esAdmin = iniciarSesion();
                if (esAdmin) {
                    menuAdministrador();
                } else if (!esAdmin && matrizUsuarios.buscarUsuario(departamento, empresa, nombreUsuario) != nullptr) {
                    menuUsuario();
                } else {
                    cout << RED << "X Error: Credenciales invalidas" << RESET << endl;
                    cout << RED << "<<-- Regresando al menu principal..." << RESET << endl;
                    system("pause");
                }
                break;
            }
            case 2: {
                cout << GREEN << ">> Cerrando el sistema..." << RESET << endl;
                cout << GREEN << "¡Hasta pronto!" << RESET << endl;
                system("pause");
                break;
            }
            default: {
                cout << RED << "X Error: Opcion invalida (1-2)" << RESET << endl;
                system("pause");
            }
        }
    } while (opcion != 2);

    return 0;
}

// los colores y validaciones de como hacer un menu me lo jale de un video ajajajaj por si se pregunta porque muy colorido XDDD