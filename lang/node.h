#ifndef NODE_H
#define NODE_H

typedef enum {
	nodeProgram,
} NodeType;

typedef struct Node Node;
struct Node {
	NodeType type;
	Node     *next, *child;
};


#endif
