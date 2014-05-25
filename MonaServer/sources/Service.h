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

#pragma once

#include "Mona/FileWatcher.h"
#include "Mona/Expirable.h"
#include "Script.h"

class Service;
class ServiceHandler {
public:
	virtual void startService(Service& service){}
	virtual void stopService(Service& service){}
	virtual const std::string& wwwPath() = 0;
};


class Service : public Mona::FileWatcher, public Mona::Expirable<Service> {
public:
	Service(lua_State* pState, ServiceHandler& handler);
	virtual ~Service();

	
	Service*			open(const std::string& path, Mona::Expirable<Service>& expirableService);
	void				open() {open(true); lua_pop(_pState, 1); }

	const std::string	path;
	const std::string	lastError;

	static int	Item(lua_State *pState);

private:
	Service(lua_State* pState, const std::string& path, ServiceHandler& handler);

	Service*	open(Mona::Expirable<Service>& expirableService,const std::string& path);
	bool		open(bool create);
	void		close(bool full);

	void		loadFile();
	void		clearFile() { close(false); }
	void		clearEnvironment();

	static int	Index(lua_State* pState);

	bool						_loaded;
	bool						_created;
	lua_State*					_pState;
	std::vector<std::string>	_packages;

	std::map<std::string,Service*>	_services;
	ServiceHandler&					_handler;
};
