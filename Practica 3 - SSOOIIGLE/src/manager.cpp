/************************************************************************************
 * Práctica 2 - Sistemas Operativos II                                              *
 * SSOOIIGLE v2                                                                     *
 * Autores: Diego García Álvarez                                                    *
 *          Pablo Mollá GIl                                                         *
 * 10/05/2025                                                                       *
 ************************************************************************************/

// Inclusión de bibliotecas necesarias
#include <iostream>
#include <clients.h>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <processor.h>
#include <definitions.h>
#include <iomanip>

// Declaración de funciones
void create_clients_thread();
void create_dictionary();
void signal_handler(int sig);

// Variables globales
std::vector<std::thread> v_searching_threads;   // Vector de hilos de búsqueda
std::vector<std::thread> v_client_threads;      // Vector de hilos de clientes
std::unordered_set<std::string> dictionary_set;  // Set de diccionarios

int main(){
    create_dictionary();
    std::cout << "-------------------------------------------------------------------------------------" << std::endl;
    std::cout << std::setw(80 - ((sizeof("WELCOME TO SSOOIIGLE v2.0")-1) / 2)) << "WELCOME TO SSOOIIGLE v2.0" << std::endl;
    std::cout << "Número de procesadores disponibles: " << NUMBER_OF_THREADS << std::endl;
    std::cout << "-------------------------------------------------------------------------------------" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::thread payment_th(pay_system);
    payment_th.detach();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    signal(SIGINT, signal_handler);
    books_names();
    create_clients_thread();

    std::cout << "Todos los hilos terminados. Para salir -> Ctrl+C" << std::endl;

    return EXIT_SUCCESS;
}
/****************************************************
* Crea los hilos para alojar los clientes          *
* @param .     No requerido                        *
* @return      No devuelve nada                    *
***************************************************/
void create_clients_thread(){
    /* Calculamos un número aleatorio para el tipo de clientes y elegimos al azar la palabra entre las que hay en el diccionario,   *
    *  con esto creamos los clientes y le asignamos su hilo correspondiente                                                         */
    for(int i=0; i< NUMBER_OF_THREADS; i++){
        int type = rand() % 3;
        std::string random_word = *std::next(std::begin(dictionary_set), std::rand() % dictionary_set.size());

        v_client_threads.push_back(std::thread(create_threads, i, type, random_word));
        std::this_thread::sleep_for(std::chrono::seconds(1));   // Esperamos un segundo para no colapsar la terminal
    }
    /* Esperamos a la finalización de todos los clientes */
    std::for_each(v_client_threads.begin(), v_client_threads.end(), [](std::thread &t){t.join();});
}

/****************************************************
* Manejador de señales para interrupciones         *
* @param num   Número de la señal recibida         *
* @return      No devuelve nada                    *
***************************************************/
void signal_handler(int num){
    std::cout << "\n[!] SIGINT signal received, killing processes and freeing resources." << std::endl;
    exit(EXIT_SUCCESS);
}

/****************************************************
* Se crea una variable para todas las palabras      *
* @param .     No requerido                         *
* @return      No devuelve nada                     *
 ***************************************************/
void create_dictionary(){
    std::ifstream file("docs/Diccionario.txt");
    if (file.is_open()){
        std::string word;
        while (file >> word){ 
            dictionary_set.insert(word);
        }
        file.close();
    }
    else{
        std::cout << "Error: El archivo no existe o no se puede abrir." << std::endl;
        exit(EXIT_FAILURE);
    }
}
