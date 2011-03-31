#include "tracker.h"

#include <cwiid.h>

#define WIIMOTE_WIDTH 1014
#define WIIMOTE_HEIGHT 760      // TODO

#define TOGGLE_BIT(bf,b)        \
        (bf) = ((bf) & b)               \
                   ? ((bf) & ~(b))      \
                   : ((bf) | (b))
 
cwiid_mesg_callback_t callback;
cwiid_err_t err;

cwiid_wiimote_t* wiimote = NULL;

float dd_rotx = 0, dd_roty = 0, dd_rho=0;

void callback(cwiid_wiimote_t *wiimote, int mesg_count,
              union cwiid_mesg mesg[], struct timespec *timestamp)
{
	int i;
	for(i=0; i < mesg_count; i++)
	{
		switch (mesg[i].type) {
 
		case CWIID_MESG_BTN:
			if(mesg[i].btn_mesg.buttons & CWIID_BTN_LEFT)
				(dd_roty) -= 5;
			if(mesg[i].btn_mesg.buttons & CWIID_BTN_RIGHT)
				(dd_roty) += 5;
			if(mesg[i].btn_mesg.buttons & CWIID_BTN_UP)
				(dd_rho) += 1;
			if(mesg[i].btn_mesg.buttons & CWIID_BTN_DOWN)
				(dd_rho) -= 1;
			break;
		case CWIID_MESG_IR:
		{
			bool valid_source = bool(mesg[i].ir_mesg.src[0].valid);
			bool updated = false;
 
			if(valid_source)
			{
				int wiimote_pos_x = mesg[i].ir_mesg.src[0].pos[CWIID_X];
				int wiimote_pos_y = mesg[i].ir_mesg.src[0].pos[CWIID_Y];

				dd_rotx = -0.5f + float(wiimote_pos_x)/WIIMOTE_WIDTH;
			}
			break;
		}
		default:
			break;
		}
	}
}

void init_tracker()
{
	bdaddr_t addr;
	bacpy(&addr,BDADDR_ANY);

	wiimote = cwiid_connect(&addr,0);

	if(wiimote != NULL)
	{
		printf("Connected to %s\n",batostr(&addr));
		cwiid_set_mesg_callback(wiimote, callback);

		// Allumage LED 1
		unsigned char leds = 0x00;
		TOGGLE_BIT(leds, CWIID_LED1_ON);
		cwiid_set_led(wiimote, leds);

		// Activation du reporting de l'IR et des boutons
		unsigned char reportMode = 0x00;
		TOGGLE_BIT(reportMode, CWIID_RPT_IR);
		TOGGLE_BIT(reportMode, CWIID_RPT_BTN);
		cwiid_set_rpt_mode(wiimote, reportMode);

		// Activer la réception des messages (message callback et cwiid_get_mesg)
		cwiid_enable(wiimote, CWIID_FLAG_MESG_IFC);
	}
	else
	{
		printf("Connection failed\n");
	}
}

void tracker(GLfloat* d_rho, GLfloat* d_rotx, GLfloat* d_roty)
{
	*d_rho += dd_rho;
	*d_rotx += dd_rotx;
	*d_roty += dd_roty;

	dd_rho = 0;
	dd_rotx = 0;
	dd_roty = 0;
}

void close_tracker()
{
	if(wiimote != NULL)
	{
		cwiid_close(wiimote);
	}
}
