#pragma once

#include "sha-256.h"
#include <ctype.h>
#include <malloc.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>

typedef struct Info {
    unsigned int nonce;
    BYTE *text;
} Info;

typedef struct Node {
    Info info;
    struct Node *next;
    struct Node *previous;
} Node;

typedef struct List {
    Node *latest;
    Node *first;
} List;

enum Colors {
    BLACK,
    BLUE,
    GREEN,
    CYAN,
    RED,
    MAGENTA,
    BROWN,
    GREY,
    DARKGREY,
    LIGHTBLUE,
    LIGHTGREEN,
    LIGHTCYAN,
    LIGHTRED,
    LIGHTMAGENTA,
    YELLOW,
    WHITE
};

Node *sha256(BYTE *text, char *constant);
Node *saveInfo(BYTE *text, int nonce);
void printHex(unsigned char *data);

void insertLatest(List *node, Node *info);
void showSingle(Info info);
void showList(List *list);
void clearList(List *list);

void color(int value);