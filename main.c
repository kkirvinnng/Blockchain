#include "main.h"

#include "sha-256.h"

int main() {
    List list = {NULL, NULL};

    while(1){
        switch(selectMenuOption()){
            case 0:
                break;
            case 1:
                break;
            default:
                break;
        }

        printf(" > PRESS ESC TO EXIT OR ANY KEY TO CONTINUE\n");
        if (getch() == '\e') break;
        system("cls");
    }
    addDataEncrypted(&list);
    color(LIGHTCYAN);
    printf("\n   SHOWING BLOCKCHAIN...");

    showList(&list);
    clearList(&list);

    return 0;
}

int selectMenuOption(){
    int i;
    bool enter = false;
    gotoxy(6, 3);
    color(WHITE);
    printf(">");

    do{
        i = 0;
        char c;
        menu();
        switch(c = getch()){
            case 72: // UP
                i = i > 0 ? i-=1 : i;
                break;
            case 80: // DOWN
                i = i < 1 ? i+=1 : i;
                break;
            case '\r':
                enter = true;
                break;
        }
        system("cls");

        gotoxy(6, 3 + (i*2));
        color(WHITE);
        printf(">");
    } while (!enter);
}

void menu() {
    color(LIGHTCYAN);
    gotoxy(8, 3);
    printf("ENCRYPT AND ADD DATA TO BLOCKCHAIN");
    gotoxy(8, 5);
    printf("JUST ENCRYPT DATA");
}

void addDataEncrypted(List *list) {
    Node *sha;
    while(1){
        sha = encryptData();
        insertLatest(list, sha);

        color(LIGHTMAGENTA);
        printf(" > PRESS ESC TO EXIT OR ANY KEY TO CONTINUE\n");
        if (getch() == '\e') break;
        system("cls");
    }
}

Node *encryptData() {
    Node *sha;
    char str[50];
    char *aux;
    char constant[6];

    color(LIGHTGREEN);
    printf("\n TOKEN : ");
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
                printf("\n TOKEN : ");
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
    printf("\n > A HASH WICH DATA IS \"%s\" AND STARTS WITH \"%s\" HAS BEE FOUND!.\n\n", str, aux);
    color(LIGHTMAGENTA);

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
    color(GREY);
    printf("\n > The nonce found for \"");
    color(LIGHTCYAN);
    printf("%s", info.text);
    color(GREY);
    printf("\" is: %d.\n", info.nonce);
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