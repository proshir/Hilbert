#include <iostream>
#include <vector>
#include <map>
using namespace std;

class Node 
{
    public:
        Node(Node* _left=0, Node* _right=0, char _value=0) : value(_value), left(_left), right(_right) {}
        ~Node()
        {
            if(left)
                delete left;
            if(right)
                delete right;
            value = 0;
        }

        bool impl() const
        {
            return value == 0;
        } 

        Node* left;
        Node* right;
        char value;
};

bool node_equal(const Node* lhs, const Node* rhs) 
{
    if (lhs == 0 || rhs == 0)
        return false;
    if (lhs->impl() && rhs->impl())
        return node_equal(lhs->left, rhs->left) && node_equal(lhs->right, rhs->right) ;
    return lhs->value == rhs->value;
}

string to_string(const Node* self)
{
    if (self->impl())
        return "(" + to_string(self->left) + ">" + to_string(self->right) + ")" ;
    std::string s{self->value};
    return s;
}

pair<int,Node *> convert_to_tree (string s, int i);

enum Proof_Type
{
    asmp,
    a1,
    a2,
    mp
};

class Proof
{
public:
    Proof(){}
    Proof(vector<Node*> &_asmps, Node* _node)
    {
        node = _node;
        proof_type = asmp;
        asmps = _asmps;
    }
    virtual ~Proof(){}
    Proof_Type proof_type;
    Node* node;
    int ni;
    vector<Node*> asmps;
};

string to_string(Proof* self);

class A1_Proof : public Proof
{
public:
    A1_Proof(vector<Node*> &_asmps, Node* A, Node* B)
    {
        node = new Node(A, new Node(B, A));
        proof_type = a1;
        asmps = _asmps;
    }
    A1_Proof(vector<Node*> &_asmps, Node* _node)
    {
        node = _node;
        proof_type = a1;
        asmps = _asmps;
    }
};

string to_string(const A1_Proof* self)
{
    return to_string(self->ni) + " " + to_string(self->node) + " A1";
}

class A2_Proof : public Proof
{
public:
    A2_Proof(vector<Node*> &_asmps, Node* A, Node* B, Node* C)
    {
        node = new Node(new Node(A, new Node(B, C)), new Node(new Node(A, B), new Node(A, C)));
        proof_type = a2;
        asmps = _asmps;
    } 
    A2_Proof(vector<Node*> &_asmps, Node* _node)
    {
        node = _node;
        proof_type = a2;
        asmps = _asmps;
    }     
};

string to_string(const A2_Proof* self)
{
    return to_string(self->ni) + " " + to_string(self->node) + " A2";
} 

bool is_A1(Node* point);

bool is_A2(Node* point);

int is_asmp(vector<Node*> & asmps, Node* node);

class MP : public Proof
{
public:
    MP(vector<Node*> &_asmps, Proof* _AB, Proof* _A)
    {
        AB = _AB;
        A = _A;
        node = AB->node->right;
        asmps = _asmps;
        proof_type = mp;
    }

    Proof* AB;
    Proof* A;
};

string to_string(const MP* self)
{
    return to_string(self->ni) + " " + to_string(self->node) + " " + to_string(self->AB->ni) + "," + to_string(self->A->ni);
}

Proof* DT(vector<Node*> asmps, Node* node);

Proof* help_from_user(vector<Node*>& asmps, Node* node);

Proof* convert_to_impl(vector<Node*> &new_asmps,Proof* proof);

Proof* hilbert(vector<Node*>& asmps, Node* node);

void print_proof(Proof* proof, int& i);

Node* get_node_console();

void print_asmps(vector<Node*>& asmps);

int main()
{
    vector<Node*> asmps;
    cout << "What do you want me to prove? ";
    Node* node = get_node_console();
    cout << "===================================\n";
    Proof* proof = hilbert(asmps, node);
    int ni = 0;
    print_proof(proof, ni);
}

Node* get_node_console()
{
    string x = "";
    cin >> x;
    return convert_to_tree(x, 0).second;
}

void print_proof(Proof* proof, int& i)
{
    if (proof->proof_type == mp)
    {
        MP mP = dynamic_cast<MP&>(*proof);
        print_proof(mP.A, i);
        print_proof(mP.AB, i);
    }
    proof->ni = ++i;
    cout << to_string(proof) << endl;
}

pair<int,Node *> convert_to_tree (string s, int i)
{
    if (s[i] == '(')
    {
        pair<int,Node*> nodeLeft = convert_to_tree(s, i + 1);
        pair<int,Node*> nodeRight = convert_to_tree(s, nodeLeft.first + 1);
        Node* node = new Node(nodeLeft.second, nodeRight.second);
        return {nodeRight.first+1, node};
    } 
    Node* node = new Node(0, 0, s[i]);
    return {i+1,node};
}

