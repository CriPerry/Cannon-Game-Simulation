/*
 ============================================================================
 Project	: Cannon
 Author		: Perissutti Cristian
 Description: 	Real-time application development project for the course of
 	 	"Informatica e Sistemi in Tempo Reale"
 ============================================================================
*/

#include "Cannon.h"

// Function definitions

/*creates a sphere given radius rad*/
void sphere(float rad, int lats, int longs)
{
	int i, j;
	
	double lat0, z0, zr0, lat1, z1, zr1, lng, x, y;
	//cycle trough latitude and longitude
	for(i = 0; i <= lats; i++) 
	{
		lat0 = PI * (-0.5 + (double) (i - 1) / lats);
		z0  = rad * sin(lat0);
		zr0 =  cos(lat0);
		lat1 = PI * (-0.5 + (double) i / lats);
		z1 = rad * sin(lat1);
		zr1 = cos(lat1);
		glBegin(GL_QUAD_STRIP);
	
		for(j = 0; j <= longs; j++) 
		{
			lng = 2 * PI * (double) (j - 1) / longs;
			x = rad * cos(lng);
			y = rad * sin(lng);
    			//creates a mesh of rectangles
			glNormal3f(x * zr0, y * zr0, z0);
			glVertex3f(x * zr0, y * zr0, z0);
			glNormal3f(x * zr1, y * zr1, z1);
			glVertex3f(x * zr1, y * zr1, z1);
		}
	glEnd();
	}
}

/*creates a cylinder given base radius r and height h*/
void cylinder(float r, float h)
{
	float x, y;
	float angle;
	float angle_stepsize;
	
	x = 0.0;
    	y = 0.0;
    	angle = 0.0;
    	angle_stepsize = 0.1;
	
    	glBegin(GL_QUAD_STRIP);
        while( angle < 2*PI )
        {
        	x = r * cos(angle);
        	y = r * sin(angle);
        	glVertex3f(x, y , h);
        	glVertex3f(x, y , 0.0);
        	angle = angle + angle_stepsize;
        }
        
        glVertex3f(r, 0.0, h);
        glVertex3f(r, 0.0, 0.0);
        
        glEnd();
}

/*return the sign of the number*/
int sign(float num) 
{
    if (num > 0) 
    {
        return 1;
    } 
    else if (num < 0) 
    {
        return -1;
    } 
    else 
    {
        return 0; 
    }
}

/*creates the groud and the playground*/
void create_ground(void)
{
	//creates playground
	glColor3f(0.133f, 0.545f, 0.133f);
	glBegin(GL_QUADS);
		glVertex3f(-g_x, 0.0f, -g_z);
		glVertex3f(-g_x, 0.0f, g_z);
		glVertex3f(g_x, 0.0f, g_z);
		glVertex3f(g_x, 0.0f, -g_z);
	glEnd();
	
	//creates ground
	glColor3f(0.0f, 0.392f, 0.0f);
	glBegin(GL_QUADS);
		glVertex3f(-eth, -0.001f, -eth);
		glVertex3f(-eth, -0.001f,  eth);
		glVertex3f(eth, -0.001f,  eth);
		glVertex3f(eth, -0.001f, -eth);
	glEnd();
		
}

