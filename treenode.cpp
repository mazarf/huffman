// Nodes of a binary tree. For use in Huffman Encoding

#ifndef TREENODE_CPP
#define TREENODE_CPP

#include <iostream>
using namespace std;

class TreeNode
{
	public:

	TreeNode();
	TreeNode(int f, unsigned char c, TreeNode* l, TreeNode* r);
	
	// overloaded operators - for heap
	bool operator<(TreeNode t);
	bool operator<=(TreeNode t);
	bool operator==(TreeNode t);
	bool operator>(TreeNode t);
	
	int frequency;
	unsigned char character;
	
	TreeNode *left, *right;	
		
}; // treenode

// constructors

TreeNode::TreeNode()
{
	frequency = 0;
}

TreeNode::TreeNode(int f, unsigned char c, TreeNode* l, TreeNode* r)
{
	frequency = f;
	character = c;
	left = l;
	right = r;
}

// end constructors

// operators

bool TreeNode::operator<(TreeNode t)
{
	if(frequency < t.frequency)
	{
		return true;
	}
	else if(frequency == t.frequency)
	{ // tie breaker
		if(character < t.character)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
} // less than

bool TreeNode::operator<=(TreeNode t)
{
	if((*this) < t || (*this) == t)
		return true;
	else
		return false;
} // less than or equal to

bool TreeNode::operator==(TreeNode t)
{ // this should never ever happen but whatever

	if(frequency == t.frequency && character == t.character)
	{
		//cout << "wtf";
		return true;
	}
	else
		return false;
} // equals

bool TreeNode::operator>(TreeNode t)
{
	
	if(frequency > t.frequency)
	{
		return true;
	}
	else if(frequency == t.frequency)
	{
		if(character > t.character)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
		
} // greater than

// end operators

#endif
