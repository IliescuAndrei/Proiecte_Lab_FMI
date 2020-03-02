#include<stdio.h>
#include<stdlib.h>
 
typedef struct proces
{
    char nume[5];
    int timpOcupare; // pana la cat o sa dureze
    int timpInceput; // cand ajunge la cpu
    int prioritate;
    int timpAsteptare, timpProcesare;
    int verificat;
 
}procese;
 
 
void bubble_sort(procese temp[], int n)
{
    procese t;
    // procesele sunt sortate crescator dupa timpul de inceput
    // primul din vector va fi primul care ajunge la cpu
 
    int ok = 1;
    while(ok == 1)
    {
        ok = 0;
        for(int i = 0; i < n-1; ++i)
            if(temp[i].timpInceput > temp[i+1].timpInceput)
            {
                t = temp[i+1];
                temp[i+1] = temp[i];
                temp[i] = t;
                ok = 1;
            }
    }
 
}
 
int accept(procese P[])
{
    int n;
 
    printf("\n Introdu numar procese : ");
    scanf("%d",&n);
 
    for(int i = 0; i < n; ++i)
    {
        printf("\n PROCESUL [%d]", i+1);
        printf(" Introdu nume proces : ");
        scanf("%s", P[i].nume);
        printf(" Introdu timp ocupare : ");
        scanf("%d", &P[i].timpOcupare);
        printf(" Introdu timpul de inceput : ");
        scanf("%d", &P[i].timpInceput);
        printf(" Introdu prioritate : ");
        scanf("%d", &P[i].prioritate);
 
    }
 
    printf("\n PROC.\tB.T.\tA.T.\tPRIORITY");
    for(int i = 0; i < n; ++i)
        printf("\n %s\t%d\t%d\t%d", P[i].nume, P[i].timpOcupare, P[i].timpInceput, P[i].prioritate);
 
    return n;
}
 
// FCFS Algorithm
void FCFS(procese P[],int n){
    procese temp[10];
    int sumaTimpAsteptare=0,sumaTimpProcesare=0;
    int x = 0;
    float timpMediuAsteptare=0.0,timpMediuProcesare=0.0;
    int i,j;
    for(i=0;i<n;i++)
        temp[i]=P[i];
 
    bubble_sort(temp,n);
 
        printf("\n\n PROC.\tB.T.\tA.T.");
        for(i=0;i<n;i++)
            printf("\n %s\t%d\t%d",temp[i].nume,temp[i].timpOcupare,temp[i].timpInceput);
 
        sumaTimpAsteptare = temp[0].timpAsteptare = 0;
        sumaTimpProcesare = temp[0].timpProcesare = temp[0].timpOcupare - temp[0].timpInceput;
 
        for(i=1;i<n;i++){
            temp[i].timpAsteptare = (temp[i-1].timpOcupare + temp[i-1].timpInceput + temp[i-1].timpAsteptare) - temp[i].timpInceput;;
            temp[i].timpProcesare = (temp[i].timpAsteptare + temp[i].timpOcupare);
            sumaTimpAsteptare+=temp[i].timpAsteptare;
            sumaTimpProcesare+=temp[i].timpProcesare;
        }
        timpMediuAsteptare = (float)sumaTimpAsteptare/n;
        timpMediuProcesare = (float)sumaTimpProcesare/n;
        printf("\n\n PROC.\tB.T.\tA.T.\tW.T\tT.A.T");
        for(i=0;i<n;i++)
            printf("\n %s\t%d\t%d\t%d\t%d",temp[i].nume,temp[i].timpOcupare,temp[i].timpInceput,temp[i].timpAsteptare,temp[i].timpProcesare);
 
        printf("\n\n GANTT CHART\n ");
        for(i=0;i<n;i++)
            printf("   %s   ",temp[i].nume);
        printf("\n ");
 
        printf("0\t");
        for(i=1;i<=n;i++){
            x+=temp[i-1].timpOcupare;
            printf("%d      ",x);
        }
        printf("\n\n Timpul mediu de asteptare = %0.2f\n Timpul mediu de procesare = %0.2f.",timpMediuAsteptare,timpMediuProcesare);
}
 
