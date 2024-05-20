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
                showBackground(shiftY);
                loadBlock(0, 0);
                loadBlock(75, 39 + 20);
                loadBlock(150, 78 + 20);
                loadBlock(225, 117 + 40);
                loadBlock(300, 156 + 60);
                loadBlock(375, 195 + 80);
                loadBlock(450, 234 + 100);
                loadBlock(525, 273 + 120);
                loadBlock(600, 317 + 140);
                loadBlock(675, 351 + 170);
            }
            else if (stage == 2)
            {
                shiftY = shiftY + 50;
                showBackground2(shiftY);
            }
            else if (stage == 3)
            {
                shiftY = shiftY + 50;
                showBackground3(shiftY);
            }
        }
    }
}
