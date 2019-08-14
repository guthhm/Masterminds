#include "history.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define READ 0
#define APPEND 1
#define SORT 2

#define  Single_player 49   //ASCII code to '1'
#define  Multiplayer 50     //ASCII code to '2'
#define  PvP 51             //ASCII code to '3'
#define  CvP 52             //ASCII code to '4'
#define  CvC 53             //ASCII code to '5'

void GameHistory (int open_mode, int game_mode, int code_length, int num_colors, int code[12], int tries, int time_taken, int hits[6], int board[16][16], char players_names[20], char color_repeat){

    int i=0, j, aux=0, playerID_num, gen_playerID=0;
    char line[50] = {NULL}, info_str[50], cmp_str[20], talvez_funcione[50], playerID[5][4] ={{0}};
    char *token;
    char *array[10];

    FILE *historic;

    historic = fopen("historico.dat", "a+");

    if(!historic){
        printf("\nOh oh! It seems like an error... Please, try again...\n");
        exit(0);
    }

    if (open_mode == READ){

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
    if (open_mode == APPEND){

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

            switch (game_mode){

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

        if(game_mode == Single_player){

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
