// bst e array con nodi compresi tra partenza e arrivo
// funziona ma lento nella malloc
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_AUTO 512

// struttura per rappresentare una stazione
typedef struct {
    int distanza;
    int auto_max;
    int autonomie_auto[MAX_AUTO];
} Stazione;

// struttura per rappresentare un nodo dell'albero (bst)
typedef struct TreeNode {
    Stazione stazione;
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

// lista per trovare i nodi raggiungibili da un certo nodo, non tengo traccia del padre
typedef struct NodePercorso {
    int valore;
    struct NodePercorso* next;
} NodePercorso;

// struttura per costruire l'array di raggiungibilità
typedef struct NodoArray {
    Stazione stazione;
    struct NodoArray* padre;
} NodoArray;

// nodo di un albero per trovare il percorso minimo, con riferimento al padre, usato per il percorso all'indietro
typedef struct ListNode {
    int indice;
    struct ListNode* right;
    struct ListNode* left;
} ListNode;

// lista di bst per tenere traccia dei vari "livelli" di ricerca, usato per il percorso all'indietro
typedef struct BstList {
    ListNode* bst;
    struct BstList* next;
} BstList;



//*** FUNZIONI PER GESTIONE ALBERO CON STAZIONI ***//

// crea nodo dell'albero con le stazioni
TreeNode* createNode(Stazione stazione) {
    TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));
    newNode->stazione = stazione;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

// inserisce un nodo nell'albero
TreeNode* insertNode(TreeNode* root, Stazione stazione) {
    if (root == NULL) {
        printf("aggiunta\n");
        return createNode(stazione);
    }

    if (stazione.distanza < root->stazione.distanza) {
        root->left = insertNode(root->left, stazione);
    } else if (stazione.distanza > root->stazione.distanza) {
        root->right = insertNode(root->right, stazione);
    } else {
        printf("non aggiunta\n");
    }

    return root;
}

// trova l'elemento minimo nell'albero
TreeNode* findMin(TreeNode* node) {
    TreeNode* current = node;
    while (current->left != NULL) {
        current = current->left;
    }
    return current;
}

// cancella una stazione dall'albero
TreeNode* deleteNode(TreeNode* root, int distanza) {
    if (root == NULL) {
        printf("non demolita\n");
        return root;
    }

    if (distanza < root->stazione.distanza) {
        root->left = deleteNode(root->left, distanza);
    } else if (distanza > root->stazione.distanza) {
        root->right = deleteNode(root->right, distanza);
    } else {
        if (root->left == NULL) {
            TreeNode* temp = root->right;
            //cancellaElementoLista(&root->stazione.stazioni_adiacenti, distanza);
            free(root);
            printf("demolita\n");
            return temp;
        } else if (root->right == NULL) {
            TreeNode* temp = root->left;
            //cancellaElementoLista(&root->stazione.stazioni_adiacenti, distanza);
            free(root);
            printf("demolita\n");
            return temp;
        }

        TreeNode* temp = findMin(root->right);
        root->stazione = temp->stazione;
        root->right = deleteNode(root->right, temp->stazione.distanza);
    }

    return root;
}

// cerca un nodo nell'albero
TreeNode* searchNode(TreeNode* root, int distanza) {
    if (root == NULL || root->stazione.distanza == distanza) {
        return root;
    }

    if (distanza < root->stazione.distanza) {
        return searchNode(root->left, distanza);
    } else {
        return searchNode(root->right, distanza);
    }
}

// stampa l'albero in ordine crescente
void inorderTraversal(TreeNode* root) {
    if (root == NULL) {
        return;
    }

    inorderTraversal(root->left);
    printf("%d ", root->stazione.distanza);
    inorderTraversal(root->right);
}

// elimina l'albero
void freeTree(TreeNode* root) {
    if (root != NULL) {
        freeTree(root->left);
        freeTree(root->right);
        free(root);
    }
}



//*** FUNZIONI PER GESTIONE LISTA SENZA PADRE PER IL PERCORSO ***//

// inserisce un nodo nella lista senza padre
void inserisciInTesta(NodePercorso ** testa, int valore) {
    NodePercorso * nuovoNodo = (NodePercorso *)malloc(sizeof(NodePercorso));
    nuovoNodo->valore = valore;
    nuovoNodo->next = *testa;
    *testa = nuovoNodo;
}