/*creates the wall given position on x axis and height, the wall is made of 5 rectangulars*/
void create_wall(float x_pos, float wall_h)
{		
	pthread_mutex_lock(&mux);
	
	//cannon area face
	glColor3f(0.698f, 0.133f, 0.133f);
	glBegin(GL_QUADS);
		glVertex3f(w_posx, w_h, -g_z);
		glVertex3f(w_posx, w_h, g_z);
		glVertex3f(w_posx, 0.0f, g_z);
		glVertex3f(w_posx, 0.0f, -g_z);
	glEnd();
	
	//target area face
	glBegin(GL_QUADS);
		glVertex3f(w_posx + w_width, w_h, -g_z);
		glVertex3f(w_posx + w_width, w_h, g_z);
		glVertex3f(w_posx + w_width, 0.0f, g_z);
		glVertex3f(w_posx + w_width, 0.0f, -g_z);
	glEnd();
	
	//top face
	glBegin(GL_QUADS);
		glVertex3f(w_posx, w_h, -g_z);
		glVertex3f(w_posx, w_h, g_z);
		glVertex3f(w_posx + w_width, w_h, g_z);
		glVertex3f(w_posx + w_width, w_h, -g_z);
	glEnd();
	
	//lateral faces
	glBegin(GL_QUADS);
		glVertex3f(w_posx, w_h, -g_z);
		glVertex3f(w_posx + w_width, w_h, -g_z);
		glVertex3f(w_posx + w_width, 0.0f, -g_z);
		glVertex3f(w_posx, 0.0f, -g_z);
	glEnd();
	
	glBegin(GL_QUADS);
		glVertex3f(w_posx, w_h, g_z);
		glVertex3f(w_posx + w_width, w_h, g_z);
		glVertex3f(w_posx + w_width, 0.0f, g_z);
		glVertex3f(w_posx, 0.0f, g_z);
	glEnd();
		
	pthread_mutex_unlock(&mux);
}

/*Creates the targets*/
void create_target()
{		
	pthread_mutex_lock(&mux);
	
	//blue target
	glColor3f(0.000f, 0.000f, 1.000f);
	glBegin(GL_QUADS);
		glVertex3f(tar_vec[0].posx - tar_vec[0].dim, 0.1f, tar_vec[0].posz + tar_vec[0].dim);
		glVertex3f(tar_vec[0].posx - tar_vec[0].dim, 0.1f, tar_vec[0].posz - tar_vec[0].dim);
		glVertex3f(tar_vec[0].posx + tar_vec[0].dim, 0.1f, tar_vec[0].posz - tar_vec[0].dim);
		glVertex3f(tar_vec[0].posx + tar_vec[0].dim, 0.1f, tar_vec[0].posz + tar_vec[0].dim);
	glEnd();
	
	//red target
	glColor3f(0.863f, 0.078f, 0.235f);
	glBegin(GL_QUADS);
		glVertex3f(tar_vec[1].posx - tar_vec[1].dim, 0.1f, tar_vec[1].posz + tar_vec[1].dim);
		glVertex3f(tar_vec[1].posx - tar_vec[1].dim, 0.1f, tar_vec[1].posz - tar_vec[1].dim);
		glVertex3f(tar_vec[1].posx + tar_vec[1].dim, 0.1f, tar_vec[1].posz - tar_vec[1].dim);
		glVertex3f(tar_vec[1].posx + tar_vec[1].dim, 0.1f, tar_vec[1].posz + tar_vec[1].dim);
	glEnd();
		
	pthread_mutex_unlock(&mux);
}

/*creates cannon at given position on z axis and rotated of an angle*/
void create_cannon(float angles, float posiz)
{	
	pthread_mutex_lock(&mux);
	
	//tranlsate and rotate to the correct position
	glTranslatef(c_pos, 0.0f, posiz);
	glRotatef(90, 0, 1, 0);
	glRotatef(-angles, 1, 0, 0);
	
	//create the cannon
	glColor3f(0.666f, 0.666f, 0.666f);
  	cylinder(c_rad, c_len);
  	
  	//return to previous position and orientation
	glRotatef(angles, 1, 0, 0);
	glRotatef(-90, 0, 1, 0);
	glTranslatef(-c_pos, 0.0f, -posiz);
		
	pthread_mutex_unlock(&mux);
}

/*creates the projectile at given position*/
void create_projectile(float xcen, float ycen, float zcen)
{	
	pthread_mutex_lock(&mux);
	
	//translate to correct position
	glTranslatef(xcen, ycen, zcen);
	
	//create the projectile
	glColor3f(0.184f, 0.310f, 0.310f);
	sphere(p_rad * 2.2, 20, 20);
	
	//return to previous position
	glTranslatef(-xcen, -ycen, -zcen);
		
	pthread_mutex_unlock(&mux);
}

