#include <iostream>

#include <boost/thread.hpp>

int main(int argc, char* argv[])
{
    boost::thread::id tid = boost::this_thread::get_id();
    std::cout<<"thread id is "<<tid<<std::endl;
    return 0;
}
