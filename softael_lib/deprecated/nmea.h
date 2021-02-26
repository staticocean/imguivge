
#ifndef __NMEA_H__
#define __NMEA_H__

// -----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <math.h>

#include "types.h"

// -----------------------------------------------------------------------------

#define NMEA_GLL_PREFIX 	"GLL"
#define NMEA_HDT_PREFIX 	"HDT"
#define NMEA_P_PREFIX 		"P"

#define NMEA_USART_ID 		0

#define NMEA_SIGN(x) 		((x < 0) ? 0x2D : 0x2B) /* -/+ */
#define NMEA_SIGN_EW(x) 	((x < 0) ? 0x57 : 0x45) /* -/+ */

#define NMEA_PREFIX_SIZE 	32

// -----------------------------------------------------------------------------

static char NMEA_DONE[] = "DONE";

// -----------------------------------------------------------------------------

//typedef union { float32_t value; uint8_t data[4]; } nmea_float_t;
//typedef union { int32_t   value; uint8_t data[4]; } nmea_int_t 	;
//typedef union { uint32_t  value; uint8_t data[4]; } nmea_uint_t ;

// -----------------------------------------------------------------------------

typedef struct nmea_inst
{
	uint8_t bom_ackn;
	uint8_t msg_ackn;

    uint8_t data_in[256];
    uint8_t data_in_size;
    uint8_t data_in_offset;

} 	nmea_inst_t;

typedef struct nmea_config
{

}	nmea_config_t;

// -----------------------------------------------------------------------------

typedef struct nmea_header
{
	uint32_t 	field_count;

	uint32_t 	prefix_size;
	uint8_t		prefix_data[NMEA_PREFIX_SIZE];

} nmea_header_t;

// -----------------------------------------------------------------------------

typedef enum nmea_type 
{
	NMEA_TYPE_INT 	  = 0x01,
	NMEA_TYPE_INT32   = 0x02,
	NMEA_TYPE_INT64   = 0x03,
	NMEA_TYPE_UINT 	  = 0x04,
	NMEA_TYPE_UINT32  = 0x05,
	NMEA_TYPE_UINT64  = 0x06,
	NMEA_TYPE_CHAR 	  = 0x07,
	NMEA_TYPE_FLOAT   = 0x08,
	NMEA_TYPE_FLOAT32 = 0x09,
	NMEA_TYPE_FLOAT64 = 0x0A,
	NMEA_TYPE_TEXT 	  = 0x0B,
	NMEA_TYPE_NULL 	  = 0x0C,

} nmea_type_t;

// -----------------------------------------------------------------------------

typedef struct nmea_field_int
{
	int32_t 	size;
	
} nmea_field_int_t;

// -----------------------------------------------------------------------------

typedef struct nmea_field_int32
{
	int32_t 	size;
	
} nmea_field_int32_t;

// -----------------------------------------------------------------------------

typedef struct nmea_field_int64
{
	int32_t 	size;
	
} nmea_field_int64_t;

// -----------------------------------------------------------------------------

typedef struct nmea_field_uint
{
	int32_t 	size;
	
} nmea_field_uint_t;

// -----------------------------------------------------------------------------

typedef struct nmea_field_uint32
{
	int32_t 	size;
	
} nmea_field_uint32_t;

// -----------------------------------------------------------------------------

typedef struct nmea_field_uint64
{
	int32_t 	size;
	
} nmea_field_uint64_t;

// -----------------------------------------------------------------------------

typedef struct nmea_field_float
{
	int32_t 	integ;
	int32_t 	frac;
	
} nmea_field_float_t;

// -----------------------------------------------------------------------------

typedef struct nmea_field_float32
{
	int32_t 	integ;
	int32_t 	frac;
	
} nmea_field_float32_t;

// -----------------------------------------------------------------------------

typedef struct nmea_field_float64
{
	int32_t 	integ;
	int32_t 	frac;
	
} nmea_field_float64_t;

// -----------------------------------------------------------------------------

typedef struct nmea_field_char
{

} nmea_field_char_t;

// -----------------------------------------------------------------------------

typedef struct nmea_field_null
{
	int32_t 	frac;
	
} nmea_field_null_t;

// -----------------------------------------------------------------------------

typedef struct nmea_field_text
{
	uint8_t 	size;

} nmea_field_text_t;

// -----------------------------------------------------------------------------

typedef struct nmea_field
{	
	union
	{
		nmea_field_int_t 		__int__;
		nmea_field_int32_t 		__int32__;
		nmea_field_int64_t 		__int64__;
		nmea_field_uint_t		__uint__;
		nmea_field_uint32_t		__uint32__;
		nmea_field_uint64_t		__uint64__;
		nmea_field_char_t		__char__;
		nmea_field_float_t		__float__;
		nmea_field_float32_t	__float32__;
		nmea_field_float64_t	__float64__;
		nmea_field_text_t		__text__;
		nmea_field_null_t		__null__;
	};
	
	uint8_t 		null;
	nmea_type_t 	type;
	
} 	nmea_field_t;

// -----------------------------------------------------------------------------

typedef struct __attribute__((packed)) nmea_hdg_desc
{	
	nmea_header_t  	header;

	nmea_field_t 	heading;
	
	nmea_field_t 	deviation;
	nmea_field_t 	deviation_ew;
	
	nmea_field_t 	variation;
	nmea_field_t 	variation_ew;

} 	nmea_hdg_desc_t;

typedef struct __attribute__((packed)) nmea_hdg_data
{
	float32_t 	heading;

	float32_t 	deviation;
	uint8_t 	deviation_ew[4];
	
	float32_t 	variation;
	uint8_t 	variation_ew[4];
	
} 	nmea_hdg_data_t;

