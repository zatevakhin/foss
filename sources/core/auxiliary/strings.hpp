//
// Created by zatevaxin on 29.08.17.
//

#ifndef FOSS_AUXILIARY_HPP
#define FOSS_AUXILIARY_HPP

#include <iostream>
#include <memory>


namespace aux {

    template<typename ... T>
    std::string format( const std::string& format, T ... args ) {
        ssize_t size = snprintf( nullptr, 0, format.c_str(), args ... ) + 1;
        std::unique_ptr<char[]> buf( new char[ size ] );
        snprintf( buf.get(), (size_t)size, format.c_str(), args ... );
        return std::string( buf.get(), buf.get() + size - 1 );
    }

}


#endif //FOSS_AUXILIARY_HPP
