void drawChar(unsigned char ch, int x, int y, unsigned int attr, int zoom);
void drawString(int x, int y, char *str, unsigned int attr, int zoom);
unsigned char getUart();
void wait_msec(unsigned int msVal);
void set_wait_timer(int set, unsigned int msVal);
void uart_sendi(int num);
int my_strncmp(const char *command, const char *compare, int index);
char *my_strstr(const char *haystack, const char *needle);

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
extern const unsigned long epd_bitmap_fire_right[];
extern const unsigned long epd_bitmap_fire_left[];
extern const unsigned long epd_bitmap_stage3_monster[];
extern const unsigned long epd_bitmap_stage2_monster[];
extern const unsigned long epd_bitmap_stage1_monster[];
extern const unsigned long epd_bitmap_stage3_ball[];
extern const unsigned long epd_bitmap_stage2_ball[];
extern const unsigned long epd_bitmap_stage1_ball[] extern