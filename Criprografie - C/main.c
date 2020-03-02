//
//  main.c
//  Criptare
//
//  Created by Iliescu Andrei on 29/12/2018.
//  Copyright © 2018 Iliescu Andrei. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

//Definirea unui pixel
typedef struct
{
    unsigned char red;
    unsigned char green;
    unsigned char blue;
}pixel;

unsigned char xor (unsigned char x, unsigned char y)
{
    return x ^ y;
}

//XORSHIFT32
uint32_t xorshift32(uint32_t state[static 1])
{
    unsigned int x = state[0];
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    state[0] = x;
    return x;
}

//Returneaza dimensiunea unui fisier deschis
long dimensiuneFisier(FILE *f)
{
    long p, dim;
    
    p = ftell(f);
    fseek(f, 0, SEEK_END);
    dim = ftell(f);
    fseek(f, p, SEEK_SET);
    
    return dim;
}

//Citeste, returneaza dimensiunea
pixel* citire (const char* nume,int *dim)
{
    FILE *fin;
    fin = fopen(nume,"rb");
    if(fin == NULL)
    {
        printf("EROARE la deschiderea fisierului %s",nume);
        return NULL;
    }
    unsigned char val;
    
    *dim = dimensiuneFisier(fin);
    
    unsigned char *v = (unsigned char*)malloc(*dim * sizeof(char));
    
    for(int i=0;i<*dim;i++)
    {
        fread(&val,sizeof(char),1,fin);
        v[i]=val;
    }
    
    
    int latime,inaltime;
    fseek(fin, 18, SEEK_SET);
    fread(&latime, sizeof(unsigned int), 1, fin);
    fread(&inaltime, sizeof(unsigned int), 1, fin);
    
    int padding;
    if(latime % 4 != 0)                         //afla padding
        padding = 4 - (3 * latime) % 4;
    else
        padding = 0;
    
    fclose(fin);
    
    pixel *liniar_pixel=(pixel*)malloc(inaltime*latime*sizeof(pixel));                          //aloc spatiu pentru imaginea liniarizata pe pixeli
    
    int j;
    int k=latime*(inaltime-1);
    int l=54;
    for(int i=0;i<inaltime;i++)                                                                     //scriu imaginea liniarizata pe pixeli
    {
        for(j=0;j<latime;j++)
        {
            liniar_pixel[k].blue=v[l];
            l++;
            liniar_pixel[k].green=v[l];
            l++;
            liniar_pixel[k].red=v[l];
            l++;
            
            k++;
            
            
        }
        k=k-2*latime;
        l+=padding;
    }
    
    free(v);
    return liniar_pixel;
}

//Scrie o imagine bitmap dintr-un vector cu o imagine liniarizata
void scriere(pixel *liniar_pixel,const char* nume_original,  int dim,  const char *nume, int latime ,int inaltime, int padding)
{
    unsigned char val;
    FILE *fout;
    fout = fopen(nume,"wb");
    if(fout == NULL)
    {
        printf("EROARE la crearea fisierului %s",nume);
        return;
    }

    FILE *fin = fopen(nume_original,"rb");
    unsigned char liniar[54];
    
    for(int i=0;i<54;i++)
    {
        fread(&val,sizeof(char),1,fin);
        liniar[i]=val;
    }
    
    fclose(fin);
    
    unsigned char *liniar_recon=(unsigned char*)malloc(dim*sizeof(char));
    for(int i=0;i<54;i++)
    {
        liniar_recon[i]=liniar[i];
    }

    int k = latime*(inaltime-1);
    int l = 54;
    for(int i=0;i<inaltime;i++)
    {
        for(int j=0;j<latime;j++)
            {
                liniar_recon[l]=liniar_pixel[k].blue;
                l++;
                liniar_recon[l]=liniar_pixel[k].green;
                l++;
                liniar_recon[l]=liniar_pixel[k].red;
                l++;
    
                k++;
            }
        k=k-2*latime;
        l+=padding;
    }
    
    for(int i=0;i<dim;i++)
    {
        val=liniar_recon[i];
        fwrite(&val,1,1,fout);
    }
    
    free(liniar_recon);
    
    fclose(fout);
}