/*creates the wind vane*/
void create_wvane()
{		
	//creates the black vertical rod
	glLineWidth(4.0);
	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_LINES);				
		glVertex3f(-g_x, 0, -g_z);
		glVertex3f(-g_x, wvane_h, -g_z);
	glEnd();
	glLineWidth(1.0);
	
	pthread_mutex_lock(&mux);
	
	//creates a red flag with the same direction of the wind. if there is no wind there is no flag 
	glColor3f(1.0f, 0.0f, 0.0f);			
	glBegin(GL_TRIANGLES);					
		glVertex3f(-g_x, wvane_h - 1, -g_z);
		glVertex3f(-g_x, wvane_h, -g_z);
		glVertex3f(-g_x + (wind_x * 4), wvane_h - 0.5, -g_z + (wind_z * 4));
	glEnd();
		
	pthread_mutex_unlock(&mux);
}


/*set random -wall height and position- -target direction and initial position- -ghost position noise- -wind speed components-, called when a shot ends*/
void rset()
{
	pthread_mutex_lock(&mux);
	
	n_x = noise * (((float) rand() / (float) RAND_MAX) - 0.5);						//random ghost position noise
	n_z = noise * (((float) rand() / (float) RAND_MAX) - 0.5);
	
	w_h = 2 +(w_maxh * (float) rand() / (float) RAND_MAX);							//random new wall height
	w_posx = -(g_x / 2) + (g_x * (float) rand() / (float) RAND_MAX);					//random new wall position
	
	if(wind)
	{
		wind_x = (maxwind * (float) rand() / (float) RAND_MAX) - 0.1;					//set random wind
		wind_z = (maxwind * (float) rand() / (float) RAND_MAX) - 0.1;
	}
	
	tar_vec[0].posx = w_posx + w_width + tar_vec[0].dim  + ((g_x - (2 * tar_vec[0].dim) - w_posx - w_width) * (float) rand() / (float) RAND_MAX);	//random blue target new initial position
	tar_vec[0].posz = -g_z + tar_vec[0].dim +((2 * g_z - (2 * tar_vec[0].dim))* (float) rand() / (float) RAND_MAX);
	t_angle = 2 * PI * (float) rand() / (float) RAND_MAX;												//random blue target new direction (angle)
	tar_vec[0].spdx = t_spd * cos(t_angle);
	tar_vec[0].spdz = t_spd * sin(t_angle);
	
	tar_vec[1].posx = w_posx + w_width + tar_vec[1].dim  + ((g_x - (2 * tar_vec[1].dim) - w_posx - w_width) * (float) rand() / (float) RAND_MAX);	//random red target new initial position
	tar_vec[1].posz = -g_z + tar_vec[1].dim +((2 * g_z - (2 * tar_vec[1].dim)) * (float) rand() / (float) RAND_MAX);
	t_angle = 2 * PI * (float) rand() / (float) RAND_MAX;												//random red target new direction (angle)
	tar_vec[1].spdx = t_spd * cos(t_angle);
	tar_vec[1].spdz = t_spd * sin(t_angle);
	
	pthread_mutex_unlock(&mux);
}


/*calculate where the projectile will land*/
void trajectory()
{	
	float vlx, vly, vlz;
	
	if(!shoot)
	{
		vlx = (0.6 + c_powstp * (c_pow - 1)) * (cos(c_angle * PI / 180));	//initialization
		vly = (0.6 + c_powstp * (c_pow - 1)) * (sin(c_angle * PI / 180));
		vlz = 0;
		g_posx = p_posx;
		g_posz = p_posz;
		g_posy = p_posy;
		
		while(g_posy > 0)							//calculate the landing position of the projectile simualting a shot
		{
			g_posx += vlx;						
			g_posy += vly;
			g_posz += vlz;
			vly -= (g * 1.5 + (Cd * rho * vly * vly * PI * p_rad * p_rad / (2 * p_mass * 1.5 * 1.5 * 1.5) * sign(vly))) ;		
			vlx -= (sign(vlx) * Cd * rho * (vlx - wind_x) * (vlx - wind_x) * PI * p_rad * p_rad / (2 * p_mass * 1.5 * 1.5 * 1.5));
			vlz += sign(wind_z) * (Cd * rho * (vlz - wind_z) * (vlz - wind_z) * PI * p_rad * p_rad / (2 * p_mass)* 1.5 * 1.5 * 1.5);
		}
		g_posx += n_x;								//add noise to the landing position
		g_posz += n_z;
	}
	
	glTranslatef(g_posx, 0, g_posz);						//creates the ghost
	glColor3f(1.0f, 0.549f, 0.0f);
	sphere(g_rad, 10, 10);
}

