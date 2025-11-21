# Documentação das Interfaces, Protocolos e Módulos de Comunicação  
Projeto: Monitoramento da Saúde Materna com IoT  

Este documento detalha toda a comunicação do sistema:  
- As **interfaces** utilizadas entre hardware, software e serviços em nuvem;  
- Os **protocolos de rede** e mensagens;  
- Os **módulos de comunicação** implementados no ESP32, HiveMQ Cloud e Node-RED.

---

# 1. Visão Geral da Comunicação

O projeto utiliza um fluxo de comunicação baseado em **Internet (TCP/IP)** e **MQTT com TLS**, garantindo transporte seguro e confiável dos dados do dispositivo até a nuvem.

Fluxo completo:

1. O **ESP32** coleta dados dos sensores (DHT22 + potenciômetro).  
2. Conecta-se à Internet via **Wi-Fi (TCP/IP)**.  
3. Estabelece sessão segura com o broker MQTT na nuvem via **TLS (porta 8883)**.  
4. Publica mensagens no tópico:  
   **`gestacao/gestacao_device_01/dados`**  
5. O **Node-RED** assina o mesmo tópico e exibe/processa os dados em tempo real.

---

# 2. Interfaces do Sistema

As interfaces são os pontos de integração entre os componentes do projeto.

## 2.1 Interface ESP32 → Sensores
### Barramento utilizado
- **DHT22:** comunicação digital proprietária (1-wire-like)
- **Potenciômetro:** entrada analógica (ADC) no pino 34

### Dados coletados
| Sensor         | Tipo de Sinal | Variável obtida   |
|----------------|---------------|--------------------|
| DHT22          | Digital       | Temperatura e Umidade |
| Potenciômetro  | Analógico     | HR simulado (batimentos) |

---

## 2.2 Interface ESP32 → OLED
- Protocolo: **I2C**  
- Pinos ESP32: **SDA (21)** e **SCL (22)**  
- Endereço do display: **0x3C**

Exibe:
- Temperatura  
- Umidade  
- HR (potenciômetro)  
- Status (NORMAL, ATENÇÃO, CRITICO)

---

## 2.3 Interface ESP32 → LEDs e Buzzer
Atuadores digitais:

| Atuador | Pino | Descrição |
|---------|------|-----------|
| LED Verde   | 14 | Status NORMAL |
| LED Amarelo | 27 | Status ATENÇÃO |
| LED Vermelho| 26 | Status CRÍTICO |
| Buzzer      | 25 | Alertas sonoros |

---

## 2.4 Interface ESP32 → Internet
O ESP32 conecta-se ao roteador usando:

- **Protocolo:** Wi-Fi IEEE 802.11b/g/n  
- **Modo:** Station (STA)  
- **Stack TCP/IP interno do ESP32**

Permite acessar o broker MQTT na nuvem.

---

# 3. Protocolos Utilizados

## 3.1 Wi-Fi (802.11)
Utilizado apenas como meio físico e enlace de dados.

Função:
- Transportar pacotes IP entre ESP32 ↔ Internet

---

## 3.2 TCP/IP
A camada base para comunicação com o broker MQTT.

- **Transporte:** TCP sobre IPv4  
- **Porta usada:** 8883 (MQTT + TLS)  
- **Garantia:** entrega ordenada e confirmada dos pacotes

---

## 3.3 TLS (Transport Layer Security)
O sistema utiliza **conexão segura**, exigida pelo HiveMQ Cloud.

Recursos usados:
- Certificado raiz fornecido no firmware (`root_ca`)
- Cliente seguro: `WiFiClientSecure`
- Verificação do servidor antes da conexão MQTT

Objetivo:
- Proteção contra interceptação ou fraude (Man-in-the-middle)

---

## 3.4 MQTT (Message Queuing Telemetry Transport)
Protocolo principal de comunicação na nuvem.

### Características:
- Baseado em **publish/subscribe**  
- Baixa latência  
- Ideal para IoT  

### Versão utilizada:
- **MQTT 3.1.1** (implementada pela biblioteca PubSubClient)

---

# 4. Módulos de Comunicação Implementados

## 4.1 Módulo Wi-Fi (`connectWiFi()`)
Funções:
- Configura modo station
- Conecta ao roteador
- Exibe IP
- Garante reconexão em caso de queda

---

## 4.2 Módulo MQTT (`connectMQTT()`)
Funções:
- Define servidor/porta
- Constrói ClientID dinâmico
- Autentica com **usuário e senha**
- Garante reconexão automática

---

## 4.3 Módulo de Telemetria (`publishTelemetry()`)
Responsável por enviar os dados em formato JSON ao broker.

Tópico:
gestacao/gestacao_device_01/dados

Formato do payload:
``json
{
  "device": "gestacao_device_01",
  "temperature": 36.7,
  "humidity": 50.0,
  "heartrate": 92,
  "spo2": 0.0,
  "status": "ATENCAO"
}``

---

4.4 Node-RED como Assinante MQTT

O Node-RED utiliza:
- Nó mqtt in
- Configuração:
Endereço do broker: [xxxxxx.s1.eu.hivemq.cloud](https://console.hivemq.cloud/clusters/87212734588542ee85f53c5497bb0e89)
  - Porta 8883
  - TLS ativado
  - Mesmos usuário/senha do ESP32
Assinatura do tópico:
gestacao/gestacao_device_01/dados

Objetivo:
- Exibir dados em tempo real
Registrar histórico (opcional)
Encaminhar para banco, dashboard, e-mail etc.

5. Estrutura dos Tópicos MQTT
gestacao/
   └── gestacao_device_01/
          └── dados
   
Padrão adotado:
<categoria>/<dispositivo>/<tipo_de_mensagem>
Permite:
- Escalar para múltiplos sensores
- Separar telemetria de comandos (futuro)

6. Formato das Mensagens
Tipo: Telemetria
JSON enviado pelo ESP32 a cada 3 segundos

Campos:
| Campo       | Tipo    | Descrição                      |
| ----------- | ------- | ------------------------------ |
| device      | String  | ID único do dispositivo        |
| temperature | Float   | ºC                             |
| humidity    | Float   | %                              |
| heartrate   | Inteiro | bpm simulados                  |
| spo2        | Float   | mantido a 0 (não implementado) |
| status      | String  | NORMAL / ATENÇÃO / CRITICO     |

7. Segurança na Comunicação

O sistema implementa os seguintes mecanismos:

✔ TLS 1.2 obrigatório pelo HiveMQ Cloud
✔ Certificado raiz embutido no firmware
✔ Autenticação por usuário e senha
✔ Porta 8883 (MQTT seguro)
✔ Reconexão automática

8. Resumo Arquitetural
   
   [DHT22 + POT] --> [ESP32]
                      |
                 (Wi-Fi TCP/IP)
                      |
                  [TLS + MQTT]
                      |
             [HiveMQ Cloud Broker]
                      |
             (subscrição do tópico)
                      |
                   [Node-RED]
