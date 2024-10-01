#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#define MAXCLUBNAME 40
#define MAXSURNAME 80
#define MAXNAME 40
#define MAXCLUBNAME 40

typedef enum pos { Goalkeeper, Defender, Midfielder, Forward } pos;

typedef struct playerinfo {
    char club[MAXCLUBNAME];
    char name[MAXNAME];
    char surname[MAXSURNAME];
    unsigned short age;
    enum pos position;
    int cl_won;
    union playercl {
        int cl_year;
        char cl_final[4];
    } cl;
} player;

int pc = 0;

void fill_info(player* A, int* ppc);
player* input_base(player* A, int* ppc); // ����� �����
void output_base(player* A, int* ppc);
void print_player(player* A, int i); 

void search_player(player* A, int* ppc) {

    char tmp_name[MAXNAME];
    printf("\nEnter player name: ");
    scanf("%s", &tmp_name);
    char tmp_surname[MAXSURNAME];
    printf("\nEnter player surname: ");
    scanf("%s", &tmp_surname);

    for (int i = 0; i < *ppc; i++) {
        if (strcmp(tmp_name, A[i].name) == 0) {
            if (strcmp(tmp_surname, A[i].surname) == 0) {
                print_player(A, i);

                printf("\n\nPress enter to continue: ");
                getchar();
                getchar();
                return;
            }
        }
    }

    printf("\nPlayer not found, press any button to continue: ");
    getchar();
    getchar();
    return;
}

void delete_player(player* A, int* ppc) {

    if (*ppc > 0) { // ����� �� ������� �������� ����� ���� ��� �����
        printf("\nPick the number of a player (from 1 to %d) which you want to delete: ", *ppc);
        int pick = 0;
        scanf("%d", &pick);
        if ((pick < 1) || (pick > *ppc)) { // ����� �� ������� �������� ������� ��� � ����
            printf("\nWrong number");
            printf("\n\nPress enter to continue: ");
            getchar();
            getchar();
        }
        else {
            pick--;
            int pickend = *ppc;
            pickend--;
            for (pick; pick < pickend; pick++) {
                A[pick] = A[pick + 1];
            }
            *ppc = pickend;
        }
    }
    else {
        printf("\nBase is already empty");
        printf("\n\nPress enter to continue: ");
        getchar();
        getchar();
    }
    return;
}


int main() {

    int* ppc;
    ppc = &pc;
    player* A = NULL; // ��������� �� ��� ���������
    A = input_base(A, ppc);

    while (1) {

        system("cls");
        printf("\nSelect task:\n");
        printf("1. Add new player in a club\n");
        printf("2. Search a player by name and surname\n");
        printf("3. Output a player\n");
        printf("4. Output whole base\n");
        printf("5. Delete a player\n");
        printf("0. Exit\n");

        printf("\n");

        int tmpc = 0;
        int tmp;
        scanf("%d", &tmp);

        if (tmp == 1) { // ���������� ������ ������

            if (*ppc > 0) { // ���� �� ������, �� ������� realloc
                tmpc = *ppc + 1;
                A = (player*)realloc(A, tmpc * sizeof(player));
                fill_info(A, ppc);
                int tpc = 0; // ������ ������ ppc++;
                tpc = *ppc + 1;
                *ppc = tpc;
            }
            if (*ppc == 0) { // ���� ��� ������ ����� �� ���� ����, �� �������� ������ � ������� malloc
                
                A = (player*)malloc(sizeof(player));
                if (A == NULL) printf("Fail");
                fill_info(A, ppc);
                int tpc = 0;
                tpc = *ppc + 1;
                *ppc = tpc;
            }
        }
        if (tmp == 2) { // �����
            search_player(A, ppc);
        }
        if (tmp == 3) { // ����� ������
            printf("\nPick a number of a player to output (from 1 to %d): ", *ppc);
            int pick = 0;
            scanf("%d", &pick);
            if ((pick <= 0) || (pick > *ppc)) printf("\nIncorrect number of a player");
            else {
                pick--;
                print_player(A, pick);
            }
            printf("\n\nPress enter to continue: ");
            getchar();
            getchar();
        }  
        if (tmp == 4) { // ����� ���� ����
            printf("\nForballers base:\n\n");
            for (int i = 0; i < *ppc; i++) {
                print_player(A, i);
                printf("\n");
            }
            printf("\n\nPress enter to continue: ");
            getchar();
            getchar();
        }
        if (tmp == 5) { // �������� ������
            delete_player(A, ppc);
            A = (player*)realloc(A, *ppc * sizeof(player));
        }
        if (tmp == 0) { // �����
            output_base(A, ppc);
            free(A);
            return 0;
        }
    }
}


