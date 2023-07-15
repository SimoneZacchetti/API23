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

// nodo di una lista per trovare il percorso minimo, con riferimento al padre
typedef struct ListNode {
    Stazione* stazione;
    struct ListNode* padre;
    struct ListNode* next;
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
        // TODO verificare se è corretto il fatto che stampi se la stazione è già presente
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



//*** FUNZIONI PER GESTIONE LISTA CON PADRE ***//

// crea nodo della lista con padre
ListNode* creaListNode(Stazione* stazione, ListNode* padre) {
    ListNode* newNode = (ListNode*)malloc(sizeof(ListNode));
    newNode->stazione = stazione;
    newNode->padre = padre;
    newNode->next = NULL;
    return newNode;
}

// inserisce un nodo nella lista con padre in ordine crescente
ListNode* inserisciInOrdine(ListNode* head, Stazione* stazione, ListNode* padre) {
    ListNode* newNode = creaListNode(stazione, padre);

    if (head == NULL) {
        // Inserimento in testa alla lista
        newNode->next = head;
        return newNode;
    }

    if (stazione->distanza < head->stazione->distanza) {
        // Inserimento in testa alla lista
        newNode->next = head;
        return newNode;
    }

    ListNode* current = head;
    ListNode* prev = NULL;

    while (current != NULL && current->stazione->distanza <= stazione->distanza) {
        if (current->stazione == stazione) {
            // La stazione è già stata inserita, quindi liberiamo il nodo appena creato
            free(newNode);
            return head;
        }
        prev = current;
        current = current->next;
    }

    // Inserimento in posizione corretta
    prev->next = newNode;
    newNode->next = current;

    return head;
}

// Function to display a list
void stampaNodeList(ListNode* head) {
    ListNode* current = head;
    while (current != NULL) {
        printf("%d ", current->stazione->distanza);
        if (current->padre != NULL) {
            printf("padre: %d ", current->padre->stazione->distanza);
        } else {
            printf("padre: NULL ");
        }
        printf("\n");
        current = current->next;
    }
    printf("\n");
}

// cancella la lista
void freeList(ListNode* head) {
    ListNode* current = head;
    while (current != NULL) {
        ListNode* temp = current;
        current = current->next;
        free(temp);
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
        freeList(temp->list); // Libera la memoria occupata dalla lista ListNode
        free(temp);
    }
}

// funzione per stampare la lista di liste
void stampaLista(ListList* head) {
    ListList* current = head;
    while (current != NULL) {
        stampaNodeList(current->list);
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


// funzione per la ricerca di tutti i nodi ragguibili da una stazione in avanti
void cercaRaggiungibili(TreeNode* node, ListNode** resultList, ListNode* partenza, int autonomia, int* trovato, int arrivo, int* massimo, ListNode** nodoArrivo) {
    if (node == NULL || *trovato) {
        return;
    }

    if (node->stazione.distanza > partenza->stazione->distanza + autonomia) {
        cercaRaggiungibili(node->left, resultList, partenza, autonomia, trovato, arrivo, massimo, nodoArrivo);
    } else if (node->stazione.distanza < partenza->stazione->distanza) {
        cercaRaggiungibili(node->right, resultList, partenza, autonomia, trovato, arrivo, massimo, nodoArrivo);
    } else {
        cercaRaggiungibili(node->left, resultList, partenza, autonomia, trovato, arrivo, massimo, nodoArrivo);

        if (node->stazione.distanza == arrivo){
            *trovato = 1;
            *nodoArrivo = creaListNode(&(node->stazione), partenza);
            return;
        }

        if (node->stazione.distanza <= partenza->stazione->distanza + autonomia && node->stazione.distanza > partenza->stazione->distanza) {
            if (node->stazione.distanza > *massimo){
                *resultList = inserisciInOrdine(*resultList, &(node->stazione), partenza);
            }
        }

        cercaRaggiungibili(node->right, resultList, partenza, autonomia, trovato, arrivo, massimo, nodoArrivo);
    }
}


// funzione per la ricerca di tutti i nodi ragguibili da una stazione all'indietro
void cercaRaggiungibiliIndietro(TreeNode* node, ListNode** resultList, ListNode* partenza, int autonomia, int* trovato, int arrivo, int* minimo, ListNode** nodoArrivo) {
    if (node == NULL || *trovato) {
        return;
    }

    if (node->stazione.distanza < partenza->stazione->distanza - autonomia) {
        cercaRaggiungibiliIndietro(node->right, resultList, partenza, autonomia, trovato, arrivo, minimo, nodoArrivo);
    } else if (node->stazione.distanza >= partenza->stazione->distanza) {
        cercaRaggiungibiliIndietro(node->left, resultList, partenza, autonomia, trovato, arrivo, minimo, nodoArrivo);
    } else {
        cercaRaggiungibiliIndietro(node->left, resultList, partenza, autonomia, trovato, arrivo, minimo, nodoArrivo);

        if (node->stazione.distanza == arrivo){
            *trovato = 1;
            *nodoArrivo = creaListNode(&(node->stazione), partenza);
            return;
        }

        if (node->stazione.distanza >= partenza->stazione->distanza - autonomia && node->stazione.distanza < partenza->stazione->distanza) {
            // inserisco solo i nodi che sono piu piccoli del primo elemento della lista, gli altri saranno già presenti
            if (node->stazione.distanza < *minimo){
                *resultList = inserisciInOrdine(*resultList, &(node->stazione), partenza);
            }
        }

        cercaRaggiungibiliIndietro(node->right, resultList, partenza, autonomia, trovato, arrivo, minimo, nodoArrivo);
    }
}


// funzione per trovare il nodo con distanza piu alta nella lista, usato nel percorso in avanti
// migliorabile
int trovaMassimo(ListNode* list){
    int massimo = 0;
    ListNode* curr = list;
    while(curr != NULL){
        if (curr->stazione->distanza > massimo){
            massimo = curr->stazione->distanza;
        }
        curr = curr->next;
    }
    return massimo;
}

// funzione che cerca il percorso minimo tra due stazioni in ordine crescente
void cercaPercorso(TreeNode* root, int partenza, int arrivo){
    int massimo = 0, trovato = 0;

    TreeNode * partenzaTreeNode = searchNode(root, partenza);
    ListNode * partenzaNode = creaListNode(&partenzaTreeNode->stazione, NULL);
    ListNode* raggiungibili = NULL;
    ListNode* nodoArrivo = NULL;

    cercaRaggiungibili(root, &raggiungibili, partenzaNode, partenzaTreeNode->stazione.auto_max, &trovato, arrivo, &massimo, &nodoArrivo);

    // se non trovo nodi raggiungibili dalla partenza stampo nessun percorso
    if (raggiungibili == NULL){
        printf("nessun percorso\n");
        return;
    }

    // se trovo l'arrivo stampo il percorso e libero la memoria
    if (trovato){
        printf("%d %d\n", partenza, arrivo);
        freeList(partenzaNode);
        freeList(raggiungibili);
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
        ListNode * currNode = curr->list;
        ListNode * nuoviRaggiungibili = NULL;
        while(currNode != NULL && !trovato){
            cercaRaggiungibili(root, &nuoviRaggiungibili, currNode, currNode->stazione->auto_max, &trovato, arrivo, &massimo, &nodoArrivo);
            currNode = currNode->next;
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
void cercaPercorsoIndietro(TreeNode* root, int partenza, int arrivo){
    int minimo = INT_MAX, trovato = 0;

    TreeNode * partenzaTreeNode = searchNode(root, partenza);
    ListNode * partenzaNode = creaListNode(&partenzaTreeNode->stazione, NULL);
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
        freeList(partenzaNode);
        freeList(raggiungibili);
        return;
    }

    // il minimo diventa il nodo con distanza minore della lista di raggiungibili
    minimo = raggiungibili->stazione->distanza;

    // creo la lista di liste
    ListList* head = NULL;
    // inserisco la prima lista dei raggiungibili
    head = inserisciLista(head, raggiungibili);

    // scorro la lista di liste
    ListList * curr = head;
    while(curr != NULL && !trovato){
        ListNode * currNode = curr->list;
        ListNode * nuoviRaggiungibili = NULL;
        // scorro la lista dei nodi raggiungibili
        while(currNode != NULL && !trovato){
            cercaRaggiungibiliIndietro(root, &nuoviRaggiungibili, currNode, currNode->stazione->auto_max, &trovato, arrivo, &minimo, &nodoArrivo);
            currNode = currNode->next;
        }
        if (nuoviRaggiungibili != NULL){
            minimo = nuoviRaggiungibili->stazione->distanza;
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


int main() {
    TreeNode* root = NULL;
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
                TreeNode* stazione = searchNode(root, distanza);
                if (stazione != NULL) {
                    aggiungiAuto(stazione, autonomia);
                } else {
                    printf("non aggiunta\n");
                }
            }
        } else if (strcmp(comando, "rottama-auto") == 0) {
            if (fscanf(file, "%d %d", &distanza, &autonomia) != EOF) {
                TreeNode* stazione = searchNode(root, distanza);
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