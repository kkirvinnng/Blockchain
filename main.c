#include "main.h"
#include "sha-256.h"

int main() {

    List list = {NULL, NULL};

    Node *sha;
    char str[50];

    char *aux;

    char constant[6];

    while (1) {
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
                //   a-z  A-Z                    0 - 9
                if (!isalpha(constant[i]) && !isdigit(constant[i])) {
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

        char *constant2 = (char *)malloc(sizeof(char) * strlen(constant));
        aux = (char *)malloc(sizeof(char) * strlen(constant) - 1);
        strcpy(constant2, constant);

        strncpy(aux, constant2, strlen(constant) - 1);
        sha = sha256(str, constant2);
        color(LIGHTGREEN);
        printf("\n > A Hash wich data is \"%s\" and starts with \"%s\" has been found!.\n\n", str, aux);
        insertLatest(&list, sha);
        color(LIGHTMAGENTA);

        printf(" > PRESS ESC TO EXIT OR ANY KEY TO CONTINUE\n");
        if (getch() == '\e') {

            break;
        }
        system("cls");

        free(aux);
        free(constant2);
    }

    printf("\n Showing blockchain...\n");

    showList(&list);

    clearList(&list);

    return 0;
}

void printHex(unsigned char *data) {
    for (int i = 0; i < 32; i++) {
        printf("%02x", data[i]);
        fflush(stdout);
    }
    printf("\n");
}

Node *sha256(BYTE *text, char *constant) {

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

        for (int i = 0; i < strlen(constant) - 1; i++) {
            if (tmp[i] != constant[i]) {

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
    printf("\n > The winning Nonce \"");
    color(LIGHTMAGENTA);
    printf("%s", info.text);
    color(GREY);
    printf("\" es: %d.", info.nonce);
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