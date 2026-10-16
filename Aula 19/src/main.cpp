#include <WiFi.h>
#include <WebServer.h>

// Dados da rede Wi-Fi
const char *ssid = "iPhone de Kauan";
const char *password = "kauan2526";

WebServer server(80);

const int LED = 2;
bool ledLigado = false;

// Página HTML
String paginaHTML() {
    String html = R"rawliteral(
    <!DOCTYPE html>
    <html lang="pt-BR">
    <head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">

        <title>ESP32 | Controle de LED</title>

        <style>
            * {
                margin: 0;
                padding: 0;
                box-sizing: border-box;
            }

            body {
                font-family: Arial, Helvetica, sans-serif;
                background: #eef3f9;
                color: #172b45;
                min-height: 100vh;
            }

            header {
                background: #172b45;
                color: white;
                padding: 22px;
                text-align: center;
            }

            header h2 {
                font-size: 25px;
                margin-bottom: 5px;
            }

            header p {
                color: #cbd5e1;
                font-size: 14px;
            }

            main {
                width: 90%;
                max-width: 750px;
                margin: 50px auto;
                padding: 35px 25px;
                background: white;
                border-radius: 18px;
                box-shadow: 0 8px 25px rgba(23, 43, 69, 0.10);
                text-align: center;
            }

            .icone {
                font-size: 65px;
                margin-bottom: 15px;
            }

            h1 {
                font-size: 32px;
                margin-bottom: 12px;
            }

            .descricao {
                color: #64748b;
                margin-bottom: 30px;
                line-height: 1.5;
            }

            .botoes {
                display: flex;
                justify-content: center;
                gap: 15px;
                flex-wrap: wrap;
            }

            .botao {
                display: block;
                flex: 1;
                min-width: 160px;
                padding: 20px 15px;
                color: white;
                text-decoration: none;
                border-radius: 12px;
                font-size: 17px;
                font-weight: bold;
                transition: transform 0.2s, opacity 0.2s;
            }

            
            .botao:hover {
                transform: translateY(-4px);
                opacity: 0.9;
            }

            .botao span {
                display: block;
                font-size: 30px;
                margin-bottom: 10px;
            }

            .ligar {
                background: #16a34a;
            }

            .desligar {
                background: #dc2626;
            }

            .piscar {
                background: #f59e0b;
            }

            footer {
                margin-top: 35px;
                padding-top: 20px;
                border-top: 1px solid #e2e8f0;
                color: #64748b;
                font-size: 13px;
                line-height: 1.8;
            }

            @media (max-width: 500px) {
                main {
                    margin: 25px auto;
                    padding: 25px 18px;
                }

                h1 {
                    font-size: 26px;
                }

                .botao {
                    flex-basis: 100%;
                }
            }
        </style>
    </head>

    <body>

        <header>
            <h2>ESP32</h2>
            <p>Sistema de controle inteligente</p>
        </header>

        <main>
            <div class="icone">💡</div>

            <h1>Controle de LED</h1>

            <p class="descricao">
                Controle seu LED diretamente pelo navegador.
                Escolha uma das opções abaixo.
            </p>

            <div class="botoes">

                <a class="botao ligar" href="/ligar">
                    <span>☀</span>
                    Ligar LED
                </a>

                <a class="botao desligar" href="/desligar">
                    <span>◯</span>
                    Desligar LED
                </a>

                <a class="botao piscar" href="/piscar">
                    <span>✨</span>
                    Piscar LED
                    <small style="display:block; margin-top:8px; font-weight:normal;">
                        10 vezes
                    </small>
                </a>

            </div>

            <footer>
                <strong>ESP32 — Servidor Web</strong>
                <br>
                Projeto de automação com Wi-Fi.
            </footer>
        </main>

    </body>
    </html>
    )rawliteral";

    return html;
}

void setup() {
    Serial.begin(115200);

    // Configura o LED
    pinMode(LED, OUTPUT);
    digitalWrite(LED, LOW);

    // Conecta ao Wi-Fi
    WiFi.begin(ssid, password);

    Serial.print("Conectando ao Wi-Fi");

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("\nConectado!");
    Serial.print("Acesse no navegador: http://");
    Serial.println(WiFi.localIP());

    // Rota principal
    server.on("/", HTTP_GET, []() {
        server.send(200, "text/html", paginaHTML());
    });

    // Rota para ligar o LED
    server.on("/ligar", HTTP_GET, []() {
        ledLigado = true;
        digitalWrite(LED, HIGH);

        Serial.println("LED ligado!");

        server.sendHeader("Location", "/");
        server.send(303, "text/plain", "");
    });

    // Rota para desligar o LED
    server.on("/desligar", HTTP_GET, []() {
        ledLigado = false;
        digitalWrite(LED, LOW);

        Serial.println("LED desligado!");

        server.sendHeader("Location", "/");
        server.send(303, "text/plain", "");
    });

    server.on("/piscar", HTTP_GET, []() {
        int contador = 0;

        while (contador < 10) {
          digitalWrite(LED, HIGH);
          delay(500);

          digitalWrite(LED, LOW);
          delay(500);

          contador++;
        }


        server.sendHeader("Location", "/");
        server.send(303, "text/plain", "");
    });

    // Inicia o servidor
    server.begin();

    Serial.println("Servidor iniciado!");
}

void loop() {
    server.handleClient();
}