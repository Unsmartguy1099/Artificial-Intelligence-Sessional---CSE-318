#include <iostream>
#include <vector>
#include <algorithm>
#include <bits/stdc++.h>
#include <cstdlib>
#include <ctime>   
#include <windows.h>
#include <string>

using namespace std;

class TreeNode {
public: 
    int feature_index;
    double threshold;
    TreeNode* left;
    TreeNode* right;
    double value;

    TreeNode(int feature_index = -1, double threshold = 0.0, TreeNode* left = nullptr, TreeNode* right = nullptr, double value = -1) {
        this->feature_index = feature_index;
        this->threshold = threshold;
        this->left = left;
        this->right = right;
        this->value = value;
    }
};

double findMajorityClass(const vector<double>& labels) {
   vector<int> classCounts;
    for (int i=0;i<labels.size();i++) {
        if (labels.at(i) >= classCounts.size()) {
            classCounts.resize(labels.at(i) + 1);
        }
        classCounts[labels.at(i)]++;
    }
    int majorityClass = 0;
    int maxCount = 0;
    for (int i = 0; i < classCounts.size(); i++) {
        if (classCounts[i] > maxCount) {
            maxCount = classCounts[i];
            majorityClass = i;
        }
    }
    return majorityClass;
}

double Entropy(const std::vector<double>& labels) {
   map<double, int> labelCounts;
    for (int i = 0; i < labels.size(); i++) {
        if (labelCounts.find(labels.at(i)) == labelCounts.end()) {
            labelCounts[labels.at(i)] = 1;
        } else {
            labelCounts[labels.at(i)]++;
        }
    }
    double entropy = 0.0;
    int totalSamples = labels.size();
    for (const auto& pair : labelCounts) {
        double probability = (double)(pair.second) / totalSamples;
        entropy =entropy - probability * log2(probability);
    }
    return entropy;
}

double InfoGain(const vector<double>& parentY, const std::vector<double>& leftDecisions, const std::vector<double>& rightDecisions) {
    double parentEntropy = Entropy(parentY);
    double leftWeight = static_cast<double>(leftDecisions.size()) / parentY.size();
    double rightWeight = static_cast<double>(rightDecisions.size()) / parentY.size();
    double weightedAvgEntropy = leftWeight *Entropy(leftDecisions) + rightWeight *Entropy(rightDecisions);
    return parentEntropy - weightedAvgEntropy;
}

// Recursive decision tree-building function
TreeNode* buildDecisionTree(const vector<vector<double>>& Attributes, const vector<double>& Decisions, int maxDepth) {
    if (maxDepth == 0 || Attributes.empty()) {
        // Create a leaf node with the majority class label as the value
        double leafValue = findMajorityClass(Decisions);
        return new TreeNode(-1, 0.0, nullptr, nullptr, leafValue);
    }
    //cout<<maxDepth<<endl;
    int numSamples =Attributes.size();
    int numFeatures = Attributes[0].size();
    int bestFeature = -1;
    double bestThreshold = 0.0;
    double bestInfoGain = -1.0;

    for (int featureIndex = 0; featureIndex < numFeatures; ++featureIndex) {
        // Try all unique thresholds for the current feature
        vector<double> thresholds;
        for (int sampleIndex = 0; sampleIndex < numSamples; ++sampleIndex) {
            thresholds.push_back(Attributes[sampleIndex][featureIndex]);
        }

        sort(thresholds.begin(), thresholds.end());
        thresholds.erase(unique(thresholds.begin(), thresholds.end()), thresholds.end());

        for (double threshold : thresholds) {
            vector<vector<double>> leftAttributes;
            vector<double> leftDecisions;
            vector<vector<double>> rightAttributes;
            vector<double> rightDecisions;
            for (int sampleIndex = 0; sampleIndex < numSamples; ++sampleIndex) {
                if (Attributes[sampleIndex][featureIndex] > threshold) {
                    rightAttributes.push_back(Attributes.at(sampleIndex));
                    rightDecisions.push_back(Decisions.at(sampleIndex));
                } else {
                    leftAttributes.push_back(Attributes.at(sampleIndex));
                    leftDecisions.push_back(Decisions.at(sampleIndex));
                }
            }
            double infoGain = InfoGain(Decisions, leftDecisions, rightDecisions);
            if (infoGain > bestInfoGain) {
                bestInfoGain = infoGain;
                bestFeature = featureIndex;
                bestThreshold = threshold;
            }
        }
    }

    if (bestInfoGain > 0) {
        TreeNode* decisionNode = new TreeNode(bestFeature, bestThreshold);
        vector<vector<double>> leftAttributes, rightAttributes;
        vector<double> leftDecisions, rightDecisions;
        for (int sampleIndex = 0; sampleIndex < numSamples; ++sampleIndex) {
            if (Attributes.at(sampleIndex).at(bestFeature)<= bestThreshold) {
                leftAttributes.push_back(Attributes.at(sampleIndex));
                leftDecisions.push_back(Decisions.at(sampleIndex));
            } else {
                rightAttributes.push_back(Attributes.at(sampleIndex));
                rightDecisions.push_back(Decisions.at(sampleIndex));
            }
        }
        decisionNode->left = buildDecisionTree(leftAttributes, leftDecisions, maxDepth - 1);
        decisionNode->right = buildDecisionTree(rightAttributes, rightDecisions, maxDepth - 1);
        return decisionNode;
    } else {
        double leafValue = findMajorityClass(Decisions);
        return new TreeNode(-1, 0.0, nullptr, nullptr, leafValue);
    }
}

