#include <iostream>
#include <string>
#include <limits>

using namespace std;

// Variables globales para almacenar la sesión del usuario
string nombreUsuario, contrasenia, departamento, empresa;

// Función para generar un carácter aleatorio
char generarCaracterAleatorio() {
    const string caracteres = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    return caracteres[rand() % caracteres.length()];
}

// Función para generar un ID único de 15 caracteres alfanuméricos
string generarIDActivo() {
    string id;
    
    for (int i = 0; i < 15; i++) {
        id += generarCaracterAleatorio();
    }
    
    return id;
}

// Función para realizar el inicio de sesión
bool iniciarSesion() {
    cout << "Ingresar Nombre de Usuario: ";
    cin >> nombreUsuario;
    cout << "Ingresar Contrasena: ";
    cin >> contrasenia;

    // Verificar si es un usuario administrador
    if (nombreUsuario == "ADMINISTRADOR" && contrasenia == "ADMINISTRADOR") {
        departamento = "N/A";
        empresa = "N/A";
        return true;
    } else {
        cout << "Ingresar Departamento: ";
        cin >> departamento;
        cout << "Ingresar Empresa: ";
        cin >> empresa;
        return false;
    }
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
                // Registrar usuario
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