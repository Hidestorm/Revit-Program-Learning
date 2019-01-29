#pragma once
#include <stdint.h>
#include <iostream>
#include "SqlTable.h"
//using namespace std;

enum NodeType
{
	NODE_INTERNAL,
	NODE_LEAF
};

/*
* Common Node Header Layout
*/
const uint32_t NODE_TYPE_SIZE = sizeof(NodeType);
const uint32_t NODE_TYPE_OFFSET = 0;
const uint32_t IS_ROOT_SIZE = sizeof(bool);
const uint32_t IS_ROOT_OFFSET = NODE_TYPE_SIZE;
const uint32_t PARENT_POINTER_SIZE =/* sizeof(uint32_t); */sizeof(char*);
const uint32_t PARENT_POINTER_OFFSET = IS_ROOT_OFFSET + IS_ROOT_SIZE;
const uint32_t COMMOND_NODE_HEADER_SIZE = NODE_TYPE_SIZE + IS_ROOT_SIZE + PARENT_POINTER_SIZE;

/*
* Leaf Node Header Layout
*/
const uint32_t LEAF_NODE_NUM_CELLS_SIZE = sizeof(uint32_t);
const uint32_t LEAF_NODE_NUM_CELLS_OFFSET = COMMOND_NODE_HEADER_SIZE;
const uint32_t LEAF_NODE_HEADER_SIZE = COMMOND_NODE_HEADER_SIZE + LEAF_NODE_NUM_CELLS_SIZE;

/*
* Leaf Node Body Layout
*/
const uint32_t LEAF_NODE_KEY_SIZE = sizeof(uint32_t);
const uint32_t LEAF_NODE_KEY_OFFSET = 0;
const uint32_t LEAF_NODE_VALUE_SIZE = ROW_SIZE;
const uint32_t LEAF_NODE_VALUE_OFFSET = LEAF_NODE_KEY_OFFSET + LEAF_NODE_KEY_SIZE;
const uint32_t LEAF_NODE_CELL_SIZE = LEAF_NODE_KEY_SIZE + LEAF_NODE_VALUE_SIZE;
const uint32_t LEAF_NODE_SPACE_FOR_CELLS = PAGE_SIZE - LEAF_NODE_HEADER_SIZE;
const uint32_t LEAF_NODE_MAX_CELLS = LEAF_NODE_SPACE_FOR_CELLS / LEAF_NODE_CELL_SIZE;

#define  LEAF_NODE_NUM_CELLS(node)		(char*)	(node + LEAF_NODE_NUM_CELLS_OFFSET)
#define  LEAF_NODE_CELL(node, cell_num) (char*) (node + LEAF_NODE_HEADER_SIZE + cell_num * LEAF_NODE_CELL_SIZE)
#define  LEAF_NODE_KEY(node, cell_num)	(char*) (LEAF_NODE_CELL(node, cell_num))
#define  LEAF_NODE_VALUE(node, cell_num)(char*) (LEAF_NODE_CELL(node, cell_num) + LEAF_NODE_KEY_OFFSET )
#define  INITIALIZE_LEAF_NODE(node)		*LEAF_NODE_NUM_CELLS(node) = 0


//void printfConstants()
//{
//	std::cout << "ROW_SIZE:" << ROW_SIZE << std::endl;
//	std::cout << "COMMON_NODE_HEADER_SIZE: " << COMMOND_NODE_HEADER_SIZE << std::endl;
//	std::cout << "LEAF_NODE_HEADER_SIZE: " << LEAF_NODE_HEADER_SIZE << std::endl;
//	std::cout << "LEAF_NODE_CELL_SIZE: " << LEAF_NODE_CELL_SIZE << std::endl;
//	std::cout << "LEAF_NODE_SPACE_FOR_CELLS: " << LEAF_NODE_SPACE_FOR_CELLS << std::endl;
//	std::cout << "LEAF_NODE_MAX_CELLS: " << LEAF_NODE_MAX_CELLS << std::endl;
//}
