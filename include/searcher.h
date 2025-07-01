/************************************************************************************
 * Práctica 2 - Sistemas Operativos II                                              *
 * SSOOIIGLE - searcher.h                                                           *
 * Autor: Diego García Álvarez                                                      *
 * 29/06/2025                                                                       *
 * Funcionalidades necesarias para realizar las búsquedas.                          *
 ************************************************************************************/

// Inclusión de bibliotecas necesarias
#include <iostream>
#include <thread>
#include <string>
#include <queue>
#include <thread_struct.h>
#include <mutex>
#include <atomic>
#include <fstream>
#include <shared_mutex>
#include <my_lib.h>
#include <definitions.h>
#include <request_struct.h>

// Declaración de funciones
void pay_system();

// Variables globales
std::queue<std::shared_ptr<request>> payment_queue;
std::mutex pay_mtx;
std::condition_variable payment_cv;
std::mutex waiting_mtx;

class Searcher{
    public:
        int id;
        int type;
        std::string word;
        int word_num;
        std::atomic<int> word_counter;
        std::atomic<int> credit_counter;
        std::vector<thread_struct> v_thread_struct;
        std::shared_mutex searcher_mtx;
        std::vector<std::thread> v_threads_books;
    
    public:
        Searcher(int id, std::string word, int type, int words_num){
            this->word = word;
            this->id = id;
            this->type = type;
            this->word_num = words_num;
            this->word_counter = 0;
        }
    
        int getType() const{
            return type;
        }
    
        int getWordsNum() const{
            return word_num;
        }
    
        int getId() const{
            return id;
        }
        

    /****************************************************
    * Gestiona los hilos para las busquedas             *
    * @param file_name              Nombre del fichero  *
    * @param word                   Palabra a buscar    *
    * @param v_thread_struct_client Vector de clientes  *
    * @return                       No devuelve nada    *
    ****************************************************/
    void new_search(std::string file_name, std::string word, std::vector<thread_struct> &v_thread_struct_client){
        int lines_num = count_lines(file_name);
        int lines_per_thread = lines_num / NUMBER_OF_THREADS;

        for (int i = 0; i < NUMBER_OF_THREADS; i++){
            int id = i;
            int init_line = i * lines_per_thread;
            int final_line = init_line + lines_per_thread - 1;
            if (i == NUMBER_OF_THREADS - 1){
                final_line = lines_num - 1;
            }
            v_thread_struct.push_back(thread_struct{id, init_line, final_line, word, std::queue<result_struct>()});
            v_threads_books.push_back(std::thread(&Searcher::find_word, this, file_name, i));
        }
        std::for_each(v_threads_books.begin(), v_threads_books.end(), std::mem_fn(&std::thread::join));
        // Se añade la info de los hilos al vector de cliente.
        for (int i = 0; i < NUMBER_OF_THREADS; i++){
            v_thread_struct_client.push_back(v_thread_struct[i]);
        }
        // Se limpian los vectores
        v_thread_struct.clear();
        v_threads_books.clear();
    }


    /****************************************************
    * Busca la palabra en el fichero                    *
    * @param file_name    Nombre del fichero            *
    * @param thread_v_pos Vector de clientes            *
    * @return             No devuelve nada              *
    ****************************************************/
    void find_word(std::string file_name, int thread_v_pos){
        int current_line = 0;
        std::fstream file;
        std::string line;

        file.open(file_name.c_str());
        std::string current_word, previous_word;
        word_to_upper(v_thread_struct[thread_v_pos].word);

        // Lectura línea por línea hasta el límite asignado al hilo
        while (getline(file, line) && current_line <= v_thread_struct[thread_v_pos].final_line){
            result_struct result_str;
            if (current_line >= v_thread_struct[thread_v_pos].initial_line){
                std::istringstream words_in_line(line);
                std::string previous_word, current_word, next_word;
                
                // Procesamiento palabra por palabra en la línea actual
                while (words_in_line >> current_word){
                    word_to_upper(current_word);
                    if (test_symbol(v_thread_struct[thread_v_pos].word, current_word)){
                        word_to_lower(previous_word);
                        result_str.pre_word = previous_word;
                        result_str.line = current_line + 1;

                        // Verificación de palabra posterior
                        if (words_in_line >> next_word){
                            result_str.post_word = next_word;
                        }
                        else{
                            result_str.post_word = "[No hay palabra posterior]";
                        }

                        // Sección crítica: actualización del vector de resultados
                        std::unique_lock<std::shared_mutex> lock(searcher_mtx);
                        v_thread_struct[thread_v_pos].result.push(result_str);
                        if(this->getType() == 0){
                            word_counter++;
                        }
                        if(this->getType() == 1){
                            credit_counter--;
                        }
                        lock.unlock();
                    }
                    previous_word = current_word;
                }
                waiting_mtx.lock();
                if(credit_counter == 0){
                    std::cout << "Waiting for payment..." << std::endl;
                    std::shared_ptr<request> p = std::make_shared<request>();

                    p->balance = credit_counter;
                    p->mtx.lock();

                    std::unique_lock<std::mutex> lock(pay_mtx);
                    payment_queue.push(p);
                    lock.unlock();

                    payment_cv.notify_one();

                    p->mtx.lock();

                    credit_counter = p->balance;
                }
                waiting_mtx.unlock();
            }
            current_line++;
        }
        file.close();
    }
};

/****************************************************
* Gestiona el pago de los clientes premium.         *
* @param .  No requerido                            *
* @return   No devuelve nada                        *
****************************************************/
void pay_system(){
    std::cout << "Pay system running..." << std::endl;
    while(true){
        std::unique_lock<std::mutex> lock(pay_mtx);
        auto wait_result = payment_cv.wait_for(lock, std::chrono::seconds(15), []()
                                               { return !payment_queue.empty(); });
        if(!wait_result){
            std::cout << "Error, no payment done in 15 seconds." << std::endl;
            return;
        }

        std::cout << "Preparing payment..." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        payment_queue.front()->balance = 100;
        payment_queue.front()->mtx.unlock();
        payment_queue.pop();
        std::cout << "Payment done!" << std::endl;
    }
}