/*Masterminds IST (@version) v1.7;
Created by (@author): Gustavo Henrique Martins & Martim Torre;
IST Number: 91632 & 93135;
Course: Integrated Master in Electrical and Computer Engineering (MEEC);

Year: 2019;

Description: Classical Masterminds game, where which player has to decode a random color code made by the computer, or by another
player, depending on it's game mode, in limited tries.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

/*---------- General definitions ----------*/
#define version 1.7
#define YES 1
#define NO 0

#define RC_RP 66  //"Right color, right place"
#define RC_WP 87  //"Right color, wrong place"
//#define WC_WP 48  //"Wrong color, wrong place"

#define RUNNING_GAME 2
#define WON_GAME 1
#define LOST_GAME 0

#define READ 0
#define APPEND 1
#define SORT 2

/*---------- Game Modes ----------*/
#define  Single_player 49   //ASCII code to '1'
#define  Multiplayer 50     //ASCII code to '2'
#define  PvP 51             //ASCII code to '3'
#define  CvP 52             //ASCII code to '4'
#define  CvC 53             //ASCII code to '5'

/*---------- OS Detection ----------*/

#ifdef __linux__         // UNIX SYSTEMS

    #define CLEAR_SCR printf("\033[2J")

    /*---------- Unix systems colors definitions ----------*/

    #define RED           printf ("\e[101m")
    #define GREEN         printf ("\e[102m")
    #define BLUE          printf ("\e[44m")
    #define YELLOW        printf ("\e[103m")
    #define PURPLE        printf ("\e[45m")
    #define GREY          printf ("\e[100m")
    #define WHITE         printf ("\e[40m")
    #define GOLD          printf ("\e[43m")
    #define DARK_CYAN     printf ("\e[46m")
    #define CYAN          printf ("\e[106m")
    #define PINK          printf ("\e[105m")
    #define SILVER        printf ("\e[47m")

    #define DEFAULT_TXT     printf ("\e[49m")

#elif defined _WIN32    // WINDOWS SYSTEMS

    #include <windows.h>
    #define CLEAR_SCR system("cls")

    /*---------- Windows colors definitions ----------*/

    #define RED             SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 204)
    #define GREEN           SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 170)
    #define BLUE            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 17)
    #define YELLOW          SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 238)
    #define PURPLE          SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 85)
    #define GREY            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 136)
    #define WHITE           SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 255)
    #define GOLD            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 102)
    #define DARK_CYAN       SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 51)
    #define CYAN            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 187)
    #define PINK            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 221)
    #define SILVER          SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 119)

    #define DEFAULT_TXT     SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7)

#elif defined (__APPLE__) && defined (__MACH__)

    #include "TargetConditionals.h"

    #if TARGET_OS_MAC == 1

        #define CLEAR_SCR  system("clear")

        #define RED           printf ("\e[101m")
        #define GREEN         printf ("\e[102m")
        #define BLUE          printf ("\e[44m")
        #define YELLOW        printf ("\e[103m")
        #define PURPLE        printf ("\e[45m")
        #define GREY          printf ("\e[100m")
        #define BLACK         printf ("\e[40m")
        #define GOLD          printf ("\e[43m")
        #define DARK_CYAN     printf ("\e[46m")
        #define CYAN          printf ("\e[106m")
        #define PINK          printf ("\e[105m")
        #define SILVER        printf ("\e[47m")

        #define DEFAULT_TXT     printf ("\e[49m")

    #endif // TARGET_OS_MAC

#else

    #error "Unknown compiler"

#endif

