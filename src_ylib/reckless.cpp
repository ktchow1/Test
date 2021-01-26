#include<iostream>
#include<string>
#include<thread>
#include<reckless/file_writer.hpp>
#include<reckless/severity_log.hpp>


class algo
{
    // Step 1 - Instantiate a file_writer
    reckless::file_writer writer;

    // Step 2 - Instantiate a severity_log
    reckless::severity_log
    <
        reckless::indent<4>,        // indentation
        ' ',                        // delimitor
        reckless::timestamp_field,  // use timestamp as 1st field
        reckless::severity_field    // use severity  as 2nd field
    > log;

public:
    algo(const std::string& filename) : writer(filename.c_str()), log(&writer)
    {
    }

    void run()
    {
        // Suppose algo is doing something, and we log arbitrarily ...
        double m = 3.141592654;
        double c = 1.2345678e-9;
        log.debug("price model m=%f c=%f", m, c);
        log.debug("price model m=%f c=%f", m+1, c);
        log.debug("price model m=%f c=%f", m+2, c);
        log.debug("price model m=%f c=%f", m+3, c);
        std::this_thread::sleep_for(std::chrono::milliseconds(300));

        std::uint32_t p = 150;
        std::uint32_t q = 10000;
        {
            reckless::scoped_indent indent;
            log.info ("hitter place price=%d quant=%d", p, q);
            log.info ("hitter place price=%d quant=%d", p, q+1);
            {
                reckless::scoped_indent indent;
                log.info ("hitter place price=%d quant=%d", p, q+2);
                log.info ("hitter place price=%d quant=%d", p, q+3);
            }
            log.info ("hitter place price=%d quant=%d", p, q+4);
            log.info ("hitter place price=%d quant=%d", p, q+5);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(300));

        log.warn("fitter %p", "unused 1");
        log.warn("fitter %p", "unused 2");
        log.warn("fitter %p", "unused 3");
        log.warn("fitter %p", "unused 4");
        std::this_thread::sleep_for(std::chrono::milliseconds(300));

        using namespace std::string_literals;
        log.error("quoter %s", "runtime_error_"s + std::to_string(301));
        log.error("quoter %s", "runtime_error_"s + std::to_string(302));
    }
};


void test_reckless()
{
    algo x("reckless.log");
    x.run();
}
