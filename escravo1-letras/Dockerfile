FROM gcc:latest

WORKDIR /app

COPY lib/ /app/lib/
COPY escravo1-letras/escravo1.cpp .

RUN g++ -std=c++17 -o escravo1 escravo1.cpp -I./ -pthread

EXPOSE 8081

CMD ["./escravo1"]
