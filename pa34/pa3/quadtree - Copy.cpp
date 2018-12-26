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
	//cout << "calling buildtree" << endl;
	buildTree(source, setresolution);
}

// Quadtree
//   - parameters: Quadtree const & other - reference to a const Quadtree
//                    object, which the current Quadtree will be a copy of
//   - copy constructor for the Quadtree class
Quadtree::Quadtree(Quadtree const& other) 
{
	//delete root;

	//QuadtreeNode* temp = root;
	//QuadtreeNode* othertemp = other.root;
	//root = NULL;
	//delete temp;

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
		//cout << "reached bottom" << endl;
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
		//cout << "not bottom" << endl;
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
		//cout << "destructing" << endl;
		deletetree(root);
		//QuadtreeNode* del = root;
		//root = NULL;
		//delete del;
		//cout << (root == NULL) << endl;
	}
	
	//cout << (root != NULL) << endl;

}

void Quadtree::deletetree(QuadtreeNode * temp)
{
	if ((temp->neChild == NULL) && (temp->nwChild == NULL) && (temp->seChild == NULL) && (temp->swChild == NULL))
	{
		//cout << "deleting bottom level" << endl;
		/*QuadtreeNode* del = temp;
		temp = NULL;
		delete del;*/
		delete temp;
		//cout << (temp == NULL) << endl;
		return;
	}
	else
	{
		//cout << "deleting upper level " <<temp->x <<" "<<temp->y<< endl;
		//cout << (temp->nwChild == NULL) << " " << (temp->neChild == NULL) << (temp->swChild == NULL) << (temp->seChild == NULL) << endl;
		if (temp->neChild)
			deletetree(temp->neChild);
		if (temp->nwChild)
			deletetree(temp->nwChild);
		if (temp->seChild)
			deletetree(temp->seChild);
		if (temp->swChild)
			deletetree(temp->swChild);

		QuadtreeNode* del = temp;
		temp = NULL;
		delete del;
		//delete temp;
		/*QuadtreeNode* del1 = temp;
		temp = NULL;
		delete del1;*/
		//delete temp;
	}
}

