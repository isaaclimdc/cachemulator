#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cstddef>
#include <omp.h>
#include <numeric>
#include <pthread.h>

#include "CycleTimer.h"
#include "bfs.h"
#include "graph.h"

#define NO_DEBUG
#define ROOT_NODE_ID 0
#define NOT_VISITED_MARKER -1
#define HYBRID_THRESH 0.02

#define MAX_THREADS 100

/*******************************/
/****** Utility functions ******/
/*******************************/

void printArray(int *A, int N) {
    printf("[");
    for (int i = 0; i < N; i++) {
        printf("%d%s", A[i], (i==N-1 ? "" : ", "));
    }
    printf("]\n");
}

void printTime(const char *str, double startTime, double endTime) {
#ifndef NO_DEBUG
    printf("%s: %0.5f\n", str, (endTime-startTime));
#endif
}

void printVertexSet(const char *str, vertex_set *vset) {
#ifndef NO_DEBUG
    printf("%s:\n", str);
    printf("Count: %d\n", vset->count);
    printf("Present: ");
    printArray(vset->present, vset->count);
#endif
}

void vertex_set_clear(vertex_set *list) {
    list->count = 0;
}

void vertex_set_init(vertex_set *list, int count) {
    list->alloc_count = count;
    list->present = (int *)malloc(sizeof(int) * list->alloc_count);
    vertex_set_clear(list);
}

/**********************/
/****** Top-down ******/
/**********************/

void *top_down_thread(void *arg) {
    for (int v=0; v<g->num_nodes; v++) {
        if (frontier[v]) {
            int start_edge = g->outgoing_starts[v];
            int end_edge = (v == g->num_nodes-1) ?
                               g->num_edges :
                               g->outgoing_starts[v+1];

            // Attempt to add all neighbors to the new frontier
            for (int ngh=start_edge; ngh<end_edge; ngh++) {
                int outgoing = g->outgoing_edges[ngh];

                if (distances[outgoing] == NOT_VISITED_MARKER) {
                    distances[outgoing] = distances[v]+1;
                    newFrontier[outgoing] = true;

                    if (!hasNewFrontier) {
                        hasNewFrontier = true;
                    }
                }
            }
        }
    }
}

void top_down_step(
    graph *g,
    bool *frontier,
    bool *newFrontier,
    int *distances,
    bool &hasNewFrontier)
{
    int num_threads = g->num_nodes;
    pthread_t threads[MAX_THREADS];

    for (int i=0; i<num_threads; i++) {
      pthread_create(&threads[i], 0, threadFunc, p);
    }

    for (int i=0; i<num_threads; i++) {
      pthread_join(threads[i], 0);
    }
}

void bfs_top_down(
    graph *graph,
    solution *sol)
{
    bool *frontier = (bool *)calloc(graph->num_nodes, sizeof(bool));
    bool *newFrontier = (bool *)calloc(graph->num_nodes, sizeof(bool));

    // Initialize all nodes to NOT_VISITED_MARKER
    //#pragma omp parallel for
    for (int i=0; i<graph->num_nodes; i++)
        sol->distances[i] = NOT_VISITED_MARKER;

    // Setup frontier with the root node
    frontier[ROOT_NODE_ID] = 1;
    sol->distances[ROOT_NODE_ID] = 0;

    bool hasNewFrontier = true;
    while (hasNewFrontier) {
        memset(newFrontier, false, sizeof(bool) * graph->num_nodes);
        hasNewFrontier = false;

        top_down_step(graph, frontier, newFrontier, sol->distances,
                      hasNewFrontier);

        bool *tmp = frontier;
        frontier = newFrontier;
        newFrontier = tmp;
    }
}