void TheRegister (int open_mode, char players_names[20], int code_length, char color_repeat, char code[8], int gamemode, int num_colors, int tries, int time_taken, char board[16][8], char hits[16][5]){

    int i=0, j, aux=0, playerID_num, gen_playerID=0;
    char line[50] = {NULL}, info_str[50], cmp_str[20], talvez_funcione[50], playerID[5][4] ={{0}};
    char *token;
    char *array[10];

    //printf("\n O REGISTRO\n");

    FILE *historic;

    historic = fopen("historico.dat", "a+");

    if(!historic){
        printf("\nOh oh! It seems like an error... Please, try again...\n");
        exit(0);
    }

    if(open_mode == READ){

        printf("\n\n\tTHE GUINNESS BOOK\n\n");

        while (fgets(line, 50, historic) != NULL){

            i++;
            if(line[0] == 'G')
                printf("GameID%s\n", &line[2]);
            if(line[0]  == 'T')
                printf("Try: %s", &line[2]);

        }

        if(i == 0)
            printf("\nOh oh, you need to play for a while before using the leaderboards.\n");

    }
    if(open_mode == APPEND){

        gen_playerID = (rand()%999)+1;

        while (fgets(line, 50, historic) != NULL){  // Primeiro, le-se o arquivo para encontrar informacoes sobre o ultimo jogo

            if(line[0] == 'G'){

                strcpy(info_str, line);
                playerID_num = atoi(&line[7]);
                //printf("\n%d\n", playerID_num);

                while(gen_playerID == playerID_num)
                    gen_playerID = (rand()%999)+1;

            }

        }

        token = strtok(info_str, " ");

        while(token != NULL){

            array[i++] = token;
            token = strtok(NULL, " ");

        }

        fprintf(historic, "G ");

        /*----- Gera GameID -----*/

        aux = atoi(array[1]);
        aux++;
        //sprintf(aux_str, "%d", aux);

        if(aux<10)
            fprintf(historic, "00%d ", aux);
        if(aux<100 && aux>9)
            fprintf(historic, "0%d ", aux);
        if(aux>99)
            fprintf(historic, "%d ", aux);

        fclose(historic);
        historic = fopen("historico.dat", "a+");

        aux = 0;

        while(fgets(talvez_funcione, sizeof talvez_funcione, historic) != NULL){

            if(talvez_funcione[0] == 'G')
                sscanf(&talvez_funcione[11], "%s", cmp_str);

            //printf("\nString 1: %s", cmp_str);

            switch (gamemode){

            case Multiplayer:

                    if(strstr(cmp_str, players_names) != NULL && talvez_funcione[0] == 'G'){
                        sscanf(&talvez_funcione[6], "%s", playerID[i]);
                        //printf("\nMP PlayerID(%d): %s\n", i, playerID[i]);
                        break;
                    }

                break;

            case Single_player:

                    if(strstr(cmp_str, players_names) != NULL && talvez_funcione[0] == 'G'){
                        sscanf(&talvez_funcione[6], "%s", playerID[0]);
                        //printf("\nSP PlayerID: %s\n", playerID[0]);
                        aux = 1;

                    }
                    break;

            }
        }

        fclose(historic);
        historic = fopen("historico.dat", "a+");

        //printf("\nAux: %d\n", aux);

        if(gamemode == Single_player){

            if(aux == 1)
                fprintf(historic, "%s ", playerID[0]);
            if(aux == 0){

                printf("\nGen playerID: %d", gen_playerID);

                if(gen_playerID<10)
                    fprintf(historic, "P00%d ", gen_playerID);
                if(gen_playerID<100 && gen_playerID>9)
                    fprintf(historic, "P0%d ", gen_playerID);
                if(gen_playerID>99)
                    fprintf(historic, "P%d ", gen_playerID);
            }

            fprintf(historic, "%s ", players_names);
            fprintf(historic, "%d ", code_length);
            fprintf(historic, "%d ", num_colors);
            fprintf(historic, "%c ", color_repeat);
            for (i=0; i<code_length; i++)
                fprintf(historic, "%c", code[i]);
            fprintf(historic, " %d ", tries);
            fprintf(historic, "%d\n", time_taken);
            for(i=0; i<tries; i++){
                fprintf(historic, "T ");
                if(i<10)
                    fprintf(historic, "0%d ", i+1);
                if(i>9)
                    fprintf(historic, "%d ", i+1);
                for(j=0; j<code_length; j++){
                    fprintf(historic, "%c", board[i][j]);
                }
                fprintf(historic, " %s\n", hits[i]);
            }

        }

        //fprintf(historic, "\n");

        //printf("\n%s\n", aux_str);

    }
    if(open_mode == SORT){

        printf("\nModo de Ordenacao");

    }

    //fprintf(historic,"Ola");

    fclose(historic);

}

int CommandDetection (char user_input[20], int *mods){

    int i=0, loop, error_aux = NO;
    char command_list[7][14] = {"/help", "/rules", "/settings", "/records", "/exit", "/godmode", "/testmode"};

    printf("\nUser input: %s\n", user_input);
    loop = YES;

    for(i=0; i<=6; i++){

        error_aux = error_aux+1;

        if(strstr(command_list[i], user_input) != NULL){

            //printf("\nString comparada.\n");
            error_aux = NO;

            switch (i){

            case 0:
                // HELP
                CLEAR_SCR;
                printf("\nWelcome to Masterminds v%.1f!\n", version);
                printf("\n  This is the valid commands list:\n");
                printf(" /help\t\t/rules\t\t/settings\t\t/records\t/testmode\t/exit\n");
                printf("This game was created by Gustavo Henrique Martins and Martim Torres under the scope of the final project of Programming discipline at Instituto Superior Tecnico.");
                printf("\nYou can use another command or just press 'Enter' to start the game!\n>");
                break;

            case 1:
                // RULES
                CLEAR_SCR;
                printf("\n  Masterminds rules:\n\n");
                printf("\tMasterminds game is a very traditional game which consists of a secret code to be discovered. The player has limited attempts to hit the code, and after every attempt, white blocks (");
                WHITE;
                printf("  ");
                DEFAULT_TXT;
                printf(") at right side will indicate right colors in wrong place, and red blocks (");
                RED;
                printf("  ");
                DEFAULT_TXT;
                printf(") will indicate right colors at right place. This rules are the same for all game modes.\n");
                printf("\n\n  Single Player:\n\nIn this mode the player needs to discover the random code generated by the machine. Number of attempts depends on difficulty and code length chosen by the player.\n");
                printf("\n\n  Multiplayer:\n\nThe rules are the same than in 1 player mode, each player will complete his game and in the end win who hits the code in fewer attempts.\n");
                printf("\n\n  Player vs Player:\n\nIn the PvP mode, 2-4 players will compete against each other. First, one player will create a secret code, and then the other players will try to guess his code. Win who guess it first.\n");
                printf("\n\n  Computer vs Player:\n\nThe roles are reversed. Now the player create a code to be discovered by the machine. You define how many tries and the code length. You think you're enough to can beat this amazing Artificial Intelligence?\n");
                printf("\n\n  Computer vs Computer:\n\nOK, maybe this is not the most exciting mode in this game, but it is a really good tutorial. And improves my grade at the discipline.");
                printf("\n\n  PRESS ENTER TO START THE GAME OR TYPE ANOTHER COMMAND\n");
                break;

            case 2:
                // SETTINGS
                CLEAR_SCR;
                printf("\nBacking to the Settings!");
                printf("\nAny progress will be lost. Keep that in mind.\n");  /// @todo (student#5#): Implementar a funcionalidade
                break;

            case 3:
                // RECORDS
                TheRegister(APPEND, 0, 5, 'N', "LFGHI", Single_player, 12, 8, 125, 0, 0);  // Voltar para modo READ
                printf("\n\nPress Enter to start the game or type another command.\n>");
                break;

            case 4:
                // EXIT
                printf("\nHope you enjoyed! We see you soon! :)\n");
                exit(0);
                break;

            case 5:
                // GODMODE
                loop = NO;
                error_aux = NO;
                *mods = 1;
                break;

            case 6:
                // TESTEMODES
                printf("\nYou're entering the Test Mode! Beep\n");
                *mods = 2;
                loop = NO;
                sleep(4);
                break;

            default:
                // DEFAULT - Esta condição só age em caso de qualquer erro inesperado.
                printf("\nOops! Something went wrong! Type your command again!\n\n>");
                break;
            }
        }
        if(error_aux == 7){  // Se nenhum dos comandos acima for válido, "i" incrementa até 7.
            loop = YES;
            printf("\nOuch! Invalid Command! :(\n\n>");
            break;
        }
    }

    return loop;

}