// Funzione per liberare la memoria occupata dalla lista senza padre
void freeListPercorso(NodePercorso* head) {
    NodePercorso* current = head;
    while (current != NULL) {
        NodePercorso* temp = current;
        current = current->next;
        free(temp);
    }
}

// Funzione per stampare tutti gli elementi della lista percorso
void stampaListaPercorso(NodePercorso * head) {
    NodePercorso * current = head;
    while (current != NULL) {
        printf("%d", current->valore);
        current = current->next;
        if (current != NULL) {
            printf(" ");
        }
    }
    printf("\n");
}



//*** FUNZIONI PER GESTIONE DEL BST DI INDICI PER IL PERCORSO ALL'INDIETRO ***//
// Nota: gli indici sono messi nel bst in ordine crescente; in un array ordinato in modo decrescente, l'indice piu
// piccolo corrisponde a un nodo piu grande.

// Funzione per creare un nuovo nodo
ListNode* creaNodo(int indice) {
    ListNode* nuovoNodo = (ListNode*)malloc(sizeof(ListNode));
    nuovoNodo->indice = indice;
    nuovoNodo->right = NULL;
    nuovoNodo->left = NULL;
    return nuovoNodo;
}

// Funzione per inserire un nodo in nel bst di indici
void inserisciNodo(ListNode** root, int indice) {
    // Se l'albero è vuoto, crea un nuovo nodo come radice
    if (*root == NULL) {
        *root = creaNodo(indice);
        return;
    }

    // Se la distanza del nodo corrente è uguale a quella del nodo da inserire, non fare nulla
    if ((*root)->indice == indice) {
        return;
    }

    // Altrimenti, ricorre ricorsivamente sulla sottoalbero destro o sinistro
    if (indice < (*root)->indice) {
        inserisciNodo(&((*root)->left), indice);  // Inserimento nel sottoalbero sinistro
    } else {
        inserisciNodo(&((*root)->right), indice); // Inserimento nel sottoalbero destro
    }
}

// Stampa l'albero in-order
void stampaAlbero(ListNode* root) {
    if (root != NULL) {
        stampaAlbero(root->left);
        printf("%d ", root->indice);
        stampaAlbero(root->right);
    }
}

// Funzione di utilità per cercare un elemento nell'albero binario di ricerca
ListNode* search(ListNode* root, int indice) {
    if (root == NULL || root->indice == indice)
        return root;

    if (indice < root->indice)
        return search(root->left, indice);
    else
        return search(root->right, indice);
}

// Funzione per trovare l'elemento precedente dato un BST e un elemento contenuto in esso
ListNode* findPredecessor(ListNode* root, int key) {
    ListNode* current = search(root, key);
    if (current == NULL)
        return NULL; // L'elemento non è presente nell'albero

    // Se il nodo ha un sottoalbero sinistro, allora l'elemento precedente sarà
    // il nodo più a destra di tale sottoalbero sinistro
    if (current->left != NULL) {
        current = current->left;
        while (current->right != NULL)
            current = current->right;
        return current;
    }

    // Se il nodo non ha un sottoalbero sinistro, dobbiamo risalire l'albero
    // finché non troviamo un nodo il cui sottoalbero destro contenga l'elemento dato
    ListNode* predecessor = NULL;
    while (root != NULL) {
        if (key > root->indice) {
            predecessor = root;
            root = root->right;
        } else if (key < root->indice) {
            root = root->left;
        } else {
            break; // Abbiamo trovato l'elemento dato
        }
    }

    return predecessor;
}

// Funzione per deallocare la lista
void liberaAlberoIndici(ListNode* testa) {
    if (testa != NULL) {
        liberaAlberoIndici(testa->left);
        liberaAlberoIndici(testa->right);
        free(testa);
    }
}



//*** FUNZIONE PER GESTIRE LA LISTA DI BST  PER IL PERCORSO ALL'INDIETRO ***//

