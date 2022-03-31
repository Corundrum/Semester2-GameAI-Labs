#pragma once
#ifndef __DECISION_TREE__
#define __DECISION_TREE__

#include "Agent.h"
#include "CloseCombatCondition.h"
#include "LOSCondition.h"
#include "RadiusCondition.h"
#include "TreeNode.h"
#include "TreeNodeType.h"

class DecisionTree
{
public:
	//Constructor
	DecisionTree();
	DecisionTree(Agent* agent);

	//Destructor
	~DecisionTree();

	//Getters and Setters
	Agent* getAgent() const;
	void setAgent(Agent* agent);

	//readonly Getters
	LOSCondition* getLOSNode() const;
	RadiusCondition* getRadiusNode() const;
	CloseCombatCondition* getCloseCombatNode() const;

	//Public Functions
	TreeNode* addNode(TreeNode* parent, TreeNode* child_node, TreeNodeType type);
	void display();
	void update();
	void clean();

	void makeDecision(); //in order tree traversal

private:
	//Private Instance Members
	Agent* m_agent;

	LOSCondition* m_LOSNode;
	RadiusCondition* m_RadiusNode;
	CloseCombatCondition* m_CloseCombatNode;

	std::vector<TreeNode*> m_treeNodeList;

	//Private Methods
	//void m_buildTree();
};

#endif // !__DECISION_TREE__