/*set camera position and orientation*/
void adjcamera (void)
{
	pthread_mutex_lock(&mux);
	
	//set view window parameters
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	glFrustum (-1.0, 1.0, -1.0, 1.0, 1.0, 60.0);
	glMatrixMode (GL_MODELVIEW);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	
	//change position and orientation of the scene
	glTranslatef (-camera.posx, 0, -camera.posz);
	glRotatef (camera.xangle, 1, 0, 0);			
	glRotatef (camera.yangle, 0, 1, 0);
	glTranslatef (0, -camera.posy, 0);	
	pthread_mutex_unlock(&mux);
}

/*target thread function*/
void *move_target(void *arg)
{
	set_period(arg);
	
	//choose which target control
	int k = ((struct thread_param *)arg)->tar_n;
	
	//must run forever
	while(1)
	{
		pthread_mutex_lock(&mux);
		
		//target bounce if it goes out of playground
		if(tar_vec[k].posx + tar_vec[k].spdx >= g_x - tar_vec[k].dim | tar_vec[k].posx + tar_vec[k].spdx <= w_posx + tar_vec[k].dim + w_width) tar_vec[k].spdx *= -1;
		if(tar_vec[k].posz + tar_vec[k].spdz >= g_z - tar_vec[k].dim | tar_vec[k].posz + tar_vec[k].spdz <= -g_z + tar_vec[k].dim + w_width) tar_vec[k].spdz *= -1;
		
		//compute new target position
		tar_vec[k].posx += tar_vec[k].spdx;
		tar_vec[k].posz += tar_vec[k].spdz;
		
		pthread_mutex_unlock(&mux);
		
		deadline_miss(arg);
		wait_for_period(arg);
	}
	return NULL;
}

/*cannon thread function*/
void *cannon(void* arg)
{
	set_period(arg);
	
	//run while the projectile is flying
	while(p_posy - p_rad >= 0)
	{
		pthread_mutex_lock(&mux);
		
		//condition: the target doesn't hit the wall and doesn't exit the playground
		if(!(p_posx < -g_x || p_posx > g_x || (p_posy - p_rad <= w_h && w_posx - p_posx > -p_rad  && w_posx - p_posx < p_rad)))
		{
			//compute projectile position and speed
			p_posx += p_velx;
			p_posy += p_vely;
			p_posz += p_velz;
			p_vely -= (g * 1.5 + (Cd * rho * p_vely * p_vely * PI * p_rad * p_rad / (2 * p_mass * 1.5 * 1.5 * 1.5) * sign(p_vely))) ;
			p_velx -= (sign(p_velx) * Cd * rho * (p_velx - wind_x) * (p_velx - wind_x) * PI * p_rad * p_rad / (2 * p_mass * 1.5 * 1.5 * 1.5));
			p_velz += sign(wind_z) * (Cd * rho * (p_velz - wind_z) * (p_velz - wind_z) * PI * p_rad * p_rad / (2 * p_mass)* 1.5 * 1.5 * 1.5);
		}
		else
		{
			//if projectile hit the wall or exit the playground, end the shot.
			shoot = 0;
			
			pthread_mutex_unlock(&mux);
			
			rset();
			pthread_exit(NULL);
		}
		
		pthread_mutex_unlock(&mux);
		
		deadline_miss(arg);
		wait_for_period(&tpar_c);
	}
	
	pthread_mutex_lock(&mux);	
	
	//if blue target is hit, +1 point	
	if(p_posx >= tar_vec[0].posx - tar_vec[0].dim && p_posx <= tar_vec[0].posx + tar_vec[0].dim && p_posz >= tar_vec[0].posz - tar_vec[0].dim && p_posz <= tar_vec[0].posz + tar_vec[0].dim)
	{
		score += 1;
	}
	
	//if red target is hit, +3 points
	if(p_posx >= tar_vec[1].posx - tar_vec[1].dim && p_posx <= tar_vec[1].posx + tar_vec[1].dim && p_posz >= tar_vec[1].posz - tar_vec[1].dim && p_posz <= tar_vec[1].posz + tar_vec[1].dim)
	{
		score += 3;
	}
	
	//check win condition and end shot
	if(score >= max_points) win = 1;
	shoot = 0;
	
	pthread_mutex_unlock(&mux);
	
	rset();		
}

