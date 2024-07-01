/*
 ============================================================================
 Name        : DictionarySystem.c
 Author      : Hussein_Awad
 Description : Your program should read a set of words and their relevant information from a
			   file named Words.txt. The user should be then able to enter words into the
			   dictionary with their relevant information.
 ============================================================================
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
//////////////////////////////////////////////////////////////////////STRUCT AVL NODE
typedef struct AVLnode *AVLNode;
struct AVLnode
{
    char Element[50];
    char meaning[150];
    char synonym[50];
    char antonym[50];
    AVLNode Left;
    AVLNode Right;
    int Height;
};
//////////////////////////////////////////////////////////////////////MAKE EMPTY FUNCTION
AVLNode MakeEmpty( AVLNode T )
{
    if( T != NULL )
    {
        MakeEmpty( T->Left );
        MakeEmpty( T->Right );
        free( T );
    }
    return NULL;
}
//////////////////////////////////////////////////////////////////////HRIGHT FUNCTION
int Height( AVLNode P )
{
    if( P == NULL )
        return -1;
    else
        return P->Height;
}
//////////////////////////////////////////////////////////////////////MAX FUNCTION
int Max( int Lhs, int Rhs )
{
    return Lhs > Rhs ? Lhs : Rhs;
}
//////////////////////////////////////////////////////////////////////ROTATE 1 LEFT
AVLNode SingleRotateWithLeft( AVLNode K2 )
{
    AVLNode  K1;
    K1 = K2->Left;
    K2->Left = K1->Right;
    K1->Right = K2;
    K2->Height = Max( Height( K2->Left ), Height( K2->Right ) ) + 1;
    K1->Height = Max( Height( K1->Left ), K2->Height ) + 1;
    return K1;  // New root
}
//////////////////////////////////////////////////////////////////////ROTATE 1 RIGHT
AVLNode SingleRotateWithRight( AVLNode K1 )
{
    AVLNode K2;
    K2 = K1->Right;
    K1->Right = K2->Left;
    K2->Left = K1;
    K1->Height = Max( Height( K1->Left ), Height( K1->Right ) ) + 1;
    K2->Height = Max( Height( K2->Right ), K1->Height ) + 1;
    return K2;  // New root
}
//////////////////////////////////////////////////////////////////////ROTATE 2 LEFT
AVLNode DoubleRotateWithLeft( AVLNode K3 )
{
    K3->Left = SingleRotateWithRight( K3->Left );
    return SingleRotateWithLeft( K3 );
}
//////////////////////////////////////////////////////////////////////ROTATE 2 RIGHT
AVLNode DoubleRotateWithRight( AVLNode K1 )
{
    K1->Right = SingleRotateWithLeft( K1->Right );
    return SingleRotateWithRight( K1 );
}
//////////////////////////////////////////////////////////////////////INSERT NODE
AVLNode Insert(char X[50],char M[150],char S[50],char A[50],AVLNode T)
{
    if( T == NULL )
    {
        T = malloc( sizeof( struct AVLnode ) );
        if( T == NULL )
            printf( "Out of space!!!" );
        else
        {
            strcpy(T->Element,X);
            strcpy(T->meaning,M);
            strcpy(T->synonym,S);
            strcpy(T->antonym,A);
            T->Height = 0;
            T->Left = T->Right = NULL;
        }
    }
    else if(strcmp(X,T->Element) < 0)
    {
        T->Left = Insert(X,M,S,A,T->Left);
        if( Height( T->Left ) - Height( T->Right ) == 2 )
        {
            if(strcmp(X,T->Left->Element) < 0)
                T = SingleRotateWithLeft( T );
            else
                T = DoubleRotateWithLeft( T );
        }
    }
    else if(strcmp(X,T->Element) > 0)
    {
        T->Right = Insert( X,M,S,A,T->Right );
        if( Height( T->Right ) - Height( T->Left ) == 2 )
        {
            if(strcmp(X,T->Right->Element) > 0)
                T = SingleRotateWithRight( T );
            else
                T = DoubleRotateWithRight( T );
        }
    }
    return T;
}
//////////////////////////////////////////////////////////////////////PRINT TREE
void PrintInOrder( AVLNode t)
{
    if( t != NULL)
    {
        PrintInOrder( t->Left );
        printf("%s\n",t->Element);
        PrintInOrder( t->Right );
    }
}
//////////////////////////////////////////////////////////////////////PRINT SYNONYM & ANTONYM TREE
void PrintInOrderSA( AVLNode t)
{
    if( t != NULL)
    {
        PrintInOrderSA( t->Left );
        printf("%s\n%s\n", t->synonym,t->antonym);
        PrintInOrderSA( t->Right );
    }
}
//////////////////////////////////////////////////////////////////////READ FILE.DATA
AVLNode File(AVLNode T)
{
    FILE *InPut;
    char *token;
    char Read[100];
    char E[50],M[150],S[50],A[50];
    InPut=fopen("Words.txt","r");
    while (fgets(Read,100,InPut)!= NULL)
    {
        token = strtok(Read,":/*");
        strcpy(E,token);
        token= strtok(NULL,":/*");
        strcpy(M,token);
        token= strtok(NULL,":/*");
        strcpy(S,token);
        token= strtok(NULL,":/*");
        strcpy(A,token);
        T = Insert(E,M,S,A,T);
    }
    return T;
}
//////////////////////////////////////////////////////////////////////FIND NODE
AVLNode Find( char X[50], AVLNode T )
{
    if( T == NULL )
        return NULL;
    if(strcmp(X,T->Element)<0)
        return Find( X, T->Left );
    else if(strcmp(X,T->Element)>0)
        return Find( X, T->Right );
    else
        return T;
}
//////////////////////////////////////////////////////////////////////LIST YOUR CHOOSEN
void ListData()
{
    printf("                            HELLO IN MY PROGRAM\n\n");
    printf("               1.Read Words file and create the dictionary\n");
    printf("               2.Insert a new word from user with all its associated data\n");
    printf("               3.Find a word and give the user the option to update the information\n");
    printf("               4.List words in the dictionary in lexicographic order\n");
    printf("               5.List all synonym and antonyms of a given word\n");
    printf("               6.List all words that start with the same first letter in lexicographic order\n");
    printf("               7.Delete a word from the dictionary\n");
    printf("               8.Delete all words that start with a specific letter\n");
    printf("               9.Save all words in file Dictionary.data\n");
    printf("               10.The security section of the program [Print hashed table]\n");
}
//////////////////////////////////////////////////////////////////////INSERT FROM USER NODE
AVLNode FromUser(AVLNode T)
{
    char E[50],M[150],S[50],A[50];
    printf("\n             PUT THE WORD YOU WANT TO ADD: ");
    scanf("%s",E);
    printf("\n             PUT THE MEANING OF THE WORD THERE ARE COMMAS BETWEEN THEM: ");
    printf("\n             NOTE: YOU CAN NOT ADD MORE THAN THREE MEANINGS\n");
    scanf("%s",M);
    printf("               PUT THE WORD SYNONYM: ");
    scanf("%s",S);
    printf("\n             PUT THE WORD ANTONYM: ");
    scanf("%s",A);
    T = Insert(E,M,S,A,T);
    return T;
}
//////////////////////////////////////////////////////////////////////FIND WORD AND UPDATE INFORMATION DATA
AVLNode FindWord(AVLNode T)
{
    char word[50];
    char key[4];
    char NewMeaning[150],NewSynonym[50],NewAntonyms[50];
    printf("               WHAT IS THE WORD THAT YOU WANT: ");
    scanf("%s",word);
    T = Find(word,T);
    printf("\n             IF THE WORD EXISTS,IT WILL APPEAR HERE,AND IF [NULL] APPEARS,IT DOES NOT EXIST: %s",T->Element);
    printf("\n             DO YOU WANT TO MODIFY THE WORD INFORMATION?");
    printf("\n             YES OR NO: ");
    scanf("%s",key);
    if (strcmp(key,"YES")==0 || strcmp(key,"yes")==0)
    {
        printf("\n             THE NEW MEANINGS OF THE WORD THERE ARE COMMAS BETWEEN THEM: ");
        scanf("%s",NewMeaning);
        strcpy(T->meaning,NewMeaning);
        printf("\n             THE NEW SYNONYM IS : ");
        scanf("%s",NewSynonym);
        strcpy(T->synonym,NewSynonym);
        printf("\n             THE NEW ANTONYM IS : ");
        scanf("%s",NewAntonyms);
        strcpy(T->antonym,NewAntonyms);
        printf("%s  %s  %s",T->meaning,T->synonym,T->antonym);
    }
    if(strcmp(key,"NO")==0 || strcmp(key,"no")==0)
    {
        printf("\n             THE WORD INFORMATION IS SAME:)");
    }
    return T;
}
//////////////////////////////////////////////////////////////////////GET BALANVCE FOR TREE
int getBalance(AVLNode T)
{
    if (T == NULL)
        return 0;
    return Height(T->Left) - Height(T->Right);
}
//////////////////////////////////////////////////////////////////////MIN VALUE NODE
AVLNode minValueNode(AVLNode T)
{
    AVLNode C = T;

    while (C->Left != NULL)
        C = C->Left;
    return C;
}
//////////////////////////////////////////////////////////////////////DELETE NODE
AVLNode deleteNode(char key[50],AVLNode T)
{
    if (T== NULL)
        return T;

    if (strcmp(key,T->Element)<0)
        T->Left=deleteNode(key,T->Left);

    else if(strcmp(key,T->Element)>0)
        T->Right = deleteNode(key,T->Right);

    else
    {
        if( (T->Left == NULL) || (T->Right == NULL) )
        {
            AVLNode temp = T->Left ? T->Left : T->Right;

            if (temp == NULL)
            {
                temp = T;
                T = NULL;
            }
            else
                *T = *temp;
            free(temp);
        }
        else
        {
            AVLNode temp = minValueNode(T->Right);
            strcpy(T->Element,temp->Element);
            T->Right = deleteNode(temp->Element,T->Right);
        }
    }
    if (T == NULL)
        return T;

    T->Height = 1 + Max(Height(T->Left),Height(T->Right));
    int balance = getBalance(T);

    if (balance > 1 && getBalance(T->Left) >= 0)
        return SingleRotateWithRight(T);

    if (balance > 1 && getBalance(T->Left) < 0)
    {
        T->Left = SingleRotateWithLeft(T->Left);
        return SingleRotateWithRight(T);
    }
    if (balance < -1 && getBalance(T->Right) <= 0)
        return SingleRotateWithLeft(T);

    if (balance < -1 && getBalance(T->Right) > 0)
    {
        T->Right = SingleRotateWithRight(T->Right);
        return SingleRotateWithLeft(T);
    }
    return T;
}
//////////////////////////////////////////////////////////////////////DELETE TREE[ALL NODES]
void deleteTree(AVLNode node)
{
    if (node == NULL) return;
    deleteTree(node->Left);
    deleteTree(node->Right);
    printf("\n             Deleting node: %s", node->Element);
    free(node);
}
//////////////////////////////////////////////////////////////////////PRINT TREE TO DICTIONARY FILE
void export_recursive(FILE* fp, AVLNode root)
{
    if(root!=NULL)
    {
        export_recursive(fp,root->Left);
        fprintf(fp,"%s: %s / %s * %s",root->Element,root->meaning,root->synonym,root->antonym);
        export_recursive(fp,root->Right);
    }
}
void DictionaryFile (AVLNode root)
{
    FILE *fp;
    fp=fopen("Dictionary.txt","w");
    export_recursive(fp, root);
    fclose(fp);
}
//////////////////////////////////////////////////////////////////////CLEAR SCREEN OUTPUT
void ClearScreen()
{
    printf("\n                 PUT [LIST] TO RETURN THE LIST : ");
    char ch[20];
    scanf("%s",ch);
    if(strcmp(ch,"LIST")==0||strcmp(ch,"list")==0)
    {
        system("cls");
        ListData();
    }
    else
        printf("\n                 ERORR: PUT THE WORD [LIST] TO RETURN THE LIST ^-^");
}
//////////////////////////////////////////////////////////////////////HASH #1
unsigned int Hash1(char* key,unsigned int TableSize)
{
    unsigned int hashValue = 0;
    while( *key != '\0' )
    {
        hashValue = ( hashValue << 5 ) + *key++;
    }
    return (hashValue % TableSize);
}
//////////////////////////////////////////////////////////////////////HASH #2
int Hash2(char*key,int TableSize)
{
    int pr,val;
    pr = primeNumber(TableSize);
    val= pr-(Hash1(key,TableSize)%pr);
    return val;
}
//////////////////////////////////////////////////////////////////////COLLISION!
int collision(char*key,int TableSize,int i)
{
    int fnl;
    fnl = (Hash1(key,TableSize)+ i * Hash2(key,TableSize))%TableSize;
    return fnl;
}
//////////////////////////////////////////////////////////////////////SIZE FOR HASH TABLE
int HashTabeleSize()
{
    FILE *fp = fopen("Dictionary.txt","r");
    int ch;
    int count=0;
    do
    {
        ch = fgetc(fp);
        if(ch == '\n') count++;
    }
    while( ch != EOF );
    printf("Total number of lines [SIZE] : %d\n",count);
    return count;
}
//////////////////////////////////////////////////////////////////////CHECK NUMBER PRIME OR NOT
int prime(int n)
{
    int i;
    for(i=2; i<=n/2; i++)
    {
        if(n%i!=0)
            continue;
        else
            return 1;
    }
    return 0;
}
//////////////////////////////////////////////////////////////////////PRIME NUMBER LESS THAN SIZE TABLE
int primeNumber(int i)
{
    int p;
    p=prime(i);
    if(p==1)
    {
        i--;
        primeNumber(i);
    }
    if(p==0)
        return i;
}
//////////////////////////////////////////////////////////////////////INSERT HASH
int InsertHash(int Size,char WordsTable[Size][50])
{
    char *Read;
    char fRead[50];
    printf("enter the value : ");
    if(scanf("%s",fRead))
        Size = Size + 4;
    Read=fRead;
    int Index;
    Index = Hash1(Read,Size);
    for(int i=0; i<=Size; i++)
    {
        if(strlen(WordsTable[Index])==1)
        {
            strcpy(WordsTable[Index],Read);
            printf("%s =>> %d\n",WordsTable[Index],Index);
            break;
        }
        if(strlen(WordsTable[Index])!=1)
            Index = collision(Read,Size,i);
    }
    return Size;
}
//////////////////////////////////////////////////////////////////////SEARCH HASH
void SearchHash(int Size,char WordsTable[Size][50])
{
    char *Read;
    char fRead[50];
    printf("enter the value : ");
    scanf("%s",fRead);
    Read=fRead;
    int Index;
    Index = Hash1(Read,Size);
    int Test=1;
    for(int i=0; i<=Size; i++)
    {
        if(strlen(WordsTable[Index])!=1)
        {
            if(strcmp(fRead,WordsTable[Index])==0)
            {
                Test=2;
                printf("IT IS EXISTE");
                break;
            }
            else
            {
                Index = collision(Read,Size,i);
            }

        }

    }
    if(Test==1)
        printf("IT IS NOT EXISTE");

}
//////////////////////////////////////////////////////////////////////DELETE HASH
void DeleteHash(int Size,char WordsTable[Size][50])
{
    char *Read;
    char fRead[50];
    printf("enter the value : ");
    scanf("%s",fRead);
    Read=fRead;
    int Index;
    Index = Hash1(Read,Size);
    int Test=1;
    for(int i=0; i<=Size; i++)
    {
        if(strlen(WordsTable[Index])!=1)
        {
            if(strcmp(fRead,WordsTable[Index])==0)
            {
                Test=2;
                strcpy(WordsTable[Index],"");
                printf("IT IS DELETED");
                break;
            }
            else
            {
                Index = collision(Read,Size,i);
            }

        }

    }
    if(Test==1)
        printf("IT IS NOT EXISTE");

}
//////////////////////////////////////////////////////////////////////SAVE HASH
void SaveHash(int Size,char WordsTable[Size][50])
{
    FILE*SAV;
    SAV=fopen("Dicti.txt","w");
    int Index;
    Index=0;
    for(int i=0; i<=Size; i++)
    {
        if(strlen(WordsTable[Index])!=1)
        {
            fprintf(SAV,"%s=>>%d",WordsTable[Index],Index);
            Index++;
        }
    }
    printf("IT SAVED");
}
//////////////////////////////////////////////////////////////////////HASH TABLE FOR ALL
void Hash()
{
    int Size;
    Size = HashTabeleSize()+20;
    char WordsTable[Size][50];
    FILE*DIC;
    DIC=fopen("Dictionary.txt","r");
    int Index;
    char *Reader;
    char Line[100];
    while (fgets(Line,100,DIC)!= NULL)
    {
        Reader = strtok(Line,":");
        Index = Hash1(Reader,Size);
        for(int i=0; i<=Size; i++)
        {
            if(strlen(WordsTable[Index])==1)
            {
                strcpy(WordsTable[Index],Reader);
                printf("%s =>> %d\n",WordsTable[Index],Index);
                break;
            }
            if(strlen(WordsTable[Index])!=1)
                Index = collision(Reader,Size,i);
        }
        Reader = strtok(NULL,":");
    }
    int S=1;
    while(1==S)
    {
        printf("\nWELCOME TO THE SECURITY SECTION, CHOOSE FROM THE OPTIONS IN FRONT OF YOU\n");
        printf("1.Print the size of the hash table\n");
        printf("2.Print the function that was used\n");
        printf("3.Add a word to be hashed and add it to the table\n");
        printf("4.Search for a specific word.\n");
        printf("5.Delete a specific record.\n");
        printf("6.Save hash table back to file.\n");
        printf("7.Exit\n");
        int X;
        scanf("%d",&X);
        switch(X)
        {
        case 1:
            printf("THE SIZE OF HASH TABLE IS: %d",Size);
            ClearScreen();
            break;
        case 2:
            printf("Double Hashing [h(x) = h1(x) + i * h2(x)");
            printf("h2(x) = prime number � h1(x) mod prime number");
            ClearScreen();
            break;
        case 3:
            Size = InsertHash(Size,WordsTable);
            break;
        case 4:
            SearchHash(Size,WordsTable);
            break;
        case 5:
            DeleteHash(Size,WordsTable);
            break;
        case 6:
            SaveHash(Size,WordsTable);
            break;
        case 7:
            S=2;
            system("cls");
            ListData();
            break;
        }
    }
}
//////////////////////////////////////////////////////////////////////MAIN FUNCTION
int main()
{
    ListData();
    AVLNode T;
    T = MakeEmpty(NULL);
    T = File(T);
    while(1==1)
    {
        printf("\n               PIEASE CHOOSE FROM THE LIST IN FRONT OF YOU : ");
        int A;
        scanf("%d",&A);
        switch(A)
        {
        case 1:
            T = File(T);
            printf("                 DATA IS READED AND AVL TREE IS CREATED ._.\n");
            ClearScreen();
            break;
        case 2:
            FromUser(T);
            printf("                 IT IS INSERT\n");
            PrintInOrder(T);
            ClearScreen();
            break;
        case 3:
            FindWord(T);
            ClearScreen();
            break;
        case 4:
            PrintInOrder(T);
            ClearScreen();
            break;
        case 5:
            PrintInOrderSA(T);
            ClearScreen();
            break;
        case 6:
            break;
        case 7:
            printf("                 THE WORS YOU NEED DELETED IS:\n");
            char x[50];
            scanf("%s",x);
            T = deleteNode(x,T);
            PrintInOrder(T);
            ClearScreen();
            break;
        case 8:
            deleteTree(T);
            ClearScreen();
            break;
        case 9:
            DictionaryFile(T);
            printf("\n               DATA IS SAVED ._.\n");
            ClearScreen();
            break;
        case 10:
            printf("THE HASHING TABEL OF Dictionary.data IS:\n");
            Hash();
            break;
        }
    }
    return 0;
}