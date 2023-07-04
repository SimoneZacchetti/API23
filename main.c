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

// struttura per rappresentare un nodo dell'albero
typedef struct TreeNode {
    Stazione stazione;
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

// struttura per trovare il percorso minimo, con riferimento al padre
typedef struct Node {
    int valore;
    struct Node* padre;
    struct Node* next;
} Node;

// lista per trovare i nodi raggiungibili da un certo nodo, non tengo traccia del padre
typedef struct NodePercorso {
    int valore;
    struct NodePercorso* next;
} NodePercorso;


//*** FUNZIONI PER GESTIONE LISTA CON PADRE ***//

// crea nodo della lista con padre
Node* creaNodo(int valore, Node* padre, Node* next){
    Node* nuovoNodo = (Node*)malloc(sizeof(Node));
    nuovoNodo->valore = valore;
    nuovoNodo->padre = padre;
    nuovoNodo->next = next;
    return nuovoNodo;
}

// inserisce un nodo nella lista con padre
void inserisciNodo(Node** head, int valore, Node* padre) {
    Node* newNode = creaNodo(valore, padre, NULL);
    if (*head == NULL) {
        *head = newNode;
    } else {
        Node* current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }
}

// Funzione per liberare la memoria occupata dalla lista con padre
void freeList(Node* head) {
    Node* current = head;
    while (current != NULL) {
        Node* temp = current;
        current = current->next;
        free(temp);
    }
}

// funzione che mette nella prima lista i nodi della seconda lista che non sono già presenti nella prima
void appendUniqueNodes(Node** firstList, Node* secondList) {
    if (*firstList == NULL) {
        *firstList = secondList;
        return;
    }

    Node* current = *firstList;

    while (current->next != NULL) {
        current = current->next;
    }

    Node* secondCurrent = secondList;

    while (secondCurrent != NULL) {
        int alreadyExists = 0;

        Node* temp = *firstList;

        while (temp != NULL) {
            if (temp->valore == secondCurrent->valore) {
                alreadyExists = 1;
                break;
            }
            temp = temp->next;
        }

        if (!alreadyExists) {
            inserisciNodo(&current->next, secondCurrent->valore, secondCurrent->padre);
            current = current->next;
        }

        secondCurrent = secondCurrent->next;
    }
}

void stampaLista(Node* head){
    Node* current = head;
    while (current != NULL) {
        printf("distanza %d, padre %d\n", current->valore, current->padre->valore ? current->padre->valore : -1);
        current = current->next;
        if (current != NULL) {
            printf(" ");
        }
    }
    printf("\n");
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



//*** FUNZIONI PER GESTIONE ALBERO ***//

// crea nodo dell'albero
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
//    cercaAdiacenti(*root);
}


void comandoAggiungiStazione(TreeNode** root, char* restoStringa) {
    // Implementazione per l'aggiunta della stazione
    // Utilizza il parametro restoStringa per ottenere gli argomenti necessari

    // Esempio di implementazione:
    int distanza;
    int numero_auto;
    int autonomie[MAX_AUTO];

    char* token = strtok(restoStringa, " ");
    if (token == NULL) {
        printf("Errore: specificare la distanza della stazione.\n");
        return;
    }
    distanza = atoi(token);

    token = strtok(NULL, " ");
    if (token == NULL) {
        printf("Errore: specificare il numero di auto.\n");
        return;
    }
    numero_auto = atoi(token);

    int i = 0;
    while (i < numero_auto) {
        token = strtok(NULL, " ");
        if (token == NULL) {
            printf("Errore: specificare le autonomie delle auto.\n");
            break;
        }
        autonomie[i] = atoi(token);
        i++;
    }

    if (i == numero_auto) {
        aggiungiStazione(root, distanza, numero_auto, autonomie);
    }
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
//    int cambiata_auto_max = 0;

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
//                cambiata_auto_max = 1;
            }
            printf("rottamata\n");
            break;
        }
    }

    if (i == MAX_AUTO) {
        printf("non rottamata\n");
    }
}

