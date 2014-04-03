//
//  IRC_Plugin.h
//  IRCd
//
//  Created by Alex Turner on 4/2/14.
//  Copyright (c) 2014 EvilZone. All rights reserved.
//

#ifndef __IRCd__IRC_Plugin__
#define __IRCd__IRC_Plugin__

#include <memory>

class IRC_Plugin
{
public:
	IRC_Plugin(void* handle) { m_handle = handle; }
	virtual ~IRC_Plugin();

private:
	void* m_handle;
};

#endif /* defined(__IRCd__IRC_Plugin__) */
