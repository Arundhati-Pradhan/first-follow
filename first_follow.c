#include<stdio.h>
//#include<conio.h>
#include<stdlib.h>
#include<string.h>

/* Terminals of the grammar */
char T[20];
/* Non Terminals of the grammar */
char NT[20];
/* Table consisting of the FIRST of allNon Terminals */
char FT[10][20];
/* Table consisting of the FIRST of all Terminals */
char FLWT[10][20];
/* Array consisting of productions of the grammar */
char P[20][10];
/* Number of Productions */
int NOP=0;
/* Number of Non Terminals */
int NONT;
/* Number of Terminals */
int NOT;
/* Function to read grammar from the user */
void getGrammar()
{
int i;
/* Production */
char prod[30];
printf("Enter the grammar and e for epsilon: \n");
//printf("Press Enter when completed::\n");
while(1)
{
gets(prod);
if(strcmp(prod,"")==0)
{
break;
}
strcpy(P[NOP++],prod);
}
}
/* Function to get all the terminals and Non terminals of the grammar */
void getTerminalsAndNonTerminals()
{
printf("Enter the Non Terminal symbols : \n");
//printf("Dont use any seperators in between::\n");
gets(NT);
NONT = strlen(NT);
printf("Enter the Terminal symbols : \n");
//printf("Dont use any seperators in between::\n");
gets(T);
strcat(T,"e");
NOT = strlen(T);
}
int getIndex(char A[],char c)
{
int i=0;
for(i=0;i<strlen(A);i++)
{
if(A[i]==c)
return i;
}
return -1;
}
void addEpsilon(char c)
{
FT[getIndex(NT,c)][strlen(T)-1]='e';
}
int hasEpsilon(char c)
{
if(FT[getIndex(NT,c)][strlen(T)-1]=='e')
return(1);
return(0);
}
int isNonTerminal(char c)
{
int i;
for(i=0;i<strlen(NT);i++)
{
if(NT[i]==c)
{
return 1;
}
}
return 0;
}
int isTerminal(char c)
{
int i=0;
for(i=0;i<strlen(T)-1;i++)
{
if(T[i]==c)
return 1;
}
return 0;
}

void ComputeFollow()
{
    int len,i,j,k,added=0;
    char sym1,sym2,elm1,elm2,sym;
    FLWT[getIndex(NT,'E')][5]='$';
    for(i=0;i<8;i++)
    {
        len=strlen(P[i]);
        for(j=3;j<len;j++)
        {
            sym1=P[i][j];
            sym2=P[i][j+1];
            if(isNonTerminal(sym1) && isTerminal(sym2))
            {
            FLWT[getIndex(NT,sym1)][getIndex(T,sym2)]=sym2;
            }
        }
    }
    for(i=0;i<8;i++)
    {
        len=strlen(P[i]);
        for(j=3;j<len;j++)
        {
            sym1=P[i][j];
            sym2=P[i][j+1];
            if(isNonTerminal(sym1) && isNonTerminal(sym2))
            {
                for(k=0;k<6;k++)
                {
                    elm1=FLWT[getIndex(NT,sym1)][k];
                    elm2=FT[getIndex(NT,sym2)][k];
                    if(elm1!=elm2)
                    {
                        if(elm2!='\0' && elm2!='e')
                        {
                        FLWT[getIndex(NT,sym1)][k]=elm2;
                        }
                    }
                }
            }
        }
    }
    for(i=0;i<8;i++)
    {
        sym1=P[i][strlen(P[i])-1];
        if(isNonTerminal(sym1))
        {
            sym2=P[i][0];
            for(k=0;k<6;k++)
            {
                elm1=FLWT[getIndex(NT,sym1)][k];
                elm2=FLWT[getIndex(NT,sym2)][k];
                if(elm1!=elm2)
                {
                    if(elm2!='\0' && elm2!='e')
                    {
                    added=1;
                    FLWT[getIndex(NT,sym1)][k]=elm2;
                    }
                }
            }
        }
        sym=P[i][strlen(P[i])-1];
        if(isNonTerminal(sym) && hasEpsilon(sym))
        {
            sym1=P[i][strlen(P[i])-2];
            if(isNonTerminal(sym1))
            {
                sym2=P[i][0];
                for(k=0;k<6;k++)
                {
                    elm1=FLWT[getIndex(NT,sym1)][k];
                    elm2=FLWT[getIndex(NT,sym2)][k];
                    if(elm1!=elm2)
                    {
                        if(elm2!='\0' && elm2!='e')
                        {
                        added=1;
                        FLWT[getIndex(NT,sym1)][k]=elm2;
                        }
                    }
                }
            }
            if(i==7)
            {
                if(added)
                {
                i=-1;
                added=0;
                }
            }
        }
    }
}

void computeFirst()
{
int added=0,i,j,k;
char X,elm1,elm2;
//clrscr();
for(i=0;i<NONT;i++)
{
for(j=0;j<NOT;j++)
{
FT[i][j]='\0';
}
}
for(i=0;i<NOP;i++)
{
X = P[i][3];
if(X=='e')
{
addEpsilon(P[i][0]);
}
else if(isTerminal(X))
{
FT[getIndex(NT,P[i][0])][getIndex(T,X)]=X;
}
}
for(i=0;i<NOP;i++)
{
X = P[i][3];
if(isNonTerminal(X))
{
for(j=3;j<strlen(P[i]);j++)
{
X=P[i][j];
if(isTerminal(X))
{
FT[getIndex(NT,P[i][0])][getIndex(T,X)]=X;
break;
}
for(k=0;k<NOT;k++)
{
elm1=FT[getIndex(NT,P[i][0])][k];
elm2=FT[getIndex(NT,X)][k];
if((elm1!=elm2) && elm2!='e')
{
if(elm2!='\0')
{
added=1;
FT[getIndex(NT,P[i][0])][k]=elm2;
}
}
}
if(!hasEpsilon(X))
{
break;
}
}
if(j==strlen(P[i]))
{
added=1;
FT[getIndex(NT,P[i][0])][5]='e';
}
}
if(i==(NOP-1))
{
if(added)
{
i=-1;
added=0;
}
}
}
}
void main()
{
int i,j,k;
getGrammar();
getTerminalsAndNonTerminals();
computeFirst();
printf("\nGiven Grammar :\n");
for(i=0;i<NOP;i++)
{
printf("%s\n",P[i]);
}
printf("\nFIRST of given grammar : \n");
for(i=0;i<NONT;i++)
{
printf("FIRST(%c)={",NT[i]);
for(j=0;j<NOT;j++)
{
if(FT[i][j]!='\0')
printf("%c,",FT[i][j]);
}
printf("\b}\n");
}


ComputeFollow();
printf("\nFOLLOW of given grammar : \n");
for(i=0;i<NONT;i++)
{
printf("FOLLOW(%c)={",NT[i]);
for(j=0;j<NOT;j++)
{
if(FLWT[i][j]!='\0')
printf("%c,",FLWT[i][j]);
}
printf("\b}\n");
}

//getch();
}

