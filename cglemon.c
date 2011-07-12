
#include <stdio.h>
#include <strings.h>
#include <stdlib.h>
#include "glk.h"
#include "cheapglk.h"

#ifdef GLK_MODULE_LEMON


#include "SDL.h"
#include "SDL_events.h"

#include "SDL_opengl.h"
#include <GL/glu.h>

#define logit printf

int w = 1280;
int h = 800;

float znear=1;
float zfar=20;

SDL_Surface* s;

SDL_Surface *initsdl(int w,int h,int *bppp,Uint32 flags)
{
	// SDL_INIT_EVENTTHREAD
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
		fprintf(stderr,"Couldn't initialize SDL: %s\n",SDL_GetError());
		exit( 1 );
	}
	
	SDL_Surface *s;
	static int bpp=0; 
	Uint32 video_flags;
	video_flags = flags;
	int rgb_size[3]={0,0,0};
	printf("yoyoyo\n");
	if (flags& SDL_OPENGL)
	{
/*
	if ( SDL_GetVideoInfo()->vfmt->BitsPerPixel <= 8 ) {
		bpp = 8;
	} else {
		bpp = 16;  // More doesn't seem to work 
	}*/
	bpp=SDL_GetVideoInfo()->vfmt->BitsPerPixel;
	switch (bpp) {
	    case 8:
		rgb_size[0] = 3;
		rgb_size[1] = 3;
		rgb_size[2] = 2;
		break;
	    case 15:
	    case 16:
		rgb_size[0] = 5;
		rgb_size[1] = 5;
		rgb_size[2] = 5;
		break;
            default:
		rgb_size[0] = 8;
		rgb_size[1] = 8;
		rgb_size[2] = 8;
		break;
	}
	SDL_GL_SetAttribute( SDL_GL_RED_SIZE, rgb_size[0] );
	SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, rgb_size[1] );
	SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, rgb_size[2] );
	SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
	}
	
	video_flags|=(SDL_RESIZABLE|SDL_ANYFORMAT|SDL_DOUBLEBUF);
	
	s= SDL_SetVideoMode( w, h, bpp, video_flags );
	if (s == NULL ) {
		fprintf(stderr, "Couldn't set video mode: %s\n", SDL_GetError());
		SDL_Quit();
		exit(1);
	}
	
//#ifdef chaos
	printf("Screen BPP: %d\n", SDL_GetVideoSurface()->format->BitsPerPixel);
	printf("\n");
#ifdef GL

	printf( "Vendor     : %s\n", glGetString( GL_VENDOR ) );
	printf( "Renderer   : %s\n", glGetString( GL_RENDERER ) );
	printf( "Version    : %s\n", glGetString( GL_VERSION ) );
	printf( "Extensions : %s\n", glGetString( GL_EXTENSIONS ) );
	printf("\n");
	int value;
	SDL_GL_GetAttribute( SDL_GL_RED_SIZE, &value );
	printf( "SDL_GL_RED_SIZE: requested %d, got %d\n", rgb_size[0],value);
	SDL_GL_GetAttribute( SDL_GL_GREEN_SIZE, &value );
	printf( "SDL_GL_GREEN_SIZE: requested %d, got %d\n", rgb_size[1],value);
	SDL_GL_GetAttribute( SDL_GL_BLUE_SIZE, &value );
	printf( "SDL_GL_BLUE_SIZE: requested %d, got %d\n", rgb_size[2],value);
	SDL_GL_GetAttribute( SDL_GL_DEPTH_SIZE, &value );
	printf( "SDL_GL_DEPTH_SIZE: requested %d, got %d\n", bpp, value );
	SDL_GL_GetAttribute( SDL_GL_DOUBLEBUFFER, &value );
	printf( "SDL_GL_DOUBLEBUFFER: requested 1, got %d\n", value );
#endif
	
	printf("mustlock=%i\n", SDL_MUSTLOCK(s));

	*bppp=bpp;
	char * x= (char*)malloc(20);
	if(x&&SDL_VideoDriverName(x,20))
	    printf("Current SDL video driver is %s.\n",x);
	   
//#endif
	return s;
}


void resetviewport(void)
{
	glViewport( 0, 0, SDL_GetVideoSurface()->w,SDL_GetVideoSurface()->h );
}

void resetmatrices(void)
{
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity( );
        glMatrixMode( GL_MODELVIEW );
        glLoadIdentity();
}

void gle(void)
{
	GLenum gl_error;
	gl_error = glGetError( );
	if( gl_error != GL_NO_ERROR )
	{
	    if(gl_error==GL_STACK_UNDERFLOW)
		logit("QUACK QUACK QUACK, UNDERFLOVING STACK\n");
	    else if(gl_error==GL_STACK_OVERFLOW)
		logit("QUACK QUACK QUACK, OVERFLOVING STACK\n");
	    else if(gl_error==GL_INVALID_OPERATION)
		logit("INVALID OPERATION, PATIENT EXPLODED\n");
	    else if(gl_error==GL_INVALID_VALUE)
		logit("GL_INVALID_VALUE");
	    else
		logit("OpenGL error: 0x%X\n", gl_error );
	}
}
void sdle(void)
{
	char* sdl_error;
	sdl_error = SDL_GetError( );
	if( sdl_error[0] != '\0' )
	{
		logit("testgl: SDL error '%s'\n", sdl_error);
		SDL_ClearError();
	}
}


int bpp=8;


void glk_make_sdl_window(void)
{


	s=initsdl(w,h,&bpp,SDL_OPENGL);
	SDL_EnableUNICODE(1);
	SDL_InitSubSystem( SDL_INIT_TIMER);
	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY/2, SDL_DEFAULT_REPEAT_INTERVAL/3);
	gle();
	resetviewport();
	resetmatrices();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glClearColor( 0.0, 0.0, 0.0, 0.0 );
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	printf("LEMON\n");
}


#endif /* GLK_MODULE_LEMON */
