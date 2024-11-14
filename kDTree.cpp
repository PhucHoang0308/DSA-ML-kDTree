#include "kDTree.hpp"

/* TODO: You can implement methods, functions that support your data structures here.
 * */
kDTree::kDTree(int k) : k(k), root(nullptr){}
kDTree::kDTree(const kDTree &other) : k(other.k), root(copyTree(other.root))
{
}

void kDTree::inordersupport(kDTreeNode *node, bool &isLastNode) const
{
    if (node == nullptr)
        return;

    // Duyệt qua nút con bên trái
    inordersupport(node->left,isLastNode);

    // In ra giá trị của node hiện tại
    if (!isLastNode)
    {
        cout << " ";
    }
    cout << "(";
    for (size_t i = 0; i < node->data.size(); ++i)
    {
        cout << node->data[i];
        if (i != node->data.size() - 1)
        {
            cout << ", ";
        }
    }
    cout << ")";

    // Đặt first thành false sau khi in node đầu tiên
    isLastNode = false;

    // Duyệt qua nút con bên phải
    inordersupport(node->right, isLastNode);
}

void kDTree::preordersupport(kDTreeNode *node, bool &isLastNode) const
{
    if (node==nullptr)return;
    if(!isLastNode) cout<<' ';
    
    cout << "(";
    size_t dataSize = node->data.size();
    for (size_t i = 0; i < dataSize; ++i)
    {
        cout << node->data[i]; // In ra giá trị của phần tử thứ i trong vector data

        if (i != dataSize - 1)
        {
            cout << ", "; // In dấu phẩy và khoảng trắng nếu không phải là phần tử cuối cùng
        }
    }
    cout << ")";
    isLastNode=false;
    // Duyệt qua nút con bên trái
    preordersupport(node->left, isLastNode);
    // Duyệt qua nút con bên phải
    preordersupport(node->right, isLastNode);
}

void kDTree::postordersupport(kDTreeNode *node, bool isLastNode) const
{
    if (node == nullptr)
        return;

    // Duyệt qua nút con bên trái
    postordersupport(node->left, false);

    // Duyệt qua nút con bên phải
    postordersupport(node->right, isLastNode && node->right == nullptr);

    // In ra giá trị của vector data của nút hiện tại theo định dạng (a1, a2, a3, ..., an)
    cout << "(";
    size_t dataSize = node->data.size();
    for (size_t i = 0; i < dataSize; ++i)
    {
        cout << node->data[i]; // In ra giá trị của phần tử thứ i trong vector data

        if (i != dataSize - 1)
        {
            cout << ", "; // In dấu phẩy và khoảng trắng nếu không phải là phần tử cuối cùng
        }
    }
    cout << ")";

    // Nếu node hiện tại là node cuối cùng (là node trên đường duyệt post-order cuối cùng), không in khoảng trắng
    if (!isLastNode)
    {
        cout << " ";
    }
}

int kDTree::heightsupport(kDTreeNode *node) const
{
    if (node==nullptr)return 0;
    int left=heightsupport(node->left);
    int right=heightsupport(node->right);

    if (left>right)return left+1;
    else return right+1;
}

int kDTree::nodeCountsupport(kDTreeNode *node) const
{
    if (node == nullptr)
    return 0;

    // Duyệt qua nút con bên trái
    int left=nodeCountsupport(node->left);

    // Duyệt qua nút con bên phải
    int right=nodeCountsupport(node->right);
    return 1+left+right;
}

int kDTree::leafCountsupport(kDTreeNode *node) const
{
    if (node == nullptr)
    return 0;
    // Duyệt qua nút con bên trái
    int left=leafCountsupport(node->left);

    // Duyệt qua nút con bên phải
    int right=leafCountsupport(node->right);
    if (node->left==nullptr && node->right==nullptr)
    return 1+left+right;
    else
    return left+right;
}

void kDTree::insertsupport(const vector<int> &point, kDTreeNode *&node, int k1)
{
    if(point.size()!=k)return;
    if(k1==k)k1=0;
    if (node==nullptr) {
        // Tạo nút mới
        node = new kDTreeNode(point);

        return;
    }
    if(point[k1]<node->data[k1]) insertsupport(point, node->left, k1+1);
    else insertsupport(point, node->right, k1+1);
}

