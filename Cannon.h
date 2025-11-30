/*
 ============================================================================
 Project	: Cannon
 Author		: Perissutti Cristian 
 Description: Real-time application development project for the course of
 	 	 	 	 "Informatica e Sistemi in Tempo Reale"
 ============================================================================
*/

#include <allegro.h>
#include "alleggl.h"
#include <math.h>
#include <pthread.h>
#include <sched.h>
#include <time.h>
#include <stdio.h>

//Structure Definitions

struct cams {					//camera properties
	double xangle, yangle, zangle;
	double posx, posy, posz;
};

struct tar{					//target structure	
	float posx, posz;			//target position
	float spdx, spdz;			//target speed
	float dim;				//target dimension
};

struct thread_param{				
	int arg;
	long wcet;
	int period;
	int rel_dl;
	int priority;
	int dl_miss;
	int tar_n;
	struct timespec activ_time;
	struct timespec abs_dl;
};

//Function Prototype Definitions

//basic graphic
void cylinder(float r, float h);
void sphere(float rad, int lats, int longs);

//other functions
void create_ground(void);
void create_wall(float x_pos, float wall_h);
void create_target();
void create_cannon(float angles, float posiz);
void create_projectile(float xcen, float ycen, float zcen);
void create_wvane(void);
void rset();
void refresh_menu();
void trajectory();
void adjcamera (void);
int sign(float num);	

//thread functions
void *move_target(void *arg);
void *cannon(void *arg);
void *input_ex(void *arg);
void *rendering(void *arg);


//time management
void time_copy(struct timespec *td, struct timespec ts);
void time_add_ms(struct timespec *t, int ms);
int time_cmp(struct timespec t1, struct timespec t2);
void set_period(struct thread_param *tpar);
void wait_for_period(struct thread_param *tpar);
int deadline_miss(struct thread_param *tpar);

//Global Constant Definitions

// constants
#define	PI 		3.1415927 
#define g		0.0098
#define rho		1,225		// air density
#define Cd		0.47		// sphere drag coefficient

//colors
#define GOLD		0x56C1F6
#define ORANGE		0x00ACCC

#define X_RES		1280		// X resolution
#define Y_RES		720		// Y resolution
#define M_Y		200		// menu height

#define	g_x 		20		// playground dimension
#define g_z 		10

#define eth		1000		// ground dimension

#define cam_ang_spd	0.5		// camera angular and linear speed
#define cam_dist_spd	0.2

#define p_rad		0.22		// projectile radius
#define p_mass		100		// projectile mass 

#define t_spd		0.05		// target speed

#define	c_rad		0.7		// cannon radius
#define	c_len		6		// cannon lenght
#define c_pos		-19		// cannon position (x axis)
#define cp_stp		0.2		// cannon movement step
#define ca_stp		1		// cannon rotation step
#define c_maxp		10		// max cannon power
#define c_minp		1		// min cannon power
#define c_powstp	0.15		// cannon power increase step

#define w_width		0.5		// wall width
#define w_maxh		8

#define g_rad		0.2		// ghost radius

#define noise		1.5		// noise intensity

#define wvane_h		3		// windvane height
#define maxwind		0.2

#define max_points	10		//points to reach to win

//Global Variables Declaration

pthread_t 		tid_c;		// cannon thread
pthread_attr_t 		att_c;
struct sched_param 	c_sched;
struct thread_param 	tpar_c;
pthread_t 		tid_t;		// target thread
pthread_attr_t 		att_t;
struct sched_param 	t_sched;
struct thread_param 	tpar_t;
pthread_t 		tid_f;		// second target thread
pthread_attr_t 		att_f;
struct sched_param 	f_sched;
struct thread_param 	tpar_f;
pthread_t 		tid_in;		// input thread
pthread_attr_t 		att_in;
struct sched_param 	in_sched;
struct thread_param 	tpar_in;
pthread_t 		tid_ren;	// rendering thread
pthread_attr_t 		att_ren;
struct sched_param 	ren_sched;
struct thread_param 	tpar_ren;

float w_h, w_posx; 					// wall height and position
float c_posz, c_angle; 					// cannon position (z axis), angle
float p_posx, p_posy, p_posz, p_velx, p_vely, p_velz; 	// projectile coordinates and velocity components
float t_angle;						// target direction (angle)
float g_posx, g_posy, g_posz;				// ghost position
float n_x, n_z;						// ghost noise components
float wind_x, wind_z;					// wind components
int c_pow;						// cannon power level
int wind;						// wind activation flag
int score;						// current score
int shoot;						// show if a projectile is flying
int ez;							// ghost activation flag
int win;						// win flag

struct tar tar_vec[2];					//vector with structure of the two targets
struct cams camera;					

pthread_mutex_t mux = PTHREAD_MUTEX_INITIALIZER;	// graphic race condition handling