int GameSetup (int *game_mode, int *num_of_players, int *code_length, char *color_repeat, char players_names[5][20], int *difficulty, int *max_time, int mods, int *num_rand_tries){

    int i, loop, local_num_of_players, local_code_length, local_difficulty, local_max_time, line_num, local_rand_tries;
    char local_color_repeat;
    char user_input[20], local_names[5][20]={{0}};

    CLEAR_SCR;
    printf("\n\tWelcome to Masterminds!\n");
    if(mods == 1)
        printf("\nHey, how did you knew this? Anyway, use your new powers wisely!\n");
    printf("\nTo start the game you need to choose your game mode!\n");
    printf("Available game modes are:\n\n\t1 - Single Player\t\t2 - Multiplayer\t\t3 - Player vs Player\t\t4 - Computer vs Player\t\t5 - Computer vs Computer\n");
    printf("\nType your mode's number\n>");

    do{

        fgets(user_input, sizeof (user_input), stdin);

        switch (user_input[0]){

        case '/':
            CommandDetection(user_input, &mods);
            loop = YES;
            break;

        case Single_player:

            /*-------------------- SINGLE PLAYER --------------------*/

            *game_mode = Single_player;
            *num_of_players = 1;
            printf("\n\t\tSingle Player\n");

            printf("\nInsert your name:\n>");

            do{
            scanf(" %s", local_names[0]);
                if(strlen(local_names[0])>20)
                    printf("\nYou doesn't know your own name? Fine. Just make sure it isn't greater than 20 characters.\n\nNow, insert it again\n\n>");
            }while(strlen(local_names[0])>20);

            sprintf(players_names[0], "%s", local_names[0]);

            do{

                printf("\nChoose your code length between 4 - 8.\n>");
                scanf("%d", &local_code_length);
                *code_length = local_code_length;

                if (local_code_length<4 || local_code_length>8)
                    printf("\nHey, your code length is wrong! :(\n\n");

            }while(local_code_length<4 || local_code_length>8);

            do{

                printf("\nYou want to run against time? In case you want it, enter a maximum time between 60s to 400s. \nIf you don't just type 0.\n>");
                scanf("%d", &local_max_time);
                *max_time = local_max_time;

                if (local_max_time == 0){
                    break;
                }

                if (local_max_time<60 || local_max_time>400)
                    printf("\nWould be better if you insert a valid time value...\n\n");

            }while(local_max_time<60 || local_max_time>400);

            do{

                printf("\nIt's the last question, I promise. Can we repeat colors? (Use just Y/N)\n\n>");
                scanf(" %c", &local_color_repeat);
                *color_repeat = local_color_repeat;

                if (local_color_repeat != 'Y' && local_color_repeat != 'N' && local_color_repeat != 'y' && local_color_repeat != 'n') /// @todo (guthh#3#): Adicionar suporte a n e y minusculos
                    printf("\nI think you hit the wrong key! Be sure of typing just Y/N.\n");

            }while(local_color_repeat != 'Y' && local_color_repeat != 'N' && local_color_repeat != 'y' && local_color_repeat != 'n');

            printf("\nMaybe I've lied... Now you need to choose the difficulty (Or enter your own maximum of attempts between 10 - 20):\n\n1 - Easy\t\t2 - Normal\t\t3 - Hard\n\n>");

            do{
                scanf(" %d", &local_difficulty);
                *difficulty = local_difficulty;

                if (local_difficulty != 1 && local_difficulty != 2 && local_difficulty != 3 && local_difficulty<10)
                    printf("\nAre you trying to cheat? Please choose one difficulty to play!\n\n>");
                if (local_difficulty>20)
                    printf("\nThis make things unfair. Please set your difficulty right.\n\n>");

            }while((local_difficulty != 1 && local_difficulty != 2 && local_difficulty != 3 && local_difficulty<10) || local_difficulty>20);

            loop = NO;
            break;


        case Multiplayer:

            /*-------------------- MULTIPLAYER --------------------*/

            *game_mode = Multiplayer;
            printf("\n\t\tMultiplayer\n");

            do{

                printf("\nChoose number of players between 2 - 5:\n>");
                scanf("%d", &local_num_of_players);
                *num_of_players = local_num_of_players;

                if (local_num_of_players<2)
                    printf("\nCheck your group again! I think someone is missing! You need at least 2 to play! :(\n");
                if (local_num_of_players>5)
                    printf("\nCheck if you have counted right! There are too many people!\n");

            }while(local_num_of_players<2 || local_num_of_players>5);

            for (i=0; i<local_num_of_players; i++){

                printf("\nInsert Player's %d name:\n>", i+1); // 'i+1' para que a contagem de jogadores comece em 1

                do{
                    scanf(" %s", local_names[i]);
                    printf("\n\tYour name is: %s\n", local_names[i]);

                    if(strlen(local_names[0])>20)
                        printf("\nYou doesn't know your own name? Fine. Just make sure it isn't greater than 20 characters.\n\nNow, insert it again\n\n>");

                }while(strlen(local_names[0])>20);

                sprintf(players_names[i], "%s", local_names[i]);

            }

            /*for (i=0; i<5;i++)
                printf("\nPlayers Names %d: %s", i+1, players_names[i]);*/

            do{

                printf("\nHow big will be the code? Pick up a number between 4 - 8(This is the key size for everyone):\n>");
                scanf("%d", &local_code_length);
                *code_length = local_code_length;

                if (local_code_length<4)
                    printf("\nHey! This makes it very easy! Choose a higher challenge!\n");
                if (local_code_length>8)
                    printf("\nYou are beyond this game's capacity! Please, choose a smaller code...\n");

            }while(local_code_length<4 || local_code_length>8);

            do{

                printf("\nIt's the last question, I promise. Can we repeat colors? (Use just Y/N)\n\n>");
                scanf(" %c", &local_color_repeat);
                //printf("\n%c\n", local_color_repeat);
                *color_repeat = local_color_repeat;

                if (local_color_repeat != 'Y' && local_color_repeat != 'N' && local_color_repeat != 'y' && local_color_repeat != 'n')
                    printf("\nI think you hit the wrong key! Be sure of typing just Y/N.\n");

            }while(local_color_repeat != 'Y' && local_color_repeat != 'N' && local_color_repeat != 'y' && local_color_repeat != 'n');

            printf("\nMaybe I've lied... Now you need to choose the difficulty (Or enter your own maximum of attempts between 10 - 20, and again: this definition is for everyone):\n\n1 - Easy\t\t2 - Normal\t\t3 - Hard\n\n>");

            do{
                scanf(" %d", &local_difficulty);
                *difficulty = local_difficulty;

                if (local_difficulty != 1 && local_difficulty != 2 && local_difficulty != 3 && local_difficulty<10)
                    printf("\nAre you trying to cheat? Please choose one difficulty to play!\n\n>");
                if (local_difficulty>20)
                    printf("\nThis make things unfair. Please set your difficulty right.\n\n>");

            }while((local_difficulty != 1 && local_difficulty != 2 && local_difficulty != 3 && local_difficulty<10) || local_difficulty>20);

            loop = NO;
            break;


        case PvP:

            /*-------------------- PvP --------------------*/

            *game_mode = PvP;
            printf("\n\t\tPlayer vs Player\n");

            for (i=0; i<2; i++){

                printf("\nInsert Player's %d name:\n>", i+1); // 'i+1' para que a contagem de jogadores comece em 1
                scanf(" %s", local_names[i]);
                printf("\n\tYour name is: %s\n", local_names[i]);
                sprintf(players_names[i], "%s", local_names[i]);

            }

            /*for (i=0; i<5;i++)
                printf("\nPlayers Names %d: %s", i+1, players_names[i]);*/

            do{

                printf("\nHow big will be the code? Pick up a number between 4 - 8:\n>");
                scanf("%d", local_code_length);
                *code_length = local_code_length;

                if (local_code_length<4)
                    printf("\nHey! This makes it very easy! Choose a higher challenge!\n");
                if (local_code_length>8)
                    printf("\nYou are beyond this game's capacity! Please, choose a smaller code...\n");

            }while(local_code_length<4 || local_code_length>8);

            printf("\nNow you need to choose the difficulty:\n\n1 - Easy\t\t2 - Normal\t\t3 - Hard\n\n>");

            printf("\nMaybe I've lied... Now you need to choose the difficulty (Or enter your own maximum of attempts between 10 - 20, and again: this definition is for everyone):\n\n1 - Easy\t\t2 - Normal\t\t3 - Hard\n\n>");

            do{
                scanf(" %d", &local_difficulty);
                *difficulty = local_difficulty;

                if (local_difficulty != 1 && local_difficulty != 2 && local_difficulty != 3 && local_difficulty<10)
                    printf("\nAre you trying to cheat? Please choose one difficulty to play!\n\n>");
                if (local_difficulty>20)
                    printf("\nThis make things unfair. Please set your difficulty right.\n\n>");

            }while((local_difficulty != 1 && local_difficulty != 2 && local_difficulty != 3 && local_difficulty<10) || local_difficulty>20);

            loop = NO;
            break;


        case CvP:

            /*-------------------- CvP --------------------*/

            *game_mode = CvP;
            *num_of_players = 1;
            printf("\n\t\tComputer vs Player\n");

            printf("\nInsert your name:\n>");
            scanf(" %s", local_names[0]);
            sprintf(players_names[0], "%s", local_names[0]);

            do{

                printf("\nChoose your code length between 4 - 8.\n>");
                scanf("%d", &local_code_length);
                *code_length = local_code_length;

                if (local_code_length<4 || local_code_length>8)
                    printf("\nHey, your code length is wrong! :(\n\n");

            }while(local_code_length<4 || local_code_length>8);

            printf("\nMaybe I've lied... Now you need to choose the difficulty (Or enter your own maximum of attempts between 10 - 20, and again: this definition is for everyone):\n\n1 - Easy\t\t2 - Normal\t\t3 - Hard\n\n>");

            do{
                scanf(" %d", &local_difficulty);
                *difficulty = local_difficulty;

                if (local_difficulty != 1 && local_difficulty != 2 && local_difficulty != 3 && local_difficulty<10)
                    printf("\nAre you trying to cheat? Please choose one difficulty to play!\n\n>");
                if (local_difficulty>20)
                    printf("\nThis make things unfair. Please set your difficulty right.\n\n>");

            }while((local_difficulty != 1 && local_difficulty != 2 && local_difficulty != 3 && local_difficulty<10) || local_difficulty>20);

            loop = NO;
            break;


        case CvC:

            /*-------------------- CvC --------------------*/

            *game_mode = CvC;
            *num_of_players = 1;
            printf("\n\t\tComputer vs Computer\n");

            sprintf(players_names[0], "Computer");

            do{

                printf("\nChoose the code length between 4 - 8.\n>");
                scanf("%d", &local_code_length);
                *code_length = local_code_length;

                if (local_code_length<4 || local_code_length>8)
                    printf("\nHey, the code length is wrong! :(\n\n");

            }while(local_code_length<4 || local_code_length>8);

            do{

                printf("\nNow, choose the maximum of tries to the PC against itself.\n>");
                scanf(" %d", &local_difficulty);
                *difficulty = local_difficulty;

                if (local_difficulty != 1 && local_difficulty != 2 && local_difficulty != 3 && local_difficulty<10)
                    printf("\nI'm not a robot... Wait... I am! But I can't do this in such few tries. Be nice and pick another number.\n\n>");
                if (local_difficulty>20)
                    printf("\nThis make things unfair. Please set your difficulty right.\n\n>");

            }while((local_difficulty != 1 && local_difficulty != 2 && local_difficulty != 3 && local_difficulty<10) || local_difficulty>20);

            do{

                printf("\nHow many random tries? Don't forget that much you give, early we guess.\n>");
                scanf("%d", &local_rand_tries);
                *num_rand_tries = local_rand_tries;

                if (local_rand_tries>local_difficulty)
                    printf("\nThis make no sense. You need to pick a number smaller than the general tries.\n\n");

            }while(local_rand_tries>local_difficulty);

            do{

                printf("\nIt's the last question, I promise. Can we repeat colors? (Use just Y/N)\n\n>");
                scanf(" %c", &local_color_repeat);
                //printf("\n%c\n", local_color_repeat);
                *color_repeat = local_color_repeat;

                if (local_color_repeat != 'Y' && local_color_repeat != 'N' && local_color_repeat != 'y' && local_color_repeat != 'n')
                    printf("\nI think you hit the wrong key! Be sure of typing just Y/N.\n");

            }while(local_color_repeat != 'Y' && local_color_repeat != 'N' && local_color_repeat != 'y' && local_color_repeat != 'n');

            printf("\nMaybe I've lied... Now you need to choose the difficulty (Or enter your own maximum of attempts between 10 - 20, and again: this definition is for everyone):\n\n1 - Easy\t\t2 - Normal\t\t3 - Hard\n\n>");

            loop = NO;
            break;

        }

    }while(loop == YES);

    if(local_difficulty>9 || local_difficulty<21)
        line_num = local_difficulty;
    if (local_difficulty == 1)
        line_num = local_code_length + 8;
    if (local_difficulty == 2)
        line_num = local_code_length + 6;
    if (local_difficulty == 3)
        line_num = local_code_length + 4;

    return line_num;

}

