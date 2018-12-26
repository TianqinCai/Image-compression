/**
 * @file main.cpp
 * Contains code to test your Quadtree implementation.
 */

#include <iostream>
#include "png.h"
#include "quadtree.h"
//#include "quadtree_given.cpp"

using std::cout;
using std::endl;

int main()
{

    PNG imgIn, imgOut;
    imgIn.readFromFile("in.png");
    // test constructor, decompress
	Quadtree halfTree(imgIn,128);
	//cout << "building tree" << endl;
	//halfTree.printTree(cout);
    imgOut = halfTree.decompress();
    imgOut.writeToFile("outHalf.png");

    // now for the real tests
    Quadtree fullTree;
    fullTree.buildTree(imgIn, 256);

    //// you may want to experiment with different commands in this section

    //// test pruneSize and idealPrune (slow in valgrind, so you may want to
    //// comment these out when doing most of your testing for memory leaks)
    cout << "fullTree.pruneSize(0) = "      << fullTree.pruneSize(0) << endl;
    cout << "fullTree.pruneSize(100) = "    << fullTree.pruneSize(100) << endl;
    cout << "fullTree.pruneSize(1000) = "   << fullTree.pruneSize(1000) << endl;
    cout << "fullTree.pruneSize(100000) = " << fullTree.pruneSize(100000) << endl;

    cout << "fullTree.idealPrune(1000) = "  << fullTree.idealPrune(1000) << endl;
    cout << "fullTree.idealPrune(10000) = " << fullTree.idealPrune(10000) << endl;

	cout << "Test some creation/deletion functions" << endl;
    Quadtree fullTree2;
    fullTree2 = fullTree;
	//cout << "doing decompress for fulltree2" << endl;
    imgOut = fullTree2.decompress();
    imgOut.writeToFile("outCopy.png");

    //// test clockwiseRotate
	//cout << "doing rotation" << endl;
    fullTree.clockwiseRotate();
	//cout << "full tree constructed" << endl;
    imgOut = fullTree.decompress();
	//cout << "rotated complete" << endl;
    imgOut.writeToFile("outRotated.png");

    // test prune
    fullTree = fullTree2;
	//cout << "doing prune" << endl;
    fullTree.prune(1000);
	//fullTree.printTree(cout);
    imgOut = fullTree.decompress();
	
	//cout << "prune is done" << endl;
    imgOut.writeToFile("outPruned.png");

    //// test several functions in succession
	//cout << "testing fullTree3" << endl;
    Quadtree fullTree3(fullTree2);
	//cout << "used copy constructor" << endl;
    fullTree3.clockwiseRotate();
	//cout << "fullTree3 rotated" << endl;
    fullTree3.prune(10000);
	//cout << "fullTree3 pruned" << endl;
    fullTree3.clockwiseRotate();
	//cout << "fullTree3 rotated" << endl;
    fullTree3.clockwiseRotate();
	//cout << "fullTree3 rotated" << endl;
    fullTree3.clockwiseRotate();
	//cout << "fullTree3 rotated" << endl;
    imgOut = fullTree3.decompress();
	//cout << "fullTree3 decompressed" << endl;
    imgOut.writeToFile("outEtc.png");

    //// ensure that printTree still works
    Quadtree tinyTree(imgIn, 32);
    cout << "Printing tinyTree:\n";
    tinyTree.prune(100);
    tinyTree.printTree();

    return 0;
}