// inserisce un bst nella lista di bst
BstList* inserisciBst(BstList* head, ListNode* bst) {
    BstList* newList = (BstList*)malloc(sizeof(BstList));
    newList->bst = bst;
    newList->next = NULL;

    if (head == NULL) {
        // La lista head è vuota, quindi il nuovo nodo diventa la testa
        return newList;
    }

    // Scorrere la lista head fino all'ultimo elemento
    BstList* current = head;
    while (current->next != NULL) {
        current = current->next;
    }

    // Collegare il nuovo nodo alla fine della lista
    current->next = newList;

    return head;
}

// Funzione per liberare la memoria occupata dalla lista BstList
void freeBstList(BstList* head) {
    BstList* current = head;
    while (current != NULL) {
        BstList* temp = current;
        current = current->next;
        liberaAlberoIndici(temp->bst); // Libera la memoria occupata dalla lista ListNode
        free(temp);
    }
}

// funzione per stampare la lista di liste
void stampaLista(BstList* head) {
    BstList* current = head;
    while (current != NULL) {
        printf("albero: ");
        liberaAlberoIndici(current->bst);
        current = current->next;
    }
}



//*** FUNZIONI DI UTILITÀ PER IL PERCORSO ***//

// ritorna il numero di nodi compresi tra partenza e arrivo
int contaStazioni(struct TreeNode* root, int low, int high) {
    if (root == NULL) {
        return 0; // Caso base: albero vuoto o fine della ricerca.
    }

    // Se il nodo corrente rientra nel range, sommiamo il suo conteggio.
    if (root->stazione.distanza >= low && root->stazione.distanza <= high) {
        return 1 + contaStazioni(root->left, low, high) + contaStazioni(root->right, low, high);
    }
        // Se il nodo corrente è minore del valore più basso del range, dobbiamo cercare solo nel sottoalbero destro.
    else if (root->stazione.distanza < low) {
        return contaStazioni(root->right, low, high);
    }
        // Se il nodo corrente è maggiore del valore più alto del range, dobbiamo cercare solo nel sottoalbero sinistro.
    else {
        return contaStazioni(root->left, low, high);
    }
}

// Funzione ricorsiva per popolare l'array NodoArray in ordine crescente
void populateArrayAvanti(struct TreeNode* root, int low, int high, NodoArray* arr, int* index) {
    if (root == NULL) {
        return;
    }

    // Visita in-order del BST
    populateArrayAvanti(root->left, low, high, arr, index);

    if (root->stazione.distanza >= low && root->stazione.distanza <= high) {
        arr[*index].stazione = root->stazione;
        arr[*index].padre = NULL;
        (*index)++;
    }

    populateArrayAvanti(root->right, low, high, arr, index);
}

// Funzione ricorsiva per popolare l'array NodoArray in ordine decrescente
void populateArrayIndietro(struct TreeNode* root, int low, int high, NodoArray arr[], int* index) {
    if (root == NULL) {
        return;
    }

    // Visita in-order inverso del BST
    populateArrayIndietro(root->right, low, high, arr, index);

    if (root->stazione.distanza >= low && root->stazione.distanza <= high) {
        arr[*index].stazione = root->stazione;
        arr[*index].padre = NULL;
        (*index)++;
    }

    populateArrayIndietro(root->left, low, high, arr, index);
}


// Funzione per liberare l'array di tipo NodoArray
void freeArray(NodoArray** arr, int size) {
    for (int i = 0; i < size; i++) {
        free(arr[i]);
    }
    free(arr);
}

// Funzione di ricerca binaria per trovare l'indice del valore minore a quello cercato (se il valore cercato non è presente)
// restituisce -1 se il target è minore del primo elemento
int binarySearch(NodoArray* arr, int low, int high, int target) {
    int left = low;
    int right = high;
    int result = -1;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (arr[mid].stazione.distanza == target) {
            // Abbiamo trovato l'elemento target, salviamo l'indice e continuiamo la ricerca a sinistra per trovare il primo elemento uguale a target.
            result = mid;
            right = mid - 1;
            return result;
        } else if (arr[mid].stazione.distanza > target) {
            // L'elemento al centro è maggiore di target, quindi dobbiamo cercare a sinistra.
            right = mid - 1;
        } else {
            // L'elemento al centro è minore di target, quindi dobbiamo cercare a destra.
            left = mid + 1;
            // Aggiorniamo il risultato con l'indice dell'elemento più piccolo ma maggiore di target.
            result = mid;
        }
    }

    return result;
}

