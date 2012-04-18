#include "TCPExporter.h"
#include <cerrno>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <time.h>

namespace IPFIX {

    void TCPExporter::_sendMessage(uint8_t *base, size_t len) {
        int      rv;
        timespec ts;
        
        if (fd_ < 0) {
            fd_ = addr_.create_socket();
        }

        while ((rv = send(fd_, base, len, 0)) < 0) {
            // we failed. close.
            close(fd_);
            
            if (retry_) {
                // increase retry delay
                if (retry_delay_ * 2 <= retry_delay_max_) {
                    retry_delay_ *= 2;
                }

                // wait before retry
                ts.tv_sec = retry_delay_;
                ts.tv_nsec = 0;

                // FIXME check return, add max count, all that friendly stuff
                nanosleep(&ts, NULL);

                // try to reconnect
                fd_ = addr_.create_socket();
                
            } else {
                throw IOError(strerror(errno));
            }            
        }
        
        // we succeeded, reset retry delay
        retry_delay_ = retry_delay_min_;
    }

    TCPExporter::~TCPExporter() {
        if (fd_ >= 0) {
            flush();
            close(fd_);
            fd_ = -1;
        }

    }

}