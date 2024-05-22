#include "./utils.h"
// #include "./font.h"
#include "./framebf.h"
#include "../uart/uart1.h"

struct screenData screen[1024][768];

void startGame()
{
    drawString(330, 200, "Infinity stair", 0x00AA0000, 3);
    drawString(340, 400, "Press Enter to start", 0x0000BB00, 2);
}

void show_game_over_fn()
{
    int x = 1024;
    int y = 768;

    for (int i = 0; i < y; i++)
    {
        wait_msec(1);
        for (int j = 0; j < x; j++)
        {
            drawPixelARGB32(j, i, 0x000000);
        }
    }
    drawString(390, 200, "Game Over", 0x00AA0000, 3);
    drawString(340, 400, "Press r to restart the game", 0x0000BB00, 2);

    while (1)
    {
        unsigned char c = uart_getc();
        if (c == 'r')
        {
            break;
        }
    }
}

void showBackground(int shiftY, int stage)
{
    int originalHeight = 948;                   // 상단 절반의 원래 높이
    int originalWidth = 177;                    // 원본 가로 크기
    float scaleX = 1024.0 / originalWidth;      // 가로 스케일 비율
    int scaledWidth = 1024;                     // 스케일된 가로 크기, 화면을 꽉 채움
    int scaledHeight = originalHeight * scaleX; // 스케일된 세로 크기

    int yOffset = (768 - scaledHeight) / 2; // 세로 중앙 정렬을 위한 Y 오프셋
    // int shiftY = -350;                      // 아래로 이동시킬 픽셀 수
    for (int y = 0; y < scaledHeight; y++)
    {
        int srcY = y / scaleX - shiftY; // 원본 이미지에서의 Y 인덱스 조정
        if (srcY < 0)
            srcY = 0; // 넘어가지 않도록 경계 처리
        if (srcY >= originalHeight)
            srcY = originalHeight - 1; // 넘어가지 않도록 경계 처리

        for (int x = 0; x < scaledWidth; x++)
        {
            int srcX = x / scaleX;                     // 원본 이미지에서의 X 인덱스
            int index = (srcY * originalWidth + srcX); // 원본 이미지의 RGB888 데이터 인덱스
            unsigned int attr;
            if (stage == 1)
            {
                attr = epd_bitmap_stage1[index];
            }
            else if (stage == 2)
            {
                attr = epd_bitmap_stage2[index];
            }
            else if (stage == 3)
            {
                attr = epd_bitmap_stage3[index];
            }
            screen[x][y].prev_value = attr;
            drawPixelARGB32(x, y + yOffset, attr); // Y 오프셋을 적용하여 중앙에 이미지를 그림
        }
    }
}

void loadBlock(int start_x, int start_y, int stage)
{
    int img_w = 75;
    int img_h = 40;

    for (int y = start_y; y < start_y + img_h; y++)
    {

        for (int x = start_x; x < start_x + img_w; x++)
        {
            unsigned int attr;
            if (stage == 1)
            {
                attr = epd_bitmap_stage_1_block[(y - start_y) * img_w + (x - start_x)];
            }
            else if (stage == 2)
            {
                attr = epd_bitmap_stage_2_block[(y - start_y) * img_w + (x - start_x)];
            }
            else if (stage == 3)
            {
                attr = epd_bitmap_stage_3_block[(y - start_y) * img_w + (x - start_x)];
            }
            drawPixelARGB32(x, y, attr); // Y 오프셋을 적용하여 중앙에 이미지를 그림
        }
    }
}

void load_character(int start_w, int start_h, int direction)
{
    int character_w = 70;
    int character_h = 120;
    if (direction == 1)
    {
        re_load_background(start_w - 75, start_h + 57, character_w, character_h);
    }
    else if (direction == 0)
    {
        re_load_background(start_w + 75, start_h + 57, character_w, character_h);
    }
    for (int h = start_h; h < start_h + character_h; h++)
    {
        for (int w = start_w; w < start_w + character_w; w++)
        {
            unsigned int attr;
            if (direction == 1)
            {
                attr = epd_bitmap_right_stand[(h - start_h) * character_w + (w - start_w)];
            }
            else if (direction == 0)
            {
                attr = epd_bitmap_left_stand[(h - start_h) * character_w + (w - start_w)];
            }
            if (attr != 0x00000000)
            {
                drawPixelARGB32(w, h, attr);
            }
        }
    }
}

void show_die_character_fn(int start_w, int start_h, int direction)
{
    int character_w = 110;
    int character_h = 69;
    for (int h = start_h; h < start_h + character_h; h++)
    {
        for (int w = start_w; w < start_w + character_w; w++)
        {
            unsigned int attr;
            if (direction == 1)
            {
                attr = epd_bitmap_right_die[(h - start_h) * character_w + (w - start_w)];
            }
            else if (direction == 0)
            {
                attr = epd_bitmap_left_die[(h - start_h) * character_w + (w - start_w)];
            }
            if (attr != 0x00000000)
            {
                drawPixelARGB32(w, h, attr);
            }
        }
    }
}

unsigned int *create_block_array(unsigned int current_block)
{
    static unsigned int block_array[13];
    block_array[0] = current_block;
    for (int i = 1; i < 13; i++)
    {
        int temp = block_array[i - 1];
        if (temp == 24)
        {

            block_array[i] = 99;
        }
        else if (temp == 999)
        {
            block_array[i] = 924;
        }
        else
        {
            int n = generateRandomBit();
            if (n == 1)
            {
                block_array[i] = temp + 75;
            }
            else if (n == 0)
            {
                block_array[i] = temp - 75;
            }
        }
    }
    return block_array;
}

void create_block(unsigned int *block_array)
{
    int h = 708;
    for (int i = 0; i < 13; i++)
    {
        loadBlock(block_array[i], h, 1);
        h -= 57;
    }
}

void *show_timer(unsigned int curret_time)
{
    char str[3];
    int i = 0;

    if (curret_time < 10)
    {
        str[i++] = '0';
    }

    if (curret_time == 0)
    {
        str[i++] = '0';
    }
    else
    {
        int temp = curret_time;
        int digit_count = 0;

        while (temp > 0)
        {
            temp /= 10;
            digit_count++;
        }

        for (int j = digit_count - 1; j >= 0; j--)
        {
            str[i + j] = (curret_time % 10) + '0';
            curret_time /= 10;
        }
        i += digit_count;
    }

    str[i] = '\0';

    drawString(20, 20, str, 0x00AA0000, 3);
}

void re_load_background(unsigned int start_w, unsigned int start_h, int img_w, int img_h)
{

    for (int w = start_w; w < start_w + img_w; w++)
    {
        for (int h = start_h; h < start_h + img_h; h++)
        {
            unsigned int attr = screen[w][h].prev_value;
            drawPixelARGB32(w, h, attr);
        }
    }
}