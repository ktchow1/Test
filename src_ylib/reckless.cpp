#include<iostream>
#include<string>
#include<thread>
#include<reckless/file_writer.hpp>
#include<reckless/severity_log.hpp>


// ************************************************************ //
// Reckless logger decouples logging into :
// 1. formatter deals with formatting (involves memcpy into %s)
// 2. writer deals with file writing  (involves file I/O)
// 3. logger is the user interface
// ************************************************************ //
struct custom_arg
{
    std::uint8_t r;
    std::uint8_t g;
    std::uint8_t b;
};

// Format function for customized argument (see example in reckless GIT repo)
const char* format(reckless::output_buffer* buffer, const char* formatting_str, const custom_arg& x)
{
    // supporting multiple formats ...
    if (*formatting_str == 'a')
    {
        reckless::template_formatter::format(buffer, "(%d,%d,%d)", (std::uint32_t)(x.r), (std::uint32_t)(x.g), (std::uint32_t)(x.b)); 
    }
    else if (*formatting_str == 'b')
    {
        reckless::template_formatter::format(buffer, "(%d|%d|%d)", (std::uint32_t)(x.r), (std::uint32_t)(x.g), (std::uint32_t)(x.b)); 
    }
    else if (*formatting_str == 'c')
    {
        reckless::template_formatter::format(buffer, "[%d|%d|%d]", (std::uint32_t)(x.r), (std::uint32_t)(x.g), (std::uint32_t)(x.b)); 
    }
    else if (*formatting_str == 'd')
    {
        reckless::template_formatter::format(buffer, "<%d-%d-%d>", (std::uint32_t)(x.r), (std::uint32_t)(x.g), (std::uint32_t)(x.b)); 
    }
    return formatting_str + 1; // This is a must.
}

void test_reckless_custom_argument()
{
    std::string filename("reckless.log");

    // Step 1 - Instantiate a file_writer
    reckless::file_writer writer(filename.c_str());

    // Step 2 - Instantiate a severity_log
    reckless::severity_log 
    <
        reckless::indent<4U>,       // indentation
        ' ',                        // delimitor
        reckless::timestamp_field,  // use timestamp as 1st field
        reckless::severity_field    // use severity  as 2nd field
    > 
    logger(&writer);

    // Suppose we have an algo doing something, and log arbitrarily ...
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

    // Custom POD 
    custom_arg x(120,180,240);
    logger.debug("rgb is %a", x);
    logger.debug("rgb is %b", x);
    logger.debug("rgb is %c", x);
    logger.debug("rgb is %d", x);
}


// ********************************************************* //
// *** Customization on reckless logger with inheritance *** //
// ********************************************************* //
// Customization of header :
// 1. timestamp
// 2. code location
// 3. self-defined severity
//
// Headers are different from arguments.
// Headers are predefined. Headers can be customized POD.
// Arguments can be variadic. Arguments can also be customized POD.
//
// Remark :
// 1. There is problem for reckless-log to handle std::string, but char array is OK. 
// 2. There is no tutorial about inheriting reckless::basic_log, we discovered it 
//    by tracing code in class policy_formatter. How is this possible?
// 

class custom_log : public reckless::basic_log
{  
public:
    custom_log(reckless::file_writer* reckless_writer) : reckless::basic_log(reckless_writer)
    {
    }
    
    template<typename... ARGS>
    void log(const char* formatting_str, ARGS&&... args)
    {
        // Forward to base class's write function, with customized header
        reckless::basic_log::write
        <
            reckless::policy_formatter<reckless::indent<4U>, ' '>
        > 
        (reckless::indent<4U>(), formatting_str, std::forward<ARGS>(args)...);
        // I dont know why I need to default construct an indent, 
        // but this is how class policy_formatter is implemented.
    }
};

void test_reckless_custom_header_and_argument()
{
    std::string filename("reckless2.log");

    // Step 1 - Instantiate a file_writer
    reckless::file_writer writer(filename.c_str());

    // Step 2 - Instantiate a custom_log
    custom_log logger(&writer);

    // Suppose we have an algo doing something, and log arbitrarily ...
    double m = 3.141592654;
    double c = 1.2345678e-9;
    logger.log("custom logger name=%s value=%d", "david", 110);
    logger.log("custom logger name=%s value=%d", "susan", 120);
    logger.log("custom logger name=%s value=%d", "frank", 130);
        

}






