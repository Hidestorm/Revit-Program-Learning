#include "TreeStruct.h"

bool Node::isNodeRoot(char * node)
{
	return *(bool*)(node + IS_ROOT_OFFSET);
}

void Node::SetNodeRoot(char * node, bool isRoot)
{
	*(node + IS_ROOT_OFFSET) = isRoot;
}

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
	SetNodeRoot(node, false);
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

char * Node::GetInternalNodeNumKeys(char * node)
{
	return node + INTERNAL_NODE_NUM_KEYS_OFFSET;;
}

char * Node::GetInternalNodeRightChild(char * node)
{
	return node + INTERNAL_NODE_RIGHT_CHILD_OFFSET;
}

char * Node::GetInternalNodeCell(char * node, uint32_t cellNum)
{
	return node + INTERNAL_NODE_HEADER_SIZE + cellNum * INTERNAL_NODE_CHILD_SIZE;
}

char * Node::GetInternalNodeChild(char * node, uint32_t childNum)
{
	uint32_t num_keys = *GetInternalNodeNumKeys(node);
	if (childNum > num_keys)
	{
		std::cout << "Tried to access child_num " << childNum << "> num_keys" << num_keys << std::endl;
		exit(EXIT_FAILURE);
	}
	else if (childNum == num_keys)
	{
		return GetInternalNodeRightChild(node);
	}
	else
	{
		return GetNodeCell(node, childNum);
	}
}

char * Node::GetInternalNodeKey(char * node, uint32_t keyNum)
{
	return GetNodeCell(node, keyNum) + INTERNAL_NODE_CHILD_SIZE;
}

void Node::InitializeInternalNode(char * node)
{
	SetNodeType(node, NODE_INTERNAL);
	SetNodeRoot(node, true);
	*GetInternalNodeNumKeys(node) = 0;
}

uint32_t Node::GetNodeMaxKey(char * node)
{
	switch (GetNodeType(node))
	{
	case NODE_INTERNAL:
		return *GetInternalNodeKey(node, *GetInternalNodeNumKeys(node) - 1);
	case NODE_LEAF:
		return *GetNodeCell(node, *GetNodeNumCells(node) - 1);
	}
}
