#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>
#include <algorithm>
#include <random>

using namespace std;

ifstream in ("date.in");

unsigned int baza_radix, nr_teste;

void afisare(unsigned int v[], unsigned int dim)
{
    for (int i = 0; i<dim; i++) cout<<v[i]<<" ";  cout<<"\n";
}

void radixsort(unsigned int v[],unsigned int base, unsigned int dim)
{
    int maxim = -1;

    for (int i = 0; i<dim; i++)
    {
        int c = v[i];
        if (maxim - c < 0)  maxim = v[i];
    }

    unsigned int *aux,p_base=1,*fcv;
    aux = new unsigned int[dim];
    fcv = new unsigned int[base];

    while(maxim/p_base!=0)
    {
        for (int i = 0; i<base; i++)    fcv[i]=0;
        for (int i = 0; i<dim; i++)    fcv[(v[i]/p_base)%base]++;
        for (int i = 1; i<base; i++)    fcv[i]+=fcv[i-1];
        for (int i = dim-1; i>=0; i--)  aux[fcv[(v[i]/p_base)%base]-1]=v[i],fcv[(v[i]/p_base)%base]--;
        for (int i = 0; i<dim; i++) v[i] = aux[i];
        p_base*=base;
    }
}

void mergesort(unsigned int v[],int st,int dr,unsigned int dim)
{
    if (st==dr) return;

    unsigned int m=st+(dr-st)/2,*aux;

    mergesort(v,st,m,dim);

    mergesort(v,m+1,dr,dim);

    aux = new unsigned int[dr-st+1];

    unsigned int i,j,k;

    i=st; j=m+1;    k=0;

    while (i<=m && j<=dr)
    {
        if (v[i]<=v[j]) aux[k++]=v[i++];

        else aux[k++]=v[j++];
    }

    while (i<=m)    aux[k++]=v[i++];

    while (j<=dr)   aux[k++]=v[j++];

    k=0;

    for (i=st;i<=dr;i++)    v[i]=aux[k++];

    delete[]aux;
}

void shellsort(unsigned int v[], unsigned int dim)
{
    for (unsigned int i = dim/2; i != 0; i = i>>1)
    {
        for (unsigned int j = i; j<dim; j++)
        {
            unsigned int aux = v[j],k;

            for (k = j; (k >= i) && (aux < v[k-i]); k -= i) v[k] = v[k - i];

            v[k] = aux;
        }
    }
}

void countsort(unsigned int v[], unsigned int dim)
{
    int maxim = -1;

    for (int i = 0; i<dim; i++)
    {
        int c = v[i];
        if (maxim - c < 0)  maxim = v[i];
    }

    unsigned int *aux,*fcv;
    fcv = new unsigned int[maxim+1];
    aux = new unsigned int[dim];

    for (int i = 0; i<=maxim; i++)    fcv[i]=0;
    for (int i = 0; i<dim; i++)    fcv[v[i]]++;
    for (int i = 1; i<=maxim; i++)    fcv[i]+=fcv[i-1];
    for (int i = dim-1; i>=0; i--)  aux[fcv[v[i]]-1]=v[i],fcv[v[i]]--;
    for (int i = 0; i<dim; i++) v[i] = aux[i];
}

int partitie (unsigned int v[],int st,int dr)
{
    int m=st;

    for (unsigned int i=st;i<=dr;i++)
        if (v[i]<v[dr]) swap(v[m++],v[i]);

    swap (v[dr],v[m]);

    return m;
}

void quicksort (unsigned int v[],int st, int dr)
{
    if (st>=dr) return;

    int m=partitie(v,st,dr);

    quicksort(v,st,m-1);

    quicksort(v,m+1,dr);
}

void c_sort(unsigned int v[], unsigned int dim)
{
    sort(v,v+dim);
}

bool corect(unsigned int v[], unsigned int dim)
{
    for (int i = 0; i<dim-1; i++)
        if (v[i]>v[i+1])    return false;
    return true;
}

void prepare(unsigned int v[], unsigned int cv[], unsigned int dim)
{
    for (int i = 0; i<dim; i++) v[i] = cv[i];
}

void sorts_compare(unsigned int v[], unsigned int dim)
{
    unsigned int *cv;
    cv = new unsigned int[dim];
    prepare(cv,v,dim);

    prepare(v,cv,dim);
    auto ts1 = chrono::steady_clock::now();
    c_sort(v,dim);
    auto ts2 = chrono::steady_clock::now();
    chrono::duration<double> ts = ts2-ts1;
    if (corect(v,dim))    cout<<"Sortul din C++: "<<ts.count()<<" secunde\n";
    else cout<<"Gresit!\n";

    prepare(v,cv,dim);
    auto tm1 = chrono::steady_clock::now();
    mergesort(v,0,dim-1,dim);
    auto tm2 = chrono::steady_clock::now();
    chrono::duration<double> tm = tm2-tm1;
    if (corect(v,dim))    cout<<"Mergesort: "<<tm.count()<<" secunde\n";
    else cout<<"Gresit!\n";

    prepare(v,cv,dim);
    auto tq1 = chrono::steady_clock::now();
    quicksort(v,0,dim-1);
    auto tq2 = chrono::steady_clock::now();
    chrono::duration<double> tq = tq2-tq1;
    if (corect(v,dim))    cout<<"Quick sort: "<<tq.count()<<" secunde\n";
    else cout<<"Gresit!\n";

    prepare(v,cv,dim);
    auto tc1 = chrono::steady_clock::now();
    countsort(v,dim);
    auto tc2 = chrono::steady_clock::now();
    chrono::duration<double> tc = tc2-tc1;
    if (corect(v,dim))    cout<<"Counting sort: "<<tc.count()<<" secunde\n";
    else cout<<"Gresit!\n";

    prepare(v,cv,dim);
    auto t1 = chrono::steady_clock::now();
    radixsort(v,baza_radix,dim);
    auto t2 = chrono::steady_clock::now();
    chrono::duration<double> t = t2-t1;
    if (corect(v,dim))    cout<<"Radixsort: "<<t.count()<<" secunde\n";
    else cout<<"Gresit!\n";

    prepare(v,cv,dim);
    auto ti1 = chrono::steady_clock::now();
    shellsort(v,dim);
    auto ti2 = chrono::steady_clock::now();
    chrono::duration<double> ti = ti2-ti1;
    if (corect(v,dim))    cout<<"Shellsort: "<<ti.count()<<" secunde\n";
    else cout<<"Gresit!\n";

    cout<<"\n";
}

void generator(unsigned int dim,unsigned int limita)
{
    unsigned int *v;
    v = new unsigned int[dim];

    srand((unsigned)time(NULL));

    for (int i = 0; i<dim; i++) v[i] = rand() % limita;

    sorts_compare(v,dim);
}

void rulari(unsigned int teste)
{
    unsigned int dim_vec, elem_max;

    for (int i = 0; i<teste; i++)
    {
        in>>dim_vec>>elem_max;
        cout<<"Testul "<<i+1<<": "<<dim_vec<<" numere naturale mai mici decat "<<elem_max<<":\n";
        generator(dim_vec,elem_max);
    }
}

int main()
{
    in>>baza_radix>>nr_teste;

    rulari(nr_teste);

    return 0;
}
