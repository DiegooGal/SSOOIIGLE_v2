/************************************************************************************
 * Práctica 2 - Sistemas Operativos II                                              *
 * SSOOIIGLE v2                                                                     *
 * Autores: Diego García Álvarez                                                    *
 * 25/06/2025                                                                       *
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
#include <atomic>

// Declaración de funciones
void create_clients_threads();

// Variables globales
std::vector<std::thread> v_client_threads;
std::atomic<bool> running = true;

int main(){
    create_dictionary();
    std::cout << "-------------------------------------------------------------------------------------" << std::endl;
    std::cout << std::setw(80 - ((sizeof("WELCOME TO SSOOIIGLE v2.0")-1) / 2)) << "WELCOME TO SSOOIIGLE v2.0" << std::endl;
    std::cout << "Número de procesadores disponibles: " << NUMBER_OF_THREADS << std::endl;
    std::cout << "-------------------------------------------------------------------------------------" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    
    while (running){

    }

    std::cout "\n [!] CTRL+C...";
    return 0;
}

/****************************************************
* Crea los hilos para alojar los clientes          *
* @param .     No requerido                        *
* @return      No devuelve nada                    *
***************************************************/
void create_clients_threads(){
    for(int = 0; i<NUMBER_OF_THREADS){
        int type = rand() % 3;
        std::string random_word = *std::next(std::begin(dictionary_set), std::rand() % dictionary_set.size());
        
        c_client_threads.push_back(std::thread(create_threads, i, type, random_word));
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    std::for_each(v_client_threads.begin(), v_client_threads.end(), [](std::thread &t){t.join();});
}

/****************************************************
* Manejador de señales para interrupciones         *
* @param num   Número de la señal recibida         *
* @return      No devuelve nada                    *
***************************************************/
void signal_handler(int num){
    std::cout << "\n[!] SIGINT signal received, killing processes and freeing resources." << std::endl;
    running = false;
}

/****************************************************
* Se crea una variable para todas las palabras      *
* @param .     No requerido                         *
* @return      No devuelve nada                     *
 ***************************************************/
void create_dictionary(){
    std::ifstream file("docs/Diccionario.txt");
    if (file.empty())
    {
        std::cout << "\n[!] El archivo no se pudo abrir o está vacío." << std::endl;
        exit(EXIT_FAILURE);
    }
    else if (file.is_open())
    {
        std::string word;
        while(file >> word){
            dictionary_set.insert(word);
        }
        file.close();
    }
}
