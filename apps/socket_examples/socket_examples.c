
#include "socket_examples.h"

#include "lwip/opt.h"

#if 1//LWIP_SOCKET

#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"
#include <string.h>
#include <stdio.h>
#include <windows.h>
#ifndef SOCK_TARGET_HOST
#define SOCK_TARGET_HOST  "10.0.0.172"
#endif

#ifndef SOCK_TARGET_PORT
#define SOCK_TARGET_PORT  80
#endif
CRITICAL_SECTION critSec1;
int reclen = 0;
struct hostent* hip = NULL;
static void sockex_nonblocking_connect(void *arg)
{
	char *m_requestheader = "GET http://www.163.com HTTP/1.1\r\nHost:www.163.com\r\nAccept:*/*\r\nUser-Agent:Mozilla/4.0(compatible; MSIE 7.0; Windows NT 6.0; Trident/4.0)\r\nConnection:Close\r\n\r\n";

  int s;
  int ret;
  u32_t opt;
  struct sockaddr_in addr;
  fd_set readset;
  fd_set writeset;
  fd_set errset;
  struct timeval tv;
  u32_t ticks_a, ticks_b;
  int err;
  int tick = 0;
  
  int count = 0;
  u8_t buf[1024];
  char* strHeader1 = "POST http://uas.mmarket.com:8090/register HTTP/1.1\r\nX-Online-Host: uas.mmarket.com:8090\r\ncontent-type: Application\r\nx-up-calling-line-id:\r\nUser-Agent: Java/1.6.0_16\r\nAccept: text/html, image/gif, image/jpeg, *; q=.2, */*; q=.2\r\nProxy-Connection:\r\nContent-Length: 74\r\nHost: uas.mmarket.com:8090\r\n\r\n<req><imsi>00000000000</imsi><apid>3</apid><channel_id></channel_id></req>";
  
 
  LWIP_UNUSED_ARG(arg);
  /* set up address to connect to */
  memset(&addr, 0, sizeof(addr));
  addr.sin_len = sizeof(addr);
  addr.sin_family = AF_INET;
  addr.sin_port = PP_HTONS(SOCK_TARGET_PORT);
  //addr.sin_addr.s_addr = inet_addr(SOCK_TARGET_HOST);
  addr.sin_addr.s_addr = inet_addr(inet_ntoa(*((struct in_addr *)hip->h_addr_list[0])));

  /* first try blocking: */

  /* create the socket */
  
  
  s = lwip_socket(AF_INET, SOCK_STREAM, 0);
  LWIP_ASSERT("s >= 0", s >= 0);

  /* connect */
  ret = lwip_connect(s, (struct sockaddr*)&addr, sizeof(addr));
  /* should succeed */
  LWIP_ASSERT("ret == 0", ret == 0);

  /* write something */
  ret = lwip_write(s, m_requestheader, strlen(m_requestheader));
  //LWIP_ASSERT("ret == 4", ret == 4);

  
  while(1)
  {
	  count = lwip_read(s,buf,1024);
	  if (count == -1)
	  {
		  break;
	  }
	  if(count != 0){
		  tick = 0;
			EnterCriticalSection(&critSec1);
		  reclen += count;
		  LeaveCriticalSection(&critSec1);
		  printf("get Len:%d\r\n",reclen/1024);
		 
	  }else
	  {
		  sys_msleep(300);
		  if(tick == 10)
		  {
			  break;
		  }
	  }
	  tick++;
  }
  /* close */
  ret = lwip_close(s);
  LWIP_ASSERT("ret == 0", ret == 0);
}

void socket_examples_init(void)
{
	while(hip == NULL)
	{
		hip = gethostbyname("www.163.com");

		if(hip != NULL)
			printf("IP Address : %s\n",inet_ntoa(*((struct in_addr *)hip->h_addr_list[0])));
	}
	InitializeCriticalSection(&critSec1);

  sys_thread_new("sockex_nonblocking_connect", sockex_nonblocking_connect, NULL, 0, 0);
 /* sys_thread_new("sockex_nonblocking_connect", sockex_nonblocking_connect, NULL, 0, 0);
  sys_thread_new("sockex_nonblocking_connect", sockex_nonblocking_connect, NULL, 0, 0);
  sys_thread_new("sockex_nonblocking_connect", sockex_nonblocking_connect, NULL, 0, 0);
  sys_thread_new("sockex_nonblocking_connect", sockex_nonblocking_connect, NULL, 0, 0);*/
 // sys_thread_new("sockex_testrecv", sockex_testrecv, NULL, 0, 0);
  /*sys_thread_new("sockex_testtwoselects", sockex_testtwoselects, NULL, 0, 0);*/
}

#endif /* LWIP_SOCKETS */