void BoardPrinting(int line_num, int code_length, char content[16][16], char players_names[20], int game_mode){

    int i, j, h=0;
    char printed_color = 0;

    CLEAR_SCR;
    printf("\n\n\t\t\tPLAYER: %s\n", players_names);
    printf("\n\t");

    for (j=0; j<code_length; j++) //Imprime linha superior da primeira parte do tabuleiro
            printf("  %d  ",j+1);

    printf("\t\t");

    for (j=0; j<code_length; j++) //Imprime linha superior segunda parte do tabuleiro
            printf("  %d  ", j+1);

    printf("\n\t");

    for (j=0; j<code_length; j++) //Imprime linha superior da primeira parte do tabuleiro
            printf("_____");

    printf("\t\t");

    for (j=0; j<code_length; j++) //Imprime linha superior segunda parte do tabuleiro
            printf("_____");

    printf("\n");

    for (i=0; i<line_num; i++){

            printf("\t");

            for (j=0; j<code_length; j++){ //Espaco das cores escolhidas

                printf("|");

                switch (content[i][j]){ //Condicional para imprimir as cores no devido espaço
                case 0:
                    printed_color = ' ';
                    break;
                case 'A':
                    printed_color = 'A';
                    RED;
                    break;
                case 'B':
                    printed_color = 'B';
                    GREEN;
                    break;
                case 'C':
                    printed_color = 'C';
                    BLUE;
                    break;
                case 'D':
                    printed_color = 'D';
                    YELLOW;
                    break;
                case 'E':
                    printed_color = 'E';
                    PURPLE;
                    break;
                case 'F':
                    printed_color = 'F';
                    GREY;
                    break;
                case 'G':
                    printed_color = 'G';
                    WHITE;  // BLACK IN LINUX
                    break;
                case 'H':
                    printed_color = 'H';
                    GOLD;
                    break;
                case 'I':
                    printed_color = 'I';
                    DARK_CYAN;
                    break;
                case 'J':
                    printed_color = 'J';
                    CYAN;
                    break;
                case 'K':
                    printed_color = 'K';
                    PINK;
                    break;
                case 'L':
                    printed_color = 'L';
                    SILVER;
                    break;
                default:
                    printf("\nOops! Something went wrong! We are very sorry... :( Please restart the game!");
                    exit(0);
                }

                printf(" %c ", printed_color);
                DEFAULT_TXT;
                printf("|");
            }

            printf("\t\t");

            for(h=0; h<code_length; h++){  // Espaco das dicas
                    printf("|");

                    switch (content[i][h+8]){  // Condicional para imprimir as cores no devido espaço
                case 0:
                    printed_color = ' ';
                    break;
                case 'W':
                    printed_color = 'P';
                    WHITE;
                    break;
                case 'B':
                    printed_color = 'B';
                    RED;
                    break;
                }

                    printf(" %c ", printed_color);
                    DEFAULT_TXT;
                    printf("|");
                }

            printf("\n\t");

            for (j=0; j<code_length; j++)
                printf("-----");

            printf("\t\t");

            for (j=0; j<code_length; j++)
                printf("-----");

            printf("\n");
    }

    printf("\t  A    B    C    D    E    F\n");
    h = 0;

    for (i =0; i<2; i++){ //Imprime tabela de cores para escolher

        printf("\t");

        for (j =0; j<6; j++){
            printf("|");

            switch (i+j+h){ //Condicional para imprimir as cores no devido espaço
            case 0:
                printed_color = 'A';
                RED; //RED
                break;
            case 1:
                printed_color = 'B';
                GREEN; //GREEN
                break;
            case 2:
                printed_color = 'C';
                BLUE; //BLUE
                break;
            case 3:
                printed_color = 'D';
                YELLOW; //YELLOW
                break;
            case 4:
                printed_color = 'E';
                PURPLE; // PURPLE
                break;
            case 5:
                printed_color = 'F';
                GREY; //GREY
                break;
            case 7:
                printed_color = 'G';
                WHITE; //WHITE (BLACK IN LINUX)
                break;
            case 8:
                printed_color = 'H';
                GOLD; //GOLD
                break;
            case 9:
                printed_color = 'I';
                DARK_CYAN; //DARK CYAN
                break;
            case 10:
                printed_color = 'J';
                CYAN; //CYAN
                break;
            case 11:
                printed_color = 'K';
                PINK; //PINK
                break;
            case 12:
                printed_color = 'L';
                SILVER; //SILVER
                break;
            default:
                printf("\nOops! Something went wrong! We are very sorry... :( Please restart the game!");
                exit(0);
            }

            printf(" %c ", printed_color);
            DEFAULT_TXT;
            printf("|");
        }

        printf("\n");
        h = 6;
    }

    printf("\t  G    H    I    J    K    L\n");

}

