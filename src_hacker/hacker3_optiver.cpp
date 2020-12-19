#include<iostream>
#include<sstream>
#include<string>
#include<array>

std::uint32_t c2i(char c) { return c-'A'; }
char i2c(std::uint32_t i) { return 'A'+i; }

class tree_checker
{
public:
    tree_checker()
    {
        clear();
    }
    
    void clear()
    {
        root = empty;
        for(auto& x:tree)
        {
            x.parent = empty;
            x.lhs = empty;
            x.rhs = empty;
        }
        for(auto& x:errors)
        {
            x = false;
        }
    }
    
    void debug() const
    {
        std::cout << "\n[tree-debug]";
        for(std::uint32_t n=0; n!=26; ++n)
        {
            if (n % 6 ==0) std::cout << "\n";
            std::cout << (int)n << " : " << tree[n].parent << tree[n].lhs << tree[n].rhs << ",  ";
        }
        std::cout << "\nErrors : ";
        for(const auto& x:errors)
        {
            std::cout << std::boolalpha << x << " ";
        }
    }
    
    bool load(const std::string& str)
    {
        std::uint32_t n=0; 
        while(n<str.size())
        {
            char x; char y;
            if (!extract_one_edge(str, n, x, y))
            {
                errors[0] = true;
                return false; // stop processing and return E1
            }
            
            std::uint32_t xi = c2i(x);
            std::uint32_t yi = c2i(y);
            
            // In the following, whenever we encounter error, can we just stop processing and return that error?
            // No, because there may be another error with higher priority lying at the latter part of the input string.
            if      	(tree[xi].lhs == empty)		tree[xi].lhs = y;
            else if	(tree[xi].lhs == y)		errors[1] = true; // duplicated
            else if	(tree[xi].rhs == empty)		tree[xi].rhs = y;
            else if	(tree[xi].rhs == y)		errors[1] = true; // duplicated
            else   						errors[2] = true; // more than 2 children
            
            if      	(tree[yi].parent == empty)	tree[yi].parent = x;
            else if	(tree[yi].parent == x)		errors[1] = true; // duplicated (this checking is redundant)
            else     					errors[4] = true; // more than 1 parent, i.e. not a tree   
        }
        
        // Final root checking
        for(std::uint32_t m=0; m!=26; ++m)
        {
            if (tree[m].parent == empty && (tree[m].lhs!=empty || tree[m].rhs!=empty))
            {
                if (root == empty) root = i2c(m);
                else errors[3] = true; // multiple roots
            }
        }
        if (root == empty) errors[4] = true; // If there is no root, implies that the tree forms a cycle.
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
    
    // Recursive function for depth first search
    void print_to_ss(char c, std::stringstream& ss) const
    {
        std::uint32_t i = c2i(c);
        
        ss << c;    
        if (tree[i].lhs != empty)
        {
            ss << "(";
            print_to_ss(tree[i].lhs, ss);
            ss << ")";
        }
        if (tree[i].rhs != empty)
        {
            ss << "(";
            print_to_ss(tree[i].rhs, ss);
            ss << ")";
        }   
    }

private:
    static const char empty = '*';
    struct info
    {
        char parent;
        char lhs;
        char rhs;
    };    
    
    char root;
    std::array<info,26> tree;
    std::array<bool,5> errors;
};

void test_tree_checker()
{
    std::string str("(A,B) (A,C) (B,D) (B,E) (C,F) (C,G) (D,H) (D,I) (E,J) (F,K)");
//  std::getline(std::cin, str);
    
    tree_checker chk;
    chk.load(str);
    chk.debug();
    std::cout << "\nprint tree : " << chk.get_output();
    std::cout << "\n\n";
}