//CRIPTARE
void criptare(const char* nume_in,const char* nume_out,const char* nume_cheie)
{
    int dim,i,k,inter,padding;
    unsigned int aux;
    pixel *liniar_pixel;
    liniar_pixel = citire(nume_in,&dim);
    
    FILE *f=fopen(nume_in,"rb");                //afla latimea si inaltimea
    int latime,inaltime;
    fseek(f, 18, SEEK_SET);
    fread(&latime, sizeof(unsigned int), 1, f);
    fread(&inaltime, sizeof(unsigned int), 1, f);
    fclose(f);

    if(latime % 4 != 0)                         //afla padding
        padding = 4 - (3 * latime) % 4;
    else
        padding = 0;
    
    
    FILE *fcheie = fopen(nume_cheie,"r");       //citeste cheia
    if(fcheie==NULL)
    {
        printf("EROARE la deschiderea fisierului %s",nume_cheie);
        return;
    }
    
    unsigned int r0[1];
    fscanf(fcheie,"%u",&r0[0]);
    
    
    unsigned int *r=(unsigned int*)malloc((inaltime*latime*2-1)*sizeof(unsigned int));          //aloca spatiu pt secventa R
    for(i=0;i<inaltime*latime*2-1;i++)
    {
        r[i]=xorshift32(r0);
    }
    
    unsigned int *tau=(unsigned int*)malloc((inaltime*latime)*sizeof(unsigned int));            //aloca spatiu pentru permutarea Tau
    for(i=0;i<inaltime*latime;i++)                                                              //o initializeaza
        tau[i]=i;

    i = 0;
    for(k=inaltime*latime-1;k>0;k--)                                                            //genereaza permutarea aleatorie
    {
        inter=r[i]%(k+1);
        i++;
        aux=tau[k];
        tau[k]=tau[inter];
        tau[inter]=aux;
    }
    
    pixel *liniar_pixel_copie=(pixel*)malloc(inaltime*latime*sizeof(pixel));                                //copie la vectorul de pixeli
    for(i=0;i<inaltime*latime;i++)
        liniar_pixel_copie[i]=liniar_pixel[i];

    for(i=0;i<latime*inaltime;i++)                                                                          //aplic permutarea
    {
        liniar_pixel[tau[i]]=liniar_pixel_copie[i];
    }
    
    unsigned int sv;
    fscanf(fcheie,"%u",&sv);
    fclose(fcheie);
    
    unsigned char bytes[4];
    
    bytes[0] = (sv >> 24) & 0xFF;
    bytes[1] = (sv >> 16) & 0xFF;
    bytes[2] = (sv >> 8) & 0xFF;
    bytes[3] = sv & 0xFF;
    
    
    liniar_pixel[0].blue=xor(liniar_pixel[0].blue,bytes[3]);
    liniar_pixel[0].green=xor(liniar_pixel[0].green,bytes[2]);
    liniar_pixel[0].red=xor(liniar_pixel[0].red,bytes[1]);
    
    bytes[0] = (r[inaltime*latime-1] >> 24) & 0xFF;
    bytes[1] = (r[inaltime*latime-1] >> 16) & 0xFF;
    bytes[2] = (r[inaltime*latime-1] >> 8) & 0xFF;
    bytes[3] = r[inaltime*latime-1] & 0xFF;
    
    liniar_pixel[0].blue=xor(liniar_pixel[0].blue,bytes[3]);
    liniar_pixel[0].green=xor(liniar_pixel[0].green,bytes[2]);
    liniar_pixel[0].red=xor(liniar_pixel[0].red,bytes[1]);
    
    
    for(int i=1;i<inaltime*latime;i++)
    {
        bytes[0] = (r[inaltime*latime-1+i] >> 24) & 0xFF;
        bytes[1] = (r[inaltime*latime-1+i] >> 16) & 0xFF;
        bytes[2] = (r[inaltime*latime-1+i] >> 8) & 0xFF;
        bytes[3] = r[inaltime*latime-1+i] & 0xFF;
        
        liniar_pixel[i].blue=xor(liniar_pixel[i-1].blue,liniar_pixel[i].blue);
        liniar_pixel[i].green=xor(liniar_pixel[i-1].green,liniar_pixel[i].green);
        liniar_pixel[i].red=xor(liniar_pixel[i-1].red,liniar_pixel[i].red);
        
        liniar_pixel[i].blue=xor(liniar_pixel[i].blue,bytes[3]);
        liniar_pixel[i].green=xor(liniar_pixel[i].green,bytes[2]);
        liniar_pixel[i].red=xor(liniar_pixel[i].red,bytes[1]);
    }
    
    
    scriere(liniar_pixel,nume_in,dim,nume_out,latime,inaltime,padding);
    free(r);
    free(tau);
    free(liniar_pixel_copie);
    free(liniar_pixel);
}