Proof* hilbert(vector<Node*>& asmps, Node* node)
{
    if (is_A1(node))
        return new A1_Proof(asmps, node);

    if (is_A2(node))
        return new A2_Proof(asmps, node);

    if (asmps.size())
    {
        int u = is_asmp(asmps, node);
        if (u > -1)
            return new Proof(asmps, node);
    }

    for (int i=0; i<asmps.size(); i++)
        if (node_equal(asmps[i]->right, node))
            for(int j=0;j<asmps.size(); j++)
                if (node_equal(asmps[i]->left, asmps[j]))
                {
                    Proof* AB = new Proof(asmps, asmps[i]);
                    Proof* A = new Proof(asmps, asmps[j]);
                    return new MP(asmps, AB, A);
                }

    if (node->impl())
        return DT(asmps, node);

    for (int i=0; i<asmps.size(); i++)
        if (asmps[i]->impl() && is_asmp(asmps, asmps[i]->right) == -1)
            for(int j=0;j<asmps.size(); j++)
                if (node_equal(asmps[i]->left, asmps[j]))
                {
                    Proof* BC = hilbert(asmps, new Node(asmps[i]->right, node)); // ⊢ B>node
                    Proof* AB = new Proof(asmps, asmps[i]);
                    Proof* A = new Proof(asmps, asmps[j]);
                    MP* B = new MP(asmps, AB, A);
                    return new MP(asmps, BC, B); // ⊢ ((A^AB)>B)>node
                }

    return help_from_user(asmps, node);
}

bool is_A1(Node* point)
{
    return (point->impl() &&
            point->right->impl() &&
            node_equal(point->left, point->right->right));
}

bool is_A2(Node* point)
{
    return (point->impl() &&
            point->left->impl() &&
            point->left->right->impl() &&
            point->right->impl() &&
            point->right->left->impl() &&
            point->right->right->impl() &&
            node_equal(point->left->left, point->right->left->left) &&
            node_equal(point->left->left, point->right->right->left) &&
            node_equal(point->left->right->left, point->right->left->right) &&
            node_equal(point->left->right->right, point->right->right->right));
}

int is_asmp(vector<Node*> & asmps, Node* node)
{
    for (int i=0; i<asmps.size(); i++)
    {
        if (node_equal(node, asmps[i]))
            return i;
    }

    return -1;
}

Proof* convert_to_impl(vector<Node*> &new_asmps,Proof* proof)
{
    vector<Node*> asmps = new_asmps;
    Node* A = asmps.back();
    asmps.pop_back();
    Node* node = new Node(A, proof->node);

    if (is_A1(node))
        return new A1_Proof(asmps, node);

    if (is_A2(node))
        return new A2_Proof(asmps, node);

    if (asmps.size())
    {
        int u = is_asmp(asmps, node);
        if (u > -1)
            return new Proof(asmps, node);
    }

    if (proof->proof_type == a1 ||
        proof->proof_type == a2 ||
        (proof->proof_type == asmp &&
         !node_equal(proof->node, A)))
    {
        Proof* B = 0;

        if (proof->proof_type == a1)
            B = new A1_Proof(asmps, proof->node);
        else if (proof->proof_type == a2)
            B = new A2_Proof(asmps, proof->node);
        else
            B = new Proof(asmps, proof->node);

        A1_Proof* BA = new A1_Proof(asmps, proof->node, A);

        return new MP(asmps, BA, B);
    } else if (proof->proof_type == mp)
    {
        MP mP =  dynamic_cast<MP&>(*proof);
        Proof* CB = convert_to_impl(new_asmps, mP.AB ); // a>(c>b)
        Proof* C = convert_to_impl(new_asmps, mP.A); // a>c

        A2_Proof* ACB = new A2_Proof(asmps, A, C->node->right, proof->node); // ( a>(c>b) ) > ((a>c)>(a>b))
        MP* ACAB = new MP(asmps, ACB, CB); // (a>c) > (a>b)
        return new MP(asmps, ACAB, C); // (a>b)
    } 
    else // a>a
    {
        node = new Node(proof->node, proof->node);
        A1_Proof* BBB = new A1_Proof(asmps, proof->node, node);
        A2_Proof* BBBB = new A2_Proof(asmps, proof->node, node, proof->node);
        MP* BB = new MP(asmps, BBBB, BBB);
        A1_Proof* B = new A1_Proof(asmps, proof->node, proof->node);
        return new MP(asmps, BB, B);
    }
}

Proof* DT(vector<Node*> asmps, Node* node) // X,A D B -> X D A>b
{
    vector<Node*> new_asmps = asmps;
    new_asmps.push_back(node->left);
    Proof* proof = hilbert(new_asmps, node->right);
    return convert_to_impl(new_asmps, proof);
}

void print_asmps(vector<Node*>& asmps)
{
    for (int i=0; i<asmps.size(); i++)
        cout << i+1 << ") " << to_string(asmps[i]) << endl;
}

Proof* help_from_user(vector<Node*>& asmps, Node* node)
{
    Node* P;
    while(true)
    {
        cout << "==============HELP==============\n";
        cout << "What should we prove with the help of the assumption of the following statements in order to reach the conclusion with it?\n";
            print_asmps(asmps);
        cout << "The formulas above ⊢ P > " << to_string(node) << endl;
        cout << "What could that P be?  ";
        P = get_node_console();
        cout << "================================\n";

        if (is_asmp(asmps, P) == -1)
            break;
        delete P;
    }
    Proof* A = hilbert(asmps, P);
    Proof* AB = hilbert(asmps, new Node(P, node));
    return new MP(asmps, AB, A);
}

string to_string(Proof* self)
{
    if (self->proof_type == asmp)
    {
        return to_string(self->ni) + " " + to_string(self->node) + " " + "Assumption";
    } 
    else if (self->proof_type == a1)
    {
        A1_Proof& point = dynamic_cast<A1_Proof&>(*self);
        return to_string(&point);
    }
    else if (self->proof_type == a2)
    {
        A2_Proof& point = dynamic_cast<A2_Proof&>(*self);
        return to_string(&point);
    }
    else
    {
        MP& point = dynamic_cast<MP&>(*self);
        return to_string(&point);
    }
}