/*
Copyright 2014 Mona
mathieu.poux[a]gmail.com
jammetthomas[a]gmail.com

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License received along this program for more
details (or else see http://www.gnu.org/licenses/).

This file is a part of Mona.
*/

#include "LUAServer.h"

using namespace std;
using namespace Mona;


int LUAServer::Send(lua_State* pState) {
	SCRIPT_CALLBACK(ServerConnection,server)
		const char* handler(SCRIPT_READ_STRING(NULL));
		if(!handler || strcmp(handler,".")==0) {
			ERROR("Invalid '",handler,"' handler for the sending server message")
		} else {
			shared_ptr<ServerMessage> pMessage(new ServerMessage(handler,server.poolBuffers()));
			SCRIPT_READ_DATA(*pMessage)
			server.send(pMessage);
		}
	SCRIPT_CALLBACK_RETURN
}

int LUAServer::Reject(lua_State* pState) {
	SCRIPT_CALLBACK(ServerConnection,server)
		server.reject(SCRIPT_READ_STRING("unknown error"));
	SCRIPT_CALLBACK_RETURN
}




int LUAServer::Get(lua_State* pState) {
	SCRIPT_CALLBACK(ServerConnection,server)
		const char* name = SCRIPT_READ_STRING(NULL);
		if(name) {
			if(strcmp(name,"send")==0) {
				SCRIPT_WRITE_FUNCTION(LUAServer::Send)
				SCRIPT_CALLBACK_FIX_INDEX(name)
			} else if(strcmp(name,"isTarget")==0) {
				SCRIPT_WRITE_BOOL(server.isTarget)
				SCRIPT_CALLBACK_FIX_INDEX(name)
			} else if(strcmp(name,"address")==0) {
				SCRIPT_WRITE_STRING(server.address.toString().c_str())
				SCRIPT_CALLBACK_FIX_INDEX(name)
			} else if (strcmp(name,"reject")==0) {
				SCRIPT_WRITE_FUNCTION(LUAServer::Reject)
				SCRIPT_CALLBACK_FIX_INDEX(name)
			} else if (strcmp(name,"configs")==0) {
				if(Script::Collection(pState, 1, "configs")) {
					for (auto& it : server)
						Script::PushKeyValue(pState, it.first, it.second);
					Script::FillCollection(pState, server.count());
				}
				SCRIPT_CALLBACK_FIX_INDEX(name)
			} else {
				Script::Collection(pState,1, "pState");
				lua_getfield(pState, -1, name);
				lua_replace(pState, -2);
				SCRIPT_CALLBACK_FIX_INDEX(name)
			}
		}
	SCRIPT_CALLBACK_RETURN
}

int LUAServer::Set(lua_State* pState) {
	SCRIPT_CALLBACK(ServerConnection,server)
		lua_rawset(pState,1); // consumes key and value
	SCRIPT_CALLBACK_RETURN
}