const nmea_hdg_desc_t nmea_hdg_desc = 
{
	.header = 
	{ .field_count = 5, .prefix_size = 5, .prefix_data = "xxHDG" },

	.heading = 
	{ .type = NMEA_TYPE_FLOAT, .null = 0, .__float__ = { .integ = 0, .frac = 0 } },

	.deviation = 
	{ .type = NMEA_TYPE_FLOAT, .null = 0, .__float__ = { .integ = 0, .frac = 0 } },

	.deviation_ew = 
	{ .type = NMEA_TYPE_CHAR , .null = 0, .__char__  = {  } },

	.variation = 
	{ .type = NMEA_TYPE_FLOAT, .null = 0, .__float__ = { .integ = 0, .frac = 0 } },

	.variation_ew = 
	{ .type = NMEA_TYPE_CHAR , .null = 0, .__char__  = {  } },
};


// -----------------------------------------------------------------------------

typedef struct __attribute__((packed)) nmea_hdm_desc
{
	nmea_header_t  	header;

	nmea_field_t 	heading_angle;
	nmea_field_t 	heading_type;

} 	nmea_hdm_desc_t;

typedef struct __attribute__((packed)) nmea_hdm_data
{
	float32_t 	heading_angle;
	uint8_t 	heading_type[4];

} 	nmea_hdm_data_t;

const nmea_hdm_desc_t nmea_hdm_desc = 
{
	.header = 
	{ .field_count = 2, .prefix_size = 5, .prefix_data = "xxHDM" },

	.heading_angle = 
	{ .type = NMEA_TYPE_FLOAT, .null = 0, .__float__ = { .integ = 0, .frac = 3 } },

	.heading_type = 
	{ .type = NMEA_TYPE_CHAR , .null = 0, .__char__  = {  } },
};

//------------------------------------------------------------------------------

typedef struct __attribute__((packed)) nmea_hdt_desc
{	
	nmea_header_t  	header;

	nmea_field_t 	heading_angle;
	nmea_field_t 	heading_type;

} 	nmea_hdt_desc_t;

typedef struct __attribute__((packed)) nmea_hdt_data
{
	float32_t 		heading_angle;
	uint8_t		 	heading_type[4];

} 	nmea_hdt_data_t;

const nmea_hdt_desc_t nmea_hdt_desc = 
{
	.header = 
	{ .field_count = 2, .prefix_size = 5, .prefix_data = "xxHDT" },

	.heading_angle = 
	{ .type = NMEA_TYPE_FLOAT, .null = 0, .__float__ = { .integ = 0, .frac = 0 } },

	.heading_type = 
	{ .type = NMEA_TYPE_CHAR , .null = 0, .__char__  = {  } },
};

//------------------------------------------------------------------------------
/*\
0      1          2  3  4    5  6  7
$GPZDA,172809.456,12,07,1996,00,00*45

0.	Message ID $GPZDA
1.	UTC
2.	Day, ranging between 01 and 31
3.	Month, ranging between 01 and 12
4.	Year
5.	Local time zone offset from GMT, ranging from 00 through �13 hours
6.	Local time zone offset from GMT, ranging from 00 through 59 minutes
7.	The checksum data, always begins with *
 */

typedef struct __attribute__((packed)) nmea_zda_desc
{	
	nmea_header_t  	header;

	nmea_field_t 	time;
	
	nmea_field_t 	day;
	nmea_field_t 	month;
	nmea_field_t 	year;
	
	nmea_field_t 	lzh;
	nmea_field_t 	lzm;
	
	nmea_field_t 	hh;

} 	nmea_zda_desc_t;

typedef struct __attribute__((packed)) nmea_zda_data
{
	float32_t 		time;
	
	uint32_t        day;
	uint32_t        month;
	uint32_t       	year;
	
	int32_t         lzh;
	int32_t         lzm;

} 	nmea_zda_data_t;

const nmea_zda_desc_t nmea_zda_desc = 
{
	.header = 
	{ .field_count = 6, .prefix_size = 5, .prefix_data = "xxZDA" },

    .time =
    { .type = NMEA_TYPE_FLOAT, .null = 0, .__float__ =
        { .integ = 6, .frac = 3}},
    
    .day =
    { .type = NMEA_TYPE_UINT, .null = 0, .__uint__  = 
    	{ .size = 2 }},

    .month =
    { .type = NMEA_TYPE_UINT, .null = 0, .__uint__  = 
    	{ .size = 2 }},

    .year =
    { .type = NMEA_TYPE_UINT, .null = 0, .__uint__  = 
    	{ .size = 4 }},

    .lzh =
    { .type = NMEA_TYPE_UINT, .null = 0, .__int__  = 
    	{ .size = 0 }},

    .lzm =
    { .type = NMEA_TYPE_UINT, .null = 0, .__int__  = 
    	{ .size = 0 }},
};

//------------------------------------------------------------------------------

/*
1      2         3      4   5    6      7     8      9      10     11
$PASHR,hhmmss.ss,HHH.HH,T,RRR.RR,PPP.PP,heave,rr.rrr,pp.ppp,hh.hhh,QF*CC<CR>
1.  hhmmss.ss - UTC time
2.  HHH.HH - Heading value in decimal degrees
3.  T - True heading (T displayed if heading is relative to true north)
4.  RRR.RR - Roll in decimal degrees (- sign will be displayed when applicable)
5.  PPP.PP - Pitch in decimal degrees (- sign will be displayed when applicable)
6.  heave - Heave, in meters
7.  rr.rrr - Roll standard deviation in decimal degrees
8.  pp.ppp - Pitch standard deviation in decimal degrees
9.  hh.hhh - Heading standard deviation in decimal degrees
10. QF - Quality Flag (0 = No position
                       1 = All non-RTK fixed integer positions
                       2 = RTK fixed integer position)
11. *CC - Checksum
12. <CR> - Carriage return
13. <LF> - Line feed
 */

typedef struct __attribute__((packed)) nmea_pashr_desc 
{
    nmea_header_t header;

    nmea_field_t time;

    nmea_field_t heading;
    nmea_field_t true_heading;
    nmea_field_t roll;
    nmea_field_t pitch;

    nmea_field_t heave;

    nmea_field_t roll_deviation;
    nmea_field_t pitch_deviation;
    nmea_field_t heading_deviation;
    
    nmea_field_t qf;
    nmea_field_t insf;

}	nmea_pashr_desc_t;