void *input_ex(void *arg)
{
	int prevx, prevy;
	
	set_period(arg);
	
	while(1)
	{
		if (win == 0)
		{
			//check mouse movements and compute camera orientation
			prevx = 0;
			prevy = 0;
			
			get_mouse_mickeys(&prevx, &prevy);
			
			//input active only if no projectile is flying
			if(!shoot)
			{
				if (mouse_b & 1) 
				{
					int x,y;
					get_mouse_mickeys(&x, &y);
					
					pthread_mutex_lock(&mux);
					camera.yangle += (x - prevx) * 0.1;
					camera.xangle += (y - prevy) * 0.1;
					pthread_mutex_unlock(&mux);
				}
			
				//compute camera position
				pthread_mutex_lock(&mux);
				if (key[KEY_W]) camera.posz -= cam_dist_spd;
				if (key[KEY_S]) camera.posz += cam_dist_spd;
				if (key[KEY_A]) camera.posx -= cam_dist_spd;
				if (key[KEY_D]) camera.posx += cam_dist_spd;
				if (key[KEY_Q]) camera.posy -= cam_dist_spd;
				if (key[KEY_E]) camera.posy += cam_dist_spd;
				pthread_mutex_unlock(&mux);
			
				// ghost activation
				if (key[KEY_T]) 
				{

					while(key[KEY_T]){}
					
					pthread_mutex_lock(&mux);
					ez = (ez + 1) % 2;
					pthread_mutex_unlock(&mux);
				}
			
				// wind activation
				if (key[KEY_V]) 
				{
					while(key[KEY_V]){}
					
					pthread_mutex_lock(&mux);
					if(wind)
					{
						wind = 0;
						wind_x = 0;
						wind_z = 0;
					}
					else
					{
						wind = 1;
						wind_x = maxwind * (((float) rand() / (float) RAND_MAX) - 0.5) ;
						wind_z = maxwind * (((float) rand() / (float) RAND_MAX) - 0.5);
					};
					pthread_mutex_unlock(&mux);
				}
			
				//change cannon power
				if (key[KEY_M]) 
				{
					while(key[KEY_M]){}
					pthread_mutex_lock(&mux);
					if(c_pow < c_maxp && !(shoot)) c_pow += 1;
					pthread_mutex_unlock(&mux);
				}
				if (key[KEY_N] && c_pow > c_minp && !(shoot)) 
				{	
					while(key[KEY_N]){}
					
					pthread_mutex_lock(&mux);
					c_pow -= 1;
					pthread_mutex_unlock(&mux);
				}
				
				//compute cannon orientation
				pthread_mutex_lock(&mux);
				if (key[KEY_UP] && c_angle + 1 < 90)
				{
					c_angle += ca_stp;
				}
			
				if (key[KEY_DOWN] && c_angle - 1 > 0)
				{
					c_angle -= ca_stp;
				}
		
				//compute cannon position
				if (key[KEY_LEFT] && c_posz > -(g_z - 1))
				{				
					c_posz -= cp_stp;
				}
			
				if (key[KEY_RIGHT] && c_posz < (g_z - 1))
				{
					c_posz += cp_stp;
				}
				
				pthread_mutex_unlock(&mux);
				
				//start a shot
				if (key[KEY_SPACE])
				{
					while(key[KEY_SPACE]){};
					pthread_mutex_lock(&mux);
					shoot = 1;
					pthread_mutex_unlock(&mux);
					
					/*cannon thread*/
					tpar_c.period = 20;
					tpar_c.rel_dl = tpar_c.period;
					tpar_c.dl_miss = 0;
					tpar_c.priority = 20;
					pthread_attr_init(&att_c);
					pthread_attr_setinheritsched(&att_c, PTHREAD_EXPLICIT_SCHED);
					pthread_attr_setschedpolicy(&att_c, SCHED_OTHER);
					c_sched.sched_priority = tpar_c.priority;
					pthread_attr_setschedparam(&att_c, &c_sched);
					pthread_create(&tid_c, &att_c, cannon, &tpar_c);
				}
				
				pthread_mutex_lock(&mux);	
				
				//if not shoot, the projectile is positioned inside the cannon
				p_posx = c_pos + (cos(c_angle * PI / 180)) * 2;
				p_posy = 2 * (sin(c_angle * PI / 180));
				p_posz = c_posz;
				p_velx = (0.6 + c_powstp * (c_pow - 1)) * (cos(c_angle * PI / 180));
				p_vely = (0.6 + c_powstp * (c_pow - 1)) * (sin(c_angle * PI / 180));
				p_velz = 0;
				
				pthread_mutex_unlock(&mux);
			}
		}
		else
		{
			//to restart the game after win
			if(key[KEY_ENTER])
			{
				while(key[KEY_ENTER]){}
			
				pthread_mutex_lock(&mux);
				win = 0;
				score = 0;
				pthread_mutex_unlock(&mux);
			}
		}
		deadline_miss(arg);
		wait_for_period(arg);
	}	
}

