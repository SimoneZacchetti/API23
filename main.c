// red and black invece del bst primario per le stazioni
// lista di bst per tenere traccia dei vari "livelli" di ricerca FUNZIONANTE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_AUTO 512

// Definizione dei colori dei nodi
#define RED 0
#define BLACK 1

// struttura per rappresentare una stazione
typedef struct {
    int distanza;
    int auto_max;
    int autonomie_auto[MAX_AUTO];
} Stazione;

// Definizione della struttura del nodo dell'albero
typedef struct RBNode {
    Stazione stazione;
    struct RBNode* parent;
    struct RBNode* left;
    struct RBNode* right;
    int color;
} RBNode;


// nodo di una lista per trovare il percorso minimo, con riferimento al padre, provo a trasformarlo in un albero
typedef struct ListNode {
    Stazione* stazione;
    struct ListNode* padre;
    struct ListNode* right;
    struct ListNode* left;
} ListNode;

// lista per trovare i nodi raggiungibili da un certo nodo, non tengo traccia del padre
typedef struct NodePercorso {
    int valore;
    struct NodePercorso* next;
} NodePercorso;

// lista di bst per tenere traccia dei vari "livelli" di ricerca
typedef struct ListList {
    ListNode* list;
    struct ListList* next;
} ListList;




// *** FUNZIONI PER L'ALBERO PRIMARIO RB *** //

// Funzione ausiliaria per creare un nuovo nodo
RBNode* createRBNode(Stazione stazione) {
    RBNode* newRBNode = (RBNode*)malloc(sizeof(RBNode));
    newRBNode->stazione = stazione;
    newRBNode->parent = NULL;
    newRBNode->left = NULL;
    newRBNode->right = NULL;
    newRBNode->color = RED; // Inizialmente tutti i nodi sono rossi
    return newRBNode;
}

// Funzione ausiliaria per eseguire la rotazione a sinistra
void rotateLeft(RBNode** root, RBNode* x) {
    RBNode* y = x->right;
    x->right = y->left;
    if (y->left != NULL)
        y->left->parent = x;
    y->parent = x->parent;
    if (x->parent == NULL)
        *root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;
    y->left = x;
    x->parent = y;
}

// Funzione ausiliaria per eseguire la rotazione a destra
void rotateRight(RBNode** root, RBNode* x) {
    RBNode* y = x->left;
    x->left = y->right;
    if (y->right != NULL)
        y->right->parent = x;
    y->parent = x->parent;
    if (x->parent == NULL)
        *root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;
    y->right = x;
    x->parent = y;
}

