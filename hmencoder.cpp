#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <algorithm>
#include <queue>
#include <sstream>

using namespace std;


struct node {//pointer implementation of binary tree
    int weight;
    int representative;
    node * left_child;
    node * right_child;
};

node * makenode (int index, int weightvalue) {//make base nodes
    node * x = new node;
    x->weight = weightvalue;
    x->representative = index;
        //cout << x->representative << endl;
    x->left_child = NULL;
    x->right_child = NULL;
    //cout << x << endl;
    return x;
}

/*node * makeparentnode (int index, int weightvalue, node *left, node *right) {//make parents nodes with children
    node *x = new node;
    x->weight = weightvalue;
    x->representative = index;
    x->left_child = left;
    x->right_child = right;
    //cout << x << "parent" << endl;
    return x;
}*/

node * mincomparison(queue<node> &q1, queue<node> &q2) {//compare the two queues to find minimum node
    node * x = new node;

    if (q2.empty()) {
        //cout << q1.front().representative << "1st out" << endl;
        x->weight = q1.front().weight;//if one queue is empty return the other
        x->representative = q1.front().representative;
        x->left_child = NULL;
        x->right_child = NULL;
        
        //cout << x << " address x" << endl;
        q1.pop();
        //cout << x << "firstpop" << endl;

        //cout << x << " address x after pop" << endl;
        return x;
    }
    if (q1.empty()) {
        x->weight = q2.front().weight;//if one queue is empty return the other
        x->representative = q2.front().representative;
        x->left_child = q2.front().left_child;
        x->right_child = q2.front().right_child;
        q2.pop();
        return x;
    }
    if (q2.front().weight < q1.front().weight) {
        x->weight = q2.front().weight;//if one queue is empty return the other
        x->representative = q2.front().representative;
        x->left_child = q2.front().left_child;
        x->right_child = q2.front().right_child;
        q2.pop();
        return x;
    }
     else if (q2.front().weight == q1.front().weight) {
         if (q2.front().representative < q1.front().representative) {
            x->weight = q2.front().weight;//if one queue is empty return the other
            x->representative = q2.front().representative;
            x->left_child = q2.front().left_child;
            x->right_child = q2.front().right_child;
            q2.pop();
            return x;
         }
         else {        
        x->weight = q1.front().weight;//if one queue is empty return the other
        x->representative = q1.front().representative;
        x->left_child = NULL;
        x->right_child = NULL;
        
        //cout << x << " address x" << endl;
        q1.pop();
        return x;
         }
     }
    else {
        x->weight = q1.front().weight;//if one queue is empty return the other
        x->representative = q1.front().representative;
        x->left_child = NULL;
        x->right_child = NULL;
        q1.pop();
        return x;
    }
}

void codeoutput(node * current, int a[], int i, double &bitcount, string codes[]) {  
    //output codes, recurring until leaf is reached then printing the array
    //cout << current << " current" << endl;
    if (current->left_child != NULL) {
        a[i] = 0;

        codeoutput(current->left_child, a, i+1, bitcount, codes);
    }
    if (current->right_child != NULL) {
        a[i] = 1;
        //cout << root->right_child->representative << endl;
        codeoutput(current->right_child, a, i+1, bitcount, codes);
    }

    if (current->left_child == NULL && current->right_child == NULL) {
        //cout << char(current->representative) << ": ";
        string codestring = "";
        for (int j = 0; j < i; j++) {
            codestring += to_string(a[j]);
            bitcount++;
        }
        codes[current->representative] = codestring;
        //cout << endl;
    }
    return;
}

int minindex(int a[]) {//index of minimum element in starting array
    int min = 2147483647;
    int minindex = -1;
    for (int i = 0; i < 128; i++) {
        if (a[i] < min && a[i] > 0) {
            minindex = i;
            min = a[i];
        }
    }
    return minindex;
}