/*render the scene*/
void *rendering(void *arg)
{
	int x, y;
	int srcX, srcY;
	char s[50];
	
	set_period(arg);
	
	// visual settings
	allegro_gl_clear_settings();
	allegro_gl_set(AGL_DOUBLEBUFFER, 1);
	allegro_gl_set(AGL_COLOR_DEPTH, 32);
	allegro_gl_set(AGL_Z_DEPTH, 16);
	allegro_gl_set(AGL_SUGGEST, AGL_COLOR_DEPTH | AGL_Z_DEPTH | AGL_DOUBLEBUFFER);
	set_color_depth(desktop_color_depth());
	
	//window creation
	set_window_title("Cannon - Cristian Perissutti");
	if(set_gfx_mode(GFX_OPENGL_WINDOWED, X_RES, Y_RES,0, 0) != 0) printf("graphic mode failed");
	
	allegro_gl_begin();
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	glClearColor(0.0f, 0.749f, 1.0f, 0.5f);
	glEnable (GL_DEPTH_TEST);
	allegro_gl_end();
	
	//running till user press ESC button
	while(!key[KEY_ESC])
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		if (win == 0)
		{
			//store graphic elements in buffer
			create_ground();
			create_wall(w_posx, w_h);
			create_target();
			create_cannon(c_angle, c_posz);
			create_projectile(p_posx, p_posy, p_posz);
			create_wvane();
			if(ez) trajectory();

			pthread_mutex_lock(&mux);
			
			//menù creation in allegro_mode
			
			allegro_gl_set_allegro_mode();
			rectfill(screen, 0, Y_RES, X_RES, Y_RES - M_Y, GOLD);
			
			//instructions and commands
			textout_ex(screen, font, "Spara con il cannone per colire i bersagli, il bersaglio rosso  vale 3 punti, quello blu 1, vittoria a 10 punti", 30, Y_RES - M_Y + 20, 0, -1);
			textout_ex(screen, font, "Usa i tasti Q W E A S D per muovere la videocamera", 30, Y_RES - M_Y + 50, 0, -1);
			textout_ex(screen, font, "Usa tasto sinistro + drag del mouse per ruotare la videocamera", 30, Y_RES - M_Y + 70, 0, -1);
			textout_ex(screen, font, "Usa le frecce dx e sx per muovere il cannone", 30, Y_RES - M_Y + 90, 0, -1);
			textout_ex(screen, font, "Usa le frecce su e giù per cambiare l'angolo del cannone", 30, Y_RES - M_Y + 110, 0, -1);
			textout_ex(screen, font, "Premi il tasto T per attivare o disattivare il mirino", 30, Y_RES - M_Y + 130, 0, -1);
			textout_ex(screen, font, "Premi il tasto V per attivare il vento", 30, Y_RES - M_Y + 150, 0, -1);
			textout_ex(screen, font, "Premi il tasto SPACE per sparare", 30, Y_RES - M_Y + 170, 0, -1);
			textout_ex(screen, font, "Usa i tasti N e M per cambiare la potenza del cannone", 30, Y_RES - M_Y + 190, 0, -1);
	
			//useful informations and score
			sprintf(s, "SCORE : %d", score);
			textout_ex(screen, font, s, 800, Y_RES - M_Y + 50, 14, -1);
			sprintf(s, "Potenza : %d", c_pow);
			textout_ex(screen, font, s, 800, Y_RES - M_Y + 70, 14, -1);
			sprintf(s, "Angolo : %d", (int)c_angle);
			textout_ex(screen, font, s, 800, Y_RES - M_Y + 90, 14, -1);
			if(wind)
			{
				textout_ex(screen, font, "vento attivo", 800, Y_RES - M_Y + 150, 14, -1);
			}
			else
			{
				textout_ex(screen, font, "vento non attivo", 800, Y_RES - M_Y + 150, 14, -1);
			}
			if(ez)
			{
				textout_ex(screen, font, "mirino attivo", 800, Y_RES - M_Y + 170, 14, -1);
			}
			else
			{
				textout_ex(screen, font, "mirino non attivo", 800, Y_RES - M_Y + 170, 14, -1);
			}
			//exit allegro mode
			allegro_gl_unset_allegro_mode();
		
			pthread_mutex_unlock(&mux);
		}
		else 
		{
		
			//victory screen in allegro mode
			allegro_gl_set_allegro_mode();
			rectfill(screen, 0, 0, X_RES, Y_RES, ORANGE);
			textout_centre_ex(screen, font, "VITTORIA", X_RES / 2, Y_RES / 2, 14, -1);
			textout_centre_ex(screen, font, "Premi <ENTER> per cominciare una nuova partita, premi <ESC> per uscire", X_RES / 2, Y_RES / 2 + 100, 14, -1);
			//exit allegro mode
			allegro_gl_unset_allegro_mode();
		} 
	
		//flip screen buffer -double buffering-
		glFlush();
		algl_draw_mouse();
		allegro_gl_flip();
		
		//change camera view
		adjcamera();
	
		deadline_miss(arg);
		wait_for_period(arg);
	}
	
}
	
