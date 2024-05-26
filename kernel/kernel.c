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
unsigned int *monster_index;

void all_clear_fn();
void game_start_fn();
void game_init_fn();
int is_die_check(int current_character, int current_block, int timer);
void start_new_stage(int stage);
void pause_mode();

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
                    create_monster_array(monster_index, stage);
                }
                else if (stage == 2)
                {
                    showBackground(shiftY, stage);
                    create_block(block_array, stage);
                    create_monster_array(monster_index, stage);
                }
                else if (stage == 3)
                {
                    showBackground(shiftY, stage);
                    create_block(block_array, stage);
                    create_monster_array(monster_index, stage);
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
                    uart_puts("character moves to \"left\" side \n");
                }

                else if (c == 'd')
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
                    uart_puts("character moves to \"right\" side \n");
                }
                else if (c == ' ')
                {
                    uart_puts("\" space \" pressed \n");
                    pause_mode();
                    re_load_background(450, 300, 200, 50);
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
                    if (stage == 4)
                    {
                        all_clear_fn();
                    }
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
        timer = 25;
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

void all_clear_fn()
{
    all_clear();
    while (1)
    {
    }
}
void pause_mode()
{
    drawString(450, 300, "PAUSE", 0x00AA0000, 4);
    static char currentCommand[100];
    int charIndex = 0;
    while (1)
    {
        char c = uart_getc();
        if (c == ' ')
        {
            uart_sendc('\n');
            break;
        }

        else if (c != ' ' && c != '\n')
        {
            if (c == 127)
            {
                if (charIndex > 0)
                {
                    currentCommand[charIndex] = '\0';
                    uart_sendc('\b');
                    uart_sendc(' ');
                    uart_sendc('\b');
                    charIndex--;
                }
            }
            else
            {
                uart_sendc(c);
                currentCommand[charIndex] = c;
                charIndex++;
            }
        }
        else if (c == '\n')
        {
            uart_sendc(c);

            currentCommand[charIndex] = '\0';
            if (my_strncmp(currentCommand, "stage_?", 7))
            {
                uart_puts("current stage is ");
                uart_sendi(stage);
                uart_puts("\n");
            }
            else if (my_strncmp(currentCommand, "block_?", 7))
            {
                int num_of_block = phase * 11 + 1 - (12 - step);
                uart_puts("you went up ");
                uart_sendi(num_of_block);
                uart_puts(" blocks \n");
            }
            else if (my_strncmp(currentCommand, "character_?", 11))
            {
                uart_puts("character's current x coordinate : ");
                uart_sendi(current_w_index);
                uart_puts("\n");
                uart_puts("character's current y coordinate : ");
                uart_sendi(current_h_index);
                uart_puts("\n");
            }
            else if (my_strncmp(currentCommand, "stage=", 6))
            {
                char *destination_stage = my_strstr(currentCommand, "=");
                destination_stage += 1;

                if (my_strncmp(destination_stage, "1", 1) == 1)
                {
                    stage = 1;
                    stage_start_flag = 0;
                    is_load_flag = 0;
                    shiftY = -700;
                    step = 0;
                    current_h_index = 708 - 120;
                    gmae_over_flag = 0;
                    current_w_index = block_array[0];
                }
                else if (my_strncmp(destination_stage, "2", 1) == 1)
                {
                    stage = 2;
                    stage_start_flag = 0;
                    is_load_flag = 0;
                    shiftY = -700;
                    current_h_index = 708 - 120;
                    step = 0;
                    gmae_over_flag = 0;
                    current_w_index = block_array[0];
                }
                else if (my_strncmp(destination_stage, "3", 1) == 1)
                {
                    stage = 3;
                    stage_start_flag = 0;
                    is_load_flag = 0;
                    shiftY = -700;
                    current_h_index = 708 - 120;
                    gmae_over_flag = 0;
                    step = 0;
                    current_w_index = block_array[0];
                }
                else
                {
                    uart_puts("Available stages are only 1, 2, and 3 \n");
                }
            }
            else if (my_strncmp(currentCommand, "uart_config", 11))
            {
                uart_puts("current uart is uart 1 \n");
                uart_puts("baud rate = 115200 \n");
                uart_puts("data bit  = 8 \n");
                uart_puts("handshaking  = off \n");
            }
            else
            {
                uart_puts("\"");
                for (int i = 0; i < charIndex; i++)
                {
                    uart_sendc(currentCommand[i]);
                }

                uart_puts("\" is not supported command \n");
            }
            charIndex = 0;
        }
    }
}
