#include "mbox.h"
#include "../uart/uart0.h"
#include "../uart/uart1.h"
#include "../lib/framebf.h"
#include "../lib/utils.h"
#include "../lib/game.h"

// int w_index = [ 24, 99, 174, 249, 324, 399, 474, 549, 624, 699, 774, 849, 924 ];
// int h_index = [ 0, 57, 114, 171, 228, 285, 342, 399, 456, 513, 570, 627, 684 ];

unsigned int first_block = 399;           // first block in each stage
unsigned int *block_array;                // the random block array
unsigned int game_start = 0;              // if game is started, change to 1
unsigned int current_w_index = 399;       // start w index
unsigned int current_h_index = 708 - 120; // start h index
unsigned int step = 0;                    // if it reach 12, background is changed
unsigned int isDie = 0;                   // if user die, change to 1
unsigned int ms_counter = 0;              // check the time
unsigned int stage_1_timer = 40;
unsigned int stage_2_timer = 35;
unsigned int stage_3_timer = 30;
unsigned int phase = 7;

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

    startGame();
    block_array = create_block_array(first_block);

    int shiftY = -350;
    int stage = 1;

    while (1)
    {
        // read each char
        wait_msec(300);
        char c = uart_getc();
        ms_counter++;
        if (ms_counter == 3)
        {
            ms_counter = 0;
            stage_1_timer -= 1;
        }

        // send back
        // uart_sendc(c);

        if (c == '\n')
        {
            game_start = 1;
        }

        show_timer(stage_1_timer);

        if (game_start == 1)
        {

            if (c == 'a')
            {
                step += 1;
                current_w_index -= 75;
                current_h_index -= 57;
            }
            if (c == 'd')
            {
                step += 1;
                current_w_index += 75;
                current_h_index -= 57;
            }

            if (step == 12)
            {
                step = 0;
                shiftY += 100;
                current_h_index = 708 - 120;
                block_array = create_block_array(block_array[12]);
            }

            if (shiftY > 450)
            {
                shiftY = -350;
                stage++;
            }

            if (stage == 1)
            {

                if (step == 0)
                {
                    showBackground(shiftY, stage);
                    create_block(block_array);
                }

                load_character(current_w_index, current_h_index);
            }

            else if (stage == 2)
            {
                // shiftY = shiftY + 50;
                showBackground(shiftY, stage);
            }
            else if (stage == 3)
            {
                // shiftY = shiftY + 50;
                showBackground(shiftY, stage);
            }
        }
    }
}
