#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include <iostream>

#include "quadtree.h"

string printTree(const Quadtree & tree)
{
	stringstream output;
	tree.printTree(output);
	return output.str();
}

string readFile(const string & filename)
{
	std::ifstream file;
	file.open(filename.c_str());
	if (!file.good())
		return "";

	std::stringbuf linebuf;
	file.get(linebuf, '\0');
	linebuf.pubsync();
	return linebuf.str();
}

#define ASSERT_TREE_EQUALS(tree, filename)     \
	do {                                       \
		string output = printTree(tree);       \
		REQUIRE(output == readFile(filename));  \
	} while (0)

		
TEST_CASE("Default_constructor", "[default_constructor]") {
	Quadtree first;
	ASSERT_TREE_EQUALS(first, "empty.out");
}

TEST_CASE("Constructor_1", "[Constructor]") {
	PNG small("small.png");
	Quadtree first(small, 16);
	ASSERT_TREE_EQUALS(first, "small_16.out");
}

TEST_CASE("Constructor_2", "[Constructor]") {
	PNG small("small.png");
	Quadtree first(small, 8);
	ASSERT_TREE_EQUALS(first, "small_8.out");
}

TEST_CASE("BuildTree_1", "[BuildTree]") {
	PNG small("small.png");
	Quadtree first;
	first.buildTree(small, 16);
	ASSERT_TREE_EQUALS(first, "small_16.out");
}

TEST_CASE("BuildTree_2", "[BuildTree]") {
	PNG small("small.png");
	Quadtree first;
	first.buildTree(small, 8);
	ASSERT_TREE_EQUALS(first, "small_8.out");
}

TEST_CASE("BuildTree_3", "[BuildTree]") {
	PNG small("small.png");
	PNG monochrome("monochrome.png");
	Quadtree first(monochrome, 16);
	first.buildTree(small, 8);
	ASSERT_TREE_EQUALS(first, "small_8.out");
}

TEST_CASE("CopyConstructor_1", "[CopyConstructor]") {
	Quadtree first;
	Quadtree second(first);
	REQUIRE(first == second);
}

TEST_CASE("CopyConstructor_2", "[CopyConstructor]") {
	PNG pixel("pixel.png");
	Quadtree first(pixel, 1);
	Quadtree second(first);
	REQUIRE(first == second);
}

TEST_CASE("CopyConstructor_3", "[CopyConstructor]") {
	PNG large("large.png");
	Quadtree first(large, 16);
	Quadtree second(first);
	REQUIRE(first == second);
}

TEST_CASE("CopyConstructor_4", "[CopyConstructor]") {
	PNG large("large.png");
	PNG monochrome("monochrome.png");
	Quadtree * first = new Quadtree(large, 16);
	Quadtree * second = new Quadtree(*first);
	REQUIRE(*first == *second);
	delete first;
	first = new Quadtree(monochrome, 16);
	REQUIRE(!(*first == *second));
	delete first;
	delete second;
}

TEST_CASE("AssignOperator_1", "[AssignOperator]") {
	Quadtree first;
	Quadtree second;
	second = first;
	REQUIRE(first == second);
}

TEST_CASE("AssignOperator_2", "[AssignOperator]") {
	PNG pixel("pixel.png");
	PNG monochrome("monochrome.png");
	Quadtree first(pixel, 1);
	Quadtree second;
	second = first;
	REQUIRE(first == second);
}

TEST_CASE("AssignOperator_3", "[AssignOperator]") {
	PNG large("large.png");
	PNG monochrome("monochrome.png");
	Quadtree first(large, 16);
	Quadtree second(monochrome, 8);
	second = first;
	REQUIRE(first == second);
}

TEST_CASE("AssignOperator_4", "[AssignOperator]") {
	PNG large("large.png");
	PNG small("small.png");
	PNG monochrome("monochrome.png");
	Quadtree * first = new Quadtree(large, 16);
	Quadtree * second = new Quadtree(small, 8);
	*second = *first;
	REQUIRE(*first == *second);
	delete first;
	first = new Quadtree(monochrome, 16);
	REQUIRE(!(*first == *second));
	delete first;
	delete second;
}

TEST_CASE("GetPixel_1", "[GetPixel]") {
	PNG pixel("pixel.png");
	Quadtree first(pixel, 1);
	RGBAPixel soln;
	soln.red   = 0;
	soln.green = 0;
	soln.blue  = 128;
	REQUIRE(soln == first.getPixel(0, 0));
}

TEST_CASE("GetPixel_2", "[GetPixel]") {
	PNG tiny("tiny.png");
	Quadtree first(tiny, 2);
	RGBAPixel soln;
	soln.red   = 0;
	soln.green = 0;
	soln.blue  = 255;
	REQUIRE(soln == first.getPixel(0, 1));
}

