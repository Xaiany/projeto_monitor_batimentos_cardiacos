# Documentação do Software e Estrutura do Código

Este documento descreve o software embarcado no ESP32, suas funções, bibliotecas utilizadas e a lógica de funcionamento do sistema de monitoramento materno.

---

## Bibliotecas utilizadas

- **WiFi.h / WiFiClientSecure.h** – conexão Wi-Fi e TLS  
- **PubSubClient.h** – comunicação MQTT  
- **DHT.h** – leitura do sensor DHT22  
- **Adafruit_GFX.h / Adafruit_SSD1306.h** – controle do display OLED  
- **Wire.h** – barramento I2C  

---

## Estrutura geral do código

### 1. Declaração de constantes  
Inclui:
- Credenciais Wi-Fi  
- Endereço do broker MQTT  
- Pinos de sensores e atuadores  
- Limites de temperatura, umidade e batimentos  

---

### 2. Inicialização (`setup()`)

- Inicia Serial, I2C e periféricos  
- Inicia DHT22  
- Configura ADC para o potenciômetro  
- Exibe mensagens iniciais no display OLED  
- Configura TLS e conecta ao Wi-Fi e MQTT  

---

### 3. Loop principal (`loop()`)

A cada 3 segundos:
- Lê temperatura e umidade do DHT22  
- Lê o potenciômetro e converte para batimentos cardíacos  
- Determina o status (NORMAL, ATENÇÃO, CRÍTICO)  
- Atualiza LEDs e buzzer  
- Exibe os valores no display OLED  
- Publica telemetria em formato JSON via MQTT  

---

## Lógica de classificação do status

A classificação utiliza contadores de alerta:

- 0 parâmetros fora da faixa → **NORMAL**  
- 1 parâmetro fora da faixa → **ATENÇÃO**  
- 2 ou mais parâmetros fora da faixa → **CRÍTICO**

---

## Estrutura JSON publicada via MQTT

Exemplo:

```json
{
  "device": "gestacao_device_01",
  "temperature": 36.5,
  "humidity": 50.0,
  "heartrate": 90,
  "spo2": 0.0,
  "status": "NORMAL"
}