// Funzione ausiliaria per eseguire la correzione dei colori dopo l'inserimento
void fixInsert(RBNode** root, RBNode* z) {
    while (z->parent != NULL && z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            RBNode* y = z->parent->parent->right;
            if (y != NULL && y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    rotateLeft(root, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rotateRight(root, z->parent->parent);
            }
        } else {
            RBNode* y = z->parent->parent->left;
            if (y != NULL && y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    rotateRight(root, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rotateLeft(root, z->parent->parent);
            }
        }
    }
    (*root)->color = BLACK;
}

// Funzione principale per l'inserimento di un nodo nell'albero
RBNode* insertNode(RBNode* root, Stazione stazione) {
    RBNode* z = createRBNode(stazione);
    RBNode* y = NULL;
    RBNode* x = root;
    while (x != NULL) {
        y = x;
        if (z->stazione.distanza == x->stazione.distanza) {
            printf("non aggiunta\n");
            free(z);  // Liberare la memoria del nodo non utilizzato
            return root;
        } else if (z->stazione.distanza < x->stazione.distanza) {
            x = x->left;
        } else {
            x = x->right;
        }
    }
    z->parent = y;
    if (y == NULL)
        root = z;
    else if (z->stazione.distanza < y->stazione.distanza)
        y->left = z;
    else
        y->right = z;
    fixInsert(&root, z);
    printf("aggiunta\n");
    return root;
}


// Funzione ausiliaria per la ricerca di un nodo con una distanza specifica nell'albero
RBNode* searchNode(RBNode* root, int distanza) {
    RBNode* current = root;
    while (current != NULL) {
        if (distanza == current->stazione.distanza)
            return current;
        else if (distanza < current->stazione.distanza)
            current = current->left;
        else
            current = current->right;
    }
    return NULL;
}

// Funzione ausiliaria per eseguire la correzione dei colori dopo l'eliminazione
void fixDelete(RBNode** root, RBNode* x) {
    if (x == NULL || *root == NULL) {
        return;
    }

    RBNode* sibling;
    while (x != *root && x->color == BLACK) {
        if (x == x->parent->left) {
            sibling = x->parent->right;
            if (sibling == NULL) {
                break;  // Il sibling è NULL, esco dal ciclo
            }

            if (sibling->color == RED) {
                sibling->color = BLACK;
                x->parent->color = RED;
                rotateLeft(root, x->parent);
                sibling = x->parent->right;
            }

            if ((sibling->right == NULL || sibling->right->color == BLACK) &&
                (sibling->left == NULL || sibling->left->color == BLACK)) {
                sibling->color = RED;
                x = x->parent;
            } else {
                if (sibling->right == NULL || sibling->right->color == BLACK) {
                    if (sibling->left != NULL) {
                        sibling->left->color = BLACK;
                    }
                    sibling->color = RED;
                    rotateRight(root, sibling);
                    sibling = x->parent->right;
                }
                sibling->color = x->parent->color;
                x->parent->color = BLACK;
                if (sibling->right != NULL) {
                    sibling->right->color = BLACK;
                }
                rotateLeft(root, x->parent);
                x = *root;
            }
        } else {
            sibling = x->parent->left;
            if (sibling == NULL) {
                break;  // Il sibling è NULL, esco dal ciclo
            }

            if (sibling->color == RED) {
                sibling->color = BLACK;
                x->parent->color = RED;
                rotateRight(root, x->parent);
                sibling = x->parent->left;
            }

            if ((sibling->left == NULL || sibling->left->color == BLACK) &&
                (sibling->right == NULL || sibling->right->color == BLACK)) {
                sibling->color = RED;
                x = x->parent;
            } else {
                if (sibling->left == NULL || sibling->left->color == BLACK) {
                    if (sibling->right != NULL) {
                        sibling->right->color = BLACK;
                    }
                    sibling->color = RED;
                    rotateLeft(root, sibling);
                    sibling = x->parent->left;
                }
                sibling->color = x->parent->color;
                x->parent->color = BLACK;
                if (sibling->left != NULL) {
                    sibling->left->color = BLACK;
                }
                rotateRight(root, x->parent);
                x = *root;
            }
        }
    }

    if (x != NULL) {
        x->color = BLACK;
    }
}

// Funzione ausiliaria per trovare il nodo minimo a partire da un nodo specifico
RBNode* findMinimumNode(RBNode* node) {
    while (node->left != NULL) {
        node = node->left;
    }
    return node;
}

// Funzione ausiliaria per sostituire un nodo con un altro nodo nell'albero
void transplant(RBNode** root, RBNode* u, RBNode* v) {
    if (u->parent == NULL) {
        *root = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }
    if (v != NULL) {
        v->parent = u->parent;
    }
}

// Funzione principale per eliminare un nodo dall'albero
RBNode* deleteNode(RBNode* root, int distanza) {
    RBNode* z = searchNode(root, distanza);
    if (z == NULL) {
        printf("non demolita\n");
        return root; // Il nodo da eliminare non è presente nell'albero
    }

    RBNode* x;
    RBNode* y = z;
    int yOriginalColor = y->color;

    if (z->left == NULL) {
        x = z->right;
        transplant(&root, z, z->right);
    } else if (z->right == NULL) {
        x = z->left;
        transplant(&root, z, z->left);
    } else {
        y = findMinimumNode(z->right);
        yOriginalColor = y->color;
        x = y->right;
        if (y->parent == z) {
            if (x != NULL) {
                x->parent = y;
            }
        } else {
            transplant(&root, y, y->right);
            y->right = z->right;
            if (y->right != NULL) {
                y->right->parent = y;
            }
        }
        transplant(&root, z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }

    if (yOriginalColor == BLACK) {
        fixDelete(&root, x);
    }

    free(z);
    printf("demolita\n");
    return root;
}


// Funzione ausiliaria per liberare la memoria dell'albero
void deleteTree(RBNode* root) {
    if (root == NULL) return;
    deleteTree(root->left);
    deleteTree(root->right);
    free(root);
}

// Funzione ausiliaria per stampare l'albero in ordine crescente
void inorderTraversal(RBNode* root) {
    if (root == NULL) return;
    inorderTraversal(root->left);
    printf("%d ", root->stazione.distanza);
    inorderTraversal(root->right);
}



// *** FUNZIONI PER GESTIONE DI ALBERO RB DI RAGGIUNGIBILITA *** //

// Crea un nodo dell'albero
ListNode* creaTreeNode(Stazione* stazione, ListNode* padre) {
    ListNode* newNode = (ListNode*)malloc(sizeof(ListNode));
    newNode->stazione = stazione;
    newNode->padre = padre;
    newNode->right = NULL;
    newNode->left = NULL;
    return newNode;
}

// Inserisce un nodo nell'albero in ordine crescente
void inserisciInOrdine(struct ListNode** root, Stazione* stazione, ListNode* padre) {
    // Se l'albero è vuoto, crea un nuovo nodo come radice
    if (*root == NULL) {
        *root = creaTreeNode(stazione, padre);
        return;
    }

    // Se la distanza del nodo corrente è uguale a quella del nodo da inserire, non fare nulla
    if ((*root)->stazione->distanza == stazione->distanza) {
        return;
    }

    // Altrimenti, ricorre ricorsivamente sulla sottoalbero sinistro o destro
    if (stazione->distanza < (*root)->stazione->distanza) {
        inserisciInOrdine(&((*root)->left), stazione, padre);
    } else {
        inserisciInOrdine(&((*root)->right), stazione, padre);
    }
}




// Stampa l'albero in-order
void stampaAlbero(ListNode* root) {
    if (root != NULL) {
        stampaAlbero(root->left);
        printf("%d ", root->stazione->distanza);
        if (root->padre != NULL) {
            printf("padre: %d \n", root->padre->stazione->distanza);
        } else {
            printf("padre: NULL \n");
        }
        stampaAlbero(root->right);
    }
}

// Dealloca l'albero
void freeAlbero(ListNode* root) {
    if (root != NULL) {
        freeAlbero(root->left);
        freeAlbero(root->right);
        free(root);
    }
}



//*** FUNZIONE PER GESTIRE LA LISTA DI LISTE ***//

// inserisce una lista nella lista di liste
ListList* inserisciLista(ListList* head, ListNode* list) {
    ListList* newList = (ListList*)malloc(sizeof(ListList));
    newList->list = list;
    newList->next = NULL;

    if (head == NULL) {
        // La lista head è vuota, quindi il nuovo nodo diventa la testa
        return newList;
    }

    // Scorrere la lista head fino all'ultimo elemento
    ListList* current = head;
    while (current->next != NULL) {
        current = current->next;
    }

    // Collegare il nuovo nodo alla fine della lista
    current->next = newList;

    return head;
}

// Funzione per liberare la memoria occupata dalla lista ListList
void freeListList(ListList* head) {
    ListList* current = head;
    while (current != NULL) {
        ListList* temp = current;
        current = current->next;
        freeAlbero(temp->list); // Libera la memoria occupata dalla lista ListNode
        free(temp);
    }
}

// funzione per stampare la lista di liste
void stampaLista(ListList* head) {
    ListList* current = head;
    while (current != NULL) {
        printf("albero: ");
        stampaAlbero(current->list);
        current = current->next;
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


//*** FUNZIONI PER GESTIONE COMANDI RICEVUTI DA INPUT ***//

// aggiunge una stazione al bst
void aggiungiStazione(RBNode** root, int distanza, int numero_auto, int autonomie[]) {
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
void aggiungiAuto(RBNode* stazione, int autonomia) {
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
void rottamaAuto(RBNode* stazione, int autonomia) {
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


// funzione per la ricerca di tutti i nodi ragguibili da una stazione in avanti
void cercaRaggiungibili(RBNode* node, ListNode** resultList, ListNode* partenza, int autonomia, int* trovato, int arrivo, int* massimo, ListNode** nodoArrivo) {
    if (node == NULL || *trovato){
        return;
    }

    if (node->stazione.distanza <= *massimo) {
        cercaRaggiungibili(node->right, resultList, partenza, autonomia, trovato, arrivo, massimo, nodoArrivo);
    } else if (node->stazione.distanza > partenza->stazione->distanza + partenza->stazione->auto_max){
        cercaRaggiungibili(node->left, resultList, partenza, autonomia, trovato, arrivo, massimo, nodoArrivo);
    } else {
        if (node->stazione.distanza <= partenza->stazione->distanza + autonomia && node->stazione.distanza > partenza->stazione->distanza) {
            if (node->stazione.distanza == arrivo){
                *trovato = 1;
                *nodoArrivo = creaTreeNode(&(node->stazione), partenza);
                return;
            }
            if (node->stazione.distanza > *massimo){
                if (*massimo < partenza->stazione->distanza){
                    *massimo = partenza->stazione->distanza;
                }
                inserisciInOrdine(resultList, &(node->stazione), partenza);
            }
        }
        cercaRaggiungibili(node->left, resultList, partenza, autonomia, trovato, arrivo, massimo, nodoArrivo);
        cercaRaggiungibili(node->right, resultList, partenza, autonomia, trovato, arrivo, massimo, nodoArrivo);
    }
}



// funzione per la ricerca di tutti i nodi ragguibili da una stazione all'indietro
void cercaRaggiungibiliIndietro(RBNode* node, ListNode** resultList, ListNode* partenza, int autonomia, int* trovato, int arrivo, int* minimo, ListNode** nodoArrivo) {
    if (node == NULL || *trovato) {
        return;
    }

    if (node->stazione.distanza >= *minimo) {
        cercaRaggiungibiliIndietro(node->left, resultList, partenza, autonomia, trovato, arrivo, minimo, nodoArrivo);
    } else if (node->stazione.distanza < partenza->stazione->distanza - partenza->stazione->auto_max){
        cercaRaggiungibiliIndietro(node->right, resultList, partenza, autonomia, trovato, arrivo, minimo, nodoArrivo);
    } else {
        if (node->stazione.distanza >= partenza->stazione->distanza - autonomia && node->stazione.distanza < partenza->stazione->distanza) {
            if (node->stazione.distanza == arrivo){
                *trovato = 1;
                *nodoArrivo = creaTreeNode(&(node->stazione), partenza);
                return;
            }
            if (node->stazione.distanza < *minimo){
                if (*minimo > partenza->stazione->distanza){
                    *minimo = partenza->stazione->distanza;
                }
                inserisciInOrdine(resultList, &(node->stazione), partenza);
            }
        }
        cercaRaggiungibiliIndietro(node->left, resultList, partenza, autonomia, trovato, arrivo, minimo, nodoArrivo);
        cercaRaggiungibiliIndietro(node->right, resultList, partenza, autonomia, trovato, arrivo, minimo, nodoArrivo);
    }

}


// funzione per trovare il nodo con distanza piu alta nella lista, usato nel percorso in avanti
int trovaMassimo(ListNode* list){
    if (list == NULL) return 0;

    ListNode * curr = list;
    while (curr->right != NULL){
        curr = curr->right;
    }

    return curr->stazione->distanza;
}

ListNode* trovaMinimo(ListNode* root) {
    if (root == NULL) {
        // Se l'albero è vuoto, restituisce NULL
        return NULL;
    } else if (root->left == NULL) {
        // Se non ci sono sottoalberi a sinistra, il nodo corrente è il minimo
        return root;
    } else {
        // Altrimenti, continua a navigare verso sinistra per trovare il minimo
        return trovaMinimo(root->left);
    }
}


// Funzione per trovare l'elemento successivo in un BST dato un elemento presente nel BST
ListNode* findSuccessor(ListNode* root, ListNode* element) {
    if (root == NULL) {
        return NULL; // L'albero è vuoto, non c'è un elemento successivo
    }

    // Trova il nodo con il valore dato all'interno dell'albero
    ListNode* current = root;
    ListNode * successor = NULL;

    while (current != NULL && current->stazione->distanza != element->stazione->distanza) {
        if (element->stazione->distanza < current->stazione->distanza) {
            successor = current; // Il nodo corrente potrebbe essere il successore
            current = current->left;
        } else {
            current = current->right;
        }
    }

    if (current == NULL) {
        return NULL; // L'elemento dato non è presente nell'albero
    }

    // Se il nodo ha un sottoalbero destro, trova il minimo in quel sottoalbero
    if (current->right != NULL) {
        return trovaMinimo(current->right);
    } else {
        // Altrimenti, l'elemento successivo sarà il primo nodo padre con valore maggiore
        return successor;
    }
}




// funzione che cerca il percorso minimo tra due stazioni in ordine crescente
void cercaPercorso(RBNode* root, int partenza, int arrivo){
    int massimo = 0, trovato = 0;

    RBNode * partenzaTreeNode = searchNode(root, partenza);
    ListNode * partenzaNode = creaTreeNode(&partenzaTreeNode->stazione, NULL);
    ListNode* raggiungibili = NULL;
    ListNode* nodoArrivo = NULL;

    cercaRaggiungibili(root, &raggiungibili, partenzaNode, partenzaTreeNode->stazione.auto_max, &trovato,
                       arrivo, &massimo, &nodoArrivo);

    // se non trovo nodi raggiungibili dalla partenza stampo nessun percorso
    if (raggiungibili == NULL){
        printf("nessun percorso\n");
        return;
    }

    // se trovo l'arrivo stampo il percorso e libero la memoria
    if (trovato){
        printf("%d %d\n", partenza, arrivo);
        freeAlbero(partenzaNode);
        freeAlbero(raggiungibili);
        return;
    }

    // creo la lista di liste
    ListList* head = NULL;
    // inserisco la prima lista dei raggiungibili
    head = inserisciLista(head, raggiungibili);

    // trovo il massimo dei raggiungibili
    massimo = trovaMassimo(raggiungibili);

    // scorro la lista di liste
    ListList * curr = head;

    while(curr != NULL && !trovato){
        ListNode * currTree = curr->list;
        ListNode * nuoviRaggiungibili = NULL;
        ListNode * piuPiccolo = trovaMinimo(currTree);

        while(piuPiccolo != NULL && !trovato){
            cercaRaggiungibili(root, &nuoviRaggiungibili, piuPiccolo, piuPiccolo->stazione->auto_max, &trovato, arrivo, &massimo, &nodoArrivo);
            piuPiccolo = findSuccessor(currTree, piuPiccolo);
        }
        if (nuoviRaggiungibili != NULL){
            massimo = trovaMassimo(nuoviRaggiungibili);
            head = inserisciLista(head, nuoviRaggiungibili);
        }
        curr = curr->next;
    }

    NodePercorso * percorso = NULL;
    // se trovo l'arrivo risalgo i padri dei nodi, stampo il percorso e libero la memoria
    if (trovato){
        while (nodoArrivo != NULL){
            inserisciInTesta(&percorso, nodoArrivo->stazione->distanza);
            nodoArrivo = nodoArrivo->padre;
        }
        stampaListaPercorso(percorso);
        freeListPercorso(percorso);
        freeListList(head);
        return;
    }

    // se non trovo l'arrivo stampo nessun percorso e libero la memoria
    if(curr == NULL){
        freeListList(head);
        printf("nessun percorso\n");
        return;
    }
}


// funzione che cerca il percorso minimo tra due stazioni in ordine decrescente
void cercaPercorsoIndietro(RBNode* root, int partenza, int arrivo){
    int minimo = INT_MAX, trovato = 0;

    RBNode * partenzaTreeNode = searchNode(root, partenza);
    ListNode * partenzaNode = creaTreeNode(&partenzaTreeNode->stazione, NULL);
    ListNode* raggiungibili = NULL;
    ListNode* nodoArrivo = NULL;

    cercaRaggiungibiliIndietro(root, &raggiungibili, partenzaNode, partenzaTreeNode->stazione.auto_max, &trovato, arrivo, &minimo, &nodoArrivo);
    // in raggiungibili ho tutti i nodi raggiungibili da partenza in un bst
    // se non trovo nodi raggiungibili dalla partenza stampo nessun percorso
    if (raggiungibili == NULL){
        printf("nessun percorso\n");
        return;
    }

    // se trovo l'arrivo stampo il percorso e libero la memoria
    if (trovato){
        printf("%d %d\n", partenza, arrivo);
        freeAlbero(partenzaNode);
        freeAlbero(raggiungibili);
        return;
    }

    // il minimo diventa il nodo con distanza minore della lista di raggiungibili
    minimo = raggiungibili->stazione->distanza;

    // creo la lista di liste
    ListList* head = NULL;
    // inserisco la prima lista dei raggiungibili
    head = inserisciLista(head, raggiungibili);

    //printf("primo albero raggiungibilità\n");
    //stampaAlbero(raggiungibili);
    // scorro la lista di liste
    ListList * curr = head;
    while(curr != NULL && !trovato){
        ListNode * currTree = curr->list;
        ListNode * nuoviRaggiungibili = NULL;
        ListNode * piuPiccolo = trovaMinimo(currTree);

        // scorro la lista dei nodi raggiungibili
        while(piuPiccolo != NULL && !trovato){
            cercaRaggiungibiliIndietro(root, &nuoviRaggiungibili, piuPiccolo, piuPiccolo->stazione->auto_max, &trovato, arrivo, &minimo, &nodoArrivo);
            //printf("nuovi raggiungibili\n");
            //stampaAlbero(nuoviRaggiungibili);
            //if (piuPiccolo != NULL) printf("piu piccolo %d\n", piuPiccolo->stazione->distanza); else printf("NULL\n");
            piuPiccolo = findSuccessor(currTree, piuPiccolo);
            //if (piuPiccolo != NULL) printf("piu piccolo %d\n", piuPiccolo->stazione->distanza); else printf("NULL\n");

        }
        if (nuoviRaggiungibili != NULL){
            minimo = nuoviRaggiungibili->stazione->distanza;
            head = inserisciLista(head, nuoviRaggiungibili);
            //printf("minimo %d\n", minimo);
            //stampaLista(head);
        }

        curr = curr->next;
    }

    NodePercorso * percorso = NULL;
    // se trovo l'arrivo risalgo i padri dei nodi, stampo il percorso e libero la memoria
    if (trovato){
        while (nodoArrivo != NULL){
            inserisciInTesta(&percorso, nodoArrivo->stazione->distanza);
            nodoArrivo = nodoArrivo->padre;
        }
        stampaListaPercorso(percorso);
        freeListPercorso(percorso);
        freeListList(head);
        return;
    }

    // se non trovo l'arrivo stampo nessun percorso e libero la memoria
    if(curr == NULL){
        freeListList(head);
        printf("nessun percorso\n");
        return;
    }
}


int main() {
    RBNode* root = NULL;
    FILE* file = stdin;

    char comando[20];
    int distanza, numero_auto, autonomia;
    int partenza, arrivo;
    int autonomie[MAX_AUTO];

    while ((fscanf(file, "%s", comando) != EOF)) {
        if (strcmp(comando, "aggiungi-stazione") == 0) {
            if(fscanf (file, "%d %d", &distanza, &numero_auto) != EOF) {
                for (int i = 0; i < numero_auto; i++) {
                    if(fscanf(file, "%d", &autonomie [i]) != EOF){}
                }
                aggiungiStazione(&root, distanza, numero_auto, autonomie);
            }
        } else if (strcmp(comando, "demolisci-stazione") == 0) {
            if (fscanf(file, "%d", &distanza) != EOF) {
                root = deleteNode(root, distanza);
            }
        } else if (strcmp(comando, "aggiungi-auto") == 0) {
            if (fscanf(file, "%d %d", &distanza, &autonomia) != EOF) {
                RBNode* stazione = searchNode(root, distanza);
                if (stazione != NULL) {
                    aggiungiAuto(stazione, autonomia);
                } else {
                    printf("non aggiunta\n");
                }
            }
        } else if (strcmp(comando, "rottama-auto") == 0) {
            if (fscanf(file, "%d %d", &distanza, &autonomia) != EOF) {
                RBNode* stazione = searchNode(root, distanza);
                if (stazione != NULL) {
                    rottamaAuto(stazione, autonomia);
                } else {
                    printf("non rottamata\n");
                }
            }
        } else if (strcmp(comando, "pianifica-percorso") == 0) {
            if (fscanf(file, "%d %d", &partenza, &arrivo) != EOF) {
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