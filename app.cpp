#include <iostream>
#include <string>
#include <limits>
#include <ctime>
#include <fstream>
#include <sstream>

using namespace std;

// Variables globales para almacenar la sesion del usuario

string nombreUsuario, contrasena, departamento, empresa;

// Nodo para el árbol AVL
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


// Nodo para la matriz dispersa
struct NodoMatriz {
    string nombreUsuario;
    string contrasena;
    NodoAVL* arbolAVL; // Árbol AVL para almacenar activos
    NodoMatriz* derecha;
    NodoMatriz* abajo;

    NodoMatriz(string usuario, string pass)
        : nombreUsuario(usuario), contrasena(pass),
        arbolAVL(nullptr), derecha(nullptr), abajo(nullptr) {}
};

// Clase para manejar la matriz dispersa
class MatrizDispersa {
public:
    NodoMatriz* cabeza;

public:
    MatrizDispersa() : cabeza(nullptr) {}

    // Insertar un usuario en la matriz
    void insertarUsuario(string departamento, string empresa, string usuario, string contrasena) {
        NodoMatriz* nodoDepartamento = obtenerOInsertarEncabezado(departamento, true);
        NodoMatriz* nodoEmpresa = obtenerOInsertarEncabezado(empresa, false, nodoDepartamento);

        // Crear el nodo del usuario si no existe
        NodoMatriz* actual = nodoEmpresa->abajo;
        while (actual != nullptr) {
            if (actual->nombreUsuario == usuario) {
                cout << "Error: El usuario ya existe en esta empresa y departamento.\n";
                return;
            }
            actual = actual->abajo;
        }

        NodoMatriz* nuevoUsuario = new NodoMatriz(usuario, contrasena);
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

void generarReporteGraphviz() {
    ofstream archivo("reporte_matriz.dot");
    if (!archivo.is_open()) {
        cout << "Error al crear el archivo de reporte.\n";
        return;
    }

    archivo << "digraph MatrizLayout {\n";
    archivo << "  rankdir=LR;\n"; // Cambiar la dirección del gráfico a izquierda-derecha
    archivo << "  node [shape=box, style=filled, fontname=Arial];\n";
    archivo << "  splines=ortho;\n";
    archivo << "  nodesep=0.5;\n";
    archivo << "  ranksep=0.5;\n";

    NodoMatriz* actualDepartamento = cabeza;
    int fila = 1;

    // Añadir nodos de departamentos en la primera fila
    archivo << "  { rank=same; ";
    while (actualDepartamento) {
        archivo << "\"" << actualDepartamento->nombreUsuario << "\" [fillcolor=lightblue]; ";
        actualDepartamento = actualDepartamento->derecha;
    }
    archivo << "}\n";

    actualDepartamento = cabeza;
    while (actualDepartamento) {
        NodoMatriz* actualEmpresa = actualDepartamento->abajo;
        int columna = 1;

        // Añadir nodos de empresas en la primera columna
        while (actualEmpresa) {
            archivo << "  \"" << actualEmpresa->nombreUsuario << "\" [fillcolor=lightgreen];\n";
            actualEmpresa = actualEmpresa->abajo;
            columna++;
        }

        actualDepartamento = actualDepartamento->derecha;
        fila++;
    }

    archivo << "}\n";
    archivo.close();
    system("dot -Tpng reporte_matriz.dot -o reporte_matriz.png");
    cout << "Reporte generado como 'reporte_matriz.png'.\n";
}
private:
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


int obtenerAltura(NodoAVL* nodo) {
    return nodo ? nodo->altura : 0;
}

int obtenerBalance(NodoAVL* nodo) {
    return nodo ? obtenerAltura(nodo->izquierda) - obtenerAltura(nodo->derecha) : 0;
}

NodoAVL* rotarDerecha(NodoAVL* y) {
    NodoAVL* x = y->izquierda;
    NodoAVL* T2 = x->derecha;

    x->derecha = y;
    y->izquierda = T2;

    y->altura = max(obtenerAltura(y->izquierda), obtenerAltura(y->derecha)) + 1;
    x->altura = max(obtenerAltura(x->izquierda), obtenerAltura(x->derecha)) + 1;

    return x;
}

NodoAVL* rotarIzquierda(NodoAVL* x) {
    NodoAVL* y = x->derecha;
    NodoAVL* T2 = y->izquierda;

    y->izquierda = x;
    x->derecha = T2;

    x->altura = max(obtenerAltura(x->izquierda), obtenerAltura(x->derecha)) + 1;
    y->altura = max(obtenerAltura(y->izquierda), obtenerAltura(y->derecha)) + 1;

    return y;
}

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

// Función auxiliar para encontrar el nodo con el valor mínimo en un árbol AVL
NodoAVL* encontrarMinimo(NodoAVL* nodo) {
    while (nodo->izquierda != nullptr)
        nodo = nodo->izquierda;
    return nodo;
}

// Función para eliminar un nodo del árbol AVL
NodoAVL* eliminarNodoAVL(NodoAVL* raiz, string id) {
    if (!raiz)
        return raiz;

    if (id < raiz->idActivo)
        raiz->izquierda = eliminarNodoAVL(raiz->izquierda, id);
    else if (id > raiz->idActivo)
        raiz->derecha = eliminarNodoAVL(raiz->derecha, id);
    else {
        // Nodo encontrado
        if (!raiz->izquierda || !raiz->derecha) {
            NodoAVL* temp = raiz->izquierda ? raiz->izquierda : raiz->derecha;
            delete raiz;
            return temp;
        } else {
            // Nodo con dos hijos
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

    // Recorrido en orden (inorder traversal)
    mostrarActivosDisponiblesAVL(nodo->izquierda);

    // Mostrar solo los activos disponibles
    if (nodo->disponible) {
        cout << "ID Activo: " << nodo->idActivo << endl;
        cout << "Nombre: " << nodo->nombreActivo << endl;
        cout << "Descripcion: " << nodo->descripcion << endl;
        cout << "Tiempo Máximo de Renta: " << nodo->tiempoMaximoRenta << " dias" << endl;
        cout << "----------------------------" << endl;
    }

    mostrarActivosDisponiblesAVL(nodo->derecha);
}

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

// Clase para manejar la lista circular de transacciones
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

        // Buscar la posición correcta para insertar ordenadamente por idTransaccion
        NodoTransaccion* actual = cabeza;
        do {
            if (nuevo->idTransaccion < actual->idTransaccion) {
                // Insertar antes del actual
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

        // Si llegamos aquí, insertar al final
        nuevo->siguiente = cabeza;
        nuevo->anterior = cabeza->anterior;
        cabeza->anterior->siguiente = nuevo;
        cabeza->anterior = nuevo;
    }

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

// Variable global para manejar las transacciones
ListaTransacciones listaTransacciones;

// Función para mostrar los activos de un árbol AVL en orden
void mostrarActivosAVL(NodoAVL* nodo);

// Declaración de la función para generar un ID único de activo
string generarIDActivo();

// Declaración de la función para generar un ID único de activo
string generarIDActivo();

// Modificar la función de rentarActivo
void rentarActivo() {
    NodoMatriz* usuarioActual = matrizUsuarios.buscarUsuario(departamento, empresa, nombreUsuario);
    if (!usuarioActual) {
        cout << "Error: Usuario no encontrado.\n";
        return;
    }

    cout << "---------------------------------------------------" << endl;
    cout << "-------------- Catalogo de Activos ----------------" << endl;
    cout << "---------------------------------------------------" << endl;

    // Mostrar activos disponibles de todos los usuarios menos los del usuario actual
    bool hayActivos = false;
    NodoMatriz* deptoActual = matrizUsuarios.cabeza;
    while (deptoActual != nullptr) {
        NodoMatriz* empresaActual = deptoActual->derecha;
        while (empresaActual != nullptr) {
            NodoMatriz* userActual = empresaActual->abajo;
            while (userActual != nullptr) {
                // Evitar mostrar los activos del usuario logeado
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

    // Buscar y validar el activo en los árboles AVL de otros usuarios
    NodoAVL* activoEncontrado = nullptr;
    NodoMatriz* propietarioActivo = nullptr; // Guardar referencia al propietario del activo

    deptoActual = matrizUsuarios.cabeza;
    while (deptoActual != nullptr) {
        NodoMatriz* empresaActual = deptoActual->derecha;
        while (empresaActual != nullptr) {
            NodoMatriz* userActual = empresaActual->abajo;
            while (userActual != nullptr) {
                // Ignorar los activos del usuario logeado
                if (userActual->nombreUsuario != nombreUsuario && userActual->arbolAVL) {
                    NodoAVL* actual = userActual->arbolAVL;
                    while (actual != nullptr) {
                        if (idActivoRentar == actual->idActivo) {
                            // Verificar si el activo está disponible
                            if (!actual->disponible) {
                                cout << "Error: El activo ya está rentado.\n";
                                return;
                            }

                            activoEncontrado = actual;
                            propietarioActivo = userActual; // Guardar el propietario del activo

                            // Validar el tiempo máximo de renta
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
                if (activoEncontrado) break; // Salir del bucle si el activo ya fue encontrado
                userActual = userActual->abajo;
            }
            if (activoEncontrado) break; // Salir del bucle si el activo ya fue encontrado
            empresaActual = empresaActual->derecha;
        }
        if (activoEncontrado) break; // Salir del bucle si el activo ya fue encontrado
        deptoActual = deptoActual->derecha;
    }

    if (!activoEncontrado) {
        cout << "Error: No se encontró el activo especificado o no está disponible.\n";
        return;
    }

    // Cambiar el estado de disponibilidad del activo a false (rentado)
    activoEncontrado->disponible = false;

    string idTransaccion = generarIDActivo(); // Generar ID único para la transacción
    listaTransacciones.insertarTransaccion(
        idTransaccion, idActivoRentar, activoEncontrado->nombreActivo, nombreUsuario,
        departamento, empresa, diasRenta
    );
}

// Función para devolver un activo (cambiar a disponible)
void devolverActivo(string idActivo) {
    // Buscar el activo en todos los árboles AVL de usuarios
    NodoMatriz* deptoActual = matrizUsuarios.cabeza;
    while (deptoActual != nullptr) {
        NodoMatriz* empresaActual = deptoActual->derecha;
        while (empresaActual != nullptr) {
            NodoMatriz* userActual = empresaActual->abajo;
            while (userActual != nullptr) {
                NodoAVL* actual = userActual->arbolAVL;
                while (actual != nullptr) {
                    if (idActivo == actual->idActivo) {
                        // Encontrar y cambiar el estado a disponible
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

// Función para eliminar una transacción de la lista circular
void eliminarTransaccion(string idTransaccion) {
    if (!listaTransacciones.cabeza) return;

    NodoTransaccion* actual = listaTransacciones.cabeza;
    NodoTransaccion* anterior = nullptr;

    // Recorrer la lista para encontrar la transacción
    do {
        if (actual->idTransaccion == idTransaccion) {
            // Si es el único nodo
            if (actual->siguiente == actual) {
                listaTransacciones.cabeza = nullptr;
                delete actual;
                return;
            }

            // Si es la cabeza
            if (actual == listaTransacciones.cabeza) {
                listaTransacciones.cabeza = actual->siguiente;
            }

            // Ajustar los punteros para "saltar" el nodo a eliminar
            if (anterior) {
                anterior->siguiente = actual->siguiente;
            }

            // Liberar memoria
            delete actual;
            return;
        }

        anterior = actual;
        actual = actual->siguiente;
    } while (actual != listaTransacciones.cabeza);
}



void activosRentados() {
    // Verificar si hay transacciones para el usuario actual
    bool tieneTransacciones = false;
    NodoTransaccion* actual = listaTransacciones.cabeza;
    
    if (!actual) {
        cout << "No hay activos rentados.\n";
        return;
    }

    // Encabezado del listado de activos rentados
    cout << "---------------------------------------------------" << endl;
    cout << "-------------- Activos Rentados -------------------" << endl;
    cout << "---------------------------------------------------" << endl;

    // Arreglo para almacenar IDs de activos rentados
    string activosRentadosUsuario[100];  // Ajusta el tamaño según tus necesidades
    int contadorActivos = 0;

    // Mostrar solo las transacciones del usuario actual
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
            
            // Guardar el ID del activo para potential devolución
            activosRentadosUsuario[contadorActivos++] = actual->idActivo;
        }
        actual = actual->siguiente;
    } while (actual != listaTransacciones.cabeza);

    // Si no hay transacciones para este usuario
    if (!tieneTransacciones) {
        cout << "No tiene activos rentados en este momento.\n";
        return;
    }

    // Opción de devolución de activo
    char opcion;
    cout << "\n¿Desea devolver un activo? (S/N): ";
    cin >> opcion;

    if (opcion == 'S' || opcion == 's') {
        string idActivoDevolver;
        cout << "Ingrese el ID del activo a devolver: ";
        cin >> idActivoDevolver;

        // Buscar la transacción del activo a devolver
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
            // Realizar la devolución
            cout << "\n--- Resumen de Devolucion ---\n";
            cout << "ID Activo: " << transaccionADevolver->idActivo << endl;
            cout << "Activo rentado: " << transaccionADevolver->activo << endl;
            cout << "Dias rentados: " << transaccionADevolver->diasRenta << endl;
            
            // Devolver el activo (cambiar a disponible)
            devolverActivo(transaccionADevolver->idActivo);

            // Marcar la transacción como devuelta (sin eliminarla)
            transaccionADevolver->estadoTransaccion = "DEVUELTO";

            cout << "Activo devuelto exitosamente.\n";
        } else {
            cout << "No se encontró un activo con ese ID entre sus rentados.\n";
        }
    }
}

bool verificarPropiedadActivo(const string& idActivo, const string& nombreUsuario) {
    NodoMatriz* usuarioActual = matrizUsuarios.buscarUsuario(departamento, empresa, nombreUsuario);
    if (!usuarioActual || !usuarioActual->arbolAVL) {
        return false;
    }

    NodoAVL* activo = buscarActivoEnAVL(usuarioActual->arbolAVL, idActivo);
    return activo != nullptr; // Retorna true si el activo fue encontrado
}



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
        // Filtrar: El activo debe ser del usuario actual, pero rentado por otro usuario
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
        // Verificar si el usuario actual es el mismo que el de la transacción
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



// Función para mostrar los activos de un árbol AVL en orden
void mostrarActivosAVL(NodoAVL* nodo) {
    if (!nodo)
        return;

    mostrarActivosAVL(nodo->izquierda);
    cout << ">> ID: " << nodo->idActivo << "; Nombre = " << nodo->nombreActivo
        << "; Descripcion: " << nodo->descripcion << "; Tiempo Maximo de Renta = " << nodo->tiempoMaximoRenta << endl;
    mostrarActivosAVL(nodo->derecha);
}

// Función para eliminar un activo del usuario
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

    // Guardar la información del activo antes de eliminarlo
    string nombreActivoEliminado;
    string descripcionActivoEliminado;
    int tiempoMaximoRentaActivoEliminado;
    
    // Buscar el activo y guardar su información
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

    // Intentar eliminar el nodo del árbol AVL
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


// Función para generar un carácter aleatorio
char generarCaracterAleatorio() {
    //generan números pseudoaleatorios
    static unsigned long seed1 = 1;
    static unsigned long seed2 = 0;
    const std::string caracteres = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    
    // Combinar dos semillas
    seed1 = (seed1 * 24271 + 3522483246);
    seed2 = (seed2 * 2143618259 + 12345);
    
    unsigned long combinedSeed = seed1 ^ seed2;
    return caracteres[combinedSeed % caracteres.length()];
}

// Función para generar un ID único de 15 caracteres alfanuméricos
string generarIDActivo() {
    string id;
    
    for (int i = 0; i < 15; i++) {
        id += generarCaracterAleatorio();
    }
    return id;
}

// Función para iniciar sesion
bool iniciarSesion() {
    cout << "Ingresar Nombre de Usuario: ";
    cin >> nombreUsuario;
    cout << "Ingresar Contrasena: ";
    cin >> contrasena;

    // Verificar si es un usuario administrador
    if (nombreUsuario == "admin" && contrasena == "admin") {
        departamento = "N/A";
        empresa = "N/A";
        return true; // Inicia sesion como administrador
    } else {
        cout << "Ingresar Departamento: ";
        cin >> departamento;
        cout << "Ingresar Empresa: ";
        cin >> empresa;

        NodoMatriz* nodoUsuario = matrizUsuarios.buscarUsuario(departamento, empresa, nombreUsuario);

        if (nodoUsuario != nullptr && nodoUsuario->contrasena == contrasena) {
            cout << "Inicio de sesion exitoso. Bienvenido, " << nodoUsuario->nombreUsuario << "!\n";
            return false; // Inicia sesion como usuario normal
        } else {
            cout << "Error: Usuario o contrasena incorrectos.\n";
            return false; // No permite iniciar sesion
        }
    }
}

// Función para registrar usuarios
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

// Función para agregar un activo
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

void modificarActivo() {
    NodoMatriz* usuarioActual = matrizUsuarios.buscarUsuario(departamento, empresa, nombreUsuario);

    if (!usuarioActual || !usuarioActual->arbolAVL) {
        cout << "No tienes activos registrados para modificar.\n";
        return;
    }

    // Mostrar la lista de activos
    cout << "Lista de activos disponibles:\n";
    mostrarActivosAVL(usuarioActual->arbolAVL);

    // Pedir al usuario el ID del activo a modificar
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

    // Validar si el activo fue encontrado
    if (activoEncontrado) {
        cout << "\nActivo seleccionado:\n";
        cout << ">> ID: " << activoEncontrado->idActivo << "\n";
        cout << ">> Nombre: " << activoEncontrado->nombreActivo << "\n";
        cout << ">> Descripcion actual: " << activoEncontrado->descripcion << "\n";
        cout << ">> Tiempo Maximo de Renta: " << activoEncontrado->tiempoMaximoRenta << "\n";

        // Pedir la nueva descripción
        cout << "Ingrese la nueva descripcion para este activo: ";
        cin.ignore(); // Limpiar el buffer
        string nuevaDescripcion;
        getline(cin, nuevaDescripcion);

        // Actualizar la descripción del activo
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

// menu de usuario normal
void menuUsuario() {
    int opcion;
    do {
        cout << "---------------------------------------------------" << endl;
        cout << "-----Menu principal de un usuario ya ingresado-----" << endl;
        cout << "---------------------------------------------------" << endl;
        cout << "1. Agregar Activo" << endl;
        cout << "2. Eliminar Activo" << endl;
        cout << "3. Modificar Activo" << endl;
        cout << "4. Rentar Activo" << endl;
        cout << "5. Activos Rentados" << endl;
        cout << "6. Mis Activos Rentados" << endl;
        cout << "7. Cerrar Sesion" << endl;
        cout << "Ingresar Opcion: ";
        cin >> opcion;

        switch (opcion) {
            case 1:
            {
                agregarActivo();   
            }
            break;
            case 2:
                eliminarActivo();
                break;
            case 3:
                modificarActivo();
                break;
            case 4:
                rentarActivo();
                break;
            case 5:
                activosRentados();
                break;
            case 6:
                mostrarMisActivosRentados();
                break;
            case 7:
                // Cerrar sesion
                break;
            default:
                cout << "Opcion invalida, intente de nuevo" << endl;
        }
    } while (opcion != 7);
}

// menu de usuario administrador
void menuAdministrador() {
    int opcion;
    do {
        cout << "******************************" << endl;
        cout << "******Menu Administrador******" << endl;
        cout << "******************************" << endl;
        cout << "1. Registrar Usuario" << endl;
        cout << "2. Reporte Matriz Dispersa" << endl;
        cout << "3. Reporte Activos Disponibles de un Departamento" << endl;
        cout << "4. Reporte Activos Disponibles de una Empresa" << endl;
        cout << "5. Reporte Transacciones" << endl;
        cout << "6. Reporte Activos de un Usuario" << endl;
        cout << "7. Activos rentados por un Usuario" << endl;
        cout << "8. Ordenar Transacciones" << endl;
        cout << "9. Salir" << endl; // Nueva opción para salir
        cout << "Ingresar Opcion: ";
        cin >> opcion;

        switch (opcion) {
            case 1:
                registrarUsuario();
                break;
            case 2:
                matrizUsuarios.generarReporteGraphviz();
                break;
            case 3:
                // Reporte activos disponibles de un departamento
                break;
            case 4:
                // Reporte activos disponibles de una empresa
                break;
            case 5:
                // Reporte transacciones
                break;
            case 6:
                // Reporte activos de un usuario
                break;
            case 7:
                // Activos rentados por un usuario
                break;
            case 8:
                // Ordenar transacciones
                break;
            case 9:
                cout << "Regresando al menu principal...\n";
                return; // Salir del menu y regresar al menu principal
            default:
                cout << "Opcion invalida. Intente de nuevo." << endl;
        }
    } while (true);
}

int main() {
    int opcion;
    do {
        cout << "=================================================" << endl;
        cout << "====Bienvenido al sistema de renta de activos====" << endl;
        cout << "=================================================" << endl;
        cout << "1. Iniciar sesion" << endl;
        cout << "2. Salir" << endl;
        cout << "Ingresar Opcion: ";
        cin >> opcion;

        switch (opcion) {
            case 1: {
                bool esAdmin = iniciarSesion();
                if (esAdmin) {
                    menuAdministrador();
                } else if (!esAdmin && matrizUsuarios.buscarUsuario(departamento, empresa, nombreUsuario) != nullptr) {
                    menuUsuario();
                } else {
                    // No muestra menu si las credenciales no son válidas
                    cout << "No se pudo iniciar sesion. Regresando al menu principal...\n";
                }
                break;
            }
            case 2:
                cout << "Saliendo del programa..." << endl;
                break;
            default:
                cout << "Opcion invalida, intente de nuevo" << endl;
        }
    } while (opcion != 2);

    return 0;
}