#include<iostream>
#include<string>
#include<memory>
#include<thread>
#include<reckless/file_writer.hpp>
#include<reckless/severity_log.hpp>


// ************************************************************ //
// Reckless logger decouples logging into :
// 1. logger is the user interface
// -  push header, argument, formatting str in mpscq
// 2. writer is the consumer thread
// -  pop from mpmcq
// -  do formatting by replacing %d %s with arguments
// -  do file IO
// -  after last access of arguments, they are destructed 
//    explicitly inside reckless logger T::~T()
//
// Header and argument are different
// -  header does not support runtime formatting specifier
// -  header does not support variadic
// -  header is predefined as :
//       template parameter of reckless::severity_log
//    or template parameter of reckless::policy_formatter
// -  argument does support formatting string
// -  argument does support variadic
// -  both of them can be custom struct (pls define formatter)
// -  formatter for header is a member fct of header class 
// -  formatter for argument is a global fct
//
// There is problem for reckless log in handling std::string,
// it will crash due to SSO. Thus we develop our own wrapper,
// which is non-copyable, please always declare temp-obj.
//
// There are 2 ways :
// - custom_str  (non-copyable, using raw ptr)
// - custom_str2 (non-copyable, using unique_ptr)
// Advantage of the latter is no move / delete is needed.
// ************************************************************ //
struct custom_arg
{
    std::uint8_t r;
    std::uint8_t g;
    std::uint8_t b;
};

struct custom_str 
{
    custom_str() = delete;
   ~custom_str()
    {
        if (ptr!=nullptr) delete [] ptr; 
    }

    custom_str(const custom_str&) = delete;
    custom_str(custom_str&& rhs) : size(rhs.size), ptr(rhs.ptr)
    {
        rhs.size = 0;
        rhs.ptr  = nullptr;
    }

    custom_str& operator=(const custom_str&) = delete;
    custom_str& operator=(custom_str&& rhs)
    {
        std::swap(size, rhs.size);
        std::swap(ptr, rhs.ptr);
        return *this;
    }

    explicit custom_str(const std::string& str) : size(str.size()), ptr(new char[size+1])
    {
        memcpy(ptr, str.c_str(), size);
        ptr[size] = '\0';
    }

    std::uint32_t size;
    char* ptr;
};

struct custom_str2 // simpler implementation than previous one
{
    custom_str2() = delete;
   ~custom_str2() = default;
    custom_str2(const custom_str2&) = delete;
    custom_str2(custom_str2&&) = default;
    custom_str2& operator=(const custom_str2&) = delete;
    custom_str2& operator=(custom_str2&&) = default;

    explicit custom_str2(const std::string& str) : size(str.size()), uptr(new char[size+1])
    {
        memcpy(uptr.get(), str.c_str(), size);
        uptr.get()[size] = '\0';
    }

    std::uint32_t size;
    std::unique_ptr<char[]> uptr;
};


// Format function for custom argument (there is an example in reckless repo about custom argument)
const char* format(reckless::output_buffer* buffer, const char* formatting_str, const custom_arg& x)
{
    // Support multiple format specfiers
    if (*formatting_str == 'a')
    {
        reckless::template_formatter::format
        (
            buffer, "(%d,%d,%d)", 
            (std::uint32_t)(x.r), 
            (std::uint32_t)(x.g), 
            (std::uint32_t)(x.b)
        ); 
    }
    else if (*formatting_str == 'b')
    {
        reckless::template_formatter::format
        (
            buffer, "(%d|%d|%d)", 
            (std::uint32_t)(x.r), 
            (std::uint32_t)(x.g), 
            (std::uint32_t)(x.b)
        ); 
    }
    else if (*formatting_str == 'c')
    {
        reckless::template_formatter::format
        (
            buffer, "[%d|%d|%d]", 
            (std::uint32_t)(x.r), 
            (std::uint32_t)(x.g), 
            (std::uint32_t)(x.b)
        ); 
    }
    else if (*formatting_str == 'd')
    {
        reckless::template_formatter::format
        (
            buffer, "<%d-%d-%d>", 
            (std::uint32_t)(x.r), 
            (std::uint32_t)(x.g), 
            (std::uint32_t)(x.b)
        ); 
    }
    return formatting_str + 1; // This is a must.
}

