#ifndef FOSS_TRACE_HPP
#define FOSS_TRACE_HPP

#include <iostream>
#include <sstream>
#include <iomanip>
#include <chrono>


namespace 
{
    const std::string timestamp()
    {
        auto now = std::chrono::system_clock::now();
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
        auto tt = std::chrono::system_clock::to_time_t(now);

        std::stringstream ss;
        ss << std::put_time(std::localtime(&tt), "%Y-%m-%d %X");
        ss << '.' << std::setfill('0') << std::setw(3) << ms.count();
        return std::move(ss.str());
    }

}


#define trc_warning(M, ...) fprintf(stdout, "[\e[1;33mW\e[0m] %s :: " M "\n", timestamp().c_str(), ##__VA_ARGS__)
#define trc_error(M, ...)   fprintf(stdout, "[\e[1;31mE\e[0m] %s :: " M "\n", timestamp().c_str(), ##__VA_ARGS__)
#define trc_log(M, ...)     fprintf(stdout, "[\e[1;32mL\e[0m] %s :: " M "\n", timestamp().c_str(), ##__VA_ARGS__)
#define trc_debug(M, ...)   fprintf(stdout, "[\e[1;34mD\e[0m] %s :: " M "\n", timestamp().c_str(), ##__VA_ARGS__)


#define TRC_VEC3(v) fprintf(stdout,"(%8.4f, %8.4f, %8.4f)\n", v.x, v.y, v.z)
#define TRC_VEC4(v) fprintf(stdout,"(%8.4f, %8.4f, %8.4f)\n", v.x, v.y, v.z, v.w)
#define MTX_ROW(m) fprintf(stdout,"[%8.4f, %8.4f, %8.4f, %8.4f]\n", m[0], m[1], m[2], m[3])
#define PRINT_MTX4x4(mtx) fprintf(stdout, #mtx "\n"); MTX_ROW(mtx[0]); MTX_ROW(mtx[1]); MTX_ROW(mtx[2]); MTX_ROW(mtx[3])

#endif //FOSS_TRACE_HPP
