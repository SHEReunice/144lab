#include "wrapping_integers.hh"
#include <math.h>

// Dummy implementation of a 32-bit wrapping integer

// For Lab 2, please replace with a real implementation that passes the
// automated checks run by `make check_lab2`.

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

//! Transform an "absolute" 64-bit sequence number (zero-indexed) into a WrappingInt32
//! \param n The input absolute 64-bit sequence number
//! \param isn The initial sequence number
WrappingInt32 wrap(uint64_t n, WrappingInt32 isn) {
    // DUMMY_CODE(n, isn);
    // uint32_t x;
    // uint64_t base = pow(2, 32);
    // x = n % base;
    // // WrappingInt32 seqno = operator+(isn, x);
    
    return operator+(isn, n); //直接返回32位，相当于包含了模运算，如果加起来的和超过32位，只会保留地位，去掉高位
}

//! Transform a WrappingInt32 into an "absolute" 64-bit sequence number (zero-indexed)
//! \param n The relative sequence number
//! \param isn The initial sequence number
//! \param checkpoint A recent absolute 64-bit sequence number
//! \returns the 64-bit sequence number that wraps to `n` and is closest to `checkpoint`
//!
//! \note Each of the two streams of the TCP connection has its own ISN. One stream
//! runs from the local TCPSender to the remote TCPReceiver and has one ISN,
//! and the other stream runs from the remote TCPSender to the local TCPReceiver and
//! has a different ISN.
uint64_t unwrap(WrappingInt32 n, WrappingInt32 isn, uint64_t checkpoint) {
    // DUMMY_CODE(n, isn, checkpoint);
    uint64_t base = 1ul << 32;
    uint64_t mod = checkpoint >> 32;
    uint64_t rest = WrappingInt32(n)- isn;
    uint64_t absolute_seqno_1, absolute_seqno_2, absolute_seqno_3;
    absolute_seqno_1 = rest + mod * base;
    if (mod == 0){
        absolute_seqno_2 = absolute_seqno_1;
    }else{
        absolute_seqno_2 = rest + (mod - 1) * base;
    }
    absolute_seqno_3 = rest + (mod + 1) * base;

    uint64_t abs_1 = absolute_seqno_1 > checkpoint ? absolute_seqno_1 - checkpoint : checkpoint - absolute_seqno_1;
    uint64_t abs_2 = absolute_seqno_2 > checkpoint ? absolute_seqno_2 - checkpoint : checkpoint - absolute_seqno_2;
    uint64_t abs_3 = absolute_seqno_3 > checkpoint ? absolute_seqno_3 - checkpoint : checkpoint - absolute_seqno_3;
    if (abs_1 <= abs_2 && abs_1 <= abs_3) return absolute_seqno_1;
    else if(abs_2 <= abs_1 && abs_2 <= abs_3) return absolute_seqno_2;
    else return absolute_seqno_3;

    
}
