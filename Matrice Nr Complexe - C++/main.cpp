#include <iostream>
#include <fstream>

using namespace std;

class complex {
    float re,im;
public:
    float get_re();
    float get_im();
    void set_re(float);
    void set_im(float);
    complex operator+ (complex);
    complex operator* (complex);

    friend std::istream& operator>> (std::istream &in, complex &a);
    friend std::ostream& operator<< (std::ostream &out, const complex &a);
};
std::istream& operator>> (std::istream &in, complex &a) {
    in >> a.re;
    in >> a.im;

    return in;
}

std::ostream& operator<<(std::ostream &out, const complex &a) {
    out<<a.re<<" + "<<a.im<<"i ";

    return out;
}

complex complex::operator+ (complex b) {
    complex c{};
    c.set_re(this->get_re() + b.get_re());
    c.set_im(this->get_im() + b.get_im());
    return c;
}

complex complex::operator*(complex b) {
    complex c{};
    c.set_re(this->get_re() * b.get_re() - this->get_im() * b.get_im());
    c.set_im(this->get_re() * b.get_im() + this->get_im() * b.get_re());

    return c;
}

float complex::get_re() {
    return re;
}

float complex::get_im() {
    return im;
}

void complex::set_re(float x) {
    re = x;
}

void complex::set_im(float x) {
    im = x;
}


complex determinant( complex matrix[10][10], int n) {
    complex det{},sig{},aux{};
    det.set_re(0);det.set_im(0);
    sig.set_re(-1);sig.set_im(0);
    aux.set_re(1);aux.set_im(0);

    complex submatrix[10][10];

    if (n == 2)
        return (  (matrix[0][0] * matrix[1][1]) + ( sig * (matrix[1][0] * matrix[0][1])) );
    else {
        for (int x = 0; x < n; x++) {
            int subi = 0;
            for (int i = 1; i < n; i++) {
                int subj = 0;
                for (int j = 0; j < n; j++) {
                    if (j == x)
                        continue;
                    submatrix[subi][subj] = matrix[i][j];
                    subj++;
                }
                subi++;
            }

            det = det + aux * matrix[0][x] * determinant( submatrix, n - 1 ) ;
            aux = aux *sig;
        }
    }
    return det;
}


struct node{
    int linie,coloana;
    complex numar;
    node *next;
};



class Matrice{
    node *start = NULL;
public:
    Matrice(complex **v,int linie,int coloana);
    ~Matrice();
    node* get_start();
    virtual void afisare()=0;
};


Matrice::Matrice(complex **v,int linie,int coloana) {
    for(int i=0;i<linie;++i)
        for(int j=0;j<coloana;++j){
            if(v[i][j].get_re() != 0   ||  v[i][j].get_im() != 0 ){
                if(start == NULL){
                    start = new node;
                    start->linie = i;
                    start->coloana = j;
                    start->numar.set_re(v[i][j].get_re());
                    start->numar.set_im(v[i][j].get_im());
                    start->next=NULL;
                }else{
                    node *p=start;
                    while(p->next != NULL)
                        p = p->next;

                    node *q = new node;
                    q->linie = i;
                    q->coloana = j;
                    q->numar.set_re(v[i][j].get_re());
                    q->numar.set_im(v[i][j].get_im());
                    q->next=NULL;

                    p->next = q;
                }
            }
        }
}

Matrice::~Matrice() {
    if(start != NULL){
        node *p = start;
        node *q = p->next;
        while(q->next !=NULL){
            delete p;
            p=q;
            q=q->next;
        }
        delete p;
        delete q;
    }
}

node* Matrice::get_start() {
    return start;
}




class Matrice_oarecare: public Matrice{
    int linie,coloana;
public:
    void set_linie(int);
    void set_coloana(int);
    Matrice_oarecare(complex **v,int linie,int coloana) : Matrice(v,linie,coloana) {};
    virtual void afisare();
};

void Matrice_oarecare::set_linie(int x) {
    linie = x;
}

void Matrice_oarecare::set_coloana(int x) {
    linie = x;
}

void Matrice_oarecare::afisare() {
    node*p = get_start();
    while(p->next != NULL){
        cout<<p->linie<<"  "<<p->coloana<<"  "<<p->numar<<endl;
        p=p->next;
    }
    cout<<p->linie<<"  "<<p->coloana<<"  "<<p->numar<<endl;
}


class Matrice_patratica: public Matrice{
    int dim;
public:
    Matrice_patratica(complex **v,int dim) : Matrice(v,dim,dim) {} ;
    virtual void afisare();
    void set_dim(int);
};

void Matrice_patratica::set_dim(int x) {
    dim = x;
}

void Matrice_patratica::afisare() {
    node*p = get_start();
    while(p->next != NULL){
        cout<<p->linie<<"  "<<p->coloana<<"  "<<p->numar<<endl;
        p=p->next;
    }
    cout<<p->linie<<"  "<<p->coloana<<"  "<<p->numar<<endl;

    complex matrix[10][10];
    for(int i=0;i<dim;i++)
        for(int j=0;j<dim;j++){
            matrix[i][j].set_re(0);
            matrix[i][j].set_im(0);
        }

    p=get_start();
    while(p->next != NULL){
        int i = p->linie;
        int j = p->coloana;
        matrix[i][j] = p->numar;
        p = p->next;
    }
    int i = p->linie;
    int j = p->coloana;
    matrix[i][j] = p->numar;

    cout<<determinant(matrix,dim);

}




int main() {

    ifstream f("date.in");


    complex** v = new complex*[3];
    for(int i = 0;i<3;i++)
        v[i] = new complex[3];

    for(int i=0;i<3;i++) {
        for (int j = 0; j < 3; j++) {
            f>>v[i][j];
            cout << v[i][j]<<"     ";
        }
        cout<<endl;
    }
    cout<<endl;


    Matrice_oarecare M(v,3,3);

    M.afisare();

    cout<<endl<<endl;

    Matrice_patratica P(v,3);
    P.set_dim(3);

    P.afisare();


    return 0;
}