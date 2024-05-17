unsigned int seed = 0x12345678; // Choose any non-zero value

int generateRandomBit() {
    // LCG parameters
    unsigned int a = 1103515245;
    unsigned int c = 12345;
    unsigned int m = (1u << 31); // 2^31

    // Update seed using the LCG formula
    seed = (a * seed + c) % m;

    // Extract the least significant bit as the random bit
    return seed & 1;
}