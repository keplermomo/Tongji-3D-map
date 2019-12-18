#ifndef TREE_H
#define TREE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"


typedef glm::vec3 v3;
typedef glm::vec4 v4;
typedef struct robot {
	v3 position;
	v4 color;
	v3 size;
	glm::mat4 model;
	int id;
	robot() {};
	robot(v3 p, v4 c, v3 s, int i) {
		position = p;
		color = c;
		size = s;
		id = i;
		model = glm::mat4(1.0f);
		model = glm::translate(model, position);
		model = glm::scale(model, size);
	}
}robot;
typedef robot TreeDataType;
typedef struct STreeNode* pSTreeNode;


struct STreeNode
{
	TreeDataType data;
	pSTreeNode pFirstChild;
	pSTreeNode pNextBrother;
	pSTreeNode pParent;

	STreeNode(TreeDataType Value)
	{
		data = Value;
		pFirstChild = NULL;
		pNextBrother = NULL;
		pParent = NULL;
	}
};


class CTree
{
public:
	CTree();
	CTree(TreeDataType Value);
	~CTree();

public:
	void InsertChild(TreeDataType parentValue, TreeDataType Value);    //  parentValue：该节点的父结点；Value：该节点的值
	void InsertBrother(pSTreeNode pBrotherNode, TreeDataType Value);
	pSTreeNode Search(pSTreeNode pNode, TreeDataType Value);

	//旋转某一结点及其所有子节点
	void NodeDraw(pSTreeNode pNode, Shader ourShader);
	void NodeRotate(pSTreeNode pNode, float angle, v3 axis, v3 guanjie);
	void NodeTranslate(pSTreeNode pNode, v3 move);
	void NodeScale(pSTreeNode pNode, v3 scale);
	void FreeMemory(pSTreeNode pNode);    //  释放内存
	void NodePrint(pSTreeNode pNode);

public:
	pSTreeNode pRoot;
};

#endif