TEST_CASE("GetPixel_3", "[GetPixel]") {
	PNG small("small.png");
	Quadtree first(small, 16);
	RGBAPixel soln;
	soln.red   = 126;
	soln.green = 70;
	soln.blue  = 32;
	REQUIRE(soln == first.getPixel(9, 9));
}

TEST_CASE("GetPixel_4", "[GetPixel]") {
	PNG large("large.png");
	Quadtree first(large, 512);
	RGBAPixel soln;
	soln.red   = 89;
	soln.green = 127;
	soln.blue  = 18;
	REQUIRE(soln == first.getPixel(302, 178));
}

TEST_CASE("GetPixel_Time", "[GetPixel][Time]") {
	PNG large("large.png");
	Quadtree first(large, 512);
	// Assuming each should take time < 2s, this should pass before 20s timeout
	for(int i = 1; i < 1024; i*=2){
		RGBAPixel result = first.getPixel(512/i, 512/i);
	}
	// Check if actually works and pass if not timed out
	RGBAPixel soln;
	soln.red   = 89;
	soln.green = 127;
	soln.blue  = 18;
	REQUIRE(soln == first.getPixel(302, 178));
}

TEST_CASE("Decompress_Unpruned_1", "[Decompress][Unpruned]") {
	PNG in("pixel.png");
	PNG input(in); // In case their decompress modifies the input (it shouldn't, but hacks...)
	Quadtree first(input, 1);
	PNG out = first.decompress();
	REQUIRE(out == input);
}

TEST_CASE("Decompress_Unpruned_2", "[Decompress][Unpruned]") {
	PNG in("tiny.png");
	PNG input(in); // In case their decompress modifies the input (it shouldn't, but hacks...)
	Quadtree first(input, 2);
	PNG out = first.decompress();
	REQUIRE(out == input);
}

TEST_CASE("Decompress_Unpruned_3", "[Decompress][Unpruned]") {
	PNG in("small.png");
	PNG input(in); // In case their decompress modifies the input (it shouldn't, but hacks...)
	Quadtree first(input, 16);
	PNG out = first.decompress();
	REQUIRE(out == input);
}

TEST_CASE("Decompress_Unpruned_4", "[Decompress][Unpruned]") {
	PNG in("large.png");
	PNG input(in); // In case their decompress modifies the input (it shouldn't, but hacks...)
	Quadtree first(input, 512);
	PNG out = first.decompress();
	REQUIRE(out == input);
}

TEST_CASE("Decompress_Unpruned_Time", "[Decompress][Unpruned][Time]") {
	PNG in("large.png");
	PNG input(in); // In case their decompress modifies the input (it shouldn't, but hacks...)
	// Assuming each should take time < 2s, this should pass before 20s timeout
	for(int i = 1; i < 1024; i*=2){
		Quadtree first(input, 512/i);
		PNG out = first.decompress();
	}
	// Check that actually works and pass if have not timed out
	Quadtree first(input, 512);
	PNG out = first.decompress();
	REQUIRE(out == input);
}

TEST_CASE("ClockwiseRotate_1", "[ClockwiseRotate]") {
	Quadtree first;
	first.clockwiseRotate();
	ASSERT_TREE_EQUALS(first, "empty.out");
}

TEST_CASE("ClockwiseRotate_2", "[ClockwiseRotate]") {
	PNG tiny("tiny.png");
	Quadtree first(tiny, 2);
	first.clockwiseRotate();
	PNG rVal = first.decompress();
	PNG soln("tinyRotated.png");
	REQUIRE(rVal == soln);
}

TEST_CASE("ClockwiseRotate_3", "[ClockwiseRotate]") {
	PNG large("large.png");
	Quadtree first(large, 512);
	first.clockwiseRotate();
	PNG rVal = first.decompress();
	PNG soln("largeRotated.png");
	REQUIRE(rVal == soln);
}

TEST_CASE("ClockwiseRotate_4", "[ClockwiseRotate]") {
	PNG large("large.png");
	Quadtree first(large, 512);
	first.clockwiseRotate();
	first.clockwiseRotate();
	PNG rVal = first.decompress();
	PNG soln("largeRotatedTwice.png");
	REQUIRE(rVal == soln);
}

TEST_CASE("Valgrind_ClockwiseRotate", "[Valgrind]")
{
	PNG tiny("tiny.png");
	Quadtree * first = new Quadtree(tiny, 2);
	first->clockwiseRotate();
	PNG rVal = first->decompress();
	PNG soln("tinyRotated.png");
	REQUIRE(rVal == soln);
	delete first;
}

TEST_CASE("Valgrind_Big3", "[Valgrind]")
{
	PNG small("small.png");
	Quadtree * first = new Quadtree(small, 16);
	Quadtree * second = new Quadtree(*first);
	REQUIRE(*first == *second);
	*second = *first;
	REQUIRE(*first == *second);
	delete first;
	delete second;
}

