//
// Created by james on 24/09/22.
//

#ifndef UNTITLED1_BINANCE_WEBSOCKET_H
#define UNTITLED1_BINANCE_WEBSOCKET_H


#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdarg>

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <exception>

#include <jsoncpp/json/json.h>
#include <libwebsockets.h>


#define BINANCE_WS_HOST "stream.binance.com"
#define BINANCE_WS_PORT 9443


using namespace std;

typedef int (*CB)(Json::Value &json_value );


class binance_websocket {

private:
    struct lws_context *context;
    vector<struct lws_protocols> protocols;

    static map <struct lws *,CB> handles ;
    static int  event_cb( struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len );

public:
    binance_websocket();
    ~binance_websocket();
    void connect_endpoint(CB user_cb, const string& path);
    void init();
    void enter_event_loop();

};

#endif //UNTITLED1_BINANCE_WEBSOCKET_H