void kDTree::removesupport(const vector<int> &point, kDTreeNode *&node, int k1) {
    if (node == nullptr) {
        // Node là nullptr, không có gì để xóa
        return;
    }

    if (point == node->data) {
        if (node->left == nullptr && node->right == nullptr) {
            delete node;
            node = nullptr;
        } else if (node->right != nullptr) {
            // Node có cây con bên phải
            kDTreeNode *minRight = nodefindremove(node->right, (k1 + 1) % k);
            node->data = minRight->data; // Thay thế giá trị của node bằng giá trị node nhỏ nhất bên phải
            removesupport(minRight->data, node->right, (k1 + 1) % k); // Đệ quy xóa node nhỏ nhất
        } else {
            // Node chỉ có cây con bên trái
            kDTreeNode *minLeft = nodefindremove(node->left, (k1 + 1) % k);
            node->data = minLeft->data; // Thay thế giá trị của node bằng giá trị node nhỏ nhất bên trái
            node->right = node->left; // Di chuyển cây con bên trái để thành cây con bên phải
            node->left = nullptr; // Gán cây con bên trái của node thành nullptr
            removesupport(minLeft->data, node->right, (k1 + 1) % k); // Đệ quy xóa node nhỏ nhất
        }
    } else {
        // Đi tìm node cần xóa ở cây con bên trái hoặc bên phải
        if (point[k1] < node->data[k1]) {
            removesupport(point, node->left, (k1 + 1) % k); // Xóa ở cây con bên trái
        } else {
            removesupport(point, node->right, (k1 + 1) % k); // Xóa ở cây con bên phải
        }
    }
}

kDTreeNode* kDTree::nodefindremove(kDTreeNode *&node, int k1) {
    if (node == nullptr) {
        return nullptr;
    }

    // Đệ quy tìm node nhỏ nhất theo chiều phân chia
    if (k1 == 0 && node->left != nullptr) {
        return nodefindremove(node->left, (k1 + 1) % k);
    } else if (k1 == 1 && node->right != nullptr) {
        return nodefindremove(node->right, (k1 + 1) % k);
    }

    return node;
}


bool kDTree::searchsupport(const vector<int> &point, kDTreeNode *&node, int k1)
{
    if (node == nullptr) {
        // Đã đạt nút lá, không tìm thấy point
        return false;
    }

    // So sánh giá trị của point và node tại chiều k1
    if (point == node->data) {
        // Nếu giá trị của point giống với giá trị của node
        return true;
    } else if (point[k1] < node->data[k1]) {
        // Nếu giá trị của point nhỏ hơn giá trị của node tại chiều k1,
        // tiếp tục tìm kiếm trong cây con bên trái
        return searchsupport(point, node->left, (k1 + 1) % k);
    } else {
        // Nếu giá trị của point lớn hơn giá trị của node tại chiều k1,
        // tiếp tục tìm kiếm trong cây con bên phải
        return searchsupport(point, node->right, (k1 + 1) % k);
    }
}

void kDTree::sort( vector<vector<int>> &pointList, int k1) {
    int size = pointList.size();
    bool swapped;
    // Sử dụng thuật toán sắp xếp nổi bọt
    for (int i = 0; i < size - 1; i++) {
        swapped = false;
        // Lặp qua từng cặp phần tử để so sánh và đổi chỗ nếu cần
        for (int j = 0; j < size - i - 1; j++) {
            // So sánh theo chỉ số k1 của vector con thứ j và j+1
            if (pointList[j][k1] > pointList[j + 1][k1]) {
                // Nếu điều kiện đúng, đổi chỗ hai vector con
                vector<int> temp = pointList[j];
                pointList[j] = pointList[j + 1];
                pointList[j + 1] = temp;
                swapped = true;
            }
        }
        if (!swapped)
            break;
    }
}


