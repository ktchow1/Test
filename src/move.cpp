#include<iostream>

struct Y
{
    Y() : value(123)
    {
    }

    ~Y() = default;
    Y(const Y&) = delete;
    Y(Y&& rhs)
    {
        value = rhs.value;
        rhs.value = 666;
    }
    Y& operator=(const Y&) = delete;
    Y& operator=(Y&&) = default;

    std::uint32_t value;
};

class T
{
public:
    T() : y{}
    {
    }
    ~T() = default;
    T(const T&) = default;
    T(T&&) = default;
    T& operator=(const T&) = default;
    T& operator=(T&&) = default;

    const Y& get_const_value() const
    {
        return y;
    }

    Y& get_value()
    {
        return y;
    }

    void print() const
    {
        std::cout << "\nT.y = " << y.value << std::flush;
    }

    Y y;
};

void test_move()
{
    T t;
    t.print();
//  Y y0 = t.get_value();                   // compile error : try to invoke copy of Y
//  Y y1 = std::move(t.get_const_value());  // compile error : try to invoke copy of Y (to create a temp copy for moving)
    Y y2 = std::move(t.get_value());
    t.print();
}