const char* format(reckless::output_buffer* buffer, const char* formatting_str, const custom_str& x)
{
    if (*formatting_str == 'a')
    {
        reckless::template_formatter::format
        (
            buffer, "(custom_str = %s)", x.ptr // null terminated, so no size is needed
        ); 
    }
    else if (*formatting_str == 'b')
    {
        reckless::template_formatter::format
        (
            buffer, "[custom_str = %s,%d]", x.ptr, x.size
        ); 
    }  
    return formatting_str + 1; // This is a must.
}

const char* format(reckless::output_buffer* buffer, const char* formatting_str, const custom_str2& x)
{
    if (*formatting_str == 'a')
    {
        reckless::template_formatter::format
        (
            buffer, "(custom_str2 = %s)", x.uptr.get() // null terminated, so no size is needed
        ); 
    }
    else if (*formatting_str == 'b')
    {
        reckless::template_formatter::format
        (
            buffer, "[custom_str2 = %s,%d]", x.uptr.get(), x.size
        ); 
    }  
    return formatting_str + 1; // This is a must.
}


// ******************** //
// *** Normal usage *** //
// ******************** //
void test_reckless_custom_argument()
{
    std::string filename("reckless.log");

    // *** Step 1 - Instantiate a file_writer *** //
    reckless::file_writer writer(filename.c_str());

    // *** Step 2 - Instantiate a severity_log *** //
    reckless::severity_log 
    <
        reckless::indent<4U>,       // indentation
        ' ',                        // delimitor
        reckless::timestamp_field,  // use timestamp as 1st header
        reckless::severity_field    // use severity  as 2nd header
    > 
    logger(&writer);


    // ******************** //
    // *** Normal usage *** //
    // ******************** //
    double m = 3.141592654;
    double c = 1.2345678e-9;
    logger.debug("price model m=%f c=%f", m, c);
    logger.debug("price model m=%f c=%f", m+1, c);
    logger.debug("price model m=%f c=%f", m+2, c);
    logger.debug("price model m=%f c=%f", m+3, c);
    std::this_thread::sleep_for(std::chrono::milliseconds(300));

    std::uint32_t p = 150;
    std::uint32_t q = 10000;
    {
        reckless::scoped_indent indent;
        logger.info ("hitter place price=%d quant=%d", p, q);
        logger.info ("hitter place price=%d quant=%d", p, q+1);
        {
            reckless::scoped_indent indent;
            logger.info ("hitter place price=%d quant=%d", p, q+2);
            logger.info ("hitter place price=%d quant=%d", p, q+3);
        }
        logger.info ("hitter place price=%d quant=%d", p, q+4);
        logger.info ("hitter place price=%d quant=%d", p, q+5);
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(300));

    logger.warn("fitter %s", "unused 1");
    logger.warn("fitter %s", "unused 2");
    logger.warn("fitter %s", "unused 3");
    logger.warn("fitter %s", "unused 4");
    std::this_thread::sleep_for(std::chrono::milliseconds(300));

    using namespace std::string_literals;
    logger.error("quoter %s", "runtime_error_"s + std::to_string(301));
    logger.error("quoter %s", "runtime_error_"s + std::to_string(302));


    // *********************** //
    // *** Custom argument *** //
    // *********************** //
    custom_arg x(120,180,240);
    logger.debug("x is %a", x);
    logger.debug("x is %b", x);
    logger.debug("x is %c", x);
    logger.debug("x is %d", x);

    std::string str("This is just a test.");
    logger.debug("s is %a", custom_str(str)); // always remember to create temp-str
    logger.debug("s is %b", custom_str(str)); 
    logger.debug("s is %a", custom_str2(str)); 
    logger.debug("s is %b", custom_str2(str)); 
}


