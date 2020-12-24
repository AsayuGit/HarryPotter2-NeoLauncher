#ifndef _INI_H
#define _INI_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct key key;
typedef struct section section;
typedef struct ini ini;

section* iniAddSection(ini* ini, char* name);
void iniAddKey(section* dst, char* name, char* data);
void iniFree(ini* ini);
void iniSaveToFile(ini* ini);
ini* iniLoadFromFile(const char* filename);
char* iniGetKey(ini* ini, const char* sectionName, const char* keyName);
int iniSetKey(ini* ini, const char* sectionName, const char* keyName, char* data);

#endif