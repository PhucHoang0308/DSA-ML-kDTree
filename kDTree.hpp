#include "main.hpp"
#include "Dataset.hpp"
/* TODO: Please design your data structure carefully so that you can work with the given dataset
 *       in this assignment. The below structures are just some suggestions.
 */
struct kDTreeNode
{
    vector<int> data;
    kDTreeNode *left;
    kDTreeNode *right;
    kDTreeNode(vector<int> data, kDTreeNode *left = nullptr, kDTreeNode *right = nullptr)
    {
        this->data = data;
        this->left = left;
        this->right = right;
    }

    friend ostream &operator<<(ostream &os, const kDTreeNode &node)
    {
        os << "(";
        for (int i = 0; i < node.data.size(); i++)
        {
            os << node.data[i];
            if (i != node.data.size() - 1)
            {
                os << ", ";
            }
        }
        os << ")";
        return os;
    }
};

class kDTree
{
private:
    int k;
    kDTreeNode *root;

public:
    kDTree(int k = 2);
    ~kDTree(){};

    const kDTree &operator=(const kDTree &other);
    kDTreeNode *copyTree(kDTreeNode *node) const;//2
    void clearTree(kDTreeNode *node); // 1
    kDTree(const kDTree &other);

    void inorderTraversal() {
        bool first = true; 
        inordersupport(root, first);};
    void inordersupport(kDTreeNode *node,bool &isLastNode) const;

    void preorderTraversal() {
        bool first = true;
        preordersupport(root,first);};
    void preordersupport(kDTreeNode *node,bool &isLastNode) const;
    
    void postorderTraversal() {postordersupport(root,true);};
    void postordersupport(kDTreeNode *node,bool isLastNode) const;

    int height() {return heightsupport(root);};
    int heightsupport(kDTreeNode *node)const;

    int nodeCount() {return nodeCountsupport(root);};
    int nodeCountsupport(kDTreeNode *node)const;
      
    int leafCount() {return leafCountsupport(root);};
    int leafCountsupport(kDTreeNode *node)const;  

    void insert(const vector<int> &point){insertsupport(point, root,0);};
    void insertsupport(const vector<int> &point, kDTreeNode *&node, int k1);

    void remove(const vector<int> &point){removesupport(point,root,0);};
    void removesupport(const vector<int> &point,kDTreeNode *&node,int k1);
    kDTreeNode* nodefindremove(kDTreeNode *&node,int k1);

    bool search(const vector<int> &point){return searchsupport(point,root,0);};
    bool searchsupport(const vector<int> &point,kDTreeNode *&node,int k1);

    void buildTree(const vector<vector<int>> &pointList){buildTreesupport(pointList,root,0);};
    void buildTreesupport(const vector<vector<int>> &pointList,kDTreeNode *&node,int k1);
    void sort(vector<vector<int>> &pointList,int k1);

    void nearestNeighbour(const vector<int> &target, kDTreeNode *&best){
        nearestNeighboursupport(target, root, best, 0);
    };
    void nearestNeighboursupport(const vector<int>& target, kDTreeNode *node, kDTreeNode *&best, int k1);

    double distance(const vector<int> &a, const vector<int> &b) {
        double dist = 0.0; // Sử dụng kiểu double
        for (size_t i = 0; i < a.size(); ++i) {
            dist += pow(a[i] - b[i], 2); // Thêm vào dist với số thực
        }
        return sqrt(dist); // Trả về căn bậc hai của dist
    }

    void kNearestNeighbour(const vector<int> &target, int k, vector<kDTreeNode *> &bestList){
        kNearestNeighbousupport(target, root, k, bestList);
    };
    void kNearestNeighbousupport(const vector<int> &target,kDTreeNode *node, int k, vector<kDTreeNode *> &bestList);
    void SpecnearestNeighboursupport(const vector<int>& target, kDTreeNode *node, kDTreeNode *&best, int k1,vector<kDTreeNode *>bestprev );

    bool vectorEquals(const vector<int>& v1, const vector<int>& v2) {
    // Kiểm tra xem hai vector có cùng kích thước hay không
    if (v1.size() != v2.size()) {
        return false;
    }

    // Kiểm tra từng phần tử của hai vector
    for (size_t i = 0; i < v1.size(); ++i) {
        if (v1[i] != v2[i]) {
            return false;
        }
    }

    return true;
    }

    bool check(const vector<int>& target, const vector<kDTreeNode*>& target2) {
    // Duyệt qua từng phần tử trong danh sách target2
    for (const auto& node : target2) {
        // Kiểm tra xem vector data của node có bằng vector target hay không
        if (vectorEquals(node->data, target)) {
            return true;
        }
    }

    return false;
    }
};

class kNN
{
private:
    int k;  
    kDTree kdtree;
    vector<vector<int>> Xtree;
    vector<vector<int>> ytree;

public:
    int findpos(vector<vector<int>> x,vector<int> target);
    kNN(int k = 5) : k(k) {}
    void fit(Dataset &X_train, Dataset &y_train);
    Dataset predict(Dataset &X_test);
    double score(const Dataset &y_test, const Dataset &y_pred);
};

// Please add more or modify as needed
