#include "mbox.h"
#include "../uart/uart0.h"
#include "../uart/uart1.h"
#include "../lib/framebf.h"
#include "../lib/utils.h"
#include "../lib/game.h"

// int w_index = [ 24, 99, 174, 249, 324, 399, 474, 549, 624, 699, 774, 849, 924 ];
// int h_index = [ 0, 57, 114, 171, 228, 285, 342, 399, 456, 513, 570, 627, 684 ];

unsigned int first_block = 399; // first block in each stage
unsigned int *block_array;      // the random block array
unsigned int game_start = 0;    // if game is started, change to 1

unsigned int current_w_index = 399;       // character's start w index
unsigned int current_h_index = 708 - 120; // character's start h index

unsigned int step = 0;       // if it reach 12, background is changed
unsigned int die_flag = 0;   // if user die, change to 1
unsigned int ms_counter = 0; // check the time

unsigned int stage_1_timer = 10;
unsigned int stage_2_timer = 35;
unsigned int stage_3_timer = 30;
unsigned int phase = 7;

void game_start_fn();
int is_die_check(int current_character, int current_block, int timer);

void main()
{
    // set up serial console
    uart_init();

    // Initialize frame buffer
    framebf_init();

    startGame();
    block_array = create_block_array(first_block);

    int shiftY = -350;
    int stage = 1;

    while (1)
    {
        if (game_start == 0)
        {
            game_start = 1;
            game_start_fn();
        }

        else if (game_start == 1)
        {

            wait_msec(300);
            char c = uart_getc();
            ms_counter++;
            if (ms_counter == 3)
            {
                // check time about 1 second
                ms_counter = 0;
                stage_1_timer -= 1;
            }
            show_timer(stage_1_timer);

            // move logic
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

            // die logic
            if (step != 0)
            {
                die_flag = is_die_check(current_w_index, block_array[step], stage_1_timer);
            }

            if (die_flag == 0) // not die
            {
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
            else if (die_flag == 1) // die
            {
                show_game_over_fn();
            }
        }
    }
}

int is_die_check(int current_character, int current_block, int timer)
{
    if (timer == 0)
    {
        return 1;
    }
    else
    {
        if (current_block == current_character)
        {
            return 0;
        }
        else
        {
            return 1;
        }
    }
}

void game_start_fn()
{
    startGame();

    while (1)
    {
        char c = uart_getc();
        if (c == '\n')
        {
            break;
        }
    }
}
