﻿/* Copyright (C) 2013 Interactive Brokers LLC. All rights reserved. This code is subject to the terms
 * and conditions of the IB API Non-Commercial License or the IB API Commercial License, as applicable. */

#pragma once

#include "StdAfx.h"
#include "EDecoder.h"
#include "EMutex.h"
#include "EReaderOSSignal.h"

class EClientSocket;
class EReaderSignal;
class EMessage;

class TWSAPIDLLEXP EReader
{  
    EClientSocket *m_pClientSocket;
    EReaderSignal *m_pEReaderSignal;
    EDecoder processMsgsDecoder_;
    std::deque<shared_ptr<EMessage>> m_msgQueue;
    EMutex m_csMsgQueue;
    std::vector<char> m_buf;
	EDecoder threadReadDecoder_;
    bool m_needsWriteSelect;
    bool m_isAlive;
    HANDLE m_hReadThread;

	void onReceive();
	void onSend();
	bool bufferedRead(char *buf, int size);

public:
    EReader(EClientSocket *clientSocket, EReaderSignal *signal);
    ~EReader(void);

protected:
	bool processNonBlockingSelect();
    shared_ptr<EMessage> getMsg(void);
    void readToQueue();
#if defined(IB_POSIX)
    static void * readToQueueThread(void * lpParam);
#elif defined(IB_WIN32)
    static DWORD WINAPI readToQueueThread(LPVOID lpParam);
#else
#   error "Not implemented on this platform"
#endif
    
    EMessage * readSingleMsg();

public:
    void processMsgs(void);
    void checkClient();

private:
	void start();
};

