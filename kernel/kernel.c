#include "mbox.h"
#include "../uart/uart0.h"
#include "../uart/uart1.h"
#include "../lib/framebf.h"
#include "../lib/utils.h"
#include "../lib/game.h"

unsigned int first_block;  // first block in each stage
unsigned int *block_array; // the random block array
unsigned int game_start;   // if game is started, change to 1

unsigned int current_w_index; // character's start w index
unsigned int current_h_index; // character's start h index

unsigned int step;           // if it reach 12, background is changed
unsigned int gmae_over_flag; // if user die, change to 1
unsigned int ms_counter;     // check the time
unsigned int is_jump;        // is the character jumping? 0 = no jumping, 1 = is jumping

unsigned int timer;

unsigned int phase;
unsigned int stage_start_flag; // stage start flag

unsigned int is_load_flag;
int shiftY;
int stage;
int direction; // 1 is right, 0 is left

void game_start_fn();
void game_init_fn();
int is_die_check(int current_character, int current_block, int timer);
void start_new_stage(int stage);

void main()
{
    // set up serial console
    uart_init();

    // Initialize frame buffer
    framebf_init();

    unsigned char c;

    while (1)
    {
        if (game_start == 1)
        {
            // load items logic

            if (stage_start_flag == 0)
            {
                show_stage_clear(stage);
                stage_start_flag = 1;
                start_new_stage(stage);
            }

            if (step == 0 && is_load_flag == 0 && gmae_over_flag == 0)
            {
                is_load_flag = 1;
                if (stage == 1)
                {
                    showBackground(shiftY, stage);
                    create_block(block_array, stage);
                }
                else if (stage == 2)
                {
                    showBackground(shiftY, stage);
                    create_block(block_array, stage);
                }
                else if (stage == 3)
                {
                    showBackground(shiftY, stage);
                    create_block(block_array, stage);
                }

                show_phase(phase);
            }
            if (is_jump == 0 && gmae_over_flag == 0)
            {
                load_character(current_w_index, current_h_index, direction);
            }

            set_wait_timer(1, 10);
            c = getUart();
            set_wait_timer(0, 0);
            ms_counter++;
            show_timer(timer);

            // move logic
            if (is_jump == 1)
            {
                is_jump = 0;
                if (direction == 1)
                {
                    current_w_index += 38;
                    current_h_index -= 17;
                }
                else if (direction == 0)
                {
                    current_w_index -= 38;
                    current_h_index -= 17;
                }

                show_jump(current_w_index, current_h_index, direction);
                wait_msec(100);

                re_load_background(current_w_index, current_h_index, 70, 130);
            }

            else if (is_jump == 0)
            {
                if (c == 'a')
                {
                    step += 1;
                    // current_w_index -= 75;
                    // current_h_index -= 57;
                    re_load_background(current_w_index, current_h_index, 70, 120);
                    current_w_index -= 37;
                    current_h_index -= 40;
                    direction = 0;
                    is_jump = 1;
                    // re load at here
                }

                if (c == 'd')
                {
                    step += 1;
                    // current_w_index += 75;
                    // current_h_index -= 57;
                    re_load_background(current_w_index, current_h_index, 70, 120);
                    current_w_index += 37;
                    current_h_index -= 40;
                    direction = 1;
                    is_jump = 1;
                    // re load at here
                }
            }

            if (step == 12 && is_jump == 0)
            {
                is_load_flag = 0;
                step = 0;
                phase++;
                shiftY += 100;
                current_h_index = 708 - 120;
                block_array = create_block_array(block_array[12]);
                // load_background_with_transition(block_array, stage, shiftY, current_w_index, direction);
            }

            // die logic
            if (step != 0)
            {
                if (is_jump == 1)
                {
                    if (direction == 0)
                    {
                        gmae_over_flag = is_die_check(current_w_index - 38, block_array[step], timer);
                    }
                    else if (direction == 1)
                    {
                        gmae_over_flag = is_die_check(current_w_index + 38, block_array[step], timer);
                    }
                }
                else
                {
                    if (direction == 0)
                    {
                        gmae_over_flag = is_die_check(current_w_index, block_array[step], timer);
                    }
                    else if (direction == 1)
                    {
                        gmae_over_flag = is_die_check(current_w_index, block_array[step], timer);
                    }
                }
            }

            if (gmae_over_flag == 0)
            {
                if (ms_counter == 100)
                {
                    ms_counter = 0;
                    timer -= 1;
                }
                // move logic
                if (shiftY == 0)
                {
                    is_load_flag = 0;
                    shiftY = -700;
                    stage++;
                    stage_start_flag = 0;
                }
            }
            else if (gmae_over_flag == 1) // game over
            {

                show_die_character_fn(current_w_index, current_h_index, direction, is_jump);

                wait_msec(600);
                show_game_over_fn();
                gmae_over_flag = 0;
                game_start = 0;
            }
        }

        else if (game_start == 0)
        {
            game_init_fn();
            game_start = 1;
            game_start_fn();
            block_array = create_block_array(first_block);
        }
    }
}

int is_die_check(int current_character, int current_block, int timer)
{
    if (timer == 0)
    {
        return 1; // 1
    }
    else
    {
        if (current_block == current_character)
        {
            return 0;
        }
        else
        {
            return 1; // 1
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

void game_init_fn()
{
    first_block = 399;
    current_w_index = 399;
    current_h_index = 588;
    step = 0;
    gmae_over_flag = 0;
    ms_counter = 0;
    timer = 25;
    phase = 1;
    shiftY = -700;
    stage = 1;
    direction = 1;
    is_load_flag = 0;
    is_jump = 0;
    stage_start_flag = 0;
    int x = 1024;
    int y = 768;

    for (int i = 0; i < y; i++)
    {
        for (int j = 0; j < x; j++)
        {
            drawPixelARGB32(j, i, 0x000000);
        }
    }
}

void start_new_stage(int stage)
{
    if (stage == 1)
    {
        timer = 5;
        phase = 1;
    }
    else if (stage == 2)
    {
        timer = 20;
        phase = 1;
    }
    else if (stage == 3)
    {
        timer = 15;
        phase = 1;
    }
}
