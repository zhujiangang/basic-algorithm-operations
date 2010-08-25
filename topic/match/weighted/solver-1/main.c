#include "graphtypes.h"

main(argc,argv)
int argc;
char *argv[];

{
	Graph graph;
	int *Mate;
	int i,size;

	if (argc < 2){
		printf("Usage: wmatch datafile\n");
		exit(1);
	}
	graph = ReadGraph(&size,argv[1]);
	Mate = Weighted_Match(graph,1);

	for (i=1; i<=size; i++)
		printf("%d %d\n",i,Mate[i]);
}


