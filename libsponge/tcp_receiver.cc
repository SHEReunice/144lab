#include "tcp_receiver.hh"

// Dummy implementation of a TCP receiver

// For Lab 2, please replace with a real implementation that passes the
// automated checks run by `make check_lab2`.

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

bool TCPReceiver::segment_received(const TCPSegment &seg) {
    // DUMMY_CODE(seg);
    bool eof = false;
    uint64_t index = 0;;
    bool receive_flag = false;
    string data = seg.payload().copy();
    uint64_t checkpoint = _reassembler.stream_out().bytes_written();
    if(!isn_flag && !seg.header().syn){ return false;} //就是之前没收到过syn，这一次也没有syn
    //第一个携带syn标志的到达段的序列号为初始序列号
    if(seg.header().syn){
        if(isn_flag) return false; //重复的syn
        isn = seg.header().seqno;
        isn_flag = true;
        index = unwrap(isn+1, isn, 0); //isn不写进去
        if(data.length() == 0){receive_flag = true;}
            
    }else{ //之前收到过syn，这一次不携带syn
        // uint64_t checkpoint = _reassembler.stream_out().bytes_written();
        // uint64_t n=_reassembler.stream_out().bytes_written();
        index = unwrap(seg.header().seqno, isn, checkpoint);
        
        // if(!(index-1 >= n+window_size() || index-1+data.length() <= n)){ receive_flag = true;} //如果接收的payload超过了window或者payload是空的 都不要 
    }
    if(seg.header().fin){
        if(fin_flag) return false;
            fin_flag = true;
            eof = true;
            receive_flag = true;

    // }else if(data.length() == 0 && index-1 == checkpoint){
    //     return true;
    }else if(index-1 >= checkpoint+window_size() || index-1 + data.length() <= checkpoint){
        if(!seg.header().syn) return false;
    }else{
        receive_flag = true;
    }
        
    _reassembler.push_substring(data, index-1, eof);
    return receive_flag;
}

optional<WrappingInt32> TCPReceiver::ackno() const {
    if(!isn_flag) {return nullopt;}
    uint64_t ack = _reassembler.stream_out().bytes_written()+1; //加上一个syn标志
    if(_reassembler.stream_out().input_ended() && fin_flag) { ack++;} //加上一个fin标志
    return wrap(ack, isn);
}

size_t TCPReceiver::window_size() const { return _capacity-_reassembler.stream_out().buffer_size(); }


