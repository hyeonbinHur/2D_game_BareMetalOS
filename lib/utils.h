void drawChar(unsigned char ch, int x, int y, unsigned int attr, int zoom);
void drawString(int x, int y, char *str, unsigned int attr, int zoom);
unsigned char getUart();
void wait_msec(unsigned int msVal);
void set_wait_timer(int set, unsigned int msVal);

extern const unsigned long epd_bitmap_stage1[];
