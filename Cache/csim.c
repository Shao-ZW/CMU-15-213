#include "cachelab.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <limits.h>
#include <getopt.h>
#include <string.h>

void get_argument(int argc, char** argv);
void update_cache(unsigned int address);
void init_cache(void);
void update_timestamp(int set);
void handle(void);
void free_cache(void);

struct cache_line{
    int valid_bit;
    int tag;
    int timestamp;
};

int hit_count, miss_count, eviction_count;
int s, E, b, S;
char t[30];
struct cache_line** _cache;

int main(int argc, char** argv)
{
    get_argument(argc, argv);
    
    init_cache();
    handle();
    free_cache();

    printSummary(hit_count, miss_count, eviction_count);

    return 0;
}

void update_cache(unsigned int address)
{
    int a_tag = address >> (s + b);
    int a_set = (address >> b) & ((-1U) >> (64 - s));
    int i = a_set;
    int k = 0;
    
    update_timestamp(a_set);
    
    for(int j = 0; j < E; ++j)
        if(_cache[i][j].tag == a_tag && _cache[i][j].valid_bit)
        {
            ++hit_count;
            _cache[i][j].timestamp = 0;
            return;
        }

    
    ++miss_count;

    for(int j = 0; j < E; ++j)
    {
        if(!_cache[i][j].valid_bit) 
        {
            _cache[i][j].valid_bit = 1;
            _cache[i][j].tag = a_tag;
            _cache[i][j].timestamp = 0;
            return;
        }     

        if(_cache[i][j].timestamp > _cache[i][k].timestamp)
            k = j;
    }
          
    ++eviction_count;

    _cache[i][k].tag = a_tag;
    _cache[i][k].timestamp = 0;
}

void init_cache(void)
{
    _cache = (struct cache_line**) malloc(sizeof(struct cache_line*) * S);
    for(int i = 0; i < S; ++i)
    {
        _cache[i] = (struct cache_line*) malloc(sizeof(struct cache_line) * E); 

        for(int j = 0; j < E; ++j)
        {
            _cache[i][j].valid_bit = 0;
            _cache[i][j].timestamp = 0;
        }
    }      
}

void free_cache(void)
{
    for(int i = 0; i < S; ++i)
        free(_cache[i]);
    free(_cache);
}

void update_timestamp(int set)
{
    for(int j = 0; j < E; ++j)  
        if(_cache[set][j].valid_bit)
            ++_cache[set][j].timestamp;
}

void get_argument(int argc, char** argv)
{
    int opt;

    while(-1 != (opt = getopt(argc, argv, "s:E:b:t:")))
    {
        switch(opt)
        {
            case 's':
                s = atoi(optarg);
                S = 1 << s;
                break;
            case 'E':
                E = atoi(optarg);
                break;
            case 'b':
                b = atoi(optarg);
                break;
            case 't':
                strcpy(t, optarg);
                break;
            default: 
				exit(-1);
        }
    }
}

void handle(void)
{
    FILE* pFile = fopen(t, "r");

    if(!pFile)  exit(-1);
    
    char op;
    unsigned int address, bits_size; 

    while(fscanf(pFile, "%c %xu,%d", &op, &address, &bits_size) > 0)//address为16进制，读取时要用%xu
    {
        switch(op)  
        {
            case 'S':
                update_cache(address);
                break;
            case 'L':
                update_cache(address);
                break;
            case 'M':
                update_cache(address);
                update_cache(address);
                break;
            default: 
                break;
        }
    }

    fclose(pFile);
}
