# Documentação das Interfaces, Protocolos e Módulos de Comunicação

Este documento descreve toda a comunicação do projeto “Monitoramento da Saúde Materna com IoT”, incluindo:  
- Interface com sensores e atuadores  
- Comunicação entre módulos internos  
- Comunicação entre o ESP32 e a Internet  
- Interação com o broker MQTT (HiveMQ Cloud)  
- Assinatura e processamento via Node-RED

---

## 1. Interface com Sensores e Atuadores

### **Sensores**
| Sensor | Interface | Função |
|-------|-----------|---------|
| **DHT22** | Digital (pino 4) | Mede temperatura e umidade |
| **Potenciômetro** | Analógica (ADC – pino 34) | Simula batimentos cardíacos (MAP para 50–160 bpm) |

### **Atuadores**
| Atuador | Interface | Função |
|---------|-----------|--------|
| LED Verde | Digital (pino 14) | Status NORMAL |
| LED Amarelo | Digital (pino 27) | Status ATENÇÃO |
| LED Vermelho | Digital (pino 26) | Status CRÍTICO |
| Buzzer | Digital (pino 25) | Emissão de alertas sonoros |
| OLED SSD1306 | I2C (SDA=21, SCL=22) | Exibe leituras e status |

Todas essas interfaces são tratadas diretamente pelo ESP32 dentro do loop principal.

---

## 2. Comunicação via Internet (TCP/IP)

O ESP32 se conecta à rede Wi-Fi através do módulo integrado:

- Protocolo de camada 3: **IPv4**
- Protocolo de transporte: **TCP**
- Autenticação Wi-Fi: WPA2 (padrão)
- DHCP: utilizado para atribuir IP automaticamente

Após conectado, ele estabelece uma sessão segura com o broker MQTT.

---

## 3. Protocolo MQTT

### **Broker utilizado**
- **HiveMQ Cloud**
- Porta: **8883** (MQTT seguro – TLS)
- Autenticação: **username + password**
- Criptografia: **TLS 1.2**
- Certificado raiz incluído no firmware

### **Cliente MQTT no ESP32**
O ESP32 utiliza:
- `WiFiClientSecure` para a conexão TLS
- `PubSubClient` para publicar as mensagens

Configuração simplificada:

```cpp
mqttClient.setServer(MQTT_SERVER, 8883);
mqttClient.connect(clientId, MQTT_USER, MQTT_PASS);

