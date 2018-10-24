/*Aceasta tema a presupus simularea transformarii unui NFA in DFA
  Programul primeste la intrare datele unui NFA iar apoi il transforma in DFA
  La iesire sunt scrise datele DFA-ului
  
  Documentatie
  
  DFA - https://en.wikipedia.org/wiki/Deterministic_finite_automaton
  NFA - https://en.wikipedia.org/wiki/Nondeterministic_finite_automaton
  NFA to DFA - https://en.wikipedia.org/wiki/Powerset_construction
  STL Vector - http://www.cplusplus.com/reference/vector/vector/
  STL Queue - http://www.cplusplus.com/reference/queue/queue/
 */

#include <iostream>
#include <fstream>
#include <string.h>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

ifstream fin("date1.in");
ofstream g("dfa.out");

struct Nod{
    int stare,stareNoua;
    char litera[101];
    Nod *next;
};

struct DFA
{
   vector<vector<int> > stari;
   vector<vector<int> > dfa1;
   vector<vector<int> > dfa2;
   vector<vector<int> > stariFinale;
   vector<char*> litere;
};

class NFA{
    int nrStari;
    int nrLitere;
    int nrTranzitii;
    int nrFinale;
    int stareInitiala;
    int stari[101];
    int stariFinale[101];
    char litere[101][101];
    char ch;
    Nod *v[101];
public:
///setter
    void set_states_nr(int x){nrStari=x;}
    void set_letters_nr(int x){nrLitere=x;}
    void set_init_state(int x){stareInitiala=x;}
    void set_final_states_nr(int x){nrFinale=x;}
    void set_trans_nr(int x){nrTranzitii=x;}
    void set_transitions(){for(int i=0;i<50;i++) v[i]=NULL;}
///getter
    int get_states_nr(){return nrStari;}
    int get_letter_nr(){return nrLitere;}
    int get_final_states_nr(){return nrFinale;}
    int get_trans_nr(){return nrTranzitii;}
    int get_init_state(){return stareInitiala;}
    int get_states(int i){return stari[i];}
    char* get_alphabet(int i){return litere[i];}
    int get_final_states(int i){return stariFinale[i];}
///metode
    int convertToInt(char a){
        if(a>='a' && a<='z')
            return a-'a';
        else if(a>='A' && a<='Z')
            return a-'A';
        else return a-'0';
    }

    void readStates(){
        char a;
        for(int i=0;i<nrStari;i++)
        {
            fin>>a;
            stari[i]=convertToInt(a);
        }
    }
    void readAlphabet(){ ///citim alfabetul
        char a;
        for(int i=0;i<nrLitere;i++)
        {
            fin>>litere[i];
        }
    }

    void readFinalStates(){ ///citim starile finale si le convertim la int
        char a;
        for(int i=0;i<nrFinale;i++)
        {
            fin>>a;
            stariFinale[i]=convertToInt(a);
        }
    }

    void readTransitions(){ ///citim tranzitiile si le salvam intr--un vector de liste pentru fiecare stare
        char a,b,c[101];
        int x,y;
        for(int i=0;i<nrTranzitii;i++)
        {
            fin>>a>>b>>c; ///citim elementele tranzitiei (stare1, stare2, litera)
            x=convertToInt(a);
            y=convertToInt(b);
            Nod *p; ///initializam un nod p in care salvam tranzitia curenta
            p=new Nod;
            p->stare=x;
            p->stareNoua=y;
            strcpy(p->litera,c);
            p->next=NULL;
            if(v[x]==NULL) ///daca nu exista tranzitii pentru starea x, initializam prima tranzitie
                v[x]=p;
            else ///altfel o adaugam in lista de tranzitii pentru starea x
            {
                Nod *q=v[x];
                while(q->next!=NULL)
                    q=q->next;
                q->next=p;
            }
        }
    }

    bool checkStareFinala(int stare)
    {
        //cout<<"FINAL "<<stare<<'\n';
        for(int i=0;i<nrFinale;i++)
            if(stare==stariFinale[i]) return 1;
        //cout<<"NU"<<'\n';
        return 0;
    }

    bool checkCoada( vector<vector<int> > dfa, vector<int> p)
    {
       // cout<<"COADA: "<<dfa.size()<<'\n';
        for(int i=0;i<dfa.size();i++)
            {
                vector<int> vect = dfa[i];
                int ok=0;
    /*
                for(int j=0;j<vect.size();j++)
                       cout<<(char)(vect[j]+'a');
                cout<<" ";
    */
                if(vect.size()==p.size())
                    {
                        for(int j=0;j<vect.size();j++)
                            if(vect[j]!=p[j]) ok=1;
                        if(ok==0) return 1;
                    }
            }
       // cout<<'\n';
        return 0;
    }

    void afisDFACifre(DFA x)
    {
        int i,j;

        /// afis stari
        g<<x.stari.size()<<" ";
        for(i=0;i<x.stari.size();i++)
        {
            for(j=0;j<x.stari[i].size();j++)
                g<<(x.stari[i][j]);
            g<<" ";
        }
        g<<'\n';

        ///afis litere
        g<<nrLitere<<" ";
        for(i=0;i<nrLitere;i++) g<<litere[i]<<" ";
        g<<'\n';

        ///st initiala
        g<<(x.dfa1[0][0])<<'\n';

        ///st finale
        g<<x.stariFinale.size()<<" ";
        for(i=0;i<x.stariFinale.size();i++)
        {
            for(j=0;j<x.stariFinale[i].size();j++)
                g<<(x.stariFinale[i][j]);
            g<<" ";
        }
        g<<'\n';

        /// afis tranzitii
        g<<x.dfa1.size()-1<<'\n';
        for(i=1;i<x.dfa1.size();i++)
        {
           vector<int> vect = x.dfa1[i];

            for(j=0;j<vect.size();j++)
                    g<<(vect[j]);

            g<<" ";

            vect = x.dfa2[i];

            for(j=0;j<vect.size();j++)
                    g<<(vect[j]);

            g<<" ";
            g<<x.litere[i];

            g<<'\n';
        }
    }

