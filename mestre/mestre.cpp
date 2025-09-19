#include "lib/httplib.h"
#include "lib/json.hpp"
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <future>

using json = nlohmann::json;

void contarLetras(const std::string &texto, std::promise<int> promise)
{
    try
    {
        httplib::Client cli("escravo1-letras", 8081);
        cli.set_connection_timeout(5, 0); 

        if (auto res_health = cli.Get("/health"))
        {
            if (res_health->status == 200)
            {
                if (auto res = cli.Post("/letras", texto, "text/plain"))
                {
                    if (res->status == 200)
                    {
                        promise.set_value(std::stoi(res->body));
                        return;
                    }
                }
            }
        }
        
        throw std::runtime_error("Falha ao comunicar com escravo de letras.");
    }
    catch (...)
    {
       
        promise.set_exception(std::current_exception());
    }
}


void contarNumeros(const std::string &texto, std::promise<int> promise)
{
    try
    {
        httplib::Client cli("escravo2-numeros", 8082);
        cli.set_connection_timeout(5, 0);

        if (auto res_health = cli.Get("/health"))
        {
            if (res_health->status == 200)
            {
                if (auto res = cli.Post("/numeros", texto, "text/plain"))
                {
                    if (res->status == 200)
                    {
                        promise.set_value(std::stoi(res->body));
                        return;
                    }
                }
            }
        }
        throw std::runtime_error("Falha ao comunicar com escravo de numeros.");
    }
    catch (...)
    {
        promise.set_exception(std::current_exception());
    }
}

int main(void)
{
    httplib::Server svr;

    svr.Post("/processar", [](const httplib::Request &req, httplib::Response &res)
             {
        std::string texto = req.body;
        
        std::promise<int> promise_letras;
        std::future<int> future_letras = promise_letras.get_future();
        std::thread t1(contarLetras, std::cref(texto), std::move(promise_letras));

        std::promise<int> promise_numeros;
        std::future<int> future_numeros = promise_numeros.get_future();
        std::thread t2(contarNumeros, std::cref(texto), std::move(promise_numeros));
        
        t1.detach();
        t2.detach();

        try {
       
            int contagem_letras = future_letras.get();
            int contagem_numeros = future_numeros.get();

            json json_response;
            json_response["letras"] = contagem_letras;
            json_response["numeros"] = contagem_numeros;

            res.set_content(json_response.dump(4), "application/json");
        } catch (const std::exception& e) {
            res.status = 503; 
            res.set_content(e.what(), "text/plain");
        } });

    std::cout << "Servidor Mestre rodando em http://0.0.0.0:8080" << std::endl;
    svr.listen("0.0.0.0", 8080);
    return 0;
}