/*
// Funzione ricorsiva per trovare il valore più distante dal nodo (in avanti)
TreeNode* findFurthest(struct TreeNode* root, int distanza, int autonomia) {

    if (node == NULL)
        return NULL; // Ritorna NULL se l'intero k non viene trovato
    int sum = x + y;
    if (node->stazione.distanza <= sum) {
        if (node->right != NULL && node->right->stazione.distanza <= sum)
            return findFurthest(node->right, x, y);
        else
            return node; // Ritorna il puntatore all'intero k
    } else {
        return findFurthest(node->left, x, y);
    }

    TreeNode *curr = root;
    TreeNode *result = NULL;
    while (curr != NULL) {
        if (curr->stazione.distanza <= distanza + autonomia) {
            result = curr;
            curr = curr->right;
        } else {
            curr = curr->left;
        }
    }
    return result;
}


// Funzione ricorsiva per trovare il valore più distante dal nodo (all'indietro)
struct TreeNode* findFurthestBackward(struct TreeNode* node, int distanza, int autonomia) {
    if (node == NULL) return NULL;

    int diff = distanza - autonomia;

    struct TreeNode* curr = node;
    struct TreeNode* result = NULL;

    while (curr != NULL) {
        if (curr->stazione.distanza >= diff) {
            result = curr;
            curr = curr->left;
        } else {
            if (curr->right != NULL && curr->right->stazione.distanza >= diff) {
                return findFurthestBackward(curr->right, distanza, autonomia);
            }
            curr = curr->right;
        }
    }
    return result;
}


void cercaPercorsoMinimo(TreeNode* root, TreeNode* partenza, TreeNode* arrivo){
    if (partenza->stazione.distanza == arrivo->stazione.distanza) {
        printf("%d\n", partenza->stazione.distanza);
        return;
    }

    TreeNode *curr = partenza;
    Node *tappe = NULL;
    aggiungiInCoda(&tappe, partenza->stazione.distanza);
    printf("curr: %d, auto_max: %d\n", curr->stazione.distanza, curr->stazione.auto_max);
    if (partenza->stazione.distanza < arrivo->stazione.distanza) {
        while (curr->stazione.distanza < arrivo->stazione.distanza){
            TreeNode *max = findFurthest(root, curr->stazione.distanza, curr->stazione.auto_max);
            if (max->stazione.distanza == curr->stazione.distanza) {
//                printf("curr: %d, auto_max: %d\n", curr->stazione.distanza, curr->stazione.auto_max);
                printf("nessun percorso\n");
                eliminaLista(&tappe);
                return;
            }
            curr = max;
            if (curr->stazione.distanza > arrivo->stazione.distanza) {
                aggiungiInCoda(&tappe, arrivo->stazione.distanza);
            } else {
                aggiungiInCoda(&tappe, max->stazione.distanza);
            }
        }
    } else {
        // caso in cui la partenza è più avanti dell'arrivo
        while (curr->stazione.distanza > arrivo->stazione.distanza){
            TreeNode *max = findFurthestBackward(root, curr->stazione.distanza, curr->stazione.auto_max);
            if (max->stazione.distanza == curr->stazione.distanza) {
                printf("nessun percorso\n");
                eliminaLista(&tappe);
                return;
            }
            printf("curr: %d, auto_max: %d\n", curr->stazione.distanza, curr->stazione.auto_max);
            curr = max;
            if (curr->stazione.distanza < arrivo->stazione.distanza) {
                aggiungiInCoda(&tappe, arrivo->stazione.distanza);
            } else {
                aggiungiInCoda(&tappe, max->stazione.distanza);
            }
        }
    }

    stampaLista(tappe);
    eliminaLista(&tappe);
}
*/

/**
 * Trova i valori compresi tra partenza e partenza + autonomia
 * @param node nodo di partenza dell'albero
 * @param partenza partenza
 * @param autonomia autonomia
 * @param resultList lista risultante
 */
void trovaValoriRange(TreeNode* node, Node* partenza, int autonomia, Node** resultList) {
    // TODO verificare perche da problemi node == NULL || node->stazione.distanza > partenza->valore + autonomia || node->stazione.distanza < partenza->valore
    if (node == NULL) {
        return;
    }

    trovaValoriRange(node->left, partenza, autonomia, resultList);

    if (node->stazione.distanza > partenza->valore && node->stazione.distanza <= partenza->valore + autonomia){
        inserisciNodo(resultList, node->stazione.distanza, partenza);
    }

    trovaValoriRange(node->right, partenza, autonomia, resultList);
}

/**
 * Trova i valori compresi tra partenza e partenza - autonomia
 * @param node nodo di partenza dell'albero
 * @param partenza partenza
 * @param autonomia autonomia
 * @param resultList lista risultante
 */
void trovaValoriRangeIndietro(TreeNode* node, Node* partenza, int autonomia, Node** resultList) {
    if (node == NULL) {
        return;
    }

    trovaValoriRangeIndietro(node->left, partenza, autonomia, resultList);

    if (node->stazione.distanza >= partenza->valore - autonomia && node->stazione.distanza < partenza->valore) {
        inserisciNodo(resultList, node->stazione.distanza, partenza);
    }

    trovaValoriRangeIndietro(node->right, partenza, autonomia, resultList);
}


/**
 * Cerca i nodi raggiungibili da una qualunque stazione di partenza in avanti
 * @param root nodo di partenza
 * @param partenza partenza
 * @param autonomia autonomia
 * @return lista di nodi raggiungibili
 */
