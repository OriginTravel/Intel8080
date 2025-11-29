#include <stdio.h>
#include <stdint.h>

int main(){
    uint8_t code[] = {
        0x21, 1, 2
    };
    FILE* file = fopen("./build/test", "w");
    fwrite(code, 1, sizeof(code), file);
    fclose(file);
    return 0;
}