int main(int argc, char* argv[]){

    string filename = argv[1];
    ifstream fin;
    fin.open(filename.c_str());

    if (fin.fail()) {
        cout << "Error in file opening" <<endl;
        exit(1);
    }

    int frequency[128] = {0}; //array to store the occurence of each character

    string line;
    string target;
    while (getline(fin, line)) {
        target += line;
        //cout << line << endl;
        int n = target.length();
        for (int i = 0; i < n; i++) {
            char x = target[i];
            frequency[int(x)] += 1;
        }
    }
    if (target == "") {
        ofstream fout;
        fout.open("encodemsg.txt");
        fout << endl;
        fout.close();
        fout.open("code.txt");
        fout << "Ave = 0 bits per symbol" <<endl;
        fout.close();
        fin.close();
        return 0;
        }
    
    fin.close();
    

    //create two queues and fill them up accordingly: q1 is for leaf nodes, q2 is for joined nodes
    queue<node> q1;
    queue<node> q2;

    int count = 0;

    for (int i = 0; i < 128; i++) {
        if (frequency[i] != 0) {
            count++;
        }
    }
    for (int i = 0; i < count; i++) {
        int index = minindex(frequency);
        //cout << frequency[index] << char(index) << "pleasework" << endl;
        q1.push(*makenode(index, frequency[index]));
        frequency[index] *= -1; //convert to negative to get next max
    }
    double bitcount = 0;
    string codes[128];
    int breakcondition = 0;
    //cout << q1.front().representative << "frontofqueue";
//while queues are not empty and not size 1 respectively
//make parent nodes and connect them up
    while(!(q1.empty() && q2.size() == 1)) {
        if (q1.size() + q2.size() == 1) {
            q2.push(q1.front());
            q1.pop();
            codes[q2.front().representative] = "0";
            bitcount = 1;
            breakcondition = 1;
            break;
        }
        node * left = mincomparison(q1,q2);
        node * right = mincomparison(q1,q2);
        if (right->representative < left->representative) {
            node * temp = right;
            right = left;
            left = temp;
        }
        node * parent;
        parent = makenode(min(left->representative, right->representative), left->weight+right->weight);
        parent->left_child = left;
        parent->right_child = right;
    //cout << x << "parent" << endl;
        //node *leftptr = &left;
        //cout << "leftptr address " << leftptr << " " << leftptr->weight << endl;
        //node *rightptr = &right;
        //cout << "rightptr address " << rightptr << " " << rightptr->weight << endl;
        q2.push(*parent);
    }
    //cout << q2.front().weight << endl;
    node *rootptr = &q2.front();
    int a[100];

    node * current = rootptr;


    if (breakcondition != 1) {
        codeoutput(rootptr, a, 0, bitcount, codes);
    }
    
    double averagesum = 0;

    ofstream fout;
    fout.open("code.txt");

    for (int i = 0; i < 128; i++) {
        if (codes[i] != "") {
            averagesum += (frequency[i]*-1)*codes[i].length();
            if (frequency[i] < -1) {
                count += ((frequency[i]*-1) -1);
            }
            if (i == 32) {
                fout << "space: " << codes[i] << endl;
            }
            else {
                fout << char(i) << ": " << codes[i] << endl;
            //cout << averagesum << " " << codes[i].length() << endl;
            }
        }
    }

    fout << setprecision(4) << "Ave = " << averagesum/count << " bits per symbol" << endl;

    fout.close();

    fin.open(filename.c_str());
    fout.open("encodemsg.txt");
    string inputx;
    int linecount = 0;
    while (getline(fin, inputx)) {
        for (int i = 0; i < inputx.length(); i++) {
            for (int j = 0; j < codes[int(inputx[i])].length(); j++) {
                fout << codes[int(inputx[i])][j];
                linecount++;
                if (linecount == 80) {
                    fout << endl;
                    linecount = 0;
                }
            }
        }
        
    }
    fout << endl;

    fout.close();

    fin.close();


}