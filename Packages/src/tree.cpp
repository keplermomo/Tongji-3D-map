#include "tree.h"
#include "shader.h"
#include <iostream>
using namespace std;
extern Shader ourShader;


CTree::CTree()
{
	pRoot = NULL;
}

CTree::CTree(TreeDataType Value)
{
	pRoot = new STreeNode(Value);
	if (pRoot == NULL)
		return;
}

CTree::~CTree()
{
	if (pRoot == NULL)
		return;

	FreeMemory(pRoot);
}

void CTree::FreeMemory(pSTreeNode pNode)
{
	if (pNode == NULL)
		return;

	if (pNode->pFirstChild != NULL)
		FreeMemory(pNode->pFirstChild);

	if (pNode->pNextBrother != NULL)
		FreeMemory(pNode->pNextBrother);

	delete pNode;
	pNode = NULL;
}

void CTree::NodePrint(pSTreeNode pNode)
{
	if (pNode == NULL)
		return;
	
	cout << pNode->data.id << endl;
	NodePrint(pNode->pFirstChild);
	NodePrint(pNode->pNextBrother);
}

void CTree::InsertChild(TreeDataType parentValue, TreeDataType Value)
{
	if (pRoot == NULL)
		return;

	pSTreeNode pFindNode = Search(pRoot, parentValue);
	if (pFindNode == NULL)
		return;

	if (pFindNode->pFirstChild == NULL)
	{
		pFindNode->pFirstChild = new STreeNode(Value);
		pFindNode->pFirstChild->pParent = pFindNode;
		return;
	}
	else
	{
		InsertBrother(pFindNode->pFirstChild, Value);
		return;
	}
}

void CTree::InsertBrother(pSTreeNode pBrotherNode, TreeDataType Value)
{
	if (pBrotherNode->pNextBrother != NULL)
		InsertBrother(pBrotherNode->pNextBrother, Value);
	else
	{
		pBrotherNode->pNextBrother = new STreeNode(Value);
		pBrotherNode->pNextBrother->pParent = pBrotherNode->pParent;
		return;
	}
}

pSTreeNode CTree::Search(pSTreeNode pNode, TreeDataType Value)
{
	if (pNode == NULL)
		return NULL;

	if (pNode->data.id == Value.id)   //找到对应值的条件
		return pNode;

	if (pNode->pFirstChild == NULL && pNode->pNextBrother == NULL)
		return NULL;
	else
	{
		if (pNode->pFirstChild != NULL)
		{
			pSTreeNode pNodeTemp = Search(pNode->pFirstChild, Value);
			if (pNodeTemp != NULL)
				return pNodeTemp;
			else
			{
				return Search(pNode->pNextBrother, Value);
			}
		}
		else
			return Search(pNode->pNextBrother, Value);
	}
}




void CTree::NodeDraw(pSTreeNode pNode,Shader ourShader)
{
	if (pNode == NULL)
		return;
	ourShader.setMat4("model", pNode->data.model);
	ourShader.setVec4("MyColor", pNode->data.color);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	NodeDraw(pNode->pFirstChild, ourShader);
	NodeDraw(pNode->pNextBrother, ourShader);

}

void CTree::NodeRotate(pSTreeNode pNode, float angle, v3 axis, v3 guanjie)
{
	if (pNode == NULL)
		return;
	glm::mat4 model = glm::mat4(1.0f);

	model = glm::translate(model, guanjie);
	model = glm::rotate(model, angle, axis);
	model = glm::translate(model, -guanjie);
	
	pNode->data.model = model* pNode->data.model;
	NodeRotate(pNode->pFirstChild, angle, axis, guanjie);
	NodeRotate(pNode->pNextBrother, angle, axis, guanjie);
}

void CTree::NodeTranslate(pSTreeNode pNode, v3 move)
{
	if (pNode == NULL)
		return;

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, move);
	pNode->data.model = model * pNode->data.model;
	pNode->data.position += move;
	//cout << pNode->data.position.z << endl;;
	NodeTranslate(pNode->pFirstChild, move);
	NodeTranslate(pNode->pNextBrother, move);
}

void CTree::NodeScale(pSTreeNode pNode, v3 scale)
{
	if (pNode == NULL)
		return;
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, pRoot->data.position);
	model = glm::scale(model, scale);
	model = glm::translate(model, -pRoot->data.position);
	pNode->data.model = model * pNode->data.model;

	NodeScale(pNode->pFirstChild,scale);
	NodeScale(pNode->pNextBrother, scale);
}
