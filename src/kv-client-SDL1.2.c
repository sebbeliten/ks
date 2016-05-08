
#include <SDL/SDL.h>
#include "ok-SDL1.2.h"

#include "dasnethelp.h"

#include <errno.h>
#define perror_getaddrinfo(getaddrinfo_return_value) \
  { fprintf(stderr,"getaddrinfo: %s\n",gai_strerror(getaddrinfo_return_value)); exit(1); }
#define perrno(string) \
  { dasnethelps_perror(string); exit(1); }

#include <stdbool.h>
char *opt_address      =   NULL; //-a
char *opt_port         = "1111"; //-p
bool  opt_dont_connect =  false; //-d

#include "macroopt.h"
int main(int argc, char *argv[])
{
#ifdef OS_WINDOWS
  freopen("CON","w",stdout);
  freopen("CON","w",stderr);
  //http://sdl.beuc.net/sdl.wiki/FAQ_Console
#endif

  macroopt
  case 'h':
    puts(
    "-a ;; specify address to connect to\n"
    "-p ;; which port to supplement the address\n"
    "-d ;; don't connect to server (ignore -a and -p)\n"
    );
    exit(1);
    break;
  case 'a':gotarg
    opt_address = optarg;
    break;
  case 'p':gotarg
    opt_port = optarg;
    break;
  case 'd':
    opt_dont_connect = true;
    break;
  done
  else
    opt_address = argv[optind];

  if(opt_dont_connect) goto skip_connect;

  if(dasnethelps_init()){ 
    fprintf(stderr,"WSAStartup failed.\n");
    exit(1);
  }

  struct addrinfo hints, *res;

  memset(&hints,0,sizeof hints);

  hints.ai_family = AF_UNSPEC;
  hints.ai_flags = AI_PASSIVE;
  hints.ai_socktype = SOCK_STREAM;
  
  { int return_value;
    if(return_value=getaddrinfo(opt_address,opt_port,&hints,&res))
      perror_getaddrinfo(return_value);
  }

  int server;
  if((server = socket(res->ai_family,res->ai_socktype,res->ai_protocol)) < 0){
    perrno("socket()");
  }
  
  if(connect(server,res->ai_addr,res->ai_addrlen)){
    perrno("connect()");
  }

  skip_connect:

  SDL_Init(SDL_INIT_VIDEO);
  SDL_Surface *window = SDL_SetVideoMode(32,32,0,0);

  bool running = true;
  while(running)
  {
    SDL_Event event;
    if(!SDL_WaitEvent(&event))
    { 
      fprintf(stderr,"SDL_WaitEvent(): error\n");
      exit(1);
    }

    //SDL_keysym ks = event.key.keysym;
    int32_t ok;
    int32_t ch = event.key.keysym.sym;
    switch(event.type)
    {
      case SDL_KEYDOWN:
      case SDL_KEYUP:
      {
        char str[32];
        if((ok=SDLKey_TO_OK(ch)))
          strcpy(str,(char*)OK_TO_STRING(ok));
        else
        {
          //if(ch == 'q')
          //{
          //  running = false;
          //  break;
          //}
          str[0] = ch;
          str[1] = '\0';
        }
        puts(str);

        int32_t release = event.type == SDL_KEYUP ? 1:0;
        
        release = htonl(release);
        ok = htonl(ok);
        ch = htonl(ch);
        
        if(opt_dont_connect)
          break;
        int32_t buf[3] = {release,ok,ch};
        send(server,buf,3 * sizeof(int32_t),0);
        break;
      }
      case SDL_QUIT:
        running = false;
        break;
    }
  }
  dasnethelps_quit();
  SDL_Quit();
  return 0;
}
