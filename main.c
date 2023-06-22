#include <stdio.h>
#include <stdlib.h>

#define MAX_AUTO 512

typedef struct {
    int numero_stazione;
    int auto_max;
    int autonomie_auto[MAX_AUTO];
} Stazione;

typedef struct TreeNode {
    Stazione stazione;
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

TreeNode* createNode(Stazione stazione) {
    TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));
    newNode->stazione = stazione;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

TreeNode* insertNode(TreeNode* root, Stazione stazione) {
    if (root == NULL) {
        return createNode(stazione);
    }

    if (stazione.numero_stazione < root->stazione.numero_stazione) {
        root->left = insertNode(root->left, stazione);
    } else if (stazione.numero_stazione > root->stazione.numero_stazione) {
        root->right = insertNode(root->right, stazione);
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

TreeNode* deleteNode(TreeNode* root, int numero_stazione) {
    if (root == NULL) {
        return root;
    }

    if (numero_stazione < root->stazione.numero_stazione) {
        root->left = deleteNode(root->left, numero_stazione);
    } else if (numero_stazione > root->stazione.numero_stazione) {
        root->right = deleteNode(root->right, numero_stazione);
    } else {
        if (root->left == NULL) {
            TreeNode* temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            TreeNode* temp = root->left;
            free(root);
            return temp;
        }

        TreeNode* temp = findMin(root->right);
        root->stazione = temp->stazione;
        root->right = deleteNode(root->right, temp->stazione.numero_stazione);
    }

    return root;
}

TreeNode* searchNode(TreeNode* root, int numero_stazione) {
    if (root == NULL || root->stazione.numero_stazione == numero_stazione) {
        return root;
    }

    if (numero_stazione < root->stazione.numero_stazione) {
        return searchNode(root->left, numero_stazione);
    } else {
        return searchNode(root->right, numero_stazione);
    }
}

void inorderTraversal(TreeNode* root) {
    if (root != NULL) {
        inorderTraversal(root->left);
        printf("Numero stazione: %d\n", root->stazione.numero_stazione);
        inorderTraversal(root->right);
    }
}

void freeTree(TreeNode* root) {
    if (root != NULL) {
        freeTree(root->left);
        freeTree(root->right);
        free(root);
    }
}

int main() {
    TreeNode* root = NULL;

    // Inserimento di alcune stazioni
    Stazione stazione1 = {1, 10, {100, 200, 300}};
    Stazione stazione3 = {3, 8, {120, 220, 320}};
    Stazione stazione4 = {4, 12, {180, 280, 380}};
    Stazione stazione2 = {2, 5, {150, 250, 350}};

    root = insertNode(root, stazione1);
    root = insertNode(root, stazione2);
    root = insertNode(root, stazione3);
    root = insertNode(root, stazione4);

    // Stampa dell'albero in ordine
    printf("Albero in ordine:\n");
    inorderTraversal(root);

    // Esempio di ricerca di una stazione
    int numero_stazione_da_cercare = 2;
    TreeNode* result = searchNode(root, numero_stazione_da_cercare);
    if (result != NULL) {
        printf("\nStazione trovata con numero: %d\n", result->stazione.numero_stazione);
        printf("Capacità massima auto: %d\n", result->stazione.auto_max);
    } else {
        printf("\nStazione non trovata\n");
    }

    // Esempio di cancellazione di una stazione
    int numero_stazione_da_cancellare = 1;
    root = deleteNode(root, numero_stazione_da_cancellare);

    // Stampa dell'albero dopo la cancellazione
    printf("\nAlbero dopo la cancellazione:\n");
    inorderTraversal(root);

    // Liberazione della memoria occupata dall'albero
    freeTree(root);
    //ciao

    return 0;
}
