#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>
#include <future>
#include <QTimer> // <<< CORREÇÃO: Adicionado o include que faltava

// Caminho corrigido para compilação local, voltando um diretório
#include "../lib/httplib.h"
#include "../lib/json.hpp"

// Usar o nlohmann::json para facilitar o parsing do JSON
using json = nlohmann::json;

// Estrutura para conter o resultado da chamada de rede
struct ProcessingResult {
    bool success;
    int letter_count;
    int number_count;
    std::string error_message;
};

// Função que executa a requisição HTTP numa thread separada
ProcessingResult processText(const std::string& text_to_process) {
    httplib::Client cli("localhost", 8080);
    cli.set_connection_timeout(10, 0); // Timeout de 10 segundos

    auto res = cli.Post("/processar", text_to_process, "text/plain");

    if (res) {
        if (res->status == 200) {
            try {
                json response_json = json::parse(res->body);
                return {true, response_json["letras"], response_json["numeros"], ""};
            } catch (const json::parse_error& e) {
                return {false, 0, 0, "Erro ao descodificar JSON: " + std::string(e.what())};
            }
        } else {
            return {false, 0, 0, "Servidor retornou erro " + std::to_string(res->status) + ": " + res->body};
        }
    } else {
        auto err = res.error();
        return {false, 0, 0, "Erro de conexão: " + httplib::to_string(err)};
    }
}


// A classe da nossa janela principal
class MainWindow : public QWidget {
public:
    MainWindow(QWidget *parent = nullptr) : QWidget(parent) {
        setupUI();
    }

private:
    void setupUI() {
        // Configurações da janela
        setWindowTitle("Cliente - Análise de Texto");
        setMinimumSize(400, 300);

        // Layout principal
        QVBoxLayout *layout = new QVBoxLayout(this);

        // Widgets da interface
        infoLabel = new QLabel("Digite ou cole o texto a ser analisado abaixo:");
        textInput = new QTextEdit();
        processButton = new QPushButton("Processar Texto");
        statusLabel = new QLabel("Pronto.");
        resultLettersLabel = new QLabel("Quantidade de Letras: -");
        resultNumbersLabel = new QLabel("Quantidade de Números: -");

        // Adiciona os widgets ao layout
        layout->addWidget(infoLabel);
        layout->addWidget(textInput);
        layout->addWidget(processButton);
        layout->addWidget(statusLabel);
        layout->addWidget(resultLettersLabel);
        layout->addWidget(resultNumbersLabel);
        
        // Conecta o sinal de clique do botão à nossa função (slot)
        connect(processButton, &QPushButton::clicked, this, &MainWindow::onProcessButtonClick);
    }
    
    void onProcessButtonClick() {
        std::string text = textInput->toPlainText().toStdString();
        if (text.empty()) {
            QMessageBox::warning(this, "Aviso", "A caixa de texto está vazia.");
            return;
        }

        // Desabilita o botão e atualiza o status para evitar cliques duplos
        processButton->setEnabled(false);
        statusLabel->setText("A processar...");

        // Executa a tarefa de rede de forma assíncrona para não bloquear a GUI
        processing_future = std::async(std::launch::async, processText, text);
        
        // Usa um QTimer para verificar o resultado sem bloquear a thread principal
        QTimer *timer = new QTimer(this);
        connect(timer, &QTimer::timeout, [this, timer]() {
            if (processing_future.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
                timer->stop(); 
                ProcessingResult result = processing_future.get();
                handleResult(result);
                timer->deleteLater();
            }
        });
        timer->start(100);
    }

    void handleResult(const ProcessingResult& result) {
        if (result.success) {
            resultLettersLabel->setText("Quantidade de Letras: " + QString::number(result.letter_count));
            resultNumbersLabel->setText("Quantidade de Números: " + QString::number(result.number_count));
            statusLabel->setText("Processamento concluído com sucesso!");
        } else {
            resultLettersLabel->setText("Quantidade de Letras: -");
            resultNumbersLabel->setText("Quantidade de Números: -");
            statusLabel->setText("Falha no processamento.");
            QMessageBox::critical(this, "Erro", QString::fromStdString(result.error_message));
        }
        
        processButton->setEnabled(true);
    }

    QLabel *infoLabel;
    QTextEdit *textInput;
    QPushButton *processButton;
    QLabel *statusLabel;
    QLabel *resultLettersLabel;
    QLabel *resultNumbersLabel;
    std::future<ProcessingResult> processing_future;
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow window;
    window.show();
    return app.exec();
}

