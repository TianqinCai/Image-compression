/**
 * @file quadtree.cpp
 * Quadtree class implementation.
 */

#include <cstddef>
#include <cstdlib>
#include <iostream>

using namespace std;

#include "quadtree.h"
#include "png.h"

// Quadtree
//   - parameters: none
//   - constructor for the Quadtree class; makes an empty tree
Quadtree::Quadtree() 
{
	root = NULL;
}



// Quadtree
//   - parameters: PNG const & source - reference to a const PNG
//                    object, from which the Quadtree will be built
//                 int resolution - resolution of the portion of source
//                    from which this tree will be built
//   - constructor for the Quadtree class; creates a Quadtree representing
//        the resolution by resolution block in the upper-left corner of
//        source
Quadtree::Quadtree(PNG const& source, int setresolution)
{
	cout << "calling buildtree" << endl;
	buildTree(source, setresolution);
}

// Quadtree
//   - parameters: Quadtree const & other - reference to a const Quadtree
//                    object, which the current Quadtree will be a copy of
//   - copy constructor for the Quadtree class
Quadtree::Quadtree(Quadtree const& other) 
{
	QuadtreeNode* temp = root;
	QuadtreeNode* othertemp = other.root;
	root = NULL;
	delete temp;
	res = other.res;
	temp->x = othertemp->x;
	temp->y = othertemp->y;
	copytree(othertemp, temp);
}
void Quadtree::copytree(QuadtreeNode* othertemp, QuadtreeNode * temp)
{
	if ((othertemp->neChild == NULL) && (othertemp->nwChild == NULL) && (othertemp->seChild == NULL) && (othertemp->swChild == NULL))
	{
		temp->neChild = NULL;
		temp->nwChild = NULL;
		temp->seChild = NULL;
		temp->swChild = NULL;
		temp->element.alpha = othertemp->element.alpha;
		temp->element.red = othertemp->element.red;
		temp->element.blue = othertemp->element.blue;
		temp->element.green = othertemp->element.green;
		return;
	}
	else
	{
		temp->nwChild = new QuadtreeNode();
		temp->nwChild->x = temp->x / 2;
		temp->nwChild->y = temp->y/ 2;
		copytree(othertemp->nwChild, temp->nwChild);

		temp->neChild = new QuadtreeNode();
		temp->neChild->x = temp->x/ 2 + temp->x;
		temp->neChild->y = temp->y / 2;
		copytree(othertemp->neChild, temp->neChild);

		temp->swChild = new QuadtreeNode();
		temp->swChild->x = temp->x / 2;
		temp->swChild->y = temp->y / 2 + temp->y;
		copytree(othertemp->swChild, temp->swChild);

		temp->seChild = new QuadtreeNode();
		temp->seChild->x = temp->x / 2 + temp->x;
		temp->seChild->y = temp->y / 2 + temp->y;
		copytree(othertemp->seChild, temp->seChild);

	}
}

// ~Quadtree
//   - parameters: none
//   - destructor for the Quadtree class
Quadtree::~Quadtree()
{
	deletetree(root);
}

void Quadtree::deletetree(QuadtreeNode * temp)
{
	if ((temp->neChild == NULL) && (temp->nwChild == NULL) && (temp->seChild == NULL) && (temp->swChild == NULL))
	{
		QuadtreeNode* del = temp;
		del = NULL;
		delete temp;
		return;
	}
	else
	{
		if (temp->neChild)
			deletetree(temp->neChild);
		if (temp->nwChild)
			deletetree(temp->nwChild);
		if (temp->seChild)
			deletetree(temp->seChild);
		if (temp->swChild)
			deletetree(temp->swChild);
	}
}

// operator=
//   - parameters: Quadtree const & other - reference to a const Quadtree
//                    object, which the current Quadtree will be a copy of
//   - return value: a const reference to the current Quadtree
//   - assignment operator for the Quadtree class
Quadtree const& Quadtree::operator=(Quadtree const& other)
{
	QuadtreeNode* temp = root;
	QuadtreeNode* othertemp = other.root;
	root = NULL;
	delete temp;
	res = other.res;
	temp->x = othertemp->x;
	temp->y = othertemp->y;
	copytree(othertemp, temp);
	return *this;
}

// buildTree (public interface)
//   - parameters: PNG const & source - reference to a const PNG
//                    object, from which the Quadtree will be built
//                 int resolution - resolution of the portion of source
//                    from which this tree will be built
//   - transforms the current Quadtree into a Quadtree representing
//        the resolution by resolution block in the upper-left corner of
//        source



void Quadtree::init(PNG const& source, QuadtreeNode * temp, int res)
{
	if (res == 1)
	{
		temp->neChild = NULL;
		temp->nwChild = NULL;
		temp->seChild = NULL;
		temp->swChild = NULL;
		
		temp->element.alpha = source.operator()(temp->x, temp->y)->alpha;
		temp->element.red = source.operator()(temp->x, temp->y)->red;
		temp->element.green = source.operator()(temp->x, temp->y)->green;
		temp->element.blue = source.operator()(temp->x, temp->y)->blue;
		return;
	}
	else
	{
		temp->nwChild = new QuadtreeNode();
		temp->nwChild->x = res / 2;
		temp->nwChild->y = res / 2;
		init(source, temp->nwChild, res / 2);
		cout << temp->nwChild->element.red<<endl;

		temp->neChild = new QuadtreeNode();
		temp->neChild->x = res / 2+res;
		temp->neChild->y = res / 2;
		init(source, temp->neChild, res / 2);
		cout << temp->neChild->element.red << endl;

		temp->swChild = new QuadtreeNode();
		temp->swChild->x = res / 2;
		temp->swChild->y = res / 2+res;
		init(source, temp->swChild, res / 2);
		cout << temp->swChild->element.red << endl;

		temp->seChild = new QuadtreeNode();
		temp->seChild->x = res / 2 + res;
		temp->seChild->y = res / 2 + res;
		init(source, temp->seChild, res / 2);
		cout << temp->seChild->element.red << endl;

		temp->element.alpha = (temp->neChild->element.alpha + temp->nwChild->element.alpha + temp->seChild->element.alpha + temp->swChild->element.alpha) / 4;
		temp->element.blue = (temp->neChild->element.blue + temp->nwChild->element.blue + temp->seChild->element.blue + temp->swChild->element.blue) / 4;
		temp->element.red = (temp->neChild->element.red + temp->nwChild->element.red + temp->seChild->element.red + temp->swChild->element.red) / 4;
		temp->element.green = (temp->neChild->element.green + temp->nwChild->element.green + temp->seChild->element.green + temp->swChild->element.green) / 4;
	}
}


