/* Copyright (C) 2001 by First Peer, Inc. All rights reserved.
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions
** are met:
** 1. Redistributions of source code must retain the above copyright
**    notice, this list of conditions and the following disclaimer.
** 2. Redistributions in binary form must reproduce the above copyright
**    notice, this list of conditions and the following disclaimer in the
**    documentation and/or other materials provided with the distribution.
** 3. The name of the author may not be used to endorse or promote products
**    derived from this software without specific prior written permission. 
**  
** THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
** ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
** IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
** ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
** FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
** DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
** OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
** HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
** LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
** OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
** SUCH DAMAGE. */


#ifndef  _XMLRPC_CLIENT_H_
#define  _XMLRPC_CLIENT_H_ 1

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*=========================================================================
**  Initialization and Shutdown
**=========================================================================
**  These routines initialize and terminate the XML-RPC client. If you're
**  already using libwww on your own, you can pass
**  XMLRPC_CLIENT_SKIP_LIBWWW_INIT to avoid initializing it twice.
*/

#define XMLRPC_CLIENT_NO_FLAGS         (0)
#define XMLRPC_CLIENT_SKIP_LIBWWW_INIT (1)

extern void
xmlrpc_client_init(int flags,
		   char *appname,
		   char *appversion);

extern void
xmlrpc_client_cleanup();


/*=========================================================================
**  xmlrpc_client_call
**=========================================================================
**  A synchronous XML-RPC client. Do not attempt to call any of these
**  functions from inside an asynchronous callback!
*/

extern xmlrpc_value *
xmlrpc_client_call (xmlrpc_env *env,
		    char *server_url,
		    char *method_name,
		    char *args_format,
		    ...);

extern xmlrpc_value *
xmlrpc_client_call_params (xmlrpc_env *env,
			   char *server_url,
			   char *method_name,
			   xmlrpc_value *param_array);


/*=========================================================================
**  xmlrpc_client_call_asynch
**=========================================================================
**  An asynchronous XML-RPC client.
*/

/* A timeout in milliseconds. */
typedef unsigned long timeout_t;

/* A callback function to handle the response to an asynchronous call.
** If 'fault->fault_occurred' is true, then response will be NULL. All
** arguments except 'user_data' will be deallocated internally; please do
** not free any of them yourself.
** WARNING: param_array may (or may not) be NULL if fault->fault_occurred
** is true, and you set up the call using xmlrpc_client_call_asynch. */
typedef void (*xmlrpc_response_handler) (char *server_url,
					 char *method_name,
					 xmlrpc_value *param_array,
					 void *user_data,
					 xmlrpc_env *fault,
					 xmlrpc_value *result);

/* Make an asynchronous XML-RPC call. We make internal copies of all
** arguments except user_data, so you can deallocate them safely as soon
** as you return. Errors will be passed to the callback. You will need
** to run the event loop somehow; see below.
** WARNING: If an error occurs while building the parameter array, the
** response handler will be called with a NULL param_array. */
extern void
xmlrpc_client_call_asynch (char *server_url,
			   char *method_name,
			   xmlrpc_response_handler callback,
			   void *user_data,
			   char *args_format,
			   ...);

/* As above, but the parameter list is supplied as an xmlrpc_value
** containing an array. We make our own reference to the param_array,
** so you can DECREF yours as soon as this function returns. */
extern void
xmlrpc_client_call_asynch_params (char *server_url,
				  char *method_name,
				  xmlrpc_response_handler callback,
				  void *user_data,
				  xmlrpc_value *param_array);

/* Return true iff there are uncompleted asynchronous calls.
** The exact value of this during a response callback is undefined. */
extern int
xmlrpc_client_asynch_calls_are_unfinished (void);


/*=========================================================================
**  Event Loop Interface
**=========================================================================
**  These functions can be used to run the XML-RPC event loop. If you
**  don't like these, you can also run the libwww event loop directly.
*/

/* Some more flags. */
#define XMLRPC_CLIENT_FINISH_ASYNCH (1)
#define XMLRPC_CLIENT_USE_TIMEOUT   (2)

/* The generalized event loop. This uses the above flags. For more details,
** see the wrapper functions below. If you're not using the timeout, the
** 'milliseconds' parameter will be ignored.
** Note that ANY event loop call will return immediately if there are
** no outstanding XML-RPC calls. */
extern void
xmlrpc_client_event_loop_run_general (int flags, timeout_t milliseconds);

/* Finish all outstanding asynchronous calls. Alternatively, the loop
** will exit if someone calls xmlrpc_client_event_loop_end. */
extern void
xmlrpc_client_event_loop_finish_asynch (void);

/* Finish all outstanding asynchronous calls. Alternatively, the loop
** will exit if someone calls xmlrpc_client_event_loop_end or the
** timeout expires. */
extern void
xmlrpc_client_event_loop_finish_asynch_timeout (timeout_t milliseconds);

/* Run the event loop forever. The loop will exit if someone calls
** xmlrpc_client_event_loop_end. */
extern void
xmlrpc_client_event_loop_run (void);

/* Run the event loop forever. The loop will exit if someone calls
** xmlrpc_client_event_loop_end or the timeout expires.
** (Note that ANY event loop call will return immediately if there are
** no outstanding XML-RPC calls.) */
extern void
xmlrpc_client_event_loop_run_timeout (timeout_t milliseconds);

/* End the running event loop immediately. This can also be accomplished
** by calling the corresponding function in libwww.
** (Note that ANY event loop call will return immediately if there are
** no outstanding XML-RPC calls.) */
extern void
xmlrpc_client_event_loop_end (void);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _XMLRPC_CLIENT_H_ */
