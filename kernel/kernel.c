#include "mbox.h"
#include "../uart/uart0.h"
#include "../uart/uart1.h"
#include "../lib/framebf.h"
#include "../lib/utils.h"
#include "../lib/game.h"

void main()
{
    // set up serial console
    uart_init();
    // say hello
    uart_puts("\n\nHello World!\n");
    uart_puts("\n\nHello World!\n");
    uart_puts("\n\nHello World!\n");

    // Initialize frame buffer
    framebf_init();
    // // Draw something on the screen
    // drawRectARGB32(100, 100, 400, 400, 0x00AA0000, 1); // RED
    // drawRectARGB32(150, 150, 400, 400, 0x0000BB00, 1); // GREEN
    // drawRectARGB32(200, 200, 400, 400, 0x000000CC, 1); // BLUE
    // drawRectARGB32(250, 250, 400, 400, 0x00FFFF00, 1); // YELLOW
    // drawPixelARGB32(300, 300, 0x00FF0000);             // RED

    // drawChar("A", 200, 400, 0x00AA0000, 3);
    startGame();

    int shiftY = -350;
    int stage = 1;

    while (1)
    {
        // read each char
        char c = uart_getc();
        // send back
        // uart_sendc(c);
        if (shiftY > 300)
        {
            shiftY = -350;
            stage++;
        }

        if (c == 'w')
        {
            if (stage == 1)
            {
                shiftY = shiftY + 50;

                showBackground(shiftY, stage);
                loadBlock(10, 10, stage);
                loadBlock(75 + 15, 58 + 10, stage);
                loadBlock(150 + 15, 116 + 10, stage);
                loadBlock(225 + 15, 174 + 10, stage);
                loadBlock(300 + 15, 232 + 10, stage);
                loadBlock(375 + 15, 290 + 10, stage);
                loadBlock(450 + 15, 348 + 10, stage);
                loadBlock(525 + 15, 406 + 10, stage);
                loadBlock(600 + 15, 464 + 10, stage);
                loadBlock(675 + 15, 522 + 10, stage);
                loadBlock(750 + 15, 580 + 10, stage);
                loadBlock(825 + 15, 638 + 10, stage);
                loadBlock(900 + 15, 696 + 10, stage);
            }
            else if (stage == 2)
            {
                shiftY = shiftY + 50;
                showBackground(shiftY, stage);
            }
            else if (stage == 3)
            {
                shiftY = shiftY + 50;
                showBackground(shiftY, stage);
            }
        }
    }
}
