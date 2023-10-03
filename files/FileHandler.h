#include <string>
#include <filesystem>
#include <vector>

using std::string;
using std::vector;

class FileHandler {
public:
	static void getFileTree(string& baseDir);
};

// template <typename T>
// class AdjacencyList {
// public:
// 	AdjacencyList<T>() {

// 	}
// private:
// 	AdjacencyNode<T> head*;
// };


// template <typename T>
// class AdjacencyNode {
// public:
// 	AdjacencyNode() {

// 	}
// private:
// 	T data;
// 	vector<AdjacencyNode<T>>* children;
// };