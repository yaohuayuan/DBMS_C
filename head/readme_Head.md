# 该目录下结构
1. ByteBuffer
## ByteBuffer
这个是自己实现的一个ByteBuffer,结构体如下所示
```c
typedef struct
{
    uint64_t size;
    uint8_t* data;
    uint64_t position;
    uint64_t limit;
}ByteBuffer;
```
详细见readme_ByteBuffer.md