player* input_base(player* A, int* ppc) {

    FILE* base = fopen("baza.txt", "r");

    fseek(base, 0, SEEK_END);
    long pos = ftell(base);
    if (pos == 0)
    {
        return(0);
    }
    else rewind(base);

    int fpc = 0, tmpc = 0;
    

    while (!feof(base)) {

        *ppc = fpc; // �������� pc

        fscanf(base, "%d", &fpc);

        if (fpc == -1) {
            break;
        }
        fpc--;

        if (fpc > 0) { // ���� �� ������, �� ������� realloc
            tmpc = fpc + 1;
            A = (player*)realloc(A, tmpc * sizeof(player));
            if (A == NULL) printf("Fail");
            //fscanf(base, "%s", &A[fpc].club);
            int c = 0;
            c = fgetc(base);
            char tmpclub[MAXCLUBNAME];
            fgets(tmpclub, MAXCLUBNAME, base);
            c = strlen(tmpclub);
            c--;
            char newtmpclub[MAXCLUBNAME];
            int z = 0;
            for (z = 0; z < c; z++) {
                newtmpclub[z] = tmpclub[z];
            }
            newtmpclub[z] = '\0';
            strcpy(A[fpc].club, newtmpclub);

            fscanf(base, "%s", &A[fpc].name);
            fscanf(base, "%s", &A[fpc].surname);
            fscanf(base, "%d", &A[fpc].age);
            char tmppos[15];
            fscanf(base, "%s", &tmppos);
            if (strcmp(tmppos, "Goalkeeper") == 0) A[fpc].position = Goalkeeper;
            if (strcmp(tmppos, "Defender") == 0) A[fpc].position = Defender;
            if (strcmp(tmppos, "Midfielder") == 0) A[fpc].position = Midfielder;
            if (strcmp(tmppos, "Forward") == 0) A[fpc].position = Forward;
            //fgetc(base);
            fscanf(base, "%d", &A[fpc].cl_won);
            if (A[fpc].cl_won == 1) {
                fscanf(base, "%d", &A[fpc].cl.cl_year);
            }
            if (A[fpc].cl_won == 2) {
                fscanf(base, "%s", &A[fpc].cl.cl_final);
            }

            fgetc(base);
            fpc++;
        }
        if (fpc == 0) { // ���� ��� ������ ����� �� ���� ����, �� �������� ������ � ������� malloc

            A = (player*)malloc(sizeof(player));
            if (A == NULL) printf("Fail");

            //fscanf(base, "%s", &A[fpc].club);
            int c = 0;
            c = fgetc(base);
            char tmpclub[MAXCLUBNAME];
            fgets(tmpclub, MAXCLUBNAME, base);
            c = strlen(tmpclub);
            c--;
            char newtmpclub[MAXCLUBNAME];
            int z = 0;
            for (z = 0; z < c; z++) {
                newtmpclub[z] = tmpclub[z];
            }
            newtmpclub[z] = '\0';
            strcpy(A[fpc].club, newtmpclub);

            fscanf(base, "%s", &A[fpc].name);
            fscanf(base, "%s", &A[fpc].surname);
            fscanf(base, "%d", &A[fpc].age);
            char tmppos[15];
            fscanf(base, "%s", &tmppos);
            if (strcmp(tmppos, "Goalkeeper") == 0) A[fpc].position = Goalkeeper;
            if (strcmp(tmppos, "Defender") == 0) A[fpc].position = Defender;
            if (strcmp(tmppos, "Midfielder") == 0) A[fpc].position = Midfielder;
            if (strcmp(tmppos, "Forward") == 0) A[fpc].position = Forward;
            //fgetc(base);
            fscanf(base, "%d", &A[fpc].cl_won);
            if (A[fpc].cl_won == 1) {
                fscanf(base, "%d", &A[fpc].cl.cl_year);
            }
            if (A[fpc].cl_won == 2) {
                fscanf(base, "%s", &A[fpc].cl.cl_final);
            }

            fgetc(base);
            fpc++;
        }
    }
    fclose(base);
    return(A);
}

