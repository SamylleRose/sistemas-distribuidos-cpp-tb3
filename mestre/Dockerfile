FROM gcc:latest

WORKDIR /app

COPY lib/ /app/lib/
COPY mestre/mestre.cpp .

RUN g++ -std=c++17 -o mestre mestre.cpp -I./ -pthread

EXPOSE 8080

CMD ["./mestre"]