///Round Robin Scheduling - preentiv - poate fi intrerupt si sa continue mai tarziu in cazul in care a ajuns la quantumul introdus
void RR(procese P[], int n)
{
    int nrProceseVerif = 0, t, timpCurent = 0, k, i, Q = 0;
    int sumaTimpAsteptare = 0, sumaTimpProcesare = 0;
    float timpMediuAsteptare = 0.0, timpMediuProcesare = 0.0;
    procese temp1[10], temp2[10];
 
    for(i = 0; i < n; i++)
        temp1[i] = P[i];
 
    bubble_sort(temp1, n); ///sorteaza crescator dupa timpul de inceput
 
    for(i = 0; i < n; i++)
        temp2[i] = temp1[i];
 
    ///quantumul reprezinta timpul maxim de executare pe care il are procesul la dispozitie
    printf("\n Introdu quantum : ");
    scanf("%d",&Q);
 
    for(k = 0 ;; k++)
    {
        if(k > n - 1)
            k = 0;
 
        if(temp1[k].timpOcupare > 0)
            printf("(%s  %d)", temp1[k].nume , timpCurent);
            //printf("(  %d  %s)", timpCurent, temp1[k].nume);
        t = 0;
 
        ///ii scade din timpul de ocupare pana in quantum si adauga la cat timp s-a executat
        ///il va relua mai tarziu daca trece de quantum, din nou
        while(t < Q && temp1[k].timpOcupare > 0)
        {
            t++;
            timpCurent++;
            temp1[k].timpOcupare--;
        }
 
        if(temp1[k].timpOcupare <= 0 && temp1[k].verificat != 1) ///si-a ocupat deja timpul si il calculam cat i-a luat pana aucm sa il faca
        {
            temp1[k].timpAsteptare = timpCurent - temp2[k].timpOcupare - temp1[k].timpInceput;
            temp1[k].timpProcesare = timpCurent - temp1[k].timpInceput; ///timpul de cand a inceput si pana s-a terminat
            nrProceseVerif++;
            temp1[k].verificat = 1;
            sumaTimpAsteptare += temp1[k].timpAsteptare;
            sumaTimpProcesare += temp1[k].timpProcesare;
        }
 
        if(nrProceseVerif == n)
            break;
    }
    printf("  %d", timpCurent);
    timpMediuAsteptare = (float)sumaTimpAsteptare / n;
    timpMediuProcesare = (float)sumaTimpProcesare / n;
    printf("\n\n Timpul mediu de asteptare = %0.2f\n Timpul mediu de procesare = %0.2f.", timpMediuAsteptare, timpMediuProcesare);
}
 
 
//Priority Non Pre-emptive
void PRT_NP(procese P[],int n)
{
    procese temp[10];
    procese t;
    int sumaTimpAsteptare=0,sumaTimpProcesare=0;
    float timpMediuAsteptare=0.0,timpMediuProcesare=0.0;
    int i,j;
    int x = 0;
 
    for(i=0;i<n;i++)
        temp[i]=P[i];
 
    bubble_sort(temp,n);
 
    for(i=2;i<n;i++)
        for(j=1;j<n-i+1;j++){
            if(temp[j].prioritate > temp[j+1].prioritate){
                t = temp[j];
                temp[j] = temp[j+1];
                temp[j+1] = t;
            }
        }
 
    printf("\n\n PROC.\tTimp Ocupare\tTimp Inceput");
        for(i=0;i<n;i++)
            printf("\n %s\t%d\t\t%d",temp[i].nume,temp[i].timpOcupare,temp[i].timpInceput);
 
        sumaTimpAsteptare = temp[0].timpAsteptare = 0;
        sumaTimpProcesare = temp[0].timpProcesare = temp[0].timpOcupare - temp[0].timpInceput;
 
        for(i=1;i<n;i++){
            temp[i].timpAsteptare = (temp[i-1].timpOcupare + temp[i-1].timpInceput + temp[i-1].timpAsteptare) - temp[i].timpInceput;;
            temp[i].timpProcesare = (temp[i].timpAsteptare + temp[i].timpOcupare);
            sumaTimpAsteptare+=temp[i].timpAsteptare;
            sumaTimpProcesare+=temp[i].timpProcesare;
        }
        timpMediuAsteptare = (float)sumaTimpAsteptare/n;
        timpMediuProcesare = (float)sumaTimpProcesare/n;
        printf("\n\n PROC.\tTimp Ocupare\tTimp Inceput\tTimp Asteptare\tTimp Procesare");
        for(i=0;i<n;i++)
            printf("\n %s\t%d\t\t%d\t\t%d\t\t%d",temp[i].nume,temp[i].timpOcupare,temp[i].timpInceput,temp[i].timpAsteptare,temp[i].timpProcesare);
 
        printf("\n\n GANTT CHART\n ");
        for(i=0;i<n;i++)
            printf("   %s   ",temp[i].nume);
        printf("\n ");
 
        printf("0\t");
        for(i=1;i<=n;i++){
            x+=temp[i-1].timpOcupare;
            printf("%d      ",x);
        }
        printf("\n\n Timpul mediu de asteptare = %0.2f\n Timpul mediu de procesare = %0.2f.",timpMediuAsteptare,timpMediuProcesare);
}
 