    void afisDFALitere(DFA x)
    {
        int i,j;

        /// afis stari
        g<<x.stari.size()<<" ";
        for(i=0;i<x.stari.size();i++)
        {
            for(j=0;j<x.stari[i].size();j++)
                g<<char(x.stari[i][j]+'a');
            g<<" ";
        }
        g<<'\n';

        ///afis litere
        g<<nrLitere<<" ";
        for(i=0;i<nrLitere;i++) g<<litere[i]<<" ";
        g<<'\n';

        ///st initiala
        g<<char(x.dfa1[0][0]+'a')<<'\n';

        ///st finale
        g<<x.stariFinale.size()<<" ";
        for(i=0;i<x.stariFinale.size();i++)
        {
            for(j=0;j<x.stariFinale[i].size();j++)
                g<<char(x.stariFinale[i][j]+'a');
            g<<" ";
        }
        g<<'\n';

        /// afis tranzitii
        g<<x.dfa1.size()-1<<'\n';
        for(i=1;i<x.dfa1.size();i++)
        {
           vector<int> vect = x.dfa1[i];

            for(j=0;j<vect.size();j++)
                    g<<char(vect[j]+'a');

            g<<" ";

            vect = x.dfa2[i];

            for(j=0;j<vect.size();j++)
                    g<<char(vect[j]+'a');

            g<<" ";
            g<<x.litere[i];

            g<<'\n';

        }
    }

    void NFAtoDFA(char ch)
    {
        queue<vector<int> > coada;
        DFA x;
        vector<int> p,stare,p2;

        p.push_back(stareInitiala);
        coada.push(p);
        x.dfa2.push_back(p);
        x.dfa1.push_back(p);
        x.litere.push_back("0");
        x.stari.push_back(p);

        p.clear();
        p2.clear();

        if(checkStareFinala(stareInitiala))
            x.stariFinale.push_back(p);

        while(!coada.empty())
        {
            stare = coada.front();
    /*
            cout<<"Stare: ";
            for(int k=0;k<stare.size();k++) cout<<(char)(stare[k]+'a');
            cout<<'\n';
    */

            for(int j=0;j<nrLitere;j++)
               {
                   //cout<<"Litera: "<<litere[j]<<" "<<stare.size()<<'\n';
                    bool stareF = 0;
                    for(int i=0;i<stare.size();i++)
                        {
                            int stareActuala = stare[i];
                           // cout<<"StareActuala : "<<stareActuala<<'\n';

                            Nod *q=v[stareActuala];

                            if(q!=NULL)
                               {
                                    while(q!=NULL && strcmp(q->litera,litere[j])<0) q=q->next;
                                    if(q!=NULL && strcmp(q->litera,litere[j])==0)
                                       {
                                           //cout<<q->litera;
                                           while(q!=NULL && strcmp(q->litera,litere[j])==0)
                                                {
                                                    if(checkStareFinala(q->stareNoua))
                                                        stareF=1;
                                                    p.push_back(q->stareNoua);
                                                    q=q->next;
                                                }

                                        }
                                }


                        }

                        p2.clear();
                        sort(p.begin(),p.end());
                        copy(p.begin(),unique(p.begin(),p.end()),back_inserter(p2));

    /*
                        for(int k=0;k<p2.size();k++) cout<<(char)(p2[k]+'a');
                        cout<<'\n';
    */
                        if(!p2.empty())
                        {
                            if(!checkCoada(x.stari,p2))
                            {
                                coada.push(p2);
                                x.stari.push_back(p2);
                                if(stareF==1) x.stariFinale.push_back(p2);
                            }
                            x.dfa2.push_back(p2);
                            x.dfa1.push_back(stare);
                            x.litere.push_back(litere[j]);
                        }
                        p.clear();
                        p2.clear();
               }

            coada.pop();
        }

        if(ch>='0' && ch<='9') afisDFACifre(x);
            else afisDFALitere(x);

    }
};

int main()
{
    NFA automat;
    int x;
    char a;
    fin>>x;
    automat.set_states_nr(x);
    automat.readStates(); ///citire stari

    /*cout<<automat.get_states_nr();
    for(int i=0;i<automat.get_states_nr();i++)
        cout<<" "<<automat.get_states(i);*/

    fin>>x;
    automat.set_letters_nr(x);
    automat.readAlphabet(); ///citire alfabet

    /*cout<<endl<<automat.get_letter_nr();
    for(int i=0;i<automat.get_letter_nr();i++)
        cout<<" "<<automat.get_alphabet(i);*/

    fin>>a;
    x=automat.convertToInt(a);
    automat.set_init_state(x); ///citire stare initiala

    //cout<<endl<<automat.get_init_state();

    fin>>x;
    automat.set_final_states_nr(x);
    automat.readFinalStates(); ///citire stari finale

    /*cout<<endl<<automat.get_final_states_nr();
    for(int i=0;i<automat.get_final_states_nr();i++)
    cout<<" "<<automat.get_final_states(i);*/

    fin>>x;
    automat.set_trans_nr(x);
    automat.set_transitions();
    automat.readTransitions(); ///citire tranzitii

    automat.NFAtoDFA(a);

    return 0;
}
