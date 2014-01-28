bin_PROGRAMS = stampmq

stampmq_CPPFLAGS = $(BOOST_CPPFLAGS) $(LOG4CXX_CFLAGS) $(YAMLCPP_CFLAGS) -std=c++11 -Wall -g
stampmq_LDFLAGS = $(BOOST_THREAD_LIBS) $(BOOST_SYSTEM_LIBS) $(BOOST_REGEX_LIBS) $(BOOST_PROGRAM_OPTIONS_LIBS) $(BOOST_FILESYSTEM_LIBS) $(LOG4CXX_LIBS) $(YAMLCPP_LIBS) -lssl -lcrypto
stampmq_SOURCES = connection.cpp  connector.cpp  main.cpp  server.cpp  principal.cpp frame.cpp connect_frame.cpp \
              connected_frame.cpp unknown_frame.cpp frame_factory.cpp destination.cpp topic.cpp queue.cpp \
              hub.cpp destination_factory.cpp send_frame.cpp subscribe_frame.cpp unsubscribe_frame.cpp \
              error_frame.cpp disconnect_frame.cpp authenticator.cpp config_auth.cpp config.cpp \
              fatal_exception.cpp connection_exception.cpp acl.cpp acl_chain.cpp policy.cpp \
              yaml_configurator.cpp thread_pool.cpp ack_frame.cpp plain_connector.cpp ssl_connector.cpp \
              ssl_connection.cpp plain_socket.cpp ssl_socket.cpp socket.cpp plain_connection.cpp \
              subscription.cpp message_delivery.cpp frame_delivery.cpp 
