#include <iostream>
#include <string.h>
#include <string>
#include <vector>
using namespace std;


class Nevertebrate{
    char* nume;
public:
    Nevertebrate(char* num){nume=num;};
    ~Nevertebrate(){};
    void spune(){cout<<"nevertebrata"<<endl;}

};

class Vertebrate{
protected:
    char* nume;
public:
    Vertebrate(char* num){
        nume = num;
    }
    virtual ~Vertebrate(){};

//    char* get_nume(){
//        return nume;
//    }
//    void set_nume(char* num){
//        nume = num;
//    }

    virtual void spune() =0;
};

class Mamifere:public Vertebrate {
    int nr_pui;
public:
    Mamifere(char* num ,int x) :Vertebrate(num) {nr_pui = x;}
    ~Mamifere(){};
    void spune();
    friend istream& operator>>(istream& i,Mamifere &ob);
    Mamifere* operator=(Mamifere &ob);
};

class Pesti:public Vertebrate {
    int nr_icre;
    char* tip_apa;
public:
    Pesti(char* num, int x,char* tip) :Vertebrate(num) {
        nr_icre = x;
        tip_apa = tip;
    };
    ~Pesti(){};
    void spune();
};

class Pasari:public Vertebrate{
    int marime_aripi;
    int nr_oua;
public:
    Pasari(char* num,int x, int y):Vertebrate(num){marime_aripi=x;nr_oua=y;};
    ~Pasari(){};
    void spune();
};

class Reptile:public Vertebrate{
    int nr_oua;
    int marime_solzi;
public:
    Reptile(char* num,int x,int y):Vertebrate(num){nr_oua=x;marime_solzi=y;};
    ~Reptile(){};
    void spune();
};



void Mamifere::spune(){
    cout<<"Nume: "<<nume<<"  \nAproximare pui nascuti vii: "<<nr_pui<<endl<<endl;
}

void Pesti::spune() {
    cout<<"Nume: "<<nume<<" \nTip apa: "<<tip_apa<<"  \nAproximare numar icre: "<<nr_icre<<endl<<endl;
}

void Pasari::spune(){
    cout<<"Nume: "<<nume<<" \nAproximare numar oua: "<<nr_oua<<" \nMarime aripi masculi: "<<marime_aripi<<" cm."<<endl<<endl;
}

void Reptile::spune(){
    cout<<"Nume: "<<nume<<" \nAproximare numar oua: "<<nr_oua<<" \nMarime solzi: "<<marime_solzi<<" cm."<<endl<<endl;
}


Mamifere* Mamifere::operator=(Mamifere &ob){
    this->nume = ob.nume;
    this->nr_pui = ob.nr_pui;
    return this;
}


istream& operator>>(istream &i, Mamifere &ob) {
    char num[20];
    i>>num;
    ob.nume=num;
    int x;
    i>>x;
    ob.nr_pui = x;
    return i;
}


template <class T>
class AtlasZoologic {
    int index;
    vector<T*> lista;
public:
    AtlasZoologic(){index=0;}
    ~AtlasZoologic() {};
    AtlasZoologic(T *a){
        lista.push_back(&a);
        index++;
    }

    AtlasZoologic <T> & operator+= (T &x){
        lista.push_back(&x);
        index++;
        return *this;
    }

    void numar_animale(){cout<<"In atlas sunt "<<index<<" animale\n\n";};

    void spune(){

        Mamifere *A;
        Pesti *B;
        Pasari *C;
        Reptile *D;

        for(int i=0;i< lista.size();i++){

            try{
                A = dynamic_cast<Mamifere*>(lista[i]);
                B = dynamic_cast<Pesti*>(lista[i]);
                C = dynamic_cast<Pasari*>(lista[i]);
                D = dynamic_cast<Reptile*>(lista[i]);

                if(A) A->spune();
                if(B) B->spune();
                if(C) C->spune();
                if(D) D->spune();
            }

            catch (bad_cast &bc){
                cerr << bc.what() <<endl;
            }
        }

    }
};




int main() {


    Mamifere om(const_cast<char *>("om"), 1);
    Mamifere urs(const_cast<char *>("urs"), 2);
    Mamifere urs2("NANANA",14);
    Pesti caras(const_cast<char *>("caras"), 300, const_cast<char *>("dulce"));
    Pasari cioara(const_cast<char *>("cioara"), 150, 3);

    om.spune();
    urs.spune();
    caras.spune();
    cioara.spune();

    urs2=urs;
    urs2.spune();

    Nevertebrate ceva(const_cast<char *>("insecta"));
    ceva.spune();

    cout<<"\n\n\n";

    AtlasZoologic<Vertebrate> V;

    V+= om;
    V+= urs;
    V+= caras;
    V+= cioara;

    V.spune();
    V.numar_animale();

    return 0;
}