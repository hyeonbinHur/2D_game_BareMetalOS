#include "./utils.h"
// #include "./font.h"
#include "./framebf.h"

void startGame()
{
    drawString(330, 200, "Infinity stair", 0x00AA0000, 3);
    drawString(340, 400, "Press Enter to start", 0x0000BB00, 2);
}

void showBackground(int shiftY)
{
    int originalHeight = 937;                   // 상단 절반의 원래 높이
    int originalWidth = 173;                    // 원본 가로 크기
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
            unsigned int attr = epd_bitmap_stage1[index];
            drawPixelARGB32(x, y + yOffset, attr); // Y 오프셋을 적용하여 중앙에 이미지를 그림
        }
    }
}