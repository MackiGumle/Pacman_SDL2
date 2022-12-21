#include "pacman.h"


char* load_map (char* path, char* map)
{
    FILE* file = fopen(path, "r");
    

    if(file == NULL)
        return NULL;

    while (fgets(map, MAP_WIDTH, file) != NULL)
    {
        
    }
       
}





