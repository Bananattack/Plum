#include "../plum.h"

namespace Plum
{
	Video::Video()
	{
	}

	void Video::startup()
	{
		const SDL_VideoInfo* vidinfo = SDL_GetVideoInfo();
		desktopWidth = vidinfo->current_w;
		desktopHeight = vidinfo->current_h;
	}

	void Video::shutdown()
	{
	}

	void Video::setResolution(int width, int height, bool win)
	{
		windowed = win;

		xres = width;
		yres = height;

		// Fullscreen is usually not supported for low-res, so upscale everything!
		if(!windowed && (width < 640 || height < 480))
		{
			width = 640;
			height = 480;
		}

		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		frontSurface = SDL_SetVideoMode(width, height, 32, (windowed ? 0 : SDL_FULLSCREEN) | SDL_OPENGL);
		if(!frontSurface)
		{
			throw Video::Exception("Video settings were not compatible your graphics card.\r\n");
		}
		windowWidth = frontSurface->w;
		windowHeight = frontSurface->h;

		glBlendEquationEXT = (void (PLUM_CALLBACK *)(int)) SDL_GL_GetProcAddress("glBlendEquationEXT");
		if(!glBlendEquationEXT)
		{
			glBlendEquationEXT = NoBlendExtension;
		}

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_TEXTURE_2D);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, xres, yres, 0, -1, 1);

		glDisable(GL_DEPTH_TEST);

        glScissor(0, 0, xres, yres);
        glEnable(GL_SCISSOR_TEST);

		glClearColor(0.0, 0.0, 0.0, 1.0);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}

	void Video::clear(Color color)
	{
		ColorChannel r, g, b, a;
		getRGBA(color, r, g, b, a);
		glClearColor(r / 255.0f,
				g / 255.0f,
				b / 255.0f,
				a / 255.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}

	// Dear god I hope nobody uses this for anything intensive.
	void Video::setPixel(int x, int y, Color color, BlendMode mode)
	{
		ColorChannel r, g, b, a;
		getRGBA(color, r, g, b, a);

		mode = (mode == BlendUnspecified) ? getBlendMode() : mode;
		useHardwareBlender(mode);

		GLdouble vertexArray[] = { x, y };
		glDisable(GL_TEXTURE_2D);

		useHardwareColor(r, g, b, a);
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(2, GL_DOUBLE, 0, vertexArray);
		glDrawArrays(GL_POINTS, 0, 1);
		glDisableClientState(GL_VERTEX_ARRAY);
	}

	void Video::line(int x, int y, int x2, int y2, Color color, BlendMode mode)
	{
		ColorChannel r, g, b, a;
		getRGBA(color, r, g, b, a);

		mode = (mode == BlendUnspecified) ? getBlendMode() : mode;
		useHardwareBlender(mode);

		GLdouble vertexArray[] = { x, y - 1, x2 + 1, y2 };
		glDisable(GL_TEXTURE_2D);

		useHardwareColor(r, g, b, a);
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(2, GL_DOUBLE, 0, vertexArray);
		glDrawArrays(GL_LINES, 0, 2);
		glDisableClientState(GL_VERTEX_ARRAY);
	}

	void Video::rect(int x, int y, int x2, int y2, Color color, BlendMode mode)
	{
		ColorChannel r, g, b, a;
		getRGBA(color, r, g, b, a);

		mode = (mode == BlendUnspecified) ? getBlendMode() : mode;
		useHardwareBlender(mode);

		if(x > x2)
		{
			PLUM_SWAP(x, x2);
		}
		if(y > y2)
		{
			PLUM_SWAP(y, y2);
		}

		glDisable(GL_TEXTURE_2D);
		glPushMatrix();
		glBegin(GL_LINES);
			useHardwareColor(r, g, b, a);
			glVertex2d(x - 1, y - 1);
			glVertex2d(x - 1, y2);
			glVertex2d(x - 1, y2);
			glVertex2d(x2 + 1, y2);
			glVertex2d(x2 + 1, y2);
			glVertex2d(x2 + 2, y - 2);
			glVertex2d(x2 + 2, y - 2);
			glVertex2d(x - 1, y - 1);
		glEnd();
	
		glPopMatrix();
	}


	void Video::solidRect(int x, int y, int x2, int y2, Color color, BlendMode mode)
	{
		ColorChannel r, g, b, a;
		getRGBA(color, r, g, b, a);	

		mode = (mode == BlendUnspecified) ? getBlendMode() : mode;
		useHardwareBlender(mode);

		if(x > x2)
		{
			PLUM_SWAP(x, x2);
		}
		if(y > y2)
		{
			PLUM_SWAP(y, y2);
		}

		glDisable(GL_TEXTURE_2D);
		glPushMatrix();
		glBegin(GL_QUADS);
			useHardwareColor(r, g, b, a);
			glVertex2d(x - 1, y - 1);
			glVertex2d(x2 + 1, y - 1);
			glVertex2d(x2 + 1, y2);
			glVertex2d(x - 1, y2);
		glEnd();

		glPopMatrix();
	}

	void Video::horizontalGradientRect(int x, int y, int x2, int y2, Color color, Color color2, BlendMode mode)
	{
		ColorChannel r, g, b, a;
		getRGBA(color, r, g, b, a);

		mode = (mode == BlendUnspecified) ? getBlendMode() : mode;
		useHardwareBlender(mode);

		if(x > x2)
		{
			PLUM_SWAP(x, x2);
		}
		if(y > y2)
		{
			PLUM_SWAP(y, y2);
		}

		glDisable(GL_TEXTURE_2D);
		glPushMatrix();
		glBegin(GL_QUADS);
			useHardwareColor(r, g, b, a);
			glVertex2d(x - 1, y - 1);
			getRGBA(color2, r, g, b, a);
			useHardwareColor(r, g, b, a);
			glVertex2d(x2 + 1, y - 1);
			getRGBA(color, r, g, b, a);
			useHardwareColor(r, g, b, a);
			glVertex2d(x2 + 1, y2);
			getRGBA(color2, r, g, b, a);
			useHardwareColor(r, g, b, a);
			glVertex2d(x - 1, y2);
		glEnd();

		glPopMatrix();
	}

	void Video::verticalGradientRect(int x, int y, int x2, int y2, Color color, Color color2, BlendMode mode)
	{
		ColorChannel r, g, b, a;
		getRGBA(color, r, g, b, a);

		mode = (mode == BlendUnspecified) ? getBlendMode() : mode;
		useHardwareBlender(mode);

		if(x > x2)
		{
			PLUM_SWAP(x, x2);
		}
		if(y > y2)
		{
			PLUM_SWAP(y, y2);
		}

		glDisable(GL_TEXTURE_2D);
		glPushMatrix();
		glBegin(GL_QUADS);
			useHardwareColor(r, g, b, a);
			glVertex2d(x - 1, y - 1);
			glVertex2d(x2 + 1, y - 1);
			getRGBA(color2, r, g, b, a);
			useHardwareColor(r, g, b, a);
			glVertex2d(x2 + 1, y2);
			glVertex2d(x - 1, y2);
		glEnd();

		glPopMatrix();
	}

	void Video::circle(int x, int y, int horizontalRadius, int verticalRadius, Color color, BlendMode mode)
	{
		ColorChannel r, g, b, a;
		getRGBA(color, r, g, b, a);

		mode = (mode == BlendUnspecified) ? getBlendMode() : mode;
		useHardwareBlender(mode);

		double x1 = x;
		double y1 = y + verticalRadius;
		double angle;

		glDisable(GL_TEXTURE_2D);
		glPushMatrix();
		glBegin(GL_LINE_STRIP);			
			useHardwareColor(r, g, b, a);
			for(angle = 0.0; angle <= 2.0 * PLUM_PI; angle += 0.01)
			{				
				glVertex2d(x1, y1);
				x1 = x + (horizontalRadius * (double) sin(angle));
				y1 = y + (verticalRadius * (double) cos(angle));			
			}
		glEnd();
		glPopMatrix();
	}

	void Video::solidCircle(int x, int y, int horizontalRadius, int verticalRadius, Color color, BlendMode mode)
	{
		ColorChannel r, g, b, a;
		getRGBA(color, r, g, b, a);

		mode = (mode == BlendUnspecified) ? getBlendMode() : mode;
		useHardwareBlender(mode);

		double x1 = x;
		double y1 = y + verticalRadius;
		double angle;

		glDisable(GL_TEXTURE_2D);
		glPushMatrix();
		glBegin(GL_TRIANGLES);	
			useHardwareColor(r, g, b, a);
			for(angle = 0.0; angle <= 2.0 * PLUM_PI; angle += 0.01)
			{	
				glVertex2d(x, y);
				glVertex2d(x1, y1);
				x1 = x + (horizontalRadius * (double) sin(angle));
				y1 = y + (verticalRadius * (double) cos(angle));
				glVertex2d(x1, y1);			
			}
		glEnd();
		glPopMatrix();
	}
}