void Quadtree::buildTree(PNG const& source, int setresolution)
{
	cout << "starting build tree" << endl;
	//if (root!=NULL)
	//{
	//	~Quadtree();
	//	//QuadtreeNode* temp = root;
	//	//root = NULL;
	//	//delete temp;
	//}

	root = new QuadtreeNode();
	cout << "building root" << endl;
	//root->element = source.operator()(setresolution/2, setresolution/2);
	root->x = setresolution / 2;
	root->y = setresolution / 2;
	init(source, root, setresolution/2);
	res = setresolution;
	//QuadtreeNode temp = root;
	//while (temp.neChild != NULL || temp.nwChild != NULL || temp.seChild != NULL || temp.swChild != NULL)
	//{
	//	cout <<t
	//}

}

// getPixel (public interface)
//   - parameters: int x, int y - coordinates of the pixel to be retrieved
//   - return value: an RGBAPixel representing the desired pixel of the
//        underlying bitmap
//   - retrieves and returns the pixel at coordinates (x, y) in the
//        underlying bitmap
//RGBAPixel Quadtree::getPixel(int x, int y) const
//{
//	//return 
//}

RGBAPixel Quadtree::getPixelhelper(QuadtreeNode * temp, int x, int y) const
{
	if (x == temp->x&&y == temp->y)
	{
		return temp->element;
	}
	else if (x < temp->x && y<temp->y)
	{
		getPixelhelper(temp->nwChild, x, y);
	}
	else if (x<temp->x&&y>temp->y)
	{
		getPixelhelper(temp->swChild, x, y);
	}
	else if (x > temp->x&&y < temp->y)
	{
		getPixelhelper(temp->neChild, x, y);
	}
	else if (x > temp->x&&y > temp->y) {
		getPixelhelper(temp->seChild, x, y);
	}
	return temp->element;
}

RGBAPixel Quadtree::getPixel(int x, int y) const
{
	return getPixelhelper(root,x,y);
}

// decompress (public interface)
//   - parameters: none
//   - return value: a PNG object representing this quadtree's underlying
//        bitmap
//   - constructs and returns this quadtree's underlying bitmap
//PNG Quadtree::decompress() const
//{
//}

PNG Quadtree::decompress() const
{
	PNG result(res,res);


	return PNG();
}

// clockwiseRotate (public interface)
//   - parameters: none
//   - transforms this quadtree into a quadtree representing the same
//        bitmap, rotated 90 degrees clockwise
void Quadtree::clockwiseRotate()
{
}

// prune (public interface)
//   - parameters: int tolerance - an integer representing the maximum
//                    "distance" which we will permit between a node's color
//                    (i.e. the average of its descendant leaves' colors)
//                    and the color of each of that node's descendant leaves
//   - for each node in the quadtree, if the "distance" between the average
//        of that node's descendant leaves' colors and the color of each of
//        that node's descendant leaves is at most tolerance, this function
//        deletes the subtrees beneath that node; we will let the node's
//        color "stand in for" the colors of all (deleted) leaves beneath it
void Quadtree::prune(int tolerance)
{
}

// pruneSize (public interface)
//   - parameters: int tolerance - an integer representing the maximum
//                    "distance" which we will permit between a node's color
//                    (i.e. the average of its descendant leaves' colors)
//                    and the color of each of that node's descendant leaves
//   - returns the number of leaves which this quadtree would contain if it
//        was pruned using the given tolerance; does not actually modify the
//        tree
int Quadtree::pruneSize(int tolerance) const
{
	return tolerance;
}

// idealPrune (public interface)
//   - parameters: int numLeaves - an integer representing the number of
//                    leaves we wish the quadtree to have, after pruning
//   - returns the minimum tolerance such that pruning with that tolerance
//        would yield a tree with at most numLeaves leaves
int Quadtree::idealPrune(int numLeaves) const
{
	return numLeaves;
}

// QuadtreeNode
//   - parameters: none
//   - constructor for the QuadtreeNode class; creates an empty
//        QuadtreeNode, with all child pointers NULL
Quadtree::QuadtreeNode::QuadtreeNode()
{
    neChild = seChild = nwChild = swChild = NULL;
}

// QuadtreeNode
//   - parameters: RGBAPixel const & elem - reference to a const
//        RGBAPixel which we want to store in this node
//   - constructor for the QuadtreeNode class; creates a QuadtreeNode
//        with element elem and all child pointers NULL
Quadtree::QuadtreeNode::QuadtreeNode(RGBAPixel const& elem)
{
    element = elem;
    neChild = seChild = nwChild = swChild = NULL;
}