int GameProcessing(int game_mode, int num_of_players, int code_length, char color_repeat, int difficulty, int max_time, int line_num, int mods, char players_names[5][20], int num_random_tries){

    int i, j, h, k, aux, attempts={0}, hits={0}, game_status, whites=0, blacks=0, repeats, counter, num_tries=0;
    int players_results[5] = {0};
    char code[8] = {0}, user_input[20] = {0}, local_board[16][16] = {0}, tries_board[16][8]={0}, hits_board[16][5]={0}, pc_try1[8], pc_try2[8], pc_guess[4] = "AAAA";
    char attempts_saver[20][2][8] = {{0},{0}};
    time_t start_count;
    double elapsed_time = 0;
    //reply *answer;

    switch (game_mode){

    case Single_player:

        /*-------------------- SINGLE PLAYER --------------------*/

        for (i=0; i<code_length; i++)  // Sorteia os numeros das cores
                code[i] = (rand() % 12) + 'A';  // Numeros de 1 a 12

        if (color_repeat == 'N' || color_repeat == 'n')
            for(i=0; i<code_length; i++)
                for(j=0; j<code_length; j++)
                    if(code[i] == code[j] && i!=j){
                        code[i] = (rand() % 12) +'A'; //Numeros de 1 a 12
                        i=0;
                    }

        //printf("\nMax time: %d", max_time);

        for(i=0; i<line_num; i++){  // Conta o numero de tentativas disponiveis

            attempts++;

            if (mods == 1){  // Mostra a chave se o comando "/godmode" for utilizado
                printf("\n\nCode: ");

                for(j=0; j<8; j++)
                    printf("%c ", code[j]);

                printf("\n");

            }

            printf("\n\tElapsed time: %.3f\t\t\tP%dB%d\n", elapsed_time, blacks, whites);

            whites = 0;
            blacks = 0;

            start_count = time(NULL);

            printf("\nInsert here your color code:\n>");
            scanf(" %s", &user_input);

            if (user_input[0] == '/')
                CommandDetection(user_input, 0);

            h=0;

            for (j=0; j<code_length; j++) { // Copia o vetor color_input para a tabela de conteudo do tabuleiro
                local_board[i][j] = user_input[j];
                tries_board[i][j] = user_input[j];
            }

            for (j=0; j<code_length; j++){

                for (h=0; h<code_length; h++)
                    if (user_input[j] == code[h])
                        local_board[i][j+8] = RC_WP; // +8 para que os 8 ultimos digitos da tabela sejam os pinos brancos e vermelhos

                if (user_input [j] == code[j])
                    local_board [i][j+8] = RC_RP;
                }

            for (j=0; j<code_length; j++){

                    if(local_board[i][j+8] == RC_RP){
                        hits = hits+1;
                        blacks++;
                    }
                    if(local_board[i][j+8] == RC_WP)
                        whites++;
            }

            sprintf(hits_board[i], "P%dB%d", blacks, whites);

            printf("\n");

            /*for (j=0; j<16; j++){
                    for (h=0; h<8; h++){
                        printf("%d", local_board[j][h]);
                    }
                    printf("\t");
                    for (h=0; h<8; h++){
                        printf("%d", local_board[j][h+8]);
                    }
                    printf("\n");
            }*/

            elapsed_time = ((double)(time(NULL)-start_count)) + elapsed_time;

            if (elapsed_time >= max_time && max_time != 0){
                printf("\nOh! That's so bad! You have reached the time. :(\n");
                break;
            }

            if (hits == code_length){
                game_status = WON_GAME;
                BoardPrinting(line_num, code_length, local_board, 0, 0);
                break;
            }

            hits = 0;

            BoardPrinting(line_num, code_length, local_board, players_names[0], game_mode);

        }

        if (i==line_num)
            game_status = LOST_GAME;

        TheRegister(APPEND, players_names[0], code_length, color_repeat, code, game_mode, 12, attempts, elapsed_time, tries_board, hits_board);

        break;



    case Multiplayer:

        /*-------------------- MULTIPLAYER --------------------*/

        for(k=0; k<num_of_players; k++){

            for(i=0; i<17; i++)
                for(j=0; j<17; j++)
                    local_board[i][j]=NULL;

            elapsed_time = 0;

            for (i=0; i<code_length; i++)  // Sorteia os numeros das cores
                code[i] = (rand() % 12) + 'A';  // Numeros de 1 a 12

            if (color_repeat == 'N' || color_repeat == 'n')
                for(i=0; i<code_length; i++)
                    for(j=0; j<code_length; j++)
                        if(code[i] == code[j] && i!=j){
                            code[i] = (rand() % 12) +'A'; //Numeros de 1 a 12
                            i=0;
                        }

            //printf("\nMax time: %d", max_time);

            for(i=0; i<line_num; i++){  // Conta o numero de tentativas disponiveis

                attempts++;

                if (mods == 1){  // Mostra a chave se o comando "/godmode" for utilizado
                    printf("\n\nCode: ");

                for(j=0; j<8; j++)
                    printf("%c ", code[j]);

                printf("\n");

                }

                printf("\n\tElapsed time: %.3f\t\t\tP%dB%d\n", elapsed_time, blacks, whites);

                whites = 0;
                blacks = 0;

                start_count = time(NULL);

                printf("\nPlayer %s, please insert here your color code:\n>", players_names[k]);
                scanf(" %s", &user_input);

                if (user_input[0] == '/')
                    CommandDetection(user_input, 0);

                h=0;

                for (j=0; j<code_length; j++) { // Copia o vetor color_input para a tabela de conteudo do tabuleiro
                    local_board[i][j] = user_input[j];
                    tries_board[i][j] = user_input[j];
                }

                for (j=0; j<code_length; j++){

                    for (h=0; h<code_length; h++)
                        if (user_input[j] == code[h])
                            local_board[i][j+8] = RC_WP; // +8 para que os 8 ultimos digitos da tabela sejam os pinos brancos e vermelhos

                    if (user_input [j] == code[j])
                        local_board [i][j+8] = RC_RP;
                    }

                for (j=0; j<code_length; j++){

                    if(local_board[i][j+8] == RC_RP){
                        hits = hits+1;
                        blacks++;
                    }

                    if(local_board[i][j+8] == RC_WP)
                        whites++;
                }

                sprintf(hits_board[i], "P%dB%d", blacks, whites);

                printf("\n");

                /*for (j=0; j<16; j++){
                    for (h=0; h<8; h++){
                        printf("%d", local_board[j][h]);
                    }
                    printf("\t");
                    for (h=0; h<8; h++){
                        printf("%d", local_board[j][h+8]);
                    }
                    printf("\n");
                }*/

                elapsed_time = ((double)(time(NULL)-start_count)) + elapsed_time;

                if (elapsed_time >= max_time && max_time != 0){
                    printf("\nOh! That's so bad! You have reached the time. :(\n");
                    break;
                }

                if (hits == code_length){
                    game_status = WON_GAME;
                    BoardPrinting(line_num, code_length, local_board, 0, 0);
                    break;
                }

                hits = 0;

                BoardPrinting(line_num, code_length, local_board, players_names[k], game_mode);

            }

            players_results[k] = i+1;

            TheRegister(APPEND, players_names[k], code_length, color_repeat, code, game_mode, 12, attempts, elapsed_time, tries_board, hits_board);

        }

        for(i=0; i<9; i++)
            printf("%d ", players_results[i]);

        for (i=0; i<num_of_players; i++){
            for (j=0; j<num_of_players; j++){
                if(players_results[j]>players_results[i]){
                    aux = players_results[j];
                    players_results[j] = players_results[i];
                    players_results[i] = aux;
                }
            }
        }

        for(i=0; i<9; i++)
            printf("%d ", players_results[i]);

        break;

    case PvP:

        /*-------------------- PLAYER VS PLAYER --------------------*/

        break;

    case CvP:

        /*-------------------- COMPUTER VS PLAYER --------------------*/



        break;

    case CvC:

        /*-------------------- COMPUTER VS COMPUTER --------------------*/

        if(color_repeat == 'N' || color_repeat == 'n')
            repeats = 0;
        if(color_repeat =='Y' || color_repeat == 'y')
            repeats = 1;

        //activate_oracle(code_length, 12, repeats);

        //generate_key(mods);

        //printf("\nChave gerada\n");

        for(i=0; i<num_random_tries; i++){
            for (j=0; j<code_length; j++)
                pc_try1[j] = (rand() % 12) + 'A';

            //printf("\nTry: %s", pc_try1);

            //answer = validate_key(pc_try1);
            num_tries++;

            strcpy(attempts_saver[i][0], pc_try1);
            //sprintf(attempts_saver[i][1], "P%dB%d", get_blacks(answer), get_whites(answer));

            for (j=0; j<2; j++){
                printf("%s", attempts_saver[i][j]);
                printf("\t");
            }

            printf("\n");

        }

        if (strcmp(pc_guess, "LLLLLLLL") == code_length){
            printf("\nOh oh! Houston, we have a problem! I'm really sorry, bu you need to restart the game.\n");
            exit(0);
        }

        i=0;

        printf("\ntentativa %d", h);

        while(k!=num_tries){

            while(pc_guess[code_length-1] < 'L'){

                //printf("\n%s", pc_guess);

                k=0;

                for(i=0; i<num_tries; i++){
                    //answer = compare_keys(pc_guess, attempts_saver[i][0]);

                    /*if((atoi(&attempts_saver[i][1][1]) == get_blacks(answer)) && (atoi(&attempts_saver[i][1][3]) == get_whites(answer))){

                        k++;
                    }*/

                    if(k==num_tries){

                        sprintf(attempts_saver[k][0], "%s", pc_guess);

                        //answer = validate_key(pc_guess);
                        //printf("\nPC GUESS CERTA  %s\n", pc_guess);

                        //sprintf(attempts_saver[k][1], "P%dB%d", get_blacks(answer), get_whites(answer));

                        break;

                    }
                }

                /*if(get_blacks(attempts) == 4){

                    break;
            }

            if((atoi(&attempts_saver[i][1][1]) == get_blacks(answer)) && (atoi(&attempts_saver[i][1][3]) == get_whites(answer)) && k==num_tries){
                num_tries++;
                printf("\n%s", pc_guess);
                break;
            }*/

                pc_guess[code_length-1]++;
                //printf("\nIncrementando PC GUESS %s", pc_guess);
            }

            counter = 0;

            printf("\n%s", pc_guess);

            while(pc_guess[(code_length-1)- counter] == 'L')
                counter++;

            //printf("\nCounter: %d", counter);
            pc_guess[(code_length-1)-counter]++;
            //printf("\nPC guess: %s", pc_guess);


            //if((atoi(&attempts_saver[i][1][1]) == get_blacks(answer)) && (atoi(&attempts_saver[i][1][3]) == get_whites(answer)) && k==num_tries)
                //break;

            for(i=1; i<=counter; i++)
                pc_guess[((code_length-1) - counter) + i] = 'A';

            //printf("\nZerado: %s", pc_guess);

        }

        printf("\n\n");

        for(i=0; i<20; i++){
            for (j=0; j<2; j++){
                printf("%s", attempts_saver[i][j]);
                printf("\t");
            }

            printf("\n");

        }

        break;

    default:

        printf("\nOops! Something went really wrong! We are very sorry... :( Please restart the game!");
        exit(0);

    }

    return game_status;

}