void PRT_P(procese P[],int n){
    int i,t_total=0,timpCurent,b[10],j,x,min_pr;
    int sumaTimpAsteptare=0,sumaTimpProcesare=0;
    float timpMediuAsteptare=0.0,timpMediuProcesare=0.0;
    procese temp[10],t;
 
    for(i=0;i<n;i++){
        temp[i]=P[i];
        t_total+=P[i].timpOcupare;
    }
 
    bubble_sort(temp,n);
 
    for(i=0;i<n;i++)
        b[i] = temp[i].timpOcupare;
 
    i=j=0;
    printf("\n GANTT CHART\n\n %d %s",i,temp[i].nume);
    for(timpCurent=0;timpCurent<t_total;timpCurent++)
    {
 
        if(b[i] > 0 && temp[i].timpInceput <= timpCurent)
            b[i]--;
 
        if(i!=j)
            printf(" %d %s",timpCurent,temp[i].nume);
 
        if(b[i]<=0 && temp[i].verificat != 1)
        {
            temp[i].verificat = 1;
            temp[i].timpAsteptare = (timpCurent+1) - temp[i].timpOcupare - temp[i].timpInceput;
            temp[i].timpProcesare = (timpCurent+1) - temp[i].timpInceput;
            sumaTimpAsteptare+=temp[i].timpAsteptare;
            sumaTimpProcesare+=temp[i].timpProcesare;
        }
        j=i;
        min_pr = 999;
        for(x=0;x<n;x++){
 
            if(temp[x].timpInceput <= (timpCurent+1) && temp[x].verificat != 1){
 
                if(min_pr != temp[x].prioritate && min_pr > temp[x].prioritate){
                    min_pr = temp[x].prioritate;
                    i=x;
                }
            }
        }
 
    }
    printf(" %d",timpCurent);
    timpMediuAsteptare = (float)sumaTimpAsteptare/n;
    timpMediuProcesare = (float)sumaTimpProcesare/n;
    printf("\n\n Timpul mediu de asteptare = %0.2f\n Timpul mediu de procesare = %0.2f.",timpMediuAsteptare,timpMediuProcesare);
}
 
 
// ---------------------- //
//SJF Non Pre-emptive
void SJF_NP(procese P[],int n)
{
    procese temp[10];
    procese t;
    int sTimpAsteptare = 0, sTimpProcesare = 0;
    int x = 0;
    float timpMedAst = 0.0, timpMedProces = 0.0;
 
    for(int i = 0; i < n; ++i)
        temp[i] = P[i]; // copiez vectorul de procese original ca sa nu il modific
 
    bubble_sort(temp, n);
 
    int ok = 1;
    while(ok == 1)
    {
        ok = 0;
        for(int i = 0; i < n-1; ++i)
            if(temp[i].timpOcupare > temp[i+1].timpOcupare)
            {
                t = temp[i+1];
                temp[i+1] = temp[i];
                temp[i] = t;
                ok = 1;
            }
    }
 
    printf("\n\n PROC.\tB.T.\tA.T.");
    for(int i = 0; i < n; ++i)
        printf("\n %s\t%d\t%d", temp[i].nume, temp[i].timpOcupare, temp[i].timpInceput);
 
    sTimpAsteptare = 0;
    sTimpProcesare = temp[0].timpOcupare - temp[0].timpInceput;
 
    temp[0].timpAsteptare = 0; // primul este procesat imediat
    temp[0].timpProcesare = temp[0].timpOcupare - temp[0].timpInceput;
 
    for(int i = 1; i < n; ++i)
    {
        temp[i].timpAsteptare = (temp[i-1].timpOcupare + temp[i-1].timpInceput + temp[i-1].timpAsteptare) - temp[i].timpInceput;
        temp[i].timpProcesare = (temp[i].timpAsteptare + temp[i].timpOcupare);
        sTimpAsteptare += temp[i].timpAsteptare;
        sTimpProcesare += temp[i].timpProcesare;
    }
 
    timpMedAst = (float)sTimpAsteptare/n;
    timpMedProces = (float)sTimpProcesare/n;
    printf("\n\n PROC.\tB.T.\tA.T.\tW.T\tT.A.T");
    for(int i = 0; i < n; ++i)
        printf("\n %s\t%d\t%d\t%d\t%d", temp[i].nume, temp[i].timpOcupare, temp[i].timpInceput, temp[i].timpAsteptare, temp[i].timpProcesare);
 
    printf("\n\n GANTT CHART\n ");
    for(int i = 0; i < n; ++i)
        printf("   %s   ", temp[i].nume);
    printf("\n ");
 
    printf("0\t");
    for(int i = 1; i <= n; ++i)
    {
        x += temp[i-1].timpOcupare;
        printf("%d      ",x);
    }
    printf("\n\n Timpul mediu de asteptare = %0.2f\n Timpul mediu de procesare = %0.2f.", timpMedAst, timpMedProces);
 
 
}
 
