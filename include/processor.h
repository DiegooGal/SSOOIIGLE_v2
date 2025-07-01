/************************************************************************************
 * Práctica 3 - Sistemas Operativos II                                              *
 * SSOOIIGLE - processor.h                                                          *
 * Autor: Diego García Álvarez                                                      *
 * 29/06/2025                                                                       *
 * Creación y gestión de clientes con sus distintas funciones, además de llamadas   *
 * a los buscadores.                                                                *
 ************************************************************************************/

// Inclusión de bibliotecas necesarias
#include <clients.h>
#include <iostream>
#include <string>
#include <cstdlib>
#include <unistd.h>
#include <mutex>
#include <thread>
#include <fstream>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <vector>
#include <thread>
#include <sys/types.h>
#include <my_lib.h>
#include <shared_mutex>
#include <thread_struct.h>
#include <request_searchs.h>
#include <searcher.h>
#include <definitions.h>

// Declaración de funciones
void create_request(Client client, int type, int balance, sh_request &req);
void create_thread(int id, int type, std::string word);
void pay_system();
void books_names();
void searchers_wait();
void insert_in_queue(sh_request req, int inf_lim, int sup_lim);
void search_for_words(Searcher &searcher, Client client, std::string word);

// Variables globales
std::mutex g_mtx;
std::queue<sh_request> petitions_queue;
std::condition_variable search_cv;
std::vector<free_client> v_free_clients;
std::vector<premium_client> v_premium_clients;
std::vector<unlimited_client> v_unlimited_clients;
bool continue_waiting = true;

void create_thread(int id, int type, std::string word){
    switch (type)
    {
    case 0:{
        free_client FreeClient = free_client(id, word, "Free Client");
        std::cout << "Cliente " << id << " tipo: Free Client creado." << std::endl;

        struct sh_request req;
        create_request(FreeClient, type, 0, req);
        insert_in_queue(req, 0, 20);

        // Se asigna el buscador.
        Searcher f_searcher = Searcher(id, word, type, WORDS_NUM);
        f_searcher.credit_counter = 1;

        // Esperamos a que un searcher quede libre.
        searchers_wait();

        std::cout << "Searcher with id " << f_searcher.getId() << " created." << std::endl;

        // Se mide el tiempo que tarda en realizar la busqueda.
        auto start_time = std::chrono::high_resolution_clock::now();
        search_for_words(f_searcher, FreeClient, word);
        auto end_time = std::chrono::high_resolution_clock::now();
        auto elapsed_time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

        std::cout << "Client with id " << FreeClient.getId() << " finished in " << elapsed_time_ms << " ms." << std::endl;
        std::cout << "Searcher with id " << f_searcher.getId() << " finished." << std::endl;

        // Se inserta el cliente al vector y se saca de la cola de peticones
        v_free_clients.push_back(FreeClient);
        petitions_queue.pop();
        search_cv.notify_one();
        break;
    }
    case 1:{
        srand(time(NULL));
        int balance = rand() % 101;
        
        premium_client PremiumClient = premium_client(id, word, "Premium Client", double(balance));
        std::cout << "Cliente " << id << " tipo: Premium Client creado." << std::endl;


        // Se crea la petición y se inserta en la cola.
        struct sh_request req;
        create_request(PremiumClient, type, balance, req);
        insert_in_queue(req, 20, 100);

        searchers_wait();

        // Se le asigna el buscador y el saldo
        Searcher f_searcher = Searcher(id, word, type, WORDS_NUM);
        f_searcher.credit_counter = PremiumClient.get_balance();
        std::cout << "Searcher with id " << f_searcher.getId() << " created." << std::endl;

        auto start_time = std::chrono::high_resolution_clock::now();
        search_for_words(f_searcher, PremiumClient, word);
        auto end_time = std::chrono::high_resolution_clock::now();
        auto elapsed_time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

        std::cout << "Client with id " << PremiumClient.getId() << " finished in " << elapsed_time_ms << " ms." << std::endl;

        // Se le asigna el saldo que le queda al cliente.
        PremiumClient.set_balance(f_searcher.credit_counter);

        std::cout << "Searcher with id " << f_searcher.getId() << " finished." << std::endl;

        // Se inserta el cliente al vector y se saca de la cola de peticones
        v_premium_clients.push_back(PremiumClient);
        petitions_queue.pop();
        search_cv.notify_one();
        break;
    }
    case 2:{
        unlimited_client UnlimitedClient = unlimited_client(id, word, "Unlimited Client");
        std::cout << "Cliente " << id << " tipo: Unlimited Client creado." << std::endl;        
        
        // Se crea la petición y se inserta en la cola.
        struct sh_request req;
        create_request(UnlimitedClient, type, 0, req);
        insert_in_queue(req, 20, 100);

        searchers_wait();
        // Se asigna el buscador
        Searcher f_searcher = Searcher(id, word, type, WORDS_NUM);
        f_searcher.credit_counter = 0;

        std::cout << "Searcher with id " << f_searcher.getId() << " created." << std::endl;
        auto start_time = std::chrono::high_resolution_clock::now();
        search_for_words(f_searcher, UnlimitedClient, word);
        auto end_time = std::chrono::high_resolution_clock::now();
        auto elapsed_time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

        std::cout << "Client with id " << UnlimitedClient.getId() << " finished in " << elapsed_time_ms << " ms." << std::endl;
        std::cout << "Searcher with id " << f_searcher.getId() << " finished." << std::endl;

        // Se inserta el cliente al vector y se saca de la cola de peticones
        v_unlimited_clients.push_back(UnlimitedClient);
        petitions_queue.pop();
        search_cv.notify_one();
        break;
    }
    default:
        break;
    }
}

/****************************************************
* Llamada al buscador para escribir los resultados  *
* @param searcher   Buscador a usar                 *
* @param Client     Cliente que busca               *
* @param word       Palabra a buscar                *
* @return           No devuelve nada                *
****************************************************/
void search_for_words(Searcher &searcher, Client client, std::string word){
    for (int i = 0; i < titles_vector.size() && searcher.word_counter < WORDS_NUM; i++)
    {
        std::cout << "Searcher " << searcher.getId() << " searching in " << titles_vector[i] << std::endl;
        searcher.new_search(titles_vector[i], word, client.v_thread_struct);
        client.print_search_results(word, titles_vector[i]);
        client.v_thread_struct.clear();
    }
}

/****************************************************
* Inserta la petición en la cola                    *
* @param req        Peticion                        *
* @param inf_lim    Limite inferior                 *
* @param sup_lim    Limite superior                 *
* @return           No devuelve nada                *
****************************************************/
void insert_in_queue(sh_request req, int inf_lim, int sup_lim){
    srand(time(NULL));
    while (true){
        int in = rand() % 101;
        if (in >= inf_lim && in <= sup_lim){
            petitions_queue.push(req);
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

/****************************************************
* Crea la petición de los clientes                  *
* @param client   Cliente que busca                 *
* @param type     Tipo de cliente                   *
* @param balance  Saldo                             *
* @param req      Petición                          *
* @return         No devuelve nada                  *
****************************************************/
void create_request(Client client, int type, int balance, sh_request &req)
{
    req.client_id = client.getId();
    req.client_type = type;
    req.balance = balance;
    req.word = client.getWord();
}

/****************************************************
* Espera a los buscadores, maximo 4                 *
* @param .  No requiere                             *
* @return   No devuelve nada                        *
****************************************************/
void searchers_wait(){
    std::unique_lock<std::mutex> lock(g_mtx);
    search_cv.wait(lock, []
                   { return petitions_queue.size() <= 4; });
    lock.unlock();
}