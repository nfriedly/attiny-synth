#include "Player.hpp"
#include "myrand.hpp"

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "tables.hpp"

using namespace myrand;

static int8_t synth_wf(uint32_t t)
{
    int16_t tmp = 0;
    tmp += int8_t(t);
    tmp += int8_t(t + (t >> 6));
    tmp += int8_t(t >> 1);
    tmp += int8_t((t >> 1) + (t >> 7));
    if (tmp < -128)
        return -128;
    else if (tmp > 127)
        return 127;
    return tmp;
}

#define BASE_PITCH 65536

uint16_t pitch_tab[12] = {
  BASE_PITCH/214, // c
  BASE_PITCH/202, // c#
  BASE_PITCH/190, // d 
  BASE_PITCH/180, // d#
  BASE_PITCH/170, // e
  BASE_PITCH/160, // f
  BASE_PITCH/151, // f#
  BASE_PITCH/143, // g
  BASE_PITCH/135, // g#
  BASE_PITCH/127, // a
  BASE_PITCH/120, // a#
  BASE_PITCH/113  // b
};

#define MD (1<<6) // drumpf
#define MS (1<<6) // synth
#define O0 (0<<4) // octave 0
#define O1 (1<<4) // octave 1
#define O2 (2<<4) // octave 1
#define O3 (3<<4) // octave 1
#define _C 0
#define CH 1
#define _D 2
#define _DH 3
#define _E 4
#define _F 5
#define FH 6
#define _G 7
#define GH 8
#define A 9
#define AH 10
#define _B 11

static int curr_note = 0;

typedef struct note {
 uint8_t machine_oct_note;
 uint8_t pos;
 uint8_t len;
} note;


static note song[] = { 
	{ MD| 0|O0,0,3 },
	{ MS|_C|O1,1,2 }
};


Player::Player()
{
}

#if 0
//#undef AMEN_01
//#define AMEN_01 KICK_VINYL02
//#undef AMEN_13
//#define AMEN_13 KICK_VINYL02
#endif

// 
#define BREAK_FREQUENCY 9
#define BREAK_OFFSET_MAGNITUDE (9+(rand()&0x3))
#define BREAK_SHUFFLE_AMOUNT 2

// Initial sequence used for break generation
int breaktab[] = {
    0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,
    16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
};

static int mul = 1<<12;
// Timing here is very inaccurate.. should slice the loop one more time
static void amen(Drumpf &drumpf, BassDrum &db, uint16_t pos)
{
    static int prev_breakstep = 0;
    int breaktabsize = sizeof(breaktab)/sizeof(int);
    int breakstep = (pos>>BREAK_FREQUENCY); 
    if ( prev_breakstep != breakstep )
    {
        // Shuffle sequence
        for ( int breaks = 0; breaks<BREAK_SHUFFLE_AMOUNT; breaks++)
        {
            int one = rand() % breaktabsize;
            int two = rand() % breaktabsize;
            int tmp = breaktab[one];
            breaktab[one] = breaktab[two];
            breaktab[two] = tmp;
        }
        // Break offset multiplier
        mul = 1<<BREAK_OFFSET_MAGNITUDE;
    }
    prev_breakstep = breakstep;

    // Only apply break during a specific timewindow part of the sequence
    if ( ((pos & 0xfff) > 0x07ff) ) 
    {
        pos += breaktab[breakstep%(sizeof(breaktab)/sizeof(int))]*mul;
    }

    switch (pos & 0x3FFF)
    {
    case 0x0060:
        drumpf.trigger(AMEN_01);
       // db.trigger();
        break;
    case 0x1040:
    case 0x2052:
        drumpf.trigger(AMEN_13);
      //  db.trigger();
        break;
    case 0x304a:
        drumpf.trigger(AMEN_35);
        //        db.trigger();
        break;

    case 0x315c:
        //db.trigger();
        drumpf.trigger(/*AMEN_36*/ /*AMEN_19*/ AMEN_05);
        break;

    case 0x022c:
        //db.trigger();
        drumpf.trigger(AMEN_02);
        break;
    case 0x123e:
    case 0x225a:
        //db.trigger();
        drumpf.trigger(/*AMEN_14*/ AMEN_13);
        break;
    case 0x3238:
        //db.trigger();
        drumpf.trigger(/*AMEN_37*/ AMEN_08);
        break;

    case 0x334c:
        //db.trigger();
        drumpf.trigger(/*AMEN_38*/ AMEN_09);
        break;

    case 0x0428:
    case 0x144e:
    case 0x2464:
        drumpf.trigger(AMEN_03);
        break;
    case 0x3426:
        drumpf.trigger(AMEN_39);
        break;

    case 0x064e:
    case 0x166e:
    case 0x2660:
        //db.trigger();
        drumpf.trigger(AMEN_04);
        break;
    case 0x363e:
        //db.trigger();
        drumpf.trigger(AMEN_40);
        break;

    case 0x0750:
    case 0x1770:
    case 0x276a:
        drumpf.trigger(AMEN_05);
        break;
    case 0x3748:
        //db.trigger();
        drumpf.trigger(/*AMEN_41*/ AMEN_23);
        break;

    case 0x084e:
    case 0x186a:
    case 0x285e:
        drumpf.trigger(/*AMEN_06*/ AMEN_04);
        break;
    case 0x3834:
        //db.trigger();
        drumpf.trigger(AMEN_42);
        break;

    case 0x0946:
    case 0x1950:
    case 0x295b:
        //db.trigger();
        drumpf.trigger(AMEN_05);
        break;
    case 0x3928:
        //db.trigger();
        drumpf.trigger(/*AMEN_43*/ AMEN_05);
        break;

    case 0x0a32:
    case 0x1a50:
        drumpf.trigger(AMEN_08);
        break;
    case 0x2a3a:
        drumpf.trigger(AMEN_13);
        break;
    case 0x3a0c:
        drumpf.trigger(AMEN_44);
        break;

    case 0x0b60:
    case 0x1b80:
        drumpf.trigger(AMEN_09);
        break;

    case 0x0c22:
    case 0x1c5a:
        drumpf.trigger(AMEN_10);
        break;
    case 0x2c50:
        drumpf.trigger(/*AMEN_16*/ AMEN_04);
        break;
    case 0x3c00:
        drumpf.trigger(AMEN_45);
        break;

    case 0x0e48:
    case 0x1e5e:
        drumpf.trigger(/*AMEN_11*/ AMEN_04);
        break;
    case 0x2e34:
        drumpf.trigger(/*AMEN_34*/ AMEN_15);
        break;
    case 0x3e0a:
        drumpf.trigger(AMEN_46);
        break;

    case 0x0f50:
    case 0x1f72:
        //db.trigger();
        drumpf.trigger(/*AMEN_12*/ AMEN_05);
        break;

    default:;
    }
}


