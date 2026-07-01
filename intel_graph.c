#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_ENTITIES 10

typedef struct Node {
    int target_id;
    char relationship[64];
    struct Node* next;
} Node;

typedef struct {
    char entity_name[64];
    Node* head;
} Vertex;

typedef struct {
    Vertex array[MAX_ENTITIES];
    int num_vertices;
} IntelGraph;

#ifdef _WIN32
    __declspec(dllexport) IntelGraph* init_graph();
    __declspec(dllexport) void define_entity(IntelGraph* g, int id, const char* name);
    __declspec(dllexport) void link_entities(IntelGraph* g, int src, int dest, const char* rel);
    __declspec(dllexport) void trace_relationship_chain(IntelGraph* g, int start_id, char* path_buffer);
    __declspec(dllexport) void free_graph(IntelGraph* g);
#endif

IntelGraph* init_graph() {
    IntelGraph* g = (IntelGraph*)malloc(sizeof(IntelGraph));
    g->num_vertices = 0;
    for (int i = 0; i < MAX_ENTITIES; i++) g->array[i].head = NULL;
    return g;
}

void define_entity(IntelGraph* g, int id, const char* name) {
    if (id < MAX_ENTITIES) {
        strncpy(g->array[id].entity_name, name, sizeof(g->array[id].entity_name) - 1);
        if (id >= g->num_vertices) g->num_vertices = id + 1;
    }
}

void link_entities(IntelGraph* g, int src, int dest, const char* rel) {
    Node* n = (Node*)malloc(sizeof(Node));
    n->target_id = dest;
    strncpy(n->relationship, rel, sizeof(n->relationship) - 1);
    n->next = g->array[src].head;
    g->array[src].head = n;
}

void trace_relationship_chain(IntelGraph* g, int start_id, char* path_buffer) {
    bool visited[MAX_ENTITIES] = {false};
    int stack[MAX_ENTITIES];
    int top = -1;

    stack[++top] = start_id;
    visited[start_id] = true;

    while (top >= 0) {
        int curr = stack[top--];
        strcat(path_buffer, g->array[curr].entity_name);
        strcat(path_buffer, " -> ");

        Node* temp = g->array[curr].head;
        while (temp) {
            if (!visited[temp->target_id]) {
                visited[temp->target_id] = true;
                stack[++top] = temp->target_id;
            }
            temp = temp->next;
        }
    }
}

void free_graph(IntelGraph* g) {
    for (int i = 0; i < g->num_vertices; i++) {
        Node* curr = g->array[i].head;
        while (curr) {
            Node* temp = curr;
            curr = curr->next;
            free(temp);
        }
    }
    free(g);
}