// Funzione di ricerca binaria per trovare l'indice del valore minore a quello cercato (se il valore cercato non è presente)
int binarySearchDecrescente(NodoArray** arr, int low, int high, int target) {
    int left = low;
    int right = high;
    int result = -1;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (arr[mid]->stazione.distanza == target) {
            // Abbiamo trovato l'elemento target, salviamo l'indice e continuiamo la ricerca a destra per trovare l'ultimo elemento uguale a target.
            result = mid;
            left = mid + 1;
            return result;
        } else if (arr[mid]->stazione.distanza > target) {
            // L'elemento al centro è maggiore di target, quindi dobbiamo cercare a destra.
            left = mid + 1;
            // Aggiorniamo il risultato con l'indice dell'elemento più grande ma minore di target.
            result = mid;
        } else {
            // L'elemento al centro è minore di target, quindi dobbiamo cercare a sinistra.
            right = mid - 1;
        }
    }

    return result;
}

// Funzione per cercare i nodi raggiungibili nel percorso all'indietro
void cercaRaggiungiliIndietro(NodoArray nodeArray[], int numeroNodi, int indice, int* indiceMinStazioneRaggiungibile, int* trovato, int arrivo, ListNode** indici) {
    int partenza = nodeArray[indice].stazione.distanza;
    int autonomia = nodeArray[indice].stazione.auto_max;

    // parto da indice+1 perché non considero l'elemento da cui parto
    // itero finchè la distanza di un nodo è maggiore della distanza di partenza - autonomia
    for (int i = indice + 1; nodeArray[i].stazione.distanza >= partenza - autonomia && i < numeroNodi; i++) {
        if (nodeArray[i].stazione.distanza == arrivo) {
            // se trovo l'arrivo, creo un nodo bst e lo aggiungo alla lista di bst
            *trovato = 1;
            nodeArray[i].padre = &nodeArray[indice];
            inserisciNodo(indici, i);
            return;
        }

        if (i > *indiceMinStazioneRaggiungibile){
            inserisciNodo(indici, i);
        }
        if (nodeArray[i].padre == NULL) {
            nodeArray[i].padre = &nodeArray[indice];
        }
        *indiceMinStazioneRaggiungibile = i;
    }
}

// trovo il l'inidice massimo del bst, in fondo a destra
ListNode* trovaIndiceMassimo(ListNode* root) {
    if (root == NULL) {
        // Se l'albero è vuoto, restituisce NULL
        return NULL;
    } else if (root->right == NULL) {
        // Se non ci sono sottoalberi a destra, il nodo corrente è il minimo
        return root;
    } else {
        // Altrimenti, continua a navigare verso destra per trovare il massimo
        return trovaIndiceMassimo(root->right);
    }
}



//*** FUNZIONI PER GESTIONE COMANDI RICEVUTI DA INPUT ***//

// aggiunge una stazione al bst
void aggiungiStazione(TreeNode** root, int distanza, int numero_auto, int autonomie[]) {
    Stazione stazione;
    stazione.distanza = distanza;
    stazione.auto_max = 0;

    // Inizializza la lista stazioni_adiacenti a NULL
    // stazione.stazioni_adiacenti = NULL;

    for (int i = 0; i < MAX_AUTO; i++) {
        if (i < numero_auto) {
            stazione.autonomie_auto[i] = autonomie[i];
            if (autonomie[i] > stazione.auto_max) {
                stazione.auto_max = autonomie[i];
            }
        } else {
            stazione.autonomie_auto[i] = -1;
        }
    }

    *root = insertNode(*root, stazione);
}

// aggiunge un'auto ad una stazione
void aggiungiAuto(TreeNode* stazione, int autonomia) {
    Stazione* staz = &(stazione->stazione);

    int i;

    for (i = 0; i < MAX_AUTO; i++) {
        if (staz->autonomie_auto[i] == -1) {
            staz->autonomie_auto[i] = autonomia;
            if (autonomia > staz->auto_max) {
                staz->auto_max = autonomia;
            }
            printf("aggiunta\n");
            break;
        }
    }

    if (i == MAX_AUTO) {
        printf("non aggiunta\n");
    }
}