/* Auxiliary functions for time management */
void time_copy(struct timespec *td, struct timespec ts) 
{
	td -> tv_sec = ts.tv_sec;
	td -> tv_nsec = ts.tv_nsec;
}

void time_add_ms(struct timespec *t, int ms) 
{
	t -> tv_sec += ms / 1000;
	t -> tv_nsec += (ms % 1000) * 1000000;
	if (t -> tv_nsec > 1000000000) 
	{
		t -> tv_nsec -= 1000000000;
		t -> tv_sec += 1;
	}
}

int time_cmp(struct timespec t1, struct timespec t2) 
{
	if (t1.tv_sec > t2.tv_sec) return 1;
	if (t1.tv_sec < t2.tv_sec) return -1;
	if (t1.tv_nsec > t2.tv_nsec) return 1;
	if (t1.tv_nsec < t2.tv_nsec) return -1;
	return 0;
}

void set_period(struct thread_param *tpar)
{
	struct timespec t;
	clock_gettime(CLOCK_MONOTONIC, &t);
	time_copy(&(tpar -> activ_time), t);
	time_copy(&(tpar -> abs_dl), t);
	time_add_ms(&(tpar -> activ_time), tpar -> period);
	time_add_ms(&(tpar -> abs_dl), tpar -> rel_dl);
}

void wait_for_period(struct thread_param *tpar) 
{
	clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &(tpar -> activ_time), NULL);
	time_add_ms(&(tpar -> activ_time), tpar->period);
	time_add_ms(&(tpar -> abs_dl), tpar->period);	/* Relative deadline is equal to period */
}