//Shortest Job First - Pre-emptive
void SJF_P(procese P[],int n){
    int i, t_total=0, timpCurent, b[10], min_timpInceput, j, x, min_timpOcupare;
    int sumaTimpAsteptare = 0, sumaTimpProcesare = 0;
    float timpMediuAsteptare = 0.0, timpMediuProcesare = 0.0;
    procese temp[10], t;
 
    for(i = 0; i < n; ++i)
    {
        temp[i] = P[i];
        t_total += P[i].timpOcupare;
    }
 
    bubble_sort(temp,n);
 
    for(i = 0; i < n ; ++i)
        b[i] = temp[i].timpOcupare;
 
    i = j = 0;
    printf("\n GANTT CHART\n\n %d %s", i, temp[i].nume);
 
    for(timpCurent = 0; timpCurent < t_total; timpCurent++)
    {
 
            if(b[i] > 0 && temp[i].timpInceput <= timpCurent)
                b[i]--;
 
            if(i!=j)
                printf(" %d %s",timpCurent,temp[i].nume);
 
            if(b[i]<=0 && temp[i].verificat != 1)
            {
 
                temp[i].verificat = 1;
                temp[i].timpAsteptare = (timpCurent+1) - temp[i].timpOcupare - temp[i].timpInceput;
                temp[i].timpProcesare = (timpCurent+1) - temp[i].timpInceput;
                sumaTimpAsteptare+=temp[i].timpAsteptare;
                sumaTimpProcesare+=temp[i].timpProcesare;
            }
            j=i;
            min_timpOcupare = 999;
 
            for(x = 0; x < n; x++)
            {
 
            if(temp[x].timpInceput <= (timpCurent+1) && temp[x].verificat != 1)
            {
                if(min_timpOcupare != b[x] && min_timpOcupare > b[x])
                {
                    min_timpOcupare = b[x];
                    i = x;
                }
            }
            }
 
    }
 
    printf(" %d",timpCurent);
 
    timpMediuAsteptare = (float)sumaTimpAsteptare/n;
    timpMediuProcesare = (float)sumaTimpProcesare/n;
 
    printf("\n\n Timpul mediu de asteptare = %0.2f\n Timpul mediu de procesare = %0.2f.", timpMediuAsteptare, timpMediuProcesare);
}
 
 
 
int main(){
 
    procese P[10];
    int ch,n;
    do{
        printf("\n\n SIMULARE ALGORITMI DE SCHEDULING\n");
        printf("\n Optiuni:");
        printf("\n 0. Introdu informatiile procesului.");
        printf("\n 1. FCFS");
        printf("\n 2. SJF (Pre-emptive)");
        printf("\n 3. SJF (Non Pre-emptive)");
        printf("\n 4. Priority Scheduling (Pre-emptive)");
        printf("\n 5. Priority Scheduling (Non Pre-emptive)");
        printf("\n 6. Round Robin");
        printf("\n 7. Exit\n Optiunea ta : ");
        scanf("%d",&ch);
        switch(ch){
        default:
            exit(0);
            case 0:
                n=accept(P);
                break;
            case 1:
                FCFS(P,n);
                break;
            case 2:
                SJF_P(P,n);
                break;
            case 3:
                SJF_NP(P,n);
                break;
            case 4:
                PRT_P(P,n);
                break;
            case 5:
                PRT_NP(P,n);
                break;
            case 6:
                RR(P,n);
                break;
            case 7:exit(0);
        }
    }while(ch != 7);
    return 0;
}