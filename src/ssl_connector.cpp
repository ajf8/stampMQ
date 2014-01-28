/* Copyright 2013 ajf http://github.com/ajf8
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <memory>
#include <boost/asio.hpp>
#include <boost/filesystem.hpp>

#include "fatal_exception.hpp"
#include "ssl_connection.hpp"
#include "server.hpp"
#include "connector.hpp"
#include "ssl_connector.hpp"

namespace stampmq {

using boost::asio::ip::tcp;

SslConnector::SslConnector(boost::asio::io_service& io_service,
                           const tcp::endpoint& endpoint, Server& server) :
    Connector(io_service, endpoint, server), context_(boost::asio::ssl::context::sslv23), socket_(io_service, context_)
{
    context_.set_options(
        boost::asio::ssl::context::default_workarounds
        | boost::asio::ssl::context::no_sslv2
        | boost::asio::ssl::context::single_dh_use);
}

std::string SslConnector::ssl_password() const
{
    return cert_pass_;
}

void SslConnector::DoAccept()
{
    std::shared_ptr<SslConnection> conn = std::make_shared<SslConnection>(io_service_, context_, *this);
    acceptor_.async_accept(conn->accept_socket(), [this, conn](boost::system::error_code ec) {
        if (!ec) {
            AddConnection(conn);
            conn->Start();
            DoAccept();
        }
    });
}

void SslConnector::Start()
{
    if (!cert_pass_.empty()) {
        context_.set_password_callback(boost::bind(&SslConnector::ssl_password, this));
    }
    if (cert_file_.empty()) {
        throw FatalException("SSL connectors must specify a cert_file parameter.");
    }
    if (!boost::filesystem::is_regular_file(boost::filesystem::path(cert_file_))) {
        throw FatalException(std::string("SSL cert_file ") + cert_file_ + " does not exist.");
    }
    context_.use_certificate_chain_file(cert_file_);
    context_.use_private_key_file(cert_file_, boost::asio::ssl::context::pem);
    //context_.use_tmp_dh_file("dh512.pem");

    DoAccept();
}

void SslConnector::set_ssl_password(const std::string &password)
{
    cert_pass_ = password;
}

void SslConnector::set_cert_file(const std::string &file)
{
    cert_file_ = file;
}

} // namespace stampmq
