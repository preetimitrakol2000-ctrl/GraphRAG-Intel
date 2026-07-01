#ifndef INTEL_GRAPH_H
#define INTEL_GRAPH_H

typedef struct Node Node;
typedef struct IntelGraph IntelGraph;
IntelGraph* init_graph();
void define_entity(IntelGraph* g, int id, const char* name);
void link_entities(IntelGraph* g, int src, int dest, const char* rel);
void trace_relationship_chain(IntelGraph* g, int start_id, char* path_buffer);
void free_graph(IntelGraph* g);

#endif