// ************************************** //
// *** Customization with inheritance *** //
// ************************************** //
// Customization of header :
// 1. timestamp
// 2. code location
// 3. self-defined severity
//
// Remark :
// There is no tutorial (nor example) about using reckless::policy_formatter.
// How is it possible to trace it by reading code?

// ******************************************************** //
// *** Helper that converts integer into array of ASCII *** //
// ******************************************************** //
template<std::uint32_t M, std::integral T> // M = number of digits, N = number to be written on buffer
void int_to_ascii(char* pc, T N) 
{
    for(std::uint32_t m=0; m!=M; ++m)
    {
        pc[M-1-m] = '0' + N%10;
        N = N/10;
    }
}

struct custom_header0
{
    inline bool format(reckless::output_buffer* buffer) 
    {

        char* pc = buffer->reserve(M+2); 
        pc[0]   = '[';
        int_to_ascii<M>(pc+1, x); 
        pc[M+1] = ']';
        buffer->commit(M+2);
        return true;
    }

    static const std::uint32_t M = 5;
    std::uint32_t x;
};

struct custom_header1
{
    inline bool format(reckless::output_buffer* buffer) 
    {
        char* pc = buffer->reserve(M*3+4); 
        pc[0] = '[';
        int_to_ascii<M>(pc+1, x); 
        pc[M+1] = '-';
        int_to_ascii<M>(pc+M+2, y); 
        pc[M*2+2] = '-';
        int_to_ascii<M>(pc+M*2+3, z); 
        pc[M*3+3] = ']';
        buffer->commit(M*3+4);
        return true;
    }

    static const std::uint32_t M = 3;
    std::uint8_t x;
    std::uint8_t y;
    std::uint8_t z;
};

class custom_log : public reckless::basic_log
{  
public:
    custom_log(reckless::file_writer* reckless_writer) : reckless::basic_log(reckless_writer)
    {
    }

    // ******************************************************************* //
    // *** Forward to base class write function, without custom header *** //
    // ******************************************************************* //
    template<typename... ARGS>
    void log(const char* formatting_str, ARGS&&... args)
    {
        reckless::basic_log::write
        <
            reckless::policy_formatter<reckless::indent<4U>, ' '>
        > 
        (
            reckless::indent<4U>(), 
            formatting_str, 
            std::forward<ARGS>(args)...
        );
        // I dont know why I need to default construct an indent, 
        // but this is how it is done in class policy_formatter.
    }

    // **************************************************************** //
    // *** Forward to base class write function, with custom header *** //
    // **************************************************************** //
    template<typename... ARGS>
    void log_details(custom_header0&& hdr0,
                     custom_header1&& hdr1, 
                     const char* formatting_str, ARGS&&... args)
    {
        reckless::basic_log::write
        <
            reckless::policy_formatter<reckless::indent<4U>, ' ', 
            custom_header0,
            custom_header1>
        > 
        (
            std::move(hdr0), 
            std::move(hdr1), 
            reckless::indent<4U>(), 
            formatting_str, 
            std::forward<ARGS>(args)...
        );
        // Why constrain header to rvalue ref? Forced move
        // for copy : slow
        // for move : fast
        // keep ref : header may dangle when writer thread writes to file
    }
};

void test_reckless_custom_header_and_argument()
{
    std::string filename("reckless2.log");

    // Step 1 - Instantiate a file_writer
    reckless::file_writer writer(filename.c_str());

    // Step 2 - Instantiate a custom_log
    custom_log logger(&writer);


    logger.log("custom logger name=%s value=%d", "david", 110);
    logger.log("custom logger name=%s value=%d", "susan", 120);
    logger.log("custom logger name=%s value=%d", "frank", 130);

    custom_arg x(110,120,130);
    logger.log_details(custom_header0(200), 
                       custom_header1(101,102,103),
                       "name=%s value=%a", "rgb0", x);
    
    logger.log_details(custom_header0(201), 
                       custom_header1(151,152,153),
                       "name=%s value=%b", "rgb1", x);

    logger.log_details(custom_header0(202), 
                       custom_header1(181,182,183),
                       "name=%s value=%c", "rgb2", x);
}






