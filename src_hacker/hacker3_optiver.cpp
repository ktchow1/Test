#include<iostream>
#include<sstream>
#include<string>
#include<array>
#include<queue>
#include<unordered_set>
#include<unordered_map>


// ******************************************************************************************************* //
// Root is the ultimate parent of all nodes.
// ******************************************************************************************************* //
// Error codes in order of priority :
// E1 = incorrect string format
// E2 = duplicated edge
// E3 = a node having more than two children
// E4 = a tree not covering all nodes (i.e. multi-trees or multi-roots)
// E5 = a tree having cycles or 
//      a node having multi-parents
// ******************************************************************************************************* //
// Hint E4
// 1. if the node-set consists of multi-separated  -trees, then there is E4 only (no E5d)
// 2. if the node-set consists of multi-overlapping-trees, then there are both E4,E5d
// ******************************************************************************************************* //
// Hint for E5 
// 1. cycle != multi-parents, although they are overlapping sets
// 2. cycle means a node having its parent / grandparent as child
//
// Lets classify E5 into 4 cases ... 
// E5a : an edge from a node to the root                 : forming cycle, with no multi-parents
// E5b : an edge from a node to its parent / grandparent : forming cycle, with    multi-parents
// E5c : an edge from a node to another branch           :      no cycle, with    multi-parents
// E5d : an edge from a node to another tree             :      no cycle, eith    multi-parents, return E4
// ...   hence E5d always comes together with E4.
// ******************************************************************************************************* //
class tree_checker
{
public:
    tree_checker() : root(empty)
    {
        for(auto& x:errors) x = false;
    }

    void debug() const
    {
        std::cout << "\n[tree]";
        for(const auto& x:tree)
        {
            std::cout << "\n" << x.first << " : ";
            std::cout << " parents=[";
            for(const auto& y:x.second.parents) std::cout << y;
            std::cout << "] children=[";
            for(const auto& y:x.second.children) std::cout << y;
            std::cout << "] label = " << x.second.label;
            std::cout << ", lhs = " << x.second.lhs_child;
            std::cout << ", rhs = " << x.second.rhs_child;
        }

        std::cout << "\nErrors : ";
        for(const auto& x:errors)
        {
            std::cout << std::boolalpha << x << " ";
        }
    }
    
    bool load(const std::string& str)
    {
        // pass through input string
        std::uint32_t n=0; 
        while(n<str.size())
        {
            char x; 
            char y;

            // *** E1 check *** //
            if (!extract_one_edge(str, n, x, y))
            {
                errors[0] = true;
                return false; 
            }
            
            // *** E2 check (fill tree-struct) *** //
            auto iter0 = tree.find(x);
            if (iter0 == tree.end())
            {
                tree[x] = info{empty,y,empty,{},{y}};
            }
            else
            {
                auto i  = iter0->second.children.find(y);
                if  (i == iter0->second.children.end())     iter0->second.children.insert(y);
                else                                        errors[1] = true;

                // For final output
                if      (iter0->second.lhs_child == empty)  iter0->second.lhs_child = y;
                else if (iter0->second.rhs_child == empty)  iter0->second.rhs_child = y;
            }

            auto iter1 = tree.find(y);
            if (iter1 == tree.end())
            {
                tree[y] = info{empty,empty,empty,{x},{}};
            }
            else
            {
                auto i  = iter1->second.parents.find(x);
                if  (i == iter1->second.parents.end())      iter1->second.parents.insert(x);
                else                                        errors[1] = true; // redundant
            }
        }

        for(const auto& x:tree)
        {
            // *** E3 check *** //
            if (x.second.children.size()>2)
            {
                errors[2] = true;
            }

            // *** E4 1st-check *** //
            if (x.second.parents.empty())
            {
                if (root == empty) root = x.first;
                else errors[3] = true; // multiple roots
            }

            // *** E5 1st-check *** //
            if (x.second.parents.size()>1)
            {
                errors[4] = true;
            }
        }

        // *** E4 2nd-check *** //
        if (!errors[3] && tree.size() > 0)
        {
            errors[3] = multiple_blobs(tree.begin()->first);
        }
        
        // *** E5 2nd-check *** //
        if (root == empty)
        {
            errors[4] = true;
        } 
        return true;
    }
    
