
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#define INF INT_MAX
#define MAX_VERTICES 20
typedef struct noeudhash{
    char * nom;
    int indiceNoeud;
    struct noeudhash* suivant;
}noeudhash;

typedef struct {
    noeudhash* tableHash[MAX_VERTICES];
    int *indice;
}TableHach;
int hash(char* nom) {
    int sum = 0;
    for (int i = 0; nom[i] != '\0'; i++) {
        sum += nom[i];
    }
    return sum % MAX_VERTICES;
}
TableHach* init_hash_table(int num_ver){
    TableHach* hash_table = (TableHach*)malloc(sizeof(TableHach));
    if (hash_table == NULL) {
        printf("Memory allocation failed for the hash table.\n");
        exit(1);
    }
    for (int i = 0; i < MAX_VERTICES; i++) {
        hash_table->tableHash[i] = NULL;
    }
    hash_table->indice=(int*)malloc(num_ver*sizeof(int));
    return hash_table;
}
void insert_vertex(TableHach* hash_table, char* nom, int index) {
    int hash_value = hash(nom);
    noeudhash* new_vertex = (noeudhash*)malloc(sizeof(noeudhash));
    if (new_vertex == NULL) {
        printf("Memory allocation failed for the new vertex.\n");
        exit(1);
    }
    new_vertex->nom = strdup(nom);
    new_vertex->indiceNoeud = index;
    hash_table->indice[index]=hash_value;

    if (hash_table->tableHash[hash_value] == NULL) {
        hash_table->tableHash[hash_value] = new_vertex;

    } else {
        noeudhash* current = hash_table->tableHash[hash_value];
        while (current->suivant != NULL) {
            current = current->suivant;
        }
        current->suivant = new_vertex;
    }
    new_vertex->suivant=NULL;
}

int trouver_indice(TableHach* hash_table, char* nom) {
    int hash_value = hash(nom);
    noeudhash* noeud = hash_table->tableHash[hash_value];
    while (noeud != NULL) {
        if (strcmp(noeud->nom, nom) == 0) {
            return noeud->indiceNoeud;
        }
        noeud = noeud->suivant;
    }
    return -1;
}



//structure pour noeud dans listchaine
struct AdjListNode {
    int dest;
    int weight;
    struct AdjListNode* next;
};

//list adjacence
struct AdjList {
    struct AdjListNode* head;
};

// Structure graph
struct Graph {
    int V;
    struct AdjList* array;
};

// noeud du heap
struct MinHeapNode {
    int v;
    int dist;
};


struct MinHeap {
    int size;
    int capacity;
    int* pos;
    struct MinHeapNode** array;
};

struct AdjListNode* newAdjListNode(int dest, int weight) {
    struct AdjListNode* newNode = (struct AdjListNode*)malloc(sizeof(struct AdjListNode));
    newNode->dest = dest;
    newNode->weight = weight;
    newNode->next = NULL;
    return newNode;
}


struct Graph* createGraph(int V) {
    struct Graph* graph = (struct Graph*)malloc(sizeof(struct Graph));
    graph->V = V;
    graph->array = (struct AdjList*)malloc(V * sizeof(struct AdjList));
    for (int i = 0; i < V; ++i)
        graph->array[i].head = NULL;
    return graph;
}
///
// Function to add an edge to a directed graph
void addEdge(struct Graph* graph, int src, int dest, int weight) {
    struct AdjListNode* newNode = newAdjListNode(dest, weight);
    newNode->next = graph->array[src].head;
    graph->array[src].head = newNode;
}


struct MinHeapNode* newMinHeapNode(int v, int dist) {
    struct MinHeapNode* minHeapNode = (struct MinHeapNode*)malloc(sizeof(struct MinHeapNode));
    minHeapNode->v = v;
    minHeapNode->dist = dist;
    return minHeapNode;
}


struct MinHeap* createMinHeap(int capacity) {
    struct MinHeap* minHeap = (struct MinHeap*)malloc(sizeof(struct MinHeap));
    minHeap->pos = (int*)malloc(capacity * sizeof(int));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array = (struct MinHeapNode**)malloc(capacity * sizeof(struct MinHeapNode*));
    return minHeap;
}


void swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b) {
    struct MinHeapNode* t = *a;
    *a = *b;
    *b = t;
}


void minHeapify(struct MinHeap* minHeap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < minHeap->size && minHeap->array[left]->dist < minHeap->array[smallest]->dist)
        smallest = left;

    if (right < minHeap->size && minHeap->array[right]->dist < minHeap->array[smallest]->dist)
        smallest = right;

    if (smallest != idx) {
        struct MinHeapNode* smallestNode = minHeap->array[smallest];
        struct MinHeapNode* idxNode = minHeap->array[idx];

        minHeap->pos[smallestNode->v] = idx;
        minHeap->pos[idxNode->v] = smallest;

        swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);

        minHeapify(minHeap, smallest);
    }
}


