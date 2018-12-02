#include <iostream>

#include <boost/thread.hpp>
#include <boost/asio.hpp>

void handler_post(int num)
{
    boost::thread::id tid = boost::this_thread::get_id();
    std::cout<<"["<<tid<<"]service print number "<<num<<std::endl;
    boost::this_thread::sleep_for(boost::chrono::milliseconds(1));
    //boost::this_thread::yield();
}

int singleservice()
{
 
    boost::shared_ptr<boost::asio::io_service> service(new boost::asio::io_service);
    boost::shared_ptr<boost::asio::io_service::work> wk(new boost::asio::io_service::work(*service));
    service->post(boost::bind(handler_post, 1));
    service->post(boost::bind(handler_post, 2));
    service->post(boost::bind(handler_post, 3));
    service->post(boost::bind(handler_post, 4));
    service->post(boost::bind(handler_post, 5));
    service->post(boost::bind(handler_post, 6));
    service->post(boost::bind(handler_post, 7));
    service->post(boost::bind(handler_post, 8));
    service->post(boost::bind(handler_post, 9));
    std::cout<<"service run"<<std::endl;

    boost::thread_group threads;
    for(int i=0; i<4; i++)
    {
        //threads.create_thread(boost::bind(&boost::asio::io_service::run,service));
        boost::thread t(boost::bind(&boost::asio::io_service::run,service));
    }
    boost::this_thread::sleep_for(boost::chrono::seconds(10));
    std::cout<<"service stop"<<std::endl;
    wk.reset();
    threads.join_all();
    return 0;
}

int multiservice()
{
 
    boost::shared_ptr<boost::asio::io_service> service(new boost::asio::io_service);
    boost::shared_ptr<boost::asio::io_service::work> wk(new boost::asio::io_service::work(*service));
    service->post(boost::bind(handler_post, 1));
    service->post(boost::bind(handler_post, 2));
    service->post(boost::bind(handler_post, 3));
    service->post(boost::bind(handler_post, 4));
    service->post(boost::bind(handler_post, 5));
    service->post(boost::bind(handler_post, 6));
    service->post(boost::bind(handler_post, 7));
    service->post(boost::bind(handler_post, 8));
    service->post(boost::bind(handler_post, 9));
    std::cout<<"service run"<<std::endl;

    boost::thread_group threads;
    for(int i=0; i<4; i++)
    {
        //threads.create_thread(boost::bind(&boost::asio::io_service::run,service));
        boost::thread t(boost::bind(&boost::asio::io_service::run,service));
    }
    boost::this_thread::sleep_for(boost::chrono::seconds(10));
    std::cout<<"service stop"<<std::endl;
    wk.reset();
    threads.join_all();
    return 0;
}

int main(int argc, char* argv[])
{
    singleservice();
    multiservice();
    return 0;
}
