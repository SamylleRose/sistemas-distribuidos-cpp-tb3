// mestre/mestre.cpp
#include "lib/httplib.h"
#include "lib/json.hpp"
#include <iostream>
#include <string>

using json = nlohmann::json;

// Função para a thread que se comunica com o Escravo 1 (Letras)
void contarLetras(const std::string &texto, int &resultado)
{
  httplib::Client cli("http://escravo1-letras:8081");
  cli.set_connection_timeout(5, 0); // 5 segundos de timeout

  if (auto res_health = cli.Get("/health"))
  {
    if (res_health->status == 200)
    {
      if (auto res = cli.Post("/letras", texto, "text/plain"))
      {
        if (res->status == 200)
        {
          resultado = std::stoi(res->body);
          return;
        }
      }
    }
  }
  resultado = -1; // Indica erro
}

// Função para a thread que se comunica com o Escravo 2 (Números)
void contarNumeros(const std::string &texto, int &resultado)
{
  httplib::Client cli("http://escravo2-numeros:8082");
  cli.set_connection_timeout(5, 0);

  if (auto res_health = cli.Get("/health"))
  {
    if (res_health->status == 200)
    {
      if (auto res = cli.Post("/numeros", texto, "text/plain"))
      {
        if (res->status == 200)
        {
          resultado = std::stoi(res->body);
          return;
        }
      }
    }
  }
  resultado = -1; // Indica erro
}

int main(void)
{
  httplib::Server svr;

  svr.Post("/processar", [](const httplib::Request &req, httplib::Response &res)
           {
        std::string texto = req.body;
        int contagem_letras = 0;
        int contagem_numeros = 0;

        // Dispara as threads em paralelo
        std::thread t1(contarLetras, std::cref(texto), std::ref(contagem_letras));
        std::thread t2(contarNumeros, std::cref(texto), std::ref(contagem_numeros));

        // Espera as threads finalizarem
        t1.join();
        t2.join();
        
        if (contagem_letras == -1 || contagem_numeros == -1) {
            res.status = 503; // Service Unavailable
            res.set_content("Erro ao comunicar com um ou mais escravos.", "text/plain");
        } else {
            // Monta a resposta JSON
            json json_response;
            json_response["letras"] = contagem_letras;
            json_response["numeros"] = contagem_numeros;

            res.set_content(json_response.dump(4), "application/json");
        } });

  std::cout << "Servidor Mestre rodando em http://0.0.0.0:8080" << std::endl;
  svr.listen("0.0.0.0", 8080);
}