typedef struct __attribute__((packed)) nmea_pashr_data 
{
    float32_t time;

    float32_t heading;
    uint8_t   true_heading[4];
    float32_t roll;
    float32_t pitch;

    uint8_t   heave[4];

    float32_t roll_acc;
    float32_t pitch_acc;
    float32_t heading_acc;
    
    uint8_t   qf[4];
    uint8_t   insf[4];

} 	nmea_pashr_data_t;

const nmea_pashr_desc_t nmea_pashr_desc = 
{
    .header =
    { .field_count = 11, .prefix_size = 5, .prefix_data = "PASHR"},

    .time =
    { .type = NMEA_TYPE_FLOAT, .null = 0, .__float__ =
        { .integ = 6, .frac = 3}},

    .heading =
    { .type = NMEA_TYPE_FLOAT, .null = 0, .__float__ =
        { .integ = 0, .frac = 2}},

    .true_heading =
    { .type = NMEA_TYPE_CHAR , .null = 0, .__char__ =
        {}},

    .roll =
    { .type = NMEA_TYPE_FLOAT, .null = 0, .__float__ =
        { .integ = 0, .frac = 2}},

    .pitch =
    { .type = NMEA_TYPE_FLOAT, .null = 0, .__float__ =
        { .integ = 0, .frac = 2}},

    .heave =
    { .type = NMEA_TYPE_CHAR , .null = 0, .__char__ =
        {}},

    .roll_deviation =
    { .type = NMEA_TYPE_FLOAT, .null = 0, .__float__ =
        { .integ = 0, .frac = 3}},

    .pitch_deviation =
    { .type = NMEA_TYPE_FLOAT, .null = 0, .__float__ =
        { .integ = 0, .frac = 3}},

    .heading_deviation =
    { .type = NMEA_TYPE_FLOAT, .null = 0, .__float__ =
        { .integ = 0, .frac = 3}},

    .qf =
    { .type = NMEA_TYPE_CHAR , .null = 0, .__char__ =
        {}},
    
    .insf =
    { .type = NMEA_TYPE_CHAR , .null = 0, .__char__ =
        {}},
};

//------------------------------NMEA-PSTATE-------------------------------------

typedef struct __attribute__((packed)) nmea_pstate_desc 
{
    nmea_header_t header;

    nmea_field_t time;

    nmea_field_t state;
    nmea_field_t system;
    
    nmea_field_t sensor;
    nmea_field_t memory;
    nmea_field_t gnss;

    nmea_field_t qf;

} 	nmea_pstate_desc_t;

typedef struct __attribute__((packed)) nmea_pstate_data 
{
    float32_t   time;

    uint8_t     state[4];

    uint8_t     system[4];
    uint8_t     sensor[4];
    uint8_t     memory[4];
    uint8_t     gnss  [4];

}	nmea_pstate_data_t;

const nmea_pstate_desc_t nmea_pstate_desc = 
{
    .header = 
    { .field_count = 6, .prefix_size = 5, .prefix_data = "PSTATE"},

    .time   = 
    { .type = NMEA_TYPE_FLOAT, .null = 0, .__float__ = { .integ = 6, .frac = 3}},
    	
    .state  = 
    { .type = NMEA_TYPE_CHAR, .null = 0, .__char__ = {}},
    
    .system = 
    { .type = NMEA_TYPE_CHAR, .null = 0, .__char__ = {}},
    
    .sensor = 
    { .type = NMEA_TYPE_CHAR, .null = 0, .__char__ = {}},
    
    .memory = 
    { .type = NMEA_TYPE_CHAR, .null = 0, .__char__ = {}},
    
    .gnss   = 
    { .type = NMEA_TYPE_CHAR, .null = 0, .__char__ = {}},
};

//------------------------------------------------------------------------------

typedef struct __attribute__((packed)) nmea_gga_desc
{	
	nmea_header_t  	header;

	nmea_field_t 	time;

	nmea_field_t 	latitude;
	nmea_field_t 	ns;
	nmea_field_t 	longitude;
	nmea_field_t 	ew;

	nmea_field_t 	quality;
	nmea_field_t 	sat_num;

	nmea_field_t 	hdop;
	nmea_field_t 	msl;
	nmea_field_t 	uoa;
	nmea_field_t 	gsep;
	nmea_field_t 	uogsep;

	nmea_field_t 	dgps_age;
	nmea_field_t 	dgps_id;

} 	nmea_gga_desc_t;

typedef struct __attribute__((packed)) nmea_gga_data
{
	float64_t 		time;

	float64_t 		latitude;
	uint8_t 		ns[4];
	float64_t 		longitude;
	uint8_t 		ew[4];

	uint32_t		quality;
	uint32_t		sat_num;

	float64_t		hdop;
	float64_t		msl;
	uint8_t		 	uoa[4];
	float64_t		gsep;
	uint8_t		 	uogsep[4];

	uint32_t		dgps_age;
	uint32_t		dgps_id;

} 	nmea_gga_data_t;

