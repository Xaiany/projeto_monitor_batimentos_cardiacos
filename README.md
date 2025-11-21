# projeto_monitor_batimentos_cardiacos

# Monitoramento da Saúde Materna com IoT

Este repositório contém o projeto **“Monitoramento da Saúde Materna com IoT”**, desenvolvido na disciplina de Internet das Coisas (IoT) da Universidade Presbiteriana Mackenzie.

O sistema utiliza um ESP32, sensores, atuadores e o protocolo MQTT para monitorar, de forma contínua, condições ambientais e um parâmetro fisiológico simulado da gestante, enviando os dados para a nuvem e permitindo acompanhamento remoto.

---

## Objetivo do projeto

Propor um dispositivo IoT capaz de:

- Coletar dados representativos do estado de saúde materna (temperatura, umidade e “batimentos cardíacos” simulados);
- Classificar automaticamente a situação como **NORMAL**, **ATENÇÃO** ou **CRÍTICO**;
- Emitir **alertas visuais** (LEDs e OLED) e **sonoros** (buzzer);
- Enviar telemetria via **internet (TCP/IP)** utilizando **MQTT** para um broker na nuvem (**HiveMQ Cloud**) e para um assinante (**Node-RED**).

---

## Visão geral da solução

### Componentes principais

- **Microcontrolador:** ESP32-DevKitC  
- **Sensores:**
  - DHT22 – temperatura e umidade;
  - Potenciômetro – simula o sensor MAX30102 para frequência cardíaca.
- **Atuadores:**
  - LEDs: verde, amarelo e vermelho;
  - Buzzer ativo;
  - Display OLED SSD1306 (I2C).
- **Serviços em nuvem e software:**
  - **HiveMQ Cloud** – broker MQTT com TLS;
  - **Node-RED** – assinante MQTT e processamento/visualização;
  - **Wokwi** – simulação do circuito e do firmware;
  - **Arduino IDE** – desenvolvimento do código para ESP32.

---

## Funcionamento resumido

1. O ESP32 lê:
   - Temperatura e umidade do **DHT22**;
   - Valor analógico do **potenciômetro**, mapeado para uma faixa de batimentos cardíacos (HR).
2. Os dados são comparados com limites pré-definidos para cada variável (temperatura, umidade e HR).
3. O sistema calcula o **status**:
   - `NORMAL`: nenhum parâmetro fora da faixa segura;
   - `ATENCAO`: um parâmetro fora da faixa;
   - `CRITICO`: dois ou mais parâmetros fora da faixa.
4. De acordo com o status:
   - LEDs indicam a condição (verde, amarelo ou vermelho);
   - O buzzer emite bipes diferentes para **ATENÇÃO** e **CRÍTICO**;
   - O display OLED mostra todos os valores atuais e o status.
5. O ESP32 publica um **JSON** em um tópico MQTT no HiveMQ Cloud.
6. O **Node-RED** assina o tópico, recebe os dados em tempo real e pode encaminhá-los para banco, dashboards, e-mail ou SMS.