// elimina un'auto da una stazione
void rottamaAuto(TreeNode* stazione, int autonomia) {
    Stazione* staz = &(stazione->stazione);

    int i;
    for (i = 0; i < MAX_AUTO; i++) {
        if (staz->autonomie_auto[i] == autonomia) {
            staz->autonomie_auto[i] = -1;
            if (autonomia == staz->auto_max) {
                staz->auto_max = 0;
                for (int j = 0; j < MAX_AUTO; j++) {
                    if (staz->autonomie_auto[j] > staz->auto_max) {
                        staz->auto_max = staz->autonomie_auto[j];
                    }
                }
            }
            printf("rottamata\n");
            break;
        }
    }

    if (i == MAX_AUTO) {
        printf("non rottamata\n");
    }
}

// Funzione per trovare il percorso minimo tra due stazioni in avanti
// Funzione per trovare il percorso minimo tra due stazioni in avanti
void cercaPercorso(TreeNode* root, int partenza, int arrivo) {
    int numeroNodi = contaStazioni(root, partenza, arrivo);

    // Creo un array di tipo NodoArray delle dimensioni numeroNodi
    NodoArray nodeArray[numeroNodi];

    // Popolo l'array con i nodi compresi tra partenza e arrivo in ordine crescente
    int i = 0;
    populateArrayAvanti(root, partenza, arrivo, nodeArray, &i);

    // loop principale in cui scorro tutto l'array
    int indiceMaxStazioneRaggiungibile;
    for (i = 0; i < numeroNodi - 1; i++) {
        indiceMaxStazioneRaggiungibile = binarySearch(nodeArray, 0, numeroNodi - 1,
                                                      nodeArray[i].stazione.distanza + nodeArray[i].stazione.auto_max);

        //printf("max ragg %d\n", indiceMaxStazioneRaggiungibile);
        if (indiceMaxStazioneRaggiungibile == i) { // se l'indice del massimo elemento raggiungibile è uguale a i significa che non posso raggiungere altri nodi
            if (nodeArray[i + 1].padre == NULL) { // se il successivo non ha nessun padre mi fermo
                printf("nessun percorso\n");
                return;
            }
        }

        if (indiceMaxStazioneRaggiungibile == numeroNodi - 1) { // se riesco ad arrivare all'ultimo nodo mi fermo
            // metto nell'ultimo nodo il padre ed evito di farlo per quelli prima
            nodeArray[numeroNodi - 1].padre = &(nodeArray[i]);
            break;
        }

        for (int j = indiceMaxStazioneRaggiungibile; j > 0 || nodeArray[j].padre != NULL; j--) {
            if (nodeArray[j].padre == NULL) {
                nodeArray[j].padre = &(nodeArray[i]);
            }
        }

    }

    NodoArray* cur = &(nodeArray[numeroNodi - 1]);
    NodePercorso* percorso = NULL;
    // costruisco la lista percorso risalendo con i padri
    while (cur->padre != NULL) {
        inserisciInTesta(&percorso, cur->stazione.distanza);
        cur = cur->padre;
    }
    // inserisco anche la partenza
    inserisciInTesta(&percorso, cur->stazione.distanza);

    stampaListaPercorso(percorso);
    freeListPercorso(percorso);

    // Poiché ora utilizziamo un array invece di un puntatore a puntatore, non è necessario liberare la memoria allocata per l'array nodeArray.
}

