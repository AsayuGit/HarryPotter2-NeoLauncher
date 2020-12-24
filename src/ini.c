#include "ini.h"

enum INI_ErrorLabelId{
	INI_ERROR_FileNotFound = 0,
	INI_ERROR_UnableToRead,
	INI_ERROR_Unitialised,
	INI_NBOFERRORLABELS	
};

char* INI_ErrorLabels[INI_NBOFERRORLABELS] = {
	"Error: Couldn't oppen file %s !\n",
	"Error: Unable to read from %s !\n",
	"Error: INI Object uninitialised !\n"
};

struct key {
    char* name;
    void* data;
    key* next;
};

struct section{
    char* name;
    key* keys;
    section* next;
};

struct ini{
    //FILE* filePointer;
	char* filePath;
    struct section* sections;
};

char* iniSubStr(char* Src, int Start, int End){
    char* string;

    string = (char*)malloc(sizeof(char)*(End - Start + 1));
    memcpy(string, &Src[Start], End - Start);
    string[End - Start] = '\0';

    return string;
}

void iniFreeKeyList(key* keyList){
    if (keyList != NULL){
        iniFreeKeyList(keyList->next);
        if (keyList->data)
			free(keyList->data);
        if (keyList->name)
			free(keyList->name);
        free(keyList);
    }
}

void iniFreeSectionList(section* sectionList){
    if (sectionList != NULL){
        iniFreeSectionList(sectionList->next);
        iniFreeKeyList(sectionList->keys);
        if (sectionList->name)
			free(sectionList->name);
        free(sectionList);
    }
}

key* iniSearch(ini* ini, const char *sectionName, const char *keyName){
    section* sectionPointer;
    key* keyPointer;

	if (!ini)
		goto Error;

	sectionPointer = ini->sections;
	while (sectionPointer != NULL){ // Section search
		if (!sectionPointer->name)
			goto secNext;
		if (strcmp(sectionPointer->name, sectionName) == 0){ // Section match ?
			keyPointer = sectionPointer->keys;
			while (keyPointer != NULL){ // Key search
				if (strcmp(keyPointer->name, keyName) == 0){ // Key match ?
					return keyPointer;
				}else{
					keyPointer = keyPointer->next;
				}
			}
			break;
		}else{
secNext:
			sectionPointer = sectionPointer->next;
		}
	}
	return NULL;

Error:
	fprintf(stderr, INI_ErrorLabels[INI_ERROR_Unitialised]);
	printf(INI_ErrorLabels[INI_ERROR_Unitialised]);
	return NULL;
}

section* iniAddSection(ini* ini, char* name){
    section* sectionPointer;
    section* newSection;
    
    newSection = (section*)malloc(sizeof(section));
    memset(newSection, 0, sizeof(*newSection));
    newSection->name = name;
	
	if (!ini)
		goto Error;

	if (ini->sections == NULL){
		ini->sections = newSection;
	}else{
		sectionPointer = ini->sections;
		while ((sectionPointer != NULL) && (sectionPointer->next != NULL)){
			sectionPointer = sectionPointer->next;
		}
		sectionPointer->next = newSection;
	}
	return newSection;

Error:
	fprintf(stderr, INI_ErrorLabels[INI_ERROR_Unitialised]);
	printf(INI_ErrorLabels[INI_ERROR_Unitialised]);
	return NULL;
}

void iniAddKey(section* dst, char* name, char* data){
    key* keyPointer;
    key* newKey;
    
    newKey = (key*)malloc(sizeof(key));
    memset(newKey, 0, sizeof(*newKey));
    newKey->name = name;
    newKey->data = data;

    if (dst->keys == NULL){
        dst->keys = newKey;
    }else{
        keyPointer = dst->keys;
        while ((keyPointer != NULL) && (keyPointer->next != NULL)){
            keyPointer = keyPointer->next;
        }
        keyPointer->next = newKey;
    }
}

void iniFree(ini* ini) {
    iniFreeSectionList(ini->sections);
	//fclose(ini->filePointer);
	free(ini->filePath);
}

