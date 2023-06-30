#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_AUTO 512

typedef struct Node {
    int valore;
    struct Node* next;
} Node;

typedef struct {
    int distanza;
    int auto_max;
    int autonomie_auto[MAX_AUTO];
    Node* stazioni_adiacenti;
} Stazione;

typedef struct TreeNode {
    Stazione stazione;
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;


//*** FUNZIONI PER GESTIONE LISTA ***//

void inserisciInOrdineDecrescente(Node** testa, int valore) {
    Node* nuovoNodo = (Node*)malloc(sizeof(Node));
    nuovoNodo->valore = valore;
    nuovoNodo->next = NULL;

    if (*testa == NULL) {
        // La lista è vuota, il nuovo Node diventa la testa
        *testa = nuovoNodo;
    } else {
        if (valore == (*testa)->valore) {
            // Il valore è già presente, non inserisco
            free(nuovoNodo);
            return;
        } else if (valore > (*testa)->valore) {
            // Inserimento in testa
            nuovoNodo->next = *testa;
            *testa = nuovoNodo;
        } else {
            Node* current = *testa;
            while (current->next != NULL && current->next->valore > valore) {
                current = current->next;
            }
            if (current->next->valore == valore) {
                // Il valore è già presente, non inserisco
                free(nuovoNodo);
                return;
            }
            // Inserimento dopo current
            nuovoNodo->next = current->next;
            current->next = nuovoNodo;
        }
    }
}

void aggiungiInCoda(Node** head, int valore) {
    Node* nuovoNodo = (Node*)malloc(sizeof(Node));
    nuovoNodo->valore = valore;
    nuovoNodo->next = NULL;

    if (*head == NULL) {
        *head = nuovoNodo;
    } else {
        Node* current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = nuovoNodo;
    }
}

// funzione per cancellare un elemento dalla lista
void cancellaElementoLista(Node** testa, int valore) {
    if (*testa == NULL) {
        // La lista è vuota
        return;
    }

    Node* daCancellare = NULL;

    if ((*testa)->valore == valore) {
        // Il Node da cancellare è la testa
        daCancellare = *testa;
        *testa = (*testa)->next;
    } else {
        Node* current = *testa;
        while (current->next != NULL && current->next->valore != valore) {
            current = current->next;
        }
        if (current->next != NULL) {
            daCancellare = current->next;
            current->next = current->next->next;
        }
    }

    if (daCancellare != NULL) {
        free(daCancellare);
    }
}

// Funzione per eliminare tutti gli elementi della lista
void eliminaLista(Node** head) {
    Node* current = *head;
    Node* next;

    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }

    *head = NULL;
}

// Funzione per stampare tutti gli elementi della lista
void stampaLista(Node* head) {
    Node* current = head;
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

TreeNode* createNode(Stazione stazione) {
    TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));
    newNode->stazione = stazione;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

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

TreeNode* findMin(TreeNode* node) {
    TreeNode* current = node;
    while (current->left != NULL) {
        current = current->left;
    }
    return current;
}

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

void inorderTraversal(TreeNode* root) {
    if (root == NULL) {
        return;
    }

    inorderTraversal(root->left);
    printf("%d ", root->stazione.distanza);
    inorderTraversal(root->right);
}



void freeTree(TreeNode* root) {
    if (root != NULL) {
        freeTree(root->left);
        freeTree(root->right);
        free(root);
    }
}

//*** FUNZIONI GESTIONE LISTE DI ADIACENZA ***//
void cercaAdiacenti(TreeNode* root) {
    if (root != NULL) {
        cercaAdiacenti(root->left);

        TreeNode* current = root->right;
        while (current != NULL) {
            if (abs(root->stazione.distanza - current->stazione.distanza) < root->stazione.auto_max) {
                inserisciInOrdineDecrescente(&(root->stazione.stazioni_adiacenti), current->stazione.distanza);
                inserisciInOrdineDecrescente(&(current->stazione.stazioni_adiacenti), root->stazione.distanza);
            }
            current = current->right;
        }

        cercaAdiacenti(root->right);
    }
}

void cercaNuoveAdiacenti(TreeNode* root, TreeNode* stazione) {
    if (root != NULL) {
        cercaNuoveAdiacenti(root->left, stazione);

        if (abs(stazione->stazione.distanza - root->stazione.distanza) <= stazione->stazione.auto_max) {
            inserisciInOrdineDecrescente(&(stazione->stazione.stazioni_adiacenti), root->stazione.distanza);
        }
        if(abs(stazione->stazione.distanza - root->stazione.distanza) <= root->stazione.auto_max) {
            inserisciInOrdineDecrescente(&(root->stazione.stazioni_adiacenti), stazione->stazione.distanza);
        }

        cercaNuoveAdiacenti(root->right, stazione);
    }
}