Node* cercaRaggiungibili(TreeNode* root, Node* partenza, int autonomia) {
    Node* resultList = NULL;
    trovaValoriRange(root, partenza, autonomia, &resultList);
    return resultList;
}

/**
 * Cerca i nodi raggiungibili da una qualunque stazione di partenza all'indietro
 * @param root nodo di partenza
 * @param partenza partenza
 * @param autonomia autonomia
 * @return lista di nodi raggiungibili
 */
Node* cercaRaggiungibiliIndietro(TreeNode* root, Node* partenza, int autonomia) {
    Node* resultList = NULL;
    trovaValoriRangeIndietro(root, partenza, autonomia, &resultList);
    return resultList;
}

// funzione che cerca il percorso minimo tra due stazioni in ordine crescente
void cercaPercorso(TreeNode* root, int partenza, int arrivo){
    TreeNode * partenzaTreeNode = searchNode(root, partenza);
    Node * partenzaNode = creaNodo(partenza, NULL, NULL);
    Node* raggiungibili = cercaRaggiungibili(root, partenzaNode, partenzaTreeNode->stazione.auto_max);
    //stampaLista(raggiungibili);
    Node* curr = raggiungibili;
    Node* nuoviRaggiungibili = NULL;
    NodePercorso* percorso = NULL;

    while(curr != NULL){
        // se il nodo corrente è il nodo di arrivo interrompi il ciclo
        if (curr->valore == arrivo){
            break;
        }
        // aggiungo a raggiungibili i nodi raggiungibili dal nodo corrente che non sono già presenti in raggiungibili
        TreeNode* current = searchNode(root, curr->valore);
        nuoviRaggiungibili = cercaRaggiungibili(root, curr, current->stazione.auto_max);
        appendUniqueNodes(&raggiungibili, nuoviRaggiungibili);
        // stampaLista(raggiungibili);

        freeList(nuoviRaggiungibili);
        curr = curr->next;
    }

    if (curr == NULL){
        // nessun percorso trovato
        printf("nessun percorso\n");
        freeList(raggiungibili);
        freeList(nuoviRaggiungibili);
        return;
    }

    while (curr != NULL){
        inserisciInTesta(&percorso, curr->valore);
        curr = curr->padre;
    }
//    printf("percorso trovato: ");
    stampaListaPercorso(percorso);
    freeList(raggiungibili);
    freeListPercorso(percorso);
}

// funzione che cerca il percorso minimo tra due stazioni in ordine decrescente
void cercaPercorsoIndietro(TreeNode* root, int partenza, int arrivo){
    TreeNode * partenzaTreeNode = searchNode(root, partenza);
    Node * partenzaNode = creaNodo(partenza, NULL, NULL);
    Node* raggiungibili = cercaRaggiungibiliIndietro(root, partenzaNode, partenzaTreeNode->stazione.auto_max);
//    printf("raggiungibili: ");
//    stampaLista(raggiungibili);

    Node* curr = raggiungibili;
    Node* nuoviRaggiungibili = NULL;
    NodePercorso* percorso = NULL;

    while(curr != NULL){
        // se il nodo corrente è il nodo di arrivo interrompi il ciclo
        if (curr->valore == arrivo){
            break;
        }
        // aggiungo a raggiungibili i nodi raggiungibili dal nodo corrente che non sono già presenti in raggiungibili
        TreeNode* current = searchNode(root, curr->valore);
        nuoviRaggiungibili = cercaRaggiungibiliIndietro(root, curr, current->stazione.auto_max);
        appendUniqueNodes(&raggiungibili, nuoviRaggiungibili);
        // stampaLista(raggiungibili);

        freeList(nuoviRaggiungibili);
        curr = curr->next;
    }

    if (curr == NULL){
        // nessun percorso trovato
        printf("nessun percorso\n");
        freeList(raggiungibili);
        freeList(nuoviRaggiungibili);
        return;
    }

    while (curr != NULL){
        inserisciInTesta(&percorso, curr->valore);
        curr = curr->padre;
    }
    // printf("percorso trovato: ");
    stampaListaPercorso(percorso);
    freeList(raggiungibili);
    freeListPercorso(percorso);
}

int main() {
    TreeNode* root = NULL;
    FILE* file = stdin;

    char comando[20];
    int distanza, numero_auto, autonomia;
    int partenza, arrivo;
    int autonomie[MAX_AUTO];
//    size_t buffer_size = 0;
//    ssize_t input_length;

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
//        inorderTraversal(root);


    // Liberazione della memoria occupata dall'albero
    freeTree(root);

    return 0;
}


// NOTE
// quando demolisco una stazione per ora non viene tolta dalla lista di adiacenza delle altre stazioni
// TODO controllare il caso di percorso al contrario (test3)
// TODO fare disambiguazione tra percorsi con uguale numero di stazioni