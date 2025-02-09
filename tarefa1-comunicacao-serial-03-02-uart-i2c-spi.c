#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/clocks.h"
#include "hardware/pio.h"
#include "pio_led.pio.h"
#include "hardware/timer.h"
#include "lib/font.h"
#include "lib/ssd1306.h"
#include "hardware/uart.h"

const uint OUT_PIN = 7, PIN_BTN_A = 5, PIN_BTN_B = 6, PIN_LED_B = 12, PIN_LED_G = 11, I2C_SCL = 15, I2C_SDA = 14, UART_TX = 0, UART_RX = 1;
const uint MATRIX_DIMENSION = 5;
#define I2C_PORT i2c1
#define endereco 0x3C

typedef unsigned char uchar;

uint32_t LAST_EVENT_A = 0, LAST_EVENT_B = 0;

const float draws[10][5][5] = {
    {{0.1, 0.1, 0.1, 0.1, 0.1},
    {0.1, 0.0, 0.0, 0.0, 0.1},
    {0.1, 0.0, 0.0, 0.0, 0.1},
    {0.1, 0.0, 0.0, 0.0, 0.1},
    {0.1, 0.1, 0.1, 0.1, 0.1}},

   {{0.0, 0.0, 0.1, 0.0, 0.0},
    {0.0, 0.1, 0.1, 0.0, 0.0},
    {0.1, 0.0, 0.1, 0.0, 0.0},
    {0.0, 0.0, 0.1, 0.0, 0.0},
    {0.0, 0.1, 0.1, 0.1, 0.0}},

   {{0.1, 0.1, 0.1, 0.1, 0.1},
    {0.0, 0.0, 0.0, 0.0, 0.1},
    {0.1, 0.1, 0.1, 0.1, 0.1},
    {0.1, 0.0, 0.0, 0.0, 0.0},
    {0.1, 0.1, 0.1, 0.1, 0.1}},

   {{0.1, 0.1, 0.1, 0.1, 0.1},
    {0.0, 0.0, 0.0, 0.0, 0.1},
    {0.1, 0.1, 0.1, 0.1, 0.1},
    {0.0, 0.0, 0.0, 0.0, 0.1},
    {0.1, 0.1, 0.1, 0.1, 0.1}},

   {{0.1, 0.0, 0.0, 0.0, 0.1},
    {0.1, 0.0, 0.0, 0.0, 0.1},
    {0.1, 0.1, 0.1, 0.1, 0.1},
    {0.0, 0.0, 0.0, 0.0, 0.1},
    {0.0, 0.0, 0.0, 0.0, 0.1}},

   {{0.1, 0.1, 0.1, 0.1, 0.1},
    {0.1, 0.0, 0.0, 0.0, 0.0},
    {0.1, 0.1, 0.1, 0.1, 0.1},
    {0.0, 0.0, 0.0, 0.0, 0.1},
    {0.1, 0.1, 0.1, 0.1, 0.1}},

   {{0.1, 0.1, 0.1, 0.1, 0.1},
    {0.1, 0.0, 0.0, 0.0, 0.0},
    {0.1, 0.1, 0.1, 0.1, 0.1},
    {0.1, 0.0, 0.0, 0.0, 0.1},
    {0.1, 0.1, 0.1, 0.1, 0.1}},

   {{0.1, 0.1, 0.1, 0.1, 0.1},
    {0.0, 0.0, 0.0, 0.1, 0.0},
    {0.0, 0.0, 0.1, 0.0, 0.0},
    {0.0, 0.1, 0.0, 0.0, 0.0},
    {0.1, 0.0, 0.0, 0.0, 0.0}},

   {{0.1, 0.1, 0.1, 0.1, 0.1},
    {0.1, 0.0, 0.0, 0.0, 0.1},
    {0.1, 0.1, 0.1, 0.1, 0.1},
    {0.1, 0.0, 0.0, 0.0, 0.1},
    {0.1, 0.1, 0.1, 0.1, 0.1}},

   {{0.1, 0.1, 0.1, 0.1, 0.1},
    {0.1, 0.0, 0.0, 0.0, 0.1},
    {0.1, 0.1, 0.1, 0.1, 0.1},
    {0.0, 0.0, 0.0, 0.0, 0.1},
    {0.0, 0.0, 0.0, 0.0, 0.1}},
};

PIO pio;
uint offset;
uint sm;
ssd1306_t ssd;