const nmea_gga_desc_t nmea_gga_desc = 
{
	.header = 
    { .field_count = 14, .prefix_size = 5, .prefix_data = "GPGGA" },

	.time = 
	{ .type = NMEA_TYPE_FLOAT64, .null = 0, .__float64__ = { .integ = 6, .frac = 3 } },

	.latitude =
	{ .type = NMEA_TYPE_FLOAT64, .null = 0, .__float64__ = { .integ = 4, .frac = 4 } },
    
	.ns =
	{ .type = NMEA_TYPE_CHAR , .null = 0, .__char__  = {  } },
	
	.longitude =
	{ .type = NMEA_TYPE_FLOAT64, .null = 0, .__float64__ = { .integ = 5, .frac = 4 } },
	
	.ew = 
	{ .type = NMEA_TYPE_CHAR , .null = 0, .__char__  = {  } },
	
	.quality =
	{ .type = NMEA_TYPE_UINT , .null = 0, .__uint__  = { .size = 0 } },
	
	.sat_num =
	{ .type = NMEA_TYPE_UINT , .null = 0, .__uint__  = { .size = 0 } },
	
	.hdop =
	{ .type = NMEA_TYPE_FLOAT64, .null = 0, .__float64__ = { .integ = 0, .frac = 2 } },
	
	.msl =
	{ .type = NMEA_TYPE_FLOAT64, .null = 0, .__float64__ = { .integ = 0, .frac = 2 } },
	
	.uoa =
	{ .type = NMEA_TYPE_CHAR , .null = 0, .__char__  = {  } },
	
	.gsep =
	{ .type = NMEA_TYPE_FLOAT64, .null = 0, .__float64__ = { .integ = 0, .frac = 2 } },
	
	.uogsep =
	{ .type = NMEA_TYPE_CHAR , .null = 0, .__char__  = {  } },
	
	.dgps_age =
	{ .type = NMEA_TYPE_UINT , .null = 0, .__uint__  = { .size = 0 } },
	
	.dgps_id =
	{ .type = NMEA_TYPE_UINT , .null = 0, .__uint__  = { .size = 0 } },
};



typedef struct __attribute__((packed)) nmea_gsa_desc
{	
	nmea_header_t  	header;

	nmea_field_t 	time;

	nmea_field_t 	latitude;
	nmea_field_t 	ns;
	nmea_field_t 	longitude;
	nmea_field_t 	ew;

	nmea_field_t 	quality;
	nmea_field_t 	sat_num;

	nmea_field_t 	hdop;
	nmea_field_t 	msl;
	nmea_field_t 	uoa;
	nmea_field_t 	gsep;
	nmea_field_t 	uogsep;

	nmea_field_t 	dgps_age;
	nmea_field_t 	dgps_id;


} 	nmea_gsa_desc_t;

typedef struct __attribute__((packed)) nmea_gsa_data
{
	float32_t 		time;

	uint8_t 		switch_mode;
	uint32_t 		solution_mode;

	float64_t 		latitude;
	uint8_t 		ns;
	float64_t 		longitude;
	uint8_t 		ew;

	uint32_t		quality;
	uint32_t		sat_num;

	float32_t		hdop;
	float32_t		msl;
	uint8_t		 	uoa;
	float32_t		gsep;
	uint8_t		 	uogsep;

	float32_t		dgps_age;
	uint32_t		dgps_id;

} 	nmea_gsa_data_t;

const nmea_gsa_desc_t nmea_gsa_desc = 
{
	.header = 
    { .field_count = 14, .prefix_size = 5, .prefix_data = "xxGSA" },

	.time = 
	{ .type = NMEA_TYPE_FLOAT, .null = 0, .__float__ = { .integ = 6, .frac = 3 } },

	.latitude =
	{ .type = NMEA_TYPE_FLOAT, .null = 0, .__float64__ = { .integ = 0, .frac = 6 } },

	.ns =
	{ .type = NMEA_TYPE_CHAR , .null = 0, .__char__  = {  } },
	
	.longitude =
	{ .type = NMEA_TYPE_FLOAT, .null = 0, .__float64__ = { .integ = 0, .frac = 6 } },
	
	.ew = 
	{ .type = NMEA_TYPE_CHAR , .null = 0, .__char__  = {  } },
	
	.quality =
	{ .type = NMEA_TYPE_UINT , .null = 0, .__uint__  = { .size = 0 } },
	
	.sat_num =
	{ .type = NMEA_TYPE_UINT , .null = 0, .__uint__  = { .size = 0 } },
	
	.hdop =
	{ .type = NMEA_TYPE_FLOAT, .null = 0, .__float__ = { .integ = 0, .frac = 2 } },
	
	.msl =
	{ .type = NMEA_TYPE_FLOAT, .null = 0, .__float__ = { .integ = 0, .frac = 2 } },
	
	.uoa =
	{ .type = NMEA_TYPE_CHAR , .null = 0, .__char__  = {  } },
	
	.gsep =
	{ .type = NMEA_TYPE_FLOAT, .null = 0, .__float__ = { .integ = 0, .frac = 2 } },
	
	.uogsep =
	{ .type = NMEA_TYPE_CHAR , .null = 0, .__char__  = {  } },
	
	.dgps_age =
	{ .type = NMEA_TYPE_FLOAT, .null = 0, .__float__ = { .integ = 0, .frac = 2 } },
	
	.dgps_id =
	{ .type = NMEA_TYPE_UINT , .null = 0, .__uint__  = { .size = 0 } },
};

// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------

uint8_t hex2str[16] =
{
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
};

uint32_t __nmea_ld__(uint8_t *string_data, int32_t int32_data)
{
	uint32_t string_offset = 0x00;

	if (int32_data < 0x00)
	{
		string_data[string_offset] = '-';
		string_offset++;

		int32_data = -int32_data;
	}

	int32_t mask = 1;
	int32_t temp = int32_data;
	while (temp != 0x00) { temp = temp / 10; mask = mask * 10; }

	if (int32_data == 0x00)
	{
		string_data[string_offset] = '0';
		string_offset++;
	}

	while (mask > 0x01)
	{
		mask = mask / 10;
		string_data[string_offset] = 0x30 + (int32_data / mask) % 10;
		string_offset++;
	}

	return string_offset;
}

uint32_t __nmea_zp_ld__(uint8_t *string_data, uint32_t size, int32_t int32_data)
{
	uint32_t string_offset = 0x00;

	if (int32_data < 0x00)
	{
		string_data[string_offset] = '-';
		string_offset++;

		int32_data = -int32_data;
	}

	int32_t mask = 1;
	int32_t temp = int32_data;
	while (temp != 0x00) { temp = temp / 10; mask = mask * 10; }

	if (int32_data == 0x00)
	{
		string_data[string_offset] = '0';
		string_offset++;
	}

	while (mask > 0x01)
	{
		mask = mask / 10;
		string_data[string_offset] = 0x30 + (int32_data / mask) % 10;
		string_offset++;
	}

	while (string_offset < size)
	{
		uint32_t i;

		for (i = 0; i < string_offset; ++i)
		{
			string_data[string_offset-i] = string_data[string_offset-i-1];
		}

		string_data[0x00] = '0';
		string_offset++;
	}

	return string_offset;
}

