# Introdução às Interfaces de Comunicação Serial com RP2040

## Sumário

1. 🎯 Objetivos
2. 🛠️ Descrição do Projeto
3. 🚀 Funcionalidades do Projeto
   - 3.1 Modificação da Biblioteca font.h
   - 3.2 Entrada de Caracteres via PC
   - 3.3 Interação com os Botões
4. ✅ Requisitos do Projeto
5. 🖥️ Configuração do Ambiente
   - 5.1 Instalações Necessárias
6. 🗁 Estrutura do Código
7. 📸 Demonstração do Projeto

## 1. 🎯 Objetivos

- Compreender o funcionamento e a aplicação de comunicação serial em microcontroladores.
- Aplicar os conhecimentos adquiridos sobre UART e I2C na prática.
- Manipular e controlar LEDs comuns e LEDs endereçáveis WS2812.
- Fixar o estudo do uso de botões de acionamento, interrupções e debounce.
- Desenvolver um projeto funcional que combine hardware e software.

## 2. 🛠️ Descrição do Projeto

Este projeto foi desenvolvido utilizando a placa **BitDogLab** e os seguintes componentes:

- **Matriz 5x5 de LEDs WS2812** (endereçáveis) conectada à GPIO 7.
- **LED RGB**, com os pinos conectados às GPIOs 11, 12 e 13.
- **Botão A**, conectado à GPIO 5.
- **Botão B**, conectado à GPIO 6.
- **Display SSD1306**, conectado via I2C (GPIO 14 e GPIO 15).

## 3. 🚀 Funcionalidades do Projeto

### 3.1 Modificação da Biblioteca font.h
Foram adicionados caracteres minúsculos à biblioteca **font.h**, permitindo uma exibição mais completa no display.

### 3.2 Entrada de Caracteres via PC
- O **Serial Monitor** do VS Code foi utilizado para digitar caracteres.
- Cada caractere digitado foi exibido no **display SSD1306**.
- Se um número (0 a 9) foi digitado, um símbolo correspondente foi exibido na **matriz 5x5 WS2812**.

### 3.3 Interação com os Botões
- **Botão A**: Alterna o estado do LED RGB **Verde** (ligado/desligado). A operação é registrada no **display SSD1306** e no **Serial Monitor**.
- **Botão B**: Alterna o estado do LED RGB **Azul** (ligado/desligado). A operação é registrada no **display SSD1306** e no **Serial Monitor**.

## 4. ✅ Requisitos do Projeto

- Uso de **interrupções (IRQ)** para os botões.
- **Debouncing via software** para evitar detecções repetitivas.
- Controle de **LEDs comuns e WS2812**.
- Utilização do **Display SSD1306 (128x64)**, incluindo suporte a caracteres maiúsculos e minúsculos.
- Envio de informação via **UART**.
- Código bem estruturado e comentado.

## 5. 🖥️ Configuração do Ambiente

### 5.1 Instalações Necessárias
- **Pico SDK** instalado na máquina.
- **ARM GNU Toolchain** instalado.
- **Visual Studio Code** instalado com as extensões:
  - C/C++
  - CMake
  - CMake Tools
  - Raspberry Pi Pico
  - Wokwi Simulator
- **Software Putty** para comunicação.
- **Placa RP2040**.

## 6. 🗁 Estrutura do Código

O projeto está organizado da seguinte forma:

- **tarefa1-comunicacao-serial-03-02-uart-i2c-spi.c** - Arquivo principal contendo a lógica de comunicação via UART, SPI e I2C.
- **CMakeLists.txt** - Configuração para compilação com o Pico SDK.
- **wokwi.toml** - Arquivo de configuração do Wokwi Simulator.
- **diagram.json** - Arquivo de design e configuração do Wokwi Simulator.
- **README.md** - Documentação do projeto.

## 7. 📸 Demonstração do Projeto

Este projeto foi testado e simulado no **Wokwi do VS Code** e na **placa RP2040**, garantindo seu funcionamento adequado.

- **Repositório GitHub**: [Clique aqui](https://github.com/demyshow/tarefa1-comunicacao-serial-03-02-uart-i2c-spi)
- **Vídeo de Demonstração**: [Clique aqui](https://drive.google.com/file/d/11bGXugWPdoIw9DrY3rgLMf69zmTbu9Tq/view?usp=sharing)

Clone o repositório do projeto e compile o código:

```sh
 git clone https://github.com/demyshow/tarefa1-comunicacao-serial-03-02-uart-i2c-spi.git
 cd C:\tarefa1-comunicacao-serial-03-02-uart-i2c-spi
```

Compile e execute o código usando o **Pico SDK**.

Após clonar o projeto, ele deve ser importado no Raspberry Pi via o seguinte caminho:

```sh
C:\tarefa1-comunicacao-serial-03-02-uart-i2c-spi\tarefa1-comunicacao-serial-03-02-uart-i2c-spi
```

---

Este documento resume o projeto, apresentando seus objetivos, funcionamento e implementação, garantindo que qualquer interessado possa replicá-lo de maneira clara e objetiva.
