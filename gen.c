#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE_WORD 12
#define COUNT_WORDS 1e7

int main(int argc, char* argv[])
{
    if (argc > 1) {
        FILE* f = fopen(argv[1], "w");
        if (!f) return -fprintf(stderr, "Cannot open file %s\n", argv[1]);

        srand(time(NULL));

        for (int i = 0; i < COUNT_WORDS; i++) {
            char word[SIZE_WORD] = "";

            int size = SIZE_WORD - rand() % 2;
            for (int i = 0; i < size; i++) {
                int character = rand() % 52;

                if (character < 26) word[i] = 'a' + character;
                else word[i] = 'A' + character - 26;
            }

            fprintf(f, "%s ", word);

            if (i % ((int)COUNT_WORDS / 10) == 0) fprintf(stderr, "i = %d\n", i);
        }

    } else {
        return -fprintf(stderr, "Wrong argument %s\n", argv[1]);
    }

    return 0;
}