uint32_t __nmea_lu__(uint8_t *string_data, uint32_t int32_data)
{
	uint32_t string_offset = 0x00;

	uint32_t mask = 1;
	uint32_t temp = int32_data;
	while (temp != 0x00) { temp = temp / 10; mask = mask * 10; }

	if (int32_data == 0x00)
	{
		string_data[string_offset] = '0';
		string_offset++;
	}

	while (mask > 0x01)
	{
		mask = mask / 10;
		string_data[string_offset] = 0x30 + (int32_data / mask) % 10;
		string_offset++;
	}

	return string_offset;
}

uint32_t __nmea_zp_lu__(uint8_t *string_data, uint32_t size, uint32_t int32_data)
{
	uint32_t string_offset = 0x00;

	uint32_t mask = 1;
	uint32_t temp = int32_data;
	while (temp != 0x00) { temp = temp / 10; mask = mask * 10; }

	if (int32_data == 0x00)
	{
		string_data[string_offset] = '0';
		string_offset++;
	}

	while (mask > 0x01)
	{
		mask = mask / 10;
		string_data[string_offset] = 0x30 + (int32_data / mask) % 10;
		string_offset++;
	}

	while (string_offset < size)
	{
		uint32_t i;

		for (i = 0; i < string_offset; ++i)
		{
			string_data[string_offset-i] = string_data[string_offset-i-1];
		}

		string_data[0x00] = '0';
		string_offset++;
	}

	return string_offset;
}


// -----------------------------------------------------------------------------

void nmea_init(nmea_inst_t *nmea_inst, nmea_config_t *nmea_config)
{
    nmea_inst->data_in_offset = 0;
    nmea_inst->data_in_size   = 0;
    nmea_inst->bom_ackn       = 0;
    nmea_inst->msg_ackn       = 0;

	return;
}

// -----------------------------------------------------------------------------

void nmea_checksum(uint8_t *data, uint8_t *checksum)
{
	int32_t i;

	// Zero out checksum -> first ex. or will be a copy of data[0]
	(*checksum) = 0x00;
 
	// Start from 2nd character of the message
	i = 1;

	while(data[i] != 0x2A) // != *
	{
		(*checksum) ^= data[i];
		i++;
	}

	return;
}

// -----------------------------------------------------------------------------

nmea_header_t* nmea_desc_get_header (const void *msg_descriptor)
{
	return ((nmea_header_t*) msg_descriptor);
}

nmea_field_t* nmea_desc_get_fields (const void *msg_descriptor)
{

	return (nmea_field_t*) ((uintptr_t) msg_descriptor
			+ (uintptr_t) sizeof(nmea_header_t));
}

// -----------------------------------------------------------------------------

