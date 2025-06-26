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
#include <request_struct.h>
#include <thread_struct.h>
#include <mutex>
#include <fstream>
#include <definitions.h>

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
}

class free_client : public Client {
    public:
        free_client(int id, std::string word, std::string client_type) : Client(id, word, client_type){}
};

class premium_client : public Client {
    public:
        double balance;

    public:
        premium_client(int id, std::word, std::string client_type) : Client(id, word, client_type){
            this->balance = balance;
        }

        void set_balance(double balance){
            this->balance = balance;
        }

        void get_balance() const{
            return balance;
        }
};

class unlimited_client : public Client {
    public:
        unlimited_client(int id, std::string word, std::string client_type) : Client(id, word, client_type){}
};