int main(){

    int i, j, loop = YES, input_length, game_mode, num_of_players, code_length, difficulty, max_time, line_num, game_status, mods, num_rand_tries;
    int board_content[16][16] = {0};
    char color_repeat;
    char user_input[20], players_names[5][20] = {0, 0, 0, 0, 0};
    FILE *init;

    srand(time(NULL));
    printf("Type '/help' to know commands and use instructions.\n\n\n\tWecolme to\n");
    printf("  _______  _______  _______ _________ _______  _______  _______ _________ _        ______   _______\n (       )(  ___  )(  ____ \\\\__   __/(  ____ \\(  ____ )(       )\\__   __/( (    /|(  __  \\ (  ____ \\\n | () () || (   ) || (    \\/   ) (   | (    \\/| (    )|| () () |   ) (   |  \\  ( || (  \\  )| (    \\/\n | || || || (___) || (_____    | |   | (__    | (____)|| || || |   | |   |   \\ | || |   ) || (_____\n | |(_)| ||  ___  |(_____  )   | |   |  __)   |     __)| |(_)| |   | |   | (\\ \\) || |   | |(_____  )\n | |   | || (   ) |      ) |   | |   | (      | (\\ (   | |   | |   | |   | | \\   || |   ) |      ) |\n | )   ( || )   ( |/\\____) |   | |   | (____/\\| ) \\ \\__| )   ( |___) (___| )  \\  || (__/  )/\\____) |\n |/     \\||/     \\|\\_______)   )_(   (_______/|/   \\__/|/     \\|\\_______/|/    \\_)(______/ \\_______)\n");
    printf("\n\tPress Enter to start!\n");

    do{

        gets(user_input);
        input_length = strlen(user_input);

        if(input_length == 0)  // Enter foi apertado sem entrada de texto
            break;

        else{
            if(user_input[0] == '/')
                // Detecta comando
                loop = CommandDetection(user_input, &mods);
            else{
                printf("Hey, that's a unknown command! Please, type '/help' to see the valid commands list!\n");
                loop = YES;
            }
        }

    }while(loop == YES);

    if(mods != 2)
        /*A função "GameSetup" retorna o valor de número de linhas que é necessário ser impresso no tabuleiro, que por consequência
        é também o número máximo de tentativas.*/
        line_num = GameSetup(&game_mode, &num_of_players, &code_length, &color_repeat, &players_names, &difficulty, &max_time, mods, &num_rand_tries);

    if(mods == 2){

        init = fopen("init.dat", "r");

        fscanf(init, "%s\n%d\n%c\n%d\n%d", &players_names[0], &code_length, &color_repeat, &num_rand_tries, &line_num);

        printf("%s\n%d\n%c\n%d\n%d", players_names[0], code_length, color_repeat, num_rand_tries, line_num);

        game_mode = CvC;
        num_of_players = 1;

    }

    for(i=0;i<16;i++)
        for(j=0;j<16;j++)
            board_content[i][j] = NULL;


    /*A função "BoardPrinting" é do tipo void e não retorna qualquer valor. Sua única função é imprimir no terminal o tabuleiro
    do jogo de modo a deixá-lo mais divertido e fácil de jogar.*/
    BoardPrinting(line_num, code_length, board_content, players_names, game_mode);


    /*A função "GameProcessing" retorna o status (ganho ou perdido) do jogo, também realiza alterações em variáveis lidas e
    impressas no Registro (TheRegister) e no tabuleiro (Board).*/
    game_status = GameProcessing(game_mode, num_of_players, code_length, color_repeat, difficulty, max_time, line_num, mods, players_names, num_rand_tries);


    if(game_status == LOST_GAME)
        printf("\nUh! That's bad! You lose! Type '/retry' to try again or '/exit' to end the game...\n");
    if(game_status == WON_GAME){
        printf("\n\n\tCongratulations! You win! Press any key and then press 'Enter' to restart the game or type '/exit'\n");
        printf("\n\t    '._==_==_=_.'\n            .-\\:      /-.\n           | (|:.     |) |\n            '-|:.     |-'\n              \\::.    /\n               '::. .'\n                 ) (\n               _.' '._\n              `""""""""");
    }

    /*Este bloco apenas imprime variaveis para identificação de possíveis erros.
    for (i=0; i<5;i++)
                printf("\nPlayers Names %d: %s", i+1, players_names[i]);

    printf("\nGame Mode: %d", game_mode);
    printf("\nNumber of players: %d", num_of_players);
    printf("\nCode Length: %d", code_length);
    printf("\nColor repeat: %c", color_repeat);
    printf("\nDifficulty: %d", difficulty);
    printf("\nNumber of lines: %d", line_num);*/

    return 0;

}
