#include<iostream>
#include<memory>


struct T
{
    std::uint32_t x;
    std::uint32_t y;
    std::uint32_t z;
};

inline std::ostream& operator<<(std::ostream& os, const T& t)
{
    os << "T = [" << t.x << ", " << t.y << ". " << t.z << "]";
    return os;
}

void test_unique_ptr_to_array()
{
    std::cout << "\nis_same0 = " << std::is_same<T*,  T[]>::value;                                                   // false
    std::cout << "\nis_same1 = " << std::is_same<T*,  decltype(std::declval<std::unique_ptr<T  >>().get())>::value;  //  true
    std::cout << "\nis_same2 = " << std::is_same<T*,  decltype(std::declval<std::unique_ptr<T[]>>().get())>::value;  //  true
    std::cout << "\nis_same3 = " << std::is_same<T**, decltype(std::declval<std::unique_ptr<T[]>>().get())>::value;  // false
    std::cout << "\nis_same4 = " << std::is_same<T[], decltype(std::declval<std::unique_ptr<T[]>>().get())>::value;  // false
    std::cout << "\nis_same5 = " << std::is_same<T&,  decltype(*(std::declval<std::unique_ptr<T  >>()   ))>::value;  //  true
    std::cout << "\nis_same6 = " << std::is_same<T&,  decltype( (std::declval<std::unique_ptr<T[]>>()[0]))>::value;  //  true

    // We can consider :
    // std::unique_ptr<T>   is a smart pointer for : T* ptr = new T();
    // std::unique_ptr<T[]> is a smart pointer for : T* ptr = new T[100];
    //
    // In fact, both are T* under the hood, hence is_same1 & is_same2 are true.
    // There is operator*  for the former (but not the latter).
    // There is operator[] for the latter (but not the former).

    std::unique_ptr<T>   t(new T(11,12,13));
    std::unique_ptr<T[]> ts(new T[5]{{21,22,23}, 
                                     {31,32,33},
                                     {41,42,43},
                                     {51,52,53},
                                     {61,62,63}}); // woo, this is how we init array.


    std::cout << "\nt     = " << *t;
    std::cout << "\nts[0] = " << ts[0];
    std::cout << "\nts[1] = " << ts[1];
    std::cout << "\nts[2] = " << ts[2];
    std::cout << "\nts[3] = " << ts[3];
    std::cout << "\nts[4] = " << ts[4];

}