void kDTree::buildTreesupport(const vector<vector<int>> &pointList,kDTreeNode *&node, int k1)
{

    // Kiểm tra nếu danh sách điểm rỗng
    if (pointList.empty()) {
        node = nullptr;
        return;
    }
    int k = pointList[0].size(); // Số chiều của điểm dữ liệu
    this->k=k;//update lại chiều
    int size = pointList.size();
    vector<vector<int>> pointlist=pointList;
    // Sắp xếp danh sách điểm theo chiều k1
    sort(pointlist, k1);
    int median;
    // Tìm điểm trung bình của danh sách điểm
    if(size%2==0) median = size/2-1;
    else median = size/2;
    // Tạo node mới
    node = new kDTreeNode(pointlist[median]);
    vector<vector<int>> left;
    vector<vector<int>> right;
    for(int i=0;i<median;i++){
        left.push_back(pointlist[i]);
    }
    for(int i=median+1;i<pointlist.size();i++){
        right.push_back(pointlist[i]);
    }
    buildTreesupport(left,node->left,(k1+1)%k);
    buildTreesupport(right,node->right,(k1+1)%k);
}


void kDTree::nearestNeighboursupport(const vector<int>& target, kDTreeNode *node, kDTreeNode *&best, int k1) {
    if (node == nullptr)
        return;

    if(target[k1]<node->data[k1])nearestNeighboursupport(target,node->left,best,(k1+1)%k);
    else nearestNeighboursupport(target,node->right,best,(k1+1)%k);
    if (node->left==nullptr && node->right==nullptr){
        if(best==nullptr){
            best=node;
        }
    }
    double R=distance(target,best->data);
    int r=abs(target[k1]-node->data[k1]);
    if(r<=R && (distance(target,node->data)>=R) ){
        if(searchsupport(best->data,node->left,k1))nearestNeighboursupport(target,node->right,best,(k1+1)%k);
        else nearestNeighboursupport(target,node->left,best,(k1+1)%k);
    }
    else if(distance(target,node->data)<R)
        best=node;
}


void kDTree::kNearestNeighbousupport(const vector<int> &target, kDTreeNode *node, int k, vector<kDTreeNode *> &bestList)
{
    if (node == nullptr)
        return;
    kDTreeNode *best=nullptr;
    nearestNeighboursupport(target,node,best,0);
    bestList.push_back(best);
    for(int i=0;i<k-1;i++){
        kDTreeNode *tmp=nullptr;
        SpecnearestNeighboursupport(target,node,tmp,0,bestList);
        bestList.push_back(tmp);
    }
    

}

const double MAX_DISTANCE = 1e9; // Một giá trị lớn đủ để làm khoảng cách lớn nhất

void kDTree::SpecnearestNeighboursupport(const vector<int> &target, kDTreeNode *node, kDTreeNode *&best, int k1, vector<kDTreeNode *> bestprev) {
    if (node == nullptr)
        return;

    if (target[k1] < node->data[k1])
        SpecnearestNeighboursupport(target, node->left, best, (k1 + 1) % k, bestprev);
    else
        SpecnearestNeighboursupport(target, node->right, best, (k1 + 1) % k, bestprev);

    // Check if node is in bestprev
    if (check(node->data, bestprev))
        return;

    if (best == nullptr || distance(target, node->data) < distance(target, best->data)) {
        best = node;
    }

    double R = best ? distance(target, best->data) : MAX_DISTANCE;
    int r = abs(target[k1] - node->data[k1]);

    if (r <= R) {
        if (target[k1] < node->data[k1])
            SpecnearestNeighboursupport(target, node->right, best, (k1 + 1) % k, bestprev);
        else
            SpecnearestNeighboursupport(target, node->left, best, (k1 + 1) % k, bestprev);
    }
}

const kDTree &kDTree::operator=(const kDTree &other)
{
    if (this == &other)
        return *this; // Self-assignment, do nothing

    // Clear existing tree
    clearTree(root);

    // Copy from other tree
    k = other.k;
    root = copyTree(other.root);

    return *this;
}

// Hàm hỗ trợ sao chép cây đệ quy
kDTreeNode* kDTree::copyTree(kDTreeNode* node) const
{
    if (node == nullptr) {
        return nullptr;
    }

    // Tạo một node mới và sao chép dữ liệu
    kDTreeNode* newNode = new kDTreeNode(node->data);
    newNode->left = this->copyTree(node->left); // Sao chép cây con bên trái
    newNode->right = this->copyTree(node->right); // Sao chép cây con bên phải

    return newNode;
}

