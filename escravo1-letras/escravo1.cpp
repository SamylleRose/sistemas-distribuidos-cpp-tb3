// escravo1-letras/escravo1.cpp
#include "lib/httplib.h"
#include "lib/json.hpp"
#include <cctype> // Para a função isalpha

int main(void)
{
  httplib::Server svr;

  // Endpoint para checagem de saúde
  svr.Get("/health", [](const httplib::Request &, httplib::Response &res)
          { res.set_content("OK", "text/plain"); });

  // Endpoint principal que conta as letras
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
}