// Funzione per trovare il percorso minimo tra due stazioni all'indietro
void cercaPercorsoIndietro(TreeNode* root, int partenza, int arrivo){
    // in questa caso i valori devono essere scambiati perche arrivo < partenza
    int numeroNodi = contaStazioni(root, arrivo, partenza);

    // Creo un array di tipo NodoArray delle dimensioni numeroNodi
    NodoArray nodeArray[numeroNodi];

    // Popolo l'array con i nodi compresi tra partenza e arrivo in ordine decrescente
    int i = 0;
    populateArrayIndietro(root, arrivo, partenza, nodeArray, &i);


    int indiceMinStazioneRaggiungibile;
    int trovato = 0;

    ListNode* indici = NULL;
    cercaRaggiungiliIndietro(nodeArray, numeroNodi, 0, &indiceMinStazioneRaggiungibile, &trovato, arrivo, &indici);

    // se ho gia trovato l'arrivo alla prima iterazione
    if (trovato == 1){
        printf("%d %d\n", partenza, arrivo);
        liberaAlberoIndici(indici);
        //freeArray(nodeArray, numeroNodi);
        return;
    }

    // se la lista di indici è vuota, non ho trovato nessun percorso
    if (indici == NULL){
        printf("nessun percorso\n");
        //freeArray(nodeArray, numeroNodi);
        return;
    }

    // creo una lista di liste indici
    BstList* listaBst = NULL;
    // aggiungo la lista di indici alla lista di liste
    listaBst = inserisciBst(listaBst, indici);
    BstList* current = listaBst;
    int x = 0;
    while(current != NULL && !trovato){
        ListNode* nuovaListaIndici = NULL;
        ListNode* curr = trovaIndiceMassimo(current->bst);

        while (curr != NULL && !trovato){
            cercaRaggiungiliIndietro(nodeArray, numeroNodi, curr->indice, &indiceMinStazioneRaggiungibile, &trovato, arrivo, &nuovaListaIndici);
            curr = findPredecessor(current->bst, curr->indice);
        }

        if (nuovaListaIndici != NULL){
            listaBst = inserisciBst(listaBst, nuovaListaIndici);
        }
        current = current->next;
        x++;
    }

    if (current == NULL){
        printf("nessun percorso\n");
        freeBstList(listaBst);
        //freeArray(nodeArray, numeroNodi);
        return;
    }

    if (trovato){
        // se ho trovato l'arrivo, risalgo con i padri e stampo il percorso
        NodePercorso* percorso = NULL;
        NodoArray* cur = &nodeArray[numeroNodi - 1]; // Aggiornato il puntatore cur

        while (cur->padre != NULL) {
            inserisciInTesta(&percorso, cur->stazione.distanza);
            cur = cur->padre;
        }

        inserisciInTesta(&percorso, cur->stazione.distanza);
        stampaListaPercorso(percorso);
        freeListPercorso(percorso);
        freeBstList(listaBst);
        //freeArray(nodeArray, numeroNodi);
    } else {
        printf("nessun percorso\n");
    }
}

int main() {
    TreeNode * root = NULL;

    char comando[20];
    int distanza, numero_auto, autonomia;
    int partenza, arrivo;
    int autonomie[MAX_AUTO];

    while ((scanf("%s", comando) != EOF)) {
        if (strcmp(comando, "aggiungi-stazione") == 0) {
            if(scanf ("%d %d", &distanza, &numero_auto) != EOF) {
                for (int i = 0; i < numero_auto; i++) {
                    if(scanf("%d", &autonomie [i]) != EOF){}
                }
                aggiungiStazione(&root, distanza, numero_auto, autonomie);
            }
        } else if (strcmp(comando, "demolisci-stazione") == 0) {
            if (scanf("%d", &distanza) != EOF) {
                root = deleteNode(root, distanza);
            }
        } else if (strcmp(comando, "aggiungi-auto") == 0) {
            if (scanf("%d %d", &distanza, &autonomia) != EOF) {
                TreeNode * stazione = searchNode(root, distanza);
                if (stazione != NULL) {
                    aggiungiAuto(stazione, autonomia);
                } else {
                    printf("non aggiunta\n");
                }
            }
        } else if (strcmp(comando, "rottama-auto") == 0) {
            if (scanf("%d %d", &distanza, &autonomia) != EOF) {
                TreeNode * stazione = searchNode(root, distanza);
                if (stazione != NULL) {
                    rottamaAuto(stazione, autonomia);
                } else {
                    printf("non rottamata\n");
                }
            }
        } else if (strcmp(comando, "pianifica-percorso") == 0) {
            if (scanf("%d %d", &partenza, &arrivo) != EOF) {
                if (partenza == arrivo) {
                    printf("%d\n", partenza);
                    continue;
                }
                if (partenza < arrivo) {
                    cercaPercorso(root, partenza, arrivo);
                } else {
                    cercaPercorsoIndietro(root, partenza, arrivo);
                }
            }
        }
    }

    return 0;
}