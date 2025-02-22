/**-----------------------------------------------------------------------------

 @file    klog.h
 @brief   Definition of kernel log related functions
 @details
 @verbatim

  A kernel-level log system was implemented. As the first step, it
  mainly supports information display.

 @endverbatim

 **-----------------------------------------------------------------------------
 */
#pragma once

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

#include <device/display/term.h>
#include <base/lock.h>

#define KLOG_BUFFER_SIZE       (UINT16_MAX + 1)

typedef enum {
    KLOG_LEVEL_VERBOSE,
    KLOG_LEVEL_DEBUG,
    KLOG_LEVEL_INFO,
    KLOG_LEVEL_WARN,
    KLOG_LEVEL_ERROR,
    KLOG_LEVEL_UNK
} klog_level_t;

typedef struct {
    uint8_t buff[KLOG_BUFFER_SIZE];
    int start, end;
    term_info_t* term;
} klog_info_t;

void klog_init(void);
void klog_vprintf(klog_level_t level, const char*, ...);
void klog_refresh(int mode);
void klog_debug(void);
void klog_lock(void);
void klog_unlock(void);
void kprintf(const char*, ...);

#define klogv(s, ...)       klog_vprintf(KLOG_LEVEL_VERBOSE, s, ##__VA_ARGS__)
#define klogd(s, ...)       klog_vprintf(KLOG_LEVEL_DEBUG, s, ##__VA_ARGS__)
#define klogi(s, ...)       klog_vprintf(KLOG_LEVEL_INFO, s,##__VA_ARGS__)
#define klogw(s, ...)       klog_vprintf(KLOG_LEVEL_WARN, s, ##__VA_ARGS__)
#define kloge(s, ...)       klog_vprintf(KLOG_LEVEL_ERROR, s, ##__VA_ARGS__)
#define klogu(s, ...)       klog_vprintf(KLOG_LEVEL_UNK, s, ##__VA_ARGS__)

#define klog_printf(s, ...) klog_vprintf(KLOG_LEVEL_INFO, s, ##__VA_ARGS__)
