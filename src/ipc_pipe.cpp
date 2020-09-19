#include <iostream>
#include <cstring>    // memcpy memset
#include <string>
#include <atomic>
#include <fcntl.h>    // pipe
#include <unistd.h>   // getpid and fork
#include <sys/stat.h>
#include <sys/types.h>

// need to link with option -lrt
namespace ipc
{
    void producer_pipe(int& fd, const std::string& comment)
    {
        pid_t pid = getpid();
        std::cout << "\npid" << pid << " is parent running producer for " << comment << std::flush;

        std::cout << "\nEnter command : ";
        while(true)
        {
            std::string str;
            std::cin >> str;
            
            ::write(fd, str.c_str(), str.size());
            if (str == "quit") break;
        }
        ::close(fd);
    }

    void consumer_pipe(int& fd, const std::string& comment)
    {
        pid_t pid = getpid();
        std::cout << "\npid" << pid << " is child running consumer for " << comment << std::flush;

        char buffer[1024];
        while(true)
        {
             auto bytes = ::read(fd, buffer, 1024);
             std::string str(buffer, bytes);
             std::cout << "\nreceive msg = " << str << std::flush;
             
             if (str == "quit") break;
             std::cout << "\nEnter command : " << std::flush;
        }
        ::close(fd);
    }
}

void test_ipc_unnamed_pipe()
{
    // Fork will copy the pipes, copied pipes can be accessed in parent, child.
    int fds[2]; 
    ::pipe(fds); // create both pipes together

    if (fork() > 0)
    {
        ::close(fds[0]); // close unused fd
        ipc::producer_pipe(fds[1], "unnamed pipe"); // fds[1] is for writer
    }
    else
    {
        ::close(fds[1]); // close unused fd
        ipc::consumer_pipe(fds[0], "unnamed pipe"); // fds[0] is for reading
    }
}

void test_ipc_named_pipe(bool is_producer)
{
    // Important
    // 1. need to create named pipe explicitly in bash using : mkfifo my_pipe
    // 2. need to create named pipe outside mapped drive
    // 3. named pipe works like a file, you can list it in bash
    // 4. can also be done using ::mkfifo system call

    if (is_producer)
    {
        // give full path please
    //  ::mkfifo("~/my_pipe", S_IFIFO|0640); // 0777
        int fd = ::open("~/my_pipe", O_CREAT|O_WRONLY);
        std::cout << "producer fd = " << fd << std::flush;
        ipc::producer_pipe(fd, "named pipe"); 
    }
    else
    {
    //  ::mkfifo("~/my_pipe", S_IFIFO|0640); // 0777
        int fd = ::open("~/my_pipe", O_RDONLY);
        std::cout << "consumer fd = " << fd << std::flush;
        ipc::consumer_pipe(fd, "named pipe"); // this thing crash, don't know why? 
    }
}






