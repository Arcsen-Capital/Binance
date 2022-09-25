#include "../include/binance_logger.h"



int    binance_logger::debug_level   = 1;
string binance_logger::debug_log_file = "/tmp/binawatch.log";
int    binance_logger::debug_log_file_enable = 0;
FILE  *binance_logger::log_fp = nullptr;



//-----------------------------------------------
void
binance_logger::write_log(basic_string<char, char_traits<char>, allocator<char>> fmt, ... )
{
    if ( debug_level == 0 ) {
        return;
    }
    if ( debug_log_file_enable == 1 ) {
        open_logfp_if_not_opened();
    }

    va_list arg;

    char new_fmt[1024];

    struct timeval tv;
    gettimeofday(&tv, nullptr);
    time_t t = tv.tv_sec;
    struct tm * now = localtime( &t );


    sprintf( new_fmt , "%04d-%02d-%02d %02d:%02d:%02d %06ld :%s\n" , now->tm_year + 1900, now->tm_mon + 1, now->tm_mday, now->tm_hour, now->tm_min, now->tm_sec , tv.tv_usec , fmt.c_str() );

    va_start (arg, fmt);

    if ( debug_log_file_enable && log_fp ) {
        vfprintf( log_fp , new_fmt ,arg );
        fflush(log_fp);
    } else {
        vfprintf ( stdout, new_fmt, arg);
        fflush(stdout);
    }

    va_end (arg);


}



//-----------------------------------------------
// Write log to channel without any timestamp nor new line
void
binance_logger::write_log_clean( const char *fmt, ... )
{
    if ( debug_level == 0 ) {
        return;
    }
    if ( debug_log_file_enable == 1 ) {
        open_logfp_if_not_opened();
    }

    va_list arg;
    va_start (arg, fmt);

    if ( debug_log_file_enable && log_fp ) {
        vfprintf( log_fp , fmt ,arg );
        fflush(log_fp);
    } else {
        vfprintf ( stdout, fmt, arg);
        fflush(stdout);
    }
    va_end (arg);

}


//---------------------
void
binance_logger::open_logfp_if_not_opened() {

    if ( debug_log_file_enable && log_fp == NULL ) {

        log_fp = fopen( debug_log_file.c_str() , "a" );

        if ( log_fp ) {
            printf("log file in %s\n", debug_log_file.c_str());
        } else {
            printf("Failed to open log file.\n" );
        }
    }

}


//---------------------
void
binance_logger::set_debug_level( int level )
{
    debug_level = level;
}

//--------------------
void
binance_logger::set_debug_logfile( string &pDebug_log_file )
{
    debug_log_file = pDebug_log_file;
}

//--------------------
void
binance_logger::enable_logfile( int pDebug_log_file_enable )
{
    debug_log_file_enable = pDebug_log_file_enable;
}

void binance_logger::close_file() {
    if ( log_fp ) {
        fclose(log_fp);
    }

}









