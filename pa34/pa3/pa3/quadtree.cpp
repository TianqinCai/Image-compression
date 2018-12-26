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
	if (root) {
		deletetree(root);
		root = new QuadtreeNode();
	}
	
	buildTree(source, setresolution);
}

// Quadtree
//   - parameters: Quadtree const & other - reference to a const Quadtree
//                    object, which the current Quadtree will be a copy of
//   - copy constructor for the Quadtree class
Quadtree::Quadtree(Quadtree const& other) 
{
	if (root) {
		deletetree(root);
	}
	root = new QuadtreeNode();
	res = other.res;
	root->x = other.root->x;
	root->y = other.root->y;
	copytree(other.root, root,res);
}
void Quadtree::copytree(QuadtreeNode* othertemp, QuadtreeNode * temp, int res)
{
	if (res == 1||((othertemp->neChild == NULL) && (othertemp->nwChild == NULL) && (othertemp->seChild == NULL) && (othertemp->swChild == NULL)))
	{
		
		temp->neChild = NULL;
		temp->nwChild = NULL;
		temp->seChild = NULL;
		temp->swChild = NULL;

		temp->element.alpha = othertemp->element.alpha;
		temp->element.red = othertemp->element.red;
		temp->element.green = othertemp->element.green;
		temp->element.blue = othertemp->element.blue;

		return;
	}
	else
	{
		
		temp->nwChild = new QuadtreeNode();

		temp->seChild = new QuadtreeNode();

		temp->swChild = new QuadtreeNode();

		temp->neChild = new QuadtreeNode();

		temp->nwChild->x = othertemp->nwChild->x;
		temp->nwChild->y = othertemp->nwChild->y;

		temp->neChild->x = othertemp->neChild->x;
		temp->neChild->y = othertemp->neChild->y;

		temp->swChild->x = othertemp->swChild->x;
		temp->swChild->y = othertemp->swChild->y;

		temp->seChild->x = othertemp->seChild->x;
		temp->seChild->y = othertemp->seChild->y;

		copytree(othertemp->nwChild, temp->nwChild, res / 2);
		copytree(othertemp->neChild, temp->neChild, res / 2);
		copytree(othertemp->swChild, temp->swChild, res / 2);
		copytree(othertemp->seChild, temp->seChild, res / 2);

		temp->element.alpha = (temp->neChild->element.alpha + temp->nwChild->element.alpha + temp->seChild->element.alpha + temp->swChild->element.alpha) / 4;
		temp->element.blue = (temp->neChild->element.blue + temp->nwChild->element.blue + temp->seChild->element.blue + temp->swChild->element.blue) / 4;
		temp->element.red = (temp->neChild->element.red + temp->nwChild->element.red + temp->seChild->element.red + temp->swChild->element.red) / 4;
		temp->element.green = (temp->neChild->element.green + temp->nwChild->element.green + temp->seChild->element.green + temp->swChild->element.green) / 4;
		return;

	}
}

// ~Quadtree
//   - parameters: none
//   - destructor for the Quadtree class
Quadtree::~Quadtree()
{
	if (root)
	{		
		deletetree(root);
	}
}

void Quadtree::deletetree(QuadtreeNode * temp)
{
	if ((temp->neChild == NULL) && (temp->nwChild == NULL) && (temp->seChild == NULL) && (temp->swChild == NULL))
	{
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

		//QuadtreeNode* del = temp;
		//temp = NULL;
		//delete del;
		delete temp;

	}
}

