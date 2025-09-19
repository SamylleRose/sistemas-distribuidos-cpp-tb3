# Atividade 4 – THREADS, MIDDLEWARE, PARALELISMO, DOCKER (C++)
Este projeto implementa um sistema distribuído em C++ utilizando uma arquitetura Mestre-Escravo. A comunicação é feita via HTTP REST, e os servidores (Mestre e Escravos) são orquestrados com Docker e Docker Compose. O cliente é uma aplicação com interface gráfica feita em Qt 5.

´´´
Estrutura de Pastas
/
├── cliente/
│   └── cliente.cpp             # Código-fonte do cliente com GUI em Qt
├── escravo1-letras/
│   ├── escravo1.cpp            # Código-fonte do servidor que conta letras
│   └── Dockerfile              # Dockerfile para o Escravo 1
├── escravo2-numeros/
│   ├── escravo2.cpp            # Código-fonte do servidor que conta números
│   └── Dockerfile              # Dockerfile para o Escravo 2
├── mestre/
│   ├── mestre.cpp              # Código-fonte do servidor Mestre
│   └── Dockerfile              # Dockerfile para o Mestre
├── lib/
│   ├── httplib.h               # Biblioteca para requisições HTTP
│   └── json.hpp                # Biblioteca para manipulação de JSON
└── docker-compose.yml          # Arquivo para orquestrar os containers
´´´
Pré-requisitos
Antes de começar, garanta que você tem os seguintes softwares instalados no seu ambiente Linux (ou WSL no Windows):

Docker e Docker Compose: Essencial para rodar os servidores. A forma mais fácil é instalar o Docker Desktop.

Compilador C++ e Ferramentas de Build: g++ e build-essential.

Bibliotecas de Desenvolvimento do Qt 5: Para compilar a interface gráfica do cliente.

Como Compilar e Executar (Passo a Passo)
Siga os comandos abaixo no seu terminal, a partir da pasta raiz do projeto.

Passo 1: Iniciar os Servidores com Docker
Primeiro, vamos construir as imagens Docker e iniciar os containers do Mestre e dos dois Escravos. Eles rodarão em segundo plano.

# O 'sudo' é necessário caso seu usuário não pertença ao grupo 'docker'.
# O '--build' garante que as imagens serão construídas do zero.
# O '-d' (detached) faz os containers rodarem em segundo plano.

sudo docker compose up --build -d

Após executar este comando, os três servidores estarão online e prontos para receber requisições.

Passo 2: Instalar as Dependências do Cliente Gráfico
Agora, vamos instalar as ferramentas de compilação e as bibliotecas do Qt 5 necessárias para o cliente. Se você já as tem, pode pular este passo.

´´´
# Atualiza a lista de pacotes e instala tudo de uma vez:
# build-essential: Compilador g++ e outras ferramentas.
# qtbase5-dev: Bibliotecas de desenvolvimento do Qt 5.
# pkg-config: Ferramenta que ajuda o compilador a encontrar as bibliotecas.

sudo apt-get update && sudo apt-get install -y build-essential qtbase5-dev pkg-config
´´´
Passo 3: Compilar o Cliente Gráfico
Com as dependências instaladas, compile o arquivo cliente.cpp para gerar o programa executável.

# Este comando compila o cliente:
# -std=c++17: Usa o padrão C++17.
# -fPIC: Flag necessária para a compilação com as bibliotecas Qt.
# $(pkg-config ...): Usa o pkg-config para linkar automaticamente com o Qt.
# -o cliente_gui: Define o nome do arquivo executável de saída.

g++ -std=c++17 -fPIC cliente/cliente.cpp -o cliente_gui $(pkg-config --cflags --libs Qt5Widgets)

Se o comando executar sem erros, um arquivo chamado cliente_gui será criado na raiz do projeto.

Passo 4: Executar o Cliente
Finalmente, inicie a aplicação gráfica!

./cliente_gui

A interface do cliente irá aparecer. Digite um texto, clique em "Processar Texto" e observe os resultados retornados pelos servidores Docker.

Como Parar os Servidores
Quando terminar de usar a aplicação, você pode parar e remover os containers Docker com o seguinte comando:

sudo docker compose down
