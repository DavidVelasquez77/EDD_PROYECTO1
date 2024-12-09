#include <iostream>
#include <string>
#include <limits>
#include <ctime>

using namespace std;

// Variables globales para almacenar la sesion del usuario

string nombreUsuario, contrasena, departamento, empresa;

// Nodo para el árbol AVL
struct NodoAVL {
    string idActivo;
    string nombreActivo; 
    string descripcion;
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

NodoAVL* crearNodoAVL(string id, string nombre, string descripcion) {
    NodoAVL* nuevoNodo = new NodoAVL();
    nuevoNodo->idActivo = id;
    nuevoNodo->nombreActivo = nombre; // Asignar nombre del activo
    nuevoNodo->descripcion = descripcion;
    nuevoNodo->izquierda = nullptr;
    nuevoNodo->derecha = nullptr;
    nuevoNodo->altura = 1; // Altura inicial del nodo
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

NodoAVL* insertarEnAVL(NodoAVL* nodo, string id, string nombre, string descripcion) {
    if (!nodo)
        return crearNodoAVL(id, nombre, descripcion);

    if (id < nodo->idActivo)
        nodo->izquierda = insertarEnAVL(nodo->izquierda, id, nombre, descripcion);
    else if (id > nodo->idActivo)
        nodo->derecha = insertarEnAVL(nodo->derecha, id, nombre, descripcion);
    else
        return nodo; // IDs duplicados no se permiten

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

// Estructura para el nodo de la lista circular de transacciones
struct NodoTransaccion {
    string idTransaccion;
    string idActivo;
    string usuario;
    string departamento;
    string empresa;
    int diasRenta;
    time_t fechaRenta;
    NodoTransaccion* siguiente;
    NodoTransaccion* anterior;

    NodoTransaccion(string idTrans, string idAct, string user, string depto, 
                    string emp, int dias) 
        : idTransaccion(idTrans), idActivo(idAct), usuario(user), 
          departamento(depto), empresa(emp), diasRenta(dias) {
        time(&fechaRenta);
        time(&fechaRenta);
        siguiente = anterior = nullptr;
    }
};

// Clase para manejar la lista circular de transacciones
class ListaTransacciones {
private:
    NodoTransaccion* cabeza;
    
public:
    ListaTransacciones() : cabeza(nullptr) {}

    void insertarTransaccion(string idTrans, string idAct, string user, 
                           string depto, string emp, int dias) {
        NodoTransaccion* nuevo = new NodoTransaccion(idTrans, idAct, user, 
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

// Función para rentar un activo
void rentarActivo() {
    NodoMatriz* usuarioActual = matrizUsuarios.buscarUsuario(departamento, empresa, nombreUsuario);
    if (!usuarioActual) {
        cout << "Error: Usuario no encontrado.\n";
        return;
    }

    cout << "---------------------------------------------------" << endl;
    cout << "-------------- Catalogo de Activos ----------------" << endl;
    cout << "---------------------------------------------------" << endl;
    
    // Mostrar activos disponibles de todos los usuarios
    bool hayActivos = false;
    NodoMatriz* deptoActual = matrizUsuarios.cabeza;
    while (deptoActual != nullptr) {
        NodoMatriz* empresaActual = deptoActual->derecha;
        while (empresaActual != nullptr) {
            NodoMatriz* userActual = empresaActual->abajo;
            while (userActual != nullptr) {
                if (userActual->arbolAVL) {
                    mostrarActivosAVL(userActual->arbolAVL);
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

    // Buscar y eliminar el activo del árbol AVL del propietario
    NodoAVL* activoEncontrado = nullptr;
    string nombreActivoRentado;
    string descripcionActivoRentado;

    deptoActual = matrizUsuarios.cabeza;
    while (deptoActual != nullptr) {
        NodoMatriz* empresaActual = deptoActual->derecha;
        while (empresaActual != nullptr) {
            NodoMatriz* userActual = empresaActual->abajo;
            while (userActual != nullptr) {
                if (userActual->arbolAVL) {
                    // Buscar el activo en el árbol AVL actual
                    NodoAVL* actual = userActual->arbolAVL;
                    bool encontrado = false;
                    
                    while (actual != nullptr && !encontrado) {
                        if (idActivoRentar == actual->idActivo) {
                            nombreActivoRentado = actual->nombreActivo;
                            descripcionActivoRentado = actual->descripcion;
                            encontrado = true;
                            activoEncontrado = actual;
                            
                            // Eliminar el activo del árbol
                            userActual->arbolAVL = eliminarNodoAVL(userActual->arbolAVL, idActivoRentar);
                            
                            // Crear nueva transacción
                            string idTransaccion = generarIDActivo(); // Reutilizamos la función de generar ID
                            listaTransacciones.insertarTransaccion(
                                idTransaccion, idActivoRentar, nombreUsuario,
                                departamento, empresa, diasRenta
                            );
                            
                            cout << "\n----Renta exitosa-----\n";
                            cout << "ID Transaccion: " << idTransaccion << endl;
                            cout << "Activo rentado: " << nombreActivoRentado << endl;
                            cout << "Por " << diasRenta << " dias\n";
                            return;
                        }
                        else if (idActivoRentar < actual->idActivo) {
                            actual = actual->izquierda;
                        }
                        else {
                            actual = actual->derecha;
                        }
                    }
                }
                userActual = userActual->abajo;
            }
            empresaActual = empresaActual->derecha;
        }
        deptoActual = deptoActual->derecha;
    }

    if (!activoEncontrado) {
        cout << "Error: No se encontró el activo especificado.\n";
    }
}

// Función para mostrar los activos de un árbol AVL en orden
void mostrarActivosAVL(NodoAVL* nodo) {
    if (!nodo)
        return;

    mostrarActivosAVL(nodo->izquierda);
    cout << ">> ID: " << nodo->idActivo << "; Nombre = " << nodo->nombreActivo
         << "; Descripcion: " << nodo->descripcion << endl;
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
    
    // Buscar el activo y guardar su información
    NodoAVL* actual = usuarioActual->arbolAVL;
    bool encontrado = false;
    
    while (actual != nullptr && !encontrado) {
        if (idActivoEliminar == actual->idActivo) {
            nombreActivoEliminado = actual->nombreActivo;
            descripcionActivoEliminado = actual->descripcion;
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

    cout << "Ingrese el nombre del nuevo activo: ";
    cin.ignore(); // Limpiar el buffer de entrada
    getline(cin, nombreActivo);

    cout << "Ingrese la descripcion del nuevo activo: ";
    getline(cin, descripcionActivo);

    idActivo = generarIDActivo(); // Generar un ID único para el activo
    cout << "El ID unico del nuevo activo es: " << idActivo << endl;

    NodoMatriz* usuarioActual = matrizUsuarios.buscarUsuario(departamento, empresa, nombreUsuario);

    if (!usuarioActual) {
        cout << "Error: Usuario no encontrado. No se puede agregar el activo.\n";
        return;
    }

    // Insertar el activo en el árbol AVL del usuario
    usuarioActual->arbolAVL = insertarEnAVL(usuarioActual->arbolAVL, idActivo, nombreActivo, descripcionActivo);
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
        cout << ">> Descripción actual: " << activoEncontrado->descripcion << "\n";

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
                // Mostrar activos rentados
                break;
            case 6:
                // Mostrar mis activos rentados
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
                // Reporte matriz dispersa
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