int isEmpty(struct MinHeap* minHeap) {
    return minHeap->size == 0;
}


struct MinHeapNode* extractMin(struct MinHeap* minHeap) {
    if (isEmpty(minHeap))
        return NULL;

    struct MinHeapNode* root = minHeap->array[0];
    struct MinHeapNode* lastNode = minHeap->array[minHeap->size - 1];
    minHeap->array[0] = lastNode;

    minHeap->pos[root->v] = minHeap->size - 1;
    minHeap->pos[lastNode->v] = 0;

    --minHeap->size;
    minHeapify(minHeap, 0);

    return root;
}


void decreaseKey(struct MinHeap* minHeap, int v, int dist) {
    int i = minHeap->pos[v];
    minHeap->array[i]->dist = dist;

    while (i && minHeap->array[i]->dist < minHeap->array[(i - 1) / 2]->dist) {
        minHeap->pos[minHeap->array[i]->v] = (i - 1) / 2;
        minHeap->pos[minHeap->array[(i - 1) / 2]->v] = i;
        swapMinHeapNode(&minHeap->array[i], &minHeap->array[(i - 1) / 2]);

        i = (i - 1) / 2;
    }
}


int isInMinHeap(struct MinHeap *minHeap, int v) {
    if (minHeap->pos[v] < minHeap->size)
        return 1;
    return 0;
}


void printArr(int dist[], int n) {
    printf("Vertex Distance from Source\n");
    for (int i = 0; i < n; ++i)
        printf("%d \t\t %d\n", i, dist[i]);
}

void printPath(int parent[],int dest,TableHach* table){
    if(parent[dest]==-1)return;
    printPath(parent,parent[dest],table);
    noeudhash* noeud=table->tableHash[table->indice[dest]];
    while (noeud!=NULL)
    {
        if(noeud->indiceNoeud==dest){
            printf(" -> %s",noeud->nom);
            break;
        }

        noeud=noeud->suivant;
    }
}
void printSolution(int dist[],int parent[],int source,int num_vertices,TableHach* table){
    noeudhash* noeud,*suiv;
    for(int i=0;i<num_vertices;++i){
            suiv=table->tableHash[table->indice[i]];
            noeud=table->tableHash[table->indice[source]];
        printf("distance from %s to %s is %d ,path is %s",noeud->nom,suiv->nom,dist[i],noeud->nom);
        printPath(parent,i,table);
        printf("\n");
    }

}


void dijkstra(struct Graph* graph,TableHach* table, int src) {
    int V = graph->V;
    int dist[V];
    int parent[V];
    struct MinHeap* minHeap = createMinHeap(V);

    for (int v = 0; v < V; ++v) {
        dist[v] = INF;
        minHeap->array[v] = newMinHeapNode(v, dist[v]);
        minHeap->pos[v] = v;
        parent[v]=-1;
    }

    minHeap->array[src] = newMinHeapNode(src, dist[src]);
    minHeap->pos[src] = src;
    dist[src] = 0;
    decreaseKey(minHeap, src, dist[src]);

    minHeap->size = V;

    while (!isEmpty(minHeap)) {
        struct MinHeapNode* minHeapNode = extractMin(minHeap);
        int u = minHeapNode->v;

        struct AdjListNode* pCrawl = graph->array[u].head;
        while (pCrawl != NULL) {
            int v = pCrawl->dest;

            if (isInMinHeap(minHeap, v) && dist[u] != INF && pCrawl->weight + dist[u] < dist[v]) {
                dist[v] = dist[u] + pCrawl->weight;
                parent[v]=u;
                decreaseKey(minHeap, v, dist[v]);
            }
            pCrawl = pCrawl->next;
        }
    }

   printSolution(dist,parent,src,V,table);
}

int main(){
    int v,poids,arret,ind1,ind2;
    char sommet1[10],sommet2[10];
    TableHach* tablehachage;
   struct Graph* graph;
        printf("donner le nombre des noeud : ");
        scanf("%d",&v);
        tablehachage=init_hash_table(v);
        graph=createGraph(v);
        printf("entrer les noeud :\n");
        for(int i=0;i<v;i++){
            printf("noeud num %d :",i+1);
            scanf("%s",sommet1);
            insert_vertex(tablehachage,sommet1,i);
        }

   printf("entrer le nombre d'arrete :");
   scanf("%d",&arret);
   printf("entrer les arrete sous form sommet1 sommet2 poids\n");
   for(int i=0;i<arret;i++){
    printf("entrer l'arret num %d :",i+1);
    scanf("%s%s%d",sommet1,sommet2,&poids);
    ind1=trouver_indice(tablehachage,sommet1);
    ind2=trouver_indice(tablehachage,sommet2);
    addEdge(graph,ind1,ind2,poids);
   }
printf("entrer la sourece :");
scanf("%s",sommet1);
int i=trouver_indice(tablehachage,sommet1);

dijkstra(graph,tablehachage,i);


return 0;
}