int deadline_miss(struct thread_param *tpar) 
{
	struct timespec now;
	clock_gettime(CLOCK_MONOTONIC, &now);
	if (time_cmp(now, tpar -> abs_dl) > 0) 
	{
		tpar -> dl_miss++;
		return 1;
	}
	return 0;
}

/*-----------------------------------------------------------------------------------

----------------------------------main function------------------------------------

-----------------------------------------------------------------------------------*/

int main(void)
{	
	//allegro and allegroGL initialization
	srand(time(NULL));
	if (allegro_init() != 0) 
	{
		printf("allegro initialization failed");
		allegro_exit();
	}

	if(install_allegro_gl() != 0)
	{ 
		printf("allegroGL initialization failed");
		return 1;
	}
	install_keyboard();
	install_mouse();
	install_timer();
	
	//variable initialization
	w_h = 5; 
	w_posx = 3; 				
	c_posz = 0;
	c_angle = 29;
	c_pow = 4;
	ez = 0;
	shoot = 0;
	wind = 0;
	tar_vec[0].dim = 1.5;
	tar_vec[1].dim = 0.7;
	win = 0;
	camera.posy = 5;
	camera.posz = 20;
	camera.posx = -5;
	
	//initialize random elements (wall position, height ecc...)
	rset();				

	/*target thread*/
	tpar_t.period = 20;
	tpar_t.rel_dl = tpar_t.period;
	tpar_t.dl_miss = 0;
	tpar_t.priority = 10;
	tpar_t.tar_n = 0;
	pthread_attr_init(&att_t);
	pthread_attr_setinheritsched(&att_t, PTHREAD_EXPLICIT_SCHED);
	pthread_attr_setschedpolicy(&att_t, SCHED_OTHER);
	t_sched.sched_priority = tpar_t.priority;
	pthread_attr_setschedparam(&att_t, &t_sched);
	pthread_create(&tid_t, &att_t, move_target, &tpar_t);
	
	/* second target thread*/							
	tpar_f.period = 20;
	tpar_f.rel_dl = tpar_t.period;
	tpar_f.dl_miss = 0;
	tpar_f.priority = 10;
	tpar_f.tar_n = 1;
	pthread_attr_init(&att_f);
	pthread_attr_setinheritsched(&att_f, PTHREAD_EXPLICIT_SCHED);
	pthread_attr_setschedpolicy(&att_f, SCHED_OTHER);
	f_sched.sched_priority = tpar_f.priority;
	pthread_attr_setschedparam(&att_f, &f_sched);
	pthread_create(&tid_f, &att_f, move_target, &tpar_f);				
	
	/*input thread*/
	tpar_in.period = 30;
	tpar_in.rel_dl = tpar_in.period;
	tpar_in.dl_miss = 0;
	tpar_in.priority = 20;
	pthread_attr_init(&att_in);
	pthread_attr_setinheritsched(&att_in, PTHREAD_EXPLICIT_SCHED);
	pthread_attr_setschedpolicy(&att_in, SCHED_OTHER);
	in_sched.sched_priority = tpar_in.priority;
	pthread_attr_setschedparam(&att_in, &in_sched);
	pthread_create(&tid_in, &att_in, input_ex, &tpar_in);
	
	/*rendering thread*/
	tpar_ren.period = 20;
	tpar_ren.rel_dl = tpar_ren.period;
	tpar_ren.dl_miss = 0;
	tpar_ren.priority = 25;
	pthread_attr_init(&att_ren);
	pthread_attr_setinheritsched(&att_ren, PTHREAD_EXPLICIT_SCHED);
	pthread_attr_setschedpolicy(&att_ren, SCHED_OTHER);
	ren_sched.sched_priority = tpar_ren.priority;
	pthread_attr_setschedparam(&att_ren, &ren_sched);
	pthread_create(&tid_ren, &att_ren, rendering, &tpar_ren);
	
	pthread_join(tid_ren, NULL);		
	return 0;
}

END_OF_MAIN();
