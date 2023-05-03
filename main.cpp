#include <iostream>
#include <cstdio>
#include <cstdlib>
using namespace std;

//cream un struct corespunzator pentru tipul de nod
struct splay
{
    int cheie;
    splay* stanga;
    splay* dreapta;
};

class SplayTree
{
public:
    SplayTree()
    {
    }

    //rotatie spre dreapta
    splay* RightRotation(splay* k2)
    {
        splay* k1 = k2->stanga;
        k2->stanga = k1->dreapta;
        k1->dreapta = k2;
        return k1;
    }

   //rotatie spre stanga
    splay* LeftRotation(splay* k2)
    {
        splay* k1 = k2->dreapta;
        k2->dreapta = k1->stanga;
        k1->stanga = k2;
        return k1;
    }

    //implementare top-down splay tree
    splay* Splay(int key, splay* root)
    {
        if (!root)
            return NULL;
        splay tata;
        //tata.dreapta -> splay tree-ul stang
        //tata.stanga -> splay tree-ul drept
        tata.stanga = tata.dreapta = NULL;
        splay* LeftTreeMax = &tata;
        splay* RightTreeMin = &tata;
        while (1)
        {
            if (key < root->cheie)
            {
                if (!root->stanga)
                    break;
                if (key < root->stanga->cheie)
                {
                    //rotatie de tip zig-zig, cu o singura rotatie spre dreapta
                    root = RightRotation(root);
                    if (!root->stanga)
                        break;
                }
                //construirea splay tree-ului (pastrarea legaturilor)
                RightTreeMin->stanga = root;
                RightTreeMin = RightTreeMin->stanga;
                root = root->stanga;
                RightTreeMin->stanga = NULL;
            }
            else if (key > root->cheie)
            {
                if (!root->dreapta)
                    break;
                if (key > root->dreapta->cheie)
                {
                    //rotatie de tip zag-zag, cu o singura rotatie spre stanga
                    root = LeftRotation(root);
                    if (!root->dreapta)
                        break;
                }
                //construirea splay-tree-ului (pastrarea legaturilor)
                LeftTreeMax->dreapta = root;
                LeftTreeMax = LeftTreeMax->dreapta;
                root = root->dreapta;
                LeftTreeMax->dreapta = NULL;
            }
            else
                break;
        }
        //Construirea Splay Tree Final, conectand cele 3 parti (left tree, middle tree, right tree)
        LeftTreeMax->dreapta = root->stanga;
        RightTreeMin->stanga = root->dreapta;
        root->stanga = tata.dreapta;
        root->dreapta = tata.stanga;
        return root;
    }

    //metoda pentru crearea de nod nou
    splay* New_Node(int key)
    {
        splay* p_node = new splay;
        if (!p_node)
        {
            fprintf(stderr, "Out of memory!\n");
            exit(1);
        }
        p_node->cheie = key;
        p_node->stanga = p_node->dreapta = NULL;
        return p_node;
    }

    splay* Insert(int cheie, splay* radacina)
    {
        static splay* nod_curent = NULL;
        if (!nod_curent)
            nod_curent = New_Node(cheie);
        else
            nod_curent->cheie = cheie;
        //daca gasim un nod care nu e radacina, adaugam nodul
        if (!radacina)
        {
            radacina = nod_curent;
            nod_curent = NULL;
            return radacina;
        }
        radacina = Splay(cheie, radacina);

        //verificam daca nodurile sunt pozitionate corect, le repozitionam daca cheia curenta ( < sau > ) cheia radacinii
        if (cheie < radacina->cheie)
        {
            nod_curent->stanga = radacina->stanga;
            nod_curent->dreapta = radacina;
            radacina->stanga = NULL;
            radacina = nod_curent;
        }
        else if (cheie > radacina->cheie)
        {
            nod_curent->dreapta = radacina->dreapta;
            nod_curent->stanga = radacina;
            radacina->dreapta = NULL;
            radacina = nod_curent;
        }
        else
            return radacina;
        nod_curent = NULL;
        return radacina;
    }

    splay* Delete(int cheie, splay* radacina)
    {
        splay* x;
        //daca e radacina returnam
        if (!radacina)
            return NULL;
        radacina = Splay(cheie, radacina);

        //daca nu gasim cheia, cautam in continuare in radacina
        if (cheie != radacina->cheie)
            return radacina;
        else
        {
            //daca gasim nodul respectiv, pastram legaturile stanga,dreapta(daca exista), legandu-le in continuare
            if (!radacina->stanga)
            {
                x = radacina;
                radacina = radacina->dreapta;
            }
            else
            {
                x = radacina;
                radacina = Splay(cheie, radacina->stanga);
                radacina->dreapta = x->dreapta;
            }
            //eliberam memoria
            free(x);
            return radacina;
        }
    }

    splay* Search(int cheie, splay* radacina)
    {
        return Splay(cheie, radacina);
    }

    //afisare de tip in ordine(stanga,radacina,dreapta);
    void InOrder(splay* root)
    {
        if (root)
        {
            InOrder(root->stanga);
            cout<< "cheie: " <<root->cheie;
            if(root->stanga)
                cout<< " | stanga: "<< root->stanga->cheie;
            if(root->dreapta)
                cout << " | dreapta: " << root->dreapta->cheie;
            cout<< "\n";
            InOrder(root->dreapta);
        }
    }
};

int main()
{
    SplayTree tree;
    int vector[10] = {9,8,7,6,5,4,3,2,1,0};
    splay *root;
    root = NULL;
    const int length = 10;
    int i;
    for(i = 0; i < length; i++)
        root = tree.Insert(vector[i], root);
    cout<<"\nInOrder: \n";
    tree.InOrder(root);
    int input, choice;
    while(1)
    {
        cout<<"\nOptions:\n";
        cout<<"1. Insert "<<endl;
        cout<<"2. Delete"<<endl;
        cout<<"3. Search"<<endl;
        cout<<"4. Exit"<<endl;
        cout<<"Alege o optiune!";
        cin>>choice;
        switch(choice)
        {
            case 1:
                cout<<"Ce valoare vreti sa inserati? ";
                cin>>input;
                root = tree.Insert(input, root);
                cout<<"\nDupa inserare "<<input<<endl;
                tree.InOrder(root);
                break;
            case 2:
                cout<<"Ce valoare doriti sa stergeti?";
                cin>>input;
                root = tree.Delete(input, root);
                cout<<"\nDupa stergere: "<<input<<endl;
                tree.InOrder(root);
                break;
            case 3:
                cout<<"Ce valoare e cautata? ";
                cin>>input;
                root = tree.Search(input, root);
                cout<<"\nDupa Cautare "<<input<<endl;
                tree.InOrder(root);
                break;

            case 4:
                exit(1);
            default:
                cout<<"\nOptiune indisponibila \n";
        }
    }
    cout<<"\n";
    return 0;
}