// Hàm hỗ trợ xóa cây đệ quy
void kDTree::clearTree(kDTreeNode* node)
{
    if (node != nullptr) {
        // Xóa cây con bên trái và bên phải trước
        this->clearTree(node->left);
        this->clearTree(node->right);

        // Xóa node hiện tại
        delete node;
    }
}

vector<vector<int>> convertListListToListVector(const list<list<int>>& listListInt) {
    vector<vector<int>> vectorVectorInt;
    
    // Duyệt qua từng list<int> trong listListInt và sử dụng constructor để chuyển đổi
    for (const auto& innerList : listListInt) {
        vector<int> vectorInt(innerList.begin(), innerList.end());
        vectorVectorInt.push_back(vectorInt);
    }
    
    return vectorVectorInt;
}
int kNN::findpos(vector<vector<int>> x, vector<int> target)
{
    int index = 0;
    for (const auto& innerList : x) {
        if (innerList == target) {
            // Trả về vị trí của targetList trong danh sách listOfLists
            return index;
        }
        index++;
    }
    // Trả về -1 nếu không tìm thấy targetList
    return -1;
}
void kNN::fit(Dataset &X_train, Dataset &y_train)
{
    int nRowsX, nColsX, nRowsY, nColsY;
    X_train.getShape(nRowsX, nColsX);
    y_train.getShape(nRowsY, nColsY);
    Xtree=convertListListToListVector(X_train.data);
    ytree=convertListListToListVector(y_train.data);
    // Tạo cây k-D Tree từ dữ liệu đặc trưng X_train
    kdtree.buildTree(Xtree);
}
int mode(double *array, int size) {
    // Tạo một mảng đếm số lần xuất hiện của mỗi phần tử
    int *count = new int[size];
    for (int i = 0; i < size; ++i) {
        count[i] = 0;
    }

    // Đếm số lần xuất hiện của mỗi phần tử trong mảng
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (array[j] == array[i]) {
                count[i]++;
            }
        }
    }

    // Tìm phần tử có số lần xuất hiện nhiều nhất
    double modeValue = array[0];
    int maxCount = count[0];
    for (int i = 1; i < size; ++i) {
        if (count[i] > maxCount) {
            maxCount = count[i];
            modeValue = array[i];
        }
    }

    // Xóa bộ đếm và trả về giá trị mode
    delete[] count;
    return modeValue;
}

Dataset kNN::predict(Dataset &X_test)
{
    vector<vector<int>> x_test= convertListListToListVector(X_test.data);
    // Tìm các điểm gần nhất với các điểm trong x_test
    Dataset predictions;
    string label = "label"; // Replace this with your label for the prediction
    predictions.columnName.push_back(label); // Add label directly to the predictions dataset
    for (const auto& x : x_test) {
        list<int> result;
        vector<kDTreeNode *> bestlist;
        vector<int> pos;
        kdtree.kNearestNeighbour(x,this->k,bestlist);
        for(int i=0;i<k;i++){
            int singlepos=findpos(Xtree,bestlist[i]->data);
            pos.push_back(singlepos);
        }
        double* y_pred=new double[k];
        for(int i=0;i<k;i++){
            int y_pred_single=ytree[pos[i]][0];
            y_pred[i]=y_pred_single;
        }
        int i = mode(y_pred,k);
        result.push_back(i);
        predictions.data.push_back(result);
    }
    return predictions;
}


double kNN::score(const Dataset &y_test, const Dataset &y_pred)
{
    int correct = 0;
    int nRowsY, nColsY;
    y_test.getShape(nRowsY, nColsY);
    vector<vector<int>> ytest=convertListListToListVector(y_test.data);
    vector<vector<int>> ypred=convertListListToListVector(y_pred.data);
    for (int i = 0; i < nRowsY; ++i) {
        int a = ytest[i][0];
        int b = ypred[i][0];
        
        if (a == b) ++correct;
    }
    double score = static_cast<double>(correct-1) / (nRowsY); // Corrected calculation
    return score;
}
