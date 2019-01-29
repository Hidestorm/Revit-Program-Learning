#include "TreeStruct.h"

char * Node::GetNodeNumCells(char * node)
{
	return node + LEAF_NODE_NUM_CELLS_OFFSET;
}

char * Node::GetNodeCell(char * node, uint32_t cellNum)
{
	return node + LEAF_NODE_HEADER_SIZE + cellNum * LEAF_NODE_CELL_SIZE;
}

char * Node::GetNodeKey(char * node, uint32_t cellNum)
{
	return GetNodeCell(node, cellNum);
}

char * Node::GetNodeValue(char * node, uint32_t cellNum)
{
	return GetNodeCell(node, cellNum) + LEAF_NODE_KEY_OFFSET;
}

void Node::InitializeNode(char * node)
{
	SetNodeType(node, NODE_LEAF);
	*GetNodeNumCells(node) = 0;
}

NodeType Node::GetNodeType(char * node)
{
	return NodeType(*(node + NODE_TYPE_OFFSET));
}

void Node::SetNodeType(char * node, NodeType type)
{
	*(node + NODE_TYPE_OFFSET) = type;
}
