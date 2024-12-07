#include <iostream>
#include <string>
#include <limits>

using namespace std;

// Variables globales para almacenar la sesión del usuario

string nombreUsuario, contrasenia, departamento, empresa;

// Nodo para el árbol AVL
struct NodoAVL {
    string idActivo;
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
private:
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

MatrizDispersa matrizUsuarios; 

// Función para iniciar sesión

// Función para iniciar sesión
bool iniciarSesion() {
    cout << "Ingresar Nombre de Usuario: ";
    cin >> nombreUsuario;
    cout << "Ingresar Contrasena: ";
    cin >> contrasenia;

    // Verificar si es un usuario administrador
    if (nombreUsuario == "ADMINISTRADOR" && contrasenia == "ADMINISTRADOR") {
        departamento = "N/A";
        empresa = "N/A";
        return true; // Inicia sesión como administrador
    } else {
        cout << "Ingresar Departamento: ";
        cin >> departamento;
        cout << "Ingresar Empresa: ";
        cin >> empresa;

        NodoMatriz* nodoUsuario = matrizUsuarios.buscarUsuario(departamento, empresa, nombreUsuario);

        if (nodoUsuario != nullptr && nodoUsuario->contrasena == contrasenia) {
            cout << "Inicio de sesión exitoso. Bienvenido, " << nodoUsuario->nombreUsuario << "!\n";
            return false; // Inicia sesión como usuario normal
        } else {
            cout << "Error: Usuario o contraseña incorrectos.\n";
            return false; // No permite iniciar sesión
        }
    }
}

// Función para registrar usuarios
void registrarUsuario() {
    string usuario, pass, depto, emp;

    cout << "Ingrese el nombre de usuario: ";
    cin >> usuario;

    cout << "Ingrese la contraseña: ";
    cin >> pass;

    cout << "Ingrese el departamento: ";
    cin >> depto;

    cout << "Ingrese la empresa: ";
    cin >> emp;

    matrizUsuarios.insertarUsuario(depto, emp, usuario, pass);
}


// Menú de usuario normal
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
                string nuevoActivo, descripcionActivo, idActivo;
                
                cout << "Ingrese el nombre del nuevo activo: " << endl;
                cin.ignore(); // Limpiar el buffer de entrada
                getline(cin, nuevoActivo);
                
                cout << "Ingrese la descripcion del nuevo activo: " << endl;
                getline(cin, descripcionActivo);
                
                idActivo = generarIDActivo();
                
                cout << "El ID unico del nuevo activo es: " << idActivo << endl;
                
                // Aquí puedes almacenar el nuevo activo con su ID, nombre y descripción
            }
            break;
            case 2:
                // Eliminar activo
                break;
            case 3:
                // Modificar activo
                break;
            case 4:
                // Rentar activo
                break;
            case 5:
                // Mostrar activos rentados
                break;
            case 6:
                // Mostrar mis activos rentados
                break;
            case 7:
                // Cerrar sesión
                break;
            default:
                cout << "Opcion invalida, intente de nuevo" << endl;
        }
    } while (opcion != 7);
}

// Menú de usuario administrador
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
            default:
                cout << "Opcion invalida. Intente de nuevo." << endl;
        }
    } while (opcion != 8);
}

int main() {
    int opcion;
    do {
        cout << "=================================================" << endl;
        cout << "====Bienvenido al sistema de renta de activos====" << endl;
        cout << "=================================================" << endl;
        cout << "1. Iniciar Sesion" << endl;
        cout << "2. Salir" << endl;
        cout << "Ingresar Opcion: ";
        cin >> opcion;

        switch (opcion) {
            case 1:
                if (iniciarSesion()) {
                    menuAdministrador();
                } else {
                    menuUsuario();
                }
                break;
            case 2:
                cout << "Saliendo del programa..." << endl;
                break;
            default:
                cout << "Opcion invalida, intente de nuevo" << endl;
        }
    } while (opcion != 2);

    return 0;
}