/************************************************************************************
 * Práctica 2 - Sistemas Operativos II                                              *
 * SSOOIIGLE v2                                                                     *
 * Autores: Diego García Álvarez                                                    *
 * 24/06/2025                                                                       *
 ************************************************************************************/

// Inclusión de bibliotecas necesarias
#include <iostream>
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
#include <clients.h>
#include <algorithm>

// Declaración de funciones
void create_clients_threads(int i);
void create_dictionary();
void signal_handler(int num);
void search_word();

// Variables globales
std::vector<std::thread> v_client_threads;
std::atomic<bool> running;
std::atomic<int> active_clients = 0;
std::vector<std::string> dictionary_set;
std::vector<std::thread> clients_set;
std::mutex clients_mutex;
std::mutex print_mutex;

int main(){
    signal(SIGINT, signal_handler);
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
    running = true;
    books_names();
    int i = 0;
    while (running){
        {
            std::lock_guard<std::mutex> lock(clients_mutex);
            if(active_clients < NUMBER_OF_THREADS){
                create_clients_threads(i);
                i++;
            }
        }

        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    for (auto &client_thread : clients_set) {
        if (client_thread.joinable()) {
            client_thread.join();
        }
    }
    
    return 0;
}

/****************************************************
* Crea los hilos para alojar los clientes          *
* @param .     No requerido                        *
* @return      No devuelve nada                    *
***************************************************/
void create_clients_threads(int i){
    int type = rand() % 3;
    std::string random_word = *std::next(std::begin(dictionary_set), std::rand() % dictionary_set.size());

    // Aumentamos el número de clientes activos
    active_clients++;

    std::thread([=]() {
        create_thread(i, type, random_word);  // Aquí haces el trabajo real del cliente

        // Al terminar, decrementa los clientes activos
        active_clients--;
    }).detach();  // Liberamos el hilo, no hay necesidad de join
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

void search_word(Client client){

}

/****************************************************
* Se crea una variable para todas las palabras      *
* @param .     No requerido                         *
* @return      No devuelve nada                     *
 ***************************************************/
void create_dictionary(){
    std::ifstream file("docs/Diccionario.txt");
    if (file.tellg() > 0 || !file)
    {
        std::cout << "\n[!] El archivo no se pudo abrir o está vacío." << std::endl;
        exit(EXIT_FAILURE);
    }
    else if (file.is_open())
    {
        std::string word;
        while(file >> word){
            dictionary_set.push_back(word);
        }
        file.close();
    }
}
