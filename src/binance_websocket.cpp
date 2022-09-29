#include "../include/binance_websocket.h"
#include "../include/binance_logger.h"

map <struct lws *,CB> binance_websocket::handles ;



//--------------------------
int
binance_websocket::event_cb( struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len )
{

    switch( reason )
    {
        case LWS_CALLBACK_CLIENT_ESTABLISHED:
            lws_callback_on_writable( wsi );
            break;

        case LWS_CALLBACK_CLIENT_RECEIVE:

            /* Handle incomming messages here. */
            try {

                //binance_logger::write_log("%p %s",  wsi, (char *)in );

                string str_result = string( (char*)in );
                Json::Reader reader;
                Json::Value json_result;
                reader.parse( str_result , json_result );

                if ( handles.find( wsi ) != handles.end() ) {
                    handles[wsi]( json_result );
                }

            } catch ( exception &e ) {
                binance_logger::write_log( "<binance_websocket::event_cb> Error ! %s", e.what() );
            }
            break;

        case LWS_CALLBACK_CLIENT_WRITEABLE:
        {
            break;
        }

        case LWS_CALLBACK_CLOSED:
        case LWS_CALLBACK_CLIENT_CONNECTION_ERROR:
            if ( handles.find( wsi ) != handles.end() ) {
                handles.erase(wsi);
            }
            break;

        default:
            break;
    }

    return 0;
}


//-------------------
void
binance_websocket::init( )
{
    unique_ptr<lws_context_creation_info> info = make_unique<lws_context_creation_info>();
    info->port = CONTEXT_PORT_NO_LISTEN;
    info->protocols = &(this->protocols[0]);
    info->gid = -1;
    info->uid = -1;
    info->options |= LWS_SERVER_OPTION_DO_SSL_GLOBAL_INIT;
    context = lws_create_context(info.release());

}


//----------------------------
// Register call backs
void
binance_websocket::connect_endpoint (CB cb, const string& path)
{
    /* Connect if we are not connected to the server. */
    unique_ptr<lws_client_connect_info> ccinfo = make_unique<lws_client_connect_info>();
    //struct lws_client_connect_info ccinfo = {0};
    ccinfo->context 	= context;
    ccinfo->address 	= BINANCE_WS_HOST;
    ccinfo->port 	= BINANCE_WS_PORT;
    ccinfo->path 	= path.c_str();
    ccinfo->host 	= lws_canonical_hostname( context );
    ccinfo->origin 	= "origin";
    ccinfo->protocol = protocols[0].name;
    ccinfo->ssl_connection = LCCSCF_USE_SSL | LCCSCF_ALLOW_SELFSIGNED | LCCSCF_SKIP_SERVER_CERT_HOSTNAME_CHECK;

    struct lws* conn = lws_client_connect_via_info(ccinfo.release());
    handles[conn] = cb;

}


//----------------------------
// Entering event loop
void
binance_websocket::enter_event_loop()
{
    while( 1 )
    {
        try {
            lws_service( context, 500 );
        } catch ( exception &e ) {
            binance_logger::write_log( "<binance_websocket::enter_event_loop> Error ! %s", e.what() );
            break;
        }
    }
    lws_context_destroy( context );
}

binance_websocket::binance_websocket() {
    context = nullptr;
    protocols.push_back( {
                                       "websocket",
                                       binance_websocket::event_cb,
                                       0,
                                       65536,
                               });
    protocols.push_back({ nullptr, nullptr, 0, 0 });
}

binance_websocket::~binance_websocket() {
    if(context)
        lws_context_destroy( context );
    context = nullptr;
    binance_logger::close_file();
}


