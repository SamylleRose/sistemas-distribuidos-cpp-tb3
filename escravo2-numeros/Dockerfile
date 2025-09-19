FROM gcc:latest

WORKDIR /app

COPY lib/ /app/lib/
COPY escravo2-numeros/escravo2.cpp .

RUN g++ -std=c++17 -o escravo2 escravo2.cpp -I./ -pthread

EXPOSE 8082

CMD ["./escravo2"]
