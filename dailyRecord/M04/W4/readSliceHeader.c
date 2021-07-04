typedef struct
{
    uint8_t* start; // 指向buf头部指针
    uint8_t* p;     // 当前指针位置
    uint8_t* end;   // 指向buf尾部指针
    int bits_left;  // 当前读取字节的剩余(可用/未读)比特个数
} bs_t;

//视频解密
static void decoder_decrypt_video_frame(AVPacket *pkt) {
    uint8_t *data = pkt->data;
    int size_pkt = pkt->size;
    int size_nalu = 0;
    int end_nalu = 0;
    int index = 0;
    enum READ_TYPE{
        NALU_LENGTH = 1, 
        NALU_TYPE,
        DECRYPT_FRAME_DATA
    } read_type;
    read_type = NALU_LENGTH;
    while (index < size_pkt) {
        switch (read_type) {
            case NALU_LENGTH: //解析nalu头，获取nalu的长度
                size_nalu = 0;
                for (int i = 0; i < 4; i++) { //获取nalu长度
                    size_nalu = (int)size_nalu * 256 + (int)data[index++];
                }
                end_nalu = index + size_nalu;
                read_type = NALU_TYPE; // 跳转到读取nalu类型
                break;

            case NALU_TYPE:
                if ((data[index] & 0X1F) == 0X05 || ((data[index] & 0X1F) == 0X01 && find_slice_type(data, &index) == 2)) { // 若为IDR帧 或普通 I帧,则解密
                    index++;
                    read_type = DECRYPT_FRAME_DATA;
                } else { // 非IDR帧或I帧则跳过
                    index = end_nalu;
                    read_type = NALU_LENGTH;
                }
                break;

            case DECRYPT_FRAME_DATA: // 解密IDR帧
                ALOGD("decrypt IDR frame");
                for (; index < end_nalu; index++) {
                    data[index] = ~data[index];
                }
                read_type = NALU_LENGTH;
                break;

            default:
                break;
        }
    }
}

/**
 解析slice_header头三个句法元素
 [h264协议文档位置]：7.3.3 Slice header syntax
 */
static int find_slice_type(uint8_t *data, int *index)
{   
    int bits_left = 8;
    bs_read_ue(data, index, &bits_left); // 读first_mb_in_slice，抛弃
    
    // 因为slice_type值为0~9，0~4和5~9重合
    int slice_type = bs_read_ue(data, index, &bits_left);
    if (slice_type > 4) {slice_type -= 5;}

    bs_read_ue(data, index, &bits_left); //读第三个句法元素
    
    return slice_type;
}

/**
 ue(v) 解码
 */
static inline uint32_t bs_read_ue(uint8_t *data, int *index, int *bits_left)
{
    int32_t r = 0; // 解码得到的返回值
    int i = 0;     // leadingZeroBits
    
    // 1.计算leadingZeroBits
    while( (bs_read_u1(data, index, bits_left) == 0) && (i < 32))
    {
        i++;
    }
    // 2.计算read_bits( leadingZeroBits )
    for (int j = 0; j < i; j++) {
        // 1.每次读取1比特，并依次从高位到低位放在r中
        r |= ( bs_read_u1(data, index, bits_left) << ( i - j - 1 ) );
    }
    // 3.计算codeNum，1 << i即为2的i次幂
    r += (1 << i) - 1;
    
    return r;
}


/** 读取1个比特 */
static inline uint32_t bs_read_u1(uint8_t *data, int *index, int *bits_left)
{
    uint32_t r = 0; // 读取比特返回值
    
    // 1.剩余比特先减1
    (*bits_left)--;
    
    // 2.计算返回值
    r = ((*index) >> (*bits_left)) & 0x01;
    
    // 3.判断是否读到字节末尾，如果是指针位置移向下一字节，比特位初始为8
    if ((*bits_left) == 0) { (*index)++; (*bits_left) = 8; }
    
    return r;
}









// 原版


typedef struct
{
    uint8_t* start; // 指向buf头部指针
    uint8_t* p;     // 当前指针位置
    uint8_t* end;   // 指向buf尾部指针
    int bits_left;  // 当前读取字节的剩余(可用/未读)比特个数
} bs_t;

/**
 解析slice_header头三个句法元素
 [h264协议文档位置]：7.3.3 Slice header syntax
 */
void parse_first_three_element(bs_t *b)
{
    currentSlice->slice_header.first_mb_in_slice = bs_read_ue(b, "SH: first_mb_in_slice");
    
    // 因为slice_type值为0~9，0~4和5~9重合
    int slice_type = bs_read_ue(b, "SH: slice_type");
    if (slice_type > 4) {slice_type -= 5;}
    currentSlice->slice_header.slice_type = slice_type;
    
    currentSlice->slice_header.pic_parameter_set_id = bs_read_ue(b, "SH: pic_parameter_set_id");
}

/**
 ue(v) 解码
 */
static inline uint32_t bs_read_ue(bs_t* b, char *traceString)
{
    int32_t r = 0; // 解码得到的返回值
    int i = 0;     // leadingZeroBits
    
    // 1.计算leadingZeroBits
    while( (bs_read_u1(b) == 0) && (i < 32) && (!bs_eof(b)) )
    {
        i++;
    }
    // 2.计算read_bits( leadingZeroBits )
    r = bs_read_u(b, i, NULL);
    // 3.计算codeNum，1 << i即为2的i次幂
    r += (1 << i) - 1;
    
    if (traceString != NULL) {
#if TRACE
        traceInput(traceString, r);
#endif
    }
    return r;
}

/**
 读取n个比特
 
 @param b 比特流操作句柄
 @param n 读取多少个比特
 @return 返回读取到的值
 */
static inline uint32_t bs_read_u(bs_t* b, int n, char *traceString)
{
    uint32_t r = 0; // 读取比特返回值
    int i;  // 当前读取到的比特位索引
    for (i = 0; i < n; i++)
    {
        // 1.每次读取1比特，并依次从高位到低位放在r中
        r |= ( bs_read_u1(b) << ( n - i - 1 ) );
    }
    
    if (traceString != NULL) {
#if TRACE
        traceInput(traceString, r);
#endif
    }
    return r;
}

/** 读取1个比特 */
static inline uint32_t bs_read_u1(bs_t* b)
{
    uint32_t r = 0; // 读取比特返回值
    
    // 1.剩余比特先减1
    b->bits_left--;
    
    if (! bs_eof(b))
    {
        // 2.计算返回值
        r = ((*(b->p)) >> b->bits_left) & 0x01;
    }
    
    // 3.判断是否读到字节末尾，如果是指针位置移向下一字节，比特位初始为8
    if (b->bits_left == 0) { b->p ++; b->bits_left = 8; }
    
    return r;
}

/** 是否已读到末尾（end_of_file） */
static inline int bs_eof(bs_t* bs) { if (bs->p >= bs->end) { return 1; } else { return 0; } }