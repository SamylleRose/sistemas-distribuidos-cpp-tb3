# Usar uma imagem base com o compilador GCC
FROM gcc:latest

# Definir o diretório de trabalho dentro do container
WORKDIR /app

# Copiar os arquivos da biblioteca e o código fonte
COPY lib/ /app/lib/
COPY mestre/mestre.cpp .

# Compilar o código C++. A flag -pthread é essencial para std::thread
RUN g++ -std=c++17 -o mestre mestre.cpp -pthread

# Expor a porta que o mestre usará
EXPOSE 8080

# Comando para iniciar o servidor quando o container rodar
CMD ["./mestre"]