void nmea_msg_render(const void *msg_descriptor, void *msg_data,
		uint32_t *string_size, uint8_t *string_data)
{
	uint32_t i;

	uint32_t		string_offset;
	uint8_t			checksum;

	int32_t 		int32_temp;
	uint32_t 		uint32_temp;
	uint8_t 		uint8_temp;
	uint32_t 		msg_offset;
	float32_t 		float32_temp;

	nmea_header_t 	*header = (nmea_header_t*) msg_descriptor;
	nmea_field_t 	*fields = (nmea_field_t*)  msg_descriptor;
    
    fields += sizeof(nmea_header_t);

	header = nmea_desc_get_header(msg_descriptor);
	fields = nmea_desc_get_fields(msg_descriptor);

	string_offset = 0x00;
	msg_offset = 0x00;

	string_offset += sprintf(&string_data[string_offset], "$%s,",
			header->prefix_data);

	for(i = 0; i < header->field_count; ++i)
	{
		if(fields[i].null == 0x00)
		{
			switch (fields[i].type)
			{
				case NMEA_TYPE_INT:
				{
					int32_temp = *((int32_t*) &((uint8_t*) msg_data)[msg_offset]);

					if (fields[i].__int__.size != 0x00)
					{
						string_offset += __nmea_zp_ld__(&string_data[string_offset], fields[i].__int__.size, int32_temp);
					}

					else
					{
						string_offset += __nmea_ld__(&string_data[string_offset], int32_temp);
					}

					break;
				}

				case NMEA_TYPE_UINT:
				{
					uint32_temp = *((uint32_t*) &((uint8_t*) msg_data)[msg_offset]);

					if (fields[i].__uint__.size != 0x00)
					{
						string_offset += __nmea_zp_lu__(&string_data[string_offset], fields[i].__uint__.size, uint32_temp);
					}

					else
					{
						string_offset += __nmea_lu__(&string_data[string_offset], uint32_temp);
					}

					break;
				}

				case NMEA_TYPE_CHAR:
				{
					uint8_temp = *((uint8_t*) &((uint8_t*) msg_data)[msg_offset]);
					string_data[string_offset] = uint8_temp;
					
					if (string_data[string_offset] < 0x30) string_data[string_offset] = 0x41;
					if (string_data[string_offset] > 0x7A) string_data[string_offset] = 0x7A;

					string_offset++;

					break;
				}

				case NMEA_TYPE_TEXT:
				{
                    uint32_t temp_index = 0x00;
                    
                    while (*((uint8_t*) &((uint8_t*) msg_data)[msg_offset+temp_index]) != 0x00 && temp_index < fields[i].__text__.size)
					{
                        uint8_temp = *((uint8_t*) &((uint8_t*) msg_data)[msg_offset+temp_index]);
                        string_data[string_offset] = uint8_temp;
                        
                        if (string_data[string_offset] < 0x41) string_data[string_offset] = 0x41;
                        if (string_data[string_offset] > 0x7A) string_data[string_offset] = 0x7A;

                        string_offset++;
                        temp_index++;
                    }

					break;
				}

				case NMEA_TYPE_FLOAT:
				{
					float32_temp = *((float32_t*) &((uint8_t*) msg_data)[msg_offset]);
					int32_temp = (int32_t) float32_temp;

					if (int32_temp == 0x00)
					{
						if (float32_temp < 0)
						{
							string_data[string_offset] = '-';
							string_offset ++;
						}
					}

					if (fields[i].__float__.integ != 0x00)
					{
						string_offset += __nmea_zp_ld__(&string_data[string_offset], fields[i].__float__.integ, int32_temp);
					}
					
					else
					{
						string_offset += __nmea_ld__(&string_data[string_offset], int32_temp);
					}

					string_data[string_offset] = '.';
					string_offset++;

					float32_temp = fabs(float32_temp) - abs(int32_temp);
					
					uint32_t temp_scale = 1;
					int32_t temp_frac = fields[i].__float__.frac;
					while (temp_frac > 0) { temp_frac--; temp_scale = temp_scale * 10; }

					uint32_temp = float32_temp * temp_scale;

					string_offset += __nmea_zp_lu__(&string_data[string_offset], fields[i].__float__.frac, uint32_temp);
					
					break;
				}

				case NMEA_TYPE_FLOAT64:
				{
					float64_t float64_temp;
					void *float64_temp_ptr = &float64_temp;
					uint8_t *float64_temp_uint8_ptr = (uint8_t*) float64_temp_ptr;
					uint8_t *msg_data_uint8_ptr = (uint8_t*) msg_data;
					
					float64_temp_uint8_ptr[0] = msg_data_uint8_ptr[msg_offset + 0x00];
					float64_temp_uint8_ptr[1] = msg_data_uint8_ptr[msg_offset + 0x01];
					float64_temp_uint8_ptr[2] = msg_data_uint8_ptr[msg_offset + 0x02];
					float64_temp_uint8_ptr[3] = msg_data_uint8_ptr[msg_offset + 0x03];
					float64_temp_uint8_ptr[4] = msg_data_uint8_ptr[msg_offset + 0x04];
					float64_temp_uint8_ptr[5] = msg_data_uint8_ptr[msg_offset + 0x05];
					float64_temp_uint8_ptr[6] = msg_data_uint8_ptr[msg_offset + 0x06];
					float64_temp_uint8_ptr[7] = msg_data_uint8_ptr[msg_offset + 0x07];

					int32_temp = (int32_t) float64_temp;

					if (int32_temp == 0x00)
					{
						if (float64_temp < 0)
						{
							string_data[string_offset] = '-';
							string_offset ++;
						}
					}

					if (fields[i].__float__.integ != 0x00)
					{
						string_offset += __nmea_zp_ld__(&string_data[string_offset], fields[i].__float__.integ, int32_temp);
					}
					
					else
					{
						string_offset += __nmea_ld__(&string_data[string_offset], int32_temp);
					}

					string_data[string_offset] = '.';
					string_offset++;

					float64_temp = float64_abs(float64_temp) - (float64_t) abs(int32_temp);
					
					uint32_t temp_scale = 1;
					int32_t temp_frac = fields[i].__float__.frac;
					while (temp_frac > 0) { temp_frac--; temp_scale = temp_scale * 10; }

					uint32_temp = (uint32_t) (float64_temp * temp_scale);

					string_offset += __nmea_zp_lu__(&string_data[string_offset], fields[i].__float__.frac, uint32_temp);
					
					break;
				}
			};
		}
		
		string_data[string_offset] = ',';
		string_offset++;

		switch (fields[i].type)
		{
			case NMEA_TYPE_INT    : msg_offset += 4; break;
			case NMEA_TYPE_UINT   : msg_offset += 4; break;
			case NMEA_TYPE_CHAR   : msg_offset += 4; break;
			case NMEA_TYPE_TEXT   : msg_offset += fields[i].__text__.size; break;
			case NMEA_TYPE_FLOAT  : msg_offset += 4; break;
			case NMEA_TYPE_FLOAT32: msg_offset += 4; break;
			case NMEA_TYPE_FLOAT64: msg_offset += 8; break;
		}
	}

	string_data[string_offset-1] = '*';

	nmea_checksum(string_data, &checksum);
	
	string_offset += sprintf(&string_data[string_offset], "%02X\r\n", checksum);
	
	*string_size = string_offset;

	return;
}

// -----------------------------------------------------------------------------

void nmea_input(nmea_inst_t *nmea_inst, uint8_t data_in)
{
	if (       nmea_inst->data_in_offset == 0
    		&& data_in == '$'
            && nmea_inst->bom_ackn == 0x00)
    {
        nmea_inst->bom_ackn = 0x01;
        nmea_inst->msg_ackn = 0x00;

        nmea_inst->data_in[nmea_inst->data_in_offset] = data_in;
        nmea_inst->data_in_offset++;

        // console_print("B");
    }

    else if (  nmea_inst->bom_ackn == 0x01
            && nmea_inst->data_in_offset > 0
            && nmea_inst->data_in_offset <= 254
            && data_in != '\n')
    {
        nmea_inst->data_in[nmea_inst->data_in_offset] = data_in;
        nmea_inst->data_in_offset++;
    }

    else if (  nmea_inst->data_in_offset > 7
    		&& nmea_inst->data_in_offset <= 254
            && nmea_inst->bom_ackn == 0x01
            && data_in == '\n')
    {
        if (   nmea_inst->data_in[nmea_inst->data_in_offset-1] == '\r'
            && nmea_inst->data_in[nmea_inst->data_in_offset-4] == '*')
        {
            nmea_inst->data_in_size = nmea_inst->data_in_offset+1;
            nmea_inst->msg_ackn = 0x01;
            
            // return i+1;
            // console_print("E!");
        }

        else
        {
            nmea_inst->data_in_size = 0x00;
            nmea_inst->msg_ackn = 0x00;

            // console_print("E?");
        }

        nmea_inst->bom_ackn = 0x00;
        nmea_inst->data_in_offset = 0x00;
    }

    else
    {
        nmea_inst->bom_ackn = 0x00;
       	nmea_inst->msg_ackn = 0x00;
        
        nmea_inst->data_in_offset = 0x00;
    }

    return;
}