    std::string get_output() const
    {
        if (errors[0]) return "E1"; 
        if (errors[1]) return "E2";
        if (errors[2]) return "E3";
        if (errors[3]) return "E4";
        if (errors[4]) return "E5";
        
        std::stringstream ss;
        ss << "(";
        print_to_ss(root, ss);
        ss << ")";
        return ss.str();
    }
 
private:   
    bool extract_one_edge(const std::string& str, std::uint32_t& n, char& x, char& y)
    {
        // read a space char
        if (n>0)
        {
            if (str[n]!=' ') return false;
            ++n;
        }
        if (str[n]!='(') return false;
        ++n;
        if (str[n]<'A' || str[n]>'Z') return false;
        x = str[n];
        ++n;
        if (str[n]!=',') return false;
        ++n;
        if (str[n]<'A' || str[n]>'Z') return false;
        y = str[n];
        ++n;
        if (str[n]!=')') return false;
        ++n;
        return true;
    }

    // Iterative implementation 
    bool multiple_blobs(char c)
    {
        std::queue<char> q;
        q.push(c);

        while(!q.empty())
        {
            auto iter = tree.find(q.front());
            q.pop();

            if (iter != tree.end() && iter->second.label == empty)
            {
                iter->second.label = labelled;
                for(auto& x:iter->second.parents)  q.push(x);
                for(auto& x:iter->second.children) q.push(x);
            }
        }

        for(const auto& x:tree)
        {
            if (x.second.label != labelled) return true;
        }
        return false;
    }

    
    // Recursive implementation
    void print_to_ss(char c, std::stringstream& ss) const
    {
        ss << c;    
        auto iter = tree.find(c);
        if (iter != tree.end())
        {
            if (iter->second.lhs_child != empty)
            {
                ss << "(";
                print_to_ss(iter->second.lhs_child, ss);
                ss << ")";

                if (iter->second.rhs_child != empty)
                {
                    ss << "(";
                    print_to_ss(iter->second.rhs_child, ss);
                    ss << ")";
                }
            }
        }
    }

private:
    static const char empty = '*';
    static const char labelled = 'x';
    struct info
    {
        char label;
        char lhs_child;
        char rhs_child;
        std::unordered_set<char> parents;
        std::unordered_set<char> children;
    };    
    
private:
    char root;
    std::unordered_map<char,info> tree;
    std::array<bool,5> errors;
};

