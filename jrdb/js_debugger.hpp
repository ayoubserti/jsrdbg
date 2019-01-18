/*
 * A Remote Debugger Client for SpiderMonkey Java Script Engine.
 * Copyright (C) 2014-2015 Slawomir Wojtasiak
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef SRC_JS_DEBUGGER_HPP_
#define SRC_JS_DEBUGGER_HPP_

#include <string>

#include <encoding.hpp>

#include "debugger.hpp"
#include <log.hpp>

#include <jsapi.h>
//#include <jsdbgapi.h>

class JSDebugger : public DebuggerEngine {
public:
    JSDebugger( DebuggerCtx &ctx );
    virtual ~JSDebugger();
    // DebuggerEngine
    int init();
    int destroy();
    int sendCtrlCommand( const std::string &command );
    int sendCommand( const DebuggerCommand &command );
    DebuggerCtx &getDebuggerCtx();
    DebuggerCtx &getContext();
private:
    JSRuntime *_rt;
    JSContext *_cx;
    JS::Heap<JSObject*> _debugger;
    JSObject *_global;
    Utils::Logger &_log;
};

#endif /* SRC_JS_DEBUGGER_HPP_ */
