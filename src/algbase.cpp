#include<iostream>
#include<memory>
#include<vector>

struct parser
{
    virtual void invoke() const = 0;
};

struct parserA : public parser { virtual void invoke() const { std::cout << "\nThis is parserA."; } };
struct parserB : public parser { virtual void invoke() const { std::cout << "\nThis is parserB."; } };
struct parserC : public parser { virtual void invoke() const { std::cout << "\nThis is parserC."; } };

struct oms 
{
    void run()
    {
        auto p = get_parser();
        p->invoke();
    } 

    virtual std::unique_ptr<parser> get_parser() = 0;
};

struct omsA : public oms { virtual std::unique_ptr<parser> get_parser() { return std::make_unique<parserA>(); } };
struct omsB : public oms { virtual std::unique_ptr<parser> get_parser() { return std::make_unique<parserB>(); } };
struct omsC : public oms { virtual std::unique_ptr<parser> get_parser() { return std::make_unique<parserC>(); } };

void test_algbase()
{
    std::unique_ptr<oms> pA(std::make_unique<omsA>());
    std::unique_ptr<oms> pB(std::make_unique<omsB>());
    std::unique_ptr<oms> pC(std::make_unique<omsC>());
    pA->run();
    pB->run();
    pC->run();

}

// *** Default parameter in virtual function *** //
struct B
{
    virtual void fct(int n=10)
    {
        std::cout << "\nB::fct with para " << n << std::flush;
    }
};

struct D : public B
{
    virtual void fct(int n=20)
    {
        std::cout << "\nD::fct with para " << n << std::flush;
    }
};

void test_algbase_default_para()
{
    B b;
    D d;
    B* pb0 = &b;
    B* pb1 = &d;
    D* pd  = &d;
    pb0->fct();
    pb1->fct();
    pd ->fct();
}