// operator=
//   - parameters: Quadtree const & other - reference to a const Quadtree
//                    object, which the current Quadtree will be a copy of
//   - return value: a const reference to the current Quadtree
//   - assignment operator for the Quadtree class
Quadtree const& Quadtree::operator=(Quadtree const& other)
{
	//cout << "using operator =" << endl;
	//QuadtreeNode* temp = root;
	//QuadtreeNode* othertemp = other.root;
	//root = NULL;
	//delete temp;
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

//void Quadtree::unvisited(QuadtreeNode* temp)
//{
//	if ((temp->neChild == NULL) && (temp->nwChild == NULL) && (temp->seChild == NULL) && (temp->swChild == NULL))
//	{
//		temp->visited = false;
//		return;
//	}
//	else
//	{
//		if (temp->nwChild->visited == true)
//		{
//			unvisited(temp->nwChild);
//		}
//		if (temp->neChild->visited == true) {
//			unvisited(temp->neChild);
//		}
//		if (temp->swChild->visited == true) {
//			unvisited(temp->swChild);
//		}
//		if (temp->seChild->visited == true) {
//			unvisited(temp->seChild);
//		}
//		temp->visited = false;
//		return;
//	}
//}

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
		//cout << "entering nw" << endl;
		 return getPixelhelper(temp->nwChild, a, b);
	}
	else if (a<=temp->x&&b>=temp->y)
	{
		//cout << "entering sw" << endl;
		return getPixelhelper(temp->swChild, a, b);
	}
	else if (a >= temp->x&&b <= temp->y)
	{
		//cout << "entering ne" << endl;
		return getPixelhelper(temp->neChild, a, b);
	}
	else if (a >= temp->x&&b >= temp->y) {
		//cout << "entering se" << endl;
		return getPixelhelper(temp->seChild, a, b);
	}
	//cout << "coordinate: "<< a <<" "<<b  <<" strangely not found " << endl;
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
	
	//if (/*(res == 1)|| */
	//	((othertemp->nwChild == NULL)/* && (othertemp->neChild == NULL) && (othertemp->seChild == NULL) && (othertemp->swChild == NULL)*/ &&
	//	(temp->nwChild == NULL)/* && (temp->neChild == NULL) && (temp->seChild == NULL) && (temp->swChild == NULL))*/))

	if((othertemp->nwChild == NULL) &&(temp->nwChild == NULL))
	 {
		cout << "reached bottom, both temp and othertemp don't have children" << endl;
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
		
		cout << "othertemp has no children but temp has children" << endl;
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
		//cout << (temp == NULL) << endl;
		//cout << (temp->neChild == NULL) << endl;
		cout << "coordinates: "<<temp->x << " " << temp->y << endl;
		//cout << "temp's children have been deleted" << endl;
		return;
	}
	else if ((temp->nwChild == NULL) && (temp->neChild == NULL) && (temp->seChild == NULL) && (temp->swChild == NULL)
		&& (othertemp->nwChild) && (othertemp->neChild) && (othertemp->swChild) && (othertemp->seChild))
	{
		//need to build new QuadtreeNodes here

		cout << "temp has no children but othertemp still have children" << endl;
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
		
		cout << "coordinates: "<<temp->x << " " << temp->y << endl;
		//cout << "temp has been assigned color" << endl;
		Rotatehelper1(temp->neChild, othertemp->nwChild, res / 2);
		Rotatehelper1(temp->seChild, othertemp->neChild, res / 2);
		Rotatehelper1(temp->swChild, othertemp->seChild, res / 2);
		Rotatehelper1(temp->nwChild, othertemp->swChild, res / 2);
		return;
	}
	else
	{
		cout << "didn't reach bottom, both temp and othertemp have children" << endl;
		Rotatehelper1(temp->neChild, othertemp->nwChild, res / 2);
		Rotatehelper1(temp->seChild, othertemp->neChild, res / 2);
		Rotatehelper1(temp->swChild, othertemp->seChild, res / 2);
		Rotatehelper1(temp->nwChild, othertemp->swChild, res / 2);
		//cout << "copying color" << endl;
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

//void Quadtree:: NWRotatehelper(QuadtreeNode* temp) {
//
//	if ((temp->neChild == NULL) && (temp->nwChild == NULL) && (temp->seChild == NULL) && (temp->swChild == NULL))
//	{
//		RGBAPixel temprgb = (*this).getPixel(temp->x, temp->y);
//		temp->element.alpha = (*this).getPixel(temp->y, (*this).res + 1 - temp->x).alpha;
//		temp->element.red = (*this).getPixel(temp->y, (*this).res + 1 - temp->x).red;
//		temp->element.green = (*this).getPixel(temp->y, (*this).res + 1 - temp->x).green;
//		temp->element.blue = (*this).getPixel(temp->y, (*this).res + 1 - temp->x).blue;
//		return;
//	}
//	else
//	{
//		NWRotatehelper(temp->nwChild);
//		NWRotatehelper(temp->swChild);
//		NWRotatehelper(temp->seChild);
//		NWRotatehelper(temp->neChild);
//		temp->element.alpha = (*this).getPixel(temp->y, (*this).res + 1 - temp->x).alpha;
//		temp->element.red = (*this).getPixel(temp->y, (*this).res + 1 - temp->x).red;
//		temp->element.green = (*this).getPixel(temp->y, (*this).res + 1 - temp->x).green;
//		temp->element.blue = (*this).getPixel(temp->y, (*this).res + 1 - temp->x).blue;
//		return;
//	}
//}

void Quadtree::clockwiseRotate()
{	
	cout << "rotating" << endl;
	//cout << (root == NULL) << endl;
	Quadtree BeforeRotate;
	BeforeRotate.root = new QuadtreeNode();
	BeforeRotate.root->x = BeforeRotate.root->y = res / 2;
	BeforeRotate.res = res;
	cout << "created a new Quadtree" << endl;
	cout << "res = " << res << endl;
	BeforeRotate.copytree(root, BeforeRotate.root,res);
	cout << "made a copy" << endl;
	
	Rotatehelper1(root, BeforeRotate.root,res);
	//Rotatehelper(root, BeforeRotate, res);
	cout << "rotated" << endl;
	//BeforeRotate.~Quadtree();

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
	cout << "start decompressing" << endl;
	//PNG* result = new PNG(res, res);
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
	//cout << "flag= "<<flag << endl;
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
		//QuadtreeNode* copy = new QuadtreeNode();
		//copy->element.alpha = temp->element.alpha;
		//copy->element.red = temp->element.red;
		//copy->element.blue = temp->element.blue;
		//copy->element.green = temp->element.green;
		//copy->x = temp->x;
		//copy->y = temp->y;
		//deletetree(temp);

		//temp = new QuadtreeNode();

		//
		//temp->element.alpha = copy->element.alpha;
		//temp->element.red = copy->element.red;
		//temp->element.blue = copy->element.blue;
		//temp->element.green = copy->element.green;
		//temp->neChild = NULL;
		//temp->nwChild = NULL;
		//temp->seChild = NULL;
		//temp->swChild = NULL;
		//temp->x = copy->x;
		//temp->y = copy->y;
		//QuadtreeNode* del = copy;
		//copy = NULL;
		//delete del;

		//cout << "bottom deleted " << temp->x << " "<<temp->y << endl;
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
			//cout << "got beyond tolerance" << endl;
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


//int findtolerance(int curr, int numLeaves, int result) {
//	for (int i = curr; i <= res*res; i += 2) {
//		if (result == numLeaves)
//		{
//			return i;
//		}
//		//result = BeforePrune.pruneSizeonthis(i);	
//		result = pruneSizeonthis(i);
//		if (result < numLeaves)
//		{
//			findtolerance(curr / 2; numLeaves; result);
//		}
//		return 
//		//storage = i;
//		//cout << "i=" << i << " result=" << result<< endl;
//		//system.
//	}
//}

int Quadtree::idealPrune(int numLeaves) const
{
	//The slow way means doing it recursively
	//The fast way means doing it by dynamic programming
	//so I need to use dynamic programming, record each steps result and reuse them
	//which means do it recursively

	Quadtree BeforePrune;
	BeforePrune.root = new QuadtreeNode();
	BeforePrune.root->x = BeforePrune.root->y = res / 2;
	BeforePrune.res = res;

	BeforePrune.copytree(root, BeforePrune.root, res);

	Quadtree BeforePrune2;
	BeforePrune2.root = new QuadtreeNode();
	BeforePrune2.root->x = BeforePrune2.root->y = res / 2;
	BeforePrune2.res = res;

	BeforePrune2.copytree(root, BeforePrune2.root, res);
	
	//Quadtree BeforePrune3;
	//BeforePrune3.root = new QuadtreeNode();
	//BeforePrune3.root->x = BeforePrune3.root->y = res / 2;
	//BeforePrune3.res = res;

	//BeforePrune3.copytree(root, BeforePrune3.root, res);

	int storage,result=0,record=0;

	for (int i = 1; i < res*res; i*=2) {
		
		//result = BeforePrune.pruneSizeonthis(i);	
		result = BeforePrune.pruneSizeonthis(i);
		if (result < numLeaves)
		{
			record = i;
			break;
		}
		storage = i;
		//cout << "i=" << i << " result=" << result<< endl;
		//system.
	}
	for (int j = record / 2; j <= record; j++) {
		//storage = result;
		//result = BeforePrune2.pruneSizeonthis(j);
		result = BeforePrune2.pruneSizeonthis(j);
		if (result < numLeaves)
		{
			break;
		}
		storage = j;
		//cout << "j=" << j << " result=" << result << endl;
	}

	return storage;
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

