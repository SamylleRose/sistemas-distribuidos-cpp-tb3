#include "lib/httplib.h"
#include <cctype> // Para a função isdigit
#include <iostream>

int main(void)
{
    httplib::Server svr;

    // Endpoint para checagem de saúde
    svr.Get("/health", [](const httplib::Request &, httplib::Response &res)
            { res.set_content("OK", "text/plain"); });

    // Endpoint principal que conta os números
    svr.Post("/numeros", [](const httplib::Request &req, httplib::Response &res)
             {
        std::string texto = req.body;
        int count = 0;
        for (char c : texto) {
            if (isdigit(c)) {
                count++;
            }
        }
        res.set_content(std::to_string(count), "text/plain"); });

    std::cout << "Servidor Escravo 2 (Numeros) rodando em http://0.0.0.0:8082" << std::endl;
    svr.listen("0.0.0.0", 8082);

    return 0;
}
