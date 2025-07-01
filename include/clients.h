/************************************************************************************
 * Práctica 3 - Sistemas Operativos II                                              *
 * SSOOIIGLE - clients.h                                                            *
 * Autor: Diego García Álvarez                                                    *
 * 31/04/2025                                                                       *
 * Clase cliente, de la que heredan cliente gratuito, premium y premium ilimitado.  *                                                                      *
 ************************************************************************************/

#include <iostream>
#include <thread>
#include <string>
#include <queue>
#include <mutex>
#include <fstream>
#include <definitions.h>
#include <thread_struct.h>
#include <string>
#pragma once

extern std::mutex print_mutex;

class Client{
    public:
        int id;
        std::string word;
        std::string client_type;
        std::vector<thread_struct> v_thread_struct;

    public:
        Client(int id, std::string word, std::string client_type){
            this->id = id;
            this->word = word;
            this->client_type = client_type;
        }

        int getId() const{
            return id;
        }

        std::string getWord() const{
            return word;
        }
    /****************************************************
    * Imprime los resultados                            *
    * @param word  Palabra a buscar                     *
    * @param title Titulo donde buscar                  *
    * @return      No devuelve nada                     *
    ****************************************************/
    void print_search_results(std::string word, std::string title){
        std::lock_guard<std::mutex> lock(print_mutex);

        std::ofstream outfile("docs/Resultados_busquedas.txt", std::ios::app);
        bool found_results = false;

        outfile << "-------------------------------------------------------------------------------------------------" << std::endl;
        outfile << "Cuenta: " << client_type << " | Id: " << id << " :: palabra " << word << std::endl;
        outfile << "Libro: " << title << "\n" << std::endl;

        for (int i = 0; i < NUMBER_OF_THREADS ; i++){
            while (!v_thread_struct[i].result.empty()){
                found_results = true;
                outfile << "[Hilo " << i << " inicio:" << v_thread_struct[i].initial_line 
                        << " - final: " << v_thread_struct[i].final_line << "]";
                outfile << " :: linea " << v_thread_struct[i].result.front().line 
                        << " :: ... " << v_thread_struct[i].result.front().pre_word << " "
                        << word << " " << v_thread_struct[i].result.front().post_word << " ..." 
                        << std::endl;

                v_thread_struct[i].result.pop();
            }
        }

        if (!found_results) {
            outfile << "No se han encontrado resultados para " << word << " en el libro " << title << std::endl;
        }
    }
};

class free_client : public Client {
    public:
        free_client(int id, std::string word, std::string client_type) : Client(id, word, client_type){}
};

class premium_client : public Client {
    public:
        double balance;

    public:
        premium_client(int id, std::string word, std::string client_type, double balance) : Client(id, word, client_type){
            this->balance = balance;
        }

        void set_balance(double balance){
            this->balance = balance;
        }

        double get_balance() const{
            return balance;
        }
};

class unlimited_client : public Client {
    public:
        unlimited_client(int id, std::string word, std::string client_type) : Client(id, word, client_type){}
};