uint32_t rgbColor(float r, float g, float b)
{
    uchar R = (r > 1 || r < 0 ? 0.1 : r) * 255;
    uchar G = (g > 1 || g < 0 ? 0.1 : g) * 255;
    uchar B = (b > 1 || b < 0 ? 0.1 : b) * 255;

    return (R << 24) | (G << 16) | (B << 8);
}

void showNumberDraw(PIO *pio, uint *sm, int drawIndex);
void gpio_irq_handler(uint gpio, uint32_t events);
void setup_project();

int main()
{
    stdio_init_all();
    setup_project();

    while (true)
    {
        if (stdio_usb_connected())
        {
            char c = '\0';
            if (scanf("%c", &c) == 1)
            {
                printf("Tecla pressionada: %c\n", c); // Exibe a tecla pressionada no terminal
                if (c >= '0' && c <= '9')
                {
                    showNumberDraw(&pio, &sm, c - '0');
                }
            }
            char stringMsg[] = "Caractere Digitado  \0";
            stringMsg[(sizeof(stringMsg) / sizeof(char)) - 2] = c;
            ssd1306_draw_string(&ssd, stringMsg, 1, 1);
            ssd1306_send_data(&ssd);
        }
    }
    sleep_ms(50);
    return 0;
}

void showNumberDraw(PIO *pio, uint *sm, int drawIndex)
{
    if (drawIndex == -1)
        return;
    for (int j = 0; j < MATRIX_DIMENSION; j++)
    {
        for (int k = 0; k < MATRIX_DIMENSION; k++)
        {
            pio_sm_put_blocking(*pio, *sm, rgbColor(draws[drawIndex][MATRIX_DIMENSION - 1 - j][(j + 1) % 2 == 0 ? k : MATRIX_DIMENSION - k - 1], 0.0, 0.0));
        }
    }
}

void gpio_irq_handler(uint gpio, uint32_t events)
{
    uint32_t current_time = to_us_since_boot(get_absolute_time());

    if (gpio == PIN_BTN_A)
    {
        if (current_time - LAST_EVENT_A > 200000)
        {
            LAST_EVENT_A = current_time;
            gpio_put(PIN_LED_G, !gpio_get(PIN_LED_G));
            ssd1306_draw_string(&ssd, gpio_get(PIN_LED_G) ? "Led VERDE ligado\0" : "Led VERDE desligado\0", 1, 24);
            ssd1306_send_data(&ssd);
        }
    }
    else if (gpio == PIN_BTN_B)
    {
        if (current_time - LAST_EVENT_B > 200000)
        {
            LAST_EVENT_B = current_time;
            gpio_put(PIN_LED_B, !gpio_get(PIN_LED_B));
            ssd1306_draw_string(&ssd, gpio_get(PIN_LED_B) ? "Led AZUL ligado\0" : "Led AZUL desligado\0", 1, 40);
            ssd1306_send_data(&ssd);
        }
    }
}

void setup_project()
{
    pio = pio0;
    set_sys_clock_khz(125000, false);

    gpio_init(PIN_BTN_A);
    gpio_init(PIN_BTN_B);
    gpio_init(PIN_LED_G);
    gpio_init(PIN_LED_B);

    gpio_set_dir(PIN_BTN_A, GPIO_IN);
    gpio_set_dir(PIN_BTN_B, GPIO_IN);
    gpio_set_dir(PIN_LED_G, GPIO_OUT);
    gpio_set_dir(PIN_LED_B, GPIO_OUT);

    gpio_pull_up(PIN_BTN_A);
    gpio_pull_up(PIN_BTN_B);

    offset = pio_add_program(pio, &pio_led_program);
    sm = pio_claim_unused_sm(pio, true);
    pio_led_program_init(pio, sm, offset, OUT_PIN);

    gpio_set_irq_enabled_with_callback(PIN_BTN_A, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(PIN_BTN_B, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT);
    ssd1306_config(&ssd);
    ssd1306_send_data(&ssd);

    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);

    uart_init(uart0, 115200);
    gpio_set_function(UART_RX, GPIO_FUNC_UART);
    gpio_set_function(UART_TX, GPIO_FUNC_UART);
    uart_set_fifo_enabled(uart0, true);

    for (int k = 0; k < 25; k++)
    {
        pio_sm_put_blocking(pio, sm, 0.0);
    }
}