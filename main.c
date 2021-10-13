#include "main.h"

#include "sha-256.h"

int main() {
    List list = {NULL, NULL};
    Node *aux;
    int option;
    do {
        option = selectMenuOption();

        system("cls");
        switch (option) {
            case 0:
                aux = encryptData();
                showSingle(aux->info);
                system("pause > nul");

                break;
            case 1:

                addDataEncrypted(&list);
                break;
            default:
                break;
        }

        system("cls");
    } while (option != 2);

    color(LIGHTCYAN);

    printf("\tSHOWING BLOCKCHAIN...\n\n");

    showList(&list);
    clearList(&list);

    return 0;
}

int selectMenuOption() {
    int i = 0;
    bool enter = false;
    gotoxy(6, 3);
    color(WHITE);
    printf(">");

    do {
        enter = false;
        menu();
        getch();
        if (GetAsyncKeyState(0x26) & 0x8000) { //  Arriba
            i = i > 0 ? i -= 1 : i;
        } else if (GetAsyncKeyState(0x28) & 0x8000) { // Abajo
            i = i < 2 ? i += 1 : i;
        } else if (GetAsyncKeyState(0x0D) & 0x8000) { // Enter
            enter = true;
        } else {
            i = i;
        }
        // fflush(stdin);

        Sleep(80);
        system("cls");
        gotoxy(6, 3 + (i * 2));
        color(WHITE);
        printf(">");
        gotoxy(20, 2);

    } while (!enter);
    return i;
}

void menu() {
    color(LIGHTCYAN);
    gotoxy(8, 3);
    printf("ENCRYPT AND ADD DATA TO BLOCKCHAIN");
    gotoxy(8, 5);
    printf("JUST ENCRYPT DATA");
    gotoxy(8, 7);
    printf("EXIT");
}

void addDataEncrypted(List *list) {
    Node *sha;
    while (1) {
        sha = encryptData();
        insertLatest(list, sha);

        color(LIGHTMAGENTA);

        printf(" > PRESS ESC TO EXIT OR ANY KEY TO CONTINUE\n");
        if (getch() == '\e') {
            system("cls");
            break;
        }
        system("cls");
    }
}

Node *encryptData() {
    Node *sha;
    char str[50];
    char *aux;
    char constant[6];

    color(LIGHTGREEN);
    printf("\n INSERT TOKEN : ");
    color(GREY);
    fflush(stdin);
    gets(str);
    color(LIGHTGREEN);
    printf("\n HASH EXAMPLE : ");
    color(GREY);
    printf("889d785a6be46c7d19582b06cd307d3bca51cafd24d37a06ad4624438b0500b1 \n");
    color(LIGHTGREEN);

    printf("\n CONDITION TO SEARCH (LIM. 4) : ");
    color(GREY);
    bool validInput = true;
    do {
        fflush(stdin);
        fgets(constant, 6, stdin);
        validInput = true;
        for (int i = 0; i < strlen(constant) - 1; i++) {
            //    c[i]  > f        ||    c[i]    < a             0 - 9
            if ((constant[i] > 102 || constant[i] < 97) && !isdigit(constant[i])) {
                validInput = false;
                system("cls");
                color(LIGHTGREEN);
                printf("\n INSERT TOKEN : ");
                color(GREY);
                printf("%s\n", str);
                color(LIGHTGREEN);
                printf("\n HASH EXAMPLE : ");
                color(GREY);
                printf("889d785a6be46c7d19582b06cd307d3bca51cafd24d37a06ad4624438b0500b1 \n");
                color(LIGHTGREEN);
                printf("\n CONDITION TO SEARCH (LIM. 4) : ");
                color(GREY);
                break;
            }
        }
    } while (!validInput);

    char *condition = (char *)malloc(sizeof(char) * strlen(constant));
    aux = (char *)calloc(sizeof(char), strlen(constant));
    strcpy(condition, constant);

    strncpy(aux, condition, strlen(constant) - 1);

    sha = sha256(str, condition);

    color(LIGHTGREEN);
    printf("\n > A HASH WICH DATA IS");
    color(LIGHTCYAN);
    printf(" \"%s\" ", str);
    color(LIGHTGREEN);
    printf("AND STARTS WITH");
    color(LIGHTCYAN);
    printf(" \"%s\" ", aux);
    color(LIGHTGREEN);
    printf("HAS BEE FOUND!.\n\n");

    free(aux);
    free(condition);

    return sha;
}

void printHex(unsigned char *data) {
    for (int i = 0; i < 32; i++) {
        printf("%02x", data[i]);
        fflush(stdout);
    }
    printf("\n");
}

Node *sha256(BYTE *text, char *condition) {
    SHA256_Context ctx;
    BYTE buf[BLOCK_SIZE];
    int nonce = 1;
    bool isValid = true;

    do {
        isValid = true;
        int length = snprintf(NULL, 0, "%d", nonce);
        char *str = (char *)malloc(length + 1 + strlen(text));
        snprintf(str, length + 1, "%d", nonce);
        strcat(str, text);
        //* SHA-CODE
        sha256_init(&ctx);
        sha256_update(&ctx, str, strlen(str));
        sha256_final(&ctx, buf);

        char tmp[5];

        snprintf(tmp, strlen(str) + 1, "%02x%02x", buf[0], buf[1]);

        color(LIGHTCYAN);
        printf(" > Nonce: %d\t", nonce);

        for (int i = 0; i < strlen(condition) - 1; i++) {
            if (tmp[i] != condition[i]) {
                isValid = false;
                break;
            }
        }
        color(GREY);

        if (isValid) {
            color(YELLOW);
        }
        printHex(buf);

        nonce++;
        free(str);
    } while (!isValid);

    return saveInfo(text, nonce - 1);
}

Node *saveInfo(BYTE *text, int nonce) {
    Node *new = (Node *)malloc(sizeof(Node));

    int length = snprintf(NULL, 0, "%d", nonce);
    (*new).info.text = (BYTE *)malloc(length + 1 + strlen(text));
    strcpy((*new).info.text, text);

    (*new).info.nonce = nonce;

    new->next = NULL;
    new->previous = NULL;

    return new;
}

void insertLatest(List *node, Node *info) {
    if (node->first == NULL) {
        node->first = info;
        node->latest = info;
    } else {
        node->latest->next = info;
        info->previous = node->latest;
        node->latest = info;
    }
}

void showSingle(Info info) {
    color(LIGHTGREEN);
    printf("\n > Nonce found for ");
    color(LIGHTCYAN);
    printf("\"%s\"", info.text);
    color(LIGHTGREEN);
    printf(" in : ");
    color(LIGHTCYAN);
    printf(" %d.\n", info.nonce);
}

void showList(List *list) {
    Node *aux = list->first;
    while (aux) {
        showSingle(aux->info);
        aux = aux->next;
    }
}
void clearList(List *list) {
    Node *iterable = list->first;
    Node *aux = iterable;

    while (iterable) {
        aux = iterable;
        iterable = iterable->next;
        free(aux->info.text);
        free(aux);
    }
}

void color(int value) {
    WORD color;

    HANDLE standarOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    if (GetConsoleScreenBufferInfo(standarOutput, &csbi)) {
        color = (csbi.wAttributes & 0xF0) + (value & 0x0F);
        SetConsoleTextAttribute(standarOutput, color);
    }
}