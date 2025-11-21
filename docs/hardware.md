# Documentação de Hardware do Projeto  
### Monitoramento da Saúde Materna com IoT – ESP32 + MQTT + Node-RED

Este documento descreve todo o hardware utilizado no protótipo, incluindo plataforma de desenvolvimento, sensores, atuadores, conexões, montagem e considerações sobre estrutura física do dispositivo.

---

## 1. Plataforma de desenvolvimento

### **ESP32 DevKitC / NodeMCU-ESP32**
A placa ESP32 é o núcleo do sistema e é responsável por:

- Leitura dos sensores (analógicos e digitais);
- Processamento dos dados de temperatura, umidade e ritmo cardíaco simulado;
- Comunicação via Wi-Fi (TCP/IP);
- Publicação dos dados no broker MQTT (HiveMQ Cloud) utilizando TLS;
- Acionamento dos atuadores: LEDs, buzzer e display OLED.

Principais recursos utilizados:

- GPIOs digitais (para LEDs e buzzer)
- GPIO analógico (para o potenciômetro)
- I2C (SDA / SCL) para o display OLED
- Alimentação: 5V via USB (Wokwi simula automaticamente)

---

## 2. Sensores

### **2.1 DHT22 – Sensor de Temperatura e Umidade**
Funções:
- Mede temperatura do ambiente (°C)
- Mede umidade relativa do ar (%)

Pinos utilizados:
- VCC → 3.3V  
- GND → GND  
- DATA → GPIO 4  

---

### **2.2 Potenciômetro – Simulação do Sensor MAX30102**

No projeto real, o sensor ideal seria o **MAX30102**, capaz de medir:

- Batimentos cardíacos (HR)
- Saturação de O₂ (SpO2)

Como o Wokwi não possui esse sensor, foi utilizado um **potenciômetro** (POT) para simular variações de batimentos cardíacos.

Conexões:
- Terminal 1 → 3.3V  
- Terminal 2 (cursor) → GPIO 34  
- Terminal 3 → GND  

Função no sistema:
- O valor lido (0–4095) é convertido para uma faixa de 50 a 160 bpm.
- Serve para simular variações leves, moderadas e críticas de HR.

---

## 3. Atuadores

### **3.1 LEDs indicadores**
Três LEDs são utilizados como alerta visual de status:

| LED | Cor | Pino | Significado |
|-----|-----|-------|-------------|
| LED Verde | Verde | GPIO 14 | Status NORMAL |
| LED Amarelo | Amarelo | GPIO 27 | Status ATENÇÃO |
| LED Vermelho | Vermelho | GPIO 26 | Status CRÍTICO |

Todos ligados em série com resistores apropriados simulados no Wokwi.

---

### **3.2 Buzzer ativo**
Função: emitir alertas sonoros conforme o estado da paciente.

- Pino utilizado: GPIO 25
- Comportamento:
  - NORMAL → silêncio  
  - ATENÇÃO → 1 bip curto  
  - CRÍTICO → 3 bipes curtos (intervalados)

---

### **3.3 Display OLED SSD1306 (I2C)**
Exibe todas as leituras do sistema:

- Temperatura
- Umidade
- Batimentos cardíacos simulados (HR)
- Valor analógico e tensão do POT
- Status geral do sistema

Conexões:

| Pino OLED | Conectar ao ESP32 |
|----------|--------------------|
| VCC | 3.3V |
| GND | GND |
| SDA | GPIO 21 |
| SCL | GPIO 22 |

Formato: 128×64 px.

---

## 4. Estrutura física, peças e impressão 3D

### **4.1 Uso no protótipo simulado**
O projeto foi executado totalmente no ambiente virtual **Wokwi**, portanto:

- **Não há caixa física**
- **Não há peças impressas em 3D**
- **Não há medidas de carcaça**
- **A montagem é unicamente virtual**

Mesmo assim, é importante registrar isso no repositório, conforme exigido pelo professor.

---

## 5. Diagrama de montagem

A simulação Wokwi contém a montagem completa do circuito, incluindo:

- Sensores conectados aos respectivos GPIOs
- Barramento I2C para o display OLED
- LEDs com resistores
- Buzzer
- Potenciômetro simulando HR

---

## 6. Link do projeto no Wokwi

*(https://wokwi.com/projects/445924913816473601)*

