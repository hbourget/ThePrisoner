#include <string.h>
#include <stdlib.h>
#include "../inih/ini.h"
#include "../headers/configurationHandler.h"

static int handler(void* config, const char* section, const char* name, const char* value)
{
    // config instance for filling in the values.
    configuration* pconfig = (configuration*)config;

    // define a macro for checking Sections and keys under the sections.
    #define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0

    // fill the values in config struct for Section 1.
    if(MATCH("user", "name"))
    {
        pconfig->name = strdup(value);
    }
    else if(MATCH("user", "number"))
    {
        pconfig->number = atoi(value);
    }

    return 1;
}

configuration getConfig()
{
    configuration config;
    config.name = NULL;  /* set defaults */
    config.number = 0;

    if (ini_parse("../config.ini", handler, &config) < 0) {
        printf("Can't load 'test.ini'\n");
    }
    return config;
}