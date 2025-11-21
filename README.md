# Monitoramento da Saúde Materna com IoT

Este repositório contém o projeto **“Monitoramento da Saúde Materna com IoT”**, desenvolvido na disciplina de Internet das Coisas (IoT) da Universidade Presbiteriana Mackenzie.

O sistema utiliza um ESP32, sensores, atuadores e o protocolo MQTT para monitorar, de forma contínua, condições ambientais e um parâmetro fisiológico simulado da gestante, enviando os dados para a nuvem e permitindo acompanhamento remoto.

---

## 🩺 Breve Descrição do Funcionamento e Uso

Este projeto apresenta um **sistema IoT para monitoramento da saúde materna**, desenvolvido com ESP32, DHT22, potenciômetro, display OLED, LEDs e buzzer.  
Ele coleta temperatura, umidade e um sinal analógico que simula batimentos cardíacos, classificando automaticamente a situação da gestante em **NORMAL**, **ATENÇÃO** ou **CRÍTICO**.

Tudo é atualizado em tempo real no display OLED, LEDs e buzzer, e as informações são enviadas via **MQTT** para um broker na nuvem (HiveMQ Cloud). No lado do assinante, o **Node-RED** recebe, exibe e processa as mensagens publicadas pelo dispositivo.

Com esse repositório, qualquer pessoa pode:

1. **Reproduzir a montagem** usando o Wokwi ou hardware real.  
2. **Compilar e enviar o firmware** completo para um ESP32.  
3. **Conectar o dispositivo à internet** via Wi-Fi.
4. **Transmitir telemetria MQTT** para o HiveMQ Cloud com conexão TLS.
5. **Assinar os dados no Node-RED**, construindo dashboards, alarmes e notificações.  

O projeto demonstra um fluxo completo de IoT aplicado à área da saúde:
- Leitura de sensores ✔️  
- Processamento de dados ✔️  
- Interface local (OLED, LEDs e buzzer) ✔️  
- Comunicação via internet (TCP/IP) ✔️  
- Protocolo MQTT seguro (TLS + autenticação) ✔️  
- Assinatura e tratamento no Node-RED ✔️  

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

## Código-fonte (Software Desenvolvido)

O firmware completo utilizado no ESP32 está disponível na pasta:

👉 [`/firmware`](./firmware)

O código está comentado e organizado para facilitar a compreensão de cada etapa do funcionamento do sistema, incluindo:
- Configuração de Wi-Fi e MQTT com TLS;
- Leitura de sensores;
- Cálculo do status (NORMAL, ATENÇÃO, CRÍTICO);
- Controle de LEDs, buzzer e display OLED;
- Publicação MQTT com JSON;
- Reconexão automática com o broker.
