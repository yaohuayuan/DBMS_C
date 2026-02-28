#ifndef NEWDBMS_RWLOCK_H
#define NEWDBMS_RWLOCK_H

#ifdef _WIN32
#include <windows.h>
typedef SRWLOCK RWLock;
#define RWLOCK_INIT SRWLOCK_INIT

/**
 * 初始化读写锁。
 *
 * @param lock 指向要初始化的 RWLock 结构体的指针。
 */
static inline void RwLockInit(RWLock* lock) {
    InitializeSRWLock(lock);
}

/**
 * 获取读锁。
 *
 * @param lock 指向 RWLock 结构体的指针。
 */
static inline void RwLockReadLock(RWLock* lock) {
    AcquireSRWLockShared(lock);
}

/**
 * 获取写锁。
 *
 * @param lock 指向 RWLock 结构体的指针。
 */
static inline void RwLockWriteLock(RWLock* lock) {
    AcquireSRWLockExclusive(lock);
}

/**
 * 释放读锁。
 *
 * @param lock 指向 RWLock 结构体的指针。
 */
static inline void RwLockUnlockRead(RWLock* lock) {
    ReleaseSRWLockShared(lock);
}

/**
 * 释放写锁。
 *
 * @param lock 指向 RWLock 结构体的指针。
 */
static inline void RwLockUnlockWrite(RWLock* lock) {
    ReleaseSRWLockExclusive(lock);
}

/**
 * 销毁读写锁。
 *
 * @param lock 指向要销毁的 RWLock 结构体的指针。
 */
static inline void RwLockDestroy(RWLock* lock) {
    (void)lock;  // No-op on Windows
}

#else
#include <pthread.h>
typedef pthread_rwlock_t RWLock;
#define RWLOCK_INIT PTHREAD_RWLOCK_INITIALIZER

/**
 * 初始化读写锁。
 *
 * @param lock 指向要初始化的 RWLock 结构体的指针。
 */
static inline void RwLockInit(RWLock* lock) { 
    pthread_rwlock_init(lock, NULL); 
}

/**
 * 获取读锁。
 *
 * @param lock 指向 RWLock 结构体的指针。
 */
static inline void RwLockReadLock(RWLock* lock) { 
    pthread_rwlock_rdlock(lock); 
}

/**
 * 获取写锁。
 *
 * @param lock 指向 RWLock 结构体的指针。
 */
static inline void RwLockWriteLock(RWLock* lock) { 
    pthread_rwlock_wrlock(lock); 
}

/**
 * 释放读锁。
 *
 * @param lock 指向 RWLock 结构体的指针。
 */
static inline void RwLockUnlockRead(RWLock* lock) { 
    pthread_rwlock_unlock(lock); 
}

/**
 * 释放写锁。
 *
 * @param lock 指向 RWLock 结构体的指针。
 */
static inline void RwLockUnlockWrite(RWLock* lock) { 
    pthread_rwlock_unlock(lock); 
}

/**
 * 销毁读写锁。
 *
 * @param lock 指向要销毁的 RWLock 结构体的指针。
 */
static inline void RwLockDestroy(RWLock* lock) { 
    pthread_rwlock_destroy(lock); 
}

#endif

#endif //NEWDBMS_RWLOCK_H