void phaser_test(int pos,Buffer &db, Buffer &pb)
{
    static int phaser_entry_num = 0;
    static int prev_entered_phaser = 0;
    if ( ((pos & 0x3ff) > 0x7f) ) 
    {
        static int phaser_depth = 8;
        static int phaser_shift = 0;
        static int phaser_speed = 0;
        static int phaser_speed2 = 0;
        static int phaser_sign = 0;
        static int phaser_strength = 0;
        static int rprev = 0;
        int phaser_filter = 0;
        if ( prev_entered_phaser == 0 )
        {
            phaser_entry_num++;
            phaser_depth = (rand() & 0x7)+1;
            phaser_shift = 16-phaser_depth;
            phaser_speed = (rand() & 0x1f+1);
            phaser_speed2 = (rand() & 0x3f+1);
            phaser_sign =  ((rand()&0x7) > 3) ? -1 : 1;
            phaser_strength = (rand()&1)+1;
        }
        prev_entered_phaser = 1;
        static int t = 0;
        static int t2 = 0;

        int so = ((pgm_read_byte(&tables::sin[(t>>8)&0xff])*phaser_depth)>>8) - phaser_shift;
        phaser_filter = ((pgm_read_byte(&tables::sin[(t2>>8)&0xff])*128)>>8) + 256;
        if ( phaser_filter > 256) phaser_filter = 256;
        if ( phaser_filter < 0 ) phaser_filter = 0;
        for (int i = 0; i < globals::SAMPLES_PER_BUFFER; i++)
        {
            int offs = i+so;
            int a,b;
            a = db[i];
            if ( offs < 0 )
            {
                b = pb[globals::SAMPLES_PER_BUFFER+offs];
            } 
            else if ( offs >= globals::SAMPLES_PER_BUFFER) 
            {
                offs = (globals::SAMPLES_PER_BUFFER-1)-(offs-globals::SAMPLES_PER_BUFFER);
                b = db[offs];
            } else 
            {
                b = db[offs];
            }
            int r = 2*a-((a-b)>>phaser_strength)*phaser_sign;
            db[i] = rprev+(((r-rprev)*phaser_filter)>>8);
            rprev = db[i];
        }
        t += phaser_speed;
        t2 += phaser_speed2;
    } else {
        prev_entered_phaser = 0;
    }
}


void Player::render(Buffer &db, Buffer &pb)
{
	bool do_amen = true;
	
    myrand::rand32();

	if (do_amen) {
		// Trigger amen
	    amen(drumpf, bd, (pos<<1)+0x40);

		// Trigger hihat
		if (((pos+0x40) & 0x7F) == 0x40)
		{
			hh.trigger(0x60, 0xC0);
		}
		
	} else {
		// Trigger bassdrum
		if ((pos & 0x7FF) == 0)
		{
			bd.trigger();
			drumpf.trigger(KICK_808);
		}
	
		// Trigger snare
		if ((pos & 0x7FF) == 0x400)
		{
			drumpf.trigger(JK_SNR_03);
		}	

		// Trigger hihat
		if ((pos & 0x3FF) == 0x260)
		{
			hh.trigger(0x40, 0x18);
		}
	}
/*
    // trigger synth
    if (((pos & 0xFFF) == 0x400))
   {
        synth.trigger((uint16_t(myrand::rand8())) + 200);
    }
    else if (((pos & 0xFFF) == 0x680))
    {
        synth.release();
    }
*/

    // change oneliner settings
    if ((pos & 0x3FFF) == 0)
    {
        one_liner.set_time(0);
		do 
	        one_liner_sel = myrand::rand16() & 0x7;
		while (one_liner_sel > 5);
    }

    // mix
    db.clear();
    pre_compress.clear();

    bd.render(db);
    one_liner.render(pre_compress, one_liner_sel);
    hh.render(db);
    drumpf.render(db);
 
    phaser_test(pos, db, pb );
    compressor1.render(db, pre_compress);

//    pre_compress.clear();
    //one_liner.render(pre_compress, one_liner_sel);
//    compressor2.render(db, pre_compress);
    //synth.render(db, synth_wf);


    pos ++;
}
