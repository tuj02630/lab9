#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LIMIT_TO_READING_LINES 233
#define NUMBER_OF_QUESTIONS_PER_STUDENT 11
#define ERROR_CODE_IN_INPUT_SCANNING 'e'
#define MAX_NAME_LENGTH 32

typedef struct {
        char * delim;
        unsigned int rows;
        unsigned int cols;
        char ** table;
} CSV;

int csv_destroy(CSV * csv) {
        if (csv == NULL) { return 0; }
        if (csv->table != NULL) { free(csv->table); }
        if (csv->delim != NULL) { free(csv->delim); }
        free(csv);
        return 0;
}
CSV * csv_create(unsigned int cols, unsigned int rows) {
        CSV * csv;

        csv = malloc(sizeof(CSV));
        csv->rows = rows;
        csv->cols = cols;
        csv->delim = strdup(",");

        csv->table = malloc(sizeof(char *) * cols * rows);
        if (csv->table == NULL) { goto error; }

        memset(csv->table, 0, sizeof(char *) * cols * rows);

        return csv;

        error:
                csv_destroy(csv);
                return NULL;
}
char * csv_get(CSV * csv, unsigned int col, unsigned int row) {
        unsigned int idx;
        idx = col + (row * csv->cols);
        return csv->table[idx];
}
int csv_set(CSV * csv, unsigned int col, unsigned int row, char * value) {
        unsigned int idx;
        idx = col + (row * csv->cols);
        csv->table[idx] = value;
        return 0;
}
int csv_save(CSV * csv, char * filename) {
        FILE * fp;
        int row, col;
        char * content;

        fp = fopen(filename, "w");
        for (row=0; row<csv->rows; row++)
        {
                for (col=0; col<csv->cols; col++)
                {
                        content = csv_get(csv, col, row);
                        fprintf(fp, "%s%s", content,
                        ((col == csv->cols-1) ? "" : csv->delim) );
                }
                fprintf(fp, "\n");
        }

        fclose(fp);
        return 0;
}
void csv_display(CSV * csv) {
        int row, col;
        char * content;

        if ((csv->rows == 0) || (csv->cols==0))
        {
                printf("[Empty table]\n");
                return ;
        }
        printf("\n[Table cols=%d rows=%d]\n", csv->cols, csv->rows);
        for (row=0; row<csv->rows; row++)
        {
                printf("[|");
                for (col=0; col<csv->cols; col++)
                {
                        content = csv_get(csv, col, row);
                        printf("%s\t|", content);
                }
                printf("]\n");
        }
        printf("\n");
}
int main()
{
        int i = 0;
        int j = 0;
        int fork = 0;
        int count = 0;
        int tempGrade = 0;
        char tempAnswer[1];
        char tempChar;
        char tempAns[2];
        char tempID[6];
        char tempName[MAX_NAME_LENGTH];
        char tempScore[NUMBER_OF_QUESTIONS_PER_STUDENT];
        int ids1[LIMIT_TO_READING_LINES];
        int ids2[LIMIT_TO_READING_LINES];
        char idArr[LIMIT_TO_READING_LINES][6];
        char nameArr[LIMIT_TO_READING_LINES][MAX_NAME_LENGTH];
        char strAnsArr[LIMIT_TO_READING_LINES][NUMBER_OF_QUESTIONS_PER_STUDENT][2];
        char correctArr[NUMBER_OF_QUESTIONS_PER_STUDENT] = {'a','d','d','a','b','b','c','a','d','a','d'};
        char gradeArr[LIMIT_TO_READING_LINES][6];
        CSV * csv;

        FILE *IN1 = fopen("newomr.txt", "r");
        FILE *IN2 = fopen("test_taker_names.txt", "r");
        csv = csv_create(14, LIMIT_TO_READING_LINES+1);
        csv_set(csv, 0,0, "Identity");
        csv_set(csv, 1,0, "Name");
        csv_set(csv, 2,0, "Q1");
        csv_set(csv, 3,0, "Q2");
        csv_set(csv, 4,0, "Q3");
        csv_set(csv, 5,0, "Q4");
        csv_set(csv, 6,0, "Q5");
        csv_set(csv, 7,0, "Q6");
        csv_set(csv, 8,0, "Q7");
        csv_set(csv, 9,0, "Q8");
        csv_set(csv, 10,0, "Q9");
        csv_set(csv, 11,0, "Q10");
        csv_set(csv, 12,0, "Q11");
        csv_set(csv, 13,0, "Grade");

        while(fscanf(IN1, "%d", &ids1[i]) == 1)
        {       
                if(i >= LIMIT_TO_READING_LINES)
                {
                        break;
                }
                for ( fork = 0; fork < NUMBER_OF_QUESTIONS_PER_STUDENT; fork++ )
                {
                        fscanf( IN1, "%c", &tempScore[fork]);
                }
                j = 0;
                while(fscanf(IN2, "%d", &ids2[j]) == 1)
                {
                        if(j >= LIMIT_TO_READING_LINES)
                        {
                                break;
                        }
                        for ( fork = 0; fork < MAX_NAME_LENGTH; fork++ )
                        {
                                fscanf( IN2, "%c", &tempChar );
                                if(tempChar == '\n')
                                {
                                        tempName[fork] = '\0';
                                        break;
                                }
                                tempName[fork] = tempChar;
                        }
                        if(ids1[i] == ids2[j])
                        {
                                tempGrade = 0;
                                sprintf(tempID, "%5.5d", ids1[i]);
                                tempID[5] = '\0';
                                for(fork = 0; fork < 6; fork++)
                                {
                                        idArr[i][fork] = tempID[fork];
                                }
                                for(fork = 0; fork < MAX_NAME_LENGTH; fork++)
                                {
                                        nameArr[i][fork] = tempName[fork];
                                        if(tempName[fork] == '\0')
                                        {
                                                break;
                                        }
                                }
                                for(fork = 0; fork < NUMBER_OF_QUESTIONS_PER_STUDENT; fork++)
                                {

                                        strAnsArr[i][fork][0] = tempScore[fork];
                                        strAnsArr[i][fork][1] = '\0';
                                        if(tempScore[fork] == correctArr[fork])
                                        {
                                                tempGrade+=100;
                                        }
                                }
                                sprintf(gradeArr[i],"%d%%\0", (tempGrade/NUMBER_OF_QUESTIONS_PER_STUDENT));
                                break;
                        }
                        j++;
                }
                i++;
        }
        for(fork = 0; fork < LIMIT_TO_READING_LINES; fork++)
        {
                csv_set(csv, 0, 1+fork, idArr[fork]);
                csv_set(csv, 1, 1+fork, nameArr[fork]);
                csv_set(csv, 13,1+fork, gradeArr[fork]);
                for(i = 0; i < NUMBER_OF_QUESTIONS_PER_STUDENT; i++)
                {
                        csv_set(csv,2+i, 1+fork,strAnsArr[fork][i]);
                }
        }               
        csv_display(csv);
        csv_save(csv, "full_data.csv");
        csv_destroy(csv);
}
