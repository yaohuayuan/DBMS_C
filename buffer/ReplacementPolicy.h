#ifndef DBMS_REPLACEMENT_POLICY_H
#define DBMS_REPLACEMENT_POLICY_H

typedef struct ReplacementPolicy {
    void* impl;                               ///< 具体替换策略的实现指针

    /**
     * 记录对指定帧的访问。
     *
     * @param impl 具体替换策略的实现指针。
     * @param frame_id 被访问的帧ID。
     */
    void (*record_access)(void* impl, int frame_id);
    
    /**
     * 选择一个要淘汰的帧。
     *
     * @param impl 具体替换策略的实现指针。
     * @return 要淘汰的帧ID。
     */
    int  (*evict)(void* impl);
    
    /**
     * 从替换策略中移除指定帧。
     *
     * @param impl 具体替换策略的实现指针。
     * @param frame_id 要移除的帧ID。
     */
    void (*remove)(void* impl, int frame_id);
    
    /**
     * 销毁替换策略实现。
     *
     * @param impl 具体替换策略的实现指针。
     */
    void (*destroy)(void* impl);
} ReplacementPolicy;

#endif