//DECRIPTARE
void decriptare(const char *nume_in,const char *nume_out,const char*nume_cheie)
{
    int dim,i,k,inter,padding;
    unsigned int aux;
    pixel *liniar_pixel;
    liniar_pixel = citire(nume_in,&dim);
    
    FILE *f=fopen(nume_in,"rb");                //afla latimea si inaltimea
    int latime,inaltime;
    fseek(f, 18, SEEK_SET);
    fread(&latime, sizeof(unsigned int), 1, f);
    fread(&inaltime, sizeof(unsigned int), 1, f);
    fclose(f);
    
    if(latime % 4 != 0)                         //afla padding
        padding = 4 - (3 * latime) % 4;
    else
        padding = 0;
    
    
    FILE *fcheie = fopen(nume_cheie,"r");       //citeste cheia
    if(fcheie==NULL)
    {
        printf("EROARE la deschiderea fisierului %s",nume_cheie);
        return;
    }
    
    unsigned int r0[1];
    fscanf(fcheie,"%u",&r0[0]);
    
    
    unsigned int *r=(unsigned int*)malloc((inaltime*latime*2-1)*sizeof(unsigned int));          //aloca spatiu pt secventa R
    for(i=0;i<inaltime*latime*2-1;i++)
    {
        r[i]=xorshift32(r0);
    }
    
    unsigned int *tau=(unsigned int*)malloc((inaltime*latime)*sizeof(unsigned int));            //aloca spatiu pentru permutarea Tau
    for(i=0;i<inaltime*latime;i++)                                                              //o initializeaza
        tau[i]=i;
    
    i = 0;
    for(k=inaltime*latime-1;k>0;k--)                                                            //genereaza permutarea aleatorie
    {
        inter=r[i]%(k+1);
        i++;
        aux=tau[k];
        tau[k]=tau[inter];
        tau[inter]=aux;
    }
    
    unsigned int *tau_invers = (unsigned int*)malloc(inaltime*latime*sizeof(unsigned int));
    for(i=0;i<inaltime*latime;i++)
    {
        tau_invers[tau[i]]=i;
    }
    
    unsigned int sv;
    fscanf(fcheie,"%u",&sv);
    fclose(fcheie);
    
    unsigned char bytes[4];
    
    bytes[0] = (sv >> 24) & 0xFF;
    bytes[1] = (sv >> 16) & 0xFF;
    bytes[2] = (sv >> 8) & 0xFF;
    bytes[3] = sv & 0xFF;
    
    pixel *liniar_pixel_copie = (pixel*)malloc(sizeof(pixel)*inaltime*latime);
    for(i=0;i<latime*inaltime;i++)
        liniar_pixel_copie[i] = liniar_pixel[i];
    
    liniar_pixel[0].blue=xor(liniar_pixel[0].blue,bytes[3]);
    liniar_pixel[0].green=xor(liniar_pixel[0].green,bytes[2]);
    liniar_pixel[0].red=xor(liniar_pixel[0].red,bytes[1]);

    bytes[0] = (r[inaltime*latime] >> 24) & 0xFF;
    bytes[1] = (r[inaltime*latime] >> 16) & 0xFF;
    bytes[2] = (r[inaltime*latime] >> 8) & 0xFF;
    bytes[3] = r[inaltime*latime] & 0xFF;

    liniar_pixel[0].blue=xor(liniar_pixel[0].blue,bytes[3]);
    liniar_pixel[0].green=xor(liniar_pixel[0].green,bytes[2]);
    liniar_pixel[0].red=xor(liniar_pixel[0].red,bytes[1]);

    for(int i=1;i<inaltime*latime;i++)
    {
        bytes[0] = (r[inaltime*latime-1+i] >> 24) & 0xFF;
        bytes[1] = (r[inaltime*latime-1+i] >> 16) & 0xFF;
        bytes[2] = (r[inaltime*latime-1+i] >> 8) & 0xFF;
        bytes[3] = r[inaltime*latime-1+i] & 0xFF;

        liniar_pixel[i].blue=xor(liniar_pixel_copie[i-1].blue,liniar_pixel[i].blue);
        liniar_pixel[i].green=xor(liniar_pixel_copie[i-1].green,liniar_pixel[i].green);
        liniar_pixel[i].red=xor(liniar_pixel_copie[i-1].red,liniar_pixel[i].red);

        liniar_pixel[i].blue=xor(liniar_pixel[i].blue,bytes[3]);
        liniar_pixel[i].green=xor(liniar_pixel[i].green,bytes[2]);
        liniar_pixel[i].red=xor(liniar_pixel[i].red,bytes[1]);
    }
    
    for(i=0;i<latime*inaltime;i++)
        liniar_pixel_copie[i] = liniar_pixel[i];
    
    for(i=0;i<latime*inaltime;i++)                                                                          //aplic permutarea
    {
        liniar_pixel[tau_invers[i]]=liniar_pixel_copie[i];
    }
    
    scriere(liniar_pixel,nume_in,dim,nume_out,latime,inaltime,padding);
    
    free(r);
    free(tau);
    free(tau_invers);
    free(liniar_pixel_copie);
    free(liniar_pixel);
    
}

