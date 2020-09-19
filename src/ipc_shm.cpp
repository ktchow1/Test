#include <iostream>
#include <cstring>    // memcpy memset
#include <string>
#include <atomic>
#include <fcntl.h>    // shared mem option
#include <unistd.h>   // getpid and fork
#include <sys/mman.h> // mmap

// need to link with option -lrt
namespace ipc
{
    const std::string buffer_name = "ABC";
    const int buffer_size = 1024;

    void producer_shared_memory()
    {
        pid_t pid = getpid();
        std::cout << "\npid" << pid << " is parent running producer for shared mempry. " << std::flush;

        auto fd   = ::shm_open(buffer_name.c_str(), O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
        auto res  = ::ftruncate(fd, buffer_size); // truncate buffer size to fixed length
        auto addr = ::mmap(NULL, buffer_size, PROT_WRITE, MAP_SHARED, fd, 0);
        std::memset(addr, 0, buffer_size);
        std::uint8_t count = 0;

        std::cout << "\nEnter command : " << std::flush;
        while(true)
        {
            std::cout << " --- " << std::flush; // It doesnt work without this line. Why?
            std::string str;
            std::cin >> str;
            std::uint8_t length = (std::uint8_t)str.size();

            ++count;
            std::memcpy(reinterpret_cast<std::uint8_t*>(addr)+2, str.c_str(), str.size());
            std::memcpy(reinterpret_cast<std::uint8_t*>(addr)+1, &length, 1);
            std::memcpy(reinterpret_cast<std::uint8_t*>(addr)+0, &count, 1);

            if (str == "quit") break;
        }
        res = munmap(addr, buffer_size);
        fd  = shm_unlink(buffer_name.c_str());
    }

    void consumer_shared_memory()
    {
        pid_t pid = getpid();
        std::cout << "\npid" << pid << " is child running consumer for shared mempry. " << std::flush;

        auto fd   = ::shm_open(buffer_name.c_str(), O_RDONLY, S_IRUSR | S_IWUSR);
        auto addr = ::mmap(NULL, buffer_size, PROT_READ, MAP_SHARED, fd, 0);
        std::uint8_t count = 0;

        while(true)
        {
            if (reinterpret_cast<std::uint8_t*>(addr)[0] == count+1)
            {
                ++count;
                std::uint8_t length = reinterpret_cast<std::uint8_t*>(addr)[1];
                std::string str(reinterpret_cast<char*>(addr)+2, length);
                std::cout << "\nreceive msg" << (int)count << " : " << str << "\n" << std::flush;

                if (str == "quit") break;
                std::cout << "\nEnter command1 : " << std::flush;
            }
        }
    }
}

void test_ipc_shared_memory()
{
    if (fork() > 0)
    {
        ipc::producer_shared_memory();
    }
    else
    {
        ipc::consumer_shared_memory();
    }
}
