void drawChar(unsigned char ch, int x, int y, unsigned int attr, int zoom);
void drawString(int x, int y, char *str, unsigned int attr, int zoom);
unsigned char getUart();
void wait_msec(unsigned int msVal);
void set_wait_timer(int set, unsigned int msVal);

extern const unsigned long epd_bitmap_stage1[];
extern const unsigned long epd_bitmap_stage2[];
extern const unsigned long epd_bitmap_stage3[];
extern const unsigned long epd_bitmap_stage_1_block[];
extern const unsigned long epd_bitmap_stage_2_block[];
extern const unsigned long epd_bitmap_stage_3_block[];
extern const unsigned long epd_bitmap_right_stand[];
extern const unsigned long epd_bitmap_left_stand[];
extern const unsigned long epd_bitmap_left_jump[];
extern const unsigned long epd_bitmap_right_jump[];
extern const unsigned long epd_bitmap_right_die[];
extern const unsigned long epd_bitmap_left_die[];