// -----------------------------------------------------------------------------

uint8_t nmea_data_get_header(nmea_header_t *header,
		uint32_t string_size, uint8_t *string_data)
{
	uint32_t i;

	uint8_t return_code;
	uint8_t string_offset;
	uint8_t header_offset;

	return_code   = 0x00;
	header_offset = 0x00;
	string_offset = 0x01;

	while (header_offset < NMEA_PREFIX_SIZE
			&& string_data[string_offset] != ',')
	{
		header->prefix_data[header_offset] = string_data[string_offset];
		
		header_offset++;
		string_offset++;
	}

    header->prefix_data[header_offset] = '\0';
    header->prefix_size = header_offset;
    
	if (string_data[header_offset] != ',')
	{
		return_code = 0x01;
	}

	if (header_offset == 0)
	{
		return_code = 0x01;
	}

	return return_code;
}

void __nmea_data_get_text__(uint32_t string_size,
		uint8_t *string_data, uint8_t *text_data)
{
	uint32_t i;

	for (i = 0; i < string_size; ++i)
	{
		if (   string_data[i] >= 32
			&& string_data[i] <= 126)
		{
			text_data[i] = string_data[i];
		}
	}
    
    text_data[i] = '\0';

	return;
}

void __nmea_data_get_uint32__(uint32_t string_size,
		uint8_t *string_data, uint32_t *uint32_data)
{
	uint32_t i;

	*uint32_data = 0;
	
	for (i = 0; i < string_size; ++i)
	{
		if(    string_data[i] >= 48
			&& string_data[i] <= 57)
		{
			*uint32_data = (*uint32_data) * 10 + (string_data[i] - 48);
		}
	}

	return;
}


void __nmea_data_get_uint64__(uint32_t string_size,
		uint8_t *string_data, uint64_t *uint64_data)
{
	uint32_t i;

	*uint64_data = 0;
	
	for (i = 0; i < string_size; ++i)
	{
		if(    string_data[i] >= 48
			&& string_data[i] <= 57)
		{
			*uint64_data = (*uint64_data) * 10 + (string_data[i] - 48);
		}
	}

	return;
}

void __nmea_data_get_int32__(uint32_t string_size,
		uint8_t *string_data, int32_t *int32_data)
{
	uint32_t string_offset;
	int32_t  int32_sign;
	uint32_t uint32_temp;

	string_offset = 0;
	int32_sign = 1;

	if (string_data[0] == '+')
	{
		int32_sign = +1;
		string_offset++;
        string_size--;
	}

	else if (string_data[0] == '-')
	{
		int32_sign = -1;
		string_offset++;
        string_size--;
	}

	__nmea_data_get_uint32__(string_size, &string_data[string_offset],
			&uint32_temp);

	*int32_data = (int32_t) uint32_temp * int32_sign;

	return;
}

void __nmea_data_get_int64__(uint32_t string_size,
		uint8_t *string_data, int64_t *int64_data)
{
	uint32_t string_offset;
	int64_t  int64_sign;
	uint64_t uint64_temp;

	string_offset = 0;
	int64_sign = 1;

	if (string_data[0] == '+')
	{
		int64_sign = +1;
		string_offset++;
        string_size--;
	}

	else if (string_data[0] == '-')
	{
		int64_sign = -1;
		string_offset++;
        string_size--;
	}

	__nmea_data_get_uint64__(string_size, &string_data[string_offset],
			&uint64_temp);

	*int64_data = (int64_t) uint64_temp * int64_sign;

	return;
}

void __nmea_data_get_float32__(uint32_t string_size,
		uint8_t *string_data, float32_t *float32_data)
{
	uint32_t i;
    uint32_t point_offset;
    
	int32_t  int32_int;
	uint32_t uint32_frac;

	uint32_frac = 0;
	*float32_data = 0;
    point_offset = 0;

	for (i = 0; i < string_size; ++i)
    {
        if (string_data[i] == '.')
        {
            point_offset = i;
            break;
        }
    }
    
    __nmea_data_get_int32__(point_offset, &string_data[0], &int32_int);

	if (point_offset != 0)
	{
		__nmea_data_get_uint32__(string_size-point_offset-1,
				&string_data[point_offset+1], &uint32_frac);

		*float32_data = (float32_t) uint32_frac;
  
		for (i = 0; i < string_size-point_offset-1; ++i)
		{
			*float32_data = *float32_data / 10.0;
		}
	}
	
	*float32_data = *float32_data + (float32_t) int32_int;
	
	return;
}

// void __nmea_data_get_float64__(uint32_t string_size,
// 		uint8_t *string_data, float64_t *float64_data)
// {
// 	uint32_t i;
//     uint32_t point_offset;

// 	int32_t  int32_int;
// 	uint32_t uint32_frac;

// 	uint32_frac = 0;
//     point_offset = 0;

// 	*float64_data = 0;

// 	for (i = 0; i < string_size; ++i)
//     {
//         if (string_data[i] == '.')
//         {
//             point_offset = i;
//             break;
//         }
//     }

//     __nmea_data_get_int32__(point_offset, &string_data[0], &int32_int);

// 	if (point_offset != 0)
// 	{
// 		__nmea_data_get_uint32__(string_size-point_offset-1,
// 				&string_data[point_offset+1], &uint32_frac);

// 		*float64_data = (float64_t) uint32_frac;

// 		for (i = 0; i < string_size-point_offset-1; ++i)
// 		{
// 			*float64_data = *float64_data / (float64_t) 10.0;
// 		}
// 	}

// 	*float64_data = *float64_data + (float64_t) int32_int;

// 	return;
// }

