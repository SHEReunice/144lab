#include "stream_reassembler.hh"

// Dummy implementation of a stream reassembler.

// For Lab 1, please replace with a real implementation that passes the
// automated checks run by `make check_lab1`.

// You will need to add private members to the class declaration in `stream_reassembler.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

StreamReassembler::StreamReassembler(const size_t capacity) : _buffer(capacity, '\0'), _flag(capacity, false), _output(capacity), _capacity(capacity) {}

//! \details This function accepts a substring (aka a segment) of bytes,
//! possibly out-of-order, from the logical stream, and assembles any newly
//! contiguous substrings and writes them into the output stream in order.
void StreamReassembler::push_substring(const string &data, const size_t index, const bool eof) {
    // DUMMY_CODE(data, index, eof);
    size_t first_unassemble = _output.bytes_written();
    // size_t first_unaccept = first_unassemble  + _capacity - _output.buffer_size();
    size_t first_unaccept = first_unassemble + _capacity;  

    if (index >= first_unaccept || index+data.length() < first_unassemble) { return; }

   size_t begin =  index;
   size_t end = index + data.length();
   begin = (begin<first_unassemble) ?  first_unassemble : begin;
   end = (end>=first_unaccept) ? first_unaccept : end;

   for(size_t i = begin; i < end; i++){
        if(!_flag[i-first_unassemble]){
            _buffer[i-first_unassemble] = data[i-index];
            _flag[i-first_unassemble] = true;
            _unassembled_bytes++;
        }
   }

        string wating = "";
        while(_flag.front()){
            wating += _buffer.front();
            _buffer.pop_front();
            _flag.pop_front();

            _buffer.emplace_back('\0');
            _flag.emplace_back(false);
        }

        if(wating.length() > 0){
            stream_out().write(wating);
            _unassembled_bytes -= wating.length();
        }

        // if(eof && _unassembled_bytes==0){
        //     _output.end_input();
        // }

        if(eof){ 
        is_end = true;
        end_index = end;
         }
        if(is_end &&  _unassembled_bytes==0){
        _output.end_input();
        }
   


}

size_t StreamReassembler::unassembled_bytes() const { return _unassembled_bytes; }

bool StreamReassembler::empty() const { return _unassembled_bytes == 0; }
