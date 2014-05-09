#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <getopt.h>

#include "CycleTimer.h"
#include "graph.h"
#include "bfs.h"


#define USE_BINARY_GRAPH 1


void usage(const char* progname) {
    printf("Usage: %s [options] graph_file_name\n", progname);
    printf("Program Options:\n");
    printf("  -t  --threads <N>  Use T threads\n");
    printf("  -?  --help         This message\n");
}

int main(int argc, char** argv) {

    int  num_threads = -1;
    std::string graph_filename;

    // parse commandline options ////////////////////////////////////////////
    int opt;
    static struct option long_options[] = {
        {"threads", 1, 0, 't'},
        {"help", 0, 0, '?'},
        {0 ,0, 0, 0}
    };

    while ((opt = getopt_long(argc, argv, "t:?", long_options, NULL)) != EOF) {

        switch (opt) {
        case 't':
        {
            num_threads = atoi(optarg);
            break;
        }
        case '?':
        default:
            usage(argv[0]);
            return 1;
        }
    }
    // end parsing of commandline options


    if (optind >= argc) {
        usage(argv[0]);
        return 1;
    }

    graph_filename = argv[optind];

    graph g;

    printf("Loading graph (this can take some time for the bigger graphs)...\n");
    load_graph_binary(graph_filename.c_str(), &g);

    printf("Graph stats:\n");
    printf("  Edges: %d\n", g.num_edges);
    printf("  Nodes: %d\n", g.num_nodes);

    solution sol1;
    sol1.distances = (int *)malloc(sizeof(int)*g.num_nodes);

    // Execute top-down version

    double start_time = CycleTimer::currentSeconds();
    bfs_top_down(&g, &sol1);
    double end_time = CycleTimer::currentSeconds();
    double par_time = end_time - start_time;
    printf("Parallel Top down BFS time: %.3f sec\n", par_time);

    return 0;
}