// ********************************** //
// This is a test-driven development. //
// ********************************** //
void test_tree_checker()
{
    for(std::uint32_t n=0; n!=55; ++n)
    {
        std::string str;
        std::string expected;
        
        // Suite 0 : Normal case (with single child or both children)
        if (n==0)   { str = "(A,B) (A,C) (B,D) (C,F)";                              expected = "(A(B(D))(C(F)))";       }
        if (n==1)   { str = "(A,B) (A,C) (B,D) (C,F) (C,G)";                        expected = "(A(B(D))(C(F)(G)))";    }
        if (n==2)   { str = "(A,B) (A,C) (B,D) (B,E) (C,F) (C,G)";                  expected = "(A(B(D)(E))(C(F)(G)))"; }

        // Suite 1 : single error
        if (n==3)   { str = "(A,B) (A,C) (B,D) (B,E) (C,F) (C G)";                  expected = "E1"; } // E1 
        if (n==4)   { str = "(A,B) (A,C) (B,D) (B,E) (C,F) (B,E)";                  expected = "E2"; } // E2
        if (n==5)   { str = "(A,B) (A,C) (B,D) (B,E) (C,F) (B,X)";                  expected = "E3"; } // E3
        if (n==6)   { str = "(A,B) (A,C) (B,D) (B,E) (C,F) (X,Y)";                  expected = "E4"; } // E4
        if (n==7)   { str = "(A,B) (A,C) (B,D) (B,E) (C,F) (D,A)";                  expected = "E5"; } // E5a 
        if (n==8)   { str = "(A,B) (A,C) (B,D) (B,E) (C,F) (D,C)";                  expected = "E5"; } // E5b
        if (n==9)   { str = "(A,B) (A,C) (B,D) (B,E) (C,F) (C,D)";                  expected = "E5"; } // E5c

        // Suite 2 : double errors
        if (n==10)  { str = "[A,B) (A,C) (B,D) (B,E) (C,F) (B,E)";                  expected = "E1"; } // E1,E2
        if (n==11)  { str = "(A B) (A,C) (B,D) (B,E) (C,F) (B,X)";                  expected = "E1"; } // E1,E3
        if (n==12)  { str = "(A,B] (A,C) (B,D) (B,E) (C,F) (X,Y)";                  expected = "E1"; } // E1,E4
        if (n==13)  { str = "(A,B),(A,C) (B,D) (B,E) (C,F) (D,A)";                  expected = "E1"; } // E1,E5a 
        if (n==14)  { str = "(A,B) (A,C) (B,D) (B,E) (C,F) (D,C) ";                 expected = "E1"; } // E1,E5b
        if (n==15)  { str = " (A,B) (A,C) (B,D) (B,E) (C,F) (C,D)";                 expected = "E1"; } // E1,E5c
                                                                                    // no E5d unless there is E4
        // when duplicated edge is not related to E3,E4,E5
        if (n==16)  { str = "(A,B) (A,C) (B,D) (B,E) (C,F) (B,X) (A,C)";            expected = "E2"; } // E2,E3
        if (n==17)  { str = "(A,B) (A,C) (B,D) (B,E) (C,F) (X,Y) (A,C)";            expected = "E2"; } // E2,E4
        if (n==18)  { str = "(A,B) (A,C) (B,D) (B,E) (C,F) (D,A) (A,C)";            expected = "E2"; } // E2,E5a 
        if (n==19)  { str = "(A,B) (A,C) (B,D) (B,E) (C,F) (D,C) (A,C)";            expected = "E2"; } // E2,E5b
        if (n==20)  { str = "(A,B) (A,C) (B,D) (B,E) (C,F) (C,D) (A,C)";            expected = "E2"; } // E2,E5c

        // when duplcated edge is ALSO the edge that causes E3,E4,E5 
        if (n==21)  { str = "(A,B) (A,C) (B,D) (B,E) (C,F) (B,X) (B,X)";            expected = "E2"; } // E2,E3
        if (n==22)  { str = "(A,B) (A,C) (B,D) (B,E) (C,F) (X,Y) (X,Y)";            expected = "E2"; } // E2,E4
        if (n==23)  { str = "(A,B) (A,C) (B,D) (B,E) (C,F) (D,A) (D,A)";            expected = "E2"; } // E2,E5a 
        if (n==24)  { str = "(A,B) (A,C) (B,D) (B,E) (C,F) (D,C) (D,C)";            expected = "E2"; } // E2,E5b
        if (n==25)  { str = "(A,B) (A,C) (B,D) (B,E) (C,F) (C,D) (C,D)";            expected = "E2"; } // E2,E5c
        
        if (n==26)  { str = "(A,B) (A,C) (B,D) (B,E) (C,F) (X,Y) (B,I)";            expected = "E3"; } // E3,E4
        if (n==27)  { str = "(A,B) (A,C) (B,D) (B,E) (C,F) (D,A) (B,I)";            expected = "E3"; } // E3,E5a 
        if (n==28)  { str = "(A,B) (A,C) (B,D) (B,E) (C,F) (D,C) (B,I)";            expected = "E3"; } // E3,E5b
        if (n==29)  { str = "(A,B) (A,C) (B,D) (B,E) (C,F) (C,D) (B,I)";            expected = "E3"; } // E3,E5c

        if (n==30)  { str = "(A,B) (A,C) (B,D) (B,E) (C,F) (D,A) (X,Y)";            expected = "E4"; } // E4,E5a [FAIL-1] 
        if (n==31)  { str = "(A,B) (A,C) (B,D) (B,E) (C,F) (D,C) (X,Y)";            expected = "E4"; } // E4,E5b
        if (n==32)  { str = "(A,B) (A,C) (B,D) (B,E) (C,F) (C,D) (X,Y)";            expected = "E4"; } // E4,E5c
        if (n==33)  { str = "(A,B) (A,C) (B,D) (B,E) (C,F) (X,Y) (Y,F)";            expected = "E4"; } // E4,E5d

        // Suite 3 : triple errors (not involve E1 as I know how its implemented)
        if (n==34)  { str = "(A,B) (A,C) (B,D) (D,E) (D,F) (X,Y) (D,G) (D,G)";      expected = "E2"; } // E2,E3,E4 
        if (n==35)  { str = "(A,B) (A,C) (B,D) (D,E) (D,F) (D,A) (D,A)";            expected = "E2"; } // E2,E3,E5a
        if (n==36)  { str = "(A,B) (A,C) (B,D) (D,E) (D,F) (D,C) (D,C)";            expected = "E2"; } // E2,E3,E5b [FAIL-2]
        if (n==37)  { str = "(A,B) (A,C) (B,D) (D,E) (X,Y) (D,A) (D,A)";            expected = "E2"; } // E2,E4,E5a
        if (n==38)  { str = "(A,B) (A,C) (B,D) (D,E) (X,Y) (D,C) (D,C)";            expected = "E2"; } // E2,E4,E5b
        if (n==39)  { str = "(A,B) (A,C) (B,D) (D,E) (X,Y) (Y,C) (Y,C)";            expected = "E2"; } // E2,E4,E5d
        if (n==40)  { str = "(A,B) (A,C) (B,D) (D,E) (D,F) (X,Y) (D,A)";            expected = "E3"; } // E3,E4,E5a
        if (n==41)  { str = "(A,B) (A,C) (B,D) (D,E) (D,F) (X,Y) (D,C)";            expected = "E3"; } // E3,E4,E5b
        if (n==42)  { str = "(A,B) (A,C) (B,D) (D,E) (D,F) (X,Y) (D,Y)";            expected = "E3"; } // E3,E4,E5d

        // Suite 4 : quad errors
        if (n==43)  { str = "(A,B) (A,C) (B,D) (D,E) (D,F) (X,Y) (D,Y) (D,Y)";      expected = "E2"; } // E2,E3,E4,E5d [FAIL-2]
        
        // Suite 5 : minimal node-set with E2,E3,E4,E5
        if (n==44)  { str = "(A,B)";                                                expected = "(A(B))"; }
        if (n==45)  { str = "(a,a)";                                                expected = "E1"; } // E1
        if (n==46)  { str = "(A,B) (A,B)";                                          expected = "E2"; } // E2
        if (n==47)  { str = "(A,B) (A,C) (A,D)";                                    expected = "E3"; } // E3
        if (n==48)  { str = "(A,B) (C,D)";                                          expected = "E4"; } // E4
        if (n==49)  { str = "(A,A)";                                                expected = "E5"; } // E5a
        if (n==50)  { str = "(A,B) (B,A)";                                          expected = "E5"; } // E5b
        if (n==51)  { str = "(A,B) (B,C) (A,C)";                                    expected = "E5"; } // E5c
        if (n==52)  { str = "(A,B) (C,B)";                                          expected = "E4"; } // E4,E5d
        if (n==53)  { str = "(";                                                    expected = "E1"; } // E1
        if (n==54)  { str = "";                                                     expected = "E5"; } // E5

        tree_checker chk;
        chk.load(str);
        auto answer = chk.get_output();

        if (answer == expected)
        {
            std::cout << "\nTest_" << n << " succeed, expected = " << expected;
        }
        else
        {
            std::cout << "\nTest_" << n << " failed, expected = " << expected << ", but answer = " << answer;
            chk.debug();
            std::cout << "\n";
        }
    }

    std::cout << "\n\n\n";
}


