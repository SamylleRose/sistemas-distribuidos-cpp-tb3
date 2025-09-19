#include "lib/httplib.h"
#include <cctype> 
#include <iostream>

int main(void)
{
    httplib::Server svr;

   
    svr.Get("/health", [](const httplib::Request &, httplib::Response &res)
            { res.set_content("OK", "text/plain"); });

    
    svr.Post("/letras", [](const httplib::Request &req, httplib::Response &res)
             {
        std::string texto = req.body;
        int count = 0;
        for (char c : texto) {
            if (isalpha(c)) {
                count++;
            }
        }
        res.set_content(std::to_string(count), "text/plain"); });

    std::cout << "Servidor Escravo 1 (Letras) rodando em http://0.0.0.0:8081" << std::endl;
    svr.listen("0.0.0.0", 8081);

    return 0;
}