// void print(int depth,TreeNode* node){
//     for(int i=0;i<depth;i++)
//         cout<<"/-";
//     cout<<"("<<node->threshold<<","<<node->value<<","<<node->feature_index<<")"<<endl;
//     if(node->left!=nullptr)
//         print(depth+1,node->left);
//     if(node->right!=nullptr)
//         print(depth+1,node->right);
// }


void printBT(const std::string& prefix, const TreeNode* node, bool isLeft)
{
    if( node != nullptr )
    {
        std::cout << prefix;

        std::cout << (isLeft ? "|---" : "\\---" );
    
        // print the value of the node
        std::cout << "("<<node->value<<","<<node->feature_index<<","<<node->threshold<<")" << std::endl;

        // enter the next tree level - left and right branch
        printBT( prefix + (isLeft ? "|   " : "    "), node->left, true);
        printBT( prefix + (isLeft ? "|   " : "    "), node->right, false);
    }
}

double predict(TreeNode* node, const vector<double>& attributes) {
    while (node->left != nullptr && node->right != nullptr) {
        int featureIndex = node->feature_index;
        double threshold = node->threshold;
        if (attributes[featureIndex] <= threshold) {
            node = node->left; 
        } else {
            node = node->right; 
        }
    }
    return node->value;
}

double Tester(int split,string file_name){
    ifstream inputFile(file_name);
    vector<vector<double>> vect;
    vector<double> vect2;
    map<string,int>* att;
    bool check=true;
    int* val;
    int att_size=0;
    if (!inputFile.is_open()) {
        cerr << "Error opening file!" << endl;
        return 1;
    }
    string line;
    while (getline(inputFile, line)) {
        istringstream iss(line); 
        vector<string> tokens;
        string token;
        while (getline(iss, token, ',')) {
            tokens.push_back(token); 
        }
        vector<double> temp;
        //Data cleaning
        if(!check){
            for (int i=0;i<tokens.size();i++){
                auto it = att[i].find(tokens.at(i));
                if (it != att[i].end()) {
                    if(i==(tokens.size()-1))
                        vect2.push_back(it->second);
                    else
                        temp.push_back(it->second);
                }else{
                    att[i].emplace(tokens.at(i),val[i]);
                    if(i==(tokens.size()-1))
                        vect2.push_back(val[i]);
                    else
                        temp.push_back(val[i]);
                    val[i]++;
                }
            //cout<<"--"<<i<<"--"<<endl;
            }
            vect.push_back(temp);
        }

        if(check){
            att_size=tokens.size();
            att=new map<string,int>[tokens.size()];
            val=new int[tokens.size()];
            for(int i=0;i<tokens.size();i++)
                val[i]=0;
            check=false;
        }
    }
    for(int i=0;i<att_size;i++){
        cout<<"----------------------"<<endl;
        for (const auto& pair : att[i]) {
            cout << "Key: " << pair.first << ", Value: " << pair.second <<endl;
        }  
    }
    cout<<vect2.size()<<" "<<vect.size()<<endl;
    inputFile.close();
    vector<vector<double>> leftVect;
    vector<vector<double>> rightVect;
    vector<double> leftVect2;
    vector<double> rightVect2;
    srand(static_cast<unsigned>(time(nullptr)));
    int randomValue = rand();
    for (size_t i = 0; i < vect.size(); ++i) {
            if(rand()%100<split){
                rightVect.push_back(vect[i]);
                rightVect2.push_back(vect2[i]);
            }else{
                leftVect.push_back(vect[i]);
                leftVect2.push_back(vect2[i]);
            }
    }
    int maxDepth = 10000;
    //training
    TreeNode* root = buildDecisionTree(rightVect,rightVect2, maxDepth);
    //print(0,root);
    printBT("", root, false); 
    double cnt=0;
    for(int i=0;i<leftVect.size();i++){
        int p=predict(root, leftVect.at(i));
        if(p==leftVect2.at(i))
            cnt++;
    }
    return cnt/(double)leftVect.size();
}

double Mean(vector<double> accuracy, int size) {
    double sum = 0.0;
    for (int i = 0; i < size; i++) {
        sum = sum+ accuracy.at(i);
    }
    return sum/size;
}

double StandardDeviation(vector<double> accuracy, int size) {
    double mean = Mean(accuracy, size);
    double squaredDifferencesSum = 0.0;
    for (int i = 0; i < size; i++) {
        double diff = accuracy.at(i) - mean;
        squaredDifferencesSum += diff * diff;
    }
    double variance = squaredDifferencesSum / size;
    return std::sqrt(variance);
}

int main(int argc, char* argv[]) {
    //initialization
    int split_ratio=80;
    int testCount=20;
    int sleep=300;
    string file_name="car.data";
    if (argc==5) {
        split_ratio=stoi(argv[1]);
        testCount=stoi(argv[2]);
        sleep=stoi(argv[3]);
        file_name=argv[4];
    }
    if(argc==2) {
        file_name=argv[1];
    }
    //-----------------
    double test=0;
    vector<double> accuracy;
    for(int i=0;i<testCount;i++){
        double acc=Tester(split_ratio,file_name);
        test=test+acc;
        accuracy.push_back(acc);
        cout<<"accuracy: "<<acc*100<<"%"<<endl;
        Sleep(sleep);
    }
    cout<<"result:"<<endl;
    cout<< "Mean of the Accuracy: " <<Mean(accuracy,testCount)*100 <<"%"<< std::endl;
    cout<< "Standard Deviation of the Accuracy: " << StandardDeviation(accuracy,testCount)*100 <<"%"<< std::endl;
    return 0;
}
