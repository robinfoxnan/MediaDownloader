// 2022-11-16����Ҫlog4zʹ���ڴ�����в���̫�������滻һ��
#pragma once
#ifndef __common_log_h
#define __common_log_h
#include <thread>
#include <string>
#include "./spdlog/logger.h"
#include "./spdlog/sinks/rotating_file_sink.h"
#include "./spdlog/sinks/stdout_color_sinks.h"

extern std::shared_ptr<spdlog::logger> logger;

// ��ջ�ϸ�ʽ���������С������Ŀ����������
#define MSG_BUF_LEN  4096

#define  LOGFMTW(...) { if (logger && logger->level() <= SPDLOG_LEVEL_WARN){  \
char buffer[MSG_BUF_LEN]; int len = snprintf(buffer, MSG_BUF_LEN, __VA_ARGS__); \
if (len < MSG_BUF_LEN) logger->warn(buffer);\
else { char * buf = new char[len + 2]; snprintf(buf, len + 2, __VA_ARGS__); logger->warn(buf);delete[] buf;} } }

#define  LOGFMTD(...) { if (logger && logger->level() <= SPDLOG_LEVEL_DEBUG) { \
char buffer[MSG_BUF_LEN]; int len = snprintf(buffer, MSG_BUF_LEN, __VA_ARGS__); \
if (len < MSG_BUF_LEN) logger->debug(buffer);\
else { char * buf = new char[len + 2]; snprintf(buf, len + 2, __VA_ARGS__); logger->debug(buf);delete[] buf;} } }

#define  LOGFMTE(...) { if (logger && logger->level() <= SPDLOG_LEVEL_ERROR) { \
char buffer[MSG_BUF_LEN]; int len = snprintf(buffer, MSG_BUF_LEN, __VA_ARGS__); \
if (len < MSG_BUF_LEN) logger->error(buffer);\
else { char * buf = new char[len + 2]; snprintf(buf, len + 2, __VA_ARGS__); logger->error(buf);delete[] buf;} } }

#define LOGFMTF(...) { if (logger && logger->level() <= SPDLOG_LEVEL_CRITICAL) { \
char buffer[MSG_BUF_LEN]; int len = snprintf(buffer, MSG_BUF_LEN, __VA_ARGS__); \
if (len < MSG_BUF_LEN) logger->error(buffer);\
else { char * buf = new char[len + 2]; snprintf(buf, len + 2, __VA_ARGS__); logger->error(buf);delete[] buf;} } }

#endif