void test_chi(const char* nume)
{
    unsigned int dim,i,j,frecvr=0,frecvg=0,frecvb=0;
    float sumar=0,sumag=0,sumab=0,frecv_est;
    
    pixel *liniar_pixel = citire(nume,&dim);
    
    FILE *fin = fopen(nume,"rb");
    if(fin == NULL)
    {
        printf("EROARE la deschiderea fisierului %s",nume);
        return;
    }
    
    int latime,inaltime;
    fseek(fin, 18, SEEK_SET);
    fread(&latime, sizeof(unsigned int), 1, fin);
    fread(&inaltime, sizeof(unsigned int), 1, fin);
    
    frecv_est=(inaltime*latime)/256;
    for(i=0;i<256;i++)
    {
        for(j=0;j<inaltime*latime;j++)
        {
            if(liniar_pixel[j].red == i)
                frecvr++;
            if(liniar_pixel[j].green == i)
                frecvg++;
            if(liniar_pixel[j].blue == i)
                frecvb++;
        }
        
        sumar += ((frecvr-frecv_est) * (frecvr-frecv_est))/frecv_est;
        sumag += ((frecvg-frecv_est) * (frecvg-frecv_est))/frecv_est;
        sumab += ((frecvb-frecv_est) * (frecvb-frecv_est))/frecv_est;
        
        frecvr=0;
        frecvg=0;
        frecvb=0;
    }
    
    printf("R:%11.2f    G:%11.2f    B:%11.2f\n",sumar,sumag,sumab);
    fclose(fin);
    
}

int main() {
    

    char* nume = "peppers.bmp";

    char * nume_out = "peppers_criptata.bmp";
    
    char* nume_decriptata = "peppers_decript.bmp";
    
    char *nume_cheie = "secret_key.txt";
    
    
    criptare(nume,nume_out,nume_cheie);
    
    decriptare(nume_out,nume_decriptata,nume_cheie);
    
    test_chi(nume);
    test_chi(nume_out);
    
    
    
    return 0;
}