//*** FUNZIONI PER GESTIONE COMANDI RICEVUTI DA INPUT ***//
void aggiungiStazione(TreeNode** root, int distanza, int numero_auto, int autonomie[]) {
    Stazione stazione;
    stazione.distanza = distanza;
    stazione.auto_max = 0;

    // Inizializza la lista stazioni_adiacenti a NULL
    stazione.stazioni_adiacenti = NULL;

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

// Funzione ricorsiva per trovare il valore più distante dal nodo (in avanti)
// TODO potrei doverla modificare come con la findFurthestBackward, aggiungendo findMaxDistance
TreeNode* findFurthest(struct TreeNode* node, int x, int y) {
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
}

int findMaxDistance(struct TreeNode* node) {
    if (node == NULL)
        return INT_MIN;

    int leftMax = findMaxDistance(node->left);
    int rightMax = findMaxDistance(node->right);

    int maxDistance = (leftMax > rightMax) ? leftMax : rightMax;
    if (node->stazione.distanza > maxDistance)
        maxDistance = node->stazione.distanza;

    return maxDistance;
}


// Funzione ricorsiva per trovare il valore più distante dal nodo (all'indietro)
struct TreeNode* findFurthestBackward(struct TreeNode* node, int distanza, int autonomia) {
    if (node == NULL)
        return NULL;

    int diff = distanza - autonomia;

    if (node->stazione.distanza >= diff) {
        if (node->left != NULL) {
            int maxDistance = findMaxDistance(node->left); // Funzione ausiliaria per trovare la distanza massima nel sottoalbero sinistro
            if (maxDistance >= diff)
                return findFurthestBackward(node->left, distanza, autonomia);
        }
        return node;
    } else {
        return findFurthestBackward(node->right, distanza, autonomia);
    }
}

void cercaPercorsoMinimo(TreeNode* root, TreeNode* partenza, TreeNode* arrivo){
    if (partenza->stazione.distanza == arrivo->stazione.distanza) {
        printf("%d\n", partenza->stazione.distanza);
        return;
    }

    TreeNode *curr = partenza;
    Node *tappe = NULL;
    aggiungiInCoda(&tappe, partenza->stazione.distanza);
    if (partenza->stazione.distanza < arrivo->stazione.distanza) {
        while (curr->stazione.distanza < arrivo->stazione.distanza){
            TreeNode *max = findFurthest(root, curr->stazione.distanza, curr->stazione.auto_max);
            if (max->stazione.distanza == curr->stazione.distanza) {
                printf("nessun percorso\n");
                eliminaLista(&tappe);
                return;
            }
            curr = max;
            if (curr->stazione.distanza > arrivo->stazione.distanza) {
                printf("metto arrivo invece del prossimo\n");
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

int main() {
    TreeNode* root = NULL;

    char* comando = NULL;
    size_t buffer_size = 0;
    ssize_t input_length;

    while ((input_length = getline(&comando, &buffer_size, stdin)) != -1) {

        if (input_length == -1) {
            // Errore di input
            printf("Errore durante la lettura dell'input.\n");
            break;
        }

        // Rimuovo il carattere di newline alla fine del comando
        comando[strcspn(comando, "\n")] = '\0';

        char* token = strtok(comando, " ");
        if (token != NULL) {
            if (strcmp(token, "aggiungi-stazione") == 0) {
                // salvo il resto della stringa in token, NULL: leggo dalla stessa stringa di prima, "": non ci sono altri separatori
                token = strtok(NULL, "");
                if (token != NULL) {
                    comandoAggiungiStazione(&root, token);
                }
            } else if (strcmp(token, "demolisci-stazione") == 0) {
                token = strtok(NULL, "");
                if (token != NULL) {
                    int distanza = atoi(token);
                    root = deleteNode(root, distanza);
                }
            } else if (strcmp(token, "aggiungi-auto") == 0){
                token = strtok(NULL, " ");
                if (token != NULL) {
                    int distanza = atoi(token);
                    TreeNode* stazione = searchNode(root, distanza);
                    if (stazione != NULL) {
                        token = strtok(NULL, "");
                        if (token != NULL) {
                            int autonomia = atoi(token);
                            aggiungiAuto(stazione, autonomia);
//                            if (cambiata_auto_max){
//                                cercaNuoveAdiacenti(root, stazione);   // ricerca le stazioni adiacenti anche dopo l'aggiunta di un'auto
//                            }
                        }
                    } else {
                        printf("non aggiunta\n");
                    }
                }
            } else if (strcmp(token, "rottama-auto") == 0){
                token = strtok(NULL, " ");
                if (token != NULL) {
                    int distanza = atoi(token);
                    TreeNode* stazione = searchNode(root, distanza);
                    if (stazione != NULL) {
                        token = strtok(NULL, "");
                        if (token != NULL) {
                            int autonomia = atoi(token);

                            rottamaAuto(stazione, autonomia);
//                            if (cambiata_auto_max){
//                                cercaNuoveAdiacenti(root, stazione);   // ricerca le stazioni adiacenti anche dopo l'aggiunta di un'auto
//                            }
                        }
                    } else {
                        printf("non rottamata\n");
                    }
                }
            } else if (strcmp(token, "pianifica-percorso") == 0){
                token = strtok(NULL, " ");
                if (token != NULL) {
                    int partenza = atoi(token);
                    token = strtok(NULL, "");
                    if (token != NULL) {
                        int arrivo = atoi(token);
                        //findMinPath(root, partenza, arrivo);
                        if (partenza == arrivo) {
                            printf("%d\n", partenza);
                            continue;
                        }
                        TreeNode* nodoPartenza = searchNode(root, partenza);
                        TreeNode* nodoArrivo = searchNode(root, arrivo);
                        cercaPercorsoMinimo(root, nodoPartenza, nodoArrivo);
                    }
                }
            }
            else {
                printf("Comando non valido.\n");
            }
        }
//        inorderTraversal(root);
    }

    // Liberazione della memoria occupata dall'albero
    freeTree(root);

    // Liberazione della memoria occupata dal buffer dell'input
    free(comando);

    return 0;
}

// NOTE
// quando demolisco una stazione per ora non viene tolta dalla lista di adiacenza delle altre stazioni
// TODO togliere lista di adiacenza
// TODO controllare il caso di percorso al contrario (test3)
// TODO fare disambiguazione tra percorsi con uguale numero di stazioni