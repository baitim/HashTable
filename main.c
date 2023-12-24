#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "hash.h"

#define MAX_BUF_SIZE 1024
#define MAX_COUNT_DISTRIBUTION 15
#define MAX_COUNT_WORDS (5 * (int)1e6)

#define STRING_HASH_FUNC string_hash_3

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

int distribution[MAX_COUNT_DISTRIBUTION] = {};

size_t string_hash_polin(const void *str)
{
    const char *c = str;
    size_t h = 0;
    const int base = 31;
    const int mod = 1e9+7; 
    while (*c) {
        unsigned char x = *c;
        h = ((h * base) % mod + x + 1) % mod;
        c++;
    }
    return h;
}

size_t string_hash_2(const void *str)
{
    const char *c = str;
    size_t h = 0x1;
    while (*c) {
        unsigned char x = *c;
        h += x;
        h = (h << 37) ^ (h >> 17);
        h = (h << 31) ^ (h >> 19);
        c++;
    }
    return h;
}

size_t string_hash_3(const void *str)
{
    const char *c = str;
    size_t h = 0xa768edfbcde34529;
    while (*c) {
        unsigned char x = *c;
        h += (x << 17) ^ (x >> 11);
        h = (h << 13) ^ (h >> 7);
        c++;
    }
    return h;
}

void string_free(void* a) {
	free(a);
}
void* string_copy(const void* a) {
	return strdup(a);
}

int string_eq(const void* a, const void* b) {
	return !strcmp(a, b);
}

int my_print(const void* a, size_t i, size_t k, void* data) {
    distribution[MIN(k, MAX_COUNT_DISTRIBUTION - 1)]++;
    // fprintf(stderr, "Element: %s\n", (const char*) a);
	return 0;
}

void ht_count_test(FILE* input_file, FILE* output_file, int count)
{
    char buf[MAX_BUF_SIZE];
    HTable* ht =  ht_init(MAX_BUF_SIZE, STRING_HASH_FUNC,
            string_copy,
            string_eq,
            string_free);

    int count_words = 0;

    clock_t start;
    clock_t end;
    double sum = 0;
    while (fscanf(input_file, "%s", buf) == 1 && count_words < count) {
        start = clock();
        ht_set(ht, buf, 0.6);
        end = clock();
        sum = (double)(end - start);
        count_words++;
    }
    start = clock();
    ht_walk(ht, my_print, NULL);
    end = clock();
    sum = (double)(end - start);

    ht_destroy(ht);

    double seconds = sum / CLOCKS_PER_SEC;
    printf("Execution time: %f\n", seconds);

    fprintf(output_file, "%d %lf\n", count_words, seconds);
    
    if (count >= MAX_COUNT_WORDS / 10) {
        for (int i = 0; i < MAX_COUNT_DISTRIBUTION; i++)
            fprintf(stderr, "%d %d\n", i+1, distribution[i]);
    }
}

void ht_fill_test(FILE* input_file, FILE* output_file, double fill_factor, int count)
{
    char buf[MAX_BUF_SIZE];
    HTable* ht =  ht_init(MAX_BUF_SIZE, STRING_HASH_FUNC,
            string_copy,
            string_eq,
            string_free);

    int count_words = 0;

    clock_t start;
    clock_t end;
    double sum = 0;
    while (fscanf(input_file, "%s", buf) == 1 && count_words < count) {
        start = clock();
        ht_set(ht, buf, fill_factor);
        end = clock();
        sum = (double)(end - start);
        count_words++;
    }
    start = clock();
    ht_walk(ht, my_print, NULL);
    end = clock();
    sum = (double)(end - start);

    ht_destroy(ht);

    double seconds = sum / CLOCKS_PER_SEC;
    printf("Execution time: %f\n", seconds);

    fprintf(output_file, "%lf %lf\n", fill_factor, seconds);
    
    // if (count >= MAX_COUNT_WORDS / 10) {
    //     for (int i = 0; i < MAX_COUNT_DISTRIBUTION; i++)
    //         fprintf(stderr, "%d\t%d\n", i+1, distribution[i]);
    // }
}

void ht_dist_test(FILE* input_file, FILE* output_file)
{
    char buf[MAX_BUF_SIZE];
    HTable* ht =  ht_init(MAX_BUF_SIZE, STRING_HASH_FUNC,
            string_copy,
            string_eq,
            string_free);

    const int count = MAX_COUNT_WORDS;
    int count_words = 0;

    clock_t start;
    clock_t end;
    double sum = 0;
    while (fscanf(input_file, "%s", buf) == 1 && count_words < count) {
        start = clock();
        ht_set(ht, buf, 0.6);
        end = clock();
        sum = (double)(end - start);
        count_words++;
    }
    start = clock();
    ht_walk(ht, my_print, NULL);
    end = clock();
    sum = (double)(end - start);

    ht_destroy(ht);

    double seconds = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Execution time: %f\n", seconds);
    
    if (count >= MAX_COUNT_WORDS / 10) {
        for (int i = 0; i < MAX_COUNT_DISTRIBUTION; i++)
            fprintf(output_file, "%d %d\n", i+1, distribution[i]);
    }
}

int main(int argc, char* argv[]) {

    if (argc > 4) {
        FILE* input_file = fopen(argv[1], "r");
        if (!input_file) return -fprintf(stderr, "Cannot open file %s\n", argv[1]);

        FILE* output_file_time = fopen(argv[2], "w");
        if (!output_file_time) return -fprintf(stderr, "Cannot open file %s\n", argv[2]);

        for (int i = 1; i < MAX_COUNT_WORDS; i = MAX((i * 1.5), (i + 1)))
            ht_count_test(input_file, output_file_time, i);

        fclose(output_file_time);

        FILE* output_file_fill = fopen(argv[3], "w");
        if (!output_file_fill) return -fprintf(stderr, "Cannot open file %s\n", argv[3]);

        int count_for_fill = 5 * 1e5;
        for (int i = 50; i <= 80; i++) {
            ht_fill_test(input_file, output_file_fill, (double)i / 100, count_for_fill);
        }

        fclose(output_file_fill);

        FILE* output_file_dist = fopen(argv[4], "w");
        if (!output_file_dist) return -fprintf(stderr, "Cannot open file %s\n", argv[4]);

        ht_dist_test(input_file, output_file_dist);

        fclose(output_file_dist);

        fclose(input_file);

    } else {
        return -fprintf(stderr, "Wrong argument %s\n", argv[1]);
    }

    return 0;
}