/*
 * A Remote Debugger for SpiderMonkey Java Script engine.
 * Copyright (C) 2014-2015 Sławomir Wojtasiak
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef PUBLIC_JSLDBG_H_
#define PUBLIC_JSLDBG_H_

#include <jsapi.h>
#include <jsdbgapi.h>
#include "jsdbg_common.h"

#include <string>
#include <stdint.h>

namespace JSR {

/**
 * Local implementation of the debugger. One of the most important
 * things you have to take into account here is that all whole component
 * has to be accessed using the same thread that is used to run java
 * script engine, so if you need to handle debugging using different
 * thread it's up to you to provide correct implementation basin on
 * this simple interface. See js_debugger.cpp file for example
 * implementation.
 */
class JSRDBG_API JSLocalDebugger {
public:
    /**
     * Creates a debugger instance for given context. Remember
     * that calling this constructor is not enough to install
     * the debugger inside a JS runtime. You have to use 'install
     * method to do it correctly.
     * @param ctx JS Context.
     * @param options Options dedicated to the debugger engine.
     */
    JSLocalDebugger( JSContext *ctx, JSDbgEngineOptions &options );
    JSLocalDebugger( JSContext *ctx );
    /**
     * Deletes the debugger instance.
     */
    virtual ~JSLocalDebugger();
public:
    /**
     * Installs debugger instance into provided JS context.
     *
     * @return Unified error code.
     */
    virtual int install();
    /**
     * Uninstall debugger instance.
     *
     * @return Unified error code.
     */
    virtual int uninstall();
    /**
     * Removes global object from a debugger.
     * This method have to be called from the JS engine thread.
     * @param debuggee The debuggee instance.
     * @return Error code.
     */
    virtual int removeDebuggee( JS::HandleObject debuggee );
    /**
     * Adds new debuggee object to a debugger. Every global new
     * object should be make visible for the debugger using this method.
     * New global objects are not registered automatically.
     * This method have to be called from the JS engine thread.
     * @param debuggee The debuggee instance.
     * @return Error code.
     */
    virtual int addDebuggee( JS::HandleObject debuggee );
    /**
     * Sends command to the installed debugger instance.
     *
     * @param command[in] Command to be send to the debugger.
     * @param hint[out] Debugger state after executing the command.
     * @return True if command has been called successfully; otherwise false.
     */
    virtual bool sendCommand( const std::string &command, DebuggerStateHint &hint );
    /**
     * Called by debugger when debugged program enters pause state.
     * @param suspended True if debugger is paused due to the 'suspended' engine option.
     * @return Returns false if debugger is not installed/initialized or has been interrupted.
     */
    virtual bool handlePause( bool suspended ) = 0;
    /**
     * Handles commands generated by debugger engine.
     *
     * @return If false is returned, then exception is throw directly into the debugger code,
     *         in order to inform it about the problem. Anyway this exception is only logged
     *         and then silently ignored, so it cannot break the debugger.
     */
    virtual bool handleCommand( const std::string &command ) = 0;
    /**
     * Handles requests for loading external scripts. There requests
     * should be very rare because JS engine is aware of all source
     * code so it's more like as just in case method. Anyway source code
     * storing can be disabled on the engine level using appropriate
     * SourcePolicy option and in such a case this method is rather
     * mandatory if we would like to see any source code while debugging.
     *
     * @param file Name of the script file.
     * @param[out] outScript Loaded script source code going there.
     *
     * @return Unified error code.
     */
    virtual int loadScript( const std::string &file, std::string &outScript );
    /**
     * Gets context for which debugger instance was registered.
     * @return JSContext.
     */
    JSContext *getCtx();
private:
    void *_impl;
};

}

#endif /* PUBLIC_JSLDBG_H_ */