ini* iniLoadFromFile(const char *filename) {
    ini* ini;
    section* lastSection;
    char* data;
    char* dataPointer;
    int fileSize;
    int Start, End;
	FILE* filePointer;

    ini = malloc(sizeof(*ini)); // Allocate memory
    if (!ini) {
        goto Error;
    }
    memset(ini, 0, sizeof(*ini));

    filePointer = fopen(filename, "rb"); // Oppening the .ini in read mode
    if (filePointer == NULL) {
		fprintf(stderr, INI_ErrorLabels[INI_ERROR_FileNotFound], filename);
		printf(INI_ErrorLabels[INI_ERROR_FileNotFound], filename);
        goto Error;
    }

    fseek(filePointer, 0, SEEK_END);
    fileSize = ftell(filePointer);
    rewind(filePointer);

    data = (char*)malloc(sizeof(char)*(fileSize + 1));
    data[fileSize] = '\0';
    if (fread(data, 1, fileSize, filePointer) != fileSize){
        fprintf(stderr, INI_ErrorLabels[INI_ERROR_UnableToRead], filename);
		printf(INI_ErrorLabels[INI_ERROR_UnableToRead], filename);
        goto Error;
    }
    dataPointer = data;
	
	lastSection = iniAddSection(ini, NULL); // Section par défaut
	lastSection->keys = NULL;
	lastSection->next = NULL;
    while (dataPointer < (data + fileSize)){ // Copying the data from the file to the ini structure
        switch (*dataPointer)
        { 
        case '\r':
        case '\n':
        case '\t':
        case ' ':
        case '\0':
            dataPointer++;
            break;

        case '[': // New Section
            End = (int)strcspn(dataPointer, "]\r\n");
            lastSection = iniAddSection(ini, iniSubStr(dataPointer, 1, End));
            dataPointer += End + 1;
            break;
        
        default: // New key and value
            Start = (int)strcspn(dataPointer, "=\r\n");
            End = (int)strcspn(dataPointer, "\r\n\0");
            iniAddKey(lastSection, iniSubStr(dataPointer, 0, Start), iniSubStr(dataPointer, Start + 1, End));
            dataPointer += End;
            break;
        }
    }

    free(data);
    fclose(filePointer);

	ini->filePath = (char*)malloc(sizeof(char)*(strlen(filename)+1));
	strcpy(ini->filePath, filename);

    return ini;

Error:
    if (filePointer){
        fclose(filePointer);
    }
    if (ini){
        iniFree(ini);
    }
    return NULL;
}

void iniSaveToFile(ini *ini){
    section* sectionPointer;
    key* keyPointer;
	char space;
	FILE* filePointer;

	if (!ini){
		fprintf(stderr, INI_ErrorLabels[INI_ERROR_Unitialised]);
		printf(INI_ErrorLabels[INI_ERROR_Unitialised]);
		return;
	}

	filePointer = fopen(ini->filePath, "wb"); // Reoppening the file in write mode
	if (!filePointer){
		fprintf(stderr, INI_ErrorLabels[INI_ERROR_FileNotFound], ini->filePath);
		printf(INI_ErrorLabels[INI_ERROR_FileNotFound], ini->filePath);
		return;
	}
	rewind(filePointer);
    
	sectionPointer = ini->sections;
    while (sectionPointer != NULL){ // Section search
		space = 0;
		keyPointer = sectionPointer->keys;
		if (sectionPointer->name){
			space = 1;
			fprintf(filePointer, "[%s]", sectionPointer->name);
			if ((sectionPointer->next != NULL) || (keyPointer != NULL)){
				fprintf(filePointer, "\r\n");
			}
		}
		sectionPointer = sectionPointer->next;
        while (keyPointer != NULL){ // Key search
			if (keyPointer->name){
				space = 1;
				fprintf(filePointer, "%s=%s", keyPointer->name, keyPointer->data);
				if ((keyPointer->next != NULL) || (sectionPointer != NULL)){
					fprintf(filePointer, "\r\n");
				}
			}
            keyPointer = keyPointer->next;
        }
		if (space && sectionPointer){
			fprintf(filePointer, "\r\n");
		}
    }
	fclose(filePointer);
}

char* iniGetKey(ini* ini, const char* sectionName, const char* keyName){
    key* keyPointer;

	if (!ini)
		goto Error;

    keyPointer = iniSearch(ini, sectionName, keyName);
    if (keyPointer){
        return keyPointer->data;
    }
    return NULL;

Error:
	fprintf(stderr, INI_ErrorLabels[INI_ERROR_Unitialised]);
	printf(INI_ErrorLabels[INI_ERROR_Unitialised]);
	return NULL;
}

int iniSetKey(ini* ini, const char* sectionName, const char* keyName, char* data){
    key* keyPointer;

	if (!ini)
		goto Error;

    keyPointer = iniSearch(ini, sectionName, keyName);
    if (keyPointer){
        free(keyPointer->data);
        keyPointer->data = (char*)malloc(sizeof(char)*(strlen(data)+1));
		strcpy(keyPointer->data, data);
        return 0;
    }
	return -1;
Error:
	fprintf(stderr, INI_ErrorLabels[INI_ERROR_Unitialised]);
	printf(INI_ErrorLabels[INI_ERROR_Unitialised]);
	return -1;
}