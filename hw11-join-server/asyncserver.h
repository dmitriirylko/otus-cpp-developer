#pragma once

#include <iostream>
#include <memory>

#include <boost/asio.hpp>

#include "parser.h"
#include "dbmanager.h"

class Session : public std::enable_shared_from_this<Session>
{
public:
    Session(boost::asio::ip::tcp::socket socket) :
        m_socket{std::move(socket)}
    {
        std::cout << "Session with endpoint: " << m_socket.remote_endpoint() << " started" << std::endl;
        m_dbManager = std::make_unique<DbManager>();
    }

    ~Session()
    {
        std::cout << "Session with endpoint: " << m_socket.remote_endpoint() << " finished" << std::endl;
    }

    void start()
    {
        read();
    }

private:
    enum { BUF_SIZE = 1024 };

    void read()
    {
        std::shared_ptr<Session> selfPtr{shared_from_this()};
        m_socket.async_read_some(boost::asio::buffer(m_buf, BUF_SIZE),
            [this, selfPtr](boost::system::error_code ec, std::size_t length){
                if(!ec)
                {
                    std::cout << "Received " << length << " bytes from " <<
                    m_socket.remote_endpoint() << " : ";
                    std::cout.write(m_buf, length);
                    std::tie(m_res, m_errorMsg) = m_parser.process(m_buf, length);
                    if(m_res == ParserErrorCode::SKIP) read();
                    else
                    {
                        // m_dbManager->execute();
                        write();
                    }
                }
            });
    }

    void write()
    {
        auto self(shared_from_this());
        std::string msg;
        if(m_res == ParserErrorCode::OK) msg = "< OK\n";
        else msg = "< ERR " + m_errorMsg + "\n";
        boost::asio::async_write(m_socket, boost::asio::buffer(msg, msg.length()),
            [this, self](boost::system::error_code ec, std::size_t){
                if(!ec)
                {
                    read();
                }
            });
    }

    boost::asio::ip::tcp::socket m_socket;
    char m_buf[BUF_SIZE];
    Parser m_parser;
    ParserErrorCode m_res;
    std::string m_errorMsg;
    std::unique_ptr<DbManager> m_dbManager;
};

class Server
{
public:
    Server(boost::asio::io_context& io_context, size_t port) :
        m_acceptor{io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)}
    {
        std::cout << "Server listening on port " << m_acceptor.local_endpoint() << " ..." << std::endl;
        accept();
    }

    ~Server() = default;

private:
    void accept()
    {
        m_acceptor.async_accept([this](boost::system::error_code ec, boost::asio::ip::tcp::socket socket){
            if(!ec)
            {
                std::cout << "Connected to server from endpoint: " << socket.remote_endpoint() << std::endl;
                std::make_shared<Session>(std::move(socket))->start();
            }
            accept();
        });
    }

    boost::asio::ip::tcp::acceptor m_acceptor;
};