void output_base(player* A, int* ppc) {

    FILE* base = fopen("baza.txt", "w");
    fseek(base, 0L, SEEK_SET);
    for (int i = 0; i < *ppc; i++) {

        fprintf(base, "%d\n", i + 1);
        fprintf(base, "%s\n", A[i].club);
        fprintf(base, "%s\n", A[i].name);
        fprintf(base, "%s\n", A[i].surname);
        fprintf(base, "%d\n", A[i].age);
        if (A[i].position == Goalkeeper) fprintf(base, "Goalkeeper\n");
        if (A[i].position == Defender) fprintf(base, "Defender\n");
        if (A[i].position == Midfielder) fprintf(base, "Midfielder\n");
        if (A[i].position == Forward) fprintf(base, "Forward\n");
        if (A[i].cl_won == 1) {
            fprintf(base, "%d\n", A[i].cl_won);
            fprintf(base, "%d\n", A[i].cl.cl_year);
        }
        if (A[i].cl_won == 2) {
            fprintf(base, "%d\n", A[i].cl_won);
            fprintf(base, "%s\n", A[i].cl.cl_final);
        }
    }
    fprintf(base, "-1");
    fclose(base);
}

void fill_info(player* A, int* ppc) {

    int fpc = 0;
    fpc = *ppc;

    printf("\nEnter club name: ");
    //scanf("%s", &A[fpc].club);
    char empty[10];
    gets_s(empty, 10);
    char temp_club[MAXCLUBNAME];
    gets_s(temp_club, MAXCLUBNAME);
    //fgets(temp_club, MAXCLUBNAME, stdin);
    strcpy(A[fpc].club, temp_club);
    
    printf("\nEnter player name: ");
    scanf("%s", &A[fpc].name);
    printf("\nEnter player surname: ");
    scanf("%s", &A[fpc].surname);
    printf("\nEnter the age of the player: ");
    scanf("%d", &A[fpc].age);
    int pick = 0;
    printf("\nPick player position:\n1. Goalkeeper\n2. Defender\n3. Midfielder\n4. Forward\n\n");
    scanf("%d", &pick);
    if (pick == 1) A[fpc].position = Goalkeeper;
    if (pick == 2) A[fpc].position = Defender;
    if (pick == 3) A[fpc].position = Midfielder;
    if (pick == 4) A[fpc].position = Forward;

    printf("\nHas this player ever won a champions league?\n\nYes - 1\t\tNo - 2\n\n");
    pick = 0;
    scanf("%d", &pick);
    A[fpc].cl_won = pick;

    if (pick == 1) {
        printf("\nEnter the last year, when the trophy was won: ");
        scanf("%d", &A[fpc].cl.cl_year);
    }
    if (pick == 2) {
        char cl_yes[4] = { 'Y', 'e', 's', '\0' };
        char cl_no[4] = { 'N' , 'o', '\0', '\0' };
        printf("\nHave the player ever been in the final?\n\nYes - 1\t\tNo - 2\n\n");
        int newpick = 0;
        scanf("%d", &newpick);
        if (newpick == 1) {
            strcpy(A[fpc].cl.cl_final, cl_yes);
        }
        if (newpick == 2) {
            strcpy(A[fpc].cl.cl_final, cl_no);
        }
        if (newpick > 2 || newpick < 1) {
            printf("\nIncorrect number\n");
        }
    }
    return;
}

void print_player(player* A, int i) {

    printf("\nNumber of the player: %d", i + 1);
    printf("\nName: %s", A[i].name);
    printf("\nSurname: %s", A[i].surname);
    if (A[i].position == Goalkeeper) printf("\nPosition: Goalkeeper");
    if (A[i].position == Defender) printf("\nPosition: Defender");
    if (A[i].position == Midfielder) printf("\nPosition: Midfielder");
    if (A[i].position == Forward) printf("\nPosition: Forward");
    printf("\nAge: %d", A[i].age);
    printf("\nClub: %s", A[i].club);
    if (A[i].cl_won == 1) {
        printf("\nChampions league trophy - owned");
        printf("\nLast win - %d", A[i].cl.cl_year);
    }
    if (A[i].cl_won == 2) {
        printf("\nChampions league trophy - not owned");
        printf("\nBeen in the final? - %s", A[i].cl.cl_final);
    }

    return;
}
