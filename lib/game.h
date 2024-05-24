void startGame();
void showBackground(int shiftY, int stage);
void loadBlock(int start_x, int start_y, int stage);
unsigned int *create_block_array(unsigned int current_block);
void create_block(unsigned int *block_array);
void load_character(int start_w, int start_h, int direction);
void *show_timer(unsigned int curret_time);
void show_game_over_fn();
void show_die_character_fn(int start_w, int start_h, int direction);

struct screenData
{
    unsigned int current_value;
    unsigned int prev_value;
};

extern struct screenData screen[1024][768];
