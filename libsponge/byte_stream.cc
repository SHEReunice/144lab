#include "byte_stream.hh"

#include <algorithm>
#include <iterator>
#include <stdexcept>

// Dummy implementation of a flow-controlled in-memory byte stream.

// For Lab 0, please replace with a real implementation that passes the
// automated checks run by `make check_lab0`.

// You will need to add private members to the class declaration in `byte_stream.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

ByteStream::ByteStream(const size_t capacity) : buffers(), buffer_capacity(capacity), written_bytes(0), read_bytes(0), write_end(false), read_end(false) {}

size_t ByteStream::write(const string &data) {
    size_t canWrite = buffer_capacity - buffers.size();
    size_t realWritten = min(canWrite, data.length());
    for(size_t i = 0; i < realWritten; i++){
        buffers.push_back(data[i]);
    }
    written_bytes += realWritten;
    return realWritten;
}

//! \param[in] len bytes will be copied from the output side of the buffer
string ByteStream::peek_output(const size_t len) const {
    size_t canRead = min(len, buffers.size());
    string output = "";
    for(size_t i = 0; i < canRead; i++){
        output += buffers[i];
    }
    return output;
}

//! \param[in] len bytes will be removed from the output side of the buffer
void ByteStream::pop_output(const size_t len) { 
    if(len > buffers.size()){
        set_error();
        return;
    }
    for(size_t i = 0; i < len; i++){
        buffers.pop_front();
    }
    read_bytes += len;
 }

void ByteStream::end_input() {write_end = true;}

bool ByteStream::input_ended() const { return write_end; }

size_t ByteStream::buffer_size() const { return buffers.size(); }

bool ByteStream::buffer_empty() const { return buffers.empty(); }

bool ByteStream::eof() const { return buffers.empty() && write_end; }

size_t ByteStream::bytes_written() const { return written_bytes; }

size_t ByteStream::bytes_read() const { return read_bytes; }

size_t ByteStream::remaining_capacity() const { return buffer_capacity - buffers.size(); }