void __nmea_data_get_float64__(uint32_t string_size,
		uint8_t *string_data, float64_t *float64_data)
{
	uint32_t i;
    uint32_t point_offset;
    uint32_t string_offset;
    
    float64_t float64_int  = 0.0;
    float64_t float64_frac = 0.0;
	float64_t float64_sign = 1.0;
	
	string_offset = 0;
	point_offset = 0;

	for (i = 0; i < string_size; ++i)
    {
        if (string_data[i] == '.')
        {
            point_offset = i;
            break;
        }
    }

	if (string_data[0] == '+')
	{
		float64_sign = +1;
		string_offset = 0x01;
	}

	else if (string_data[0] == '-')
	{
		float64_sign = -1;
		string_offset = 0x01;
	}

	for (i = string_offset; i < point_offset; ++i)
	{
		if(    string_data[i] >= 48
			&& string_data[i] <= 57)
		{
			float64_int = float64_int * 10.0;
			float64_int += (float64_t) (string_data[i] - 48);
		}
	}

	if (point_offset != 0)
	{
		for (i = point_offset+1; i < string_size; ++i)
		{
			if(    string_data[i] >= 48
				&& string_data[i] <= 57)
			{
				float64_frac = float64_frac * 10.0;
				float64_frac += (float64_t) (string_data[i] - 48);
			}
		}

		for (i = point_offset+1; i < string_size; ++i)
		{
			float64_frac = float64_frac / 10.0;
		}
	}

	*float64_data = float64_sign * (float64_int + float64_frac);
	
	return;
}

void nmea_msg_parse (const void *msg_descriptor, void *msg_ptr,
	uint32_t string_size, uint8_t *string_data)
{
	uint32_t i;

	uint8_t 		temp_data[8];

	nmea_header_t   *nmea_header;
	nmea_field_t    *nmea_fields;

	uint32_t 		string_offset;
	uint32_t 		msg_offset;
    
    uint8_t         *msg_data = msg_ptr;

	nmea_header = nmea_desc_get_header(msg_descriptor);
	nmea_fields = nmea_desc_get_fields(msg_descriptor);

	uint8_t field_size[nmea_header->field_count];

	string_offset = 2 + nmea_header->prefix_size;

	for (i = 0; i < nmea_header->field_count; ++i)
	{
		field_size[i] = 0;

		while (string_data[string_offset] != ','
			&& string_data[string_offset] != '*'
			&& string_offset < string_size)
		{
			field_size[i]++;
			string_offset++;
		}

		string_offset++;
	}

	string_offset = 2 + nmea_header->prefix_size;
	msg_offset    = 0;

	for (i = 0; i < nmea_header->field_count; ++i)
	{
		switch (nmea_fields[i].type)
		{
			case NMEA_TYPE_INT:
			{
				__nmea_data_get_int32__(field_size[i],
						&string_data[string_offset], (int32_t*) temp_data);

				msg_data[msg_offset  ] = temp_data[0];
				msg_data[msg_offset+1] = temp_data[1];
				msg_data[msg_offset+2] = temp_data[2];
				msg_data[msg_offset+3] = temp_data[3];

				break;
			}

			case NMEA_TYPE_UINT:
			{
				__nmea_data_get_uint32__(field_size[i],
						&string_data[string_offset], (uint32_t*) temp_data);

				msg_data[msg_offset  ] = temp_data[0];
				msg_data[msg_offset+1] = temp_data[1];
				msg_data[msg_offset+2] = temp_data[2];
				msg_data[msg_offset+3] = temp_data[3];

				break;
			}

			case NMEA_TYPE_CHAR:
			{
				msg_data[msg_offset] = string_data[string_offset];
				
				break;
			}

			case NMEA_TYPE_TEXT:
			{
				__nmea_data_get_text__(field_size[i],
						&string_data[string_offset],
						(uint8_t*) &msg_data[msg_offset]);

				break;
			}

			case NMEA_TYPE_FLOAT:
			{
				__nmea_data_get_float32__(field_size[i],
						&string_data[string_offset], (float32_t*) temp_data);

				msg_data[msg_offset  ] = temp_data[0];
				msg_data[msg_offset+1] = temp_data[1];
				msg_data[msg_offset+2] = temp_data[2];
				msg_data[msg_offset+3] = temp_data[3];

				break;
			}

			case NMEA_TYPE_FLOAT32:
			{
				__nmea_data_get_float32__(field_size[i],
						&string_data[string_offset], (float32_t*) temp_data);

				msg_data[msg_offset  ] = temp_data[0];
				msg_data[msg_offset+1] = temp_data[1];
				msg_data[msg_offset+2] = temp_data[2];
				msg_data[msg_offset+3] = temp_data[3];

				break;
			}

			case NMEA_TYPE_FLOAT64:
			{
				__nmea_data_get_float64__(field_size[i],
						&string_data[string_offset], (float64_t*) temp_data);

				msg_data[msg_offset  ] = temp_data[0];
				msg_data[msg_offset+1] = temp_data[1];
				msg_data[msg_offset+2] = temp_data[2];
				msg_data[msg_offset+3] = temp_data[3];
				msg_data[msg_offset+4] = temp_data[4];
				msg_data[msg_offset+5] = temp_data[5];
				msg_data[msg_offset+6] = temp_data[6];
				msg_data[msg_offset+7] = temp_data[7];

				break;
			}

			default:
			{
				break;
			}
		};

		string_offset += field_size[i]+1;
		
		switch (nmea_fields[i].type)
		{
			case NMEA_TYPE_INT  	: 	msg_offset += 4; 	break;
			case NMEA_TYPE_UINT	 	:	msg_offset += 4; 	break;
			case NMEA_TYPE_CHAR 	: 	msg_offset += 4; 	break;
			case NMEA_TYPE_TEXT 	:	msg_offset += nmea_fields[i].__text__.size; break;
			case NMEA_TYPE_FLOAT 	:	msg_offset += 4; 	break;
			case NMEA_TYPE_FLOAT32 	:	msg_offset += 4; 	break;
			case NMEA_TYPE_FLOAT64 	:	msg_offset += 8; 	break;
			default      	    	: 						break;
		};
	}

	return;
}

// -----------------------------------------------------------------------------

#endif /* __NMEA_H__ */






