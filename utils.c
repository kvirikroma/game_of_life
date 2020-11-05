#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

#include "include/bit_array2d.h"
#include "include/array2d.h"
#include "include/life_runner.h"


void exit_on_syntax_error(void)
{
    fprintf(stderr, "%s", "Invalid command line parameters detected!\n");
    exit(1);
}


void* check_pointer_after_malloc(void* ptr)
{
    if (!ptr)
    {
        fprintf(stderr, "%s", "An error occurred while allocating memory! Exiting\n");
        exit(1);
    }
    return ptr;
}


void print_field(bit_array2d* field)
{
    for (uint32_t y = 0; y < field->y_size; y++)
    {
        for (uint32_t x = 0; x < field->x_size; x++)
        {
            printf("%d ", bit_array2d_get_bit(field, x, y, 0));
        }
        printf("\n");
    }
}


void print_array2d(array2d* array)
{
    for (uint32_t y = 0; y < array->y_size; y++)
    {
        for (uint32_t x = 0; x < array->x_size; x++)
        {
            printf("%d ", *(uint32_t*)array2d_get_item_ptr(array, x, y));
        }
        printf("\n");
    }
}

void sleep_ms(double milliseconds)
{
    if (milliseconds > 0)
    {
        usleep(1000 * milliseconds);
    }
}


int64_t get_current_millisecond(void)
{
    struct timeval time;
    gettimeofday(&time, NULL);
    return ((time.tv_usec / 1000) + (time.tv_sec * 1000));
}


void save_runner_snapshot_to_file(life_runner_snapshot* snapshot, const char* filename)
{
    FILE* file = fopen(filename, "wb");
    fwrite(&snapshot->size, sizeof(uint32_t), 1, file);
    fwrite(snapshot->data, sizeof(uint8_t), snapshot->size, file);
    fclose(file);
}


bool load_runner_snapshot_from_file(life_runner_snapshot* snapshot, const char* filename, bool snapshot_was_initialized)
{
    bool result = true;
    snapshot->size = 0;
    if (snapshot_was_initialized)
    {
        life_runner_snapshot_delete(snapshot);
    }
    FILE* file = fopen(filename, "rb");
    if (!file)
    {
        return false;
    }
    fread(&snapshot->size, 1, sizeof(uint32_t), file);
    fflush(file);
    if (snapshot->size < 15)
    {
        result = false;
    }
    else
    {
        snapshot->data = malloc(snapshot->size);
        if (fread(snapshot->data, 1, snapshot->size, file) != snapshot->size)
        {
            result = false;
            life_runner_snapshot_delete(snapshot);
        }
    }
    fclose(file);
    return result;
}