// operator=
//   - parameters: Quadtree const & other - reference to a const Quadtree
//                    object, which the current Quadtree will be a copy of
//   - return value: a const reference to the current Quadtree
//   - assignment operator for the Quadtree class
Quadtree const& Quadtree::operator=(Quadtree const& other)
{
	if (root) {
		deletetree(root);
	}
	res = other.res;
	root = new QuadtreeNode();
	root->x = other.root->x;
	root->y = other.root->y;
	copytree(other.root, root,res);
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

void Quadtree::buildTreehelper(PNG const& source, QuadtreeNode * temp, int res)
{

	if (res == 1)
	{
		temp->neChild = NULL;
		temp->nwChild = NULL;
		temp->seChild = NULL;
		temp->swChild = NULL;
		
		temp->element.alpha = source(temp->x - 1, temp->y - 1)->alpha;
		temp->element.blue = source(temp->x - 1, temp->y - 1)->blue;
		temp->element.red = source(temp->x - 1, temp->y - 1)->red;
		temp->element.green = source(temp->x - 1, temp->y - 1)->green;

		return;
	}
	else
	{
		temp->nwChild = new QuadtreeNode();
		temp->seChild = new QuadtreeNode();
		temp->swChild = new QuadtreeNode();
		temp->neChild = new QuadtreeNode();

		temp->nwChild->x = temp->x- res / 4.0;
		temp->nwChild->y = temp->y- res / 4.0;

		temp->neChild->x = temp->x + res / 4.0;
		temp->neChild->y = temp->y - res / 4.0;

		temp->swChild->x = temp->x - res / 4.0;
		temp->swChild->y = temp->y + res / 4.0;

		temp->seChild->x = temp->x + res / 4.0;
		temp->seChild->y = temp->y + res / 4.0;

		buildTreehelper(source, temp->nwChild, res / 2);
		buildTreehelper(source, temp->neChild, res / 2);
		buildTreehelper(source, temp->swChild, res / 2);
		buildTreehelper(source, temp->seChild, res / 2);

		temp->element.alpha = (temp->neChild->element.alpha + temp->nwChild->element.alpha + temp->seChild->element.alpha + temp->swChild->element.alpha) / 4;
		temp->element.blue = (temp->neChild->element.blue + temp->nwChild->element.blue + temp->seChild->element.blue + temp->swChild->element.blue) / 4;
		temp->element.red = (temp->neChild->element.red + temp->nwChild->element.red + temp->seChild->element.red + temp->swChild->element.red) / 4;
		temp->element.green = (temp->neChild->element.green + temp->nwChild->element.green + temp->seChild->element.green + temp->swChild->element.green) / 4;

		return;

	}
}

void Quadtree::buildTree(PNG const& source, int setresolution)
{

	//if (root!=NULL)
	//{
	//	cout << "old tree exist" << endl;
	//	deletetree(root);
	//}

	root = new QuadtreeNode();
	root->x = (setresolution+1.0) /2.0;
	root->y = (setresolution+1.0) / 2.0;
	buildTreehelper(source, root, setresolution);
	res = setresolution;


}

// getPixel (public interface)
//   - parameters: int x, int y - coordinates of the pixel to be retrieved
//   - return value: an RGBAPixel representing the desired pixel of the
//        underlying bitmap
//   - retrieves and returns the pixel at coordinates (x, y) in the
//        underlying bitmap

RGBAPixel Quadtree::getPixelhelper(QuadtreeNode * temp, int a, int b) const
{
	if (a == temp->x&&b == temp->y)
	{
		RGBAPixel result=temp->element;
		return result;
	}
	else if (a <= temp->x && b<=temp->y)
	{
		 return getPixelhelper(temp->nwChild, a, b);
	}
	else if (a<=temp->x&&b>=temp->y)
	{
		return getPixelhelper(temp->swChild, a, b);
	}
	else if (a >= temp->x&&b <= temp->y)
	{
		return getPixelhelper(temp->neChild, a, b);
	}
	else if (a >= temp->x&&b >= temp->y) {
		return getPixelhelper(temp->seChild, a, b);
	}
	RGBAPixel result = temp->element;
	return result;
}

RGBAPixel Quadtree::getPixel(int x, int y) const
{
	RGBAPixel result = getPixelhelper(root, x+1, y+1);
	return result;
}

// clockwiseRotate (public interface)
//   - parameters: none
//   - transforms this quadtree into a quadtree representing the same
//        bitmap, rotated 90 degrees clockwise

void Quadtree::Rotatehelper1(QuadtreeNode* temp, QuadtreeNode* othertemp, int res) {

	if((othertemp->nwChild == NULL) &&(temp->nwChild == NULL))
	 {

		//if we reached bottom, then just do the rotate by assigning values
		temp->element.alpha = othertemp->element.alpha;
		temp->element.red = othertemp->element.red;
		temp->element.green = othertemp->element.green;
		temp->element.blue = othertemp->element.blue;
		return;
	}

	else if ((othertemp->nwChild == NULL) && (othertemp->neChild == NULL) && (othertemp->seChild == NULL) && (othertemp->swChild == NULL)
		&&(temp->nwChild) && (temp->neChild) && (temp->swChild) && (temp->seChild))
	{
		
		temp->element.alpha = othertemp->element.alpha;
		temp->element.red = othertemp->element.red;
		temp->element.green = othertemp->element.green;
		temp->element.blue = othertemp->element.blue;

		deletetree(temp->neChild);
		temp->neChild = NULL;
		deletetree(temp->nwChild);
		temp->nwChild = NULL;
		deletetree(temp->seChild);
		temp->seChild = NULL;
		deletetree(temp->swChild);
		temp->swChild = NULL;

		return;
	}
	else if ((temp->nwChild == NULL) && (temp->neChild == NULL) && (temp->seChild == NULL) && (temp->swChild == NULL)
		&& (othertemp->nwChild) && (othertemp->neChild) && (othertemp->swChild) && (othertemp->seChild))
	{
		//need to build new QuadtreeNodes here

		temp->element.alpha = othertemp->element.alpha;
		temp->element.red = othertemp->element.red;
		temp->element.green = othertemp->element.green;
		temp->element.blue = othertemp->element.blue;

		temp->nwChild = new QuadtreeNode();

		temp->nwChild->x = temp->x - res / 4.0;
		temp->nwChild->y = temp->y - res / 4.0;

		temp->neChild = new QuadtreeNode();
		temp->neChild->x = temp->x + res / 4.0;
		temp->neChild->y = temp->y - res / 4.0;

		temp->seChild = new QuadtreeNode();
		temp->seChild->x = temp->x + res / 4.0;
		temp->seChild->y = temp->y + res / 4.0;

		temp->swChild = new QuadtreeNode();
		temp->swChild->x = temp->x - res / 4.0;
		temp->swChild->y = temp->y + res / 4.0;
		
		Rotatehelper1(temp->neChild, othertemp->nwChild, res / 2);
		Rotatehelper1(temp->seChild, othertemp->neChild, res / 2);
		Rotatehelper1(temp->swChild, othertemp->seChild, res / 2);
		Rotatehelper1(temp->nwChild, othertemp->swChild, res / 2);
		return;
	}
	else
	{
		Rotatehelper1(temp->neChild, othertemp->nwChild, res / 2);
		Rotatehelper1(temp->seChild, othertemp->neChild, res / 2);
		Rotatehelper1(temp->swChild, othertemp->seChild, res / 2);
		Rotatehelper1(temp->nwChild, othertemp->swChild, res / 2);
		temp->element.alpha = othertemp->element.alpha;
		temp->element.red = othertemp->element.red;
		temp->element.green = othertemp->element.green;
		temp->element.blue = othertemp->element.blue;
		return;
	}
}

void Quadtree::Rotatehelper(QuadtreeNode* temp, Quadtree const& refer, int res) {

	if (res == 1) {
		temp->element.alpha = refer.getPixel(temp->y - 1, refer.res + 1 - temp->x - 1).alpha;
		temp->element.red = refer.getPixel(temp->y - 1, refer.res + 1 - temp->x - 1).red;
		temp->element.green = refer.getPixel(temp->y - 1, refer.res + 1 - temp->x - 1).green;
		temp->element.blue = refer.getPixel(temp->y - 1, refer.res + 1 - temp->x - 1).blue;
		return;
	}
	else
	{
		Rotatehelper(temp->nwChild, refer,res/2);
		Rotatehelper(temp->swChild, refer,res/2);
		Rotatehelper(temp->seChild, refer,res/2);
		Rotatehelper(temp->neChild, refer,res/2);
		temp->element.alpha = refer.getPixel(temp->y - 1, refer.res + 1 - temp->x - 1).alpha;
		temp->element.red = refer.getPixel(temp->y - 1, refer.res + 1 - temp->x - 1).red;
		temp->element.green = refer.getPixel(temp->y - 1, refer.res + 1 - temp->x - 1).green;
		temp->element.blue = refer.getPixel(temp->y - 1, refer.res + 1 - temp->x - 1).blue;
		return;
	}
}

void Quadtree::clockwiseRotate()
{	

	Quadtree BeforeRotate;
	BeforeRotate.root = new QuadtreeNode();
	BeforeRotate.root->x = BeforeRotate.root->y = res / 2;
	BeforeRotate.res = res;

	BeforeRotate.copytree(root, BeforeRotate.root,res);
	
	Rotatehelper1(root, BeforeRotate.root,res);

}


// decompress (public interface)
//   - parameters: none
//   - return value: a PNG object representing this quadtree's underlying
//        bitmap
//   - constructs and returns this quadtree's underlying bitmap

void Quadtree::decompresshelper(PNG* result, QuadtreeNode * temp, int res) const{
	if (res == 1) {
		//cout << "reached bottom level" << endl;
		//cout << "temp->x: " << temp->x << endl;
		//cout << "temp->y: " << temp->y << endl;

		(*result)(temp->x - 1, temp->y - 1)->alpha = temp->element.alpha;
		(*result)(temp->x - 1, temp->y - 1)->red = temp->element.red;
		(*result)(temp->x - 1, temp->y - 1)->green = temp->element.green;
		(*result)(temp->x - 1, temp->y - 1)->blue = temp->element.blue;

		return;
	}

	else if ((temp->nwChild == NULL) && (temp->neChild == NULL) && (temp->seChild == NULL) && (temp->swChild == NULL))
	{
		//cout << "reached pruned tails" << endl;
		//cout << "temp->x: " << temp->x << endl;
		//cout << "temp->y: " << temp->y << endl;

		for (int i = -res/2; i < res/2; i++) {
			for (int j = -res/2; j < res/2; j++) {
				(*result)(temp->x- 0.5 + i , temp->y - 0.5 + j )->alpha = temp->element.alpha;
				(*result)(temp->x - 0.5 + i, temp->y - 0.5 + j)->red = temp->element.red;
				(*result)(temp->x - 0.5 + i, temp->y - 0.5 + j)->green = temp->element.green;
				(*result)(temp->x - 0.5 + i, temp->y - 0.5 + j)->blue = temp->element.blue;
			}
		}
		(*result)(temp->x-1, temp->y-1)->alpha = temp->element.alpha;
		(*result)(temp->x-1, temp->y-1)->red = temp->element.red;
		(*result)(temp->x-1, temp->y-1)->green = temp->element.green;
		(*result)(temp->x-1, temp->y-1)->blue = temp->element.blue;

		return;
	}
	else
	{
		//cout << "not bottom" << endl;
		//cout << "temp->x: " << temp->x << endl;
		//cout << "temp->y: " << temp->y << endl;
		decompresshelper(result, temp->nwChild,res/2);
		decompresshelper(result, temp->neChild,res/2);
		decompresshelper(result, temp->seChild,res/2);
		decompresshelper(result, temp->swChild,res/2);
		return;
	}

}
PNG Quadtree::decompress() const
{
	PNG result1(res,res);
	decompresshelper(&result1, root,res);

	return result1;
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
void Quadtree::prunehelper2(QuadtreeNode* temp, int tolerance) {
	int flag = 0;
	if ((temp->neChild == NULL) && (temp->nwChild == NULL) && (temp->seChild == NULL) && (temp->swChild == NULL)) {
		return;
	}
	prunehelper(temp, temp, tolerance, flag);

	if (flag == 0)
	{
		deletetree(temp->neChild);
		temp->neChild = NULL;
		deletetree(temp->nwChild);
		temp->nwChild = NULL;
		deletetree(temp->seChild);
		temp->seChild = NULL;
		deletetree(temp->swChild);
		temp->swChild = NULL;

		return;
	}
	prunehelper2(temp->nwChild, tolerance);
	prunehelper2(temp->neChild, tolerance);
	prunehelper2(temp->swChild, tolerance);
	prunehelper2(temp->seChild, tolerance);
	return;

}

void Quadtree::prunehelper(QuadtreeNode* temp, QuadtreeNode* temp2, int tolerance, int &flag) {
	//if we reach the bottom, return to the upper level
	if ((temp->neChild == NULL) && (temp->nwChild == NULL) && (temp->seChild == NULL) && (temp->swChild == NULL))
	{

		if( ((int(temp->element.blue - temp2->element.blue)) *(int(temp->element.blue - temp2->element.blue))
			+ (int(temp->element.red - temp2->element.red)) * (int(temp->element.red - temp2->element.red)) 
			+ (int(temp->element.green - temp2->element.green))*(int(temp->element.green - temp2->element.green))) > tolerance) {
			flag++;
		}
		return;
	}

	prunehelper(temp->nwChild, temp2, tolerance, flag);
	prunehelper(temp->neChild, temp2, tolerance, flag);
	prunehelper(temp->seChild, temp2, tolerance, flag);
	prunehelper(temp->swChild, temp2, tolerance, flag);
		
		return;
}

void Quadtree::prune(int tolerance)
{
	prunehelper2(root, tolerance);
	//cout << "prune is done" << endl;
}

// pruneSize (public interface)
//   - parameters: int tolerance - an integer representing the maximum
//                    "distance" which we will permit between a node's color
//                    (i.e. the average of its descendant leaves' colors)
//                    and the color of each of that node's descendant leaves
//   - returns the number of leaves which this quadtree would contain if it
//        was pruned using the given tolerance; does not actually modify the
//        tree
void Quadtree::calculateLeaf(QuadtreeNode* temp, int &result) const {
	if ((temp->neChild == NULL) && (temp->nwChild == NULL) && (temp->seChild == NULL) && (temp->swChild == NULL))
	{
		result++;
	}
	else
	{
		calculateLeaf(temp->nwChild, result);
		calculateLeaf(temp->neChild, result);
		calculateLeaf(temp->swChild, result);
		calculateLeaf(temp->seChild, result);
	}

}


int Quadtree::pruneSize(int tolerance) const
{
	//step1. make a copy of *this,
	//step2. prune *this
	//step3. calculate the number of leaves
	//step4. copy back to this

	Quadtree BeforePrune;
	BeforePrune.root = new QuadtreeNode();
	BeforePrune.root->x = BeforePrune.root->y = res / 2;
	BeforePrune.res = res;

	BeforePrune.copytree(root, BeforePrune.root, res);
	BeforePrune.prune(tolerance);

	int result = 0;
	calculateLeaf(BeforePrune.root,result);

	return result;
}

int Quadtree::pruneSizeonthis(int tolerance) {
	prune(tolerance);
	int result = 0;
	calculateLeaf(root, result);
	//cout << "tolerance: "<<tolerance<<" leaf numbers:"<<result << endl;
	return result;
}

// idealPrune (public interface)
//   - parameters: int numLeaves - an integer representing the number of
//                    leaves we wish the quadtree to have, after pruning
//   - returns the minimum tolerance such that pruning with that tolerance
//        would yield a tree with 
//	at most numLeaves leaves

int Quadtree::findIdeal(int &result, int numLeaves, int x, int& num) {
	
	int Leaves = 0;
	for (int i=1; i < res*res*x; i *= x) {
		num++;
		Leaves = pruneSize(result+i);
		if (i == 1 && Leaves == numLeaves)
		{
			return result;
		}
		if (Leaves <= numLeaves) {
			int laststep = result+i / x;
			if (x > 2)
				x--;
			result = findIdeal(laststep, numLeaves,x,num);
			return result;
		}
		
	}
	return result;
}

int Quadtree::findIdeal2(int min, int max, int numLeaves, int &result) {

	int minLeaves,maxLeaves,mintolerance=min, maxtolerance=max;
	minLeaves = pruneSize(min);
	maxLeaves = pruneSize(max);

	//cout << "min= " << min << " max= " << max << " minLeaves=" << minLeaves << " maxLeaves=" << maxLeaves << endl;
	//base case: 
	if ((min == max-1)&&(minLeaves>numLeaves)) {
		result = max;
		return max;
	}
	if ((min == max - 1) && (minLeaves=numLeaves)) {
		result = min;
		return min;
	}
	//
	if (minLeaves == numLeaves&&maxLeaves<=numLeaves)
	{
		int temp = mintolerance;
		mintolerance -= (maxtolerance - mintolerance);
		maxtolerance = temp;
		findIdeal2(mintolerance, maxtolerance, numLeaves, result);

	}
	else if (minLeaves >= numLeaves&&maxLeaves <= numLeaves)
	{
		mintolerance = (mintolerance + maxtolerance) / 2;
		findIdeal2(mintolerance, maxtolerance, numLeaves,result);
	}
	else {
		int temp = mintolerance;
		mintolerance -= (maxtolerance - mintolerance);
		maxtolerance = temp;
		findIdeal2(mintolerance, maxtolerance, numLeaves,result);
	}
	return result;
}


int Quadtree::idealPrune(int numLeaves) const
{
	//The idea is do it with log time instead of linear time
	//My first thought it to do it by adding an exponential item to the iterator, like num=x+1^n; then after I look on the piazza I tried to use the binary search
	//it turns out that by the previous way, it takes around 50 prunesize operation to find the right answer. If use binary search we need around 20 steps

	Quadtree BeforePrune;
	BeforePrune.root = new QuadtreeNode();
	BeforePrune.root->x = BeforePrune.root->y = res / 2;
	BeforePrune.res = res;
	BeforePrune.copytree(root, BeforePrune.root, res);

	int result = 0, num=0;
	//return (BeforePrune.findIdeal(result, numLeaves,9,num)+1);
	return (BeforePrune.findIdeal2(1, res*res, numLeaves,result));

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

