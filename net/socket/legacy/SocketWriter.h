#ifndef LEGACY_SOCKETWRITER_H
#define LEGACY_SOCKETWRITER_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include <functional>
#include <stddef.h>    // for size_t
#include <sys/types.h> // for ssize_t

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

#include "socket/SocketWriter.h"
#include "socket/legacy/Socket.h"


namespace legacy {

    class SocketWriter : public ::SocketWriter<legacy::Socket> {
    protected:
        using ::SocketWriter<legacy::Socket>::SocketWriter;

        ssize_t send(const char* junk, const size_t junkSize) override;
    };

}; // namespace legacy

#endif // LEGACY_SOCKETWRITER_H
